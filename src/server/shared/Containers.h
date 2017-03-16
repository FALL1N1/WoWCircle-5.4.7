/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "Define.h"
#include <list>
#include <algorithm>
#include <functional>
#include <vector>
#include <random>

//! Because circular includes are bad
extern uint32 urand(uint32 min, uint32 max);

namespace JadeCore
{
    namespace Containers
    {
        template<class T>
        void RandomResizeSet(std::set<T> &t_set, uint32 size)
        {
            size_t set_size = t_set.size();

            while (set_size > size)
            {
                typename std::set<T>::iterator itr = t_set.begin();
                std::advance(itr, urand(0, set_size - 1));
                t_set.erase(itr);
                --set_size;
            }
        }

        template<class T>
        void RandomResizeList(std::list<T> &list, uint32 size)
        {
            size_t list_size = list.size();

            while (list_size > size)
            {
                typename std::list<T>::iterator itr = list.begin();
                std::advance(itr, urand(0, list_size - 1));
                list.erase(itr);
                --list_size;
            }
        }

        template<class T, class Predicate>
        void RandomResizeList(std::list<T> &list, Predicate& predicate, uint32 size)
        {
            //! First use predicate filter
            std::list<T> listCopy;
            for (typename std::list<T>::iterator itr = list.begin(); itr != list.end(); ++itr)
                if (predicate(*itr))
                    listCopy.push_back(*itr);

            if (size)
                RandomResizeList(listCopy, size);

            list = listCopy;
        }

        /* Select a random element from a container. Note: make sure you explicitly empty check the container */
        template <class C> typename C::value_type const& SelectRandomContainerElement(C const& container)
        {
            typename C::const_iterator it = container.begin();
            std::advance(it, urand(0, container.size() - 1));
            return *it;
        }

        /*
        * Select a random element from a container where each element has a different chance to be selected.
        *
        * @param container Container to select an element from
        * @param weights Chances of each element to be selected, must be in the same order as elements in container
        *
        * Note: container cannot be empty
        */
        template <class C>
        typename C::const_iterator SelectRandomWeightedContainerElement(C const& container, std::vector<double> weights)
        {
            Trinity::discrete_distribution_param<uint32> ddParam(weights.begin(), weights.end());
            std::discrete_distribution<uint32> dd(ddParam);
            typename C::const_iterator it = container.begin();
            std::advance(it, dd(SFMTEngine::Instance()));
            return it;
        }

        /*
        * Select a random element from a container where each element has a different chance to be selected.
        *
        * @param container Container to select an element from
        * @param weightExtractor Function retrieving chance of each element in container, expected to take an element of the container and returning a double
        *
        * Note: container cannot be empty
        */
        template <class C, class Fn>
        typename C::const_iterator SelectRandomWeightedContainerElement(C const& container, Fn weightExtractor)
        {
            std::vector<double> weights;
            weights.reserve(container.size());
            std::transform(container.begin(), container.end(), std::back_inserter(weights), weightExtractor);
            return SelectRandomWeightedContainerElement(container, weights);
        }
    }
    //! namespace Containers
}
//! namespace JadeCore

#endif
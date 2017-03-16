/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef TRINITY_OBJECTACCESSOR_H
#define TRINITY_OBJECTACCESSOR_H

#include "Define.h"
#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>
#include "UnorderedMap.h"

#include "UpdateData.h"

#include "GridDefines.h"
#include "Object.h"
#include "Player.h"
#include "Transport.h"

#include <set>

class Creature;
class Corpse;
class Unit;
class GameObject;
class DynamicObject;
class WorldObject;
class Vehicle;
class Map;
class WorldRunnable;
class Transport;

template <class T>
class HashMapHolder
{
    public:
        static_assert(std::is_same<Player, T>::value
            || std::is_same<Transport, T>::value,
            "Only Player and Transport can be registered in global HashMapHolder");

        typedef UNORDERED_MAP<uint64, T*> MapType;
        typedef ACE_RW_Thread_Mutex LockType;

        static void Insert(T* o)
        {
            TRINITY_WRITE_GUARD(LockType, i_lock);
            m_objectMap[o->GetGUID()] = o;
        }

        static void Remove(T* o)
        {
            TRINITY_WRITE_GUARD(LockType, i_lock);
            m_objectMap.erase(o->GetGUID());
        }

        static T* Find(uint64 guid)
        {
            TRINITY_READ_GUARD(LockType, i_lock);
            typename MapType::iterator itr = m_objectMap.find(guid);
            return (itr != m_objectMap.end()) ? itr->second : NULL;
        }

        static MapType& GetContainer() { return m_objectMap; }

        static LockType* GetLock() { return &i_lock; }

    private:

        //Non instanceable only static
        HashMapHolder() {}

        static LockType i_lock;
        static MapType  m_objectMap;
};

class ObjectAccessor
{
    friend class ACE_Singleton<ObjectAccessor, ACE_Null_Mutex>;
    private:
        ObjectAccessor();
        ~ObjectAccessor();
        ObjectAccessor(const ObjectAccessor&);
        ObjectAccessor& operator=(const ObjectAccessor&);

    public:
        // TODO: override these template functions for each holder type and add assertions

        // these functions return objects only if in map of specified object
        static WorldObject* GetWorldObject(WorldObject const&, uint64);
        static Object* GetObjectByTypeMask(WorldObject const&, uint64, uint32 typemask);
        static Corpse* GetCorpse(WorldObject const& u, uint64 guid);
        static GameObject* GetGameObject(WorldObject const& u, uint64 guid);
        static DynamicObject* GetDynamicObject(WorldObject const& u, uint64 guid);
        static AreaTrigger* GetAreaTrigger(WorldObject const& u, uint64 guid);
        static Unit* GetUnit(WorldObject const&, uint64 guid);
        static Creature* GetCreature(WorldObject const& u, uint64 guid);
        static Pet* GetPet(WorldObject const&, uint64 guid);
        static Player* GetPlayer(Map const*, uint64 guid);
        static Player* GetPlayer(WorldObject const&, uint64 guid);
        static Creature* GetCreatureOrPetOrVehicle(WorldObject const&, uint64);
        static Transport* GetTransport(WorldObject const& u, uint64 guid);

        // these functions return objects if found in whole world
        // ACCESS LIKE THAT IS NOT THREAD SAFE
        static Player* FindPlayer(uint64);
        static Player* FindPlayerInOrOutOfWorld(uint64);
        static Player* FindPlayerByName(const char* name);
        static Player* FindPlayerByNameInOrOutOfWorld(const char* name);

        // when using this, you must use the hashmapholder's lock
        static HashMapHolder<Player>::MapType const& GetPlayers()
        {
            return HashMapHolder<Player>::GetContainer();
        }

        static void AddObject(Player* object)
        {
            if (object->GetGUIDLow() < k_PlayerCacheMaxGuid)
                m_PlayersCache[object->GetGUIDLow()] = object;

            HashMapHolder<Player>::Insert(object);
        }

        template<class T> static void AddObject(T* object)
        {
            HashMapHolder<T>::Insert(object);
        }

        static void RemoveObject(Player* object)
        {
            if (object->GetGUIDLow() < k_PlayerCacheMaxGuid)
                m_PlayersCache[object->GetGUIDLow()] = nullptr;

            HashMapHolder<Player>::Remove(object);
        }

        template<class T> static void RemoveObject(T* object)
        {
            HashMapHolder<T>::Remove(object);
        }

        static void SaveAllPlayers();

    private:
        static uint32 k_PlayerCacheMaxGuid;
        static Player** m_PlayersCache;
};

#define sObjectAccessor ACE_Singleton<ObjectAccessor, ACE_Null_Mutex>::instance()
#endif

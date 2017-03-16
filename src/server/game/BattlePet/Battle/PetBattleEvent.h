#pragma once

#include "Define.h"
#include "BattlePetDefines.h"

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

namespace BattlePet
{
    struct Event
    {
        struct EventValue
        {
            union
            {
                struct // SET_HEALTH
                {
                    int32 m_health;
                } health;

                struct // SET_MAX_HEALTH
                {
                    int32 m_health;
                } maxHealth;

                struct // TRIGGER_ABILTY
                {
                    int32 m_abilityId;
                } trigger;

                struct // SET_SPEED
                {
                    int32 m_speed;
                } speed;

                struct // STATUS_CHANGE
                {
                    int32 m_status;
                } status;

                struct // SET_STATE
                {
                    int32 m_stateId;
                    int32 m_value;
                } state;

                struct // AURA_APPLY, AURA_CHANGE, AURA_CANCEL
                {
                    int32 m_abilityId;
                    int32 m_amount;
                    int32 m_currentRound;
                    int32 m_remaingCooldown;
                } aura;

                struct
                {
                    int32 m_raw0;
                    int32 m_raw1;
                    int32 m_raw2;
                    int32 m_raw3;
                } raw;

                // Emptry blocks (unkType = 6) for AURAS_BEGIN and AURAS_END
            };

            PBOID m_target;

            EventValue(PBOID target, int32 raw0 = 0, int32 raw1 = 0, int32 raw2 = 0, int32 raw3 = 0)
                : m_target(target)
            {
                raw.m_raw0 = raw0;
                raw.m_raw1 = raw1;
                raw.m_raw2 = raw2;
                raw.m_raw3 = raw3;
            }
        };

        struct EventInfo
        {
            /*
                //1 - normal damage
                //2 - miss
                //4 - crit damage
                //8 - miss
                //16 - block
                //32 - block
                //64 - reflect
                //128 - block
                //256 - dodge
                //512 - dodge
                //1024 - dodge
                //2048 - dodge

                //4096 - block

                //8192 - block
                //16384 - critical block*/
            uint16 m_flag; // miss, crit, dodge, block and other
            uint16 m_abilityEffectId;
            PBOID m_provider;
            PetBattleEvents m_event;

            EventInfo()
                : m_flag(0)
                , m_abilityEffectId(0)
                , m_provider(PBOID::P0_PET_0)
                , m_event(PetBattleEvents::SET_HEALTH)
            {

            }

            EventInfo(PetBattleEvents _event, PBOID provider, uint16 effectId, uint16 flag)
                : m_flag(flag)
                , m_abilityEffectId(effectId)
                , m_provider(provider)
                , m_event(_event)
            {

            }

            uint8 GetType() const
            {
                switch (m_event)
                {
                    case PetBattleEvents::SET_HEALTH:
                        return 5;
                    case PetBattleEvents::AURA_APPLY:
                    case PetBattleEvents::AURA_CANCEL:
                    case PetBattleEvents::AURA_CHANGE:
                        return 1;
                    case PetBattleEvents::PET_SWAP:
                    case PetBattleEvents::AURAS_BEGIN:
                    case PetBattleEvents::AURAS_END:
                        return 6;
                    case PetBattleEvents::STATUS_CHANGE:
                    case PetBattleEvents::SET_SPEED:
                    case PetBattleEvents::SET_MAX_HEALTH:
                        return 4;
                    case PetBattleEvents::TRIGGER_ABILTY:
                        return 0;
                    case PetBattleEvents::SET_STATE:
                        return 3;
                    default:
                        return 9; // TODO: found correct value
                }
            }
        };

        typedef std::vector<EventValue> vValues;

        Event(EventInfo info, EventValue value)
            : m_info(info)
        {
            m_values.push_back(value);
        }

        Event(EventInfo info)
            : m_info(info)
        {
        }

        EventInfo m_info;
        vValues m_values;

        EventValue Create(PBOID target, int32 raw0 = 0, int32 raw1 = 0, int32 raw2 = 0, int32 raw3 = 0);
        void Add(EventValue value);
    };

    typedef std::vector<Event const*> Events;

    class EventHolder
    {
    public:
        EventHolder();
        ~EventHolder();

        Event* Make(Event::EventInfo info);
        Event* Make(Event::EventInfo info, Event::EventValue value);

        Events const& GetEvents() { return m_events; }

        void Clear();

    private:
        void AddEvent(Event const* _event);

        Events m_events;
    };
}
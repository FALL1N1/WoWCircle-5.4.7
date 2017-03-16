//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "siege_of_the_niuzoa_temple.hpp"

DoorData const g_DoorData[] =
{
    { GO_HARDENED_RESIN,        DATA_JINBAK,    DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GO_DOOR,                  DATA_VOJAK,     DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GO_INVISIBLE_FIRE_WALL,   DATA_VOJAK,     DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { 0,                        0,              DOOR_TYPE_ROOM,     BOUNDARY_NONE }  // END
};

#define MAX_ENCOUNTER 4

static BossScenarios const g_BossScenarios[] =
{
    { DataTypes::DATA_JINBAK,   eScenarioDatas::Jinbak  },
    { DataTypes::DATA_VOJAK,    eScenarioDatas::Vojak   },
    { DataTypes::DATA_PAVALAK,  eScenarioDatas::Pavalak },
    { DataTypes::DATA_NERONOK,  eScenarioDatas::Neronok },
    { 0,                        0                       }
};

class instance_siege_of_the_niuzao_temple : public InstanceMapScript
{
    public:
        instance_siege_of_the_niuzao_temple() : InstanceMapScript("instance_siege_of_the_niuzao_temple", 1011) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_siege_of_the_niuzao_temple_InstanceMapScript(map);
        }

        struct instance_siege_of_the_niuzao_temple_InstanceMapScript : public InstanceScript
        {
            uint64 jinbakGuid;
            uint64 vojakGuid;
            uint64 pavalakGuid;
            uint64 neronokGuid;

            uint32 m_CreatureKilled;

            instance_siege_of_the_niuzao_temple_InstanceMapScript(Map* map) : InstanceScript(map) { }

            void Initialize()
            {
                jinbakGuid = 0;
                vojakGuid = 0;
                pavalakGuid = 0;
                neronokGuid = 0;

                SetBossNumber(MAX_ENCOUNTER);

                m_CreatureKilled = 0;

                if (instance->IsChallengeMode())
                    LoadScenariosInfos(g_BossScenarios, eScenarioDatas::ScenarioID);

                LoadDoorData(g_DoorData);
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_JINBAK:
                        jinbakGuid = creature->GetGUID();
                        break;
                    case NPC_VOJAK:
                        vojakGuid = creature->GetGUID();
                        break;
                    case NPC_PAVALAK:
                        pavalakGuid = creature->GetGUID();
                        break;
                    case NPC_NERONOK:
                        neronokGuid = creature->GetGUID();
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case GO_HARDENED_RESIN:
                    case GO_DOOR:
                    case GO_INVISIBLE_FIRE_WALL:
                        AddDoor(p_GameObject, true);
                        break;
                    case GameobjectIds::ChallengeModeDoor:
                        m_ChallengeDoorGuid = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case GO_HARDENED_RESIN:
                    case GO_DOOR:
                    case GO_INVISIBLE_FIRE_WALL:
                        AddDoor(p_GameObject, false);
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureKilled(Creature* p_Creature, Player* p_Player) override
            {
                if (!instance->IsChallengeMode() || !IsChallengeModeStarted() || m_CreatureKilled >= eScenarioDatas::KillCount)
                    return;

                if (p_Creature == nullptr)
                    return;

                /// Ennemies already killed
                if (m_ConditionCompleted)
                    return;

                if (!p_Creature->isElite() || p_Creature->IsDungeonBoss())
                    return;

                if (m_CreatureKilled >= eScenarioDatas::KillCount)
                    return;

                ++m_CreatureKilled;
                SendScenarioProgressUpdate(CriteriaProgressData(eScenarioDatas::Ennemies, m_CreatureKilled, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

                if (m_CreatureKilled >= eScenarioDatas::KillCount)
                    m_ConditionCompleted = true;
            }

            uint64 GetData64(uint32 id) const
            {
                switch (id)
                {
                    case DATA_JINBAK:
                        return jinbakGuid;
                    case DATA_VOJAK:
                        return vojakGuid;
                    case DATA_PAVALAK:
                        return pavalakGuid;
                    case DATA_NERONOK:
                        return neronokGuid;
                }

                return 0;
            }

            void Update(uint32 p_Diff) override
            {
                ScheduleBeginningTimeUpdate(p_Diff);
                ScheduleChallengeStartup(p_Diff);
                ScheduleChallengeTimeUpdate(p_Diff);
            }
        };
};

void AddSC_instance_siege_of_the_niuzoa_temple()
{
    new instance_siege_of_the_niuzao_temple();
}
//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "shadopan_monastery.hpp"

static BossScenarios const g_BossScenarios[] =
{
    { DataTypes::DATA_GU_CLOUDSTRIKE,   eScenarioDatas::Cloudstrike },
    { DataTypes::DATA_MASTER_SNOWDRIFT, eScenarioDatas::Snowdrift   },
    { DataTypes::DATA_SHA_VIOLENCE,     eScenarioDatas::ViolenceSha },
    { DataTypes::DATA_TARAN_ZHU,        eScenarioDatas::TaranZhu    },
    { 0,                                0                           }
};

Position snowdriftCenterPos = {3659.08f, 3015.38f, 804.74f};

class instance_shadopan_monastery : public InstanceMapScript
{
    public:
        instance_shadopan_monastery() : InstanceMapScript("instance_shadopan_monastery", 959) { }

        struct instance_shadopan_monastery_InstanceMapScript : public InstanceScript
        {
            uint8 aliveNoviceCount;
            uint8 aliveMinibossCount;

            uint64 guCloudstikeGuid;
            uint64 masterSnowdriftGuid;
            uint64 shaViolenceGuid;
            uint64 taranZhuGuid;

            uint64 azureSerpentGuid;

            uint64 cloudstikeEntranceGuid;
            uint64 cloudstikeExitGuid;
            uint64 snowdriftEntranceGuid;
            uint64 snowdriftPossessionsGuid;
            uint64 snowdriftFirewallGuid;
            uint64 snowdriftDojoDoorGuid;
            uint64 snowdriftExitGuid;
        
            uint64 shaEntranceGuid;
            uint64 shaExitGuid;
        
            std::list<uint64> minibossPositionsGuid;
            std::list<uint64> minibossPositionsGuidSave;

            std::list<uint64> firstDefeatedNovicePositionsGuid;
            std::list<uint64> firstDefeatedNovicePositionsGuidSave;

            std::list<uint64> secondDefeatedNovicePositionsGuid;
            std::list<uint64> secondDefeatedNovicePositionsGuidSave;
        
            std::list<uint64> firstArcherySet;
            std::list<uint64> secondArcherySet;
            std::list<uint64> archeryTargetGuids;

            uint32 dataStorage[MAX_DATA];

            uint32 m_CreatureKilled;
            uint32 m_PurifiedDefenders;

            instance_shadopan_monastery_InstanceMapScript(Map* map) : InstanceScript(map) { }

            void Initialize()
            {
                SetBossNumber(EncounterCount);

                aliveNoviceCount            = MAX_NOVICE;
                aliveMinibossCount          = 2;

                guCloudstikeGuid            = 0;
                masterSnowdriftGuid         = 0;
                shaViolenceGuid             = 0;
                taranZhuGuid                = 0;

                azureSerpentGuid            = 0;

                cloudstikeEntranceGuid      = 0;
                cloudstikeExitGuid          = 0;
                snowdriftEntranceGuid       = 0;
                snowdriftPossessionsGuid    = 0;
                snowdriftFirewallGuid       = 0;
                snowdriftDojoDoorGuid       = 0;
                snowdriftExitGuid           = 0;

                shaEntranceGuid             = 0;
                shaExitGuid                 = 0;
            
                firstArcherySet.clear();
                secondArcherySet.clear();

                memset(dataStorage, 0, MAX_DATA * sizeof(uint32));

                m_CreatureKilled = 0;
                m_PurifiedDefenders = 0;

                if (instance->IsChallengeMode())
                    LoadScenariosInfos(g_BossScenarios, eScenarioDatas::ScenarioID);
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

                if (m_CreatureKilled >= eScenarioDatas::KillCount && m_PurifiedDefenders >= eScenarioDatas::PurifyCount)
                    m_ConditionCompleted = true;
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_GU_CLOUDSTRIKE:    guCloudstikeGuid    = creature->GetGUID();          return;
                    case NPC_MASTER_SNOWDRIFT:  masterSnowdriftGuid = creature->GetGUID();          return;
                    case NPC_SHA_VIOLENCE:      shaViolenceGuid     = creature->GetGUID();          return;
                    case NPC_TARAN_ZHU:         taranZhuGuid        = creature->GetGUID();          return;
                    case NPC_AZURE_SERPENT:     azureSerpentGuid    = creature->GetGUID();          return;
                    case NPC_ARCHERY_TARGET:    archeryTargetGuids.push_back(creature->GetGUID());  return;
                    case NPC_SNOWDRIFT_POSITION:
                    {
                        uint32 guid = creature->GetSpawnId();

                        if (creature->GetDistance(snowdriftCenterPos) > 5.0f && creature->GetDistance(snowdriftCenterPos) < 14.0f)
                        {
                            minibossPositionsGuid.push_back(creature->GetGUID());
                            minibossPositionsGuidSave.push_back(creature->GetGUID());
                        }
                        else if (creature->GetDistance(snowdriftCenterPos) >= 14.0f  && creature->GetDistance(snowdriftCenterPos) < 18.0f)
                        {
                            firstDefeatedNovicePositionsGuid.push_back(creature->GetGUID());
                            firstDefeatedNovicePositionsGuidSave.push_back(creature->GetGUID());
                        }
                        else if (creature->GetDistance(snowdriftCenterPos) >= 18.0f && creature->GetDistance(snowdriftCenterPos) < 25.0f)
                        {
                            secondDefeatedNovicePositionsGuid.push_back(creature->GetGUID());
                            secondDefeatedNovicePositionsGuidSave.push_back(creature->GetGUID());
                        }
                        break;
                    }
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject)
            {
                switch (p_GameObject->GetEntry())
                {
                    case GO_CLOUDSTRIKE_ENTRANCE:
                        cloudstikeEntranceGuid = p_GameObject->GetGUID();
                        HandleGameObject(0, true, p_GameObject);
                        break;
                    case GO_CLOUDSTRIKE_EXIT:
                        cloudstikeExitGuid = p_GameObject->GetGUID();
                        break;
                    case GO_SNOWDRIFT_ENTRANCE:
                        snowdriftEntranceGuid = p_GameObject->GetGUID();
                        HandleGameObject(0, true, p_GameObject);
                        break;
                    case GO_SNOWDRIFT_POSSESSIONS:
                        p_GameObject->SetPhaseMask(2, true);
                        snowdriftPossessionsGuid = p_GameObject->GetGUID();
                        break;
                    case GO_SNOWDRIFT_FIRE_WALL:
                        snowdriftFirewallGuid = p_GameObject->GetGUID();
                        break;
                    case GO_SNOWDRIFT_DOJO_DOOR:
                        snowdriftDojoDoorGuid = p_GameObject->GetGUID();
                        break;
                    case GO_SNOWDRIFT_EXIT:
                        snowdriftExitGuid = p_GameObject->GetGUID();
                        break;
                    case GO_SHA_ENTRANCE:
                        shaEntranceGuid = p_GameObject->GetGUID();
                        HandleGameObject(0, true, p_GameObject);
                        break;
                    case GO_SHA_EXIT:
                        shaExitGuid = p_GameObject->GetGUID();
                        break;
                    case ObjectsIds::ChallengeModeDoor:
                        m_ChallengeDoorGuid = p_GameObject->GetGUID();
                        break;
                    default:
                        return;
                }
            }

            bool SetBossState(uint32 id, EncounterState state)
            {
                if (!InstanceScript::SetBossState(id, state))
                    return false;

                switch (id)
                {
                    case DATA_GU_CLOUDSTRIKE:
                    {
                        switch (state)
                        {
                            case NOT_STARTED:
                            case FAIL:
                            {
                                HandleGameObject(cloudstikeEntranceGuid, true);
                                HandleGameObject(cloudstikeExitGuid,     false);
                                break;
                            }
                            case IN_PROGRESS:
                            {
                                HandleGameObject(cloudstikeEntranceGuid, false);
                                HandleGameObject(cloudstikeExitGuid,     false);
                                break;
                            }
                            case DONE:
                            {
                                HandleGameObject(cloudstikeEntranceGuid, true);
                                HandleGameObject(cloudstikeExitGuid,     true);
                                HandleGameObject(snowdriftEntranceGuid,  true);
                                break;
                            }
                        }
                        break;
                    }
                    case DATA_MASTER_SNOWDRIFT:
                    {
                        switch (state)
                        {
                            case NOT_STARTED:
                            case FAIL:
                                aliveNoviceCount                    = MAX_NOVICE;
                                aliveMinibossCount                  = 2;
                                minibossPositionsGuid               = minibossPositionsGuidSave;
                                firstDefeatedNovicePositionsGuid    = firstDefeatedNovicePositionsGuidSave;
                                secondDefeatedNovicePositionsGuid   = secondDefeatedNovicePositionsGuidSave;
                            
                                HandleGameObject(snowdriftEntranceGuid, true);
                                HandleGameObject(snowdriftFirewallGuid, false);
                                HandleGameObject(snowdriftDojoDoorGuid, false);
                                HandleGameObject(snowdriftExitGuid,     false);
                                break;
                            case IN_PROGRESS:
                                HandleGameObject(snowdriftEntranceGuid, false);
                                HandleGameObject(snowdriftDojoDoorGuid, false);
                                break;
                            case DONE:
                                if (GameObject* possessions = instance->GetGameObject(snowdriftPossessionsGuid))
                                    possessions->SetPhaseMask(1, true);
                            
                                HandleGameObject(snowdriftEntranceGuid, true);
                                HandleGameObject(snowdriftFirewallGuid, true);
                                HandleGameObject(snowdriftDojoDoorGuid, true);
                                HandleGameObject(snowdriftExitGuid,     true);
                                HandleGameObject(shaEntranceGuid,       true);
                                break;
                        }
                        break;
                    }
                    case DATA_SHA_VIOLENCE:
                    {
                        switch (state)
                        {
                            case NOT_STARTED:
                            case FAIL:
                                HandleGameObject(shaEntranceGuid,   true);
                                HandleGameObject(shaExitGuid,       false);
                                break;
                            case IN_PROGRESS:
                                HandleGameObject(shaEntranceGuid,   false);
                            case DONE:
                                HandleGameObject(shaEntranceGuid,   true);
                                HandleGameObject(shaExitGuid,       true);
                                break;
                        }
                        break;
                    }
                    case DATA_TARAN_ZHU:
                    {
                        if (state == IN_PROGRESS)
                            DoAddAuraOnPlayers(SPELL_HATE);
                        else
                        {
                            Map::PlayerList const &PlayerList = instance->GetPlayers();

                            if (!PlayerList.isEmpty())
                                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                                    if (Player* player = i->getSource())
                                    {
                                        player->RemoveAurasDueToSpell(SPELL_HATE);
                                        player->RemoveAurasDueToSpell(SPELL_HAZE_OF_HATE);
                                        player->RemoveAurasDueToSpell(SPELL_HAZE_OF_HATE_VISUAL);
                                    }
                        }
                        break;
                    }
                    default:
                        break;
                }

                return true;
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_DEFEATED_NOVICE:
                    {
                        if (!--aliveNoviceCount)
                        {
                            if (Creature* snowdrift = instance->GetCreature(masterSnowdriftGuid))
                            {
                                if (snowdrift->IsAIEnabled)
                                    snowdrift->AI()->DoAction(ACTION_NOVICE_DONE);
                            }
                        }

                        break;
                    }
                    case DATA_DEFEATED_MINIBOSS:
                    {
                        if (!--aliveMinibossCount)
                        {
                            if (Creature* snowdrift = instance->GetCreature(masterSnowdriftGuid))
                            {
                                if (snowdrift->IsAIEnabled)
                                    snowdrift->AI()->DoAction(ACTION_MINIBOSS_DONE);
                            }

                            HandleGameObject(snowdriftFirewallGuid, true);
                            HandleGameObject(snowdriftDojoDoorGuid, true);
                        }

                        break;
                    }
                    case DATA_DEFENDERS_PURIFIED:
                    {
                        if (!instance->IsChallengeMode())
                            break;

                        if (m_PurifiedDefenders >= eScenarioDatas::PurifyCount)
                            break;

                        ++m_PurifiedDefenders;
                        SendScenarioProgressUpdate(CriteriaProgressData(eScenarioDatas::Purified, m_PurifiedDefenders, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

                        if (m_CreatureKilled >= eScenarioDatas::KillCount && m_PurifiedDefenders >= eScenarioDatas::PurifyCount)
                            m_ConditionCompleted = true;

                        break;
                    }
                    default:
                    {
                        if (type < MAX_DATA)
                            dataStorage[type] = data;
                        break;
                    }
                }
            }

            uint32 GetData(uint32 type)
            {
                if (type < MAX_DATA)
                    return dataStorage[type];

                return 0;
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case NPC_GU_CLOUDSTRIKE:        return guCloudstikeGuid;
                    case NPC_MASTER_SNOWDRIFT:      return masterSnowdriftGuid;
                    case NPC_SHA_VIOLENCE:          return shaViolenceGuid;
                    case NPC_TARAN_ZHU:             return taranZhuGuid;
                    case NPC_AZURE_SERPENT:         return azureSerpentGuid;
                    case NPC_ARCHERY_TARGET:        return JadeCore::Containers::SelectRandomContainerElement(archeryTargetGuids);
                    case DATA_RANDOM_FIRST_POS:
                    {
                        if (firstDefeatedNovicePositionsGuid.empty())
                            return 0;

                        uint64 guid = JadeCore::Containers::SelectRandomContainerElement(firstDefeatedNovicePositionsGuid);
                        firstDefeatedNovicePositionsGuid.remove(guid);
                        return guid;
                    }
                    case DATA_RANDOM_SECOND_POS:
                    {
                        if (secondDefeatedNovicePositionsGuid.empty())
                            return 0;

                        uint64 guid = JadeCore::Containers::SelectRandomContainerElement(secondDefeatedNovicePositionsGuid);
                        secondDefeatedNovicePositionsGuid.remove(guid);
                        return guid;
                    }
                    case DATA_RANDOM_MINIBOSS_POS:
                    {
                        if (minibossPositionsGuid.empty())
                            return 0;

                        uint64 guid = JadeCore::Containers::SelectRandomContainerElement(minibossPositionsGuid);
                        minibossPositionsGuid.remove(guid);
                        return guid;
                    }
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

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_shadopan_monastery_InstanceMapScript(map);
        }
};

void AddSC_instance_shadopan_monastery()
{
    new instance_shadopan_monastery();
}

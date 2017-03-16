#include "ScriptPCH.h"
#include "assault_on_zanvess.h"
#include "LFGMgr.h"
#include "Group.h"
#include "WeatherMgr.h"

class instance_assault_on_zanvess : public InstanceMapScript
{
    public:
        instance_assault_on_zanvess() : InstanceMapScript("instance_assault_on_zanvess", 1050) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_assault_on_zanvess_InstanceMapScript(map, SCENARIO_ID_ALLIANCE);
        }

        struct instance_assault_on_zanvess_InstanceMapScript : public InstanceScript
        {
            instance_assault_on_zanvess_InstanceMapScript(Map* map, uint32 scenarioId) : 
                InstanceScript(map), m_Scenario(map, scenarioId, STEP_4)
            {
                Initialize();
            }

            void Initialize()
            {
                teamInInstance = 0;
                isAllPlayersTeleportedToTheBeach = false;
                isFirstTowerDestroyed = false;
                isSecondTowerDestroyed = false;
                isThirdTowerDestroyed = false;

                skyAdmiralRogersGUID = 0;
                rearAdmiralZiggGUID = 0;

                m_UpdateTimer = 2000;
                m_LastArea = 0;
                heartOfZanvessChecked = false;
            }

            // HACK: there are two scenarios but only one map and one instance script
            // so we need to make different scenarios for each team
            void InitializeScenario(uint32 forTeam)
            {
                if (forTeam == ALLIANCE)
                {
                    m_Scenario = ScenarioController(instance, SCENARIO_ID_ALLIANCE, STEP_4);

                    m_Scenario.GetStep(STEP_1).AddCriteria(CRITERIA_JOIN_TO_ATTACK, 1);
                    m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_DESTROY_TOWERS_1, 1);
                    m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_DESTROY_TOWERS_2, 1);
                    m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_DESTROY_TOWERS_3, 1);
                    m_Scenario.GetStep(STEP_3).AddCriteria(CRITERIA_HEART_OF_ZANVESS, 1);
                    m_Scenario.GetStep(STEP_3).AddCriteria(CRITERIA_KILL_BOSH, 1);
                    m_Scenario.GetStep(STEP_4).AddCriteria(CRITERIA_KILL_TELVRAK, 1);
                }
                else if (forTeam == HORDE)
                {
                    m_Scenario = ScenarioController(instance, SCENARIO_ID_HORDE, STEP_4);

                    m_Scenario.GetStep(STEP_1).AddCriteria(CRITERIA_JOIN_TO_ATTACK, 1);
                    m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_DESTROY_TOWERS_1, 1);
                    m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_DESTROY_TOWERS_2, 1);
                    m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_DESTROY_TOWERS_3, 1);
                    m_Scenario.GetStep(STEP_3).AddCriteria(CRITERIA_HEART_OF_ZANVESS, 1);
                    m_Scenario.GetStep(STEP_3).AddCriteria(CRITERIA_KILL_SCOOTER, 1);
                    m_Scenario.GetStep(STEP_4).AddCriteria(CRITERIA_KILL_TELVRAK, 1);
                }

                InitializeScenarioCreatures();
            }

            void InitializeScenarioCreatures()
            {
                 m_ScenarioCreatures[STEP_3].push_back(
                    ScenarioCreature(ScenarioCreature::SpawnMode::SPAWN_ON_STEP, NPC_TEAM_LEADER_SCOOTER, 
                    { -1239.36f, 4247.61f, 58.2569f, 0.37201f }));
                 
                 m_ScenarioCreatures[STEP_4].push_back(
                    ScenarioCreature(ScenarioCreature::SpawnMode::SPAWN_ON_STEP, NPC_COMMANDER_TELVRAK, 
                    { -1272.06f, 4582.9f, 127.723f, 6.01117f }));
            }

            void ProcessScenarioCreatures(ScenarioSteps scenarioStep)
            {
                for (ScenarioCreaturesMap::iterator itrStep = m_ScenarioCreatures.begin(); itrStep != m_ScenarioCreatures.end(); ++itrStep)
                {
                    ScenarioSteps creatureStep = itrStep->first;
                    std::list<ScenarioCreature>& creatures = itrStep->second;

                    for (std::list<ScenarioCreature>::iterator itrCreature = creatures.begin(); itrCreature != creatures.end(); ++itrCreature)
                    {
                        ScenarioCreature& creature = (*itrCreature);

                        if (scenarioStep == creatureStep && !creature.IsSpawned() && creature.ShouldSpawnOnStep())
                        {
                            if (Creature* pCreature = instance->SummonCreature(creature.creatureEntry, creature.creaturePosition))
                                creature.creatureGuid = pCreature->GetGUID();
                        }
                        
                        if (scenarioStep != creatureStep && creature.IsSpawned() && creature.ShouldDespawnOnNextStep())
                        {
                            if (Creature* pCreature = instance->GetCreature(creature.creatureGuid))
                                pCreature->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void BeforePlayerEnter(Player* who)
            {
                if (!teamInInstance)
                {
                    teamInInstance = who->GetTeam();

                    InitializeScenario(teamInInstance);
                }
            }

            void OnPlayerEnter(Player* who)
            {
                m_Scenario.SendScenarioState(who);
            }
            
            void OnPlayerExit(Player* who)
            {
                who->RemoveAurasDueToSpell(SPELL_STRAFING_RUN_AURA);
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_7TH_LEGION_PRIEST:
                        if (IsAllianceInstance())
                        {
                            creature->UpdateEntry(NPC_KORKRON_PRIEST);
                        }
                        break;
                    case NPC_7TH_LEGION_INFILTRATOR:
                        if (IsAllianceInstance())
                        {
                            creature->UpdateEntry(NPC_KORKRON_INFILTRATOR);
                        }
                        break;
                    case NPC_TEAM_LEADER_SCOOTER:
                        if (IsAllianceInstance())
                        {
                            creature->UpdateEntry(NPC_SQUAD_LEADER_BOSH);
                        }
                        break;
                    case NPC_KORKRON_PRIEST:
                        if (IsHordeInstance())
                        {
                            creature->UpdateEntry(NPC_7TH_LEGION_PRIEST);
                        }
                        break;
                    case NPC_KORKRON_INFILTRATOR:
                        if (IsHordeInstance())
                        {
                            creature->UpdateEntry(NPC_7TH_LEGION_INFILTRATOR);
                        }
                        break;
                    case NPC_SQUAD_LEADER_BOSH:
                        if (IsHordeInstance())
                        {
                            creature->UpdateEntry(NPC_TEAM_LEADER_SCOOTER);
                        }
                        break;
                    case NPC_SKY_ADMIRAL_ROGERS:
                        skyAdmiralRogersGUID = creature->GetGUID();
                        break;
                    case NPC_REAR_ADMIRAL_ZIGG:
                        rearAdmiralZiggGUID = creature->GetGUID();
                        break;
                    default: 
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    default: 
                        break;
                }
            }

		    void SetData(uint32 type, uint32 data)
            {
                uint32 oldStep = m_Scenario.GetCurrentStep();

                switch (type)
                {
                    case DATA_JOINED_THE_ATTACK:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_JOIN_TO_ATTACK);
                        break;
                    case DATA_DESTROYED_TOWERS_1:
                        ProcessTowerDestroyed(type);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_DESTROY_TOWERS_1);
                        break;
                    case DATA_DESTROYED_TOWERS_2:
                        ProcessTowerDestroyed(type);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_DESTROY_TOWERS_2);
                        break;
                    case DATA_DESTROYED_TOWERS_3:
                        ProcessTowerDestroyed(type);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_DESTROY_TOWERS_3);
                        break;
                    case DATA_HEART_OF_ZANVESS:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_HEART_OF_ZANVESS);
                        break;
                    case DATA_KILLED_SCOOTER:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_KILL_SCOOTER);
                        break;
                    case DATA_KILLED_BOSH:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_KILL_BOSH);
                        break;
                    case DATA_KILLED_TELVRAK:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_KILL_TELVRAK);
                        break;
                    default:
                        break;
                }

                if (oldStep != m_Scenario.GetCurrentStep())
                {
                    ProcessScenarioCreatures((ScenarioSteps)m_Scenario.GetCurrentStep());
                }

                if (m_Scenario.IsCompleted())
                {
                    CompleteScenario();
                }
		    }

		    uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_JOINED_THE_ATTACK:
                        return m_Scenario.GetCurrentStep() == STEP_1 ? IN_PROGRESS : DONE;
                }
                return 0;
		    }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    
                    default:
                        break;
                }

                return 0;
            }

            void CompleteScenario()
            {
                DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIO, m_Scenario.GetScenarioId(), 1);

                RewardLFG();
            }

            void Update(uint32 diff)
            {
                CheckArea(diff);
            }

        private:

            ScenarioController m_Scenario;
            uint32 teamInInstance;
            bool isFirstTowerDestroyed;
            bool isSecondTowerDestroyed;
            bool isThirdTowerDestroyed;
            bool isAllPlayersTeleportedToTheBeach;

            uint64 skyAdmiralRogersGUID;
            uint64 rearAdmiralZiggGUID;

            // TODO: implement this logic into scenariocontroller
            // TODO: move creatures data into a database table
            typedef std::map<ScenarioSteps, std::list<ScenarioCreature> > ScenarioCreaturesMap;
            ScenarioCreaturesMap m_ScenarioCreatures;

            uint32 m_UpdateTimer;
            uint32 m_LastArea;
            bool heartOfZanvessChecked;

        private:

            bool IsHordeInstance() const
            {
                return teamInInstance == HORDE;
            }

            bool IsAllianceInstance() const
            {
                return teamInInstance == ALLIANCE;
            }

            Creature* GetAdmiralForTeam()
            {
                if (IsAllianceInstance())
                {
                    return instance->GetCreature(skyAdmiralRogersGUID);
                }
                else if (IsHordeInstance())
                {
                    return instance->GetCreature(rearAdmiralZiggGUID);
                }

                return NULL;
            }

            void ProcessTowerDestroyed(uint32 data)
            {
                switch (data)
                {
                    case DATA_DESTROYED_TOWERS_1:
                        isFirstTowerDestroyed = true;
                        break;
                    case DATA_DESTROYED_TOWERS_2:
                        isSecondTowerDestroyed = true;
                        break;
                    case DATA_DESTROYED_TOWERS_3:
                        isThirdTowerDestroyed = true;
                        break;
                }

                if (isFirstTowerDestroyed && isSecondTowerDestroyed && isThirdTowerDestroyed)
                {
                    if (Creature* pAdmiral = GetAdmiralForTeam())
                    {
                        pAdmiral->AI()->DoAction(ACTION_TOWERS_DESTROYED);
                    }

                    ApplyStrafingRun(true);

                    // players teleport to the beach manually by clicking on vehicles
                    //MoveAllPlayersToTheBeach();
                }
            }

            void MoveAllPlayersToTheBeach()
            {
                if (!isAllPlayersTeleportedToTheBeach)
                {
                    isAllPlayersTeleportedToTheBeach = true;

                    DoNearTeleportPlayers(beachPos);
                }
            }

            void ApplyStrafingRun(bool apply)
            {
                if (apply)
                {
                    DoAddAuraOnPlayers(SPELL_STRAFING_RUN_AURA);
                }
                else
                {
                    DoRemoveAurasDueToSpellOnPlayers(SPELL_STRAFING_RUN_AURA);
                }
            }

            void RewardLFG()
            {
                if (!instance->GetPlayers().isEmpty())
                {
                    Player* firstPlayer = instance->GetPlayers().getFirst()->getSource();
                    if (firstPlayer)
                    {
                        uint64 gguid = firstPlayer->GetGroup() ? firstPlayer->GetGroup()->GetGUID() : firstPlayer->GetGUID();
                        uint32 gDungeonId = sLFGMgr->GetDungeon(gguid);

                        Map::PlayerList const& players = instance->GetPlayers();
                        if (!players.isEmpty())
                            for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                                if (Player* player = i->getSource())
                                        sLFGMgr->RewardDungeonDoneFor(gDungeonId, player);
                    }
                }
            }

            void CheckArea(const uint32 diff)
            {
                if (m_UpdateTimer <= diff)
                {
                    m_UpdateTimer = 2000;
                    
                    Map::PlayerList const& players = instance->GetPlayers();

                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        Player* player = itr->getSource();
                    
                        uint32 newArea = player->GetAreaId();

                        if (m_LastArea != newArea)
                        {
                            AreaChanged(player, m_LastArea, newArea);

                            m_LastArea = newArea;
                        }
                    }
                }
                else
                {
                    m_UpdateTimer -= diff;
                }
            }

            void AreaChanged(Player* player, uint32 oldArea, uint32 newArea)
            {
                if (newArea == AREA_HEART_OF_ZANVESS)
                {
                    if (!heartOfZanvessChecked && m_Scenario.GetCurrentStep() == STEP_3)
                    {
                        SetData(DATA_HEART_OF_ZANVESS, DONE);
                    }
                }
            }
        };
};

void AddSC_instance_assault_on_zanvess()
{
    new instance_assault_on_zanvess();
}

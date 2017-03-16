#include "ScriptPCH.h"
#include "pursuing_the_black_harvest.h"
#include "LFGMgr.h"
#include "Group.h"
#include "WeatherMgr.h"

class instance_pursuing_the_black_harvest : public InstanceMapScript
{
    public:
        instance_pursuing_the_black_harvest() : InstanceMapScript("instance_pursuing_the_black_harvest", 1112) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_pursuing_the_black_harvest_InstanceMapScript(map, SCENARIO_ID);
        }

        struct instance_pursuing_the_black_harvest_InstanceMapScript : public InstanceScript
        {
            instance_pursuing_the_black_harvest_InstanceMapScript(Map* map, uint32 scenarioId) : 
                InstanceScript(map), m_Scenario(map, scenarioId, STEP_9)
            {
                Initialize();

                m_UpdateTimer = 2000;
                m_LastArea = 0;
            }

            void Initialize()
            {
                m_Scenario.GetStep(STEP_1).AddCriteria(CRITERIA_ENTER_THE_SANCTUM, 1);
                m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_FIND_THE_WAY, 1);
                m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_CLICK_THE_SCROLL, 1);
                m_Scenario.GetStep(STEP_3).AddCriteria(CRITERIA_FOLLOW_AKAMA, 1);
                m_Scenario.GetStep(STEP_4).AddCriteria(CRITERIA_LOOK_AROUND_THE_SANCTUM, 1);
                m_Scenario.GetStep(STEP_5).AddCriteria(CRITERIA_GET_PART_OF_SOUL, 1);
                m_Scenario.GetStep(STEP_6).AddCriteria(CRITERIA_KILL_DEMONS, 1);
                m_Scenario.GetStep(STEP_7).AddCriteria(CRITERIA_COLLECT_TREASURES, 1);
                m_Scenario.GetStep(STEP_7).AddCriteria(CRITERIA_GET_TO_THE_END_TREASURES, 1);
                m_Scenario.GetStep(STEP_8).AddCriteria(CRITERIA_GET_TO_THE_TOP, 1);
                m_Scenario.GetStep(STEP_9).AddCriteria(CRITERIA_KILL_KANRETHAD, 1);
            }

            void OnPlayerEnter(Player* who)
            {
                m_Scenario.SendScenarioState(who);
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
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
                switch (type)
                {
                    case DATA_ENTER_THE_SANCTUM:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_ENTER_THE_SANCTUM, 1);
                        // HACK: complete all steps those don't work yet
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_FIND_THE_WAY, 1);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_CLICK_THE_SCROLL, 1);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_FOLLOW_AKAMA, 1);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_LOOK_AROUND_THE_SANCTUM, 1);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_GET_PART_OF_SOUL, 1);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_KILL_DEMONS, 1);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_COLLECT_TREASURES, 1);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_GET_TO_THE_END_TREASURES, 1);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_GET_TO_THE_TOP, 1);
                        break;
                    case DATA_KILL_KANRETHAD:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_KILL_KANRETHAD, 1);
                        break;
                    default:
                        break;
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
                    case DATA_SCENARIO_STEP: return m_Scenario.GetCurrentStep();
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
                DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIO, SCENARIO_ID, 1);
            }

            void Update(uint32 diff)
            {
                CheckArea(diff);
            }

        private:

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
                if (oldArea == AREA_TRAINING_GROUND &&
                    newArea == AREA_SANCTUARY_OF_SHADOWS)
                {
                    SetData(DATA_ENTER_THE_SANCTUM, DONE);
                }
            }

        private:

            ScenarioController m_Scenario;
            uint32 m_UpdateTimer;
            uint32 m_LastArea;

        };
};

void AddSC_instance_pursuing_the_black_harvest()
{
    new instance_pursuing_the_black_harvest();
}

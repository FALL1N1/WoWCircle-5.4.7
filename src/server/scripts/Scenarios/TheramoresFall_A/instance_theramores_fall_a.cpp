#include "ScriptPCH.h"
#include "theramores_fall_a.h"
#include "LFGMgr.h"
#include "Group.h"

class instance_theramores_fall_a : public InstanceMapScript
{
    public:
        instance_theramores_fall_a() : InstanceMapScript("instance_theramores_fall_a", 1000) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_theramores_fall_a_InstanceMapScript(map);
        }

        struct instance_theramores_fall_a_InstanceMapScript : public InstanceScript
        {
            instance_theramores_fall_a_InstanceMapScript(Map* map) : 
                InstanceScript(map), m_Scenario(map, SCENARIO_ID_A, STEP_6)
            {
                Initialize();
            }

            void Initialize()
            {
                m_Scenario.GetStep(STEP_1).AddCriteria(CRITERIA_SHIP_PROTECTED, 1);
                m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_CAPTAINS_KILLED, 3);
                m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_ROKNAH_KILLED, 10);
                m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_SHIPS_BURNED, 3);
                m_Scenario.GetStep(STEP_3).AddCriteria(CRITERIA_VICIOUS_WYVERN, 2);
                m_Scenario.GetStep(STEP_3).AddCriteria(CRITERIA_GASHNUL, 1);
                m_Scenario.GetStep(STEP_4).AddCriteria(CRITERIA_JAINA_LOCATED, 1);
                m_Scenario.GetStep(STEP_5).AddCriteria(CRITERIA_GATECRUSHER_DESTROYED, 1);
                m_Scenario.GetStep(STEP_5).AddCriteria(CRITERIA_STANDARDS, 3);
                m_Scenario.GetStep(STEP_6).AddCriteria(CRITERIA_WARLORD_KILLED, 1);
                m_Scenario.GetStep(STEP_6).AddCriteria(CRITERIA_JAINA_PROTECTED, 1);

                jainaGUID = 0;
            }

            void OnPlayerEnter(Player* who)
            {
                m_Scenario.SendScenarioState(who);
            }

            bool IsEncounterInProgress() const
            {
                return false;
            }

            void OnCreatureCreate(Creature* creature)
            {
                if (m_Scenario.GetCurrentStep() < STEP_3)
                {
                    if (creature->GetPositionX() >= -3855.83f &&  creature->GetPositionX() <= -3799.60f &&
                        creature->GetPositionY() >= -4582.80f && creature->GetPositionY() <= -4508.72f)
                    {
                        thirdStepCreatures.push_back(creature->GetGUID());
                        creature->SetPhaseMask(2, true);
                    }
                }

                if (m_Scenario.GetCurrentStep() < STEP_5)
                {
                    if (creature->GetPositionX() >= -3863.92f && creature->GetPositionY() >= -4442.23f)
                    {
                        
                        fifthStepCreatures.push_back(creature->GetGUID());
                        creature->SetPhaseMask(2, true);
                    }
                }

                if (m_Scenario.GetCurrentStep() < STEP_6)
                {
                    if (creature->GetPositionX() >= -3658.0f && creature->GetPositionX() <= -3643.0f &&
                        creature->GetPositionY() >= -4460.80f && creature->GetPositionY() <= -4440.72f)
                    {
                        
                        sixthStepCreatures.push_back(creature->GetGUID());
                        creature->SetPhaseMask(2, true);
                    }
                }

                switch (creature->GetEntry())
                {
                    case NPC_LADY_JAINA_PROUDMOORE:
                        jainaGUID = creature->GetGUID();
                        if (m_Scenario.GetCurrentStep() < STEP_4)
                        {
                            creature->SetPhaseMask(2, true);
                        }
                        break;
                    default: 
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
            }

		    void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_SERGEANT_GRUD:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_SHIP_PROTECTED);
                        break;
                    case DATA_LEAKING_OIL_DRUM:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_SHIPS_BURNED);
                        if (m_Scenario.GetStep(STEP_2).IsCompleted())
                            ActivateThrirdStepCreatures();
                        break;
                    case DATA_SKY_CAPTAIN_DAZRIP:
                    case DATA_CAPTAIN_KORTHOK:
                    case DATA_CAPTAIN_MOUSSON:
                    case DATA_CAPTAIN_SEAHOOF:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_CAPTAINS_KILLED);
                        if (m_Scenario.GetStep(STEP_2).IsCompleted())
                            ActivateThrirdStepCreatures();
                        break;
                    case DATA_ROKNAH_KILLED:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_ROKNAH_KILLED);
                        if (m_Scenario.GetStep(STEP_2).IsCompleted())
                            ActivateThrirdStepCreatures();
                        break;
                    case DATA_VICIOUS_WYVERN:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_VICIOUS_WYVERN);
                        if (m_Scenario.GetStep(STEP_3).IsCompleted())
                            ActivateThrirdStepCreatures();
                        break;
                    case DATA_GASHNUL:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_GASHNUL);
                        if (m_Scenario.GetStep(STEP_3).IsCompleted())
                            ActivateFouthStepCreatures();
                        break;
                    case DATA_JAINA_LOCATED:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_JAINA_LOCATED);
                        if (m_Scenario.GetStep(STEP_4).IsCompleted())
                            ActivateFifthStepCreatures();
                        break;
                    case DATA_GATECRUSHER_DESTROYED:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_GATECRUSHER_DESTROYED);
                        if (m_Scenario.GetStep(STEP_5).IsCompleted())
                            ActivateSixthStepCreatures();
                        break;
                    case DATA_STANDARD:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_STANDARDS);
                        if (m_Scenario.GetStep(STEP_5).IsCompleted())
                            ActivateSixthStepCreatures();
                        break;
                    case DATA_WARLORD:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_WARLORD_KILLED);
                        break;
                    case DATA_JAINA_PROTECTED:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_JAINA_PROTECTED);
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
                return 0;
		    }

            uint64 GetData64(uint32 type)
            {
                return 0;
            }

        private:

            uint64 jainaGUID;
            std::list<uint64> thirdStepCreatures;
            std::list<uint64> fifthStepCreatures;
            std::list<uint64> sixthStepCreatures;

            ScenarioController m_Scenario;

        private:

            
            void ActivateThrirdStepCreatures()
            {
                for (std::list<uint64>::const_iterator itr = thirdStepCreatures.begin(); itr != thirdStepCreatures.end(); ++itr)
                {
                    if (Creature* pCreature = instance->GetCreature((*itr)))
                    {
                        pCreature->SetPhaseMask(1, true);
                    }
                }
            }

            void ActivateFouthStepCreatures()
            {
                if (Creature* pCreature = instance->GetCreature(jainaGUID))
                {
                    pCreature->SetPhaseMask(1, true);
                }
            }

            void ActivateFifthStepCreatures()
            {
                for (std::list<uint64>::const_iterator itr = fifthStepCreatures.begin(); itr != fifthStepCreatures.end(); ++itr)
                {
                    if (Creature* pCreature = instance->GetCreature((*itr)))
                    {
                        pCreature->SetPhaseMask(1, true);
                    }
                }
            }


            void ActivateSixthStepCreatures()
            {
                for (std::list<uint64>::const_iterator itr = sixthStepCreatures.begin(); itr != sixthStepCreatures.end(); ++itr)
                {
                    if (Creature* pCreature = instance->GetCreature((*itr)))
                    {
                        pCreature->SetPhaseMask(1, true);
                    }
                }
            }

            void CompleteScenario()
            {
                if (Creature* pCreature = instance->GetCreature(jainaGUID))
                {
                    pCreature->CastSpell(pCreature, 129467, true);
                }

                DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIO, SCENARIO_ID_A, 1);

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
        };
};

void AddSC_instance_theramores_fall_a()
{
    new instance_theramores_fall_a();
}

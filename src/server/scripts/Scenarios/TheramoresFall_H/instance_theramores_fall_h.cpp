#include "ScriptPCH.h"
#include "theramores_fall_h.h"
#include "LFGMgr.h"
#include "Group.h"

class instance_theramores_fall_h : public InstanceMapScript
{
    public:
        instance_theramores_fall_h() : InstanceMapScript("instance_theramores_fall_h", 999) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_theramores_fall_h_InstanceMapScript(map);
        }

        struct instance_theramores_fall_h_InstanceMapScript : public InstanceScript
        {
            instance_theramores_fall_h_InstanceMapScript(Map* map) : 
                InstanceScript(map), m_Scenario(map, SCENARIO_ID_H, STEP_5)
            {
                Initialize();
            }

            void Initialize()
            {
                captains = 0;
                barrels = 0;
                blackFuse = 0;
                gryphons = 0;
                baldruc = 0;
                bigBessa = 0;
                unmannedTanks = 0;
                hedric = 0;
                keys = 0;
                thallen = 0;

                thallenGUID = 0;
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
                    if (creature->GetPositionX() >= -3858.0f &&  creature->GetPositionX() <= -3807.0f &&
                        creature->GetPositionY() >= -4567.0f && creature->GetPositionY() <= -4515.0f)
                    {
                        thirdStepCreatures.push_back(creature->GetGUID());
                        creature->SetPhaseMask(2, true);
                    }
                }

                if (m_Scenario.GetCurrentStep() < STEP_4)
                {
                    if (creature->GetPositionX() >= -3808.0f && 
                        creature->GetEntry() != NPC_BALDRUC && 
                        creature->GetEntry() != NPC_THERAMORE_GRYPHON)
                    {
                        forthStepCreatures.push_back(creature->GetGUID());
                        creature->SetPhaseMask(2, true);
                    }
                }

                if (m_Scenario.GetCurrentStep() < STEP_5)
                {
                    if (creature->GetPositionX() >= -3786.0f && 
                        creature->GetPositionY() <= -4524.0f)
                    {
                        fifthStepCreatures.push_back(creature->GetGUID());
                        creature->SetPhaseMask(2, true);
                    }
                }

                switch (creature->GetEntry())
                {
                    case NPC_THALEN_SONGWEAVER:
                        thallenGUID = creature->GetGUID();
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
                    case DATA_CAPTAIN_BRELLINGER:
                    case DATA_CAPTAIN_DASHING:
                    case DATA_CAPTAIN_TELLERN:
                    case DATA_SQUALLSHAPER_LINARA:
                    case DATA_LIEUTENANT_GRANDERS:
                    case DATA_MAD_MORGEN_MORGENSEN:
                        UpdateCaptainsKilled();
                        CheckFirstStep();
                        break;
                    case DATA_BARREL:
                        UpdateBarrels();
                        CheckFirstStep();
                        break;
                    case DATA_BLACKFUSE:
                        UpdateBlackfuse();
                        CheckSecondStep();
                        break;
                    case DATA_BALDRUC:
                        UpdateBaldruc();
                        CheckThirdStep();
                        break;
                    case DATA_GRYPHONS:
                        UpdateGryphons();
                        CheckThirdStep();
                        break;
                    case DATA_BIG_BESSA:
                        UpdateBigBessa();
                        CheckForthStep();
                        break;
                    case DATA_TANKS:
                        UpdateTanks();
                        CheckForthStep();
                        break;
                    case DATA_HEDRIC:
                        UpdateHedric();
                        CheckFifthStep();
                        break;
                    case DATA_KEY:
                        UpdateKeys();
                        CheckFifthStep();
                        break;
                    case DATA_THALEN:
                        UpdateThallen();
                        CheckFifthStep();
                        break;
                    default:
                        break;
                }
		    }

		    uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_CURRENT_STEP:
                        return m_Scenario.GetCurrentStep();
                    default:
                        break;
                }

                return 0;
		    }

            uint64 GetData64(uint32 type)
            {
                return 0;
            }

        private:

            uint32 captains;
            uint32 barrels;
            uint32 blackFuse;
            uint32 gryphons;
            uint32 baldruc;
            uint32 bigBessa;
            uint32 unmannedTanks;
            uint32 hedric;
            uint32 keys;
            uint32 thallen;

            std::list<uint64> thirdStepCreatures; // baldruc and soldiers
            std::list<uint64> forthStepCreatures; // big bessa and soldiers
            std::list<uint64> fifthStepCreatures; // hedric and soldiers

            uint64 thallenGUID;

            ScenarioController m_Scenario;

        private:

            void UpdateCaptainsKilled()
            {
                if (m_Scenario.GetCurrentStep() != STEP_1)
                    return;

                captains++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_CAPTAINS, captains);
            }

            void UpdateBarrels()
            {
                if (m_Scenario.GetCurrentStep() != STEP_1)
                    return;

                barrels++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_BARRELS, barrels);
            }

            void CheckFirstStep()
            {
                if (m_Scenario.GetCurrentStep() != STEP_1)
                    return;

                if (captains >= 3 && barrels >= 6)
                {
                    m_Scenario.SetCurrentStep(STEP_2);
                    m_Scenario.SendScenarioStateToAll();
                }
            }

            void UpdateBlackfuse()
            {
                if (m_Scenario.GetCurrentStep() != STEP_2)
                    return;

                blackFuse++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_BLACKFUSE, blackFuse);
            }

            void CheckSecondStep()
            {
                if (m_Scenario.GetCurrentStep() != STEP_2)
                    return;

                if (blackFuse >= 1)
                {
                    m_Scenario.SetCurrentStep(STEP_3);
                    m_Scenario.SendScenarioStateToAll();
                    ActivateThrirdStepCreatures();
                }
            }

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

            void UpdateBaldruc()
            {
                if (m_Scenario.GetCurrentStep() != STEP_3)
                    return;

                baldruc++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_BALDRUC, baldruc);
            }

            void UpdateGryphons()
            {
                if (m_Scenario.GetCurrentStep() != STEP_3)
                    return;

                gryphons++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_GRYPHONS, gryphons);
            }

            void CheckThirdStep()
            {
                if (m_Scenario.GetCurrentStep() != STEP_3)
                    return;

                if (baldruc >= 1 && gryphons >= 2)
                {
                    m_Scenario.SetCurrentStep(STEP_4);
                    m_Scenario.SendScenarioStateToAll();
                    ActivateForthStepCreatures();
                }
            }

            void ActivateForthStepCreatures()
            {
                for (std::list<uint64>::const_iterator itr = forthStepCreatures.begin(); itr != forthStepCreatures.end(); ++itr)
                {
                    if (Creature* pCreature = instance->GetCreature((*itr)))
                    {
                        pCreature->SetPhaseMask(1, true);
                    }
                }
            }

            void UpdateBigBessa()
            {
                if (m_Scenario.GetCurrentStep() != STEP_4)
                    return;

                bigBessa++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_BIG_BESSA, bigBessa);
            }

            void UpdateTanks()
            {
                if (m_Scenario.GetCurrentStep() != STEP_4)
                    return;

                unmannedTanks++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_TANKS, unmannedTanks);
            }

            void CheckForthStep()
            {
                if (m_Scenario.GetCurrentStep() != STEP_4)
                    return;

                if (bigBessa >= 1 && unmannedTanks >= 3)
                {
                    m_Scenario.SetCurrentStep(STEP_5);
                    m_Scenario.SendScenarioStateToAll();
                    ActivateFifthStepCreatures();
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

            void UpdateHedric()
            {
                if (m_Scenario.GetCurrentStep() != STEP_5)
                    return;

                hedric++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_HEDRIC, hedric);
            }

            void UpdateKeys()
            {
                if (m_Scenario.GetCurrentStep() != STEP_5)
                    return;

                keys++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_KEY, keys);
            }

            void UpdateThallen()
            {
                if (m_Scenario.GetCurrentStep() != STEP_5)
                    return;

                thallen++;

                m_Scenario.SendScenarioProgressToAll(CRITERIA_SPY_FREED, thallen);
            }

            void CheckFifthStep()
            {
                if (m_Scenario.GetCurrentStep() != STEP_5)
                    return;

                if (hedric >= 1 && keys >= 1 && thallen >= 1)
                {
                    CompleteScenario();
                }
            }

            void CompleteScenario()
            {
                if (Creature* pCreature = instance->GetCreature(thallenGUID))
                {
                    pCreature->AI()->DoAction(1);
                }

                DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIO, SCENARIO_ID_H, 1);

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

void AddSC_instance_theramores_fall_h()
{
    new instance_theramores_fall_h();
}

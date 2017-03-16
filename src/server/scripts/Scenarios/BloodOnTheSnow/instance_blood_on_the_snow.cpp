#include "ScriptPCH.h"
#include "blood_on_the_snow.h"
#include "LFGMgr.h"
#include "Group.h"
#include "WeatherMgr.h"

class instance_blood_on_the_snow : public InstanceMapScript
{
    public:
        instance_blood_on_the_snow() : InstanceMapScript("instance_blood_on_the_snow", 1130) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_blood_on_the_snow_InstanceMapScript(map, SCENARIO_ID);
        }

        struct instance_blood_on_the_snow_InstanceMapScript : public InstanceScript
        {
            instance_blood_on_the_snow_InstanceMapScript(Map* map, uint32 scenarioId) : 
                InstanceScript(map), m_Scenario(map, scenarioId, STEP_6)
            {
                Initialize();
            }

            void Initialize()
            {
                m_Scenario.GetStep(STEP_1).AddCriteria(CRITERIA_TALK, 1);
                m_Scenario.GetStep(STEP_2).AddCriteria(CRITERIA_STONEBEARD, 1);
                m_Scenario.GetStep(STEP_3).AddCriteria(CRITERIA_BOLDBREW, 1);
                m_Scenario.GetStep(STEP_4).AddCriteria(CRITERIA_FORGEFELLOW, 1);
                m_Scenario.GetStep(STEP_5).AddCriteria(CRITERIA_ICE_SPIRIT, 1);
                m_Scenario.GetStep(STEP_6).AddCriteria(CRITERIA_HEKIMA, 1);
                m_Scenario.GetStep(STEP_6).AddCriteria(CRITERIA_VILLAGE, 1);

                moiraGUID = 0;
                varianGUID = 0;
                farastuGUID = 0;
                hekimaGUID = 0;
            }

            void OnPlayerEnter(Player* who)
            {
                m_Scenario.SendScenarioState(who);

                if (m_Scenario.GetCurrentStep() <= m_Scenario.GetMaxStep())
                {
                    UpdateWeatherForPlayer(who, true);
                }

                if (m_Scenario.GetCurrentStep() >= STEP_2 && m_Scenario.GetCurrentStep() <= STEP_6)
                {
                    who->AddAura(SPELL_CALL_THE_SHOT, who);
                }
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_MOIRA_THAURISSAN:
                        moiraGUID = creature->GetGUID();
                        break;
                    case NPC_VARIAN_WRYNN:
                        varianGUID = creature->GetGUID();
                        break;
                    case NPC_FARASTU:
                        farastuGUID = creature->GetGUID();
                        // don't use ShowCreature with guid, creature is not in map yet
                        ShowCreature(creature, false);
                        break;
                    case NPC_HEKIMA_THE_WISE:
                        hekimaGUID = creature->GetGUID();
                        // don't use ShowCreature with guid, creature is not in map yet
                        ShowCreature(creature, false);
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
                switch (type)
                {
                    case DATA_TALK_GRIMBOLT:
                        ApplyOrRemoveInstanceAuras(true);
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_TALK);
                        break;
                    case DATA_STONEBEARD:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_STONEBEARD);
                        break;
                    case DATA_BOLDBREW:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_BOLDBREW);
                        break;
                    case DATA_FORGEFELLOW:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_FORGEFELLOW);
                        ShowCreature(farastuGUID, true);
                        break;
                    case DATA_FARASTU:
                        UpdateWeather(false);
                        StartLastPhase();
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_ICE_SPIRIT);
                        break;
                    case DATA_HEKIMA:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_HEKIMA);
                        break;
                    case DATA_VILLAGE:
                        m_Scenario.UpdateCurrentStepCriteria(CRITERIA_VILLAGE);
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
                    case DATA_CURRENT_STEP: return m_Scenario.GetCurrentStep();
                }

                return 0;
		    }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case DATA_MOIRA: return moiraGUID;
                    case DATA_VARIAN: return varianGUID;
                    case DATA_HEKIMA: return hekimaGUID;
                    default:
                        break;
                }

                return 0;
            }

            void CompleteScenario()
            {
                ApplyOrRemoveInstanceAuras(false);

                DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIO, SCENARIO_ID, 1);

                RewardLFG();

                if (Creature* pMoira = instance->GetCreature(moiraGUID))
                {
                    pMoira->AI()->DoAction(ACTION_COMPLETE_SCENARIO);
                }
            }

        private:

            uint64 moiraGUID;
            uint64 varianGUID;
            uint64 farastuGUID;
            uint64 hekimaGUID;

            ScenarioController m_Scenario;

        private:

            void UpdateWeather(bool snow)
            {
                for (Map::PlayerList::const_iterator itr = instance->GetPlayers().begin(); itr != instance->GetPlayers().end(); ++itr)
                {
                    UpdateWeatherForPlayer(itr->getSource(), snow);
                }
            }

            void UpdateWeatherForPlayer(Player* player, bool snow)
            {
                if (snow)
                {
                    WorldPacket data(SMSG_WEATHER, (4 + 4 + 1));
                    data << float(1.0f);
                    data << uint32(6); // snow
                    data.WriteBit(0);
                    player->GetSession()->SendPacket(&data);
                }
                else
                {
                    // send weather for current zone
                    if (Weather* weather = WeatherMgr::FindWeather(player->GetZoneId()))
                        weather->SendWeatherUpdateToPlayer(player);
                    else
                    {
                        if (!WeatherMgr::AddWeather(player->GetZoneId()))
                        {
                            // send fine weather packet to remove old weather
                            WeatherMgr::SendFineWeatherUpdateToPlayer(player);
                        }
                    }
                }
            }

            void ApplyOrRemoveInstanceAuras(bool apply)
            {
                if (apply)
                {
                    DoAddAuraOnPlayers(SPELL_CALL_THE_SHOT);
                }
                else
                {
                    DoRemoveAurasDueToSpellOnPlayers(SPELL_CALL_THE_SHOT);
                }
            }

            void StartLastPhase()
            {
                Creature* pMoira = GetCreature(moiraGUID);
                Creature* pHekima = GetCreature(hekimaGUID);

                if (!pMoira || !pHekima)
                    return;

                // show creatures
                ShowCreature(hekimaGUID, true);

                Position pos;
                pHekima->GetPosition(&pos);

                pMoira->NearTeleportTo(moiraNearHekimaPos);
                
                pMoira->AI()->DoAction(ACTION_LAST_PHASE);
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

            void ShowCreature(uint64 guid, bool on)
            {
                if (on)
                {
                    if (Creature* pCreature = instance->GetCreature(guid))
                        pCreature->SetPhaseMask(1, true);
                }
                else
                {
                    if (Creature* pCreature = instance->GetCreature(guid))
                        pCreature->SetPhaseMask(2, true);
                }
            }

            void ShowCreature(Creature* pCreature, bool on)
            {
                if (on)
                {
                    pCreature->SetPhaseMask(1, true);
                }
                else
                {
                    pCreature->SetPhaseMask(2, true);
                }
            }

            Creature* GetCreature(uint64 guid)
            {
                return instance->GetCreature(guid);
            }
        };
};

void AddSC_instance_blood_on_the_snow()
{
    new instance_blood_on_the_snow();
}

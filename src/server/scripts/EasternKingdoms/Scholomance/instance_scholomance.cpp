#include "ScriptPCH.h"
#include "scholomance.h"

static BossScenarios const g_BossScenarios[] =
{
    { Data::DATA_INSTRUCTOR_CHILLHEART, eScenarioDatas::Chillheart  },
    { Data::DATA_JANDICE_BAROV,         eScenarioDatas::Barov       },
    { Data::DATA_RATTLEGORE,            eScenarioDatas::RattleGore  },
    { Data::DATA_LILLIAN_VOSS,          eScenarioDatas::LilianVoss  },
    { Data::DATA_GANDLING,              eScenarioDatas::Gandling    },
    { 0,                                0                           }
};

class instance_scholomance : public InstanceMapScript
{
    public:
        instance_scholomance() : InstanceMapScript("instance_scholomance", 1007) { }

        struct instance_scholomance_InstanceMapScript: public InstanceScript
        {
            instance_scholomance_InstanceMapScript(InstanceMap* map): InstanceScript(map)  { }

            uint64 jandiceBarovGUID;
            uint64 rattlegoreGUID;
            uint64 lillianVossGUID;
            uint64 gandlingGUID;
            uint64 chillHeartGUID;
            uint64 phylacteryGUID;
            uint64 lillianVossSoulGUID;
            uint64 gadlingEventGUID;

            uint64 cafferOfForgottenSoulsGUID;
            uint64 cafferOfForgottenSoulsHeroicGUID;

            uint32 rattlegoreEvent;
            uint32 lilianEntrance;

            uint32 m_CreatureKilled;

            void Initialize()
            {
                SetBossNumber(MAX_ENCOUNTER);

                chillHeartGUID = 0;
                jandiceBarovGUID = 0;
                rattlegoreGUID = 0;
                lillianVossGUID = 0;
                gandlingGUID = 0;
                gadlingEventGUID = 0;

                phylacteryGUID = 0;
                lillianVossSoulGUID = 0;

                cafferOfForgottenSoulsGUID = 0;
                cafferOfForgottenSoulsHeroicGUID = 0;

                rattlegoreEvent = 0;
                lilianEntrance = 0;

                if (instance->IsChallengeMode())
                    LoadScenariosInfos(g_BossScenarios, eScenarioDatas::ScenarioID);

                m_CreatureKilled = 0;
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

                ++m_CreatureKilled;
                SendScenarioProgressUpdate(CriteriaProgressData(eScenarioDatas::Ennemies, m_CreatureKilled, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

                if (m_CreatureKilled >= eScenarioDatas::KillCount)
                    m_ConditionCompleted = true;
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_JANDICE_BAROV:
                        jandiceBarovGUID = creature->GetGUID();
                        break;
                    case NPC_RATTLEGORE:
                        rattlegoreGUID = creature->GetGUID();
                        break;
                    case NPC_LILLIAN_VOSS:
                        lillianVossGUID = creature->GetGUID();
                        break;
                    case NPC_LILLIAN_VOSS_SOUL:
                        lillianVossSoulGUID = creature->GetGUID();
                        break;
                    case NPC_DARKMASTER_GANDLING:
                        gandlingGUID = creature->GetGUID();
                        break;
                    case NPC_INSTRUCTOR_CHILLHEART:
                        chillHeartGUID = creature->GetGUID();
                        break;
                    case NPC_PHYLACTERY:
                        phylacteryGUID = creature->GetGUID();
                        break;
                    case NPC_DARKMASTER_GANDLING_1:
                        gadlingEventGUID = creature->GetGUID();
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject)
            {
                switch (p_GameObject->GetEntry())
                {
                    case GO_COFFER_OF_FORGOTTEN_SOULS:
                        cafferOfForgottenSoulsGUID = p_GameObject->GetGUID();
                        break;
                    case GO_COFFER_OF_FORGOTTEN_SOULS_H:
                        cafferOfForgottenSoulsHeroicGUID = p_GameObject->GetGUID();
                        break;
                    case GameObjects::ChallengeDoor:
                        m_ChallengeDoorGuid = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_RATTLEGORE_EVENT:
                        rattlegoreEvent = data;
                        break;
                    case DATA_LILIAN_ENTRANCE:
                        lilianEntrance = data;
                        break;
                }

                if (data == DONE)
                    SaveToDB();
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_RATTLEGORE_EVENT:
                        return rattlegoreEvent;
                    case DATA_LILIAN_ENTRANCE:
                        return lilianEntrance;
                }

                return 0;
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case DATA_JANDICE_BAROV:
                        return jandiceBarovGUID;
                    case DATA_RATTLEGORE:
                        return rattlegoreGUID;
                    case DATA_LILLIAN_VOSS:
                        return lillianVossGUID;
                    case DATA_LILLIAN_VOSS_SOUL:
                        return lillianVossSoulGUID;
                    case DATA_GANDLING:
                        return gandlingGUID;
                    case DATA_INSTRUCTOR_CHILLHEART:
                        return chillHeartGUID;
                    case DATA_PHYLACTERY:
                        return phylacteryGUID;
                    case DATA_COFFER_OF_FORGOTTEN_SOULS:
                        return cafferOfForgottenSoulsGUID;
                    case DATA_COFFER_OF_FORGOTTEN_SOULS_HEROIC:
                        return cafferOfForgottenSoulsHeroicGUID;
                    case DATA_GANDLING_EVENT:
                        return gadlingEventGUID;
                }
                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state)
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                return true;
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S O " << GetBossSaveData()
                    << rattlegoreEvent << " " << lilianEntrance << " ";

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* str)
            {
                if (!str)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(str);

                char dataHead1, dataHead2;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'S' && dataHead2 == 'O')
                {
                    for (uint32 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }

                    loadStream >> rattlegoreEvent;
                    if (rattlegoreEvent != DONE)
                        rattlegoreEvent = NOT_STARTED;

                    loadStream >> lilianEntrance;
                    if (lilianEntrance != DONE)
                        lilianEntrance = NOT_STARTED;

                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
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
            return new instance_scholomance_InstanceMapScript(map);
        }
};

void AddSC_instance_scholomance()
{
    new instance_scholomance();
}

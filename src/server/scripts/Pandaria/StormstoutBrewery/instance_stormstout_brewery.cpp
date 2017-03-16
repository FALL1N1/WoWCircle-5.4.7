//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "stormstout_brewery.hpp"

static BossScenarios const g_BossScenarios[] =
{
    { Data::DATA_OOKOOK,                eScenarioDatas::OokOok      },
    { Data::DATA_HOPTALLUS,             eScenarioDatas::Hoptallus   },
    { Data::DATA_YANZHU_THE_UNCASKED,   eScenarioDatas::YanZhu      },
    { 0,                                0                           }
};

class instance_stormstout_brewery : public InstanceMapScript
{
    public:
        instance_stormstout_brewery() : InstanceMapScript("instance_stormstout_brewery", 961) { }

        struct instance_stormstout_brewery_InstanceMapScript : public InstanceScript
        {
            instance_stormstout_brewery_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                Initialize();
            }

            // Bosses.
            uint64 uiOokOok;
            uint64 uiHoptallus;
            uint64 uiYanzhuTheUncasked;
            // NPC special
            uint64 m_YanZhuSummonerGuid;
            // GameObjects
            uint64 m_HoptallusKegGuid;
            uint64 m_CarrotDoorGuid;
            bool OokOokInCombat;
            bool HoptallusSummoned;

            // Hozen killed for Ook-Ook summon.
            uint32 HozenKilled;

            uint32 m_CreatureKilled;

            void Initialize() override
            {
                SetBossNumber(MAX_ENCOUNTERS);

                // Bosses.
                uiOokOok = 0;
                uiHoptallus = 0;
                uiYanzhuTheUncasked = 0;
                // NPC Special
                m_YanZhuSummonerGuid = 0;
                // GameObjects
                m_HoptallusKegGuid = 0;
                m_CarrotDoorGuid = 0;
                OokOokInCombat = false;
                HoptallusSummoned = false;

                // Hozen killed for Ook-Ook summon.
                HozenKilled = 0;

                for (uint32 i = 0; i < MAX_ENCOUNTERS; ++i)
                    SetBossState(i, NOT_STARTED);

                if (instance->IsChallengeMode())
                    LoadScenariosInfos(g_BossScenarios, eScenarioDatas::ScenarioID);

                m_CreatureKilled = 0;
            }

            bool IsEncounterInProgress() const override
            {
                for (uint32 i = 0; i < MAX_ENCOUNTERS; ++i)
                {
                    if (GetBossState(i) == IN_PROGRESS)
                        return true;
                }

                return false;
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch(creature->GetEntry())
                {
                    // Bosses.
                    case BOSS_OOKOOK:
                        uiOokOok = creature->GetGUID();
                        break;
                    case BOSS_HOPTALLUS:
                        uiHoptallus = creature->GetGUID();
                        break;
                    case BOSS_YANZHU_THE_UNCASKED:
                        uiYanzhuTheUncasked = creature->GetGUID();
                        break;
                    // NPCs
                    case NPC_ANCESTRAL_BREWMASTER:
                        creature->AddAura(SPELL_ANCESTRAL_BREWM_V, creature);
                        break;
                    case NPC_GENERAL_PURPOSE_BUNNY_JMF:
                        if (!creature->isAlive())
                            creature->Respawn(true);
                        break;
                    default:
                        break;
                }
            }

            void OnUnitDeath(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TYPEID_PLAYER) 
                    return;

                switch (p_Killed->GetEntry())
                {
                    /// Script for Ook-ook summon bar.
                    case NPC_SODDEN_HOZEN_BRAWLER:
                    case NPC_INFLAMED_HOZEN_BRAWLER:
                    case NPC_SLEEPY_HOZEN_BRAWLER:
                    case NPC_DRUNKEN_HOZEN_BRAWLER:
                    case NPC_HOZEN_BOUNCER:
                    case NPC_HOZEN_PARTY_ANIMAL:
                    {
                        /// Increase Hozen killed count
                        HozenKilled++;

                        Map::PlayerList const& l_PlayerList = instance->GetPlayers();
                        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
                        {
                            if (Player* l_Player = l_Iter->getSource())
                            {
                                if (Aura* l_BananaAura = l_Player->GetAura(SPELL_BANANA_BAR))
                                {
                                    /// We check the counter in advance to summon Ook-Ook at right value
                                    /// Then, update player power
                                    if (HozenKilled < HOZEN_KILLS_NEEDED)
                                        l_Player->SetPower(POWER_ALTERNATE_POWER, HozenKilled);
                                    else
                                    {
                                        if (!OokOokInCombat)
                                        {
                                            if (Creature* l_OokOok = instance->GetCreature(GetData64(BOSS_OOKOOK)))
                                                if (l_OokOok->AI())
                                                    l_OokOok->AI()->DoAction(ACTION_OOK_OOK_FIGHT);

                                            l_Player->RemoveAurasDueToSpell(SPELL_BANANA_BAR);

                                            OokOokInCombat = true;
                                        }
                                    }
                                }
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case GameObjects::GAMEOBJECT_GIANT_BARREL:
                        m_HoptallusKegGuid = p_GameObject->GetGUID();
                        break;
                    case GameObjects::GAMEOBJECT_GIANT_CARROT:
                        m_CarrotDoorGuid = p_GameObject->GetGUID();
                        break;
                    case GameObjects::ChallengeModeDoor:
                        m_ChallengeDoorGuid = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void SetData(uint32 type, uint32 data) override
            {
                SetBossState(type, EncounterState(data));

                if (data == DONE)
                    SaveToDB();
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

            uint32 GetData(uint32 type) override
            {
                // First check for type.
                if (type == DATA_HOZEN_KILLED)
                    return HozenKilled;

                return GetBossState(type);
            }

            uint64 GetData64(uint32 data) override
            {
                switch(data)
                {
                    // Bosses.
                    case BOSS_OOKOOK:
                        return uiOokOok;
                    case BOSS_HOPTALLUS:
                        return uiHoptallus;
                    case BOSS_YANZHU_THE_UNCASKED:
                        return uiYanzhuTheUncasked;
                    // NPC Special
                    case NPC_GENERAL_PURPOSE_BUNNY_B:
                        return m_YanZhuSummonerGuid;
                    // GameObjects
                    case GAMEOBJECT_GIANT_BARREL:
                        return m_HoptallusKegGuid;
                    case GAMEOBJECT_GIANT_CARROT:
                        return m_CarrotDoorGuid;
                    default:
                        return 0;
                }
            }

            bool SetBossState(uint32 data, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(data, state))
                    return false;

                if (state == DONE)
                {
                    switch(data)
                    {
                        case DATA_OOKOOK_EVENT:
                        case DATA_HOPTALLUS_EVENT:
                        case DATA_YANZHU_THE_UNCASKED_EVENT:
                        break;
                    }
                }

                return true;
            }

            std::string GetSaveData() override
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "S B " << GetBossSaveData() << ' ' << HozenKilled;

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* in) override
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'S' && dataHead2 == 'B')
                {
                    for (uint32 i = 0; i < MAX_ENCOUNTERS; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
            
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;
            
                        // Below makes the player on-instance-entry display of bosses killed shit work (SMSG_RAID_INSTANCE_INFO).
                        // Like, say an unbound player joins the party and he tries to enter the dungeon / raid.
                        // This makes sure binding-to-instance-on-entrance confirmation box will properly display bosses defeated / available.
                        SetBossState(i, EncounterState(tmpState));

                        // Load killed Hozen counter.
                        uint32 temp = 0;
                        loadStream >> temp;
                        HozenKilled = temp;
                    }
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

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_stormstout_brewery_InstanceMapScript(map);
        }
};

void AddSC_instance_stormstout_brewery()
{
    new instance_stormstout_brewery();
}

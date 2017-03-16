#include "siege_of_orgrimmar.hpp"

DoorData const doorData[] =
{
    {GO_IMMERSEUS_DOOR,          DATA_IMMERSEUS,          DOOR_TYPE_PASSAGE,    BOUNDARY_NONE   },
    {GO_SHA_ENERGY_WALL,         DATA_FALLEN_PROTECTORS,  DOOR_TYPE_PASSAGE,    BOUNDARY_NONE   },
    {GO_NORUSHEN_EXIT,           DATA_NORUSHEN,           DOOR_TYPE_PASSAGE,    BOUNDARY_NONE   },
    {GO_PRIDE_ENTRANCE,          DATA_SHA_OF_PRIDE,       DOOR_TYPE_ROOM,       BOUNDARY_NONE   },
    {GO_WIND_DOOR,               DATA_IRON_JUGGERNAUT,    DOOR_TYPE_ROOM,       BOUNDARY_NONE   },
    // temporary hack untill we scripts the event
    {GO_ORGRIMMAR_FRONT_GATE,    DATA_IRON_JUGGERNAUT,    DOOR_TYPE_PASSAGE,    BOUNDARY_NONE   },
    {GO_NAZGRIM_ENTRY_DOOR,      DATA_GENERAL_NAZGRIM,    DOOR_TYPE_ROOM,       BOUNDARY_NONE   },
    {GO_OGRAID_LARGE_DOOR,       DATA_GENERAL_NAZGRIM,    DOOR_TYPE_PASSAGE,    BOUNDARY_NONE   },
    
    {0,                                      0,                         DOOR_TYPE_ROOM,       BOUNDARY_NONE},// END
};

class instance_siege_of_orgrimmar : public InstanceMapScript
{
    public:
        instance_siege_of_orgrimmar() : InstanceMapScript("instance_siege_of_orgrimmar", 1136) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_siege_of_orgrimmar_InstanceMapScript(map);
        }

        struct instance_siege_of_orgrimmar_InstanceMapScript : public InstanceScript
        {
            instance_siege_of_orgrimmar_InstanceMapScript(Map* map) : InstanceScript(map) 
            {
                teamInInstance = 0;
                firstGalakrasIntroDone = false;
                secondGalakrasIntroDone = false;
                destroyedCannonsCount = 0;
            }

            void Initialize()
            {
                SetBossNumber(DATA_MAX_BOSS_DATA);
                LoadDoorData(doorData);

                immerseusGuid                   = 0;

                for (uint8 i = 0; i < MAX_FALLEN_PROTECTORS; ++i)
                    fallenProtectorsGuid[i]    = 0;
                
                norushenGuid                    = 0;
                amalgamOfCorruptionGuid         = 0;
                shaOfPrideGuid                  = 0;
                shaOfPrideNorushenGuid          = 0;
                galakrasGuid                    = 0;
                zaelaGuid                       = 0;
                ironJuggernautGuid              = 0;

                for (uint8 i = 0; i < MAX_KORKRON_DARK_SHAMANS; ++i)
                    korkronDarkShamansGuid[i]   = 0;

                malkorokGuid                    = 0;
                spoilsOfPandariaGuid            = 0;
                thokTheBloodthirstyGuid         = 0;
                siegecrafterBlackfuseGuid       = 0;

                for (uint8 i = 0; i < MAX_PARAGONS_OF_THE_KLAXXI; ++i)
                    paragonsOfTheKlaxxiGuid[i]  = 0;

                garroshHellscreamGuid           = 0;

                lorewalkerCho1GUID              = 0;
                lorewalkerCho2GUID              = 0;
                vanityGUID = 0;
                arroganceGUID = 0;
                zealGUID = 0;

                memset(tearsOfTheValeGUIDs, 0, sizeof(tearsOfTheValeGUIDs));
                memset(shadowPrisonGuids, 0, sizeof(shadowPrisonGuids));
                for (uint8 i = 0; i < MAX_SHADOW_PRISONS; ++i)
                    memset(shadowPrisonTrapGuids[i], 0, sizeof(shadowPrisonTrapGuids[i]));

                memset(lightContainmentGUIDs, 0, sizeof(lightContainmentGUIDs));

                memset(vaultOfForbiddenTreasuresGUIDs, 0, sizeof(vaultOfForbiddenTreasuresGUIDs));

                jainaProudmooreGalakrasGUID = 0;
                kingVarianWrynnGalakrasGUID = 0;
                lorthemarTheronGalakrasGUID = 0;
                sylvanasWindrunnerGalakrasGUID = 0;
                aethasSunreaverGalakrasGUID = 0;

                voljinIronJuggernautGUID = 0;
                baineBloodhoofIronJuggernautGUID = 0;

                lorewalkerChoIntro              = 0;
                lorewalkerChoIntroImmerseus     = 0;
                beachCaptured = 0;
            }

            void OnPlayerEnter(Player* player)
            {
                if (!teamInInstance)
                {
                    teamInInstance = player->GetTeam();
                }

                if (GetBossState(DATA_GALAKRAS) == DONE)
                {
                    player->NearTeleportTo(galakrasPlayersPos[2]);
                }
                else if (GetBossState(DATA_SHA_OF_PRIDE) == DONE)
                {
                    if (IsAllianceInstance())
                    {
                        player->NearTeleportTo(galakrasPlayersPos[0]);
                    }
                    else if (IsHordeInstance())
                    {
                        player->NearTeleportTo(galakrasPlayersPos[1]);
                    }

                    SetData(DATA_FIRST_GALAKRAS_INTRO, DONE);
                }
                else if (GetBossState(DATA_FALLEN_PROTECTORS) == DONE)
                {
                    player->NearTeleportTo(norushenTeleportPos);
                }

                // Preload npc
                // General Nazgrim
                instance->LoadGrid(1631.f, -4662.f);
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_IMMERSEUS:
                        immerseusGuid = creature->GetGUID();
                        break;
                    case NPC_ROOK_STONETOE:
                        fallenProtectorsGuid[0] = creature->GetGUID();
                        break;
                    case NPC_HE_SOFTFOOT:
                        fallenProtectorsGuid[1] = creature->GetGUID();
                        break;
                    case NPC_SUN_THUNDERHEART:
                        fallenProtectorsGuid[2] = creature->GetGUID();
                        break;
                    case NPC_NORUSHEN:
                        norushenGuid = creature->GetGUID();
                        break;
                    case NPC_AMALGAM_OF_CORRUPTION:
                        amalgamOfCorruptionGuid = creature->GetGUID();
                        break;
                    case NPC_SHA_OF_PRIDE:
                        shaOfPrideGuid = creature->GetGUID();
                        break;
                    case NPC_NORUSHEN_1:
                        shaOfPrideNorushenGuid = creature->GetGUID();
                        break;
                    case NPC_GALAKRAS:
                        galakrasGuid = creature->GetGUID();
                        break;
                    case NPC_WARLORD_ZAELA:
                        zaelaGuid = creature->GetGUID();
                        break;
                    case NPC_JAINA_PROUDMOORE_2:
                        jainaProudmooreGalakrasGUID = creature->GetGUID();
                        break;
                    case NPC_KING_VARIAN_WRYNN_2:
                        kingVarianWrynnGalakrasGUID = creature->GetGUID();
                        break;
                    case NPC_LORTHEMAR_THERON_2:
                        lorthemarTheronGalakrasGUID = creature->GetGUID();
                        break;
                    case NPC_SYLVANAS_WINDRUNNER_2:
                        sylvanasWindrunnerGalakrasGUID = creature->GetGUID();
                        break;
                    case NPC_AETHAS_SUNREAVER:
                        aethasSunreaverGalakrasGUID = creature->GetGUID();
                        break;
                    case NPC_IRON_JUGGERNAUT:
                        ironJuggernautGuid = creature->GetGUID();
                        break;
                    case NPC_VOLJIN_1:
                        voljinIronJuggernautGUID = creature->GetGUID();
                        break;
                    case NPC_BAINE_BLOODHOOF_1:
                        baineBloodhoofIronJuggernautGUID = creature->GetGUID();
                        break;
                    case NPC_EARTHBREAKER_HAROMM:
                        korkronDarkShamansGuid[0] = creature->GetGUID();
                        break;
                    case NPC_WAVEBINDER_KARDIS:
                        korkronDarkShamansGuid[1] = creature->GetGUID();
                        break;
                    case NPC_GENERAL_NAZGRIM:
                        generalNazgrimGuid = creature->GetGUID();
                        break;
                    case NPC_MALKOROK:
                        malkorokGuid = creature->GetGUID();
                        break;
                    case NPC_SPOILS_OF_PANDARIA:
                        spoilsOfPandariaGuid = creature->GetGUID();
                        break;
                    case NPC_THOK_THE_BLOODTHIRSTY:
                        thokTheBloodthirstyGuid = creature->GetGUID();
                        break;
                    case NPC_SIEGECRAFTER_BLACKFUSE:
                        siegecrafterBlackfuseGuid = creature->GetGUID();
                        break;
                    case NPC_KILRUK_THE_WIND_REAVER:
                        paragonsOfTheKlaxxiGuid[0] = creature->GetGUID();
                        break;
                    case NPC_XARIL_THE_POISONED_MIND:
                        paragonsOfTheKlaxxiGuid[1] = creature->GetGUID();
                        break;
                    case NPC_KAZTIK_THE_MANIPULATOR:
                        paragonsOfTheKlaxxiGuid[2] = creature->GetGUID();
                        break;
                    case NPC_KORVEN_THE_PRIME:
                        paragonsOfTheKlaxxiGuid[3] = creature->GetGUID();
                        break;
                    case NPC_IYYOKUK_THE_LUCID:
                        paragonsOfTheKlaxxiGuid[4] = creature->GetGUID();
                        break;
                    case NPC_KAROZ_THE_LOCUST:
                        paragonsOfTheKlaxxiGuid[5] = creature->GetGUID();
                        break;
                    case NPC_SKEER_THE_BLOODSEEKER:
                        paragonsOfTheKlaxxiGuid[6] = creature->GetGUID();
                        break;
                    case NPC_RIKKAL_THE_DISSECTOR:
                        paragonsOfTheKlaxxiGuid[7] = creature->GetGUID();
                        break;
                    case NPC_HISEK_THE_SWARMKEEPER:
                        paragonsOfTheKlaxxiGuid[8] = creature->GetGUID();
                        break;
                    case NPC_GARROSH_HELLSCREAM:
                        garroshHellscreamGuid = creature->GetGUID();
                        break;
                    case NPC_LOREWALKER_CHO_1:
                        if (creature->GetPositionZ() < 400.0f)
                        {
                            lorewalkerCho1GUID = creature->GetGUID();
                        }
                        else
                        {
                            lorewalkerCho2GUID = creature->GetGUID();
                        }
                        break;
                    case NPC_VANITY:
                        vanityGUID = creature->GetGUID();
                        break;
                    case NPC_ARROGANCE:
                        arroganceGUID = creature->GetGUID();
                        break;
                    case NPC_ZEAL:
                        zealGUID = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureRemove(Creature* creature)
            {
                // There are creatures (bosses) those are summoned by npcs
                // Reset guid accessors to be able to summon bosses again
                switch (creature->GetEntry())
                {
                    case NPC_AMALGAM_OF_CORRUPTION:
                        amalgamOfCorruptionGuid = 0;
                        break;
                    case NPC_SHA_OF_PRIDE:
                        shaOfPrideGuid = 0;
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case GO_TEARS_OF_THE_VALE_10N:
                        tearsOfTheValeGUIDs[0] = go->GetGUID();
                        break;
                    case GO_TEARS_OF_THE_VALE_25N:
                        tearsOfTheValeGUIDs[1] = go->GetGUID();
                        break;
                    case GO_TEARS_OF_THE_VALE_10H:
                        tearsOfTheValeGUIDs[2] = go->GetGUID();
                        break;
                    case GO_TEARS_OF_THE_VALE_25H:
                        tearsOfTheValeGUIDs[3] = go->GetGUID();
                        break;
                    case GO_LIGHT_CONTAINMENT_1:
                        lightContainmentGUIDs[0] = go->GetGUID();
                        break;
                    case GO_LIGHT_CONTAINMENT_2:
                        lightContainmentGUIDs[1] = go->GetGUID();
                        break;
                    case GO_LIGHT_CONTAINMENT_3:
                        lightContainmentGUIDs[2] = go->GetGUID();
                        break;
                    case GO_LIGHT_CONTAINMENT_4:
                        lightContainmentGUIDs[3] = go->GetGUID();
                        break;
                    case GO_LIGHT_CONTAINMENT_TOP:
                        lightContainmentGUIDs[4] = go->GetGUID();
                        break;
                    case GO_LIGHT_CONTAINMENT_BOTTOM:
                        lightContainmentGUIDs[5] = go->GetGUID();
                        break;
                    case GO_WEST_SHADOW_PRISON:
                        shadowPrisonGuids[SHADOW_PRISON_WEST] = go->GetGUID();
                        break;
                    case GO_NORTH_SHADOW_PRISON:
                        shadowPrisonGuids[SHADOW_PRISON_NORTH] = go->GetGUID();
                        break;
                    case GO_EAST_SHADOW_PRISON:
                        shadowPrisonGuids[SHADOW_PRISON_EAST] = go->GetGUID();
                        break;
                    case GO_SOUTH_SHADOW_PRISON:
                        shadowPrisonGuids[SHADOW_PRISON_SOUTH] = go->GetGUID();
                        break;
                    case GO_WEST_SHADOW_PRISON_A:
                        shadowPrisonTrapGuids[SHADOW_PRISON_WEST][SHADOW_PRISON_TRAP_A] = go->GetGUID();
                        break;
                    case GO_WEST_SHADOW_PRISON_B:
                        shadowPrisonTrapGuids[SHADOW_PRISON_WEST][SHADOW_PRISON_TRAP_B] = go->GetGUID();
                        break;
                    case GO_WEST_SHADOW_PRISON_C:
                        shadowPrisonTrapGuids[SHADOW_PRISON_WEST][SHADOW_PRISON_TRAP_C] = go->GetGUID();
                        break;
                    case GO_NORTH_SHADOW_PRISON_A:
                        shadowPrisonTrapGuids[SHADOW_PRISON_NORTH][SHADOW_PRISON_TRAP_A] = go->GetGUID();
                        break;
                    case GO_NORTH_SHADOW_PRISON_B:
                        shadowPrisonTrapGuids[SHADOW_PRISON_NORTH][SHADOW_PRISON_TRAP_B] = go->GetGUID();
                        break;
                    case GO_NORTH_SHADOW_PRISON_C:
                        shadowPrisonTrapGuids[SHADOW_PRISON_NORTH][SHADOW_PRISON_TRAP_C] = go->GetGUID();
                        break;
                    case GO_EAST_SHADOW_PRISON_A:
                        shadowPrisonTrapGuids[SHADOW_PRISON_EAST][SHADOW_PRISON_TRAP_A] = go->GetGUID();
                        break;
                    case GO_EAST_SHADOW_PRISON_B:
                        shadowPrisonTrapGuids[SHADOW_PRISON_EAST][SHADOW_PRISON_TRAP_B] = go->GetGUID();
                        break;
                    case GO_EAST_SHADOW_PRISON_C:
                        shadowPrisonTrapGuids[SHADOW_PRISON_EAST][SHADOW_PRISON_TRAP_C] = go->GetGUID();
                        break;
                    case GO_SOUTH_SHADOW_PRISON_A:
                        shadowPrisonTrapGuids[SHADOW_PRISON_SOUTH][SHADOW_PRISON_TRAP_A] = go->GetGUID();
                        break;
                    case GO_SOUTH_SHADOW_PRISON_B:
                        shadowPrisonTrapGuids[SHADOW_PRISON_SOUTH][SHADOW_PRISON_TRAP_B] = go->GetGUID();
                        break;
                    case GO_SOUTH_SHADOW_PRISON_C:
                        shadowPrisonTrapGuids[SHADOW_PRISON_SOUTH][SHADOW_PRISON_TRAP_C] = go->GetGUID();
                        break;
                    case GO_VAULT_OF_FORBIDDEN_TREASURES_10:
                        vaultOfForbiddenTreasuresGUIDs[0] = go->GetGUID();
                        break;
                    case GO_VAULT_OF_FORBIDDEN_TREASURES_25:
                        vaultOfForbiddenTreasuresGUIDs[1] = go->GetGUID();
                        break;
                    case GO_VAULT_OF_FORBIDDEN_TREASURES_10H:
                        vaultOfForbiddenTreasuresGUIDs[2] = go->GetGUID();
                        break;
                    case GO_VAULT_OF_FORBIDDEN_TREASURES_25H:
                        vaultOfForbiddenTreasuresGUIDs[3] = go->GetGUID();
                        break;
                    case GO_IMMERSEUS_DOOR:
                    case GO_SHA_ENERGY_WALL:
                    case GO_NORUSHEN_EXIT:
                    case GO_PRIDE_ENTRANCE:
                    case GO_WIND_DOOR:
                    case GO_ORGRIMMAR_FRONT_GATE:
                    case GO_NAZGRIM_ENTRY_DOOR:
                    case GO_OGRAID_LARGE_DOOR:
                        AddDoor(go, true);
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    case GO_IMMERSEUS_DOOR:
                    case GO_SHA_ENERGY_WALL:
                    case GO_NORUSHEN_EXIT:
                    case GO_PRIDE_ENTRANCE:
                    case GO_WIND_DOOR:
                    case GO_ORGRIMMAR_FRONT_GATE:
                    case GO_NAZGRIM_ENTRY_DOOR:
                    case GO_OGRAID_LARGE_DOOR:
                        AddDoor(go, false);
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 id, EncounterState state)
            {
                if (!InstanceScript::SetBossState(id, state))
                    return false;

                switch (id)
                {
                    case DATA_SHA_OF_PRIDE:
                        if (state == DONE)
                        {
                            DelayedSpawnNpcAfterShaOfPride();
                        }
                        break;
                }
                

                return true;
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_LOREWALKER_CHO_INTRO:
                        lorewalkerChoIntro = data;
                        break;
                    case DATA_LOREWALKER_CHO_INTRO_IMMERSEUS:
                        lorewalkerChoIntroImmerseus = data;
                        break;
                    case DATA_BEACH_CAPTURED:
                        beachCaptured = data;
                        break;
                    case DATA_FIRST_GALAKRAS_INTRO:
                        DelayedFirstGalakrasIntro();
                        break;
                    case DATA_KORKRON_CANNON_DESTROYED:
                        ProcessCannonsDestroyed();
                        break;
                    case DATA_START_GALAKRAS:
                        StartGalakrasNpc();
                        events.ScheduleEvent(EVENT_START_GALAKRAS, IsAllianceInstance() ? 35000 : 25000);
                        break;
                    case DATA_RESET_GALAKRAS:
                        ResetGalakrasNpc();
                        break;
                    case DATA_IRON_JUGGERNAUT_START:
                        StartIronJuggernautNpc();
                        break;
                    case DATA_IRON_JUGGERNAUT_DEAD:
                        ProcessIronJuggernautDeadNpc();
                        break;
                }

                if (data == DONE)
                {
                    SaveToDB();
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_TEAM_IN_INSTANCE: return teamInInstance;
                    case DATA_LOREWALKER_CHO_INTRO: return lorewalkerChoIntro;
                    case DATA_LOREWALKER_CHO_INTRO_IMMERSEUS: return lorewalkerChoIntroImmerseus;
                }
                return 0;
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case DATA_IMMERSEUS: return immerseusGuid;
                    case DATA_LOREWALKER_CHO_1: return lorewalkerCho1GUID;
                    case DATA_ROOK_STONETOE: return fallenProtectorsGuid[0];
                    case DATA_HE_SOFTFOOT: return fallenProtectorsGuid[1];
                    case DATA_SUN_TENDERHEART: return fallenProtectorsGuid[2];
                    case DATA_VANITY: return vanityGUID;
                    case DATA_ARROGANCE: return arroganceGUID;
                    case DATA_ZEAL: return zealGUID;
                    case DATA_TEARS_OF_THE_VALE_10N: return tearsOfTheValeGUIDs[0];
                    case DATA_TEARS_OF_THE_VALE_25N: return tearsOfTheValeGUIDs[1];
                    case DATA_TEARS_OF_THE_VALE_10H: return tearsOfTheValeGUIDs[2];
                    case DATA_TEARS_OF_THE_VALE_25H: return tearsOfTheValeGUIDs[3];
                    case DATA_NORUSHEN: return norushenGuid;
                    case DATA_AMALGAM_OF_CORRUPTION: return amalgamOfCorruptionGuid;
                    case DATA_LIGHT_CONTAINMENT_1: return lightContainmentGUIDs[0];
                    case DATA_LIGHT_CONTAINMENT_2: return lightContainmentGUIDs[1];
                    case DATA_LIGHT_CONTAINMENT_3: return lightContainmentGUIDs[2];
                    case DATA_LIGHT_CONTAINMENT_4: return lightContainmentGUIDs[3];
                    case DATA_LIGHT_CONTAINMENT_TOP: return lightContainmentGUIDs[4];
                    case DATA_LIGHT_CONTAINMENT_BOTTOM: return lightContainmentGUIDs[5];
                    case DATA_SHA_OF_PRIDE: return shaOfPrideGuid;
                    case DATA_SHA_OF_PRIDE_NORUSHEN: return shaOfPrideNorushenGuid;
                    case DATA_WEST_SHADOW_PRISON: return shadowPrisonGuids[SHADOW_PRISON_WEST];
                    case DATA_WEST_SHADOW_PRISON_A: return shadowPrisonTrapGuids[SHADOW_PRISON_WEST][SHADOW_PRISON_TRAP_A];
                    case DATA_WEST_SHADOW_PRISON_B: return shadowPrisonTrapGuids[SHADOW_PRISON_WEST][SHADOW_PRISON_TRAP_B];
                    case DATA_WEST_SHADOW_PRISON_C: return shadowPrisonTrapGuids[SHADOW_PRISON_WEST][SHADOW_PRISON_TRAP_C];
                    case DATA_NORTH_SHADOW_PRISON: return shadowPrisonGuids[SHADOW_PRISON_NORTH];
                    case DATA_NORTH_SHADOW_PRISON_A: return shadowPrisonTrapGuids[SHADOW_PRISON_NORTH][SHADOW_PRISON_TRAP_A];
                    case DATA_NORTH_SHADOW_PRISON_B: return shadowPrisonTrapGuids[SHADOW_PRISON_NORTH][SHADOW_PRISON_TRAP_B];
                    case DATA_NORTH_SHADOW_PRISON_C: return shadowPrisonTrapGuids[SHADOW_PRISON_NORTH][SHADOW_PRISON_TRAP_C];
                    case DATA_EAST_SHADOW_PRISON: return shadowPrisonGuids[SHADOW_PRISON_EAST];
                    case DATA_EAST_SHADOW_PRISON_A: return shadowPrisonTrapGuids[SHADOW_PRISON_EAST][SHADOW_PRISON_TRAP_A];
                    case DATA_EAST_SHADOW_PRISON_B: return shadowPrisonTrapGuids[SHADOW_PRISON_EAST][SHADOW_PRISON_TRAP_B];
                    case DATA_EAST_SHADOW_PRISON_C: return shadowPrisonTrapGuids[SHADOW_PRISON_EAST][SHADOW_PRISON_TRAP_C];
                    case DATA_SOUTH_SHADOW_PRISON: return shadowPrisonGuids[SHADOW_PRISON_SOUTH];
                    case DATA_SOUTH_SHADOW_PRISON_A: return shadowPrisonTrapGuids[SHADOW_PRISON_SOUTH][SHADOW_PRISON_TRAP_A];
                    case DATA_SOUTH_SHADOW_PRISON_B: return shadowPrisonTrapGuids[SHADOW_PRISON_SOUTH][SHADOW_PRISON_TRAP_B];
                    case DATA_SOUTH_SHADOW_PRISON_C: return shadowPrisonTrapGuids[SHADOW_PRISON_SOUTH][SHADOW_PRISON_TRAP_C];
                    case DATA_VAULT_OF_FORBIDDEN_TREASURES_10: return vaultOfForbiddenTreasuresGUIDs[0];
                    case DATA_VAULT_OF_FORBIDDEN_TREASURES_25: return vaultOfForbiddenTreasuresGUIDs[1];
                    case DATA_VAULT_OF_FORBIDDEN_TREASURES_10H: return vaultOfForbiddenTreasuresGUIDs[2];
                    case DATA_VAULT_OF_FORBIDDEN_TREASURES_25H: return vaultOfForbiddenTreasuresGUIDs[3];
                    case DATA_GALAKRAS: return galakrasGuid;
                    case DATA_WARLORD_ZAELA: return zaelaGuid;
                    case DATA_IRON_JUGGERNAUT: return ironJuggernautGuid;
                    case DATA_VOLJIN_1: return voljinIronJuggernautGUID;
                    case DATA_BAINE_BLOODHOOF_1: return baineBloodhoofIronJuggernautGUID;
                    case DATA_EARTHBREAKER_HAROMM: return korkronDarkShamansGuid[0];
                    case DATA_WAVEBINDER_KARDRIS: return korkronDarkShamansGuid[1];
                    case DATA_GENERAL_NAZGRIM: return generalNazgrimGuid;
                    default:
                        break;
                }

                return 0;
            }

            std::string GetSaveData() 
            {
		    	std::ostringstream saveStream;
		    	saveStream << "S O O " << GetBossSaveData() <<  GetEventData();

		    	return saveStream.str();
		    }

            std::string GetEventData()
            {
                std::ostringstream saveStream;
		    	saveStream << lorewalkerChoIntro << " " << lorewalkerChoIntroImmerseus << " ";

		    	return saveStream.str();
            }

		    void Load(const char* in) 
            {
		    	if (!in) 
                {
		    		OUT_LOAD_INST_DATA_FAIL;
		    		return;
		    	}

		    	char dataHead1, dataHead2, dataHead3;

		    	std::istringstream loadStream(in);
		    	loadStream >> dataHead1 >> dataHead2 >> dataHead3;

		    	if (dataHead1 == 'S' && dataHead2 == 'O' && dataHead3 == 'O') 
                {
                    for (uint32 i = 0; i < DATA_MAX_BOSS_DATA; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;

                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;

                        SetBossState(i, EncounterState(tmpState));
                    }

                    loadStream >> lorewalkerChoIntro;
                    if (lorewalkerChoIntro != DONE)
                        lorewalkerChoIntro = NOT_STARTED;

                    loadStream >> lorewalkerChoIntroImmerseus;
                    if (lorewalkerChoIntroImmerseus != DONE)
                        lorewalkerChoIntroImmerseus = NOT_STARTED;


		    	} else OUT_LOAD_INST_DATA_FAIL;
		    }

            void Update(uint32 diff) override
            {
                events.Update(diff);

                ProcessEvents();
            }

        private:

            bool IsAllianceInstance()
            {
                return teamInInstance == ALLIANCE;
            }

            bool IsHordeInstance()
            {
                return teamInInstance == HORDE;
            }

            void PreLoadGrids()
            {
                instance->LoadGrid(1360.f, -5404.f); // jaina and wrynn galakras intro
                instance->LoadGrid(1756.f, -5258.f); // lor'themar and sylvanas galakras intro
            }

            void ProcessEvents()
            {
                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SPAWN_NPC_SHA_OF_PRIDE:
                            SpawnNpcAfterShaOfPride();
                            break;
                        case EVENT_FIRST_GALAKRAS_INTRO:
                            FirstGalakrasIntro();
                            break;
                        case EVENT_SECOND_GALAKRAS_INTRO:
                            SecondGalakrasIntro();
                            break;
                        case EVENT_START_GALAKRAS:
                            StartGalakras();
                            break;
                    }
                }
            }

            void DelayedSpawnNpcAfterShaOfPride()
            {
                events.ScheduleEvent(EVENT_SPAWN_NPC_SHA_OF_PRIDE, 5000);
            }

            void SpawnNpcAfterShaOfPride()
            {
                if (Creature* pJaina = instance->SummonCreature(NPC_JAINA_PROUDMOORE_1, npcAfterShaOfPridePos[0]))
                {
                    pJaina->AI()->DoAction(ACTION_SHA_OF_PRIDE_JAINA);
                }
                
                if (Creature* pLorthemar = instance->SummonCreature(NPC_LORTHEMAR_THERON_1, npcAfterShaOfPridePos[1]))
                {
                    pLorthemar->AI()->DoAction(ACTION_SHA_OF_PRIDE_LORTHEMAR);
                }
                
                if (Creature* pDummy = instance->SummonCreature(WORLD_TRIGGER, npcAfterShaOfPridePos[2]))
                {
                    pDummy->AddAura(SPELL_GOREHOWL, pDummy);
                }
            }

            void DelayedFirstGalakrasIntro()
            {
                if (firstGalakrasIntroDone)
                    return;

                events.ScheduleEvent(EVENT_FIRST_GALAKRAS_INTRO, 8000);
            }

            void FirstGalakrasIntro()
            {
                if (firstGalakrasIntroDone)
                    return;

                if (IsAllianceInstance())
                {
                    if (Creature* pJaina = instance->GetCreature(jainaProudmooreGalakrasGUID))
                    {
                        pJaina->AI()->DoAction(ACTION_FIRST_INTRO_GALAKRAS);
                    }
                    if (Creature* pWrynn = instance->GetCreature(kingVarianWrynnGalakrasGUID))
                    {
                        pWrynn->AI()->DoAction(ACTION_FIRST_INTRO_GALAKRAS);
                    }
                }
                else if (IsHordeInstance())
                {
                    if (Creature* pLorthemar = instance->GetCreature(lorthemarTheronGalakrasGUID))
                    {
                        pLorthemar->AI()->DoAction(ACTION_FIRST_INTRO_GALAKRAS);
                    }
                    if (Creature* pSylvanas = instance->GetCreature(sylvanasWindrunnerGalakrasGUID))
                    {
                        pSylvanas->AI()->DoAction(ACTION_FIRST_INTRO_GALAKRAS);
                    }
                    if (Creature* pAethas = instance->GetCreature(aethasSunreaverGalakrasGUID))
                    {
                        pAethas->AI()->DoAction(ACTION_FIRST_INTRO_GALAKRAS);
                    }
                }

                firstGalakrasIntroDone = true;
            }

            void ProcessCannonsDestroyed()
            {
                if (destroyedCannonsCount >= 7)
                    return;

                destroyedCannonsCount++;

                if (destroyedCannonsCount >= 7)
                {
                    DelayedSecondGalakrasIntro();
                }
            }

            void DelayedSecondGalakrasIntro()
            {
                if (secondGalakrasIntroDone)
                    return;

                events.ScheduleEvent(EVENT_SECOND_GALAKRAS_INTRO, 8000);
            }

            void SecondGalakrasIntro()
            {
                if (secondGalakrasIntroDone)
                    return;

                PreLoadGrids();

                if (IsAllianceInstance())
                {
                    if (Creature* pJaina = instance->GetCreature(jainaProudmooreGalakrasGUID))
                    {
                        pJaina->AI()->DoAction(ACTION_SECOND_INTRO_GALAKRAS);
                    }
                    if (Creature* pWrynn = instance->GetCreature(kingVarianWrynnGalakrasGUID))
                    {
                        pWrynn->AI()->DoAction(ACTION_SECOND_INTRO_GALAKRAS);
                    }
                }
                else if (IsHordeInstance())
                {
                    if (Creature* pLorthemar = instance->GetCreature(lorthemarTheronGalakrasGUID))
                    {
                        pLorthemar->AI()->DoAction(ACTION_SECOND_INTRO_GALAKRAS);
                    }
                    if (Creature* pSylvanas = instance->GetCreature(sylvanasWindrunnerGalakrasGUID))
                    {
                        pSylvanas->AI()->DoAction(ACTION_SECOND_INTRO_GALAKRAS);
                    }
                    if (Creature* pAethas = instance->GetCreature(aethasSunreaverGalakrasGUID))
                    {
                        pAethas->AI()->DoAction(ACTION_SECOND_INTRO_GALAKRAS);
                    }
                }

                if (Creature* pZaela = instance->GetCreature(zaelaGuid))
                {
                    pZaela->AI()->DoAction(ACTION_SECOND_INTRO_GALAKRAS);
                }

                secondGalakrasIntroDone = true;
            }

            void StartGalakrasNpc()
            {
                if (GetBossState(DATA_GALAKRAS) == IN_PROGRESS ||
                    GetBossState(DATA_GALAKRAS) == DONE)
                {
                    return;
                }

                if (IsAllianceInstance())
                {
                    if (Creature* pJaina = instance->GetCreature(jainaProudmooreGalakrasGUID))
                    {
                        pJaina->AI()->DoAction(ACTION_START_GALAKRAS);
                    }
                    if (Creature* pWrynn = instance->GetCreature(kingVarianWrynnGalakrasGUID))
                    {
                        pWrynn->AI()->DoAction(ACTION_START_GALAKRAS);
                    }
                }
                else if (IsHordeInstance())
                {
                    if (Creature* pLorthemar = instance->GetCreature(lorthemarTheronGalakrasGUID))
                    {
                        pLorthemar->AI()->DoAction(ACTION_START_GALAKRAS);
                    }
                    if (Creature* pSylvanas = instance->GetCreature(sylvanasWindrunnerGalakrasGUID))
                    {
                        pSylvanas->AI()->DoAction(ACTION_START_GALAKRAS);
                    }
                    if (Creature* pAethas = instance->GetCreature(aethasSunreaverGalakrasGUID))
                    {
                        pAethas->AI()->DoAction(ACTION_START_GALAKRAS);
                    }
                }
            }

            void StartGalakras()
            {
                if (GetBossState(DATA_GALAKRAS) == IN_PROGRESS ||
                    GetBossState(DATA_GALAKRAS) == DONE)
                {
                    return;
                }

                if (Creature* pGalakras = instance->GetCreature(galakrasGuid))
                {
                    pGalakras->AI()->DoAction(ACTION_START_GALAKRAS);
                }

                if (Creature* pZaela = instance->GetCreature(zaelaGuid))
                {
                    pZaela->AI()->DoAction(ACTION_START_GALAKRAS);
                }
            }

            void ResetGalakrasNpc()
            {
                if (IsAllianceInstance())
                {
                    if (Creature* pJaina = instance->GetCreature(jainaProudmooreGalakrasGUID))
                    {
                        pJaina->AI()->DoAction(ACTION_RESET_GALAKRAS);
                    }
                    if (Creature* pWrynn = instance->GetCreature(kingVarianWrynnGalakrasGUID))
                    {
                        pWrynn->AI()->DoAction(ACTION_RESET_GALAKRAS);
                    }
                }
                else if (IsHordeInstance())
                {
                    if (Creature* pLorthemar = instance->GetCreature(lorthemarTheronGalakrasGUID))
                    {
                        pLorthemar->AI()->DoAction(ACTION_RESET_GALAKRAS);
                    }
                    if (Creature* pSylvanas = instance->GetCreature(sylvanasWindrunnerGalakrasGUID))
                    {
                        pSylvanas->AI()->DoAction(ACTION_RESET_GALAKRAS);
                    }
                    if (Creature* pAethas = instance->GetCreature(aethasSunreaverGalakrasGUID))
                    {
                        pAethas->AI()->DoAction(ACTION_RESET_GALAKRAS);
                    }
                }
            }

            void StartIronJuggernautNpc()
            {
                if (Creature* pVoljin = instance->GetCreature(voljinIronJuggernautGUID))
                {
                    pVoljin->AI()->DoAction(ACTION_IRON_JUGGERNAUT_START);
                }

                if (Creature* pBaine = instance->GetCreature(baineBloodhoofIronJuggernautGUID))
                {
                    pBaine->AI()->DoAction(ACTION_IRON_JUGGERNAUT_START);
                }
            }

            void ProcessIronJuggernautDeadNpc()
            {
                if (Creature* pVoljin = instance->GetCreature(voljinIronJuggernautGUID))
                {
                    pVoljin->AI()->DoAction(ACTION_IRON_JUGGERNAUT_DEAD);
                }

                if (Creature* pBaine = instance->GetCreature(baineBloodhoofIronJuggernautGUID))
                {
                    pBaine->AI()->DoAction(ACTION_IRON_JUGGERNAUT_DEAD);
                }

                if (Creature* pNazgrim = instance->GetCreature(generalNazgrimGuid))
                {
                    pNazgrim->AI()->DoAction(ACTION_IRON_JUGGERNAUT_DEAD);
                }
            }

        private:

            // First part
            uint64 immerseusGuid;
            uint64 fallenProtectorsGuid[MAX_FALLEN_PROTECTORS];
            uint64 norushenGuid;
            uint64 amalgamOfCorruptionGuid;
            uint64 shaOfPrideGuid;
            uint64 shaOfPrideNorushenGuid;

            // Second part
            uint64 galakrasGuid;
            uint64 zaelaGuid;
            uint64 ironJuggernautGuid;
            uint64 korkronDarkShamansGuid[MAX_KORKRON_DARK_SHAMANS];
            uint64 generalNazgrimGuid;

            // Third part
            uint64 malkorokGuid;
            uint64 spoilsOfPandariaGuid;
            uint64 thokTheBloodthirstyGuid;

            // Last part
            uint64 siegecrafterBlackfuseGuid;
            uint64 paragonsOfTheKlaxxiGuid[MAX_PARAGONS_OF_THE_KLAXXI];
            uint64 garroshHellscreamGuid;

            uint64 lorewalkerCho1GUID;
            uint64 lorewalkerCho2GUID;
            uint64 vanityGUID;
            uint64 arroganceGUID;
            uint64 zealGUID;
            uint64 tearsOfTheValeGUIDs[4];

            uint64 lightContainmentGUIDs[6];

            uint64 shadowPrisonGuids[MAX_SHADOW_PRISONS];
            uint64 shadowPrisonTrapGuids[MAX_SHADOW_PRISONS][MAX_SHADOW_PRISON_TRAPS];

            uint64 vaultOfForbiddenTreasuresGUIDs[4];

            uint64 jainaProudmooreGalakrasGUID;
            uint64 kingVarianWrynnGalakrasGUID;
            uint64 lorthemarTheronGalakrasGUID;
            uint64 sylvanasWindrunnerGalakrasGUID;
            uint64 aethasSunreaverGalakrasGUID;

            uint64 voljinIronJuggernautGUID;
            uint64 baineBloodhoofIronJuggernautGUID;

            uint32 lorewalkerChoIntro;
            uint32 lorewalkerChoIntroImmerseus;
            uint32 beachCaptured;

            EventMap events;

            uint32 teamInInstance;
            
            bool firstGalakrasIntroDone;

            bool secondGalakrasIntroDone;
            uint32 destroyedCannonsCount;
        };
};

void AddSC_instance_siege_of_orgrimmar()
{
    new instance_siege_of_orgrimmar();
}
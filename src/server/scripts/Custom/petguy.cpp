/* Custom Shadowhack PetGuy */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "ScriptedFollowerAI.h"

class petguy : public CreatureScript
{
    public: 
        petguy() : CreatureScript("petguy") { }
                
	bool OnGossipHello(Player* player, Creature* creature)
	{
        ChatHandler handler(player);

        if (player->getClass() != CLASS_HUNTER)
            handler.PSendSysMessage("You aren't a hunter, error!");
        else if (player->getClass() == CLASS_HUNTER && player->GetPet())
		    handler.PSendSysMessage("You already have a pet, error!");
        else if (player->getClass() == CLASS_HUNTER && !player->GetPet())
            ShowMainMenu(player, creature);

		return true;
	}

    void ShowMainMenu(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Ability_Hunter_BeastSoothe:25|t [Pets] ->", GOSSIP_SENDER_MAIN, 1216);
        if (player->GetSpecializationId(player->GetActiveSpec()) == SPEC_HUNTER_BEASTMASTER)
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Ability_Hunter_BeastMastery:25|t [Exotic pets] ->"      , GOSSIP_SENDER_MAIN, 1215);

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }
                
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        uint32 pet_id=0;
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case 1215:
            {
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Chimera:25|t Chimera"     , GOSSIP_SENDER_MAIN, 1241);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_CoreHound:25|t Core Hound", GOSSIP_SENDER_MAIN, 1243);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Devilsaur:25|t Devilsaur", GOSSIP_SENDER_MAIN, 1246);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Spell_shaman_astralshift:25|t Spirit Beast"  , GOSSIP_SENDER_MAIN, 1247);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Rhino:25|t Rhino"         , GOSSIP_SENDER_MAIN, 1248);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Worm:25|t Worm"           , GOSSIP_SENDER_MAIN, 1249);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Silithid:25|t Silithid"  , GOSSIP_SENDER_MAIN, 1250);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\inv_pet_waterstrider:25|t Water Strider"  , GOSSIP_SENDER_MAIN, 1251);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\achievement_moguraid_01:25|t Quilen"  , GOSSIP_SENDER_MAIN, 1262);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\ability_hunter_pet_rhino:25|t Clefthoof"  , GOSSIP_SENDER_MAIN, 1263);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\inv_pet_waterstrider:25|t Water Strider"  , GOSSIP_SENDER_MAIN, 1264);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Spider:25|t Shale Spider"  , GOSSIP_SENDER_MAIN, 1265);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                break;
            }
            case 1216:
            {
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Bat:25|t Bat", GOSSIP_SENDER_MAIN, 1217);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Owl:25|t Owl", GOSSIP_SENDER_MAIN, 1218);
// exotic       player->ADD_GOSSIP_ITEM( 9, "Chimera", GOSSIP_SENDER_MAIN, 1216);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Dragonhawk:25|t Dragonhawk", GOSSIP_SENDER_MAIN, 1219);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_NetherRay:25|t Nether ray", GOSSIP_SENDER_MAIN, 1220);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Ravager:25|t Ravager", GOSSIP_SENDER_MAIN, 1221);
// exotic       player->ADD_GOSSIP_ITEM( 9, "Silitid", GOSSIP_SENDER_MAIN, 1216);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Spider:25|t Spider", GOSSIP_SENDER_MAIN, 1223);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Sporebat:25|t Sporebat", GOSSIP_SENDER_MAIN, 1242);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_WindSerpent:25|t Wind serpent", GOSSIP_SENDER_MAIN, 1225);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Cat:25|t Cat", GOSSIP_SENDER_MAIN, 1227);
// exotic       player->ADD_GOSSIP_ITEM( 9, "Core Hound", GOSSIP_SENDER_MAIN, 1216);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Hyena:25|t Hyena", GOSSIP_SENDER_MAIN, 1228);
// exotic       player->ADD_GOSSIP_ITEM( 11, "Moth", GOSSIP_SENDER_MAIN, 1229);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Raptor:25|t Raptor", GOSSIP_SENDER_MAIN, 1229);
                player->ADD_GOSSIP_ITEM( 7, "More pets?", GOSSIP_SENDER_MAIN, 1300);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                break;
            }
            case 1300:
            {
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_TallStrider:25|t Tallstrider", GOSSIP_SENDER_MAIN, 1230);
                // player->ADD_GOSSIP_ITEM( 11, "Wasp", GOSSIP_SENDER_MAIN, 1231);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Wolf:25|t Wolf", GOSSIP_SENDER_MAIN, 1232);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Bear:25|t Bear", GOSSIP_SENDER_MAIN, 1233);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Boar:25|t Boar", GOSSIP_SENDER_MAIN, 1234);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Crab:25|t Crab", GOSSIP_SENDER_MAIN, 1235);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Crocolisk:25|t Crocolisk", GOSSIP_SENDER_MAIN, 1236);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Gorilla:25|t Gorilla", GOSSIP_SENDER_MAIN, 1237);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Scorpid:25|t Scorpid", GOSSIP_SENDER_MAIN, 1238);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Turtle:25|t Turtle", GOSSIP_SENDER_MAIN, 1239);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_WarpStalker:25|t Warp stalker", GOSSIP_SENDER_MAIN, 1240);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Moth:25|t Moth", GOSSIP_SENDER_MAIN, 1244);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Wasp:25|t Wasp", GOSSIP_SENDER_MAIN, 1251);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Spell_Nature_GuardianWard:25|t Serpent", GOSSIP_SENDER_MAIN, 1252);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Worm:25|t Snowdrift Jormungar", GOSSIP_SENDER_MAIN, 1253);
                player->ADD_GOSSIP_ITEM( 7, "More pets?", GOSSIP_SENDER_MAIN, 1301);
                player->ADD_GOSSIP_ITEM( 7, "Back", GOSSIP_SENDER_MAIN, 1216);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                break;
            }
            case 1301:
            {
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\inv_pet_direhorn:25|t Direhorn", GOSSIP_SENDER_MAIN, 1254);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\ability_mount_goatmountwhite:25|t Goat", GOSSIP_SENDER_MAIN, 1255);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\inv_pet_porcupine:25|t Porcupine", GOSSIP_SENDER_MAIN, 1256);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\Ability_Hunter_Pet_Crocolisk:25|t Gorgonite", GOSSIP_SENDER_MAIN, 1257);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\inv_misc_ahnqirajtrinket_01:25|t Beetle", GOSSIP_SENDER_MAIN, 1258);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\inv_pet_monkey:25|t Monkey", GOSSIP_SENDER_MAIN, 1259);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\ability_hunter_aspectofthefox:25|t Fox", GOSSIP_SENDER_MAIN, 1260);
                player->ADD_GOSSIP_ITEM( 11, "|TInterface\\icons\\inv_pet_mastiff:25|t Dog", GOSSIP_SENDER_MAIN, 1261);
                player->ADD_GOSSIP_ITEM( 7, "Back", GOSSIP_SENDER_MAIN, 1300);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
                break;
            }
            case 1217:
            {
                pet_id=8601; 
                break; //bat
            }
            case 1218:
            {
                pet_id=1995; 
                break; //owl
            }
            case 1219:
            {
                pet_id=20502; 
                break; //dragonhawk
            }
            case 1220:
            {
                pet_id=21901;
                break; //nether ray
            }
            case 1221:
            {
                pet_id=23326;
                break; //ravager
            }
            case 1223:
            {
                pet_id=11921;
                break; //spider
            }
            case 1242: 
            {
                pet_id=18129; 
                break; //sporebat
            }
            case 1225: 
            {
                pet_id=26806;
                break; //Wind serpent
            }
            case 1227:
            {
                pet_id=24530; 
                break; //cat
            }
            case 1228: 
            {
                pet_id=5426; 
                break;    //hyena
            }
            case 1229: 
            {
                pet_id=20728; 
                break; //raptor
            }
            case 1230: 
            {
                pet_id=3244; 
                break; //tallstrider
            }
            case 1232:
            {
                pet_id=2753; 
                break; //wolf
            }
            case 1233: 
            {
                pet_id=8956; 
                break; //bear
            }
            case 1234:
            {
                pet_id=4511; 
                break; //boar
            }
            case 1235:
            {
                pet_id=17217; 
                break; //crab
            }
            case 1236:
            {
                pet_id=24047; 
                break; //crocolisk
            }
            case 1237: 
            {
                pet_id=51661; 
                break; //Gorilla
            }
            case 1238:
            {
                pet_id=3126; 
                break; //scorpid
            }
            case 1239: 
            {
                pet_id=4824; 
                break; //turtle
            }
            case 1240:
            {
                pet_id=23219; 
                break; //warp stalker
            }
            case 1243:
            {
                const uint32 pets[3]={21102, 11672, 17447};
                pet_id=pets[urand(0, 2)];
                break; // core hound
            }
            case 1246:
            {
                const uint32 pets[4]={20931, 32485, 6584, 6498};
                pet_id=pets[urand(0,3)];
                break; // Devilsaur
            }
            case 1247:
            {
                const uint32 pets[4]={38453,35189,33776,32517};
                pet_id=pets[urand(0,3)];
                break; // Spirit beast
            }
            case 1244:
            {
                const uint32 pets[2]={17350, 18468};
                pet_id=pets[urand(0,1)];
                break; // Moth
            }
            case 1248:
            {
                const uint32 pets[3]={25489, 30447, 28009};
                pet_id=pets[urand(0,2)];
                break; // Rhino
            }
            case 1249:
            {
                const uint32 pets[3]={30422, 11320, 11740};
                pet_id=pets[urand(0,2)];
                break; //Worm
            }
            case 1250:
            {
                const uint32 pets[3]={8205, 6582, 4132};
                pet_id=pets[urand(0,2)];
                break; //Silithid
            }
            case 1241:
            {
                const uint32 pets[4]={12800, 11497, 21879, 20932};
                pet_id=pets[urand(0,3)];
                break; //Chimera
            }
            case 1251:
            {
                const uint32 pets[3]={28086, 28085, 18283};
                pet_id=pets[urand(0,2)];
                break; //Wasp
            }
            case 1252:
            {
                pet_id=28358;
                break; //Serpent
            }
            case 1253:
            {
                // Snowdrift Jormungar 
                pet_id = 29390;
                break;
            }
            case 1254:
            {
                // Primal Direhorn Hatchling
                pet_id = 70012;
                break;
            }
            case 1255:
            {
                // Battle Goat
                pet_id = 60010;
                break;
            }
            case 1256:
            {
                // Smoky Porcupine
                pet_id = 59772;
                break;
            }
            case 1257:
            {
                // Gorgonite
                pet_id = 43339;
                break;
            }
            case 1258:
            {
                // Whitefisher Crane
                pet_id = 56752;
                break;
            }
            case 1259:
            {
                // Blistering Scarab
                pet_id = 40310;
                break;
            }
            case 1260:
            {
                // Sly Fox
                pet_id = 58548;
                break;
            }
            case 1261:
            {
                // Bullmastiff
                pet_id = 44476;
                break;
            }
            case 1262:
            {
                // Primal Direhorn Hatchling
                pet_id = 700012;
                break;
            }
            case 1263:
            {
                // Wooly Rhino Bull
                pet_id = 25489;
                break;
            }
            case 1264:
            {
                // Hexapos
                pet_id = 50960;
                break;
            }
            case 1265:
            {
                // Elementium Spinner
                pet_id = 46580;
                break;
            }
            default:break;
        }
        if(pet_id)
        {
            if (player->getSlotForNewPet() == PET_SLOT_FULL_LIST)
            {
                player->SendPetTameResult(PET_TAME_ERROR_TOO_MANY_PETS);
                return false;
            }

            if (!player->GetPet())
            {
                Creature* tempPet=creature->SummonCreature(pet_id,creature->GetPositionX(),creature->GetPositionY(),creature->GetPositionZ(), 0.0f, (TempSummonType)TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000);
                if (!tempPet)
                    return false;

                uint8 level = player->getLevel();

                if (tempPet)
                    tempPet->SetLevel(level);

                Pet* pet = player->CreateTamedPetFrom(tempPet, 13481);
                if (!pet)                                               // in very specific state like near world end/etc.
                    return false;

                // "kill" original creature
                if (tempPet)
                    tempPet->DespawnOrUnsummon();

                // prepare visual effect for levelup
                pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);

                // add to world
                pet->GetMap()->AddToMap(pet->ToCreature());

                // visual effect for levelup
                pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);

                // caster have pet now
                player->SetMinion(pet, true, player->getSlotForNewPet());

                if (player->GetTypeId() == TYPEID_PLAYER)
                {
                    pet->SavePetToDB(PET_SLOT_ACTUAL_PET_SLOT);
                    player->ToPlayer()->PetSpellInitialize();
                    player->ToPlayer()->GetSession()->SendStablePet(0);
                }
            }
            player->CLOSE_GOSSIP_MENU();
        }
        return true;
    }

    bool GossipSelect_petguy(Player *player, Creature *_Creature, uint32 sender, uint32 action )
    {
        // Main menu
        if (sender == GOSSIP_SENDER_MAIN)
        OnGossipSelect(player, _Creature, 0, action);

        return true;
    }
};

void AddSC_petguy()
{
    new petguy;
}
/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: Arena Spectator
%Complete: 100
Comment: Script allow spectate arena games
Category: Custom Script
EndScriptData */

#include "ScriptPCH.h"
#include "Player.h"
#include "Chat.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"

class arena_spectator_commands : public CommandScript
{
    public:
        arena_spectator_commands() : CommandScript("arena_spectator_commands") { }

        static bool HandleSpectateCommand(ChatHandler* handler, const char *args)
        {
            Player* player = NULL;

            std::string name = handler->extractPlayerNameFromLink((char*)args);
            if (!name.empty())
            {
                player = sObjectAccessor->FindPlayerByNameInOrOutOfWorld(name.c_str());
            }
            else
            {
                player = handler->getSelectedPlayer();
            }

            if (!player)
            {
                handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                return false;
            }

            bool result = SpectateByGuid(handler, player->GetGUID());

            return result;
        }

        static bool SpectateByGuid(ChatHandler* handler, uint64 guid)
        {
            //uint32 lowGuid = atoi(args);
            //uint64 targetGuid = MAKE_NEW_GUID(lowGuid, 0, HighGuid::Player);
            uint64 targetGuid = guid;

            Player* player = handler->GetSession()->GetPlayer();
            if (targetGuid == player->GetGUID())
            {
                handler->SendSysMessage(LANG_CANT_TELEPORT_SELF);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->isInCombat())
            {
                handler->SendSysMessage(LANG_YOU_IN_COMBAT);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!player->isAlive())
            {
                handler->SendSysMessage("You are dead.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetPet())
            {
                handler->PSendSysMessage("You must hide your pet.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetSession()->GetInterRealmBG())
            {
                handler->PSendSysMessage("You are already on interrealm server.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            /*if (target && target->GetSession() && !target->GetSession()->GetInterRealmBG())
            {
                handler->PSendSysMessage("Player was not found in arena.");
                handler->SetSentErrorMessage(true);
                return false;
            }*/

            if (InterRealmSession *session = sWorld->GetInterRealmSession())
            {
                session->SendRegisterSpectator(player, targetGuid);
                return true;
            }
            else
            {
                handler->PSendSysMessage("InterRealms doesn't work this moment.");
                handler->SetSentErrorMessage(true);
                return false;
            }
            return true;
        }

        static bool HandleSpectateCancelCommand(ChatHandler* handler, const char* /*args*/)
        {
            Player* player =  handler->GetSession()->GetPlayer();

            if (!player->isSpectator())
            {
                handler->PSendSysMessage("You are not spectator.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!player->GetBattleground())
            {
                handler->PSendSysMessage("You are not spectator.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            player->GetBattleground()->RemoveSpectator(player);
            player->CancelSpectate();
            player->TeleportToBGEntryPoint();

            return true;
        }

        static bool HandleSpectateFromCommand(ChatHandler* handler, const char *args)
        {
            Player* target;
            uint64 target_guid;
            std::string target_name;
            if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
                return false;

            Player* player = handler->GetSession()->GetPlayer();

            if (!target || target->isGameMaster())
            {
                handler->PSendSysMessage("Cant find player.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!player->isSpectator())
            {
                handler->PSendSysMessage(LANG_SPECTATOR_NOT_SPECTATOR);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->isSpectator() && target != player)
            {
                handler->PSendSysMessage("Can`t do that. Your target is spectator.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetMap() != target->GetMap())
            {
                handler->PSendSysMessage("Cant do that. Different arenas?");
                handler->SetSentErrorMessage(true);
                return false;
            }

            // check for arena preperation
            // if exists than battle didn`t begin
            if (target->HasAura(32728) || target->HasAura(32727))
            {
                handler->PSendSysMessage(LANG_SPECTATOR_BG_NOT_STARTED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->HasAuraType(SPELL_AURA_MOD_STEALTH) || target->HasAuraType(SPELL_AURA_MOD_INVISIBILITY))
            {
                handler->PSendSysMessage(LANG_SPECTATOR_CANNT_SEE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            (target == player && player->getSpectateFrom()) ? player->SetViewpoint(player->getSpectateFrom(), false) :
                                                                player->SetViewpoint(target, true);
            return true;
        }

        static bool HandleSpectateResetCommand(ChatHandler* handler, const char *args)
        {
            Player* player = handler->GetSession()->GetPlayer();

            if (!player)
            {
                handler->PSendSysMessage("Cant find player.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!player->isSpectator())
            {
                handler->PSendSysMessage(LANG_SPECTATOR_NOT_SPECTATOR);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Battleground *bGround = player->GetBattleground();
            if (!bGround)
                return false;

            if (bGround->GetStatus() != STATUS_IN_PROGRESS)
                return true;

            for (Battleground::BattlegroundScoreMap::const_iterator itr = bGround->GetPlayerScoresBegin(); itr != bGround->GetPlayerScoresEnd(); ++itr)
                if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
                {
                    uint32 tmpID = bGround->GetPlayerTeam(tmpPlayer->GetGUID());

                    // generate addon massage
                    uint64 pGuid = tmpPlayer->GetGUID();
                    std::string pName = tmpPlayer->GetName();
                    uint64 tGuid = NULL;

                    if (Player* target = tmpPlayer->GetSelectedPlayer())
                        tGuid = target->GetGUID();

                    SpectatorAddonMsg msg;
                    msg.SetPlayer(pGuid);
                    msg.SetName(pName);
                    if (tGuid != NULL)
                        msg.SetTarget(tGuid);
                    if (Pet* pPet = tmpPlayer->GetPet())
                    {
                        msg.SetPet(pPet->GetCreatureTemplate()->family);
                        msg.SetPetHP(pPet->GetHealthPct());
                    }
                    else
                    {
                        msg.SetPet(0);
                        msg.SetPetHP(0);
                    }
                    msg.SetStatus(tmpPlayer->isAlive());
                    msg.SetClass(tmpPlayer->getClass());
                    msg.SetCurrentHP(tmpPlayer->GetHealth());
                    msg.SetMaxHP(tmpPlayer->GetMaxHealth());
                    Powers powerType = tmpPlayer->getPowerType();
                    msg.SetMaxPower(tmpPlayer->GetMaxPower(powerType));
                    msg.SetCurrentPower(tmpPlayer->GetPower(powerType));
                    msg.SetPowerType(powerType);
                    msg.SetTeam(tmpID);
                    msg.SetEndTime(uint32(47*MINUTE - bGround->GetElapsedTime() / IN_MILLISECONDS));
                    msg.SendPacket(player->GetGUID());
                }

            return true;
        }

        ChatCommand* GetCommands() const
        {
            static ChatCommand spectateCommandTable[] =
            {
                { "player",         SEC_PLAYER,      true,  &HandleSpectateCommand,        "", NULL },
                { "view",           SEC_PLAYER,      true,  &HandleSpectateFromCommand,    "", NULL },
                { "reset",          SEC_PLAYER,      true,  &HandleSpectateResetCommand,   "", NULL },
                { "leave",          SEC_PLAYER,      true,  &HandleSpectateCancelCommand,  "", NULL },
                { NULL,             0,               false, NULL,                          "", NULL }
            };

            static ChatCommand commandTable[] =
            {
                { "spectate",       SEC_PLAYER, false,  NULL, "", spectateCommandTable },
                { NULL,             0,          false,  NULL, "", NULL }
            };
            return commandTable;
        }
};


enum NpcSpectatorAtions {

    // will be used for scrolling
    NPC_SPECTATOR_REFRESH_LOW               = 1,
    NPC_SPECTATOR_REFRESH_HIGH              = 2,

    NPC_SPECTATOR_ACTION_LIST_GAMES         = 1000,
    NPC_SPECTATOR_ACTION_LIST_TOP_GAMES     = 2000,

    // NPC_SPECTATOR_ACTION_SELECTED_PLAYER + player.Guid()
    NPC_SPECTATOR_ACTION_SELECTED_PLAYER    = 3000
};

const uint16 TopGamesRating = 1800;
const uint8  GamesOnPage    = 20;

const uint16 arenas[8] = 
        { BATTLEGROUND_NA, BATTLEGROUND_BE, 
        BATTLEGROUND_AA, BATTLEGROUND_RL, 
        BATTLEGROUND_DS, BATTLEGROUND_RV, 
        BATTLEGROUND_TV, BATTLEGROUND_TTP };

class npc_arena_spectator : public CreatureScript
{
    public:
        npc_arena_spectator() : CreatureScript("npc_arena_spectator") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
            if (pPlayer->InBattlegroundQueue())
            {
                pCreature->MonsterWhisper("You should leave the queue!", pPlayer->GetGUID(), true);
                return false;
            }
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "View games with high rating...", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_TOP_GAMES);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "View games with low rating...", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            if (action == NPC_SPECTATOR_REFRESH_LOW ||  (action >= NPC_SPECTATOR_ACTION_LIST_GAMES && action < NPC_SPECTATOR_ACTION_LIST_TOP_GAMES))
            {
                uint16 page = action - NPC_SPECTATOR_ACTION_LIST_GAMES;
                if (action == NPC_SPECTATOR_REFRESH_LOW)
                    page = 0;

                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Refresh", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_REFRESH_LOW);
                ShowPage(player, action == NPC_SPECTATOR_REFRESH_LOW ? 0 : action - NPC_SPECTATOR_ACTION_LIST_GAMES, false);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else if (action == NPC_SPECTATOR_REFRESH_HIGH || (action >= NPC_SPECTATOR_ACTION_LIST_TOP_GAMES && action < NPC_SPECTATOR_ACTION_SELECTED_PLAYER))
            {
                uint16 page = action - NPC_SPECTATOR_ACTION_LIST_TOP_GAMES;
                if (action == NPC_SPECTATOR_REFRESH_HIGH)
                    page = 0;

                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Refresh", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_REFRESH_HIGH);
                ShowPage(player, action == NPC_SPECTATOR_REFRESH_HIGH ? 0 : action - NPC_SPECTATOR_ACTION_LIST_TOP_GAMES, true);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else
            {
                ChatHandler handler(player);
                uint32 lowGuid = action - NPC_SPECTATOR_ACTION_SELECTED_PLAYER;
                //char p[100];
                //ACE_OS::itoa(lowGuid, p, 10);
                uint64 playerGuid = MAKE_NEW_GUID(lowGuid, 0, HighGuid::Player);
                arena_spectator_commands::SpectateByGuid(&handler, playerGuid);
            }
            return true;
        }

        std::string GetClassNameById(uint8 id)
        {
            std::string sClass = "";
            switch (id)
            {
                case CLASS_WARRIOR:         sClass = "Warrior ";        break;
                case CLASS_PALADIN:         sClass = "Pala ";           break;
                case CLASS_HUNTER:          sClass = "Hunt ";           break;
                case CLASS_ROGUE:           sClass = "Rogue ";          break;
                case CLASS_PRIEST:          sClass = "Priest ";         break;
                case CLASS_DEATH_KNIGHT:    sClass = "DK ";             break;
                case CLASS_SHAMAN:          sClass = "Shama ";          break;
                case CLASS_MAGE:            sClass = "Mage ";           break;
                case CLASS_WARLOCK:         sClass = "Warlock ";        break;
                case CLASS_DRUID:           sClass = "Druid ";          break;
                case CLASS_MONK:            sClass = "Monk ";           break;
            }
            return sClass;
        }

        std::string GetGamesStringData(Battleground *arena, uint16 firstTeamMMR, uint16 secondTeamMMR)
        {
            std::string teamsMember[BG_TEAMS_COUNT];
            uint32 firstTeamId = 0;
            for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (player->isSpectator())
                        continue;

                    uint32 team = itr->second.Team;
                    if (!firstTeamId)
                        firstTeamId = team;

                    teamsMember[firstTeamId == team] += GetSpecClassNameById(player->getClass(), player->GetSpecializationId(player->GetActiveSpec()));
                }

            std::stringstream data;
            data << teamsMember[0] << "[" << firstTeamMMR << "] - " << teamsMember[1] << "[" << secondTeamMMR << "]";
            return data.str();
        }

        Player* GetFirstPlayer(Battleground *arena)
        {
            for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                    return player;
            return NULL;
        }

        void ShowPage(Player *player, uint16 page, bool isTop)
        {
            uint16 highGames  = 0;
            uint16 lowGames   = 0;
            bool haveNextPage = false;

            SpectatorDataStore& sdataStore = sBattlegroundMgr->GetSpectatorDataStore();

            for (auto &kv : sdataStore)
            {
                std::string data = kv.second;

                if (isTop && data[0] == 'H')
                {
                    highGames++;
                    if (highGames > (page + 1) * GamesOnPage)
                    {
                        haveNextPage = true;
                        break;
                    }

                    uint32 lowGuid = GUID_LOPART(kv.first);

                    if (highGames >= page * GamesOnPage)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, data, GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + lowGuid);
                }
                else if (!isTop && data[0] == 'L')
                {
                    lowGames++;
                    if (lowGames >(page + 1) * GamesOnPage)
                    {
                        haveNextPage = true;
                        break;
                    }

                    uint32 lowGuid = GUID_LOPART(kv.first);

                    if (lowGames >= page * GamesOnPage)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, data, GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + lowGuid);
                }
            }

            if (page > 0)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Prev...", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES + page - 1);

            if (haveNextPage)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Next...", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES + page + 1);
        }

        std::string GetSpecClassNameById(uint8 Id, uint32 Spec)
        {
            std::string sClass = "";
            switch (Id)
            {
                case CLASS_WARRIOR:
                    switch (Spec)
                    {
                        case SPEC_WARRIOR_ARMS: sClass = "AWarrior "; break;
                        case SPEC_WARRIOR_FURY: sClass = "FWarrior "; break;
                        case SPEC_WARRIOR_PROTECTION: sClass = "PWarrior "; break;
                    }      
                    break;
                case CLASS_PALADIN:
                    switch (Spec)
                    {
                        case SPEC_PALADIN_HOLY: sClass = "HPala "; break;
                        case SPEC_PALADIN_PROTECTION: sClass = "PPala "; break;
                        case SPEC_PALADIN_RETRIBUTION: sClass = "RPala "; break;
                    }    
                    break;
                case CLASS_HUNTER:
                    switch (Spec)
                    {
                        case SPEC_HUNTER_BEASTMASTER: sClass = "BHunt "; break;
                        case SPEC_HUNTER_MARKSMAN: sClass = "MHunt "; break;
                        case SPEC_HUNTER_SURVIVAL: sClass = "SHunt "; break;
                    }      
                    break;
                case CLASS_ROGUE:
                    switch (Spec)
                    {
                        case SPEC_ROGUE_ASSASSINATION: sClass = "ARogue "; break;
                        case SPEC_ROGUE_COMBAT: sClass = "CRogue "; break;
                        case SPEC_ROGUE_SUBTLETY: sClass = "SRogue "; break;
                    }    
                    break;
                case CLASS_PRIEST:
                    switch (Spec)
                    {
                        case SPEC_PRIEST_DISCIPLINE: sClass = "DPriest "; break;
                        case SPEC_PRIEST_HOLY: sClass = "HPriest "; break;
                        case SPEC_PRIEST_SHADOW: sClass = "SPriest "; break;
                    }  
                    break;
                case CLASS_DEATH_KNIGHT:
                    switch (Spec)
                    {
                        case SPEC_DK_BLOOD: sClass = "BDk "; break;
                        case SPEC_DK_FROST: sClass = "FDk "; break;
                        case SPEC_DK_UNHOLY: sClass = "UDk "; break;
                    }
                    break;
                case CLASS_SHAMAN:
                    switch (Spec)
                    {
                        case SPEC_SHAMAN_ELEMENTAL: sClass = "ELShaman "; break;
                        case SPEC_SHAMAN_ENHANCEMENT: sClass = "ENShaman "; break;
                        case SPEC_SHAMAN_RESTORATION: sClass = "RShaman "; break;
                    }
                    break;
                case CLASS_MAGE:
                    switch (Spec)
                    {
                        case SPEC_MAGE_ARCANE: sClass = "AMage "; break;
                        case SPEC_MAGE_FIRE: sClass = "FIMage "; break;
                        case SPEC_MAGE_FROST: sClass = "FRMage "; break;
                    }
                    break;
                case CLASS_WARLOCK:
                    switch (Spec)
                    {
                        case SPEC_WARLOCK_AFFLICTION: sClass = "ALock "; break;
                        case SPEC_WARLOCK_DEMONOLOGY: sClass = "DemoLock "; break;
                        case SPEC_WARLOCK_DESTRUCTION: sClass = "DestLock "; break;
                    }
                    break;
                case CLASS_DRUID:
                    switch (Spec)
                    {
                        case SPEC_DRUID_BALANCE: sClass = "BDruid "; break;
                        case SPEC_DRUID_CAT: sClass = "FDruid "; break;
                        case SPEC_DRUID_BEAR: sClass = "FDruid "; break;
                        case SPEC_DRUID_RESTORATION: sClass = "RDruid "; break;
                    }
                    break;
                case CLASS_MONK:
                    switch (Spec)
                    {
                        case SPEC_MONK_BREWMASTER: sClass = "BmMonk "; break;
                        case SPEC_MONK_WINDWALKER: sClass = "WwMonk "; break;
                        case SPEC_MONK_MISTWEAVER: sClass = "MwMonk "; break;
                    }
                    break;
            }
            return sClass;
        }
};

void AddSC_arena_spectator_script()
{
    new arena_spectator_commands();
    new npc_arena_spectator();
}

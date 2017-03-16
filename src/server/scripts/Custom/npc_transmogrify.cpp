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
Name: Transmogrify Npc
%Complete: 100
Category: Custom Script
EndScriptData */

#include "ScriptPCH.h"
#include "World.h"

enum TransmogrifyActions 
{
    ACTION_TRANSMOGRIFY_ADD_DISPLAY,
    ACTION_TRANSMOGRIFY_REMOVE_DISPLAY,
    ACTION_TRANSMOGRIFY_REMOVE_ALL_DISPLAY,
    ACTION_TRANSMOGRIFY_ADD_LEGENDARY_DISPLAY,
};

class npc_transmogrify : public CreatureScript
{
    public:
        npc_transmogrify() : CreatureScript("npc_transmogrify") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
            Item *trItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);
            int32 legendaryCost = 0;
            std::string legendaryText = "";
            if (sWorld->getBoolConfig(CONFIG_TRANSMOGRIFY_LEGENDARY_ENABLED))
            {
                legendaryCost = sWorld->getIntConfig(CONFIG_TRANSMOGRIFY_LEGENDARY_COST) / GOLD;
                std::ostringstream legendaryData;
                legendaryData << "Исказить пространство [Legendary: " << legendaryCost << "з.]!";
                legendaryText = legendaryData.str();
            }

            if (trItem)
            {
                //pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "Исказить пространство!", GOSSIP_SENDER_MAIN, ACTION_TRANSMOGRIFY_ADD_DISPLAY, "Вы действительно хотите исказить пространство? После искажения предметы станут персональными и их нельзя будет вернуть продавцу!", PriceInGold, false);

                uint32 PriceInGold = trItem->GetTemplate()->ItemLevel * 10000 + trItem->GetTemplate()->DisplayInfoID;
                if (legendaryCost)
                    pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, legendaryText.c_str(), GOSSIP_SENDER_MAIN, ACTION_TRANSMOGRIFY_ADD_LEGENDARY_DISPLAY, "Вы действительно хотите исказить пространство? После искажения предметы станут персональными и их нельзя будет вернуть продавцу!", legendaryCost * GOLD, false);
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "Отменить искажение с 1 предмета.", GOSSIP_SENDER_MAIN, ACTION_TRANSMOGRIFY_REMOVE_DISPLAY);
            }
            pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "Отменить искажение со всех предметов.", GOSSIP_SENDER_MAIN, ACTION_TRANSMOGRIFY_REMOVE_ALL_DISPLAY, "Вы действительно хотите убрать трансмогрификацию со всех предметов? Это действие НЕЛЬЗЯ отменить!", 0, false);
            pPlayer->SEND_GOSSIP_MENU(51000, pCreature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->SendCloseGossip();

            // cann't do that while in combat
            if (player->isInCombat())
                return false;

            switch (action)
            {
                case ACTION_TRANSMOGRIFY_ADD_DISPLAY:
                    TransmogrifyItem(player, creature);
                    break;
                case ACTION_TRANSMOGRIFY_ADD_LEGENDARY_DISPLAY:
                    TransmogrifyItem(player, creature, true);
                    break;
                case ACTION_TRANSMOGRIFY_REMOVE_DISPLAY:
                    ClearItem(player, creature);
                    break;
                case ACTION_TRANSMOGRIFY_REMOVE_ALL_DISPLAY:
                    ClearAllItems(player, creature);
                    break;
            }
            return true;
        }

        void TransmogrifyItem(Player* player, Creature* creature, bool withPay = false)
        {
            ChatHandler handler(player);
            Item *trItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);
            Item *displayItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + 1);
            if (!trItem || !displayItem)
            {
                handler.PSendSysMessage("Положите предметы в первый и второй слот!");
                return;
            }

            if (displayItem->GetTemplate()->Quality != ITEM_QUALITY_LEGENDARY)
            {
                handler.PSendSysMessage("Предмет для трансмогрификации не является легендарным!");
                return;
            }

            uint64 PriceInGold = withPay ? sWorld->getIntConfig(CONFIG_TRANSMOGRIFY_LEGENDARY_COST) : trItem->GetTemplate()->ItemLevel * 10000 + trItem->GetTemplate()->DisplayInfoID;

            if (!player->HasEnoughMoney(PriceInGold))
            {
                player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, 0, 0, 0);
                return;
            }
            
            uint8 msg = player->CanUseItem(trItem->GetTemplate());
            uint8 msg2 = player->CanUseItem(displayItem->GetTemplate());
            if (msg != EQUIP_ERR_OK || msg2 != EQUIP_ERR_OK)
            {
                player->SendEquipError(EQUIP_ERR_NOT_EQUIPPABLE, NULL, NULL);
                return;
            }

            uint8 result = trItem->CanFakeDisplay(displayItem->GetTemplate()->ItemId, withPay);
            switch (result)
            {
                case FAKE_ERR_CANT_FIND_ITEM:
                    handler.PSendSysMessage("Не найден предмет!");
                    break;
                case FAKE_ERR_WRONG_QUALITY:
                    handler.PSendSysMessage("Нельзя трансмогрифицировать предметы с низким и легендарным качеством!");
                    break;
                case FAKE_ERR_DIFF_SLOTS:
                    handler.PSendSysMessage("Предметы в первом и втором слоте сумки имеют разные типы!");
                    break;
                case FAKE_ERR_DIFF_CLASS:
                    handler.PSendSysMessage("Предметы имеют разные классовые критерии для надевания!");
                    break;  
                case FAKE_ERR_DIFF_RACE:
                    handler.PSendSysMessage("Предметы имеют разные рассовые критерии для надевания!");
                    break;
                case FAKE_ERR_OK:
                {
                    // All okay, proceed
                    trItem->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1, 0);
                    trItem->RemoveFlag(ITEM_FIELD_MODIFIERS_MASK, 0x3);
                    
                    player->SetVisibleItemSlot(trItem->GetSlot(), trItem);

                    trItem->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1, displayItem->GetEntry());
                    trItem->SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x3);

                    player->SetVisibleItemSlot(trItem->GetSlot(), trItem);

                    trItem->UpdatePlayedTime(player);
                    trItem->SetOwnerGUID(player->GetGUID());
                    trItem->SetNotRefundable(player);
                    trItem->ClearSoulboundTradeable(player);
                    trItem->SetTransEntry(displayItem->GetEntry());

                    if (displayItem->GetTemplate()->Bonding == BIND_WHEN_EQUIPED || displayItem->GetTemplate()->Bonding == BIND_WHEN_USE)
                        displayItem->SetBinding(true);

                    displayItem->SetOwnerGUID(player->GetGUID());
                    displayItem->SetNotRefundable(player);
    
                    player->ModifyMoney(-1 * PriceInGold);
                    player->PlayDirectSound(3337);
                    creature->HandleEmoteCommand(EMOTE_ONESHOT_SPELL_CAST);
                    break;
                }
            }
        }

        void ClearAllItems(Player* player, Creature* creature)
        {
            for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; i++)
            {
                if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                {
                    item->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1, 0);
                    item->RemoveFlag(ITEM_FIELD_MODIFIERS_MASK, 0x3);
                    item->SetTransEntry(0);
                    player->SetVisibleItemSlot(item->GetSlot(), item);
                }
            }

            player->PlayDirectSound(3337);
            creature->HandleEmoteCommand(EMOTE_ONESHOT_SPELL_CAST);
        }

        void ClearItem(Player *player, Creature* creature)
        {
            ChatHandler handler(player);
            Item *trItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);
            if (!trItem)
            {
                handler.PSendSysMessage("Положите предмет в первый слот!");
                return;
            }

            uint8 msg = player->CanUseItem(trItem->GetTemplate());
            if (msg != EQUIP_ERR_OK)
            {
                player->SendEquipError(EQUIP_ERR_NOT_EQUIPPABLE, NULL, NULL);
                return;
            }

            trItem->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1, 0);
            trItem->RemoveFlag(ITEM_FIELD_MODIFIERS_MASK, 0x3);
            trItem->SetTransEntry(0);
            player->SetVisibleItemSlot(trItem->GetSlot(), trItem);

            WorldPacket data;
            data << uint8(INVENTORY_SLOT_BAG_0);
            data << uint8(trItem->GetSlot());
            player->GetSession()->HandleAutoEquipItemOpcode(data);

            player->PlayDirectSound(3337);
            creature->HandleEmoteCommand(EMOTE_ONESHOT_SPELL_CAST);
        }
};

void AddSC_transmogrify_script()
{
    new npc_transmogrify;
}

/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "Item.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "ItemEnchantmentMgr.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "ScriptMgr.h"
#include "ConditionMgr.h"

void AddItemsSetItem(Player* player, Item* item)
{
    ItemTemplate const* proto = item->GetTemplate();
    uint32 setid = proto->ItemSet;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);

    if (!set)
    {
        sLog->outError(LOG_FILTER_SQL, "Item set %u for item (id %u) not found, mods not applied.", setid, proto->ItemId);
        return;
    }

    if (set->required_skill_id && player->GetSkillValue(set->required_skill_id) < set->required_skill_value)
        return;

    ItemSetEffect* eff = NULL;

    for (size_t x = 0; x < player->ItemSetEff.size(); ++x)
    {
        if (player->ItemSetEff[x] && player->ItemSetEff[x]->setid == setid)
        {
            eff = player->ItemSetEff[x];
            break;
        }
    }

    if (!eff)
    {
        eff = new ItemSetEffect();
        eff->setid = setid;

        size_t x = 0;
        for (; x < player->ItemSetEff.size(); ++x)
            if (!player->ItemSetEff[x])
                break;

        if (x < player->ItemSetEff.size())
            player->ItemSetEff[x]=eff;
        else
            player->ItemSetEff.push_back(eff);
    }

    ++eff->item_count;

    for (uint32 x = 0; x < MAX_ITEM_SET_SPELLS; ++x)
    {
        if (!set->spells [x])
            continue;
        //not enough for  spell
        if (set->items_to_triggerspell[x] > eff->item_count)
            continue;

        uint32 z = 0;
        for (; z < MAX_ITEM_SET_SPELLS; ++z)
            if (eff->spells[z] && eff->spells[z]->Id == set->spells[x])
                break;

        if (z < MAX_ITEM_SET_SPELLS)
            continue;

        //new spell
        for (uint32 y = 0; y < MAX_ITEM_SET_SPELLS; ++y)
        {
            if (!eff->spells[y])                             // free slot
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(set->spells[x]);
                if (!spellInfo)
                {
                    sLog->outError(LOG_FILTER_PLAYER_ITEMS, "WORLD: unknown spell id %u in items set %u effects", set->spells[x], setid);
                    break;
                }

                // spell casted only if fit form requirement, in other case will casted at form change
                player->ApplyEquipSpell(spellInfo, NULL, true);
                eff->spells[y] = spellInfo;
                break;
            }
        }
    }
}

void RemoveItemsSetItem(Player*player, ItemTemplate const* proto)
{
    uint32 setid = proto->ItemSet;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);

    if (!set)
    {
        sLog->outError(LOG_FILTER_SQL, "Item set #%u for item #%u not found, mods not removed.", setid, proto->ItemId);
        return;
    }

    ItemSetEffect* eff = NULL;
    size_t setindex = 0;
    for (; setindex < player->ItemSetEff.size(); setindex++)
    {
        if (player->ItemSetEff[setindex] && player->ItemSetEff[setindex]->setid == setid)
        {
            eff = player->ItemSetEff[setindex];
            break;
        }
    }

    // can be in case now enough skill requirement for set appling but set has been appliend when skill requirement not enough
    if (!eff)
        return;

    --eff->item_count;

    for (uint32 x = 0; x < MAX_ITEM_SET_SPELLS; x++)
    {
        if (!set->spells[x])
            continue;

        // enough for spell
        if (set->items_to_triggerspell[x] <= eff->item_count)
            continue;

        for (uint32 z = 0; z < MAX_ITEM_SET_SPELLS; z++)
        {
            if (eff->spells[z] && eff->spells[z]->Id == set->spells[x])
            {
                // spell can be not active if not fit form requirement
                player->ApplyEquipSpell(eff->spells[z], NULL, false);
                eff->spells[z]=NULL;
                break;
            }
        }
    }

    if (!eff->item_count)                                    //all items of a set were removed
    {
        ASSERT(eff == player->ItemSetEff[setindex]);
        delete eff;
        player->ItemSetEff[setindex] = NULL;
    }
}

bool ItemCanGoIntoBag(ItemTemplate const* pProto, ItemTemplate const* pBagProto)
{
    if (!pProto || !pBagProto)
        return false;

    switch (pBagProto->Class)
    {
        case ITEM_CLASS_CONTAINER:
            switch (pBagProto->SubClass)
            {
                case ITEM_SUBCLASS_CONTAINER:
                    return true;
                case ITEM_SUBCLASS_SOUL_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_SOUL_SHARDS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_HERB_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_HERBS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_ENCHANTING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ENCHANTING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_MINING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_MINING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_ENGINEERING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ENGINEERING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_GEM_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_GEMS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_LEATHERWORKING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_LEATHERWORKING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_INSCRIPTION_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_INSCRIPTION_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_TACKLE_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_FISHING_SUPP))
                        return false;
                    return true;
                case ITEM_SUBCLASS_COOKING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_COOKING_SUPP))
                        return false;
                    return true;
                default:
                    return false;
            }
        case ITEM_CLASS_QUIVER:
            switch (pBagProto->SubClass)
            {
                case ITEM_SUBCLASS_QUIVER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ARROWS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_AMMO_POUCH:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_BULLETS))
                        return false;
                    return true;
                default:
                    return false;
            }
    }
    return false;
}

Item::Item()
{
    m_objectType |= TYPEMASK_ITEM;
    m_objectTypeId = TYPEID_ITEM;

    m_updateFlag = 0;

    m_valuesCount = ITEM_END;
    m_slot = 0;
    uState = ITEM_NEW;
    uQueuePos = -1;
    m_container = NULL;
    m_lootGenerated = false;
    mb_in_trade = false;
    m_lastPlayedTimeUpdate = time(NULL);

    m_refundRecipient = 0;
    m_paidMoney = 0;
    m_paidExtendedCost = 0;

    // Fuck default constructor, i don't trust it
    m_text = "";

    _dynamicTabCount = ITEM_DYNAMIC_END;

    _transEntry = 0;
}

Item::~Item() {}

bool Item::Create(uint32 guidlow, uint32 itemid, Player const* owner)
{
    Object::_Create(guidlow, 0, HighGuid::Item);

    SetEntry(itemid);
    SetObjectScale(1.0f);

    SetUInt64Value(ITEM_FIELD_OWNER, owner ? owner->GetGUID() : 0);
    SetUInt64Value(ITEM_FIELD_CONTAINED, owner ? owner->GetGUID() : 0);

    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemid);
    if (!itemProto)
        return false;

    // For Item Upgrade
    if (uint32 upgradeId = itemProto->GetBaseUpgradeId())
    {
        SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, upgradeId);
        SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x1 | 0x2 | 0x4);
    }

    SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
    SetUInt32Value(ITEM_FIELD_MAXDURABILITY, itemProto->MaxDurability);
    SetUInt32Value(ITEM_FIELD_DURABILITY, itemProto->MaxDurability);

    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        SetSpellCharges(i, itemProto->Spells[i].SpellCharges);

    SetUInt32Value(ITEM_FIELD_DURATION, itemProto->Duration);
    SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, 0);
    return true;
}

// Returns true if Item is a bag AND it is not empty.
// Returns false if Item is not a bag OR it is an empty bag.
bool Item::IsNotEmptyBag() const
{
    if (Bag const* bag = ToBag())
        return !bag->IsEmpty();
    return false;
}

void Item::UpdateDuration(Player* owner, uint32 diff)
{
    if (!GetUInt32Value(ITEM_FIELD_DURATION))
        return;

    sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::UpdateDuration Item (Entry: %u Duration %u Diff %u)", GetEntry(), GetUInt32Value(ITEM_FIELD_DURATION), diff);

    if (GetUInt32Value(ITEM_FIELD_DURATION) <= diff)
    {
        sScriptMgr->OnItemExpire(owner, GetTemplate());
        owner->DestroyItem(GetBagSlot(), GetSlot(), true);
        return;
    }

    SetUInt32Value(ITEM_FIELD_DURATION, GetUInt32Value(ITEM_FIELD_DURATION) - diff);
    SetState(ITEM_CHANGED, owner);                          // save new time in database
}

void Item::SaveToDB(SQLTransaction& trans)
{
    bool isInTransaction = !(trans.null());
    if (!isInTransaction)
        trans = CharacterDatabase.BeginTransaction();

    uint8 index = 0;
    uint32 guid = GetGUIDLow();
    
    PreparedStatement* stmt = nullptr;

    switch (uState)
    {
        case ITEM_NEW:
        {
            index = 0;

            if (GetEntry() == 82800)
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BATTLE_PET_ITEM_DATA_BY_GUID);
                stmt->setUInt32(  index, guid);
                stmt->setUInt32(++index, GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 3)); // speciesId
                stmt->setUInt32(++index, GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 4)); // breedData
                stmt->setUInt8( ++index, GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 5)); // level
                
                trans->Append(stmt);
            }
        }
        // no breaks
        case ITEM_CHANGED:
        {
            index = 0;
            stmt = CharacterDatabase.GetPreparedStatement(uState == ITEM_NEW ? CHAR_REP_ITEM_INSTANCE : CHAR_UPD_ITEM_INSTANCE);
            stmt->setUInt32(  index, GetEntry());
            stmt->setUInt32(++index, _transEntry);
            stmt->setUInt32(++index, GUID_LOPART(GetOwnerGUID()));
            stmt->setUInt32(++index, GUID_LOPART(GetUInt64Value(ITEM_FIELD_CREATOR)));
            stmt->setUInt32(++index, GUID_LOPART(GetUInt64Value(ITEM_FIELD_GIFTCREATOR)));
            stmt->setUInt32(++index, GetCount());
            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_DURATION));

            std::ostringstream ssSpells;
            for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
                ssSpells << GetSpellCharges(i) << ' ';
            stmt->setString(++index, ssSpells.str());

            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_FLAGS));

            std::ostringstream ssEnchants;
            for (uint8 i = 0; i < MAX_ENCHANTMENT_SLOT; ++i)
            {
                ssEnchants << GetEnchantmentId(EnchantmentSlot(i)) << ' ';
                ssEnchants << GetEnchantmentDuration(EnchantmentSlot(i)) << ' ';
                ssEnchants << GetEnchantmentCharges(EnchantmentSlot(i)) << ' ';
            }
            stmt->setString(++index, ssEnchants.str());

            stmt->setInt16 (++index, GetItemRandomPropertyId());
            stmt->setUInt32(++index, GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 0)); // reforge Id
            stmt->setUInt32(++index, GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1)); // Transmogrification Id
            stmt->setUInt32(++index, GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2)); // itemUpgrade Id
            stmt->setUInt16(++index, GetUInt32Value(ITEM_FIELD_DURABILITY));
            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME));
            stmt->setString(++index, m_text);
            stmt->setUInt32(++index, guid);

            trans->Append(stmt);

            if ((uState == ITEM_CHANGED) && HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_WRAPPED))
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GIFT_OWNER);
                stmt->setUInt32(0, GUID_LOPART(GetOwnerGUID()));
                stmt->setUInt32(1, guid);
                trans->Append(stmt);
            }
            break;
        }
        case ITEM_REMOVED:
        {
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
            stmt->setUInt32(0, guid);
            trans->Append(stmt);

            if (GetEntry() == 82800)
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BATTLE_PET_ITEM_DATA_BY_GUID);
                stmt->setUInt32(0, guid);
                trans->Append(stmt);
            }

            if (HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_WRAPPED))
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);
                stmt->setUInt32(0, guid);
                trans->Append(stmt);
            }

            if (!isInTransaction)
                CharacterDatabase.CommitTransaction(trans);

            delete this;
            return;
        }
        case ITEM_UNCHANGED:
            break;
    }

    SetState(ITEM_UNCHANGED);

    if (!isInTransaction)
        CharacterDatabase.CommitTransaction(trans);
}

bool Item::LoadFromDB(uint32 guid, uint64 owner_guid, Field* fields, uint32 entry, uint32 transEntry /* = 0 */)
{
    //                                              0                1          2       3        4        5         6               7              8            9            10          11         12        13
    //result = CharacterDatabase.PQuery("SELECT creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, reforgeId, transmogrifyId, upgradeId, durability, playedTime, text FROM item_instance WHERE guid = '%u'", guid);

    // create item before any checks for store correct guid
    // and allow use "FSetState(ITEM_REMOVED); SaveToDB();" for deleting item from DB
    Object::_Create(guid, 0, HighGuid::Item);

    // Set entry, MUST be before proto check
    SetEntry(entry);
    SetObjectScale(1.0f);
    SetTransEntry(transEntry);

    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    // set owner (not if item is only loaded for gbank/auction/mail
    if (owner_guid != 0)
        SetOwnerGUID(owner_guid);

    bool need_save = false;                                 // need explicit save data at load fixes
    SetUInt64Value(ITEM_FIELD_CREATOR, MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HighGuid::Player));
    SetUInt64Value(ITEM_FIELD_GIFTCREATOR, MAKE_NEW_GUID(fields[1].GetUInt32(), 0, HighGuid::Player));
    SetCount(fields[2].GetUInt32());

    uint32 duration = fields[3].GetUInt32();
    SetUInt32Value(ITEM_FIELD_DURATION, duration);
    // update duration if need, and remove if not need
    if ((proto->Duration == 0) != (duration == 0))
    {
        SetUInt32Value(ITEM_FIELD_DURATION, proto->Duration);
        need_save = true;
    }

    Tokenizer tokens(fields[4].GetString(), ' ', MAX_ITEM_PROTO_SPELLS);
    if (tokens.size() == MAX_ITEM_PROTO_SPELLS)
        for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
            SetSpellCharges(i, atoi(tokens[i]));

    SetUInt32Value(ITEM_FIELD_FLAGS, fields[5].GetUInt32());
    // Remove bind flag for items vs NO_BIND set
    if (IsSoulBound() && proto->Bonding == NO_BIND)
    {
        ApplyModFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_SOULBOUND, false);
        need_save = true;
    }

    std::string enchants = fields[6].GetString();
    _LoadIntoDataField(enchants.c_str(), ITEM_FIELD_ENCHANTMENT_1_1, MAX_ENCHANTMENT_SLOT * MAX_ENCHANTMENT_OFFSET);

    if (uint32 reforgeEntry = fields[8].GetInt32())
    {
        SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 0, reforgeEntry);
        SetFlag(ITEM_FIELD_MODIFIERS_MASK, 1);
    }

    if (uint32 transmogId = fields[9].GetInt32())
    {
        const ItemTemplate* proto1 = sObjectMgr->GetItemTemplate(transmogId);
        const ItemTemplate* proto2 = GetTemplate();
        if (ItemTemplate::CanTransmogrifyItemWithItem(proto1, proto2))
        {

            SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1, transmogId);
            SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x3);
        }
        else if (CanFakeDisplay(transEntry, true))
        {
            SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1, transmogId);
            SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x3);
        }
        else if (GetOwner())
        {
            SetState(ITEM_CHANGED, GetOwner());
        }
    }

    if (uint32 upgradeId = fields[10].GetUInt32())
    {
        if (proto->CanUpgrade(upgradeId))
        {
             SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, upgradeId);
             SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x1|0x2|0x4);
        }
    }
    else
    {
        // For Item Upgrade
        if (uint32 upgradeId = proto->GetBaseUpgradeId())
        {
            SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, upgradeId);
            SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x1 | 0x2 | 0x4);
        }
    }
    
    if (entry == 82800)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_BATTLE_PET_ITEM_DATA_BY_GUID);
        stmt->setUInt32(0, guid);
        if (PreparedQueryResult battlePetData = CharacterDatabase.Query(stmt))
        {
            if (Field* f = battlePetData->Fetch())
            {
                if (BattlePetSpeciesEntry const* species = sBattlePetSpeciesStore.LookupEntry(f[0].GetUInt32()))
                {
                    SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 3, f[0].GetUInt32());
                    SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 4, f[1].GetUInt32());
                    SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 5, f[2].GetUInt8());
                    SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 6, sObjectMgr->GetCreatureTemplate(species->NpcId)->Modelid1);

                    const uint32 cageModMask = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6;
                    ApplyModFlag(ITEM_FIELD_MODIFIERS_MASK, cageModMask, true);
                }
            }
        }
    }
    
    SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, fields[7].GetInt16());
    // recalculate suffix factor
    if (GetItemRandomPropertyId() < 0)
        UpdateItemSuffixFactor();

    uint32 durability = fields[11].GetUInt16();
    SetUInt32Value(ITEM_FIELD_DURABILITY, durability);
    // update max durability (and durability) if need
    SetUInt32Value(ITEM_FIELD_MAXDURABILITY, proto->MaxDurability);
    if (durability > proto->MaxDurability)
    {
        SetUInt32Value(ITEM_FIELD_DURABILITY, proto->MaxDurability);
        need_save = true;
    }

    SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, fields[12].GetUInt32());
    SetText(fields[13].GetString());

    if (need_save)                                           // normal item changed state set not work at loading
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_INSTANCE_ON_LOAD);
        stmt->setUInt32(0, GetUInt32Value(ITEM_FIELD_DURATION));
        stmt->setUInt32(1, GetUInt32Value(ITEM_FIELD_FLAGS));
        stmt->setUInt32(2, GetUInt32Value(ITEM_FIELD_DURABILITY));
        stmt->setUInt32(3, guid);
        CharacterDatabase.Execute(stmt);
    }

    return true;
}

/*static*/
void Item::DeleteFromDB(SQLTransaction& trans, uint32 itemGuid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
    stmt->setUInt32(0, itemGuid);
    trans->Append(stmt);
}

void Item::DeleteFromDB(SQLTransaction& trans)
{
    DeleteFromDB(trans, GetGUIDLow());
}

/*static*/
void Item::DeleteFromInventoryDB(SQLTransaction& trans, uint32 itemGuid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY_BY_ITEM);
    stmt->setUInt32(0, itemGuid);
    trans->Append(stmt);
}

void Item::DeleteFromInventoryDB(SQLTransaction& trans)
{
    DeleteFromInventoryDB(trans, GetGUIDLow());
}

ItemTemplate const* Item::GetTemplate() const
{
    return sObjectMgr->GetItemTemplate(GetEntry());
}

Player* Item::GetOwner()const
{
    return ObjectAccessor::FindPlayerInOrOutOfWorld(GetOwnerGUID());
}

uint32 Item::GetSkill() const
{
    return GetTemplate()->GetSkill();
}

int32 Item::GenerateItemRandomPropertyId(uint32 item_id)
{
    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(item_id);

    if (!itemProto)
        return 0;

    // item must have one from this field values not null if it can have random enchantments
    if ((!itemProto->RandomProperty) && (!itemProto->RandomSuffix))
        return 0;

    // item can have not null only one from field values
    if ((itemProto->RandomProperty) && (itemProto->RandomSuffix))
    {
        sLog->outError(LOG_FILTER_SQL, "Item template %u have RandomProperty == %u and RandomSuffix == %u, but must have one from field =0", itemProto->ItemId, itemProto->RandomProperty, itemProto->RandomSuffix);
        return 0;
    }

    // RandomProperty case
    if (itemProto->RandomProperty)
    {
        uint32 randomPropId = GetItemEnchantMod(itemProto->RandomProperty, ENCHANTMENT_RANDOM_PROPERTY);
        ItemRandomPropertiesEntry const* random_id = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (!random_id)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchantment id #%u used but it doesn't have records in 'ItemRandomProperties.dbc'", randomPropId);
            return 0;
        }

        return random_id->ID;
    }
    // RandomSuffix case
    else
    {
        uint32 randomPropId = GetItemEnchantMod(itemProto->RandomSuffix, ENCHANTMENT_RANDOM_SUFFIX);
        ItemRandomSuffixEntry const* random_id = sItemRandomSuffixStore.LookupEntry(randomPropId);
        if (!random_id)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchantment id #%u used but it doesn't have records in sItemRandomSuffixStore.", randomPropId);
            return 0;
        }

        return -int32(random_id->ID);
    }
}

void Item::SetItemRandomProperties(int32 randomPropId)
{
    if (!randomPropId)
        return;

    if (randomPropId > 0)
    {
        ItemRandomPropertiesEntry const* item_rand = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (item_rand)
        {
            if (GetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID) != int32(item_rand->ID))
            {
                SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, item_rand->ID);
                SetState(ITEM_CHANGED, GetOwner());
            }
            for (uint32 i = PROP_ENCHANTMENT_SLOT_1; i < PROP_ENCHANTMENT_SLOT_1 + 3; ++i)
                SetEnchantment(EnchantmentSlot(i), item_rand->enchant_id[i - PROP_ENCHANTMENT_SLOT_1], 0, 0);
        }
    }
    else
    {
        ItemRandomSuffixEntry const* item_rand = sItemRandomSuffixStore.LookupEntry(-randomPropId);
        if (item_rand)
        {
            if (GetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID) != -int32(item_rand->ID) ||
                !GetItemSuffixFactor())
            {
                SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, -int32(item_rand->ID));
                UpdateItemSuffixFactor();
                SetState(ITEM_CHANGED, GetOwner());
            }

            for (uint32 i = PROP_ENCHANTMENT_SLOT_0; i <= PROP_ENCHANTMENT_SLOT_4; ++i)
                SetEnchantment(EnchantmentSlot(i), item_rand->enchant_id[i - PROP_ENCHANTMENT_SLOT_0], 0, 0);
        }
    }
}

void Item::UpdateItemSuffixFactor()
{
    uint32 suffixFactor = GenerateEnchSuffixFactor(GetEntry());
    if (GetItemSuffixFactor() == suffixFactor)
        return;
    SetUInt32Value(ITEM_FIELD_PROPERTY_SEED, suffixFactor);
}

void Item::SetState(ItemUpdateState state, Player* forplayer)
{
    if (uState == ITEM_NEW && state == ITEM_REMOVED)
    {
        // pretend the item never existed
        RemoveFromUpdateQueueOf(forplayer);
        forplayer->DeleteRefundReference(GetGUIDLow());
        delete this;
        return;
    }
    if (state != ITEM_UNCHANGED)
    {
        // new items must stay in new state until saved
        if (uState != ITEM_NEW)
            uState = state;

        AddToUpdateQueueOf(forplayer);
    }
    else
    {
        // unset in queue
        // the item must be removed from the queue manually
        uQueuePos = -1;
        uState = ITEM_UNCHANGED;
    }
}

void Item::AddToUpdateQueueOf(Player* player)
{
    if (IsInUpdateQueue())
        return;

    ASSERT(player != NULL);

    if (player->GetGUID() != GetOwnerGUID())
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::AddToUpdateQueueOf - Owner's guid (%u) and player's guid (%u) don't match!", GUID_LOPART(GetOwnerGUID()), player->GetGUIDLow());
        return;
    }

    if (player->m_itemUpdateQueueBlocked)
        return;

    player->m_itemUpdateQueue.push_back(this);
    uQueuePos = player->m_itemUpdateQueue.size()-1;
}

void Item::RemoveFromUpdateQueueOf(Player* player)
{
    if (!IsInUpdateQueue())
        return;

    //ASSERT(player != NULL)

    if (player && player->GetGUID() != GetOwnerGUID())
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::RemoveFromUpdateQueueOf - Owner's guid (%u) and player's guid (%u) don't match!", GUID_LOPART(GetOwnerGUID()), player->GetGUIDLow());
        return;
    }

    if (player && player->m_itemUpdateQueueBlocked)
        return;

    if (player)
        player->m_itemUpdateQueue[uQueuePos] = NULL;

    uQueuePos = -1;
}

uint8 Item::GetBagSlot() const
{
    return m_container ? m_container->GetSlot() : uint8(INVENTORY_SLOT_BAG_0);
}

bool Item::IsEquipped() const
{
    return !IsInBag() && m_slot < EQUIPMENT_SLOT_END;
}

bool Item::CanBeTraded(bool mail, bool trade) const
{
    if (m_lootGenerated)
        return false;

    if ((!mail || !IsBoundAccountWide()) && (IsSoulBound() && (!HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_BOP_TRADEABLE) || !trade)))
        return false;

    if (IsBag() && (Player::IsBagPos(GetPos()) || !((Bag const*)this)->IsEmpty()))
        return false;

    if (Player* owner = GetOwner())
    {
        if (owner->CanUnequipItem(GetPos(), false) != EQUIP_ERR_OK)
            return false;
        if (owner->GetLootGUID() == GetGUID())
            return false;
    }

    if (IsBoundByEnchant())
        return false;

    return true;
}

bool Item::HasEnchantRequiredSkill(const Player* player) const
{
    // Check all enchants for required skill
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > ENGINEERING_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->requiredSkill && player->GetSkillValue(enchantEntry->requiredSkill) < enchantEntry->requiredSkillValue)
                    return false;
    }

  return true;
}

uint32 Item::GetEnchantRequiredLevel() const
{
    uint32 level = 0;

    // Check all enchants for required level
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > ENGINEERING_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->MinLevel > level)
                    level = enchantEntry->MinLevel;
    }

    return level;
}

bool Item::IsBoundByEnchant() const
{
    // Check all enchants for soulbound
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > ENGINEERING_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->slot & ENCHANTMENT_CAN_SOULBOUND)
                    return true;
    }

    return false;
}

InventoryResult Item::CanBeMergedPartlyWith(ItemTemplate const* proto) const
{
    // not allow merge looting currently items
    if (m_lootGenerated)
        return EQUIP_ERR_LOOT_GONE;

    // check item type
    if (GetEntry() != proto->ItemId)
        return EQUIP_ERR_CANT_STACK;

    // check free space (full stacks can't be target of merge
    if (GetCount() >= proto->GetMaxStackSize())
        return EQUIP_ERR_CANT_STACK;

    return EQUIP_ERR_OK;
}

bool Item::IsFitToSpellRequirements(SpellInfo const* spellInfo) const
{
    ItemTemplate const* proto = GetTemplate();

    if (spellInfo->EquippedItemClass != -1)                 // -1 == any item class
    {
        // Special case - accept vellum for armor/weapon requirements
        if ((spellInfo->EquippedItemClass == ITEM_CLASS_ARMOR ||
            spellInfo->EquippedItemClass == ITEM_CLASS_WEAPON) && proto->IsVellum())
            if (spellInfo->IsAbilityOfSkillType(SKILL_ENCHANTING)) // only for enchanting spells
                return true;

        if (spellInfo->EquippedItemClass != int32(proto->Class))
            return false;                                   //  wrong item class

        if (spellInfo->EquippedItemSubClassMask != 0)        // 0 == any subclass
        {
            if ((spellInfo->EquippedItemSubClassMask & (1 << proto->SubClass)) == 0)
                return false;                               // subclass not present in mask
        }
    }

    if (spellInfo->EquippedItemInventoryTypeMask != 0)       // 0 == any inventory type
    {
        // Special case - accept weapon type for main and offhand requirements
        if (proto->InventoryType == INVTYPE_WEAPON &&
            (spellInfo->EquippedItemInventoryTypeMask & (1 << INVTYPE_WEAPONMAINHAND) ||
             spellInfo->EquippedItemInventoryTypeMask & (1 << INVTYPE_WEAPONOFFHAND)))
        {

            // Single-Minded Fury
            // TODO: needs to be fixed correct way
            //if (spellInfo->Id == 81099)
            //    return false;

            return true;
        }
        else if ((spellInfo->EquippedItemInventoryTypeMask & (1 << proto->InventoryType)) == 0 && spellInfo->Id != 100130) // Hack Fix Wild Strikes
            return false;                                   // inventory type not present in mask
    }

    return true;
}

void Item::SetEnchantment(EnchantmentSlot slot, uint32 id, uint32 duration, uint32 charges)
{
    // Better lost small time at check in comparison lost time at item save to DB.
    if ((GetEnchantmentId(slot) == id) && (GetEnchantmentDuration(slot) == duration) && (GetEnchantmentCharges(slot) == charges))
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_ID_OFFSET, id);
    SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET, duration);
    SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET, charges);
    SetState(ITEM_CHANGED, GetOwner());
}

void Item::SetEnchantmentDuration(EnchantmentSlot slot, uint32 duration, Player* owner)
{
    if (GetEnchantmentDuration(slot) == duration)
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET, duration);
    SetState(ITEM_CHANGED, owner);
    // Cannot use GetOwner() here, has to be passed as an argument to avoid freeze due to hashtable locking
}

void Item::SetEnchantmentCharges(EnchantmentSlot slot, uint32 charges)
{
    if (GetEnchantmentCharges(slot) == charges)
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET, charges);
    SetState(ITEM_CHANGED, GetOwner());
}

void Item::ClearEnchantment(EnchantmentSlot slot)
{
    if (!GetEnchantmentId(slot))
        return;

    for (uint8 x = 0; x < MAX_ITEM_ENCHANTMENT_EFFECTS; ++x)
        SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + slot*MAX_ENCHANTMENT_OFFSET + x, 0);
    SetState(ITEM_CHANGED, GetOwner());
}

bool Item::GemsFitSockets() const
{
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint8 SocketColor = GetTemplate()->Socket[enchant_slot-SOCK_ENCHANTMENT_SLOT].Color;

        if (!SocketColor) // no socket slot
            continue;

        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id) // no gems on this socket
            return false;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry) // invalid gem id on this socket
            return false;

        uint8 GemColor = 0;

        uint32 gemid = enchantEntry->GemID;
        if (gemid)
        {
            ItemTemplate const* gemProto = sObjectMgr->GetItemTemplate(gemid);
            if (gemProto)
            {
                GemPropertiesEntry const* gemProperty = sGemPropertiesStore.LookupEntry(gemProto->GemProperties);
                if (gemProperty)
                    GemColor = gemProperty->color;
            }
        }

        if (!(GemColor & SocketColor)) // bad gem color on this socket
            return false;
    }
    return true;
}

uint8 Item::GetGemCountWithID(uint32 GemID) const
{
    uint8 count = 0;
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
            continue;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
            continue;

        if (GemID == enchantEntry->GemID)
            ++count;
    }
    return count;
}

uint8 Item::GetGemCountWithLimitCategory(uint32 limitCategory) const
{
    uint8 count = 0;
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
            continue;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
            continue;

        ItemTemplate const* gemProto = sObjectMgr->GetItemTemplate(enchantEntry->GemID);
        if (!gemProto)
            continue;

        if (gemProto->ItemLimitCategory == limitCategory)
            ++count;
    }
    return count;
}

bool Item::IsLimitedToAnotherMapOrZone(uint32 cur_mapId, uint32 cur_zoneId) const
{
    ItemTemplate const* proto = GetTemplate();
    return proto && ((proto->Map && proto->Map != cur_mapId) || (proto->Area && proto->Area != cur_zoneId));
}

// Though the client has the information in the item's data field,
// we have to send SMSG_ITEM_TIME_UPDATE to display the remaining
// time.
void Item::SendTimeUpdate(Player* owner)
{
    uint32 duration = GetUInt32Value(ITEM_FIELD_DURATION);
    if (!duration)
        return;

    WorldPacket data(SMSG_ITEM_TIME_UPDATE, (8+4));
    ObjectGuid guid = GetGUID();

    uint8 bits[8] = { 4, 6, 5, 2, 7, 0, 1, 3 };
    data.WriteBitInOrder(guid, bits);

    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[5]);
    data << uint32(duration);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[0]);

    owner->GetSession()->SendPacket(&data);
}

Item* Item::CreateItem(uint32 item, uint32 count, Player const* player)
{
    if (count < 1)
        return NULL;                                        //don't create item at zero count

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item);
    if (pProto)
    {
        if (count > pProto->GetMaxStackSize())
            count = pProto->GetMaxStackSize();

        ASSERT(count != 0 && "pProto->Stackable == 0 but checked at loading already");

        Item* pItem = NewItemOrBag(pProto);
        if (pItem->Create(sObjectMgr->GetGenerator<HighGuid::Item>().Generate(), item, player))
        {
            pItem->SetCount(count);
            return pItem;
        }
        else
            delete pItem;
    }
    else
        ASSERT(false);
    return NULL;
}

Item* Item::CloneItem(uint32 count, Player const* player) const
{
    Item* newItem = CreateItem(GetEntry(), count, player);
    if (!newItem)
        return NULL;

    newItem->SetUInt32Value(ITEM_FIELD_CREATOR,      GetUInt32Value(ITEM_FIELD_CREATOR));
    newItem->SetUInt32Value(ITEM_FIELD_GIFTCREATOR,  GetUInt32Value(ITEM_FIELD_GIFTCREATOR));
    newItem->SetUInt32Value(ITEM_FIELD_FLAGS,        GetUInt32Value(ITEM_FIELD_FLAGS) & ~(ITEM_FLAG_REFUNDABLE | ITEM_FLAG_BOP_TRADEABLE));
    newItem->SetUInt32Value(ITEM_FIELD_DURATION,     GetUInt32Value(ITEM_FIELD_DURATION));

    // player CAN be NULL in which case we must not update random properties because that accesses player's item update queue
    if (player && player->IsInWorld())
        newItem->SetItemRandomProperties(GetItemRandomPropertyId());
    return newItem;
}

bool Item::IsBindedNotWith(Player const* player) const
{
    // not binded item
    if (!IsSoulBound())
        return false;

    // own item
    if (GetOwnerGUID() == player->GetGUID())
        return false;

    if (HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_BOP_TRADEABLE))
        if (allowedGUIDs.find(player->GetGUIDLow()) != allowedGUIDs.end())
            return false;

    // BOA item case
    if (IsBoundAccountWide())
        return false;

    return true;
}

void Item::BuildUpdate(UpdateDataMapType& data_map)
{
    if (Player* owner = GetOwner())
        BuildFieldsUpdate(owner, data_map);
    ClearUpdateMask(false);
}

void Item::AddToObjectUpdate()
{
    if (Player* owner = GetOwner())
        owner->GetMap()->AddUpdateObject(this);
}

void Item::RemoveFromObjectUpdate()
{
    if (Player* owner = GetOwner())
        owner->GetMap()->RemoveUpdateObject(this);
}

void Item::SaveRefundDataToDB()
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
    stmt->setUInt32(0, GetGUIDLow());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ITEM_REFUND_INSTANCE);
    stmt->setUInt32(0, GetGUIDLow());
    stmt->setUInt32(1, GetRefundRecipient());
    stmt->setUInt32(2, GetPaidMoney());
    stmt->setUInt16(3, uint16(GetPaidExtendedCost()));
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

void Item::DeleteRefundDataFromDB(SQLTransaction* trans)
{
    if (trans && !trans->null())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
        stmt->setUInt32(0, GetGUIDLow());
        (*trans)->Append(stmt);

    }
}

void Item::SetNotRefundable(Player* owner, bool changestate /*=true*/, SQLTransaction* trans /*=NULL*/)
{
    if (!HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_REFUNDABLE))
        return;

    RemoveFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_REFUNDABLE);
    // Following is not applicable in the trading procedure
    if (changestate)
        SetState(ITEM_CHANGED, owner);

    SetRefundRecipient(0);
    SetPaidMoney(0);
    SetPaidExtendedCost(0);
    DeleteRefundDataFromDB(trans);

    owner->DeleteRefundReference(GetGUIDLow());
}

void Item::UpdatePlayedTime(Player* owner)
{
    /*  Here we update our played time
        We simply add a number to the current played time,
        based on the time elapsed since the last update hereof.
    */
    // Get current played time
    uint32 current_playtime = GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME);
    // Calculate time elapsed since last played time update
    time_t curtime = time(NULL);
    uint32 elapsed = uint32(curtime - m_lastPlayedTimeUpdate);
    uint32 new_playtime = current_playtime + elapsed;
    // Check if the refund timer has expired yet
    if (new_playtime <= 2*HOUR)
    {
        // No? Proceed.
        // Update the data field
        SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, new_playtime);
        // Flag as changed to get saved to DB
        SetState(ITEM_CHANGED, owner);
        // Speaks for itself
        m_lastPlayedTimeUpdate = curtime;
        return;
    }
    // Yes
    SetNotRefundable(owner);
}

uint32 Item::GetPlayedTime()
{
    time_t curtime = time(NULL);
    uint32 elapsed = uint32(curtime - m_lastPlayedTimeUpdate);
    return GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME) + elapsed;
}

bool Item::IsRefundExpired()
{
    return (GetPlayedTime() > 2*HOUR);
}

void Item::SetSoulboundTradeable(AllowedLooterSet& allowedLooters)
{
    SetFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_BOP_TRADEABLE);
    allowedGUIDs = allowedLooters;
}

void Item::ClearSoulboundTradeable(Player* currentOwner)
{
    RemoveFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_BOP_TRADEABLE);
    if (allowedGUIDs.empty())
        return;

    allowedGUIDs.clear();
    SetState(ITEM_CHANGED, currentOwner);
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_BOP_TRADE);
    stmt->setUInt32(0, GetGUIDLow());
    CharacterDatabase.Execute(stmt);
}

bool Item::CheckSoulboundTradeExpire()
{
    // called from owner's update - GetOwner() MUST be valid
    if (GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME) + 2*HOUR < GetOwner()->GetTotalPlayedTime())
    {
        ClearSoulboundTradeable(GetOwner());
        return true; // remove from tradeable list
    }

    return false;
}

bool Item::CanBeTransmogrified() const
{
    ItemTemplate const* proto = GetTemplate();

    if (!proto)
        return false;

    if (proto->Quality >= ITEM_QUALITY_LEGENDARY)
        return false;

    if (proto->Class != ITEM_CLASS_ARMOR &&
        proto->Class != ITEM_CLASS_WEAPON)
        return false;

    if (proto->Class == ITEM_CLASS_WEAPON && proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_CANNOT_BE_TRANSMOG)
        return false;

    return true;
}

bool Item::CanTransmogrify() const
{
    ItemTemplate const* proto = GetTemplate();

    if (!proto)
        return false;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_CANNOT_TRANSMOG)
        return false;

    if (proto->Quality >= ITEM_QUALITY_LEGENDARY)
        return false;

    if (proto->Class != ITEM_CLASS_ARMOR &&
        proto->Class != ITEM_CLASS_WEAPON)
        return false;

    if (proto->Class == ITEM_CLASS_WEAPON && proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_CAN_TRANSMOG)
        return true;

    return true;
}

bool Item::SubclassesCompatible(ItemTemplate const* proto1, ItemTemplate const* proto2) const
{
    //   Source     Destination
    if (!proto1 || !proto2)
        return false;

    // Patch 5.2 - Throne of Thunder
    // One-Handed
    // One-handed axes, maces, and swords can be Transmogrified to each other.
    if ((proto1->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_SWORD) &&
        (proto2->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_SWORD))
        return true;

    // Two-Handed
    // Two-handed axes, maces, and swords can be Transmogrified to each other.
    if ((proto1->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2) &&
        (proto2->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2))
        return true;

    // Ranged
    if ((proto1->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW) &&
        (proto2->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW))
        return true;

    // Polearm and Staff
    // Staves and polearms can be transmogrified to each other.
    if ((proto1->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_STAFF) &&
        (proto2->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_STAFF))
        return true;

    return false;
}

bool Item::CanTransmogrifyItemWithItem(Item* transmogrified, Item* transmogrifier)
{
    if (!transmogrifier || !transmogrified)
        return false;

    ItemTemplate const* proto1 = transmogrifier->GetTemplate(); // source
    ItemTemplate const* proto2 = transmogrified->GetTemplate(); // dest

    if (proto1->ItemId == proto2->ItemId)
        return false;

    if (!transmogrifier->CanTransmogrify() || !transmogrified->CanBeTransmogrified())
        return false;

    if (proto1->InventoryType == INVTYPE_BAG ||
        proto1->InventoryType == INVTYPE_RELIC ||
        proto1->InventoryType == INVTYPE_BODY ||
        proto1->InventoryType == INVTYPE_FINGER ||
        proto1->InventoryType == INVTYPE_TRINKET ||
        proto1->InventoryType == INVTYPE_AMMO ||
        proto1->InventoryType == INVTYPE_QUIVER ||
        proto1->InventoryType == INVTYPE_NON_EQUIP ||
        proto1->InventoryType == INVTYPE_TABARD)
        return false;

    if (proto2->InventoryType == INVTYPE_BAG ||
        proto2->InventoryType == INVTYPE_RELIC ||
        proto2->InventoryType == INVTYPE_BODY ||
        proto2->InventoryType == INVTYPE_FINGER ||
        proto2->InventoryType == INVTYPE_TRINKET ||
        proto2->InventoryType == INVTYPE_AMMO ||
        proto2->InventoryType == INVTYPE_QUIVER ||
        proto2->InventoryType == INVTYPE_NON_EQUIP ||
        proto2->InventoryType == INVTYPE_TABARD)
        return false;

    if (proto1->Class != proto2->Class)
        return false;

    if (proto1->SubClass != ITEM_SUBCLASS_ARMOR_COSMETIC && (proto1->Class != ITEM_CLASS_WEAPON || !proto2->IsRangedWeapon() || !proto1->IsRangedWeapon()) &&
        (proto1->SubClass != proto2->SubClass && !transmogrifier->SubclassesCompatible(proto1, proto2)))
        return false;

    if (proto1->InventoryType != proto2->InventoryType)
    {
        if (proto1->Class == ITEM_CLASS_WEAPON && proto2->Class == ITEM_CLASS_WEAPON)
        {
            if (!((proto1->InventoryType == INVTYPE_WEAPON || proto1->InventoryType == INVTYPE_WEAPONMAINHAND || proto1->InventoryType == INVTYPE_WEAPONOFFHAND) &&
                (proto2->InventoryType == INVTYPE_WEAPON || proto2->InventoryType == INVTYPE_WEAPONMAINHAND ||proto2->InventoryType == INVTYPE_WEAPONOFFHAND)))
                return false;
        }
        else if (proto2->Class == ITEM_CLASS_ARMOR && proto2->Class == ITEM_CLASS_ARMOR)
        {
            if (!((proto1->InventoryType == INVTYPE_CHEST || proto1->InventoryType == INVTYPE_ROBE) &&
                (proto2->InventoryType == INVTYPE_CHEST || proto2->InventoryType == INVTYPE_ROBE)))
                return false;
        }
    }

    // Check armor types
    if (proto1->SubClass != ITEM_SUBCLASS_ARMOR_COSMETIC && (proto1->Class == ITEM_CLASS_ARMOR || proto2->Class == ITEM_CLASS_ARMOR))
    {
        uint32 skill1 = proto1->GetSkill();
        uint32 skill2 = proto2->GetSkill();

        if ((skill1 == SKILL_PLATE_MAIL || skill1 == SKILL_LEATHER ||
            skill1 == SKILL_MAIL || skill1 == SKILL_CLOTH) ||
            (skill2 == SKILL_PLATE_MAIL || skill2 == SKILL_LEATHER ||
            skill2 == SKILL_MAIL || skill2 == SKILL_CLOTH))
            if (skill1 != skill2)
                return false;
    }

    return true;
}

bool Item::HasStats() const
{
    if (GetItemRandomPropertyId() != 0)
        return true;

    ItemTemplate const* proto = GetTemplate();
    for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (proto->ItemStat[i].ItemStatValue != 0)
            return true;
    
    // Some trinket like http://www.wowhead.com/item=102293/purified-bindings-of-immerseus should be upgradeable even if has no stats
    if (proto->InventoryType == INVTYPE_TRINKET)
        return true;

    return false;
}

bool Item::HasSpells() const
{
    ItemTemplate const* proto = GetTemplate();
    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        if (proto->Spells[i].SpellId != 0)
            return true;

    return false;
}

// used by mail items, transmog cost, stationeryinfo and others
uint32 Item::GetSellPrice(ItemTemplate const* proto, bool& normalSellPrice)
{
    normalSellPrice = true;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_HAS_NORMAL_PRICE)
    {
        return proto->BuyPrice;
    }
    else
    {
        ImportPriceQualityEntry const* qualityPrice = sImportPriceQualityStore.LookupEntry(proto->Quality + 1);
        ItemPriceBaseEntry const* basePrice = sItemPriceBaseStore.LookupEntry(proto->ItemLevel);

        if (!qualityPrice || !basePrice)
            return 0;

        float qualityFactor = qualityPrice->Factor;
        float baseFactor = 0.0f;

        uint32 inventoryType = proto->InventoryType;

        if (inventoryType == INVTYPE_WEAPON ||
            inventoryType == INVTYPE_2HWEAPON ||
            inventoryType == INVTYPE_WEAPONMAINHAND ||
            inventoryType == INVTYPE_WEAPONOFFHAND ||
            inventoryType == INVTYPE_RANGED ||
            inventoryType == INVTYPE_THROWN ||
            inventoryType == INVTYPE_RANGEDRIGHT)
            baseFactor = basePrice->WeaponFactor;
        else
            baseFactor = basePrice->ArmorFactor;

        if (inventoryType == INVTYPE_ROBE)
            inventoryType = INVTYPE_CHEST;

        float typeFactor = 0.0f;
        uint8 wepType = -1;

        switch (inventoryType)
        {
            case INVTYPE_HEAD:
            case INVTYPE_SHOULDERS:
            case INVTYPE_CHEST:
            case INVTYPE_WAIST:
            case INVTYPE_LEGS:
            case INVTYPE_FEET:
            case INVTYPE_WRISTS:
            case INVTYPE_HANDS:
            case INVTYPE_CLOAK:
            {
                ImportPriceArmorEntry const* armorPrice = sImportPriceArmorStore.LookupEntry(inventoryType);
                if (!armorPrice)
                    return 0;

                switch (proto->SubClass)
                {
                    case ITEM_SUBCLASS_ARMOR_MISCELLANEOUS:
                    case ITEM_SUBCLASS_ARMOR_CLOTH:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_LEATHER:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_MAIL:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_PLATE:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    default:
                    {
                        return 0;
                    }
                }

                break;
            }
            case INVTYPE_SHIELD:
            {
                ImportPriceShieldEntry const* shieldPrice = sImportPriceShieldStore.LookupEntry(1); // it only has two rows, it's unclear which is the one used
                if (!shieldPrice)
                    return 0;

                typeFactor = shieldPrice->Factor;
                break;
            }
            case INVTYPE_WEAPONMAINHAND:
                wepType = 0;             // unk enum, fall back
            case INVTYPE_WEAPONOFFHAND:
                wepType = 1;             // unk enum, fall back
            case INVTYPE_WEAPON:
                wepType = 2;             // unk enum, fall back
            case INVTYPE_2HWEAPON:
                wepType = 3;             // unk enum, fall back
            case INVTYPE_RANGED:
            case INVTYPE_RANGEDRIGHT:
            case INVTYPE_RELIC:
            {
                wepType = 4;             // unk enum

                ImportPriceWeaponEntry const* weaponPrice = sImportPriceWeaponStore.LookupEntry(wepType + 1);
                if (!weaponPrice)
                    return 0;

                typeFactor = weaponPrice->Factor;
                break;
            }
            default:
                return proto->BuyPrice;
        }

        normalSellPrice = false;
        return (uint32)(qualityFactor * proto->Unk430_2 * proto->Unk430_1 * typeFactor * baseFactor);
    }
}

uint32 Item::GetSpecialPrice(ItemTemplate const* proto, uint32 minimumPrice /*= 10000*/)
{
    uint32 cost = 0;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_HAS_NORMAL_PRICE)
        cost = proto->SellPrice;
    else
    {
        bool normalPrice;
        cost = Item::GetSellPrice(proto, normalPrice);

        if (!normalPrice)
        {
            if (proto->BuyCount <= 1)
            {
                ItemClassEntry const* classEntry = sItemClassStore.LookupEntry(proto->Class);
                if (classEntry)
                    cost *= classEntry->PriceFactor;
                else
                    cost = 0;
            }
            else
                cost /= 4 * proto->BuyCount;
        }
        else
            cost = proto->SellPrice;
    }

    if (cost < minimumPrice)
        cost = minimumPrice;

    return cost;
}

int32 Item::GetReforgableStat(ItemModType statType, Player* player, uint32 ilvl) const
{
    ItemTemplate const* proto = GetTemplate();
    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (proto->ItemStat[i].ItemStatType == statType)
            if (player)
                return proto->CalculateStatScaling(i, ilvl ? ilvl : player->GetEquipItemLevelFor(proto, this));
            else
                return proto->ItemStat[i].ItemStatValue;

    int32 randomPropId = GetItemRandomPropertyId();
    if (!randomPropId)
        return 0;

    if (randomPropId < 0)
    {
        ItemRandomSuffixEntry const* randomSuffix = sItemRandomSuffixStore.LookupEntry(-randomPropId);
        if (!randomSuffix)
            return 0;

        for (uint32 e = PROP_ENCHANTMENT_SLOT_0; e <= PROP_ENCHANTMENT_SLOT_4; ++e)
            if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(GetEnchantmentId(EnchantmentSlot(e))))
                for (uint32 f = 0; f < MAX_ITEM_ENCHANTMENT_EFFECTS; ++f)
                    if (enchant->type[f] == ITEM_ENCHANTMENT_TYPE_STAT && enchant->spellid[f] == statType)
                        for (int k = 0; k < 5; ++k)
                            if (randomSuffix->enchant_id[k] == enchant->ID)
                                return int32((randomSuffix->prefix[k] * GetItemSuffixFactor()) / 10000);
    }
    else
    {
        ItemRandomPropertiesEntry const* randomProp = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (!randomProp)
            return 0;

        for (uint32 e = PROP_ENCHANTMENT_SLOT_0; e <= PROP_ENCHANTMENT_SLOT_4; ++e)
            if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(GetEnchantmentId(EnchantmentSlot(e))))
                for (uint32 f = 0; f < MAX_ITEM_ENCHANTMENT_EFFECTS; ++f)
                    if (enchant->type[f] == ITEM_ENCHANTMENT_TYPE_STAT && enchant->spellid[f] == statType)
                        for (int k = 0; k < MAX_ITEM_ENCHANTMENT_EFFECTS; ++k)
                            if (randomProp->enchant_id[k] == enchant->ID)
                                return int32(enchant->amount[k]);
    }

    return 0;
}

bool Item::IsPotion() const
{
    switch (GetEntry())
    {
        case 86125: ///< Kafa Press
            return true;
    }

    return GetTemplate()->IsPotion();
}

bool Item::IsPvPItem() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
    {
        uint32 stat = proto->ItemStat[i].ItemStatType;
        if (stat == ITEM_MOD_PVP_POWER || stat == ITEM_MOD_RESILIENCE_RATING)
            return true;
    }

    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
    {
        int32 spell = proto->Spells[i].SpellId;
        if (spell == 132586 || spell == 139891)
            return true;
    }

    return false;
}

bool Item::IsStuffItem() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    uint32 invType = proto->InventoryType;

    switch (invType)
    {
        case INVTYPE_NON_EQUIP:
        case INVTYPE_BODY:
        case INVTYPE_BAG:
        case INVTYPE_TABARD:
        case INVTYPE_AMMO:
        case INVTYPE_THROWN:
        case INVTYPE_QUIVER:
        case INVTYPE_RELIC:
            return false;
        default:
            return true;
    }

    return false;
}

bool Item::IsLegendaryCloak() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    switch (proto->ItemId)
    {
        case 102245: // Qian-Le, Courage of Niuzao
        case 102246: // Xing-Ho, Breath of Yu'lon
        case 102247: // Jina-Kang, Kindness of Chi-Ji
        case 102248: // Fen-Yu, Fury of Xuen
        case 102249: // Gong-Lu, Strength of Xuen
        case 102250: // Qian-Ying, Fortitude of Niuzao
            return true;
        default:
            break;
    }

    return false;
}

void Item::BuildItemModifiers(ByteBuffer* data) const
{
    size_t position = data->wpos();

    *data << uint32(0);
    
    uint32 mask = 0;
    for (uint8 i = 0; i < 8; ++i)
    {
        uint32 modifier = GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, i);
        if (modifier == 0)
            continue;

        mask |= 1 << i;
    }

    if (mask == 0)
        return;

    *data << uint32(mask);

    uint8 count = 0;
    for (uint8 i = 0; i < 8; ++i)
    {
        uint32 modifier = GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, i);
        if (modifier == 0)
            continue;

        *data << uint32(modifier);

        ++count;
    }

    data->put(position, uint32(count * sizeof(uint32) + sizeof(uint32)));
}

float ItemTemplate::GetScalingDamageValue(uint32 ilvl) const
{
    if (Quality > ITEM_QUALITY_HEIRLOOM)
        return 0.f;

    ItemDamageEntry const* damageEntry = NULL;

    switch (InventoryType)
    {
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            if (Flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
            {
                damageEntry =  sItemDamageOneHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageOneHandStore.LookupEntry(ilvl);
            break;
    case INVTYPE_RANGED:
    case INVTYPE_THROWN:
    case INVTYPE_RANGEDRIGHT:
        if (SubClass < 4)
        {
            if (Flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
            {
                damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
            break;
        }
        else if (SubClass == 19)
        {
            damageEntry = sItemDamageOneHandCasterStore.LookupEntry(ilvl);
            break;
        }
        else
        {
            if (Flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
            {
                damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
            break;
        }
    case INVTYPE_AMMO:
        damageEntry = sItemDamageAmmoStore.LookupEntry(ilvl);
        break;
    case INVTYPE_2HWEAPON:
        if (Flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
        {
            damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
            break;
        }
        damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
        break;
    default:
        break;
    }
    return damageEntry ? damageEntry->DPS[Quality == ITEM_QUALITY_HEIRLOOM ? ITEM_QUALITY_RARE : Quality] : 0.f;
}

int32 ItemTemplate::GetRandomPointsOffset() const
{
    switch (InventoryType)
    {
        case INVTYPE_NECK:
        case INVTYPE_WRISTS:
        case INVTYPE_FINGER:
        case INVTYPE_SHIELD:
        case INVTYPE_CLOAK:
        case INVTYPE_HOLDABLE:
            return 2;
        case INVTYPE_SHOULDERS:
        case INVTYPE_WAIST:
        case INVTYPE_FEET:
        case INVTYPE_HANDS:
        case INVTYPE_TRINKET:
            return 1;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            return 3;
        case INVTYPE_RANGEDRIGHT:
            return 3 * (SubClass == 19 ? 1 : 0);
        case INVTYPE_RELIC:
            return 4;
        case INVTYPE_HEAD:
        case INVTYPE_BODY:
        case INVTYPE_CHEST:
        case INVTYPE_LEGS:
        case INVTYPE_RANGED:
        case INVTYPE_2HWEAPON:
        case INVTYPE_ROBE:
        case INVTYPE_THROWN:
            return 0;
        default:
            return -1;
    }
}

uint32 ItemTemplate::CalculateScalingStatDBCValue(uint32 ilvl) const
{
    int32 offset = GetRandomPointsOffset();
    if (offset == -1)
        return 0;

    RandomPropertiesPointsEntry const* randProperty = sRandomPropertiesPointsStore.LookupEntry(ilvl);

    if (!randProperty)
        return 0;

    switch (Quality)
    {
        case ITEM_QUALITY_UNCOMMON:
        return randProperty->UncommonPropertiesPoints[offset];
        case ITEM_QUALITY_RARE:
        case ITEM_QUALITY_HEIRLOOM:
            return randProperty->RarePropertiesPoints[offset];
        case ITEM_QUALITY_EPIC:
        case ITEM_QUALITY_LEGENDARY:
            return randProperty->EpicPropertiesPoints[offset];
        default:
            return 0;
    }
}

float ItemTemplate::GetSocketCost(uint32 ilvl) const
{
    gtItemSocketCostPerLevelEntry const* socket = sgtItemSocketCostPerLevelStore.LookupEntry(ilvl);
    return socket ? socket->cost : 0.f;
}

int32 ItemTemplate::CalculateStatScaling(uint32 index, uint32 ilvl) const
{
    _ItemStat const& itemStat = ItemStat[index];
    return CalculateStatScaling(itemStat.ItemScalingValue, itemStat.ItemSocketCostRate, ilvl);
}

int32 ItemTemplate::CalculateStatScaling(int32 scalingValue, float socketCost, uint32 ilvl) const
{
    return floor((((float)scalingValue * (float)CalculateScalingStatDBCValue(ilvl) * 0.000099999997f) - (GetSocketCost(ilvl) * socketCost)) + 0.5f);
}

uint32 ItemTemplate::CalculateArmorScaling(uint32 ilvl) const
{
    if (ilvl == ItemLevel)
        return Armor;

    uint32 quality = Quality == ITEM_QUALITY_HEIRLOOM ? ITEM_QUALITY_RARE : Quality;

    if (Class != ITEM_CLASS_ARMOR || SubClass != ITEM_SUBCLASS_ARMOR_SHIELD)
    {
        if (InventoryType == 1 || InventoryType == 5 || InventoryType == 3 || InventoryType == 7 || InventoryType == 8 || InventoryType == 9 || InventoryType == 10 || InventoryType == 6 || InventoryType == 16 || InventoryType == 20)
        {
            ItemArmorQualityEntry const* armorQuality = sItemArmorQualityStore.LookupEntry(ilvl);
            ItemArmorTotalEntry const* armorTotal = sItemArmorTotalStore.LookupEntry(ilvl);
            ArmorLocationEntry const* armorLoc = sArmorLocationStore.LookupEntry(InventoryType == 20 ? 5 : InventoryType);

            if (SubClass == 0 || SubClass > 4)
                return 0.0f;

            return (int)floor(armorQuality->Value[quality] * armorTotal->Value[SubClass - 1] * armorLoc->Value[SubClass - 1] + 0.5f);
        }
        return 0;
    }
    else
    {
        ItemArmorShieldEntry const* shieldEntry = sItemArmorShieldStore.LookupEntry(ilvl);
        return shieldEntry->Value[quality];
    }
}

void ItemTemplate::CalculateMinMaxDamageScaling(uint32 ilvl, uint32& minDamage, uint32& maxDamage) const
{
    minDamage = 0;
    maxDamage = 0;

    if (ilvl == ItemLevel)
    {
        minDamage = DamageMin;
        maxDamage = DamageMax;
        return;
    }

    if (!IsWeapon())
        return;

    float weaponMinDamageCalc = (float)Delay * GetScalingDamageValue(ilvl) * 0.001f;
    float weaponMaxDamageCalc = (((StatScalingFactor * 0.5f) + 1.f) * weaponMinDamageCalc) + 0.5f;

    if (Delay != 0)
    {
        float delayModifier = 1000.0f / (float)Delay;
        float midCalc = (delayModifier * ((1.f - (StatScalingFactor * 0.5f)) * weaponMinDamageCalc)) + ArmorDamageModifier;
        midCalc = midCalc > 1.f ? midCalc : 1.f;
        float delayCoeff = 1.f / delayModifier;
        minDamage = floor((delayCoeff * midCalc) + 0.5f);
        if (ItemId == 19019)
            maxDamage = abs(floor((delayCoeff * ((delayModifier * weaponMaxDamageCalc) + ArmorDamageModifier)) + 0.5f));
        else
            maxDamage = floor((delayCoeff * ((delayModifier * weaponMaxDamageCalc) + ArmorDamageModifier)) + 0.5f);
    }
    else
    {
        maxDamage = floor(weaponMaxDamageCalc + 0.5f);
        minDamage = floor(((1.f - (StatScalingFactor * 0.5f)) * weaponMinDamageCalc) + 0.5f);
    }
}

FakeResult Item::CanFakeDisplay(uint32 iEntry, bool withPay /* = false */)
{
    if (!iEntry)
        return FAKE_ERR_OK;

    ItemTemplate const* myTmpl = GetTemplate();
    ItemTemplate const* otherTmpl = sObjectMgr->GetItemTemplate(iEntry);

    if (!otherTmpl)
        return FAKE_ERR_CANT_FIND_ITEM;

    bool isWarglaive = otherTmpl->ItemId == 32838 || otherTmpl->ItemId == 32837;
    bool diff_inv_type = false;
    if (myTmpl->InventoryType != otherTmpl->InventoryType)
    {
        diff_inv_type = true;

        if (myTmpl->InventoryType == INVTYPE_CHEST && otherTmpl->InventoryType == INVTYPE_ROBE)
            diff_inv_type = false;

        if (myTmpl->InventoryType == INVTYPE_ROBE && otherTmpl->InventoryType == INVTYPE_CHEST)
            diff_inv_type = false;

        if (myTmpl->InventoryType == INVTYPE_RANGED && otherTmpl->InventoryType == INVTYPE_RANGEDRIGHT)
            diff_inv_type = false;

        if (myTmpl->InventoryType == INVTYPE_RANGEDRIGHT && otherTmpl->InventoryType == INVTYPE_RANGED)
            diff_inv_type = false;

        if ((myTmpl->InventoryType == INVTYPE_WEAPONMAINHAND || myTmpl->InventoryType == INVTYPE_WEAPONOFFHAND) && otherTmpl->InventoryType == INVTYPE_WEAPON)
            diff_inv_type = false;

        if (withPay && isWarglaive)
        {
            if ((otherTmpl->InventoryType == INVTYPE_WEAPONOFFHAND || otherTmpl->InventoryType == INVTYPE_WEAPONMAINHAND) && myTmpl->InventoryType == INVTYPE_WEAPON)
                diff_inv_type = false;
        }
    }

    if (diff_inv_type)
        return FAKE_ERR_DIFF_SLOTS;

    switch (myTmpl->InventoryType)
    {
        case INVTYPE_NON_EQUIP:
            //case INVTYPE_HEAD:
            //case INVTYPE_NECK:
            //case INVTYPE_SHOULDERS:
        case INVTYPE_BODY:
            //case INVTYPE_CHEST:
            //case INVTYPE_WAIST:
            //case INVTYPE_LEGS:
            //case INVTYPE_FEET:
            //case INVTYPE_WRISTS:
            //case INVTYPE_HANDS:
            //case INVTYPE_FINGER:
        case INVTYPE_TRINKET:
            //case INVTYPE_WEAPON:
            //case INVTYPE_SHIELD:
            //case INVTYPE_RANGED:
            //case INVTYPE_CLOAK:
            //case INVTYPE_2HWEAPON:
        case INVTYPE_BAG:
        case INVTYPE_TABARD:
            //case INVTYPE_ROBE:
            //case INVTYPE_WEAPONMAINHAND:
            //case INVTYPE_WEAPONOFFHAND:
            //case INVTYPE_HOLDABLE:
        case INVTYPE_AMMO:
        case INVTYPE_THROWN:
            //case INVTYPE_RANGEDRIGHT:
        case INVTYPE_QUIVER:
        case INVTYPE_RELIC:
            return FAKE_ERR_DIFF_SLOTS;
        default:
            break;
    }

    if (myTmpl->Class != otherTmpl->Class)
        return FAKE_ERR_DIFF_CLASS;

    bool diff_sub_class = false;
    if (myTmpl->SubClass != otherTmpl->SubClass)
    {
        diff_sub_class = true;
        if (myTmpl->Class == ITEM_CLASS_WEAPON)
        {
            //BOW & CROSSBOW
            if (myTmpl->SubClass == ITEM_SUBCLASS_WEAPON_BOW && otherTmpl->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW)
                diff_sub_class = false;

            if (myTmpl->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW && otherTmpl->SubClass == ITEM_SUBCLASS_WEAPON_BOW)
                diff_sub_class = false;

            //BOW & GUN
            if (myTmpl->SubClass == ITEM_SUBCLASS_WEAPON_BOW && otherTmpl->SubClass == ITEM_SUBCLASS_WEAPON_GUN)
                diff_sub_class = false;

            if (myTmpl->SubClass == ITEM_SUBCLASS_WEAPON_GUN && otherTmpl->SubClass == ITEM_SUBCLASS_WEAPON_BOW)
                diff_sub_class = false;

            //CROSSBOW & GUN
            if (myTmpl->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW && otherTmpl->SubClass == ITEM_SUBCLASS_WEAPON_GUN)
                diff_sub_class = false;

            if (myTmpl->SubClass == ITEM_SUBCLASS_WEAPON_GUN && otherTmpl->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW)
                diff_sub_class = false;

            bool twohandeAvailables[2] = { false, false };
            bool onehandAvailables[2] = { false, false };
            bool stavespolesAvailables[2] = { false, false };
            switch (myTmpl->SubClass)
            {
                case ITEM_SUBCLASS_WEAPON_STAFF:
                case ITEM_SUBCLASS_WEAPON_POLEARM:
                    stavespolesAvailables[0] = true;
                    break;
                case ITEM_SUBCLASS_WEAPON_SWORD2:
                case ITEM_SUBCLASS_WEAPON_AXE2:
                case ITEM_SUBCLASS_WEAPON_MACE2:
                    twohandeAvailables[0] = true;
                    break;
                case ITEM_SUBCLASS_WEAPON_SWORD:
                case ITEM_SUBCLASS_WEAPON_AXE:
                case ITEM_SUBCLASS_WEAPON_MACE:
                    onehandAvailables[0] = true;
                    break;
                default: break;
            }

            switch (otherTmpl->SubClass)
            {
                case ITEM_SUBCLASS_WEAPON_STAFF:
                case ITEM_SUBCLASS_WEAPON_POLEARM:
                    stavespolesAvailables[1] = true;
                    break;
                case ITEM_SUBCLASS_WEAPON_SWORD2:
                case ITEM_SUBCLASS_WEAPON_AXE2:
                case ITEM_SUBCLASS_WEAPON_MACE2:
                    twohandeAvailables[1] = true;
                    break;
                case ITEM_SUBCLASS_WEAPON_SWORD:
                case ITEM_SUBCLASS_WEAPON_AXE:
                case ITEM_SUBCLASS_WEAPON_MACE:
                    onehandAvailables[1] = true;
                    break;
                default: break;
            }

            if ((twohandeAvailables[0] && twohandeAvailables[1]) || (onehandAvailables[0] && onehandAvailables[1]) ||
                (stavespolesAvailables[0] && stavespolesAvailables[1]))
                diff_sub_class = false;
        }
    }

    if (diff_sub_class)
        return FAKE_ERR_DIFF_CLASS;

    if (myTmpl->Class == ITEM_CLASS_WEAPON && myTmpl->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return FAKE_ERR_DIFF_CLASS;

    if (otherTmpl->Class == ITEM_CLASS_WEAPON && otherTmpl->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return FAKE_ERR_DIFF_CLASS;

    if (otherTmpl->Quality == ITEM_QUALITY_POOR || otherTmpl->Quality == ITEM_QUALITY_NORMAL ||
        myTmpl->Quality == ITEM_QUALITY_POOR || myTmpl->Quality == ITEM_QUALITY_NORMAL)
         return FAKE_ERR_WRONG_QUALITY;

    if (!withPay)
    {
        if (otherTmpl->Quality == ITEM_QUALITY_LEGENDARY)
            return FAKE_ERR_WRONG_QUALITY;

        if (myTmpl->Quality == ITEM_QUALITY_LEGENDARY)
            return FAKE_ERR_WRONG_QUALITY;
    }

    return FAKE_ERR_OK;
}

bool Item::SubclassesCompatibleForRandomWeapon(ItemTemplate const* p_Transmogrifier, ItemTemplate const* p_Transmogrified)
{
    if (!p_Transmogrifier || !p_Transmogrified)
        return false;

    /// One-Handed can be transmogrified to each other
    if ((p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_SWORD ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_FIST_WEAPON) &&
        (p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_SWORD ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_FIST_WEAPON))
        return true;

    /// Two-Handed
    /// Two-handed axes, maces, and swords can be Transmogrified to each other.
    if ((p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2 ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_STAFF) &&
        (p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2 ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_STAFF))
        return true;

    return false;
}

bool Item::CanTransmogrifyIntoRandomWeapon(ItemTemplate const* p_Transmogrifier, ItemTemplate const* p_Transmogrified)
{
    if (!p_Transmogrified || !p_Transmogrifier)
        return false;

    if (p_Transmogrified->ItemId == p_Transmogrifier->ItemId)
        return false;

    if (p_Transmogrified->Class != ITEM_CLASS_WEAPON || p_Transmogrifier->Class != ITEM_CLASS_WEAPON)
        return false;

    if (p_Transmogrified->InventoryType != INVTYPE_2HWEAPON &&
        p_Transmogrified->InventoryType != INVTYPE_WEAPONMAINHAND &&
        p_Transmogrified->InventoryType != INVTYPE_WEAPONOFFHAND)
        return false;

    if (p_Transmogrifier->InventoryType != INVTYPE_2HWEAPON &&
        p_Transmogrifier->InventoryType != INVTYPE_WEAPONMAINHAND &&
        p_Transmogrifier->InventoryType != INVTYPE_WEAPONOFFHAND)
        return false;

    if (!ItemTemplate::CanTransmogrify(p_Transmogrifier) || !ItemTemplate::CanBeTransmogrified(p_Transmogrified))
        return false;

    if (!SubclassesCompatibleForRandomWeapon(p_Transmogrifier, p_Transmogrified))
        return false;

    return true;
}
void Item::RandomWeaponTransmogrificationFromPrimaryBag(Player* p_Player, Item* p_Transmogrified, bool p_Apply)
{
    if (!p_Player || !p_Transmogrified)
        return;

    uint8 l_TransmogrifiedItemSlot = p_Transmogrified->GetSlot();
    /// Wrong transmogrified item, we can change just weapons
    if (l_TransmogrifiedItemSlot != EQUIPMENT_SLOT_MAINHAND && l_TransmogrifiedItemSlot != EQUIPMENT_SLOT_OFFHAND)
        return;

    /// Apply transmogrification on weapon
    if (p_Apply)
    {
        /// Find item template of Transmogrified weapon
        ItemTemplate const* l_TransmogrifiedTemplate = p_Transmogrified->GetTemplate();
        if (!l_TransmogrifiedTemplate)
            return;

        /// Select random weapon from primary bag
        uint32 l_RandomWeaponId = p_Player->GetRandomWeaponFromPrimaryBag(l_TransmogrifiedTemplate);

        /// Find item template of Transmogrifier weapon
        ItemTemplate const* l_TransmogrifierTemplate = sObjectMgr->GetItemTemplate(l_RandomWeaponId);
        if (!l_TransmogrifierTemplate)
            return;

        /// Find item in player inventory
        Item* l_Transmogrifier = p_Player->GetItemByEntry(l_TransmogrifiedTemplate->ItemId);
        if (!l_Transmogrifier)
            return;

        /// Apply transmogrification on weapon
        p_Transmogrified->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1, l_TransmogrifierTemplate->ItemId);
        p_Transmogrified->SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x3);
        p_Player->SetVisibleItemSlot(l_TransmogrifiedItemSlot, p_Transmogrified);

        p_Transmogrified->UpdatePlayedTime(p_Player);

        p_Transmogrified->SetOwnerGUID(p_Player->GetGUID());
        p_Transmogrified->SetNotRefundable(p_Player);
        p_Transmogrified->ClearSoulboundTradeable(p_Player);

        /// Some rules for Transmogrifier weapon
        if (l_Transmogrifier != nullptr)
        {
            if (l_TransmogrifierTemplate->Bonding == BIND_WHEN_EQUIPED || l_TransmogrifierTemplate->Bonding == BIND_WHEN_USE)
                l_Transmogrifier->SetBinding(true);

            l_Transmogrifier->SetOwnerGUID(p_Player->GetGUID());
            l_Transmogrifier->SetNotRefundable(p_Player);
            l_Transmogrifier->ClearSoulboundTradeable(p_Player);
        }
    }
    /// Remove transmogrification from weapon
    else
    {
        p_Transmogrified->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1, 0);
        p_Transmogrified->RemoveFlag(ITEM_FIELD_MODIFIERS_MASK, 0x3);
        p_Player->SetVisibleItemSlot(l_TransmogrifiedItemSlot, p_Transmogrified);
    }
}

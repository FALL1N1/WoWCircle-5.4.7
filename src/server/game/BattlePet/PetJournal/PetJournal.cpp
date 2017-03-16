/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

#include "PetJounral.h"
#include "ByteBuffer.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DB2Stores.h"
#include "DB2Enums.h"
#include "TemporarySummon.h"

BattlePet::PetJournal::~PetJournal()
{
    for (BattlePet::PetTemplate* proto : m_pets)
        delete proto;

    m_pets.clear();
}

void BattlePet::PetJournal::LoadFromDb(PreparedQueryResult result)
{
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        PetTemplate* proto = new PetTemplate();
        if (!proto->LoadFromDB(fields))
        {
            delete proto;
            continue;
        }

        proto->LoadDeclinedNames();

        if (uint32 summonSpellId = proto->GetSpellId())
            m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL, summonSpellId, 1);

        m_pets.push_back(proto);
    } 
    while (result->NextRow());
}

void BattlePet::PetJournal::SaveToDb(SQLTransaction& trans)
{
    PreparedStatement* stmt = nullptr;
    for (PetList::iterator itr = m_pets.begin(); itr != m_pets.end();)
    {
        PetTemplate* proto = *itr;

        switch (proto->GetDbState())
        {
            case BATTLE_PET_UNCHANGED:
            {
                ++itr;
                continue;
            }
            case BATTLE_PET_REMOVED:
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, proto->GetId());
                trans->Append(stmt);

                stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BATTLEPET_DECLINED_NAME);
                stmt->setUInt64(0, proto->GetId());
                trans->Append(stmt);

                itr = m_pets.erase(itr);
                delete proto;

                continue;
            }
            case BATTLE_PET_NEW:
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ACCOUNT_BATTLE_PET);
                stmt->setUInt64(0, proto->GetId());
                stmt->setUInt32(1, m_owner->GetSession()->GetAccountId());
                stmt->setUInt16(2, proto->GetSpecies());
                stmt->setString(3, proto->GetNickname());
                stmt->setUInt32(4, proto->GetTimestamp());
                stmt->setUInt8(5, proto->GetLevel());
                stmt->setUInt16(6, proto->GetXp());
                stmt->setUInt16(7, proto->GetCurrentHealth());
                stmt->setUInt8(8, proto->GetQuality());
                stmt->setUInt8(9, proto->GetBreed());
                stmt->setUInt16(10, proto->GetFlags());
                trans->Append(stmt);
                break;
            }
            case BATTLE_PET_CHANGED:
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ACCOUNT_BATTLE_PET);
                stmt->setString(0, proto->GetNickname());
                stmt->setUInt32(1, proto->GetTimestamp());
                stmt->setUInt8(2, proto->GetLevel());
                stmt->setUInt16(3, proto->GetXp());
                stmt->setUInt16(4, proto->GetCurrentHealth());
                stmt->setUInt8(5, proto->GetQuality());
                stmt->setUInt16(6, proto->GetFlags());
                stmt->setUInt64(7, proto->GetId());
                trans->Append(stmt);
                break;
            }
            default:
                ASSERT(false);

        }

        proto->SaveDeclinedNames(trans);
        proto->SetDbState(BATTLE_PET_UNCHANGED);
        ++itr;
    }

    SaveSlotsToDb(trans);
}

void BattlePet::PetJournal::LoadSlotsFromDb(PreparedQueryResult result)
{
    if (result)
    {
        Field* fields = result->Fetch();

        m_loadoutFlags = fields[3].GetUInt8();

        m_loadoutFlags = 0x7; // TODO: hack

        // update flag and spell state for new alt characters
        if (m_loadoutFlags != BATTLE_PET_LOADOUT_SLOT_FLAG_NONE && !m_owner->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_BATTLE_PET_ENABLED))
        {
            m_owner->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_BATTLE_PET_ENABLED);

            m_owner->learnSpell(SPELL_BATTLE_PET_TRAINING_PASSIVE, false);
            m_owner->learnSpell(SPELL_TRACK_PETS, false);
            m_owner->learnSpell(SPELL_REVIVE_BATTLE_PETS, false);
        }

        for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
        {
            uint64 id = fields[slot].GetUInt64();
            if (!LoadoutSlotIsUnlocked(slot) || (id != 0LL && GetBattlePet(id) == nullptr))
            {
                id = 0;
                m_state = BATTLE_PET_CHANGED; // to remove incorrect data from DB at next save
            }

            m_loadout[slot] = id;
        }

        m_slotsLoaded = true;
    }
    else
        m_slotsLoaded = false;
}

void BattlePet::PetJournal::SaveSlotsToDb(SQLTransaction& trans)
{
    if (m_state == BATTLE_PET_UNCHANGED)
        return;

    PreparedStatement* stmt = nullptr;

    if (m_state == BATTLE_PET_CHANGED && !m_slotsLoaded)
        m_state = BATTLE_PET_NEW;

    switch (m_state)
    {
        case BATTLE_PET_CHANGED:
        {
            uint8 index = 0;
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ACCOUNT_BATTLE_PET_SLOTS);
            for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
            {
                stmt->setUInt64(index++, GetLoadoutSlot(slot));
            }
            stmt->setUInt8(index++, GetLoadoutFlags());
            stmt->setUInt32(index++, m_owner->GetSession()->GetAccountId());
            trans->Append(stmt);
            break;
        }
        case BATTLE_PET_NEW:
        {
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ACCOUNT_BATTLE_PET_SLOTS);
            stmt->setUInt32(0, m_owner->GetSession()->GetAccountId());
            trans->Append(stmt);

            uint8 index = 0;
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ACCOUNT_BATTLE_PET_SLOTS);
            stmt->setUInt32(index++, m_owner->GetSession()->GetAccountId());
            for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
            {
                stmt->setUInt64(index++, GetLoadoutSlot(slot));
            }
            stmt->setUInt8(index++, GetLoadoutFlags());
            trans->Append(stmt);
            break;
        }
    }

    m_state = BATTLE_PET_UNCHANGED;
}

BattlePet::PetTemplate* BattlePet::PetJournal::GetBattlePet(uint64 id) const
{
    for (auto pet : m_pets)
        if (pet->GetId() == id)
            return pet;

    return NULL;
}

uint8 BattlePet::PetJournal::GetBattlePetCount(uint16 speciesId) const
{
    uint8 counter = 0;

    for (auto pet : m_pets)
        if (pet->GetSpecies() == speciesId)
            counter++;

    return counter;
}

void BattlePet::PetJournal::UnSummonCurrentBattlePet(bool temporary)
{
    if (!m_summon || !m_summonId)
        return;

    m_summonLastId = temporary ? m_summonId : 0;
    m_summonId = 0;

    m_summon->UnSummon();
    m_summon = NULL;
}

void BattlePet::PetJournal::ResummonLastBattlePet()
{
    if (!m_summonLastId)
        return;

    m_summonId = m_summonLastId;
    if (auto const proto = GetBattlePet(m_summonId))
    {
        if (uint32 summonSpell = proto->GetSummonSpell())
            m_owner->CastSpell(m_owner, summonSpell, true);
    }
    else
        m_summonId = 0LL;

    m_summonLastId = 0LL;
}

uint8 BattlePet::PetJournal::GetLoadoutSlotForBattlePet(uint64 id)
{
    for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; i++)
        if (GetLoadoutSlot(i) == id)
            return i;

    return BATTLE_PET_LOADOUT_SLOT_NONE;
}

void BattlePet::PetJournal::UnlockLoadoutSlot(uint8 slot)
{
    if (LoadoutSlotIsUnlocked(slot) || slot >= BATTLE_PET_MAX_LOADOUT_SLOTS)
        return;

    SetLoadoutFlag(1 << slot);

    m_loadout[slot] = 0;

    // alert client of new Battle Pet loadout slot
    SendBattlePetSlotUpdate(slot, true);
}

void BattlePet::PetJournal::SetLoadoutSlot(uint8 slot, uint64 id)
{
    if (!LoadoutSlotIsUnlocked(slot))
        return;

    m_loadout[slot] = id;

    m_state = BATTLE_PET_CHANGED;
}

void BattlePet::PetJournal::SetLoadoutFlag(uint8 flag)
{
    if (HasLoadoutFlag(flag))
        return;

    m_loadoutFlags |= flag;
    m_state = BATTLE_PET_CHANGED;
}

void BattlePet::PetJournal::Create(uint16 speciesId)
{
    BattlePetSpeciesEntry const* entry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (!entry)
        return;

    if (m_pets.size() > BATTLE_PET_MAX_JOURNAL_PETS)
        return;

    uint8 speciesCount = GetBattlePetCount(speciesId);
    if (entry->HasFlag(BATTLE_PET_FLAG_UNIQUE) && speciesCount >= 1)
        return;

    if (speciesCount >= BATTLE_PET_MAX_JOURNAL_SPECIES)
        return;

    uint64 id     = sObjectMgr->BattlePetGetNewId();
    uint8 breed   = sObjectMgr->BattlePetGetRandomBreed(speciesId);
    uint8 quality = sObjectMgr->BattlePetGetRandomQuality(speciesId);

    PetTemplate* proto = new PetTemplate();
    if (!proto->Create(id, speciesId, BATTLE_PET_INITIAL_LEVEL, quality, breed))
    {
        delete proto;
        return;
    }

    if (uint32 summonSpellId = proto->GetSpellId())
        m_owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL, summonSpellId, 1);

    m_pets.push_back(proto);

    // alert the client of a new Battle Pet
    SendBattlePetUpdate(proto, true);
}

void BattlePet::PetJournal::Create(uint16 speciesId, uint8 breed, uint8 quality, uint8 level)
{
    BattlePetSpeciesEntry const* entry = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (!entry)
        return;

    if (m_pets.size() > BATTLE_PET_MAX_JOURNAL_PETS)
        return;

    uint8 speciesCount = GetBattlePetCount(speciesId);
    if (entry->HasFlag(BATTLE_PET_FLAG_UNIQUE) && speciesCount >= 1)
        return;

    if (speciesCount >= BATTLE_PET_MAX_JOURNAL_SPECIES)
        return;

    uint64 id = sObjectMgr->BattlePetGetNewId();

    PetTemplate* proto = new PetTemplate();
    if (!proto->Create(id, speciesId, level, quality, breed))
    {
        delete proto;
        return;
    }

    m_pets.push_back(proto);

    // alert the client of a new Battle Pet
    SendBattlePetUpdate(proto, true);
}

void BattlePet::PetJournal::Cage(PetTemplate* proto)
{
    if (!proto)
        return;

    const uint32 cageItemId = 82800;
    const uint32 cageModMask = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6;

    ItemPosCountVec dest;
    if (GetOwner()->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, cageItemId, 1) == EQUIP_ERR_OK)
    {
        if (Item* item = GetOwner()->StoreNewItem(dest, cageItemId, true, Item::GenerateItemRandomPropertyId(cageItemId)))
        {
            item->ApplyModFlag(ITEM_FIELD_MODIFIERS_MASK, cageModMask, true);

            item->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 3, proto->GetSpecies());
            item->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 4, proto->GetBreed() | (proto->GetQuality() << 24));
            item->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 5, proto->GetLevel());
            item->SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 6, proto->GetModel());
        }
    }

    Delete(proto);
}

void BattlePet::PetJournal::Delete(PetTemplate* proto)
{
    if (!proto)
        return;

    if (proto->GetDbState() == BATTLE_PET_REMOVED)
        return;

    // this shouldn't happen since the client doesn't allow releasing of slotted Battle Pets
    uint8 srcSlot = GetLoadoutSlotForBattlePet(proto->GetId());
    if (srcSlot != BATTLE_PET_LOADOUT_SLOT_NONE)
    {
        SetLoadoutSlot(srcSlot, 0);
        SendBattlePetSlotUpdate(srcSlot, false);
    }

    proto->SetDbState(BATTLE_PET_REMOVED);

    // alert client of deleted pet
    SendBattlePetDeleted(proto->GetId());
}

void BattlePet::PetJournal::SendBattlePetDeleted(uint64 id)
{
    ObjectGuid petEntry = id;

    WorldPacket data(SMSG_BATTLE_PET_DELETED, 1 + 8);
    data.WriteBit(petEntry[2]);
    data.WriteBit(petEntry[3]);
    data.WriteBit(petEntry[4]);
    data.WriteBit(petEntry[7]);
    data.WriteBit(petEntry[1]);
    data.WriteBit(petEntry[0]);
    data.WriteBit(petEntry[5]);
    data.WriteBit(petEntry[6]);

    data.WriteByteSeq(petEntry[7]);
    data.WriteByteSeq(petEntry[3]);
    data.WriteByteSeq(petEntry[0]);
    data.WriteByteSeq(petEntry[4]);
    data.WriteByteSeq(petEntry[2]);
    data.WriteByteSeq(petEntry[5]);
    data.WriteByteSeq(petEntry[1]);
    data.WriteByteSeq(petEntry[6]);

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePet::PetJournal::SendBattlePetJournalLock()
{
    WorldPacket data(SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED, 0);
    m_owner->GetSession()->SendPacket(&data);

    /*WorldPacket data(SMSG_BATTLE_PET_JOURNAL_LOCK_DENINED, 0);
    m_owner->GetSession()->SendPacket(&data);*/
}

void BattlePet::PetJournal::SendBattlePetJournal()
{
    uint32 petCount = 0;
    ByteBuffer journalData, slotData;

    // packet size is over estimated
    WorldPacket data(SMSG_BATTLE_PET_JOURNAL, 8 + m_pets.size() * (2 + 1 + 8 + 1 + 4 + 2 + 4 + 2 + 4 + 2 + 4 + 4 + 4 + 16 + 4 + 2)
        + BATTLE_PET_MAX_LOADOUT_SLOTS * (1 + 1 + 8 + 1));

    data.WriteBits(BATTLE_PET_MAX_LOADOUT_SLOTS, 25); // Battle Team
    auto wpos = data.wpos();
    auto bpos = data.bitpos();
    data.WriteBits(petCount, 19);               // placeholder
    
    for (BattlePet::PetTemplate const* battlePet : m_pets)
    {
        if (battlePet->GetDbState() == BATTLE_PET_REMOVED)
            continue;

        ObjectGuid petEntry = battlePet->GetId();

        data.WriteBits(battlePet->GetNickname().size(), 7);
        data.WriteBit(!battlePet->GetQuality());
        data.WriteBit(petEntry[3]);
        data.WriteBit(!battlePet->GetBreed());
        data.WriteBit(petEntry[5]);
        data.WriteBit(petEntry[0]);
        data.WriteBit(!battlePet->GetFlags());
        data.WriteBit(0);                       // BATTLE_PET_FLAG_NOT_ACCOUNT_BOUND
        data.WriteBit(petEntry[2]);
        data.WriteBit(0);                       // unknown
        data.WriteBit(petEntry[7]);
        data.WriteBit(petEntry[4]);
        data.WriteBit(petEntry[6]);
        data.WriteBit(petEntry[1]);

        if (battlePet->GetFlags())
            journalData << uint16(battlePet->GetFlags());

        journalData << uint32(battlePet->GetCurrentHealth());
        journalData << uint32(battlePet->GetSpeed());

        if (battlePet->GetBreed())
            journalData << uint16(battlePet->GetBreed());

        if (battlePet->GetQuality())
            journalData << uint8(battlePet->GetQuality());

        journalData.WriteByteSeq(petEntry[5]);
        journalData << uint16(battlePet->GetXp());
        journalData.WriteByteSeq(petEntry[2]);
        journalData.WriteByteSeq(petEntry[1]);
        journalData.WriteByteSeq(petEntry[6]);
        journalData << uint32(battlePet->GetMaxHealth());
        journalData << uint32(battlePet->GetModel());
        journalData << uint32(battlePet->GetSpecies());
        journalData << uint32(battlePet->GetNPCId());
        journalData << uint32(battlePet->GetPower());
        journalData.WriteByteSeq(petEntry[3]);
        journalData << uint16(battlePet->GetLevel());
        journalData.WriteByteSeq(petEntry[0]);
        journalData.WriteByteSeq(petEntry[4]);
        journalData.WriteString(battlePet->GetNickname());
        journalData.WriteByteSeq(petEntry[7]);

        petCount++;
    }

    for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
    {
        ObjectGuid loadoutEntry = GetLoadoutSlot(slot);

        data.WriteBit(!false);
        data.WriteBit(false/*!HasLoadoutSlot(slot)*/);  // Slot is disabled
        data.WriteBit(slot == 0);
        data.WriteBit(loadoutEntry > 0);
        data.WriteBit(loadoutEntry[3]);
        data.WriteBit(loadoutEntry[5]);
        data.WriteBit(loadoutEntry[0]);
        data.WriteBit(loadoutEntry[4]);
        data.WriteBit(loadoutEntry[1]);
        data.WriteBit(loadoutEntry[6]);
        data.WriteBit(loadoutEntry[2]);
        data.WriteBit(loadoutEntry[7]);

        slotData.WriteByteSeq(loadoutEntry[2]);
        slotData.WriteByteSeq(loadoutEntry[7]);
        slotData.WriteByteSeq(loadoutEntry[3]);
        slotData.WriteByteSeq(loadoutEntry[5]);
        slotData.WriteByteSeq(loadoutEntry[1]);
        slotData.WriteByteSeq(loadoutEntry[0]);
        slotData.WriteByteSeq(loadoutEntry[6]);
        slotData.WriteByteSeq(loadoutEntry[4]);

        if (slot != 0)
            slotData << uint8(slot);
    }

    data.WriteBit(1);                           // slots enabled

    data.PutBits(petCount, 19, wpos, bpos);

    data.append(slotData);
    data.append(journalData);
    data << uint16(1);                          // unknown

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePet::PetJournal::SendBattlePetSlotUpdate(uint8 slot, bool notification, uint64 id)
{
    ObjectGuid petEntry = id;

    WorldPacket data(SMSG_BATTLE_PET_SLOT_UPDATE, 5 + 1);
    data.WriteBits(1, 25);
    data.WriteBit(0);                   // unknown

    {
        data.WriteBit(0);               // has slot number
        data.WriteBit(1);               // unknown
        data.WriteBit(0);               // unknown
        data.WriteBit(0);               // fake
        data.WriteBit(petEntry[4]);
        data.WriteBit(petEntry[5]);
        data.WriteBit(petEntry[2]);
        data.WriteBit(petEntry[1]);
        data.WriteBit(petEntry[0]);
        data.WriteBit(petEntry[3]);
        data.WriteBit(petEntry[7]);
        data.WriteBit(petEntry[6]);
    }

    data.WriteBit(notification);
    {
        data.WriteByteSeq(petEntry[0]);
        data.WriteByteSeq(petEntry[3]);
        data.WriteByteSeq(petEntry[2]);
        data.WriteByteSeq(petEntry[1]);
        data.WriteByteSeq(petEntry[6]);
        data.WriteByteSeq(petEntry[4]);
        data.WriteByteSeq(petEntry[5]);
        data.WriteByteSeq(petEntry[7]);
        data << uint8(slot);
    }

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePet::PetJournal::SendBattlePetUpdate(uint64 id, bool notification)
{
    BattlePet::PetTemplate* pet = GetBattlePet(id);
    if (!pet || pet->GetId() != id)
        return;

    SendBattlePetUpdate(pet, notification);
}

void BattlePet::PetJournal::SendBattlePetUpdate(BattlePet::PetTemplate* pet, bool notification)
{
    if (!m_owner || !m_owner->GetSession())
        return;

    if (!pet || !pet->GetId())
        return;

    ObjectGuid petEntry = pet->GetId();
    if (!petEntry)
        return;

    WorldPacket data(SMSG_BATTLE_PET_UPDATE, 4 + 1 + 8 + pet->GetNickname().size() + 4 + 4 + 4 + 4 + 2 + 4 + 4 + 1 + 2 + 2 + 4 + 2);
    data.WriteBit(notification);
    data.WriteBits(1, 19);
    {
        data.WriteBit(false);                           // BATTLE_PET_FLAG_NOT_ACCOUNT_BOUND
        data.WriteBit(petEntry[4]);
        data.WriteBit(!pet->GetBreed());
        data.WriteBit(!pet->GetFlags());
        data.WriteBit(petEntry[1]);
        data.WriteBits(pet->GetNickname().size(), 7);
        data.WriteBit(!pet->GetQuality());
        data.WriteBit(petEntry[3]);
        data.WriteBit(petEntry[6]);
        data.WriteBit(petEntry[5]);
        data.WriteBit(petEntry[7]);
        data.WriteBit(0);                           // unknown
        data.WriteBit(petEntry[0]);
        data.WriteBit(petEntry[2]);
    }

    {
        data << uint32(pet->GetCurrentHealth());
        if (pet->GetQuality())
            data << uint8(pet->GetQuality());
        data.WriteByteSeq(petEntry[0]);
        data << uint32(pet->GetNPCId());
        data.WriteByteSeq(petEntry[6]);
        data << uint32(pet->GetPower());
        data << uint16(pet->GetLevel());
        data << uint32(pet->GetSpecies());
        data.WriteByteSeq(petEntry[4]);
        data << uint32(pet->GetSpeed());
        data.WriteByteSeq(petEntry[2]);
        data.WriteByteSeq(petEntry[1]);
        data.WriteByteSeq(petEntry[5]);
        data << uint32(pet->GetMaxHealth());
        if (pet->GetFlags())
            data << uint16(pet->GetFlags());
        data.WriteString(pet->GetNickname());
        if (pet->GetBreed())
            data << uint16(pet->GetBreed());
        data << uint16(pet->GetXp());
        data.WriteByteSeq(petEntry[7]);
        data << uint32(pet->GetModel());
    }
    
    m_owner->GetSession()->SendPacket(&data);
}

void BattlePet::PetJournal::SendPetsUpdateInLoadoutSlots(bool notification)
{
    uint8 count = 0;
    for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
    {
        ObjectGuid entry = GetLoadoutSlot(slot);
        if (!entry)
            continue;

        if (!GetBattlePet(entry))
            continue;

        ++count;
    }

    if (count == 0)
        return;

    WorldPacket data(SMSG_BATTLE_PET_UPDATE, 64);
    data.WriteBit(notification);
    data.WriteBits(count, 19);
    for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
    {
        ObjectGuid entry = GetLoadoutSlot(slot);
        if (!entry)
            continue;

        PetTemplate const* proto = GetBattlePet(entry);
        if (!proto)
            continue;
    
        data.WriteBit(false);                           // BATTLE_PET_FLAG_NOT_ACCOUNT_BOUND
        data.WriteBit(entry[4]);
        data.WriteBit(!proto->GetBreed());
        data.WriteBit(!proto->GetFlags());
        data.WriteBit(entry[1]);
        data.WriteBits(proto->GetNickname().size(), 7);
        data.WriteBit(!proto->GetQuality());
        data.WriteBit(entry[3]);
        data.WriteBit(entry[6]);
        data.WriteBit(entry[5]);
        data.WriteBit(entry[7]);
        data.WriteBit(0);                           // unknown
        data.WriteBit(entry[0]);
        data.WriteBit(entry[2]);
    }

    for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
    {
        ObjectGuid entry = GetLoadoutSlot(slot);
        if (!entry)
            continue;

        PetTemplate const* proto = GetBattlePet(entry);
        if (!proto)
            continue;

        data << uint32(proto->GetCurrentHealth());
        if (proto->GetQuality())
            data << uint8(proto->GetQuality());
        data.WriteByteSeq(entry[0]);
        data << uint32(proto->GetNPCId());
        data.WriteByteSeq(entry[6]);
        data << uint32(proto->GetPower());
        data << uint16(proto->GetLevel());
        data << uint32(proto->GetSpecies());
        data.WriteByteSeq(entry[4]);
        data << uint32(proto->GetSpeed());
        data.WriteByteSeq(entry[2]);
        data.WriteByteSeq(entry[1]);
        data.WriteByteSeq(entry[5]);
        data << uint32(proto->GetMaxHealth());
        if (proto->GetFlags())
            data << uint16(proto->GetFlags());
        data.WriteString(proto->GetNickname());
        if (proto->GetBreed())
            data << uint16(proto->GetBreed());
        data << uint16(proto->GetXp());
        data.WriteByteSeq(entry[7]);
        data << uint32(proto->GetModel());
    }

    m_owner->GetSession()->SendPacket(&data);
}

void BattlePet::PetJournal::ConvertOldPetsToBattlePet()
{
    typedef std::vector<uint32> Pets;

    Pets obtainedPets;

    for (BattlePet::PetTemplate const* battlePet : m_pets)
    {
        if (battlePet->GetDbState() == BATTLE_PET_REMOVED)
            continue;

        CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(battlePet->GetNPCId());
        if (!creatureTemplate)
            continue;

        obtainedPets.push_back(creatureTemplate->Entry);
    }

    auto const spellMap = m_owner->GetSpellMap();
    for (auto spell : spellMap)
    {
        if (!spell.second)
            continue;

        if (spell.second->state == PLAYERSPELL_REMOVED)
            continue;

        if (!spell.second->active || spell.second->disabled)
            continue;

        uint32 const spellId = spell.first;
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
            continue;

        SpellToItemMap::const_iterator i = sSpellToBattlePetItemMap.find(spellId);
        if (i == sSpellToBattlePetItemMap.end())
            continue;

        const BattlePetSpeciesEntry* species = sBattlePetSpeciesStore.LookupEntry(i->second);
        if (!species)
            continue;

        if (std::find(
            obtainedPets.begin(),
            obtainedPets.end(),
            species->NpcId) != obtainedPets.end())
            continue;

        Create(species->SpeciesId);
    }
}
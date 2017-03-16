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

#include "BattlePetTemplate.h"
#include "Common.h"
#include "DB2Stores.h"
#include "ObjectMgr.h"

bool BattlePet::PetTemplate::Create(uint64 id, uint16 speciesId, uint8 level, uint8 quality, uint8 breedId)
{
    m_proto = sBattlePetSpeciesStore.LookupEntry(speciesId);
    if (m_proto == nullptr)
        return false;

    m_id = id;
    m_species = speciesId;
    m_level = level;
    m_quality = quality;
    m_breed = breedId;

    if (CreatureTemplate const* proto = sObjectMgr->GetCreatureTemplate(GetNPCId()))
        m_modelId = proto->Modelid1;

    m_dbState = BATTLE_PET_NEW;

    CalculateStats(true);
    return true;
}

bool BattlePet::PetTemplate::LoadFromDB(Field* fields)
{
    if (!fields)
        return false;

    m_id = fields[0].GetUInt64();
    m_species = fields[1].GetUInt16();
    m_nickname = fields[2].GetString();
    m_timestamp = fields[3].GetUInt32();
    m_level = fields[4].GetUInt8();
    m_xp = fields[5].GetUInt16();
    m_curHealth = fields[6].GetUInt16();
    m_quality = fields[7].GetUInt8();
    m_breed = fields[8].GetUInt8();
    m_flags = fields[9].GetUInt16();

    m_proto = sBattlePetSpeciesStore.LookupEntry(m_species);
    if (!m_proto)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Species %u defined in `account_battle_pet` for Battle Pet %lu does not exist, skipped.", m_species, m_id);
        return false;;
    }

    if (sBattlePetBreedSet.find(m_breed) == sBattlePetBreedSet.end() && m_breed != 0)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Breed %u defined in `account_battle_pet` for Battle Pet %lu does not exist, skipped.", m_breed, m_id);
        return false;
    }

    // highest quality client supports, currently players can not obtain legendary pets on retail
    if (m_quality > ITEM_QUALITY_LEGENDARY)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Quality %u defined in `account_battle_pet` for Battle Pet %lu is invalid, skipped.", m_quality, m_id);
        return false;
    }

    // client supports up to level 255 (uint8)
    if (m_level > BATTLE_PET_MAX_LEVEL)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Level %u defined in `account_battle_pet` for Battle Pet %lu is invalid, skipped.", m_level, m_id);
        return false;
    }

    if (CreatureTemplate const* proto = sObjectMgr->GetCreatureTemplate(GetNPCId()))
        m_modelId = proto->Modelid1;
    else
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "SpeciesId %u, NPC Id %u does not have a record in creature_template for Battle Pet %lu, skipped.", m_species, m_proto->NpcId, m_id);
        return false;
    }

    CalculateStats(false);

    return true;
}

void BattlePet::PetTemplate::LoadDeclinedNames()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_BATTLEPET_DECLINED_NAME);
    stmt->setUInt64(0, m_id);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    delete m_declinedNames;
    m_declinedNames = new DeclinedName();

    Field* fields = result->Fetch();
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        m_declinedNames->name[i] = fields[i].GetString();
    }
}

void BattlePet::PetTemplate::SaveDeclinedNames(SQLTransaction& trans)
{
    if (!m_declinedNames)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BATTLEPET_DECLINED_NAME);
    stmt->setUInt64(0, m_id);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BATTLEPET_DECLINED_NAME);
    stmt->setUInt64(0, m_id);
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        stmt->setString(1 + i, m_declinedNames->name[i]);
    }
    trans->Append(stmt);
}

void BattlePet::PetTemplate::CalculateStats(bool currentHealth)
{
    float basePower = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_POWER, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_POWER, m_breed);
    float baseHealth = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_STAMINA, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_STAMINA, m_breed);
    float baseSpeed = BattlePetSpeciesMainStat(BATTLE_PET_STATE_STAT_SPEED, m_species) +
        BattlePetBreedMainStatModifier(BATTLE_PET_STATE_STAT_SPEED, m_breed);

    float qualityMod = 1.0f; // ITEM_QUALITY_POOR
    switch (m_quality)
    {
        case ITEM_QUALITY_NORMAL:
            qualityMod = 1.1f;
            break;
        case ITEM_QUALITY_UNCOMMON:
            qualityMod = 1.2f;
            break;
        case ITEM_QUALITY_RARE:
            qualityMod = 1.3f;
            break;
        case ITEM_QUALITY_EPIC:
            qualityMod = 1.4f;
            break;
        case ITEM_QUALITY_LEGENDARY:
            qualityMod = 1.5f;
            break;
    }

    // No round in older cpp, just for compatibility
    m_maxHealth = floor(((baseHealth * 5.0f * m_level * qualityMod) + 100.0f) + 0.5f);

    if (currentHealth)
        m_curHealth = m_maxHealth;

    m_curHealth = std::min<uint16>(m_curHealth, m_maxHealth);

    m_power = floor((basePower * m_level * qualityMod) + 0.5f);
    m_speed = floor((baseSpeed * m_level * qualityMod) + 0.5f);
}

void BattlePet::PetTemplate::SetNickname(std::string const& nickname)
{
    m_nickname = nickname;
    SetState(BATTLE_PET_CHANGED);
}

void BattlePet::PetTemplate::SetQuality(ItemQualities quality)
{
    if (m_quality == quality)
        return;

    m_quality = quality;
    SetState(BATTLE_PET_CHANGED);

    CalculateStats(false);
}

void BattlePet::PetTemplate::SetLevel(uint8 level)
{
    if (m_level == level)
        return;

    m_level = level;
    SetState(BATTLE_PET_CHANGED);

    CalculateStats(true);
}

void BattlePet::PetTemplate::SetXP(uint16 xp)
{
    if (m_xp == xp)
        return;

    m_xp = xp;
    SetState(BATTLE_PET_CHANGED);
}

void BattlePet::PetTemplate::SetHp(int16 hp)
{
    hp = std::max<int16>(hp, 0);
    if (m_curHealth == hp)
        return;

    m_curHealth = hp;
    SetState(BATTLE_PET_CHANGED);
}

void BattlePet::PetTemplate::SetFlag(uint16 flag)
{
    if (HasFlag(flag))
        return;

    m_flags |= flag;
    SetState(BATTLE_PET_CHANGED);
}

void BattlePet::PetTemplate::UnSetFlag(uint16 flag)
{
    if (!HasFlag(flag))
        return;

    m_flags &= ~flag;
    SetState(BATTLE_PET_CHANGED);
}

bool BattlePet::PetTemplate::IsPetAvailableForBattle() const
{
    if (GetCurrentHealth() <= 0)
        return false;

    return true;
}
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

#pragma once

#include "Common.h"
#include "Unit.h"
#include "DB2Enums.h"
#include "DB2Stores.h"
#include "DB2Structure.h"
#include "BattlePetDefines.h"
#include "DBCStores.h"

const static BattlePetStates FamilyToState[BATTLE_PET_FAMILY_MAX - 1] =
{
    BATTLE_PET_STATE_PASSIVE_HUMANOID,
    BATTLE_PET_STATE_PASSIVE_DRAGON,
    BATTLE_PET_STATE_PASSIVE_FLYING,
    BATTLE_PET_STATE_PASSIVE_UNDEAD,
    BATTLE_PET_STATE_PASSIVE_CRITTER,
    BATTLE_PET_STATE_PASSIVE_MAGIC,
    BATTLE_PET_STATE_PASSIVE_ELEMENTAL,
    BATTLE_PET_STATE_PASSIVE_BEAST,
    BATTLE_PET_STATE_PASSIVE_AQUATIC,
    BATTLE_PET_STATE_PASSIVE_MECHANICAL
};

const static uint32 PetTypePassives[BATTLE_PET_FAMILY_MAX - 1] =
{
    238, //--Humanoid - Recovery
    245, //--Dragon - Execute
    239, //--Flying - Swiftness
    242, //--Undead - Damned
    236, //--Critter - Elusive
    243, //--Magical - Spellshield
    241, //--Elemental - Weather Immune
    237, //--Beast - Enrage
    240, //--Aquatic - Purity
    244, //--Mechanical - Failsafe
};

enum BattlePetDbState
{
    BATTLE_PET_UNCHANGED = 0,
    BATTLE_PET_CHANGED = 1,
    BATTLE_PET_NEW = 2,
    BATTLE_PET_REMOVED = 3,
};
namespace BattlePet
{
class PetTemplate
{
public:
    PetTemplate()
        : m_declinedNames(nullptr)
        , m_proto(nullptr)
        , m_id(0LL)
        , m_species(0)
        , m_breed(0)
        , m_nickname("")
        , m_timestamp(0)
        , m_level(0)
        , m_xp(0)
        , m_curHealth(0),
        m_maxHealth(0)
        , m_power(0)
        , m_speed(0)
        , m_quality(0)
        , m_flags(0)
        , m_modelId(0)
        , m_dbState(BATTLE_PET_UNCHANGED)
    {
    }

    ~PetTemplate()
    {
        delete m_declinedNames;
        m_declinedNames = nullptr;
    }

    bool Create(uint64 id, uint16 speciesId, uint8 level, uint8 quality, uint8 breedId);
    bool LoadFromDB(Field* fields);

    uint32 GetNPCId() const { return m_proto->NpcId; }
    uint32 GetSpellId() const { return m_proto->SpellId; }
    uint64 GetId() const { return m_id; }
    uint16 GetSpecies() const { return m_species; }
    std::string GetNickname() const { return m_nickname; }
    uint32 GetTimestamp() const { return m_timestamp; }
    uint8 GetLevel() const { return m_level; }
    uint16 GetXp() const { return m_xp; }
    uint16 GetCurrentHealth() const { return m_curHealth; }
    uint16 GetMaxHealth() const { return m_maxHealth; }
    uint16 GetPower() const { return m_power; }
    uint16 GetSpeed() const { return m_speed; }
    uint8 GetQuality() const { return m_quality; }
    uint8 GetBreed() const { return m_breed; }
    uint8 GetFamily() const { return m_proto->FamilyId; }
    uint32 GetModel() const { return m_modelId; }

    void SetLevel(uint8 level);
    void SetXP(uint16 xp);
    void SetHp(int16 hp);

    bool HasSpeciesFlag(BattlePetSpeciesFlags flag)
    {
        return m_proto->HasFlag(flag);
    }

    bool IsPetAvailableForBattle() const;

    void SetNickname(std::string const& nickname);
    void SetTimestamp(uint32 timestamp) { m_timestamp = timestamp; }
    void SetQuality(ItemQualities quality);

    BattlePetDbState GetDbState() const { return m_dbState; }
    void SetDbState(BattlePetDbState state) { m_dbState = state; }

    uint8 GetFlags() const { return m_flags; }
    bool HasFlag(uint16 flag) const { return (m_flags & flag) != 0; }
    void SetFlag(uint16 flag);
    void UnSetFlag(uint16 flag);

    void LoadDeclinedNames();
    void SaveDeclinedNames(SQLTransaction& trans);
    void SetDeclinedName(uint8 index, std::string const& name)
    {
        if (!m_declinedNames)
        {
            m_declinedNames = new DeclinedName();
        }

        m_declinedNames->name[index] = name;

        SetState(BATTLE_PET_CHANGED);
    }

    BattlePetStates GetStateFromFamily() const
    {
        return FamilyToState[m_proto->FamilyId];
    }

    uint32 GetPetPassive() const
    {
        return PetTypePassives[m_proto->FamilyId];
    }

    uint32 GetSummonSpell() const
    {
        return m_proto->SpellId;
    }

    DeclinedName* GetDeclinedNames() { return m_declinedNames; }
    DeclinedName const* GetDeclinedNames() const { return m_declinedNames; }

    void CalculateStats(bool currentHealth = false);

    void SetState(BattlePetDbState state)
    {
        if (m_dbState == BATTLE_PET_NEW)
        {
            if (state == BATTLE_PET_CHANGED)
                return;
        }

        m_dbState = state;
    }

private:
    uint64 m_id;
    uint16 m_species;
    std::string m_nickname;
    uint32 m_timestamp;
    uint8 m_level;
    uint16 m_xp;
    uint16 m_curHealth;
    uint16 m_maxHealth;
    uint16 m_power;
    uint16 m_speed;
    uint8 m_quality;
    uint8 m_breed;
    uint16 m_flags;
    uint32 m_modelId;

    BattlePetDbState m_dbState;

    DeclinedName* m_declinedNames;

    BattlePetSpeciesEntry const* m_proto;
};
}
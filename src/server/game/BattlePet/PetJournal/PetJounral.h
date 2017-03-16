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

#include "BattlePetTemplate.h"
#include "Common.h"
#include "BattlePetDefines.h"

class TempSummon;

namespace BattlePet
{
class PetJournal
{
public:

    typedef std::vector<BattlePet::PetTemplate*> PetList;

    PetJournal(Player* owner)
        : m_owner(owner), m_summon(NULL), m_summonId(0), m_summonLastId(0), m_loadoutFlags(0x7), m_state(BATTLE_PET_UNCHANGED), m_slotsLoaded(false) {}

    ~PetJournal();

    Player* GetOwner() const { return m_owner; }
    BattlePet::PetTemplate* GetBattlePet(uint64 id) const;
    uint8 GetBattlePetCount(uint16 speciesId) const;
    uint8 GetLoadoutSlotForBattlePet(uint64 id);

    uint64 GetCurrentSummonId() const { return m_summonId; }
    TempSummon* GetCurrentSummon() const { return m_summon; }
    void SetCurrentSummonId(uint64 summonId) { m_summonId = summonId; }
    void SetCurrentSummon(TempSummon* summon) { m_summon = summon; }

    void UnSummonCurrentBattlePet(bool temporary);
    void ResummonLastBattlePet();

    void LoadFromDb(PreparedQueryResult result);
    void LoadSlotsFromDb(PreparedQueryResult result);
    void SaveToDb(SQLTransaction& trans);
    void SaveSlotsToDb(SQLTransaction& trans);

    void UnlockLoadoutSlot(uint8 slot);
    bool LoadoutSlotIsUnlocked(uint8 slot) const
    {
        if (!m_loadoutFlags || slot >= BATTLE_PET_MAX_LOADOUT_SLOTS)
            return false;

        return HasLoadoutFlag(1 << slot);
    }

    uint64 GetLoadoutSlot(uint8 slot) const
    {
        if (!LoadoutSlotIsUnlocked(slot))
            return 0;

        return m_loadout[slot];
    }

    void SetLoadoutSlot(uint8 slot, uint64 id);

    bool HasLoadoutFlag(uint8 flag) const { return (m_loadoutFlags & flag) != 0; };
    uint8 GetLoadoutFlags() const { return m_loadoutFlags; };
    void SetLoadoutFlag(uint8 flag);

    void Create(uint16 speciesId);
    void Create(uint16 speciesId, uint8 breed, uint8 quality, uint8 level);
    void Cage(PetTemplate* proto);
    void Delete(PetTemplate* proto);

    void SendBattlePetDeleted(uint64 id);
    void SendBattlePetJournalLock();
    void SendBattlePetJournal();
    void SendBattlePetSlotUpdate(uint8 slot, bool notification, uint64 id = 0);
    void SendBattlePetUpdate(uint64 id, bool notification);
    void SendBattlePetUpdate(BattlePet::PetTemplate* pet, bool notification);

    void SendPetsUpdateInLoadoutSlots(bool notification);

    void ConvertOldPetsToBattlePet();

    PetList const* GetPetList() { return &m_pets; }

private:


    Player* m_owner;
    PetList m_pets;

    TempSummon* m_summon;
    uint64 m_summonId;
    uint64 m_summonLastId;

    bool m_slotsLoaded;
    uint8 m_state;
    uint8 m_loadoutFlags;
    uint64 m_loadout[BATTLE_PET_MAX_LOADOUT_SLOTS];
};
}
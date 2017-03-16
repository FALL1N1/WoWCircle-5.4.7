/*
 * Copyright (C) 2011 TrintiyCore <http://www.trinitycore.org/>
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

#include "DB2Enums.h"
#include "DB2Stores.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "ItemPrototype.h"
#include "DB2fmt.h"

#include <map>

DB2Storage <BattlePetAbilityEntry>          sBattlePetAbilityStore(BattlePetAbilityfmt);
DB2Storage <BattlePetAbilityStateEntry>     sBattlePetAbilityStateStore(BattlePetAbilityStatefmt);
DB2Storage <BattlePetAbilityEffectEntry>    sBattlePetAbilityEffectStore(BattlePetAbilityEffectfmt);
DB2Storage <BattlePetEffectPropertiesEntry> sBattlePetEffectPropertiesStore(BattlePetEffectPropertiesfmt);
DB2Storage <BattlePetAbilityTurnEntry>      sBattlePetAbilityTurnStore(BattlePetAbilityTurnfmt);
DB2Storage <BattlePetBreedStateEntry>       sBattlePetBreedStateStore(BattlePetBreedStatefmt);
DB2Storage <BattlePetSpeciesEntry>          sBattlePetSpeciesStore(BattlePetSpeciesfmt);
DB2Storage <BattlePetSpeciesStateEntry>     sBattlePetSpeciesStateStore(BattlePetSpeciesStatefmt);
DB2Storage <BattlePetSpeciesXAbilityEntry>  sBattlePetSpeciesXAbilityStore(BattlePetSpeciesXAbilityfmt);
DB2Storage <BattlePetStateEntry>            sBattlePetStateStore(BattlePetStatefmt);
DB2Storage <ItemToBattlePetEntry>           sItemToBattlePetStore(ItemToBattlePetfmt);
DB2Storage <ItemEntry>                      sItemStore(Itemfmt);
DB2Storage <ItemCurrencyCostEntry>          sItemCurrencyCostStore(ItemCurrencyCostfmt);
DB2Storage <ItemExtendedCostEntry>          sItemExtendedCostStore(ItemExtendedCostEntryfmt);
DB2Storage <ItemSparseEntry>                sItemSparseStore(ItemSparsefmt);
DB2Storage <SpellReagentsEntry>             sSpellReagentsStore(SpellReagentsEntryfmt);
DB2Storage <ItemUpgradeEntry>               sItemUpgradeStore(ItemUpgradeEntryfmt);
DB2Storage <RulesetItemUpgradeEntry>        sRulesetItemUpgradeStore(RulesetItemUpgradeEntryfmt);
DB2Storage <SpellVisualEntry>               sSpellVisualEntryStore(SpellVisualEntryfmt);
DB2Storage <MapChallengeModeEntry>          sMapChallengeModeStore(MapChallengeModeEntryfmt);
DB2Storage <SceneScriptPackageEntry>        sSceneScriptPackageStore(SceneScriptPackageEntryfmt);
std::map<uint32, std::vector<RulesetItemUpgradeEntry const*>> sItemRulesetUpgradeMap;

BattlePetBreedSet sBattlePetBreedSet;
BattlePetSpellToSpeciesMap sBattlePetSpellToSpeciesMap;
SpellToItemMap sSpellToBattlePetItemMap;
BattlePetAbilityEffectsMap sBattlePetAbilityEffectsMap;
BattlePetAbilityStatesMap sBattlePetAbilityStatesMap;
BattlePetSpeciesStatesMap sBattlePetSpeciesStatesMap;
BattlePetBreedStatesMap sBattlePetBreedStatesMap;
BattlePetSpeciesXAbilityMap sBattlePetSpeciesXAbilityMap;
SpellReagentMap sSpellReagentMap;

typedef std::list<std::string> StoreProblemList1;

uint32 DB2FilesCount = 0;

static bool LoadDB2_assert_print(uint32 fsize,uint32 rsize, const std::string& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

struct LocalDB2Data
{
    LocalDB2Data(LocaleConstant loc) : defaultLocale(loc), availableDb2Locales(0xFFFFFFFF) {}

    LocaleConstant defaultLocale;

    // bitmasks for index of fullLocaleNameList
    uint32 availableDb2Locales;
};

template<class T>
inline void LoadDB2(StoreProblemList1& errlist, DB2Storage<T>& storage, const std::string& db2_path, const std::string& filename)
{
    // compatibility format and C++ structure sizes
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DB2FilesCount;

    std::string db2_filename = db2_path + filename;
    if (!storage.Load(db2_filename.c_str()))
    {
        // sort problematic db2 to (1) non compatible and (2) nonexistent
        if (FILE * f = fopen(db2_filename.c_str(), "rb"))
        {
            char buf[100];
            snprintf(buf, 100,"(exist, but have %d fields instead " SIZEFMTD ") Wrong client version DBC file?", storage.GetFieldCount(), strlen(storage.GetFormat()));
            errlist.push_back(db2_filename + buf);
            fclose(f);
        }
        else
            errlist.push_back(db2_filename);
    }
}

void LoadDB2Stores(const std::string& dataPath)
{
    std::string db2Path = dataPath + "dbc/";

    StoreProblemList1 bad_db2_files;

    LoadDB2(bad_db2_files, sBattlePetAbilityStore, db2Path, "BattlePetAbility.db2");
    LoadDB2(bad_db2_files, sBattlePetAbilityStateStore, db2Path, "BattlePetAbilityState.db2");
    LoadDB2(bad_db2_files, sBattlePetBreedStateStore, db2Path, "BattlePetBreedState.db2");
    LoadDB2(bad_db2_files, sBattlePetAbilityEffectStore, db2Path, "BattlePetAbilityEffect.db2");
    LoadDB2(bad_db2_files, sBattlePetEffectPropertiesStore, db2Path, "BattlePetEffectProperties.db2");
    LoadDB2(bad_db2_files, sBattlePetAbilityTurnStore, db2Path, "BattlePetAbilityTurn.db2");

    for (uint32 i = 0; i < sBattlePetBreedStateStore.GetNumRows(); i++)
        if (BattlePetBreedStateEntry const* breedStateEntry = sBattlePetBreedStateStore.LookupEntry(i))
            if (sBattlePetBreedSet.find(breedStateEntry->BreedId) == sBattlePetBreedSet.end())
                sBattlePetBreedSet.insert(breedStateEntry->BreedId);

    LoadDB2(bad_db2_files, sBattlePetSpeciesStore, db2Path, "BattlePetSpecies.db2");
    LoadDB2(bad_db2_files, sBattlePetSpeciesStateStore, db2Path, "BattlePetSpeciesState.db2");
    LoadDB2(bad_db2_files, sBattlePetSpeciesXAbilityStore, db2Path, "BattlePetSpeciesXAbility.db2");
    LoadDB2(bad_db2_files, sBattlePetStateStore, db2Path, "BattlePetState.db2");

    for (uint32 i = 0; i < sBattlePetAbilityTurnStore.GetNumRows(); i++)
    {
        BattlePetAbilityTurnEntry const* turn = sBattlePetAbilityTurnStore.LookupEntry(i);
        if (!turn)
            continue;

        if (!sBattlePetAbilityStore.LookupEntry(turn->AbilityId))
            continue;

        BattlePetAbilityEffectVector& effects = sBattlePetAbilityEffectsMap[turn->AbilityId][turn->Index];
        for (uint32 j = 0; j < sBattlePetAbilityEffectStore.GetNumRows(); ++j)
        {
            BattlePetAbilityEffectEntry const* effect = sBattlePetAbilityEffectStore.LookupEntry(j);
            if (!effect || effect->AbilityTurnId != turn->Id)
                continue;

            effects.push_back(effect);
        }
    }

    for (auto& data : sBattlePetAbilityEffectsMap)
    {
        for (auto& turn: data.second)
        {
            auto& effects = turn.second;

            std::sort(effects.begin(), effects.end(), [](BattlePetAbilityEffectEntry const* a, BattlePetAbilityEffectEntry const* b)
            {
                return a->EffectIndex < b->EffectIndex;
            });
        }
    }

    for (uint32 i = 0; i < sBattlePetSpeciesStore.GetNumRows(); i++)
    {
        if (BattlePetSpeciesEntry const* entry = sBattlePetSpeciesStore.LookupEntry(i))
        {
            sBattlePetSpellToSpeciesMap[entry->SpellId] = entry->SpeciesId;
        }
    }

    for (uint32 i = 0; i < sBattlePetAbilityStateStore.GetNumRows(); i++)
    {
        if (BattlePetAbilityStateEntry const* state = sBattlePetAbilityStateStore.LookupEntry(i))
        {
            if (state->AbilityId)
            {
                sBattlePetAbilityStatesMap[state->AbilityId].push_back(state);
            }
        }
    }

    for (uint32 i = 0; i < sBattlePetSpeciesStateStore.GetNumRows(); i++)
    {
        BattlePetSpeciesStateEntry const* state = sBattlePetSpeciesStateStore.LookupEntry(i);
        if (!state)
            continue;

        sBattlePetSpeciesStatesMap[state->SpeciesId].push_back(state);
    }

    for (uint32 i = 0; i < sBattlePetBreedStateStore.GetNumRows(); i++)
    {
        BattlePetBreedStateEntry const* state = sBattlePetBreedStateStore.LookupEntry(i);
        if (!state)
            continue;

        sBattlePetBreedStatesMap[state->BreedId].push_back(state);
    }

    for (uint32 i = 0; i < sBattlePetSpeciesXAbilityStore.GetNumRows(); ++i)
    {
        BattlePetSpeciesXAbilityEntry const* entry = sBattlePetSpeciesXAbilityStore.LookupEntry(i);
        if (!entry)
            continue;

        sBattlePetSpeciesXAbilityMap[entry->SpeciesId].push_back(entry);
    }

    LoadDB2(bad_db2_files, sItemToBattlePetStore, db2Path, "ItemToBattlePet.db2");
    LoadDB2(bad_db2_files, sItemStore, db2Path, "Item.db2");
    LoadDB2(bad_db2_files, sItemCurrencyCostStore, db2Path, "ItemCurrencyCost.db2");
    LoadDB2(bad_db2_files, sItemSparseStore, db2Path, "Item-sparse.db2");
    LoadDB2(bad_db2_files, sItemExtendedCostStore, db2Path, "ItemExtendedCost.db2");
    LoadDB2(bad_db2_files, sSpellReagentsStore, db2Path, "SpellReagents.db2");
    LoadDB2(bad_db2_files, sItemUpgradeStore, db2Path, "ItemUpgrade.db2");
    LoadDB2(bad_db2_files, sRulesetItemUpgradeStore, db2Path, "RulesetItemUpgrade.db2");
    for (uint32 i = 0; i < sRulesetItemUpgradeStore.GetNumRows(); ++i)
    {
        if (RulesetItemUpgradeEntry const* ruleset = sRulesetItemUpgradeStore.LookupEntry(i))
        {
            ItemSparseEntry const* sparse = sItemSparseStore.LookupEntry(ruleset->ItemId);
            if (!sparse)
                continue;

            if (!(sparse->Flags3 & ITEM_FLAGS_EXTRA2_UPGRADEABLE))
            {
                sLog->outError(LOG_FILTER_GENERAL, "Item %u listed in sRulesetItemUpgradeStore id %u, but dont have ITEM_FLAGS_EXTRA2_UPGRADEABLE flags (%u)", ruleset->ItemId, ruleset->Id, sparse->Flags3);
                const_cast<ItemSparseEntry*>(sparse)->Flags3 |= ITEM_FLAGS_EXTRA2_UPGRADEABLE;
            }

            if (!sItemUpgradeStore.LookupEntry(ruleset->UpgradeId))
            {
                sLog->outError(LOG_FILTER_GENERAL, "Upgrade %u listed in sRulesetItemUpgradeStore id %u not exist", ruleset->UpgradeId, ruleset->Id);
                continue;
            }

            // Legendary back has 2\4 in db2
            if (ruleset->UpgradeId == 493)
                const_cast<RulesetItemUpgradeEntry*>(ruleset)->UpgradeId = 491;
            
            sItemRulesetUpgradeMap[ruleset->ItemId].push_back(ruleset);
        }
    }

    LoadDB2(bad_db2_files, sSpellVisualEntryStore, db2Path, "SpellVisual.db2");
    LoadDB2(bad_db2_files, sMapChallengeModeStore, db2Path, "MapChallengeMode.db2");
    LoadDB2(bad_db2_files, sSceneScriptPackageStore, db2Path, "SceneScriptPackage.db2");

    for (uint32 i = 0; i < sItemToBattlePetStore.GetNumRows(); ++i)
    {
        if (ItemToBattlePetEntry const* itemEntry = sItemToBattlePetStore.LookupEntry(i))
        {
            ItemSparseEntry const* sparse = sItemSparseStore.LookupEntry(itemEntry->ItemId);
            if (!sparse)
                continue;

            ASSERT(sparse->SpellId[0] == 483 || sparse->SpellId[0] == 55884);

            uint32 learning_spell_id = sparse->SpellId[1];
            ASSERT(learning_spell_id > 0);

            sSpellToBattlePetItemMap[learning_spell_id] = itemEntry->SpeciesId;
        }
    }
    // error checks
    if (bad_db2_files.size() >= DB2FilesCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "\nIncorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found by path: %sdb2", DB2FilesCount, dataPath.c_str());
        exit(1);
    }
    else if (!bad_db2_files.empty())
    {
        std::string str;
        for (std::list<std::string>::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount,str.c_str());
        exit(1);
    }

    // Check loaded DB2 files proper version
    if (!sItemStore.LookupEntry(106130)             ||      // last item added in 5.4 (17371)
        !sItemExtendedCostStore.LookupEntry(5268)  )        // last item extended cost added in 5.4 (17371)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Please extract correct db2 files from client 5.4 17371.");
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_GENERAL, ">> Initialized %d DB2 data stores.", DB2FilesCount);
}

#define BATTLE_PET_MAIN_STAT_VALUE  8.0f
#define BATTLE_PET_MAIN_STAT_DIV    200.0f
#define BATTLE_PET_MAIN_STAT_OFFSET 1600.0f

float BattlePetSpeciesMainStat(uint16 stateId, uint16 speciesId)
{
    if (stateId != BATTLE_PET_STATE_STAT_POWER
        && stateId != BATTLE_PET_STATE_STAT_STAMINA
        && stateId != BATTLE_PET_STATE_STAT_SPEED)
        return 0.0f;

    BattlePetSpeciesStatesMap::iterator itr = sBattlePetSpeciesStatesMap.find(speciesId);
    if (itr == sBattlePetSpeciesStatesMap.end())
        return BATTLE_PET_MAIN_STAT_VALUE;

    for (BattlePetSpeciesStateEntry const* state : itr->second)
    {
        if (state->StateId == stateId)
            return BATTLE_PET_MAIN_STAT_VALUE + ((float)state->Modifier / BATTLE_PET_MAIN_STAT_DIV);
    }

    return BATTLE_PET_MAIN_STAT_VALUE;
}

float BattlePetBreedMainStatModifier(uint16 stateId, uint8 breedId)
{
    if (stateId != BATTLE_PET_STATE_STAT_POWER
        && stateId != BATTLE_PET_STATE_STAT_STAMINA
        && stateId != BATTLE_PET_STATE_STAT_SPEED)
        return 0.0f;


    BattlePetBreedStatesMap::iterator itr = sBattlePetBreedStatesMap.find(breedId);
    if (itr == sBattlePetBreedStatesMap.end())
        return 0.0f;

    for (BattlePetBreedStateEntry const* state : itr->second)
    {
        if (state->StateId == stateId)
            return ((float)state->Value - BATTLE_PET_MAIN_STAT_OFFSET) / BATTLE_PET_MAIN_STAT_DIV;
    }

    return NULL;
}

uint32 GetSpeciesForSpellId(uint32 spellId)
{
    BattlePetSpellToSpeciesMap::const_iterator itr = sBattlePetSpellToSpeciesMap.find(spellId);
    if (itr != sBattlePetSpellToSpeciesMap.end())
        return itr->second;

    return 0;
}

SpellReagentsEntry const* GetSpellReagentEntry(uint32 spellId, uint8 reagent)
{
    SpellReagentMap::const_iterator itr = sSpellReagentMap.find(spellId);
    if (itr == sSpellReagentMap.end())
        return NULL;

    return itr->second.reagents[reagent];
}
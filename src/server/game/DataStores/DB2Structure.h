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

#ifndef TRINITY_DB2STRUCTURE_H
#define TRINITY_DB2STRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Define.h"
#include "Path.h"
#include "Util.h"
#include "SharedDefines.h"

#include <map>
#include <set>
#include <vector>

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

// Structures used to access raw DB2 data and required packing to portability
struct ItemEntry
{
    uint32   ID;                                             // 0
    uint32   Class;                                          // 1
    uint32   SubClass;                                       // 2
    int32    SoundOverrideSubclass;                          // 3
    int32    Material;                                       // 4
    uint32   DisplayId;                                      // 5
    uint32   InventoryType;                                  // 6
    uint32   Sheath;                                         // 7
};

struct ItemCurrencyCostEntry
{
    //uint32  Id;
    uint32  ItemId;
};

#define MAX_ITEM_PROTO_DAMAGES 2                            // changed in 3.1.0
#define MAX_ITEM_PROTO_SOCKETS 3
#define MAX_ITEM_PROTO_SPELLS  5
#define MAX_ITEM_PROTO_STATS  10

// 5.4.0 17399 check @author : Izidor
struct ItemSparseEntry
{
    uint32     ID;                                           // 0
    uint32     Quality;                                      // 1
    uint32     Flags;                                        // 2
    uint32     Flags2;                                       // 3
    uint32     Flags3;                                       // 4
    float      Unk430_1;                                     // 5
    float      Unk430_2;                                     // 6
    uint32     BuyCount;                                     // 7
    uint32     BuyPrice;                                     // 8
    uint32     SellPrice;                                    // 9
    uint32     InventoryType;                                // 10
    int32      AllowableClass;                               // 11
    int32      AllowableRace;                                // 12
    uint32     ItemLevel;                                    // 13
    int32      RequiredLevel;                                // 14
    uint32     RequiredSkill;                                // 15
    uint32     RequiredSkillRank;                            // 16
    uint32     RequiredSpell;                                // 17
    uint32     RequiredHonorRank;                            // 18
    uint32     RequiredCityRank;                             // 19
    uint32     RequiredReputationFaction;                    // 20
    uint32     RequiredReputationRank;                       // 21
    uint32     MaxCount;                                     // 22
    uint32     Stackable;                                    // 23
    uint32     ContainerSlots;                               // 24
    int32      ItemStatType[MAX_ITEM_PROTO_STATS];           // 25 - 35
    int32      ItemStatValue[MAX_ITEM_PROTO_STATS];          // 36 - 46
    int32      ItemScalingValue[MAX_ITEM_PROTO_STATS];       // 47 - 57
    float      ItemSocketCostRate[MAX_ITEM_PROTO_STATS];     // 58 - 68
    uint32     ScalingStatDistribution;                      // 69
    uint32     DamageType;                                   // 70
    uint32     Delay;                                        // 71
    float      RangedModRange;                               // 72
    int32      SpellId[MAX_ITEM_PROTO_SPELLS];               //
    int32      SpellTrigger[MAX_ITEM_PROTO_SPELLS];          //
    int32      SpellCharges[MAX_ITEM_PROTO_SPELLS];          //
    int32      SpellCooldown[MAX_ITEM_PROTO_SPELLS];         //
    int32      SpellCategory[MAX_ITEM_PROTO_SPELLS];         // 
    int32      SpellCategoryCooldown[MAX_ITEM_PROTO_SPELLS]; //
    uint32     Bonding;                                      // 99
    char*      Name;                                         // 100
    char*      Name2;                                        // 101
    char*      Name3;                                        // 102
    char*      Name4;                                        // 103
    char*      Description;                                  // 104
    uint32     PageText;                                     // 105
    uint32     LanguageID;                                   // 106
    uint32     PageMaterial;                                 // 107
    uint32     StartQuest;                                   // 108
    uint32     LockID;                                       // 109
    int32      Material;                                     // 110
    uint32     Sheath;                                       // 111
    uint32     RandomProperty;                               // 112
    uint32     RandomSuffix;                                 // 113
    uint32     ItemSet;                                      // 114
    uint32     Area;                                         // 115
    uint32     Map;                                          // 116
    uint32     BagFamily;                                    // 117
    uint32     TotemCategory;                                // 118
    uint32     Color[MAX_ITEM_PROTO_SOCKETS];                // 119 - 121
    uint32     Content[MAX_ITEM_PROTO_SOCKETS];              // 122 - 124
    int32      SocketBonus;                                  // 125
    uint32     GemProperties;                                // 126
    float      ArmorDamageModifier;                          // 127
    uint32     Duration;                                     // 128
    uint32     ItemLimitCategory;                            // 129
    uint32     HolidayId;                                    // 130
    float      StatScalingFactor;                            // 131
    int32      CurrencySubstitutionId;                       // 132
    int32      CurrencySubstitutionCount;                    // 133
};

struct ItemUpgradeEntry
{
    uint32 Id;
    uint32 itemUpgradePath;
    uint32 itemLevelUpgrade;
    uint32 precItemUpgradeId;
    uint32 currencyId;
    uint32 currencyCost;
};

struct RulesetItemUpgradeEntry
{
    uint32 Id;
    uint32 Index;
    uint32 UpgradeId;
    uint32 ItemId;
};

#define MAX_ITEM_EXT_COST_ITEMS         5
#define MAX_ITEM_EXT_COST_CURRENCIES    5

struct ItemExtendedCostEntry
{
    uint32      ID;                                                     // 0 extended-cost entry id
    //uint32    reqhonorpoints;                                         // 1 required honor points, only 0
    //uint32    reqarenapoints;                                         // 2 required arena points, only 0
    uint32      RequiredArenaSlot;                                      // 3 arena slot restrictions (min slot value)
    uint32      RequiredItem[MAX_ITEM_EXT_COST_ITEMS];                  // 4-8 required item id
    uint32      RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS];             // 9-13 required count of 1st item
    uint32      RequiredPersonalArenaRating;                            // 14 required personal arena rating
    //uint32    ItemPurchaseGroup;                                      // 15, only 0
    uint32      RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];         // 16-20 required curency id
    uint32      RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];    // 21-25 required curency count
    //uint32    Unk_1;                                                  // 26 Only 0
    //uint32    Unk_2;                                                  // 27 Only 0
    //uint32    Unk_3;                                                  // 28
    //uint32    Unk_4;                                                  // 29 Only 0
    //uint32    Unk_5;                                                  // 30 Only 0
};

struct MapChallengeModeEntry
{
    uint32 ID;                  ///< 0
    uint32 MapID;               ///< 1
    //uint32 Field2;            ///< 2
    //uint32 Field3;            ///< 3
    //uint32 Field4;            ///< 4
    uint32 BronzeTime;          ///< 5
    uint32 SilverTime;          ///< 6
    uint32 GoldTime;            ///< 7
    //uint32 Field8;            ///< 8
    //uint32 Field9;            ///< 9
};

struct SceneScriptPackageEntry
{
    uint32 Entry;
    const char* Name;
};

// Structures used to access raw DB2 data and required packing to portability
struct BattlePetAbilityEntry
{
    uint32 AbilityId;                                       // 0 - battle pet ability id
    int32 FamilyId;                                         // 1 - battle pet family id or -1 for aura
    uint32 IconId;                                          // 2 - icon id
    uint32 Cooldown;                                        // 3 - cooldown in turns
    uint32 VisualId;                                        // 4 - index of BattlePetVisual.db2
    uint32 Flags;                                           // 5 - flags
    char* Name;                                  // 6 - name text
    char* Description;                           // 7 - description text

    //bool IsAura() const { return FamilyId == -1 || Flags == 6; }
};

struct BattlePetAbilityStateEntry
{
    uint32 Id;                                              // 0
    uint32 AbilityId;                                       // 1 - battle pet ability id (BattlePetAbility.db2)
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    int32 Value;                                           // 3 - value for state
};

#define MAX_BATTLE_PET_EFFECT_VALUES_COUNT 6

struct BattlePetAbilityEffectEntry
{
    uint32 Id;                                              // 0
    uint32 AbilityTurnId;                                   // 1 - index of BattlePetAbilityTurn.db2
    uint32 VisualId;                                        // 2 - index of BattlePetVisual.db2
    uint32 TriggerId;                                       // 3 - triggered ability
    uint32 Effect;                                          // 4 - index of BattlePetEffectProperties.db2
    uint32 EffectIndex;                                     // 5 - index

    union
    {
        struct
        {
            int32 m_points;
            int32 m_accuracy;
        } heal;
        struct
        {
            int32 m_pct;
            int32 m_accuracy;
        } healPct;
        struct
        {
            int32 m_points;
            int32 m_accuracy;
            int32 m_isPeriodic;
            int32 m_overrideIndex;
        } damage;
        struct
        {
            int32 m_baseChanceToSucceed;
            int32 m_increasePetToss;
        } catchPet;
        struct
        {
            int32 m_unused;
            int32 m_accuracy;
            int32 m_duration;
        } triggerAura;
        struct
        {
            int32 m_chainFailure;
            int32 m_accuracy;
            int32 m_duration;
            int32 m_maxAllowed;
        } triggerAuraPeriodicDamage;
        struct
        {
            int32 m_points;
            int32 m_accuracy;
            int32 m_casterState;
            int32 m_targetState;
            int32 m_isPeriodic;
        } damage2;
        struct
        {
            int32 m_points;
            int32 m_accuracy;
            int32 m_isPeriodic;
        } damagePctFromHp;
        struct
        {
            int32 m_power;
            int32 m_accuracy;
        } damageFromTakenLastRound;
        struct
        {
            int32 m_chainFailure;
            int32 m_accuracy;
            int32 m_duration;
            int32 m_maxAllowed;
            int32 m_casterState;
            int32 m_targetState;
        } addAuraCondition;
        struct
        {
            int32 EffectValues[MAX_BATTLE_PET_EFFECT_VALUES_COUNT];// 6-11 effect values, description in BattlePetEffectProperties.db2
        } raw;
    };
};

struct BattlePetEffectPropertiesEntry
{
    uint32 Id;                                              // 0
    uint32 VisualId;                                        // 1    - index of BattlePetVisual.db2
    uint32 Type[MAX_BATTLE_PET_EFFECT_VALUES_COUNT];        // 2-7  - Type
    uint32 Values[MAX_BATTLE_PET_EFFECT_VALUES_COUNT];      // 8-13 - Values
};

struct BattlePetAbilityTurnEntry
{
    uint32 Id;                                              // 0
    uint32 AbilityId;                                       // 1 - index of BattlePetAbility.db2
    uint32 VisualId;                                        // 2 - index of BattlePetVisual.db2
    uint32 Index;                                           // 3
    uint32 HasProcData;                                     // 4
    uint32 ProcType;                                        // 5
};

struct BattlePetBreedStateEntry
{
    uint32 Id;                                              // 0
    uint32 BreedId;                                         // 1 - battle pet breed id
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    int32 Value;                                            // 3 - value for state
};



typedef std::vector<BattlePetAbilityEffectEntry const*> BattlePetAbilityEffectVector;
typedef std::vector<BattlePetAbilityStateEntry const*> BattlePetAbilityStateVector;

typedef std::map<uint8 /*turnIndex*/, BattlePetAbilityEffectVector> BattlePetAbilityTurns;

typedef std::map<uint32 /*abilityId*/, BattlePetAbilityTurns> BattlePetAbilityEffectsMap;
typedef std::map<uint32 /*abilityId*/, BattlePetAbilityStateVector> BattlePetAbilityStatesMap;

typedef std::set<uint32> BattlePetBreedSet;
typedef std::map<uint32 /*spellId*/, uint32 /*speciesId*/> BattlePetSpellToSpeciesMap;

struct BattlePetSpeciesEntry
{
    uint32 SpeciesId;                                       // 0 - battle pet species id
    uint32 NpcId;                                           // 1 - npc id
    uint32 IconId;                                          // 2 - icon id
    uint32 SpellId;                                         // 3 - summon spell id (Spell.dbc)
    uint32 FamilyId;                                        // 4 - battle pet family id
    //int32 SourceType;                                     // 5
    uint32 Flags;                                           // 6 - flags (see BattlePetSpeciesFlags enum)
    char* Description;                           // 7 - description text, contains method to obtain and cost
    char* Flavor;                                // 8 - flavor text

    bool HasFlag(uint32 flag) const
    {
        return (Flags & flag) != 0;
    }
};

struct BattlePetSpeciesStateEntry
{
    uint32 Id;                                              // 0
    uint32 SpeciesId;                                       // 1 - battle pet species id (BattlePetSpecies.db2)
    uint32 StateId;                                         // 2 - battle pet state id (BattlePetState.db2)
    int32 Modifier;                                         // 3 - modifier value for state
};

struct BattlePetSpeciesXAbilityEntry
{
    uint32 Id;                                              // 0
    uint32 SpeciesId;                                       // 1 - battle pet species id (BattlePetSpecies.db2)
    uint32 AbilityId;                                       // 2 - battle pet ability id (BattlePetAbility.db2)
    uint32 RequiredLevel;                                   // 3 - required level to use this ability
    uint32 SlotId;                                          // 4 - ability slot id (0-2)
};

typedef std::vector<BattlePetSpeciesStateEntry const*> BattlePetSpeciesStateVector;
typedef std::vector<BattlePetBreedStateEntry const*> BattlePetBreedStateVector;
typedef std::vector<BattlePetSpeciesXAbilityEntry const*> BattlePetSpeciesXAbilityVector;
typedef std::map<uint32/*speciesId*/, BattlePetSpeciesStateVector> BattlePetSpeciesStatesMap;
typedef std::map<uint32/*breedId*/, BattlePetBreedStateVector> BattlePetBreedStatesMap;
typedef std::map<uint32/*speciesId*/, BattlePetSpeciesXAbilityVector> BattlePetSpeciesXAbilityMap;

struct BattlePetStateEntry
{
    uint32 StateId;                                         // 0 - battle pet state id
    //uint32 Unk1;                                          // 1 - only states 21 and 34 (linked states?)
    char* Name;                                  // 2 - name text
    uint32 Flags;                                           // 3 - flags
};

typedef std::map<uint32 /*SpellId*/, uint32 /*ItemId*/> SpellToItemMap;
struct ItemToBattlePetEntry
{
    uint32 ItemId; // 0 - item id
    uint32 SpeciesId; // 1 - battle pet species id (BattlePetSpecies.db2)
};

struct SpellVisualEntry
{
    uint32 Id;                                              // 0 m_Id
    /* 1 - 24 */
    uint32 EnemyId;                                         // 25 m_enemyId;
};

#define MAX_SPELL_REAGENTS 8

// SpellReagents.dbc
// @author Selenium: 5.4 valid
struct SpellReagentsEntry
{
    //uint32    Id;                                         // 0        m_ID
    int32     Reagent[MAX_SPELL_REAGENTS];                  // 1-9      m_reagent
    uint32    ReagentCount[MAX_SPELL_REAGENTS];             // 10-18    m_reagentCount
    uint32    CurrencyID;                                   // 19       m_CurrencyID
    uint32    CurrencyCount;                                // 20       m_CurrencyCount
};

struct SpellReagent
{
    SpellReagent()
    {
        reagents[0] = NULL;
        reagents[1] = NULL;
        reagents[2] = NULL;
        reagents[3] = NULL;
        reagents[4] = NULL;
        reagents[5] = NULL;
        reagents[6] = NULL;
        reagents[7] = NULL;
    }
    SpellReagentsEntry const* reagents[MAX_SPELL_REAGENTS];
};

typedef std::map<uint32, SpellReagent> SpellReagentMap;

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

#endif
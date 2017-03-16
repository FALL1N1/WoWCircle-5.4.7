#pragma once

#include "Define.h"

#define BATTLE_PET_MAX_JOURNAL_SPECIES 3   // client modifications required to increase
#define BATTLE_PET_MAX_JOURNAL_PETS    500 // sent to client as 25 bits (theoretical max 33,554,431)
#define BATTLE_PET_MAX_LOADOUT_SLOTS   3
#define BATTLE_PET_INITIAL_LEVEL 1
#define BATTLE_PET_MAX_LEVEL       25
#define BATTLE_PET_MAX_NAME_LENGTH 16

#define PET_BATTLE_OPPONENT_COUNT 2

#define PET_BATTLE_TRAP_ABILITY_ID 427
#define PET_BATTLE_PVP_MAX_ROUND_SEC 30
#define PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC 30

#define PET_BATTLE_FORFEIT_PENALTY 10 // 10 pct of health

enum BattlePetLoadoutSlots
{
    BATTLE_PET_LOADOUT_SLOT_1                   = 0,
    BATTLE_PET_LOADOUT_SLOT_2                   = 1,
    BATTLE_PET_LOADOUT_SLOT_3                   = 2,
    BATTLE_PET_LOADOUT_SLOT_NONE
};

// custom flags used in 'account_battle_pet_slots' db table
enum BattlePetLoadoutFlags
{
    BATTLE_PET_LOADOUT_SLOT_FLAG_NONE           = 0x0,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_1         = 0x1,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_2         = 0x2,
    BATTLE_PET_LOADOUT_SLOT_FLAG_SLOT_3         = 0x4
};

// flags used in 'account_battle_pet' db table
enum BattlePetJournalFlags
{
    BATTLE_PET_JOURNAL_FLAG_NONE                = 0x00,
    BATTLE_PET_JOURNAL_FLAG_FAVORITES           = 0x01,
    BATTLE_PET_JOURNAL_FLAG_COLLECTED           = 0x02, // name dumped from client, use unknown
    BATTLE_PET_JOURNAL_FLAG_NOT_COLLECTED       = 0x04, // IsRevoked
    BATTLE_PET_JOURNAL_FLAG_UNKNOWN_1           = 0x08, // IsLockedForConvert
    BATTLE_PET_JOURNAL_FLAG_ABILITY_1           = 0x10, // ability flags are set if the second ability for that slot is selected
    BATTLE_PET_JOURNAL_FLAG_ABILITY_2           = 0x20, // ...
    BATTLE_PET_JOURNAL_FLAG_ABILITY_3           = 0x40  // ...
};

enum BattlePetSpells
{
    SPELL_BATTLE_PET_TRAINING_PASSIVE           = 119467,
    SPELL_TRACK_PETS                            = 122026,
    SPELL_REVIVE_BATTLE_PETS                    = 125439
};

enum PetBattleEvents
{
    SET_HEALTH = 0,
    AURA_APPLY,
    AURA_CANCEL,
    AURA_CHANGE,
    PET_SWAP,
    STATUS_CHANGE,
    SET_STATE,
    SET_MAX_HEALTH,
    SET_SPEED,
    SET_POWER,
    TRIGGER_ABILTY,
    ABILITY_CHANGE,
    NPC_EMTOE, // just as in client at 0x00B616F0
    AURAS_BEGIN,
    AURAS_END,

    MAX,
};

enum PBOID : uint32
{
    P0_PET_0 = 0,
    P0_PET_1,
    P0_PET_2,
    P1_PET_0,
    P1_PET_1,
    P1_PET_2,
    PAD_0,
    PAD_1,
    WEATHER,
    INVALID,
    INVALID_TARGET,
    MISS,
    CRIT,
    BLOCKED,
};

DEFINE_ENUM_BIT_OPS(PBOID, uint32);

enum TrapStatus
{
    TRAP_STATUS_OK = 0,
    TRAP_STATUS_PET_LEVEL = 2, // one of the pets should have level >= 3
    TRAP_STATUS_OPPONENT_PET_IS_DIED = 3,
    TRAP_STATUS_OPPONENT_PET_LOW_HP = 4,
    TRAP_STATUS_PET_SPECIES_MAX = 5, // already have 3 pets same type
    TRAP_STATUS_CANT_CATCH_OPPONENTS_PET = 6,
    TRAP_STATUS_CANT_CATCH_NPCS_PETS = 7,
    TRAP_STATUS_PET_ALREADY_CATCHED = 8, // pet already captured during this battle
};

enum BattlePetActionType : uint32
{
    BATTLE_PET_ACTION_FORFEIT = 0,
    BATTLE_PET_ACTION_USE_ABILITY = 1,
    // 3 - use trap
    BATTLE_PET_ACTION_CHANGE_PET = 2,
    BATTLE_PET_ACTION_BATTLE_END = 4,
};

#define BATTLE_PET_ACTION_MAX 5

enum BattlePetDisableFlags
{
    DisableNone = 0x0,
    Pass = 0x1,
    Ability = 0x2,
    Swap = 0x4,
    Unk = 0x8,
};

DEFINE_ENUM_BIT_OPS(BattlePetDisableFlags, int);

enum BattlePetEffectFlags
{
    None = 0x0,
    Unk1 = 0x1,
    Miss = 0x2,
    Crit = 0x4,
    Blocked = 0x8,
    Dodge = 0x10,
    Heal = 0x20,
    Unk2 = 0x40, // regular damage log, same as 0x1000
    Reflect = 0x80,
    Unk3 = 0x100, // empty log, same as 0x1
    Immune = 0x200,
    Strong = 0x400,
    Weak = 0x800,
    Unk4 = 0x1000,
};

enum PetBattleEndReason : uint32
{
    Default = 0,
    Forfeit,
};

#ifndef ATTACK_ON_ZANVESS_H
#define ATTACK_ON_ZANVESS_H

#define SCENARIO_ID_ALLIANCE 203
#define SCENARIO_ID_HORDE 181

// To find criteria id:
// 1. check ScenarioStep.dbc . 
//     Structure for 5.4.7: 0 - Row Id, 1 - CriteriaTreeId, 2 - Scenario Id, 3 - step
// 2. check CriteriaTree.dbc
//     Structure for 5.4.7: 0 - Row Id, 1 - CriteriaID, 5 - CriteriaTreeId from ScenarioStep.dbc
// 3. So you've just found CriteriaId from CriteriaTree.dbc, use it
//

enum Criterias
{
    CRITERIA_JOIN_TO_ATTACK     = 22294, // step 1
    CRITERIA_DESTROY_TOWERS_1   = 22295, // step 2
    CRITERIA_DESTROY_TOWERS_2   = 22296, // step 2
    CRITERIA_DESTROY_TOWERS_3   = 22297, // step 2
    CRITERIA_HEART_OF_ZANVESS   = 22301, // step 3
    CRITERIA_KILL_SCOOTER       = 22300, // step 3 (for horde)
    CRITERIA_KILL_BOSH          = 22483, // step 3 (for alliance)
    CRITERIA_KILL_TELVRAK       = 22299, // step 4
};

enum Data
{
    DATA_JOINED_THE_ATTACK  = 1,
    DATA_DESTROYED_TOWERS_1 = 2,
    DATA_DESTROYED_TOWERS_2 = 3,
    DATA_DESTROYED_TOWERS_3 = 4,
    DATA_HEART_OF_ZANVESS   = 5,
    DATA_KILLED_SCOOTER     = 6,
    DATA_KILLED_BOSH        = 7,
    DATA_KILLED_TELVRAK     = 8,
};

enum Creatures
{
    // Alliance
    NPC_SKY_ADMIRAL_ROGERS      = 68828,
    NPC_SKY_MARSHAL             = 68833,
    NPC_TINKMASTER_OVERSPARK    = 68832, // trader
    NPC_SKYFIRE_ENGINEER        = 68829,

    NPC_7TH_LEGION_PRIEST       = 67892,
    NPC_7TH_LEGION_INFILTRATOR  = 67727,
    NPC_TEAM_LEADER_SCOOTER     = 67810,

    NPC_SKYFIRE_GYROCOPTER      = 68115,

    // Horde
    NPC_REAR_ADMIRAL_ZIGG       = 67799,
    NPC_KORKRON_MARINE          = 68857,
    NPC_CHIEF_SAPPER_GLINT      = 68863, // trader
    NPC_NALLEY                  = 68831,
    NPC_KORKRON_ENGINEER        = 68862,
    NPC_KORKRON_FIRE_BRIGADE    = 68864,
    
    NPC_KORKRON_PRIEST          = 68142,
    NPC_KORKRON_INFILTRATOR     = 68141,
    NPC_SQUAD_LEADER_BOSH       = 68143,

    NPC_KORKRON_PILOT           = 68118,
    NPC_KORKRON_GUNSHIP         = 67275,

    // Mobs
    NPC_SCORPID_RELOCATOR       = 67784, // at 1 - 2 phases of the fight
    NPC_WILD_SCORPID            = 67712, // at 1 - 2 phases of the fight

    NPC_ZANTHIK_SWARMER         = 67887, // no loot
    NPC_ZANTHIK_SLASHER         = 67809,
    NPC_ZANTHIK_BRUTALIZER      = 67774,
    NPC_ZANTHIK_AMBERMANCER     = 62838,
    NPC_COMMANDER_TELVRAK       = 67879,
};

enum GameObjects
{

};

enum InstanceActions
{
    ACTION_TOWERS_DESTROYED = 1,
};

enum InstanceSpells
{
    SPELL_STRAFING_RUN_AURA     = 133676,
    SPELL_STRAFING_RUN          = 133703,
    SPELL_STRAFING_RUN_MISSILE  = 133714,
    SPELL_STRAFING_RUN_DMG      = 133713,
};

enum InstanceAreas
{
    AREA_HEART_OF_ZANVESS   = 6605,
};

const Position beachPos = {-1024.96f, 4243.30f, 3.41f, 2.99f};

struct ScenarioCreature
{
    enum SpawnMode : int
    {
        SPAWN_NONE  = 0,
        SPAWN_ON_STEP,
        SPAWN_AND_DESPAWN_ON_STEP,
    };

    ScenarioCreature()
    {
        creatureSpawnMode = SPAWN_NONE;
        creatureEntry = 0;
        creatureGuid = 0;
    }

    ScenarioCreature(SpawnMode spawn, uint32 entry, Position pos)
    {
        creatureSpawnMode = spawn;
        creatureEntry = entry;
        creaturePosition = pos;
        creatureGuid = 0;
    }

    bool ShouldSpawnOnStep() const
    {
        return creatureSpawnMode == SPAWN_ON_STEP || 
            creatureSpawnMode == SPAWN_AND_DESPAWN_ON_STEP;
    }

    bool ShouldDespawnOnNextStep() const
    {
        return creatureSpawnMode == SPAWN_AND_DESPAWN_ON_STEP;
    }

    bool IsSpawned() const
    {
        return creatureGuid != 0;
    }

    SpawnMode creatureSpawnMode;
    uint32 creatureEntry;
    uint64 creatureGuid;
    Position creaturePosition;
};

#endif
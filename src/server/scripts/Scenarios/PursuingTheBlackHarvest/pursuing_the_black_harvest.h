
#ifndef PURSUING_THE_BLACK_HARVEST_H
#define PURSUING_THE_BLACK_HARVEST_H

#define SCENARIO_ID 200

// To find criteria id:
// 1. check ScenarioStep.dbc . 
//     Structure for 5.4.7: 0 - Row Id, 1 - CriteriaTreeId, 2 - Scenario Id, 3 - step
// 2. check CriteriaTree.dbc
//     Structure for 5.4.7: 0 - Row Id, 1 - CriteriaID, 5 - CriteriaTreeId from ScenarioStep.dbc
// 3. So you've just found CriteriaId from CriteriaTree.dbc, use it
//

enum Criterias
{
    CRITERIA_ENTER_THE_SANCTUM          = 22336, // step 1
    CRITERIA_FIND_THE_WAY               = 22338, // step 2
    CRITERIA_CLICK_THE_SCROLL           = 22400, // step 2
    CRITERIA_FOLLOW_AKAMA               = 22339, // step 3
    CRITERIA_LOOK_AROUND_THE_SANCTUM    = 22344, // step 4
    CRITERIA_GET_PART_OF_SOUL           = 22341, // step 5
    CRITERIA_KILL_DEMONS                = 22345, // step 6
    CRITERIA_COLLECT_TREASURES          = 22382, // step 7
    CRITERIA_GET_TO_THE_END_TREASURES   = 22464, // step 7
    CRITERIA_GET_TO_THE_TOP             = 22376, // step 8
    CRITERIA_KILL_KANRETHAD             = 22756, // step 9
};

enum Data
{
    DATA_ENTER_THE_SANCTUM  = 1,
    DATA_KILL_KANRETHAD,

    DATA_SCENARIO_STEP,
};

enum Creatures
{
    NPC_ASHTONGUE_WORKER                = 68098,
    NPC_ASHTONGUE_SHAMAN                = 68129,
    NPC_ASHTONGUE_PRIMALIST             = 68096,

    DEMONIC_GATEWAY                     = 70028,
    DEMONIC_SOULWELL                    = 70052,

    NPC_KANRETHAD_EBONLOCKE_AT_END      = 69964,
    NPC_JUBEKA_SHADOWBREAKER_AT_END     = 70166, 
};

enum GameObjects
{

};

enum InstanceSpells
{
    SPELL_TRUSTED_BY_THE_ASHTONGUE  = 134206,
    SPELL_PLACE_EMPOWERED_SOULCORE  = 138680,
    SPELL_DEMONIC_GRASP             = 139142,
    SPELL_METAMORPHOSIS             = 138555,

    SPELL_DEMONIC_GATEWAY           = 139142,

    SPELL_DRAIN_ENERGY              = 139200,

    SPELL_FEL_ENERGY_1              = 140116, // stun
    SPELL_FEL_ENERGY_2              = 140135, // breath
    SPELL_FEL_ENERGY_3              = 140136, // light green field
    SPELL_FEL_ENERGY_4              = 140137, // gold-green ball
    SPELL_FEL_ENERGY_5              = 140141, // green pulsing ball
    SPELL_FEL_ENERGY_6              = 140159, // green pulsing cloud 
    SPELL_FEL_ENERGY_7              = 140160, // big green ball
    SPELL_FEL_ENERGY_8              = 140161, // green tentacles
    SPELL_FEL_ENERGY_9              = 140163, // green explosion
    SPELL_FEL_ENERGY_10             = 140164, // stand state

    SPELL_CODEX_OF_XERRATH_1        = 101508,
    SPELL_CODEX_OF_XERRATH_2        = 137206,
};

enum InstanceAreas
{
    AREA_TRAINING_GROUND        = 6614,
    AREA_SANCTUARY_OF_SHADOWS   = 6613,
};

enum InstanceQuests
{
    QUEST_INFILTRATING_THE_BLACK_TEMPLE = 32325,
};

enum InstanceQuestsCredits
{
    BLACK_TEMPLE_TOP_CREDIT = 68054,
};

const Position entrancePos = {709.29f, 980.98f, 52.95f, 4.69f};
const Position kanrethadEndPos = {655.89f, 305.25f, 354.22f, 6.26f};

#endif
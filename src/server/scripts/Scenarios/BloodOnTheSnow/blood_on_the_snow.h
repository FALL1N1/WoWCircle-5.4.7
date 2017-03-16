
#ifndef BLOOD_ON_THE_SNOW_H
#define BLOOD_ON_THE_SNOW_H

#define SCENARIO_ID 210

// To find criteria id:
// 1. check ScenarioStep.dbc . 
//     Structure for 5.4.7: 0 - Row Id, 1 - CriteriaTreeId, 2 - Scenario Id, 3 - step
// 2. check CriteriaTree.dbc
//     Structure for 5.4.7: 0 - Row Id, 1 - CriteriaID, 5 - CriteriaTreeId from ScenarioStep.dbc
// 3. So you've just found CriteriaId from CriteriaTree.dbc, use it
//

enum Criterias
{
    CRITERIA_TALK           = 23249, // step 1
    CRITERIA_STONEBEARD     = 23260, // step 2
    CRITERIA_BOLDBREW       = 23261, // step 3
    CRITERIA_FORGEFELLOW    = 23262, // step 4
    CRITERIA_ICE_SPIRIT     = 23263, // step 5
    CRITERIA_HEKIMA         = 23264, // step 6
    CRITERIA_VILLAGE        = 23265, // step 6
    CRITERIA_ADDITIONAL     = 23319, // step 7
};

enum Data
{
    DATA_MOIRA  = 1,
    DATA_VARIAN,

    DATA_CURRENT_STEP,

    DATA_TALK_GRIMBOLT,
    DATA_STONEBEARD,
    DATA_BOLDBREW,
    DATA_FORGEFELLOW,
    DATA_FARASTU,
    DATA_HEKIMA,
    DATA_VILLAGE,
};

enum InstanceSpells
{
    SPELL_CALL_THE_SHOT         = 141222,
    SPELL_CALL_THE_SHOT_AURA    = 141223, // dmg?
};

enum InstanceActions
{
    ACTION_COMPLETE_SCENARIO    = 1,
    ACTION_LAST_PHASE           = 2,
};

enum Creatures
{
    // alliance mobs
    NPC_PYROMANCER_FLAMEHEARTH  = 70709,
    NPC_CABALIST_DOOMBROW       = 70708,
    NPC_MOUNTAINEER_GRIMBOLT    = 70801, // at one point
    NPC_VARIAN_WRYNN            = 70799,
    NPC_STORMWIND_ROYAL_GUARD   = 67965,
    NPC_MOIRA_THAURISSAN        = 70279,
    NPC_DARK_IRON_FORGEGUARD    = 70275,
    NPC_DARK_IRON_MOUNTAINEER_1 = 70800, // groups at start
    NPC_DARK_IRON_MOUNTAINEER_2 = 70133, // at one point near grimbolt

    // quest targets
    NPC_SHIMMERWEED_BUSKET      = 71440,
    NPC_ROASTING_SPIT           = 70597,
    NPC_SCOUT_STONEBEARD        = 70602,
    NPC_SCOUT_FORGEFELLOW       = 70745,
    NPC_SCOUT_BOLDBREW          = 70744,

    // general mobs
    NPC_ZANDALARI_RAGE_BANNER   = 70789,
    NPC_FROSTMANE_WITCH_DOCTOR  = 70464,
    NPC_FROSTMANE_SNOWTALKER    = 70463,
    NPC_FROSTMANE_PROWLER       = 70595,
    NPC_FROSTMANE_MAULER        = 70684,
    NPC_FROSTMANE_HEADHUNTER    = 70473,
    NPC_FROSTMANE_FLESH_EATER   = 70746,
    NPC_FROSTMANE_BERSERKER     = 70471,
    NPC_FROSTMANE_BONECHILLER   = 70465,
    NPC_WATER_ELEMENTAL         = 70979,

    // bosses
    NPC_BONECHILLER_BONAFU      = 70468,
    NPC_SKULLCRUSHER_GUNDU      = 70787,
    NPC_HEKIMA_THE_WISE         = 70544,
    NPC_FARASTU                 = 70474,
    NPC_ICE_SPIKE               = 70988,
    NPC_SOLID_ICE               = 70987,

};

enum GameObjects
{
    GO_CAMPFIRE = 216323,
};

const Position moiraNearHekimaPos = {-5178.34f, -249.06f, 434.99f, 4.99f};
const Position varianNearHekimaPos = { -5171.66f, -249.58f, 434.99f, 4.52f};

#endif
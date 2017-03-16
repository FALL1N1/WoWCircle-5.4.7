#ifndef DEF_THRONE_OF_THUNDER_HPP
# define DEF_THRONE_OF_THUNDER_HPP

# include "ScriptPCH.h"
# include "MoveSplineInit.h"
# include "GameObjectAI.h"
# include "ObjectMgr.h"
# include "ScriptMgr.h"

# define MAX_ENCOUNTERS 13

# define RADEN_MAX_TRIES 30
# define RADEN_TRIES_WORLDSTATE 7844

enum Data
{
    // Bosses
	DATA_JIN_ROKH_BREAKER       = 0,
	DATA_HORRIDON               = 1,
	DATA_COUNCIL_OF_ELDERS      = 2,
	DATA_TORTOS                 = 3,
	DATA_MEGAERA                = 4,
    DATA_JI_KUN                 = 5,
	DATA_DURUMU_THE_FORGOTTEN   = 6,
	DATA_PRIMORDIUS             = 7,
    DATA_DARK_ANIMUS            = 8,
	DATA_IRON_QON               = 9,
	DATA_TWIN_CONSORTS          = 10,
    DATA_LEI_SHEN               = 11,
    DATA_RA_DEN                 = 12,  // Heroic only.

    // Council of Elders
	DATA_KAZRAJIN,
	DATA_SUL_THE_SANDCRAWLER,
	DATA_FROST_KING_MALAKK,
	DATA_HIGH_PRIESTESS_MARLI,

    // Iron Qon
    DATA_ROSHAK,
    DATA_QUETZAL,
    DATA_DAMREN,

    DATA_LULIN,            
    DATA_SUEN,

    // GameObjects

    // Jin'Rokh the Breaker
    DATA_MOGU_FOUNTAIN_NE,
    DATA_MOGU_FOUNTAIN_NW,
    DATA_MOGU_FOUNTAIN_SE,
    DATA_MOGU_FOUNTAIN_SW,

    DATA_HORRIDON_FARRAKI_DOOR,
    DATA_HORRIDON_GURUBASHI_DOOR,
    DATA_HORRIDON_DRAKKARI_DOOR,
    DATA_HORRIDON_AMANI_DOOR,

    DATA_LIGHTNING_SPAN,

    DATA_HORRIDON_INTRO,
    DATA_CRAZIED_STORM_CALLER,
    DATA_GARAZHAL_THE_SPIRITBINDER,
    DATA_LIGHTNING_SPAN_EVENT,

    DATA_MEGAERA_CHEST_10_NORMAL,
    DATA_MEGAERA_CHEST_25_NORMAL,
    DATA_MEGAERA_CHEST_10_HEROIC,
    DATA_MEGAERA_CHEST_25_HEROIC,

    DATA_CHARGING_STATION,
    DATA_SOUTH_PILLAR,
    DATA_EAST_PILLAR,
    DATA_WEST_PILLAR,
    DATA_NORTH_PILLAR,
    DATA_FLOOR_SOUTH,
    DATA_FLOOR_EAST,
    DATA_FLOOR_WEST,
    DATA_FLOOR_NORTH,
    DATA_WINDOW_NORTH_WEST,
    DATA_WINDOW_SOUTH_WEST,
    DATA_WINDOW_NORTH_EAST,
    DATA_WINDOW_SOUTH_EAST,

    DATA_RADEN_TRIES_COUNT,
    DATA_CACHE_OF_STORMS_10_HEROIC,
    DATA_CACHE_OF_STORMS_25_HEROIC,
};

enum Creatures
{
    NPC_JIN_ROKH_BREAKER            = 69465,

    NPC_HORRIDON                    = 68476,

    // Council of Elders
    NPC_KAZRAJIN                    = 69134,
    NPC_SUL_THE_SANDCRAWLER         = 69078,
    NPC_FROST_KING_MALAKK           = 69131,
    NPC_HIGH_PRIESTESS_MARLI        = 69132,
    NPC_GARAJAL_THE_SPIRITBINDER    = 69135,

    NPC_TORTOS                      = 67977,
    NPC_MEGAERA                     = 68065,
    NPC_JI_KUN                      = 69712,

    NPC_DURUMU_THE_FORGOTTEN        = 68036,
    NPC_PRIMORDIUS                  = 69017,
    NPC_DARK_ANIMUS                 = 69427,

    NPC_IRON_QON                    = 68078,
    NPC_ROSHAK                      = 68079,
    NPC_QUETZAL                     = 68080,
    NPC_DAMREN                      = 68081,

    // Twin Consorts
    NPC_LULIN                       = 68905,
    NPC_SUEN                        = 68904,

    NPC_LEI_SHEN                    = 68397,

    NPC_RA_DEN                      = 69473  // Heroic only.
};

enum GameObjects
{
    GO_FIRST_DOOR              = 218665,

    // Jin'Rokh the Breaker
    GO_MOGU_FOUNTAIN_NE        = 218678,
    GO_MOGU_FOUNTAIN_NW        = 218675,
    GO_MOGU_FOUNTAIN_SE        = 218677,
    GO_MOGU_FOUNTAIN_SW        = 218676,

    GO_JIN_ROKH_FRONT_DOOR     = 218664,     // door he is looking at
    GO_JIN_ROKH_BACK_DOOR      = 218663,     // door to the next boss
    GO_HORRIDON_FRONT_DOOR     = 218667,
    GO_HORRIDON_BACK_DOOR      = 218666,
    GO_HORRIDON_FARRAKI_DOOR   = 218672,
    GO_HORRIDON_GURUBASHI_DOOR = 218670,
    GO_HORRIDON_DRAKKARI_DOOR  = 218671,
    GO_HORRIDON_AMANI_DOOR     = 218673,
    GO_ZANDALARI_INNER_1       = 218658,
    GO_ZANDALARI_OUTER_1       = 218659, 
    GO_ZANDALARI_INNER_2       = 218660,
    GO_ZANDALARI_OUTER_2       = 218661, 
    GO_ZANDALARI_COUNCIL_LEFT  = 218655,
    GO_ZANDALARI_COUNCIL_RIGHT = 218656,
    GO_ZANDALARI_COUNCIL_EXIT  = 218657,
    GO_TORTOS_EXIT             = 218980,
    GO_MEGAERA_EXIT            = 218746,

    GO_LIGHTNING_SPAN          = 218869,

    GO_TORTOS_DEATH_COLLISION  = 218987,

    GO_RA_DEN_ENTRANCE         = 218553,

    GO_RA_DEN_EXIT             = 218555, // massive door

    GO_FIRST_MOGU_BLOOD_VAT    = 218858,
    GO_SECOND_MOGU_BLOOD_VAT   = 218859,
    GO_THIRD_MOGU_BLOOD_VAT    = 218860,
    GO_FOURTH_MOGU_BLOOD_VAT   = 218861,
    GO_FIFTH_MOGU_BLOOD_VAT    = 218862,
    GO_SIXTH_MOGU_BLOOD_VAT    = 218863,
    GO_SEVENTH_MOGU_BLOOD_VAT  = 218864,
    GO_EIGHTH_MOGU_BLOOD_VAT   = 218865,
    GO_NINTH_MOGU_BLOOD_VAT    = 218866,
    GO_TENTH_MOGU_BLOOD_VAT    = 218867,

    GO_ANCIENT_MOGU_BELL       = 218723,

    GO_MEGAERA_CHEST_10_NORMAL = 218805,
    GO_MEGAERA_CHEST_25_NORMAL = 218806,
    GO_MEGAERA_CHEST_10_HEROIC = 218807,
    GO_MEGAERA_CHEST_25_HEROIC = 218808,

    GO_FEATHER_OF_JI_KUN       = 218543,
    GO_JI_KUN_EXIT_DOOR        = 218888,

    GO_DURUMU_PLATFORM_1       = 218396, // entrance
    GO_DURUMU_PLATFORM_2       = 218395, // exit

    GO_PRIMORDIUS_ENTRY_GATE   = 218584, 
    GO_PRIMORDIUS_EXIT_GATE    = 218585, 

    GO_SEWER_GATE              = 218388,
    GO_WIND_WALL               = 214548,
    GO_DOOR_TO_THE_QUEENS_CHAMBER_1 = 218588,
    GO_DOOR_TO_THE_QUEENS_CHAMBER_2 = 218589,

    GO_CHAMBER_OF_THE_TWIN_CONSORTS = 218781,
    GO_TWIN_CONSORTS_PASSAGE   = 218394,
    
    GO_DISPLACEMENT_PAD_1       = 218417, // to lei shen
    GO_DISPLACEMENT_PAD_2       = 218418, // to balcony

    GO_CHARGING_STATION         = 218397,

    GO_SOUTH_PILLAR             = 218400,
    GO_EAST_PILLAR              = 218399,
    GO_WEST_PILLAR              = 218401,
    GO_NORTH_PILLAR             = 218398,

    GO_FLOOR_FX_SOUTH           = 218419,
    GO_FLOOR_FX_EAST            = 218422,
    GO_FLOOR_FX_WEST            = 218421,
    GO_FLOOR_FX_NORTH           = 218420,

    GO_NW_WINDOW                = 218409,
    GO_SW_WINDOW                = 218408,
    GO_NE_WINDOW                = 218411,
    GO_SE_WINDOW                = 218410,
    
    GO_WIND_FX_NW               = 218415,
    GO_WIND_FX_SW               = 218414,
    GO_WIND_FX_NE               = 218413,
    GO_WIND_FX_SE               = 218412,

    GO_CACHE_OF_STORMS_10_HEROIC    = 218997,
    GO_CACHE_OF_STORMS_25_HEROIC    = 218998,
};

const Position tortosLairPos = {6044.40f, 5104.51f, -42.0f, 4.70f};
const Position jikunExitPos = {6098.19f, 4425.92f, -6.20f, 2.13f};
const Position nearIronQonPos = {6045.81f, 4763.92f, 142.83f, 4.70f};

const Position jikunNonCombatPos = {6072.99f, 4450.16f, -118.0f, 5.29f};
const Position jikunEnter = {6094.51f, 4228.89f, -6.24f, 0.97f};
const Position jikunEnterLand = {6105.46f, 4294.65f, -30.f, 0.55f};
const Position jikunExit = {6123.58f, 4358.66f, -31.f, 2.06f};
const Position jikunExitSuccess = {6094.65f, 4408.54f, -6.0f, 2.16f};
const Position jikunExitFail = {6049.32f, 4165.80f, -12.0f, 1.11f};

const Position leiShenPos = {5738.43f, 4096.86f, 156.464f, 3.14501f};

#endif ///< DEF_THRONE_OF_THUNDER_HPP
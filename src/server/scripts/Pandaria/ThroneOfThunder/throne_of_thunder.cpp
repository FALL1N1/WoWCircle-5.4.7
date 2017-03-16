//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "throne_of_thunder.hpp"

// TODO: make scripts for rare elite npc
// example: No'ku Stormsayer

enum Spells
{
    // Zandalari Blade Initiate
    SPELL_WOUNDING_STRIKE   = 140049,

    // Zandalari Water Binder
    SPELL_BIND_WATER        = 137099,
    SPELL_BIND_WATER_SUMMON = 137100,
    SPELL_DELUGE            = 136881,
    SPELL_DELUGE_AURA       = 137087,
    SPELL_DELUGE_DMG        = 137088,
    SPELL_FROST_BOLT_AOE    = 136953,
    SPELL_FROST_BOLT_DMG    = 136952,

    // Zandalari Storm Caller
    SPELL_FOCUSED_LIGHTNING_AOE     = 139203, // aoe targetting
    SPELL_FOCUSED_LIGHTNING         = 139206, // summon sphere
    SPELL_FOCUSED_LIGHTNING_AOE2    = 139209, // aoe targetting
    SPELL_FOCUSED_LIGHTNING_DMG     = 139210, 
    SPELL_FOCUSED_LIGHTNING_DUMMY   = 139233, // visual sphere on trigger
    SPELL_STORM_WEAPON              = 139218,
    SPELL_STORM_WEAPON_DMG          = 139220,
    SPELL_WATER_BOLT_AOE            = 139231,
    SPELL_WATER_BOLT_DMG            = 139251,

    // Zandalari Spear Shaper
    SPELL_BERSERKING                = 137096,
    SPELL_THROW_SPEAR_MISSILE       = 136986,
    SPELL_THROW_SPEAR_SUMMON        = 136984,
    SPELL_THROW_SPEAR_AURA          = 137058,
    SPELL_THROW_SPEAR_DMG           = 136989,
    SPELL_THROW_SPEAR_DISARM        = 137066,
    SPELL_RETRIEVE_SPEAR            = 137070,
    SPELL_RETRIEVE_SPEAR_DMG        = 137072,
    SPELL_SPEAR_SPIN                = 137077,
    SPELL_SPEAR_SPIN_DMG            = 137079,

    // Zandalari Storm Caller2
    SPELL_STORM_WEAPON2             = 139319,
    SPELL_STORM_WEAPON_DMG2         = 139320,
    SPELL_STORM_ENERGY_AOE          = 139559,
    SPELL_STORM_ENERGY              = 139322,
    SPELL_STORM_ENERGY_DMG          = 139321,

    // Ancient Python
    SPELL_STEALTH                   = 139885,
    SPELL_ANCIENT_VENOM             = 139888,

    // Spirit Flayer
    SPELL_SPIRIT_LANTERN            = 139364,
    SPELL_SPIRIT_LIGHT              = 139461,

    // Tormented Spirit
    SPELL_TORMENT                   = 139550,

    // Soul-fed Construct
    SPELL_CRUSH_ARMOR               = 33661,
    SPELL_SPIRITFIRE_BEAM           = 139895,

    // Stormbringer Draz'kil
    SPELL_CHAIN_LIGHTNING           = 139903,
    SPELL_STORMCLOUD                = 139900,
    SPELL_STORMCLOUD_DMG            = 139901,

    // Gurubashi Berserker
    SPELL_BERSERKER_FRENZY          = 138427,
    SPELL_BERSERKER_FRENZY_AURA     = 138540,
    SPELL_BLOODLETTING              = 138693,

    // Drakkari Frost Warden
    SPELL_FROST_BULWARK             = 138668,
    SPELL_GLACIAL_FREEZE_TOTEM      = 138690,
    SPELL_GLACIAL_FREEZE            = 138678,
    SPELL_GLACIAL_FREEZE_AURA       = 138687,

    // Amanishi Flame Chanter
    SPELL_CALL_FLAMES               = 138607,
    SPELL_ERUPTION                  = 138652,
    SPELL_ERUPTION_AURA             = 138655,
    SPELL_ERUPTION_DMG              = 138658,
    SPELL_FIERY_CORE                = 138610,
    SPELL_MOLTEN_BARRAGE            = 138651,

    // Farraki Sand Conjurer
    SPELL_SAND_BOLT                 = 138739,
    SPELL_SAND_BOLT_DMG             = 138740,
    SPELL_CHOKING_SANDS             = 138742,
    SPELL_CONJURE_ELEMENTALS        = 140636,

    // Zandalari High Priest
    SPELL_LIGHT_OF_THE_LOA          = 139228,

    // Zandalari Prophet
    SPELL_MARK_OF_THE_PROPHET       = 140400,
    SPELL_VISIONS_OF_DEMISE         = 140115,
    SPELL_VISIONS_OF_DEMISE_DMG     = 140385,
    SPELL_VISIONS_OF_GRANDEUR       = 139205,

    // Zandalari Warlord
    SPELL_CLEAVE                    = 140414,
    SPELL_STRENGTH_OF_THE_LOA       = 140422,
    SPELL_ZANDALARI_WARCRY          = 134856,

    // Zandalari Prelate
    SPELL_SEAL_OF_THE_LOA           = 139212,
    SPELL_MARK_OF_THE_LOA           = 139213,
    SPELL_JUDGEMENT_OF_THE_LOA      = 139223,

    // Lei Shen npc
    SPELL_CALL_LIGHTNING            = 139853,
    SPELL_LIGHTNING_STRIKE          = 139854, 
    SPELL_TELEPORT_ALL_TO_DEPTH     = 139852,

    // Shale Stalker
    SPELL_SHALE_SHARDS              = 140616,

    // Fungal Growth
    SPELL_GROW                      = 140626,
    SPELL_FUNGI_SPORES              = 140620,
    SPELL_FUNGI_SPORES_DMG          = 140621,

    // Mist Lurker
    SPELL_CHOKING_MISTS             = 140682,
    SPELL_CORROSIVE_BREATH          = 140684,
    SPELL_CORROSIVE_BREATH_DMG      = 140685,
    SPELL_CORROSIVE_BREATH_AURA     = 140686,

    // Eternal Guardian
    SPELL_ETERNAL_PRISON            = 140629,
    SPELL_LIGHTNING_NOVA            = 140628,
    SPELL_SIPHON_LIFE               = 140630,
    SPELL_SIPHON_LIFE_2             = 140631,
    SPELL_ETERNAL_GUARDIAN_SPAWN    = 140577,
    SPELL_ACTIVATE_BELL             = 140627,
    SPELL_BELL_SHAKE                = 139179,

    // Cavern Burrower
    SPELL_SONIC_CALL                = 140600,
    SPELL_CRYSTAL_BARBS             = 140619,
    SPELL_CRUSH_ARMOR2              = 140618,
    SPELL_SUBMERGE                  = 140586,

    // Bore Worm
    SPELL_SHIFTING_DOOM             = 134452,

    // Bow Fly Swarm
    SPELL_LUCIFERASE                = 134470,
    SPELL_LUCIFERASE_DMG            = 134472,

    // Gastropod
    SPELL_ABRASIVE_RADULA           = 134414,
    SPELL_SNAIL_SHELL               = 134434,
    SPELL_FIXATED                   = 140306,
    SPELL_DEVOURED                  = 134415,
    SPELL_SLIME_TRAIL_AREATRIGGER   = 134397,
    SPELL_SLIME_TRAIL_AURA          = 134398,

    // Web and Corpse Spider
    SPELL_CORPSE_SPIDER_WEB_SPAWN   = 134483,
    SPELL_CORPSE_SPIDER_SPAWN       = 134481,
    SPELL_WEB_SPRAY                 = 139498,

    // Rotting Scavenger
    SPELL_INFECTED_BITE             = 139314,
    SPELL_ROT                       = 139315,
    SPELL_FRENZIED_COMSUMPTION      = 139311,
    SPELL_FRENZIED_COMSUMPTION_DMG  = 139313,

    // Putrid Waste
    SPELL_PUTRIFY                   = 139316,
    SPELL_PUTRIFY_DMG               = 139317,

    // Muck Bat
    SPELL_MUCK_SPIT                 = 139309,

    // Untrained Quilen
    SPELL_LEAPING_RUSH              = 121190,
    SPELL_CARNIVOROUS_BITE          = 122962,

    // Weisheng
    SPELL_TRIUMPHANT_ROAR           = 139815,

    // Manchu
    SPELL_TRIUMPHANT_RUSH           = 139803,
    SPELL_WILD_SMASH_AURA           = 139795,
    SPELL_WILD_SMASH_SCRIPT         = 139796,
    SPELL_WILD_SMASH_DMG            = 139797,
};

enum Adds
{
    NPC_ZANDALARI_BLADE_INITIATE    = 70230,
    NPC_ZANDALARI_WATER_BINDER      = 69455,
    NPC_BOUND_WATER_ELEMENTAL       = 69459,
    NPC_DRAWN_WATER                 = 69395,
    NPC_ZANDALARI_STORM_CALLER      = 69390,
    NPC_FOCUSED_LIGHTNING           = 70174,
    NPC_ZANDALARI_SPEAR_SHAPER      = 69388,
    MPC_ZANDALARI_STORM_CALLER2     = 70236,
    NPC_ANCIENT_PYTHON              = 70448,
    NPC_TORMENTED_SPIRIT            = 70341,
    NPC_SPIRIT_FLAYER               = 70246,
    NPC_SOUL_FED_CONSTRUCT          = 70308,
    NPC_STORMBRINGER_DRAZKIL        = 70445,
    NPC_GURUBASHI_BERSERKER         = 69905,
    NPC_DRAKKARI_FROST_WARDEN       = 69910,
    NPC_GLACIAL_FREEZE_TOTEM        = 70047,
    NPC_AMANISHI_FLAME_CHANTER      = 69909,
    NPC_ERUPTION                    = 70029,
    NPC_FARRAKI_SAND_CONJURER       = 69899,
    NPC_SAND_ELEMENTAL              = 69944,
    NPC_GURUBASHI_BERSERKER2        = 69916,
    NPC_ZANDALARI_HIGH_PRIEST       = 69906,
    NPC_ZANDALARI_PROPHET           = 70557,
    NPC_ZANDALARI_WARLORD           = 69911,
    NPC_ZANDALARI_PRELATE           = 69927,
    NPC_LEI_SHEN_EVENT              = 70437,
    NPC_MYSTERIOUS_MUSHROOM         = 70545,
    NPC_SHALE_STALKER               = 70587,
    NPC_FUNGAL_GROWTH               = 70153,
    NPC_MIST_LURKER                 = 70594,
    NPC_ETERNAL_GUARDIAN            = 70586,
    NPC_CAVERN_BURROWER             = 70589,
    NPC_BORE_WORM                   = 68221,
    NPC_BOW_FLY_SWARM               = 68222,
    NPC_GASTROPOD                   = 68220,
    NPC_CORPSE_SPIDER               = 68248,
    NPC_WEB                         = 68249,
    NPC_ROTTING_SCAVENGER           = 70224,
    NPC_PUTRID_WASTE                = 70219,
    NPC_MUCKBAT                     = 70232,
    NPC_UNTRAINED_QUILEN_1          = 70206,
    NPC_UNTRAINED_QUILEN_2          = 70209,
    NPC_MANCHU                      = 70202,
    NPC_WEISHENG                    = 70205,
};

const Position spiritflayerPos1[13] = 
{
    {5713.401855f, 6263.392578f, 138.038414f, 4.458073f},
    {5711.047363f, 6252.970703f, 138.035538f, 4.296194f},
    {5706.226563f, 6246.150391f, 137.974174f, 3.905241f},
    {5698.590332f, 6240.289551f, 137.984688f, 3.627296f},
    {5689.059570f, 6237.239746f, 137.986359f, 3.077517f},
    {5679.937012f, 6238.814941f, 137.986359f, 2.802627f},
    {5670.291504f, 6244.667969f, 137.986588f, 2.384182f},
    {5663.910156f, 6255.004883f, 137.990692f, 1.706121f},
    {5663.932617f, 6266.145508f, 137.991066f, 1.446503f},
    {5667.715820f, 6275.750977f, 137.986961f, 1.064711f},
    {5674.412598f, 6283.752441f, 137.984909f, 0.533258f},
    {5688.580078f, 6289.006348f, 137.987099f, 6.254448f},
    {5702.403320f, 6283.515137f, 137.987099f, 5.677179f}
};

const Position spiritflayerPos2[19] = 
{
    {5482.092285f, 6257.854492f, 132.047485f, 6.240122f},
    {5498.553711f, 6257.614746f, 132.058235f, 0.033293f},
    {5512.546875f, 6258.066406f, 130.872971f, 0.030239f},
    {5529.312988f, 6258.500977f, 131.944038f, 0.024130f},
    {5566.907715f, 6259.270996f, 132.055916f, 6.261502f},
    {5585.983887f, 6258.303223f, 132.047707f, 6.224852f},
    {5602.672363f, 6258.173828f, 130.871231f, 0.005807f},
    {5616.671875f, 6258.301270f, 131.109787f, 0.014970f},
    {5636.781250f, 6258.566895f, 132.047737f, 0.011915f},

    {5636.495605f, 6269.146484f, 132.047737f, 3.142600f},
    {5623.346680f, 6269.452148f, 132.047737f, 3.145654f},
    {5607.535156f, 6269.259277f, 130.872307f, 3.173144f},
    {5590.205566f, 6269.062500f, 131.355118f, 3.136491f},
    {5572.706055f, 6269.169434f, 132.054008f, 3.133437f},
    {5553.583984f, 6269.082031f, 132.055290f, 3.163980f},
    {5532.356445f, 6268.916504f, 132.056099f, 3.130383f},
    {5515.207520f, 6268.854004f, 130.872444f, 3.160926f},
    {5501.796387f, 6268.717285f, 131.479935f, 3.167035f},
    {5483.362305f, 6268.377441f, 132.049965f, 3.167035f}
};

const Position spiritflayerPos3[16] = 
{
    {5459.971680f, 6263.718262f, 138.345879f, 4.341642f},
    {5457.041504f, 6253.376953f, 138.441185f, 4.402731f},
    {5451.681641f, 6244.478027f, 138.441185f, 3.993451f},
    {5442.734375f, 6237.701172f, 138.441185f, 3.675802f},
    {5433.077637f, 6235.482910f, 138.422035f, 3.138239f},
    {5421.366211f, 6237.002441f, 138.253265f, 2.655654f},
    {5412.348633f, 6242.738770f, 138.243645f, 2.386873f},
    {5406.313477f, 6250.382813f, 137.969048f, 1.983702f},
    {5403.034668f, 6262.096680f, 137.948959f, 1.638562f},
    {5405.832031f, 6274.675781f, 138.344994f, 1.033804f},
    {5411.674316f, 6283.166016f, 138.436462f, 0.719208f},
    {5419.626465f, 6288.601074f, 138.417229f, 0.392396f},
    {5431.413086f, 6291.487793f, 138.442223f, 6.278519f},
    {5441.747559f, 6289.384766f, 138.443604f, 5.798989f},
    {5451.087402f, 6283.937988f, 138.297729f, 5.569914f},
    {5457.380371f, 6274.509277f, 138.441902f, 5.053733f}
};

const Position spiritflayerPos4[16] = 
{
    {5450.934082f, 6263.100098f, 138.441963f, 4.708162f},
    {5450.359863f, 6256.377930f, 138.441963f, 4.509635f},
    {5446.436523f, 6249.675781f, 138.441963f, 4.033159f},
    {5439.609863f, 6245.040527f, 138.441963f, 3.596390f},
    {5433.309082f, 6243.434082f, 138.441963f, 3.138240f},
    {5425.561523f, 6244.853027f, 138.441963f, 2.869458f},
    {5418.212891f, 6248.975098f, 138.441963f, 2.374655f},
    {5414.625000f, 6253.920898f, 138.441963f, 2.075328f},
    {5412.027832f, 6263.045410f, 138.441963f, 1.601905f},
    {5413.183105f, 6270.441406f, 138.441963f, 1.207897f},
    {5416.853027f, 6276.590332f, 138.441963f, 0.795562f},
    {5423.654297f, 6282.305664f, 138.441963f, 0.389337f},
    {5431.511230f, 6283.376953f, 138.441963f, 6.183831f},
    {5438.097168f, 6282.004883f, 138.441963f, 5.853963f},
    {5444.494141f, 6278.459473f, 138.441963f, 5.572965f},
    {5450.210938f, 6271.070313f, 138.441963f, 5.145362f}
};

const Position spiritflayerPos5[19] = 
{
    {5425.482422f, 6213.105469f, 132.047310f, 4.709036f},
    {5425.360840f, 6197.530762f, 132.056717f, 4.733469f},
    {5425.735840f, 6184.936523f, 130.873024f, 4.742632f},
    {5426.153809f, 6168.562500f, 130.873024f, 4.715144f},
    {5426.266113f, 6133.562988f, 132.056450f, 4.715144f},
    {5425.978027f, 6114.213379f, 132.056450f, 4.651004f},
    {5423.932129f, 6098.960449f, 130.872444f, 4.620461f},
    {5424.258789f, 6085.170898f, 130.872444f, 4.806775f},
    {5425.753418f, 6064.566895f, 132.051674f, 4.724311f},
    {5424.713379f, 6059.989258f, 132.048492f, 6.013237f},
    {5437.947754f, 6059.711426f, 132.048492f, 0.606643f},
    {5437.367188f, 6075.051270f, 132.053337f, 1.635950f},
    {5437.173340f, 6087.718750f, 130.871750f, 1.574864f},
    {5437.102051f, 6105.212891f, 131.549538f, 1.574864f},
    {5436.959961f, 6140.204590f, 132.057236f, 1.574864f},
    {5436.888672f, 6157.704102f, 132.057236f, 1.574864f},
    {5436.817383f, 6175.204102f, 130.871758f, 1.574864f},
    {5436.746094f, 6192.704102f, 131.141632f, 1.574864f},
    {5436.675293f, 6210.204102f, 132.050056f, 1.574864f}
};

const Position leiShenEventPos = {6045.97f, 4985.88f, 148.02f, 1.56f};

const Position lightningSpanPos[6] = 
{
    {6058.758301f, 5088.322266f, 153.791229f, 3.866019f},
    {6040.281738f, 5082.804199f, 153.680832f, 3.282641f},
    {6036.992676f, 5093.713867f, 153.870758f, 1.831836f},
    {6043.072266f, 5102.285156f, 153.465012f, 0.906375f},
    {6036.556641f, 5107.791504f, 153.802719f, 2.516005f},
    {6044.575684f, 5120.748047f, 153.526337f, 0.573455f}
};

enum Events
{
    // Zandalari Blade Initiate
    EVENT_WOUNDING_STRIKE   = 1,

    // Zandalari Water Binder
    EVENT_BIND_WATER,
    EVENT_DELUGE,
    EVENT_FROST_BOLT,

    // Zandalari Storm Caller
    EVENT_FOCUSED_LIGHTNING,
    EVENT_STORM_WEAPON,
    EVENT_WATER_BOLT,

    // Zandalari Spear Shaper
    EVENT_BERSERKING,

    // Zandalari Storm Caller2
    EVENT_STORM_ENERGY,

    // Ancient Python
    EVENT_ANCIENT_VENOM,

    // Spirit Flayer
    EVENT_CHECK_PLAYERS,
    EVENT_MOVE,

    // Tormented Spirit
    EVENT_TORMENT,

    // Soul-fed Construct
    EVENT_CRUSH_ARMOR,
    EVENT_SPIRITFIRE_BEAM,

    // Stormbringer Draz'kil
    EVENT_CHAIN_LIGHTNING,
    EVENT_STORMCLOUD,

    // Gurubashi Berserker
    EVENT_BERSERKER_FRENZY,
    EVENT_BLOODLETTING,

    // Drakkari Frost Warden
    EVENT_FROST_BULWARK,
    EVENT_GLACIAL_FREEZE_TOTEM,

    // Amanishi Flame Chanter
    EVENT_CALL_FLAMES,
    EVENT_ERUPTION,

    // Farraki Sand Conjurer
    EVENT_SAND_BOLT,
    EVENT_CHOKING_SAND,

    // Zandalari High Priest
    EVENT_LIGHT_OF_THE_LOA,

    // Zandalari Prophet
    EVENT_MARK_OF_THE_PROPHET,
    EVENT_VISIONS_OF_DEMISE,
    EVENT_VISIONS_OF_GRANDEUR,

    // Zandalari Warlord
    EVENT_CLEAVE,
    EVENT_STRENGTH_OF_THE_LOA,
    EVENT_ZANDALARI_WARCRY,

    // Zandalari Prelate
    EVENT_JUDGEMENT_OF_THE_LOA,

    // Lei Shen event (lightning span)
    EVENT_LEI_SHEN_TALK,
    EVENT_LEI_SHEN_SPAN,
    EVENT_LEI_SHEN_TELEPORT,

    // Shale Stalker
    EVENT_SHALE_SHARDS,

    // Fungal Growth
    EVENT_GROW,
    EVENT_FUNGI_SPORES,

    // Mist Lurker
    EVENT_CHOKING_MISTS,
    EVENT_CORROSIVE_BREATH,

    // Eternal Guardian
    EVENT_ETERNAL_PRISON,
    EVENT_LIGHTNING_NOVA,
    EVENT_SIPHON_LIFE,

    // Cavern Burrower
    EVENT_SONIC_CALL,
    EVENT_CRYSTAL_BARBS,

    // Bow Fly Swarm
    EVENT_LUCIFERASE,

    // Gastropod
    EVENT_SLIME_TRAIL,

    // Rotting Scavenger
    EVENT_ROT,
    EVENT_FRENZIED_COMSUMPTION,
    EVENT_INFECTED_BITE,

    // Muck Bat
    EVENT_MUCK_SPIT,

    // Untrained Quilen
    EVENT_LEAPING_RUSH,
    EVENT_CARNIVOROUS_BITE,

    // Weisheng
    EVENT_TRIUMPHANT_ROAR,
    
    // Manchu
    EVENT_TRIUMPHANT_RUSH,
    EVENT_WILD_SMASH,
};

enum Actions
{
    ACTION_LEI_SHEN_EVENT   = 1,
};

enum MiscData
{
    DATA_LEI_SHEN_SPAWNED   = 1,
};

class npc_throne_of_thunder_zandalari_blade_initiate : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_blade_initiate() : CreatureScript("npc_throne_of_thunder_zandalari_blade_initiate") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_blade_initiateAI(creature);
        }

        struct npc_throne_of_thunder_zandalari_blade_initiateAI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_blade_initiateAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_WOUNDING_STRIKE, urand(3000, 6000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WOUNDING_STRIKE:
                            DoCastVictim(SPELL_WOUNDING_STRIKE);
                            events.ScheduleEvent(EVENT_WOUNDING_STRIKE, urand(6000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_zandalari_water_binder : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_water_binder() : CreatureScript("npc_throne_of_thunder_zandalari_water_binder") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_water_binderAI(creature);
        }

        struct npc_throne_of_thunder_zandalari_water_binderAI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_water_binderAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_BIND_WATER, urand(10000, 15000));
                events.ScheduleEvent(EVENT_FROST_BOLT, urand(3000, 10000));
                events.ScheduleEvent(EVENT_DELUGE, urand(10000, 20000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void JustSummoned(Creature* summon)
            {
                if (me->isInCombat())
                    DoZoneInCombat(summon);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BIND_WATER:
                            DoCast(me, SPELL_BIND_WATER);
                            break;
                        case EVENT_FROST_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_FROST_BOLT_DMG);
                            events.ScheduleEvent(EVENT_FROST_BOLT, urand(7000, 10000));
                            break;
                        case EVENT_DELUGE:
                            DoCast(me, SPELL_DELUGE);
                            events.ScheduleEvent(EVENT_DELUGE, urand(35000, 45000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_zandalari_storm_caller : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_storm_caller() : CreatureScript("npc_throne_of_thunder_zandalari_storm_caller") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_storm_callerAI(creature);
        }

        struct npc_throne_of_thunder_zandalari_storm_callerAI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_storm_callerAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_STORM_WEAPON, urand(2000, 3000));
                events.ScheduleEvent(EVENT_WATER_BOLT, urand(4000, 7000));
                events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, urand(10000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void JustSummoned(Creature* summon)
            {
                if (me->isInCombat())
                    DoZoneInCombat(summon);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WATER_BOLT:
                            DoCastAOE(SPELL_WATER_BOLT_AOE);
                            events.ScheduleEvent(EVENT_WATER_BOLT, urand(5000, 6000));
                            break;
                        case EVENT_STORM_WEAPON:
                            DoCast(me, SPELL_STORM_WEAPON);
                            break;
                        case EVENT_FOCUSED_LIGHTNING:
                            DoCastAOE(SPELL_FOCUSED_LIGHTNING);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_focused_lightning : public CreatureScript
{
    public:
        npc_throne_of_thunder_focused_lightning() : CreatureScript("npc_throne_of_thunder_focused_lightning") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_focused_lightningAI(creature);
        }

        struct npc_throne_of_thunder_focused_lightningAI : public Scripted_NoMovementAI
        {
            npc_throne_of_thunder_focused_lightningAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 2000);
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_FOCUSED_LIGHTNING)
                    {
                        DoCastAOE(SPELL_FOCUSED_LIGHTNING_AOE2);
                    }
                }
            }
        };  
};

class npc_throne_of_thunder_zandalari_spear_shaper : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_spear_shaper() : CreatureScript("npc_throne_of_thunder_zandalari_spear_shaper") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_spear_shaperAI(creature);
        }

        struct npc_throne_of_thunder_zandalari_spear_shaperAI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_spear_shaperAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_BERSERKING, urand(5000, 7000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void JustSummoned(Creature* summon)
            {
                if (me->isInCombat())
                    DoZoneInCombat(summon);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BERSERKING:
                            DoCast(me, SPELL_BERSERKING);
                            events.ScheduleEvent(EVENT_BERSERKING, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_zandalari_storm_caller2 : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_storm_caller2() : CreatureScript("npc_throne_of_thunder_zandalari_storm_caller2") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_storm_caller2AI(creature);
        }

        struct npc_throne_of_thunder_zandalari_storm_caller2AI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_storm_caller2AI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_STORM_WEAPON, urand(1000, 2000));
                events.ScheduleEvent(EVENT_STORM_ENERGY, urand(13000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    pInstance->SetData(DATA_CRAZIED_STORM_CALLER, DONE);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STORM_WEAPON:
                            DoCast(me, SPELL_STORM_WEAPON2);
                            break;
                        case EVENT_STORM_ENERGY:
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true, -SPELL_STORM_ENERGY);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                            if (target)
                                DoCast(target, SPELL_STORM_ENERGY);

                            events.ScheduleEvent(EVENT_STORM_ENERGY, urand(4000, 5000));
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};


class npc_throne_of_thunder_ancient_python : public CreatureScript
{
    public:
        npc_throne_of_thunder_ancient_python() : CreatureScript("npc_throne_of_thunder_ancient_python") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_ancient_pythonAI(creature);
        }

        struct npc_throne_of_thunder_ancient_pythonAI : public ScriptedAI
        {
            npc_throne_of_thunder_ancient_pythonAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_ANCIENT_VENOM, urand(6000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ANCIENT_VENOM:
                            DoCastVictim(SPELL_ANCIENT_VENOM);
                            events.ScheduleEvent(EVENT_ANCIENT_VENOM, urand(10000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_spirit_flayer : public CreatureScript
{
    public:
        npc_throne_of_thunder_spirit_flayer() : CreatureScript("npc_throne_of_thunder_spirit_flayer") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_spirit_flayerAI(creature);
        }

        struct npc_throne_of_thunder_spirit_flayerAI : public ScriptedAI
        {
            npc_throne_of_thunder_spirit_flayerAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetDisableGravity(true);

                currentId = 0;
                npcId = 0;

                if (me->GetPositionX() > 5710.0f && me->GetPositionX() < 5713.0f &&
                    me->GetPositionY() > 6260.0f && me->GetPositionY() < 6265.0f)
                {
                    npcId = 1;
                }
                else if (me->GetPositionX() > 5630.65f && me->GetPositionX() < 5638.0f &&
                    me->GetPositionY() > 6265.0f && me->GetPositionY() < 6270.0f)
                {
                    currentId = 8;
                    npcId = 2;
                }
                else if (me->GetPositionX() > 5480.0f && me->GetPositionX() < 5485.0f &&
                    me->GetPositionY() > 6255.0f && me->GetPositionY() < 6260.0f)
                {
                    npcId = 3;
                }
                else if (me->GetPositionX() > 5455.0f && me->GetPositionX() < 5461.0f &&
                    me->GetPositionY() > 6260.0f && me->GetPositionY() < 6266.0f)
                {
                    npcId = 4;
                }
                else if (me->GetPositionX() > 5410.0f && me->GetPositionX() < 5415.0f &&
                    me->GetPositionY() > 6260.0f && me->GetPositionY() < 6265.0f)
                {
                    currentId = 7;
                    npcId = 5;
                }
                else if (me->GetPositionX() > 5425.0f && me->GetPositionX() < 5430.0f &&
                    me->GetPositionY() > 6210.0f && me->GetPositionY() < 6216.0f)
                {
                    npcId = 6;
                }
                    
                if (npcId)
                {
                    events.ScheduleEvent(EVENT_MOVE, 2000);
                    events.ScheduleEvent(EVENT_CHECK_PLAYERS, 2000);
                }
            }

            void Reset()
            {
                me->AddAura(SPELL_SPIRIT_LANTERN, me);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type == POINT_MOTION_TYPE)
                {
                    events.ScheduleEvent(EVENT_MOVE, 100);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE:
                        {
                            Position const* nextPos = GetNextPoint();
                            if (nextPos)
                            {
                                me->GetMotionMaster()->MovementExpired(false);
                                me->GetMotionMaster()->MovePoint(1, *nextPos);
                            }
                            break;
                        }
                        case EVENT_CHECK_PLAYERS:
                        {
                            Player* player = me->FindNearestPlayer(20.0f);

                            if (player)
                            {
                                DoCastAOE(SPELL_SPIRIT_LIGHT, true);
                            }
                            else
                            {
                                if (me->isInCombat())
                                {
                                    me->DeleteThreatList();
                                    me->CombatStop(true);
                                }
                            }
                            
                            events.ScheduleEvent(EVENT_CHECK_PLAYERS, 1000);
                            break;
                        }
                    }
                }
            }

        private:

            uint32 currentId;
            uint32 npcId;

        private:

            Position const* GetNextPoint()
            {
                currentId++;

                switch (npcId)
                {
                    case 1:
                    {
                        if (currentId > 12)
                            currentId = 0;

                        return &spiritflayerPos1[currentId];
                    }
                    case 2:
                    case 3:
                    {
                        if (currentId > 18)
                            currentId = 0;

                        return &spiritflayerPos2[currentId];
                    }
                    case 4:
                    {
                        if (currentId > 15)
                            currentId = 0;

                        return &spiritflayerPos3[currentId];
                    }
                    case 5:
                    {
                        if (currentId > 15)
                            currentId = 0;

                        return &spiritflayerPos4[currentId];
                    }
                    case 6:
                    {
                        if (currentId > 18)
                            currentId = 0;

                        return &spiritflayerPos5[currentId];
                    }
                }

                return NULL;
            }

        };  
};

class npc_throne_of_thunder_tormented_spirit : public CreatureScript
{
    public:
        npc_throne_of_thunder_tormented_spirit() : CreatureScript("npc_throne_of_thunder_tormented_spirit") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_tormented_spiritAI(creature);
        }

        struct npc_throne_of_thunder_tormented_spiritAI : public ScriptedAI
        {
            npc_throne_of_thunder_tormented_spiritAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_TORMENT, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TORMENT:
                            DoCastAOE(SPELL_TORMENT);
                            events.ScheduleEvent(EVENT_TORMENT, urand(8000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_soul_fed_construct : public CreatureScript
{
    public:
        npc_throne_of_thunder_soul_fed_construct() : CreatureScript("npc_throne_of_thunder_soul_fed_construct") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_soul_fed_constructAI(creature);
        }

        struct npc_throne_of_thunder_soul_fed_constructAI : public ScriptedAI
        {
            npc_throne_of_thunder_soul_fed_constructAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CRUSH_ARMOR, urand(4000, 7000));
                events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM, urand(7000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CRUSH_ARMOR:
                            DoCastVictim(SPELL_CRUSH_ARMOR);
                            events.ScheduleEvent(EVENT_CRUSH_ARMOR, urand(10000, 15000));
                            break;
                        case EVENT_SPIRITFIRE_BEAM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_SPIRITFIRE_BEAM);
                            events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM, urand(8000, 12000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_stormbringer_drazkil : public CreatureScript
{
    public:
        npc_throne_of_thunder_stormbringer_drazkil() : CreatureScript("npc_throne_of_thunder_stormbringer_drazkil") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_stormbringer_drazkilAI(creature);
        }

        struct npc_throne_of_thunder_stormbringer_drazkilAI : public ScriptedAI
        {
            npc_throne_of_thunder_stormbringer_drazkilAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_STORMCLOUD, urand(2000, 4000));
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(3000, 7000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (InstanceScript* pInstance = me->GetInstanceScript())
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STORMCLOUD);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STORMCLOUD:
                            DoCastAOE(SPELL_STORMCLOUD);
                            events.ScheduleEvent(EVENT_STORMCLOUD, 18000);
                            break;
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_CHAIN_LIGHTNING);
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(9000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_gurubashi_berserker : public CreatureScript
{
    public:
        npc_throne_of_thunder_gurubashi_berserker() : CreatureScript("npc_throne_of_thunder_gurubashi_berserker") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_gurubashi_berserkerAI(creature);
        }

        struct npc_throne_of_thunder_gurubashi_berserkerAI : public ScriptedAI
        {
            npc_throne_of_thunder_gurubashi_berserkerAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_BERSERKER_FRENZY, urand(5000, 20000));
                events.ScheduleEvent(EVENT_BLOODLETTING, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BERSERKER_FRENZY:
                            DoCast(me, SPELL_BERSERKER_FRENZY);
                            break;
                        case EVENT_BLOODLETTING:
                            DoCastVictim(SPELL_BLOODLETTING);
                            events.ScheduleEvent(EVENT_BLOODLETTING, urand(11000, 13000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_drakkari_frost_warden : public CreatureScript
{
    public:
        npc_throne_of_thunder_drakkari_frost_warden() : CreatureScript("npc_throne_of_thunder_drakkari_frost_warden") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_drakkari_frost_wardenAI(creature);
        }

        struct npc_throne_of_thunder_drakkari_frost_wardenAI : public ScriptedAI
        {
            npc_throne_of_thunder_drakkari_frost_wardenAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_FROST_BULWARK, urand(3000, 10000));
                events.ScheduleEvent(EVENT_GLACIAL_FREEZE_TOTEM, urand(7000, 12000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FROST_BULWARK:
                        {
                            Unit* target = DoSelectLowestHpFriendly(20.0f);
                            if (!target)
                                target = me;
                            DoCast(target, SPELL_FROST_BULWARK);
                            events.ScheduleEvent(EVENT_FROST_BULWARK, urand(15000, 20000));
                            break;
                        }
                        case EVENT_GLACIAL_FREEZE_TOTEM:
                            DoCast(me, SPELL_GLACIAL_FREEZE_TOTEM);
                            events.ScheduleEvent(EVENT_GLACIAL_FREEZE_TOTEM, urand(20000, 25000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_glacial_freeze_totem : public CreatureScript
{
    public:
        npc_throne_of_thunder_glacial_freeze_totem() : CreatureScript("npc_throne_of_thunder_glacial_freeze_totem") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_glacial_freeze_totemAI(creature);
        }

        struct npc_throne_of_thunder_glacial_freeze_totemAI : public Scripted_NoMovementAI
        {
            npc_throne_of_thunder_glacial_freeze_totemAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit* owner)
            {
                DoCast(me, SPELL_GLACIAL_FREEZE);
            }
        };  
};

class npc_throne_of_thunder_amanishi_flame_chanter : public CreatureScript
{
    public:
        npc_throne_of_thunder_amanishi_flame_chanter() : CreatureScript("npc_throne_of_thunder_amanishi_flame_chanter") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_amanishi_flame_chanterAI(creature);
        }

        struct npc_throne_of_thunder_amanishi_flame_chanterAI : public ScriptedAI
        {
            npc_throne_of_thunder_amanishi_flame_chanterAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CALL_FLAMES, urand(100, 1000));
                events.ScheduleEvent(EVENT_ERUPTION, urand(8000, 12000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CALL_FLAMES:
                            DoCastVictim(SPELL_CALL_FLAMES);
                            events.ScheduleEvent(EVENT_CALL_FLAMES, 2000);
                            break;
                        case EVENT_ERUPTION:
                            DoCastAOE(SPELL_ERUPTION);
                            events.ScheduleEvent(EVENT_ERUPTION, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_eruption : public CreatureScript
{
    public:
        npc_throne_of_thunder_eruption() : CreatureScript("npc_throne_of_thunder_eruption") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_eruptionAI(creature);
        }

        struct npc_throne_of_thunder_eruptionAI : public Scripted_NoMovementAI
        {
            npc_throne_of_thunder_eruptionAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit* owner)
            {
                me->AddAura(SPELL_ERUPTION_AURA, me);
            }
        };  
};

class npc_throne_of_thunder_farraki_sand_conjurer : public CreatureScript
{
    public:
        npc_throne_of_thunder_farraki_sand_conjurer() : CreatureScript("npc_throne_of_thunder_farraki_sand_conjurer") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_farraki_sand_conjurerAI(creature);
        }

        struct npc_throne_of_thunder_farraki_sand_conjurerAI : public ScriptedAI
        {
            npc_throne_of_thunder_farraki_sand_conjurerAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();

                summons.DespawnAll();
            }

            void EnterCombat(Unit* who)
            {
                DoCast(me, SPELL_CONJURE_ELEMENTALS);

                events.ScheduleEvent(EVENT_SAND_BOLT, urand(100, 1000));
                events.ScheduleEvent(EVENT_CHOKING_SAND, urand(7000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SAND_BOLT:
                            DoCastVictim(SPELL_SAND_BOLT);
                            events.ScheduleEvent(EVENT_SAND_BOLT, 2000);
                            break;
                        case EVENT_CHOKING_SAND:
                            DoCastAOE(SPELL_CHOKING_SANDS);
                            events.ScheduleEvent(EVENT_CHOKING_SAND, urand(10000, 13000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_gurubashi_berserker2 : public CreatureScript
{
    public:
        npc_throne_of_thunder_gurubashi_berserker2() : CreatureScript("npc_throne_of_thunder_gurubashi_berserker2") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_gurubashi_berserker2AI(creature);
        }

        struct npc_throne_of_thunder_gurubashi_berserker2AI : public ScriptedAI
        {
            npc_throne_of_thunder_gurubashi_berserker2AI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_BLOODLETTING, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLOODLETTING:
                            DoCastVictim(SPELL_BLOODLETTING);
                            events.ScheduleEvent(EVENT_BLOODLETTING, urand(11000, 13000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_zandalari_high_priest : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_high_priest() : CreatureScript("npc_throne_of_thunder_zandalari_high_priest") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_high_priestAI(creature);
        }

        struct npc_throne_of_thunder_zandalari_high_priestAI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_high_priestAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_LIGHT_OF_THE_LOA, urand(5000, 7000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LIGHT_OF_THE_LOA:
                        {
                            Unit* target = DoSelectLowestHpFriendly(20.0f);
                            if (!target)
                                target = me;
                            DoCast(me, SPELL_LIGHT_OF_THE_LOA);
                            events.ScheduleEvent(EVENT_LIGHT_OF_THE_LOA, urand(8000, 11000));
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_zandalari_prophet : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_prophet() : CreatureScript("npc_throne_of_thunder_zandalari_prophet") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_prophetAI(creature);
        }

        struct npc_throne_of_thunder_zandalari_prophetAI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_prophetAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MARK_OF_THE_PROPHET, urand(5000, 12000));
                events.ScheduleEvent(EVENT_VISIONS_OF_DEMISE, urand(8000, 15000));
                events.ScheduleEvent(EVENT_VISIONS_OF_GRANDEUR, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MARK_OF_THE_PROPHET:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_MARK_OF_THE_PROPHET);
                            events.ScheduleEvent(EVENT_MARK_OF_THE_PROPHET, urand(10000, 20000));
                            break;
                        case EVENT_VISIONS_OF_DEMISE:
                            DoCast(me, SPELL_VISIONS_OF_DEMISE);
                            events.ScheduleEvent(EVENT_VISIONS_OF_DEMISE, urand(15000, 20000));
                            break;
                        case EVENT_VISIONS_OF_GRANDEUR:
                        {
                            Unit* target = DoSelectLowestHpFriendly(15.0f);
                            if (!target)
                                target = me;
                            DoCast(target, SPELL_VISIONS_OF_GRANDEUR);
                            events.ScheduleEvent(EVENT_VISIONS_OF_GRANDEUR, urand(15000, 20000));
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_zandalari_warlord : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_warlord() : CreatureScript("npc_throne_of_thunder_zandalari_warlord") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_warlordAI(creature);
        }

        struct npc_throne_of_thunder_zandalari_warlordAI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_warlordAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 7000));
                events.ScheduleEvent(EVENT_STRENGTH_OF_THE_LOA, urand(10000, 15000));
                events.ScheduleEvent(EVENT_ZANDALARI_WARCRY, urand(8000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 14000));
                            break;
                        case EVENT_STRENGTH_OF_THE_LOA:
                            DoCastVictim(SPELL_STRENGTH_OF_THE_LOA);
                            events.ScheduleEvent(EVENT_STRENGTH_OF_THE_LOA, urand(18000, 22000));
                            break;
                        case EVENT_ZANDALARI_WARCRY:
                            DoCastAOE(SPELL_ZANDALARI_WARCRY);
                            events.ScheduleEvent(EVENT_ZANDALARI_WARCRY, urand(15000, 25000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_zandalari_prelate : public CreatureScript
{
    public:
        npc_throne_of_thunder_zandalari_prelate() : CreatureScript("npc_throne_of_thunder_zandalari_prelate") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_zandalari_prelateAI(creature);
        }

        struct npc_throne_of_thunder_zandalari_prelateAI : public ScriptedAI
        {
            npc_throne_of_thunder_zandalari_prelateAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();

                me->AddAura(SPELL_SEAL_OF_THE_LOA, me);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_LIGHT_OF_THE_LOA, urand(8000, 12000));
                events.ScheduleEvent(EVENT_JUDGEMENT_OF_THE_LOA, urand(10000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LIGHT_OF_THE_LOA:
                        {
                            Unit* target = DoSelectLowestHpFriendly(20.0f);
                            if (!target)
                                target = me;
                            DoCast(me, SPELL_LIGHT_OF_THE_LOA);
                            events.ScheduleEvent(EVENT_LIGHT_OF_THE_LOA, urand(12000, 17000));
                            break;
                        }
                        case EVENT_JUDGEMENT_OF_THE_LOA:
                            DoCastVictim(SPELL_JUDGEMENT_OF_THE_LOA);
                            events.ScheduleEvent(EVENT_JUDGEMENT_OF_THE_LOA, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_lei_shen : public CreatureScript
{
    public:
        npc_throne_of_thunder_lei_shen() : CreatureScript("npc_throne_of_thunder_lei_shen") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_lei_shenAI(creature);
        }

        struct npc_throne_of_thunder_lei_shenAI : public Scripted_NoMovementAI
        {
            npc_throne_of_thunder_lei_shenAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                leiShenSpawned = false;
            }

            void Reset()
            {
                me->AddAura(SPELL_CALL_LIGHTNING, me);
            }

            uint32 GetData(uint32 type)
            {
                if (type == DATA_LEI_SHEN_SPAWNED)
                    return (leiShenSpawned ? 1 : 0);

                return 0;
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_LEI_SHEN_EVENT)
                {
                    events.ScheduleEvent(EVENT_LEI_SHEN_TALK, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LEI_SHEN_TALK:
                            // TODO: find the text
                            events.ScheduleEvent(EVENT_LEI_SHEN_SPAN, 1000);
                            break;
                        case EVENT_LEI_SHEN_SPAN:

                            leiShenSpawned = true;

                            DoCast(me, SPELL_CALL_LIGHTNING);

                            events.ScheduleEvent(EVENT_LEI_SHEN_TELEPORT, 5000);
                            break;
                        case EVENT_LEI_SHEN_TELEPORT:
                            if (InstanceScript* pInstance = me->GetInstanceScript())
                            {
                                if (GameObject* pGo = sObjectAccessor->GetGameObject(*me, pInstance->GetData64(DATA_LIGHTNING_SPAN)))
                                {
                                    pGo->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                                }
                            }

                            DoCastAOE(SPELL_TELEPORT_ALL_TO_DEPTH);

                            break;
                        default:
                            break;
                    }
                }
            }

        private:

            bool leiShenSpawned;

        };  
};

class npc_throne_of_thunder_shale_stalker : public CreatureScript
{
    public:
        npc_throne_of_thunder_shale_stalker() : CreatureScript("npc_throne_of_thunder_shale_stalker") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_shale_stalkerAI(creature);
        }

        struct npc_throne_of_thunder_shale_stalkerAI : public ScriptedAI
        {
            npc_throne_of_thunder_shale_stalkerAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SHALE_SHARDS, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHALE_SHARDS:
                            DoCastAOE(SPELL_SHALE_SHARDS);
                            events.ScheduleEvent(EVENT_SHALE_SHARDS, urand(8000, 14000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_fungal_growth : public CreatureScript
{
    public:
        npc_throne_of_thunder_fungal_growth() : CreatureScript("npc_throne_of_thunder_fungal_growth") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_fungal_growthAI(creature);
        }

        struct npc_throne_of_thunder_fungal_growthAI : public ScriptedAI
        {
            npc_throne_of_thunder_fungal_growthAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_GROW, 5000);
                events.ScheduleEvent(EVENT_FUNGI_SPORES, urand(6000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GROW:
                            DoCast(me, SPELL_GROW);
                            events.ScheduleEvent(EVENT_GROW, urand(10000, 18000));
                            break;
                        case EVENT_FUNGI_SPORES:
                            DoCastAOE(SPELL_FUNGI_SPORES);
                            events.ScheduleEvent(EVENT_FUNGI_SPORES, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_mist_lurker : public CreatureScript
{
    public:
        npc_throne_of_thunder_mist_lurker() : CreatureScript("npc_throne_of_thunder_mist_lurker") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_mist_lurkerAI(creature);
        }

        struct npc_throne_of_thunder_mist_lurkerAI : public ScriptedAI
        {
            npc_throne_of_thunder_mist_lurkerAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CHOKING_MISTS, urand(5000, 8000));
                events.ScheduleEvent(EVENT_CORROSIVE_BREATH, urand(8000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHOKING_MISTS:
                            DoCastAOE(SPELL_CHOKING_MISTS);
                            events.ScheduleEvent(EVENT_CHOKING_MISTS, urand(5000, 8000));
                            break;
                        case EVENT_CORROSIVE_BREATH:
                            DoCastVictim(SPELL_CORROSIVE_BREATH);
                            events.ScheduleEvent(EVENT_CORROSIVE_BREATH, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_eternal_guardian : public CreatureScript
{
    public:
        npc_throne_of_thunder_eternal_guardian() : CreatureScript("npc_throne_of_thunder_eternal_guardian") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_eternal_guardianAI(creature);
        }

        struct npc_throne_of_thunder_eternal_guardianAI : public ScriptedAI
        {
            npc_throne_of_thunder_eternal_guardianAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();

                DoCast(me, SPELL_ETERNAL_GUARDIAN_SPAWN, true);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_ETERNAL_PRISON, urand(10000, 12000));
                events.ScheduleEvent(EVENT_LIGHTNING_NOVA, urand(8000, 15000));
                events.ScheduleEvent(EVENT_SIPHON_LIFE, urand(20000, 30000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ETERNAL_PRISON:
                            DoCastAOE(SPELL_ETERNAL_PRISON);
                            events.ScheduleEvent(EVENT_ETERNAL_PRISON, urand(12000, 18000));
                            break;
                        case EVENT_LIGHTNING_NOVA:
                            DoCastAOE(SPELL_LIGHTNING_NOVA);
                            events.ScheduleEvent(EVENT_LIGHTNING_NOVA, urand(10000, 20000));
                            break;
                        case EVENT_SIPHON_LIFE:
                            DoCastAOE(SPELL_SIPHON_LIFE);
                            events.ScheduleEvent(EVENT_LIGHTNING_NOVA, urand(25000, 35000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_cavern_burrower : public CreatureScript
{
    public:
        npc_throne_of_thunder_cavern_burrower() : CreatureScript("npc_throne_of_thunder_cavern_burrower") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_cavern_burrowerAI(creature);
        }

        struct npc_throne_of_thunder_cavern_burrowerAI : public ScriptedAI
        {
            npc_throne_of_thunder_cavern_burrowerAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();

                DoCast(me, SPELL_SUBMERGE, true);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SONIC_CALL, urand(2000, 3000));
                events.ScheduleEvent(EVENT_CRUSH_ARMOR, urand(3000, 8000));
                events.ScheduleEvent(EVENT_CRYSTAL_BARBS, urand(10000, 12000));

                me->RemoveAura(SPELL_SUBMERGE);
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SONIC_CALL:
                            DoCastAOE(SPELL_SONIC_CALL);
                            break;
                        case EVENT_CRUSH_ARMOR:
                            DoCastVictim(SPELL_CRUSH_ARMOR2);
                            events.ScheduleEvent(EVENT_CRUSH_ARMOR, urand(17000, 20000));
                            break;
                        case EVENT_CRYSTAL_BARBS:
                            DoCastAOE(SPELL_CRYSTAL_BARBS);
                            events.ScheduleEvent(EVENT_CRYSTAL_BARBS, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

/*class npc_throne_of_thunder_bore_worm : public CreatureScript
{
    public:
        npc_throne_of_thunder_bore_worm() : CreatureScript("npc_throne_of_thunder_bore_worm") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_bore_wormAI(creature);
        }

        struct npc_throne_of_thunder_bore_wormAI : public ScriptedAI
        {
            npc_throne_of_thunder_bore_wormAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();

            }

            void EnterCombat(Unit* who)
            {

            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};*/

class npc_throne_of_thunder_bow_fly_swarm : public CreatureScript
{
    public:
        npc_throne_of_thunder_bow_fly_swarm() : CreatureScript("npc_throne_of_thunder_bow_fly_swarm") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_bow_fly_swarmAI(creature);
        }

        struct npc_throne_of_thunder_bow_fly_swarmAI : public ScriptedAI
        {
            npc_throne_of_thunder_bow_fly_swarmAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();

            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_LUCIFERASE, urand(5000, 7000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LUCIFERASE:
                            DoCastAOE(SPELL_LUCIFERASE);
                            events.ScheduleEvent(EVENT_LUCIFERASE, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_gastropod : public CreatureScript
{
    public:
        npc_throne_of_thunder_gastropod() : CreatureScript("npc_throne_of_thunder_gastropod") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_gastropodAI(creature);
        }

        struct npc_throne_of_thunder_gastropodAI : public ScriptedAI
        {
            npc_throne_of_thunder_gastropodAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();

                me->SetSpeed(MOVE_WALK, 0.3f, true);
                me->SetSpeed(MOVE_RUN, 0.3f, true);

                DoCast(me, SPELL_SNAIL_SHELL, true);

                me->AddAura(SPELL_ABRASIVE_RADULA, me);
            }

            void EnterCombat(Unit* who)
            {
                me->RemoveAura(SPELL_SNAIL_SHELL);

                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 1000);
                events.ScheduleEvent(EVENT_SLIME_TRAIL, 1000);
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_PLAYERS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 5.0f, true))
                                DoCast(target, SPELL_DEVOURED);
                            events.ScheduleEvent(EVENT_CHECK_PLAYERS, 2000);
                            break;
                        case EVENT_SLIME_TRAIL:
                        {
                            std::list<AreaTrigger*> triggers;
                            me->GetAreaTriggerList(triggers, SPELL_SLIME_TRAIL_AREATRIGGER);

                            bool canSpawn = true;
                            if (!triggers.empty())
                            {
                                for (std::list<AreaTrigger*>::const_iterator itr = triggers.begin(); itr != triggers.end(); ++itr)
                                {
                                    if (me->GetDistance((*itr)) <= 5.0f)
                                    {
                                        canSpawn = false;
                                        break;
                                    }
                                }
                            }

                            if (canSpawn)
                                DoCast(SPELL_SLIME_TRAIL_AREATRIGGER);

                            events.ScheduleEvent(EVENT_SLIME_TRAIL, 3000);
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_corpse_spider : public CreatureScript
{
    public:
        npc_throne_of_thunder_corpse_spider() : CreatureScript("npc_throne_of_thunder_corpse_spider") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_corpse_spiderAI(creature);
        }

        struct npc_throne_of_thunder_corpse_spiderAI : public ScriptedAI
        {
            npc_throne_of_thunder_corpse_spiderAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {

            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                /*
                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        default:
                            break;
                    }
                }
                */

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_rotting_scavenger : public CreatureScript
{
    public:
        npc_throne_of_thunder_rotting_scavenger() : CreatureScript("npc_throne_of_thunder_rotting_scavenger") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_rotting_scavengerAI(creature);
        }

        struct npc_throne_of_thunder_rotting_scavengerAI : public ScriptedAI
        {
            npc_throne_of_thunder_rotting_scavengerAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_ROT, urand(3000, 8000));
                events.ScheduleEvent(EVENT_FRENZIED_COMSUMPTION, urand(10000, 15000));
                events.ScheduleEvent(EVENT_INFECTED_BITE, urand(4000, 8000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ROT:
                            DoCastAOE(SPELL_ROT);
                            events.ScheduleEvent(EVENT_ROT, urand(10000, 15000));
                            break;
                        case EVENT_FRENZIED_COMSUMPTION:
                            DoCastVictim(SPELL_FRENZIED_COMSUMPTION);
                            events.ScheduleEvent(EVENT_FRENZIED_COMSUMPTION, urand(20000, 30000));
                            break;
                        case EVENT_INFECTED_BITE:
                            DoCastVictim(SPELL_INFECTED_BITE);
                            events.ScheduleEvent(EVENT_INFECTED_BITE, urand(8000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_putrid_waste : public CreatureScript
{
    public:
        npc_throne_of_thunder_putrid_waste() : CreatureScript("npc_throne_of_thunder_putrid_waste") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_putrid_wasteAI(creature);
        }

        struct npc_throne_of_thunder_putrid_wasteAI : public ScriptedAI
        {
            npc_throne_of_thunder_putrid_wasteAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                me->AddAura(SPELL_PUTRIFY, me);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_muck_bat : public CreatureScript
{
    public:
        npc_throne_of_thunder_muck_bat() : CreatureScript("npc_throne_of_thunder_muck_bat") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_muck_batAI(creature);
        }

        struct npc_throne_of_thunder_muck_batAI : public ScriptedAI
        {
            npc_throne_of_thunder_muck_batAI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MUCK_SPIT, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MUCK_SPIT:
                            DoCastVictim(SPELL_MUCK_SPIT);
                            events.ScheduleEvent(EVENT_MUCK_SPIT, urand(8000, 12000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_untrained_quilen : public CreatureScript
{
    public:
        npc_throne_of_thunder_untrained_quilen() : CreatureScript("npc_throne_of_thunder_untrained_quilen") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_untrained_quilenAI(creature);
        }

        struct npc_throne_of_thunder_untrained_quilenAI : public ScriptedAI
        {
            npc_throne_of_thunder_untrained_quilenAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetStandState(UNIT_STAND_STATE_SIT);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_LEAPING_RUSH, urand(1000, 2000));
                events.ScheduleEvent(EVENT_CARNIVOROUS_BITE, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LEAPING_RUSH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -6.0f, true))
                                DoCast(target, SPELL_LEAPING_RUSH);
                            events.ScheduleEvent(EVENT_LEAPING_RUSH, urand(10000, 20000));
                            break;
                        case EVENT_CARNIVOROUS_BITE:
                            DoCastVictim(SPELL_CARNIVOROUS_BITE);
                            events.ScheduleEvent(EVENT_CARNIVOROUS_BITE, urand(12000, 14000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_weisheng : public CreatureScript
{
    public:
        npc_throne_of_thunder_weisheng() : CreatureScript("npc_throne_of_thunder_weisheng") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_weishengAI(creature);
        }

        struct npc_throne_of_thunder_weishengAI : public ScriptedAI
        {
            npc_throne_of_thunder_weishengAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_TRIUMPHANT_ROAR, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TRIUMPHANT_ROAR:
                            DoCastAOE(SPELL_TRIUMPHANT_ROAR);
                            events.ScheduleEvent(EVENT_TRIUMPHANT_ROAR, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class npc_throne_of_thunder_manchu : public CreatureScript
{
    public:
        npc_throne_of_thunder_manchu() : CreatureScript("npc_throne_of_thunder_manchu") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_throne_of_thunder_manchuAI(creature);
        }

        struct npc_throne_of_thunder_manchuAI : public ScriptedAI
        {
            npc_throne_of_thunder_manchuAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_TRIUMPHANT_RUSH, urand(1000, 5000));
                events.ScheduleEvent(EVENT_WILD_SMASH, urand(10000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TRIUMPHANT_RUSH:
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -15.0f, true);
                            if (!target)
                                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                            if (target)
                                DoCast(target, EVENT_TRIUMPHANT_RUSH);
                            events.ScheduleEvent(EVENT_TRIUMPHANT_RUSH, urand(25000, 30000));
                            break;
                        }
                        case EVENT_WILD_SMASH:
                            DoCastVictim(SPELL_WILD_SMASH_DMG);
                            events.ScheduleEvent(EVENT_WILD_SMASH, urand(10000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };  
};

class spell_throne_of_thunder_focused_lightning_aoe : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_focused_lightning_aoe() : SpellScriptLoader("spell_throne_of_thunder_focused_lightning_aoe") { }

    class spell_throne_of_thunder_focused_lightning_aoe_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_throne_of_thunder_focused_lightning_aoe_SpellScript);

        void HandeHitTarget(SpellEffIndex effIndex)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), SPELL_FOCUSED_LIGHTNING_DMG, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_throne_of_thunder_focused_lightning_aoe_SpellScript::HandeHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_throne_of_thunder_focused_lightning_aoe_SpellScript();
    }
};

class spell_throne_of_thunder_water_bolt_aoe : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_water_bolt_aoe() : SpellScriptLoader("spell_throne_of_thunder_water_bolt_aoe") { }

    class spell_throne_of_thunder_water_bolt_aoe_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_throne_of_thunder_water_bolt_aoe_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            uint32 maxTargets = 2;
            if (GetCaster()->GetMap()->Is25ManRaid())
                maxTargets = 5;

            if (targets.size() > maxTargets)
                JadeCore::Containers::RandomResizeList(targets, maxTargets);
        }

        void HandeHitTarget(SpellEffIndex effIndex)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetCaster()->CastSpell(GetHitUnit(), SPELL_WATER_BOLT_DMG, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_throne_of_thunder_water_bolt_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_throne_of_thunder_water_bolt_aoe_SpellScript::HandeHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_throne_of_thunder_water_bolt_aoe_SpellScript();
    }
};

class spell_throne_of_thunder_spirit_light : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_spirit_light() : SpellScriptLoader("spell_throne_of_thunder_spirit_light") { }

    class spell_throne_of_thunder_spirit_light_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_throne_of_thunder_spirit_light_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            targets.clear();

            std::list<Player*> players;
            JadeCore::AnyPlayerInObjectRangeCheck check(GetCaster(), 19.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(GetCaster(), players, check);
            GetCaster()->VisitNearbyWorldObject(19.0f, searcher);
            
            for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                targets.push_back((*itr));
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_throne_of_thunder_spirit_light_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_throne_of_thunder_spirit_light_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_throne_of_thunder_spirit_light_SpellScript();
    }
};

class spell_throne_of_thunder_call_flames : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_call_flames() : SpellScriptLoader("spell_throne_of_thunder_call_flames") { }

    class spell_throne_of_thunder_call_flames_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_throne_of_thunder_call_flames_SpellScript);

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;

            Aura const* aur = GetCaster()->GetAura(SPELL_FIERY_CORE);
            if (!aur || aur->GetStackAmount() < 4)
            {
                GetCaster()->AddAura(SPELL_FIERY_CORE, GetCaster());
            }
            else
            {
                GetCaster()->RemoveAura(SPELL_FIERY_CORE);
                GetCaster()->CastSpell(GetCaster(), SPELL_MOLTEN_BARRAGE, true);
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_throne_of_thunder_call_flames_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_throne_of_thunder_call_flames_SpellScript();
    }
};

class spell_throne_of_thunder_eruption : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_eruption() : SpellScriptLoader("spell_throne_of_thunder_eruption") { }

    class spell_throne_of_thunder_eruption_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_throne_of_thunder_eruption_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            GetCaster()->SummonCreature(NPC_ERUPTION, *GetHitUnit(),  TEMPSUMMON_TIMED_DESPAWN, 6000);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_throne_of_thunder_eruption_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_throne_of_thunder_eruption_SpellScript();
    }
};

class spell_throne_of_thunder_conjure_elementals : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_conjure_elementals() : SpellScriptLoader("spell_throne_of_thunder_conjure_elementals") { }

    class spell_throne_of_thunder_conjure_elementals_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_throne_of_thunder_conjure_elementals_SpellScript);

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;

            GetCaster()->SummonCreature(NPC_SAND_ELEMENTAL, *GetCaster());
            GetCaster()->SummonCreature(NPC_SAND_ELEMENTAL, *GetCaster());
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_throne_of_thunder_conjure_elementals_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_throne_of_thunder_conjure_elementals_SpellScript();
    }
};

class spell_throne_of_thunder_lightning_strike : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_lightning_strike() : SpellScriptLoader("spell_throne_of_thunder_lightning_strike") { }

    class spell_throne_of_thunder_lightning_strike_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_throne_of_thunder_lightning_strike_AuraScript);

        void HandlePeriodic(AuraEffect const* aurEff)
        {
            if (!GetCaster())
                return;

            if (Creature* pLeiShen = GetCaster()->ToCreature())
            {
                if (!pLeiShen->AI()->GetData(DATA_LEI_SHEN_SPAWNED))
                    return;

                int32 randPos = urand(0, 5);

                GetCaster()->CastSpell(lightningSpanPos[randPos].GetPositionX(), lightningSpanPos[randPos].GetPositionY(), lightningSpanPos[randPos].GetPositionZ(), SPELL_LIGHTNING_STRIKE, true);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_throne_of_thunder_lightning_strike_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };  

    AuraScript* GetAuraScript() const
    {
        return new spell_throne_of_thunder_lightning_strike_AuraScript();
    }
};

class spell_throne_of_thunder_teleport_all_to_depth : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_teleport_all_to_depth() : SpellScriptLoader("spell_throne_of_thunder_teleport_all_to_depth") { }

    class spell_throne_of_thunder_teleport_all_to_depth_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_throne_of_thunder_teleport_all_to_depth_AuraScript);

        void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (!GetUnitOwner())
                return;

            if (GetUnitOwner()->GetTypeId() != TYPEID_PLAYER)
                return;

            GetUnitOwner()->NearTeleportTo(tortosLairPos.GetPositionX(), tortosLairPos.GetPositionY(), tortosLairPos.GetPositionZ(), tortosLairPos.GetOrientation());
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_throne_of_thunder_teleport_all_to_depth_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_SCREEN_EFFECT, AURA_EFFECT_HANDLE_REAL);
        }
    };  

    AuraScript* GetAuraScript() const
    {
        return new spell_throne_of_thunder_teleport_all_to_depth_AuraScript();
    }
};

class spell_throne_of_thunder_sonic_call: public SpellScriptLoader
{
    public:
        spell_throne_of_thunder_sonic_call() : SpellScriptLoader("spell_throne_of_thunder_sonic_call") { }

        class spell_throne_of_thunder_sonic_call_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_throne_of_thunder_sonic_call_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (!GetHitUnit() || !caster->getVictim())
                        return;

                    if (Creature* borrower = GetHitUnit()->ToCreature())
                        borrower->AI()->AttackStart(caster->getVictim());
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_throne_of_thunder_sonic_call_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_throne_of_thunder_sonic_call_SpellScript();
        }
};

class spell_throne_of_thunder_luciferase: public SpellScriptLoader
{
    public:
        spell_throne_of_thunder_luciferase() : SpellScriptLoader("spell_throne_of_thunder_luciferase") { }

        class spell_throne_of_thunder_luciferase_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_throne_of_thunder_luciferase_SpellScript);

            void HandleOnHit(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_LUCIFERASE_DMG, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_throne_of_thunder_luciferase_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_throne_of_thunder_luciferase_SpellScript();
        }
};

class spell_throne_of_thunder_devoured: public SpellScriptLoader
{
    public:
        spell_throne_of_thunder_devoured() : SpellScriptLoader("spell_throne_of_thunder_devoured") { }

        class spell_throne_of_thunder_devoured_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_throne_of_thunder_devoured_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (Unit* target = GetTarget())
                {
                    if (Unit* caster = GetCaster())
                        caster->Kill(target, false, GetSpellInfo());
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_throne_of_thunder_devoured_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_throne_of_thunder_devoured_AuraScript();
        }
};

class spell_throne_of_thunder_triumphant_roar : public SpellScriptLoader
{
public:
    spell_throne_of_thunder_triumphant_roar() : SpellScriptLoader("spell_throne_of_thunder_triumphant_roar") { }

    class spell_throne_of_thunder_triumphant_roar_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_throne_of_thunder_triumphant_roar_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
            {
                Unit* target = (*itr)->ToUnit();
                if (!target)
                {
                    itr = targets.erase(itr);
                    continue;
                }

                if (!target->isInCombat() || 
                    (target->GetEntry() != NPC_WEISHENG &&
                    target->GetEntry() != NPC_MANCHU &&
                    target->GetEntry() != NPC_UNTRAINED_QUILEN_1 &&
                    target->GetEntry() != NPC_UNTRAINED_QUILEN_2))
                {
                    itr = targets.erase(itr);
                    continue;
                }

                ++itr;
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_throne_of_thunder_triumphant_roar_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_throne_of_thunder_triumphant_roar_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_throne_of_thunder_triumphant_roar_SpellScript::FilterTargets, EFFECT_3, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_throne_of_thunder_triumphant_roar_SpellScript();
    }
};

class spell_area_throne_of_thunder_slime_trail : public SpellAreaTriggerScript
{
    public:
        spell_area_throne_of_thunder_slime_trail() : SpellAreaTriggerScript("spell_area_throne_of_thunder_slime_trail") {}

        bool OnAddTarget(AreaTrigger* trigger, Unit* target)
        {
            if (!trigger->GetCaster())
                return false;

            if (!target->isTargetableForAttack())
                return false;

            trigger->GetCaster()->AddAura(SPELL_SLIME_TRAIL_AURA, target);

            return true;
        }

        bool OnRemoveTarget(AreaTrigger* trigger, Unit* target)
        {
            target->RemoveAura(SPELL_SLIME_TRAIL_AURA);

            return true;
        }
};

class go_displacement_pad : public GameObjectScript
{
    public:
        go_displacement_pad() : GameObjectScript("go_displacement_pad") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (InstanceScript* pInstance = go->GetInstanceScript())
            {
                if (pInstance->GetBossState(DATA_LEI_SHEN) != IN_PROGRESS)
                {
                    player->NearTeleportTo(leiShenPos.GetPositionX(), leiShenPos.GetPositionY(), leiShenPos.GetPositionZ(), leiShenPos.GetOrientation());
                }
            }
            return true;
        }
};

class at_lightning_span : public AreaTriggerScript
{
public:
    at_lightning_span() : AreaTriggerScript("at_lightning_span") { }

    bool OnTrigger(Player* pPlayer, const AreaTriggerEntry* /*pAt*/, bool p_Enter)
    {
        if (InstanceScript* pInstance = pPlayer->GetInstanceScript())
        {
            if (pInstance->IsEncounterInProgress())
                return false;

            if (pInstance->GetData(DATA_LIGHTNING_SPAN_EVENT) == NOT_STARTED)
            {
                pInstance->SetData(DATA_LIGHTNING_SPAN_EVENT, DONE);
                
                if (Creature* pLeiShen = pInstance->instance->SummonCreature(NPC_LEI_SHEN_EVENT, leiShenEventPos))
                {
                    pLeiShen->AI()->DoAction(ACTION_LEI_SHEN_EVENT);
                }
            }
        }
        return true;
    }
};

void AddSC_throne_of_thunder()
{
    new npc_throne_of_thunder_zandalari_blade_initiate();
    new npc_throne_of_thunder_zandalari_water_binder();
    new npc_throne_of_thunder_zandalari_storm_caller();
    new npc_throne_of_thunder_focused_lightning();
    new npc_throne_of_thunder_zandalari_spear_shaper();
    new npc_throne_of_thunder_zandalari_storm_caller2();
    new npc_throne_of_thunder_ancient_python();
    new npc_throne_of_thunder_spirit_flayer();
    new npc_throne_of_thunder_tormented_spirit();
    new npc_throne_of_thunder_soul_fed_construct();
    new npc_throne_of_thunder_stormbringer_drazkil();
    new npc_throne_of_thunder_gurubashi_berserker();
    new npc_throne_of_thunder_drakkari_frost_warden();
    new npc_throne_of_thunder_glacial_freeze_totem();
    new npc_throne_of_thunder_amanishi_flame_chanter();
    new npc_throne_of_thunder_eruption();
    new npc_throne_of_thunder_farraki_sand_conjurer();
    new npc_throne_of_thunder_gurubashi_berserker2();
    new npc_throne_of_thunder_zandalari_high_priest();
    new npc_throne_of_thunder_zandalari_prophet();
    new npc_throne_of_thunder_zandalari_warlord();
    new npc_throne_of_thunder_zandalari_prelate();
    new npc_throne_of_thunder_lei_shen();
    new npc_throne_of_thunder_shale_stalker();
    new npc_throne_of_thunder_fungal_growth();
    new npc_throne_of_thunder_mist_lurker();
    new npc_throne_of_thunder_eternal_guardian();
    new npc_throne_of_thunder_cavern_burrower();
    //new npc_throne_of_thunder_bore_worm();
    new npc_throne_of_thunder_bow_fly_swarm();
    new npc_throne_of_thunder_gastropod();
    //new npc_throne_of_thunder_corpse_spider();
    new npc_throne_of_thunder_rotting_scavenger();      // 70224
    new npc_throne_of_thunder_putrid_waste();           // 70219
    new npc_throne_of_thunder_muck_bat();               // 70232
    new npc_throne_of_thunder_untrained_quilen();       // 70209 70206
    new npc_throne_of_thunder_weisheng();               // 70205
    new npc_throne_of_thunder_manchu();                 // 70202

    new spell_area_throne_of_thunder_slime_trail();

    new spell_throne_of_thunder_water_bolt_aoe();
    new spell_throne_of_thunder_focused_lightning_aoe();
    new spell_throne_of_thunder_spirit_light();
    new spell_throne_of_thunder_call_flames();
    new spell_throne_of_thunder_eruption();
    new spell_throne_of_thunder_conjure_elementals();
    new spell_throne_of_thunder_lightning_strike();
    new spell_throne_of_thunder_teleport_all_to_depth();
    new spell_throne_of_thunder_sonic_call();
    new spell_throne_of_thunder_luciferase();
    new spell_throne_of_thunder_devoured();
    new spell_throne_of_thunder_triumphant_roar();      // 139815

    new go_displacement_pad();

    new at_lightning_span();
}

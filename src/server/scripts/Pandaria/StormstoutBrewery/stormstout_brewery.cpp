//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "stormstout_brewery.hpp"

/* Yells and events
First Alemental after Hoptallus:
Ancient Brewmaster says: Whatzit... are they... what are they doin' to our alementals?
Ancient Brewmaster yells: Hey... hey YOU! Those are OUR, flying... beer monsters?
The Tasting Room:
/// Check Intro in Boss script, each line corresponds to a spawn. //
Meteor-like shower. Two Bloated Brew Alementals spawn in the mid.
Meteor-like shower. Nine Bubbling Brew Alementals spawn in the mid.
Meteor-like shower. Four Sudsy Brew Alementals spawn in the mid.
Meteor-like shower. Boss Yan-zhu the Uncasked spawn in the mid.
*/

/// Chen Stormstout / Auntie Stormstout intro yells.
enum IntroYells
{
    AUNTIE_ENTRANCE_SAY_1 = 0, ///< Auntie Stormstout says: Oh, hello Zan, it is good of you to visit ol' Auntie Stormstout.
    CHEN_ENTRANCE_SAY_1   = 0, ///< Chen Stormstout   says: I am not Zan - I am Chen Stormstout!
    AUNTIE_ENTRANCE_SAY_2 = 1, ///< Auntie Stormstout says: Oh, Zan! You remind me so much of your father.
    CHEN_ENTRANCE_SAY_2   = 1, ///< Chen Stormstout   says: Tell me, what has happened here?
    AUNTIE_ENTRANCE_SAY_3 = 2, ///< Auntie Stormstout says: It is certainly a nice day outside!
    CHEN_ENTRANCE_SAY_3   = 2, ///< Chen Stormstout   says: Where are the other Stormstouts? Why are hozen all over the brewery?
    AUNTIE_ENTRANCE_SAY_4 = 3, ///< Auntie Stormstout says: Have you seen the size of Esme's turnips?
    CHEN_ENTRANCE_SAY_4   = 3, ///< Chen Stormstout   says: Auntie Stormstout... why is the brewery abandoned?
    AUNTIE_ENTRANCE_SAY_5 = 4, ///< Auntie Stormstout says: Abandoned? Oh heavens no! Uncle Gao is in charge while the others are beyond the wall. Isn't that exciting?
    CHEN_ENTRANCE_SAY_5   = 4, ///< Chen Stormstout   says: I see - and where is Uncle Gao?
    AUNTIE_ENTRANCE_SAY_6 = 5, ///< Auntie Stormstout says: I have some cookies for you!
    CHEN_ENTRANCE_SAY_6   = 5, ///< Chen Stormstout   says: There is no time for cookies! Well, one cookie. Just one.
    CHEN_ENTRANCE_SAY_7   = 6  ///< Chen Stormstout   says: Wait - these are ghost cookies. These aren't filling at all!
};

/// Hozen Bouncer yells.
enum BouncerYells
{
    SAY_OOK_KILLED = 0, ///< You take down Ook-Ook...
    SAY_MEANS      = 1, ///< You know what dat mean...
    SAY_NEW_OOK    = 2, ///< You da new Ook!
    SAY_PARTY      = 3  ///< Get da party started for da new Ook!
};

enum Spells
{
    /// FRIENDLY
    SPELL_AUNTIE_VISUAL     = 115618, ///< Auntie Stormstout visual.

    SPELL_GUSHING_BREW_BVIS = 114379, ///< Gushing Brew beam visual (The Great Wheel) cast by 54020 - General Purpose Bunny JMF
    SPELL_GUSHING_BREW_A    = 114380, ///< Gushing Brew aura (NPC that has beam cast on).
    SPELL_LEAKING_BEER_B_A  = 146604, ///< Dummy for NPC on Keg.
    SPELL_WASTED            = 114370,

    /// HOSTILE

    /// 56865 - Aqua Dancer - Once killed, their deaths will damage the Sodden Hozen Brawlers for half their health.
    SPELL_AQUATIC_ILLUSION  = 107044, ///< Gives Sodden Hozen Brawler SPELL_WATER_STRIKE.
    SPELL_AQUAT_ILLUSION_R  = 114655, ///< Removal damage.
    SPELL_SPLASH            = 107030,

    /// 56867 - Fiery Trickster - Once killed, their deaths will damage the Inflamed Hozen Brawlers for half their health.
    SPELL_FIERY_ILLUSION    = 107175, ///< Gives Inflamed Hozen Brawler SPELL_FIRE_STRIKE.
    SPELL_FIERY_ILLUSION_R  = 114656, ///< Removal damage.
    SPELL_BLAZING_SPARK_MIS = 107071,
    SPELL_BLAZING_SPARK_AT  = 107135,

    /// 59605 - Sodden Hozen Brawler
    SPELL_WATER_STRIKE      = 107046,
    SPELL_INV_STLTH_DETECT  = 18950,

    /// 56924 - Inflamed Hozen Brawler
    SPELL_FIRE_STRIKE       = 107176,

    /// 56849 - Hozen Bouncer - 2 bouncers only, after Ook-Ook. After yells they run, crash into each other, die.
    SPELL_HOZEN_DOORGUARD   = 107019,
    SPELL_DANCE_PARTY_ANIM  = 128271,

    /// 56863 - Sleepy Hozen Brawler, 56862 - Drunken Hozen Brawler.
    SPELL_COSMETIC_SLEEP    = 124557, ///< Used by Sleepy.
    SPELL_UPPERCUT          = 80182,

    /// 56731 - Habanero Brew
    SPELL_PROC_EXPLOSION    = 106787,
    SPELL_SPICY_EXPLOSION   = 107205,

    SPELL_BREW_BARREL_EXPL  = 107351, ///< Barrel monkey explosion.

    /// 59519 - Stout Brew Alemental
    SPELL_BREW_BOLT_STOUT   = 115652,
    SPELL_BLACKOUT_BREW     = 106851,
    SPELL_BLACKOUT_DRUNK    = 106857, ///< At 10 stacks of SPELL_BLACKOUT_BREW.
    SPELL_BLACKOUT_BREW_AUR = 114930,
    SPELL_COSMECTIC_DARK_BR = 128244,

    /// 56748/59522 - Sudsy Brew Alemental
    SPELL_BREW_BOLT2        = 115650,
    SPELL_SUDS              = 116178, ///< Creates NPC_POOL_OF_SUDS at target location.
    AURA_SUDS               = 116179, ///< Periodic dmg trigger aura.

    /// 56684 - Unruly Alemental
    SPELL_BREW_BOLT3        = 118104,
    SPELL_BREWHAHA          = 118099,

    /// 59521 - Bubbling Brew Alemental
    SPELL_BREW_BOLT4        = 116155,
    SPELL_BUBBLE_SHIELD     = 128708,

    /// 56746/59520 - Fizzy Brew Alemental
    /// Uses SPELL_BREW_BOLT2.
    SPELL_CARBONATION_M     = 116162, ///< Missile.
    SPELL_CARBONATION_S     = 116164, ///< Creates NPC_CARBONATION_POOL at target location.
    AURA_CARBONATION        = 116168, ///< Periodic dmg trigger aura.
    SPELL_FIZZY_BREW        = 114934,
    SPELL_COSM_WHEAT_BR_SPN = 128242,

    /// 59518 - Bloated Brew Alemental
    /// Uses SPELL_BREW_BOLT.
    SPELL_BLOAT             = 106546,

    /// 66413/59494 - Yeasty Brew Alemental
    /// Uses SPELL_BREW_BOLT4.
    SPELL_COSM_MED_BREW_SPW = 128243,
    SPELL_BREW_BOLT_YEASTY  = 116155,
    SPELL_FERMENT           = 106859,
    SPELL_FERMENT_HEAL      = 114451,
    SPELL_YEAST_SPANW_COSM  = 116259,

    /// 45979 - General Purpose Bunny JMF
    SPELL_RIDE_VEHICLE      = 93970,
    SPELL_BREW_WHEAT_EFFECT = 128254,
    SPELL_BREW_MED_EFFECT   = 128256,
    SPELL_BREW_DARK_EFFECT  = 128258,
    SPELL_GUSHING_BREW      = 114381,
    SPELL_MOONBEAM_VISUAL   = 128281
};

enum Events
{
    /// Trash
    EVENT_UPPERCUT = 1,
    EVENT_WATER_STRIKE,
    EVENT_FIRE_STRIKE,

    EVENT_BREW_BOLT,
    EVENT_BLACKOUT_BREW,
    EVENT_BREW_BOLT2,
    EVENT_SUDS,
    EVENT_BREW_BOLT3,
    EVENT_CARBONATION,
    EVENT_BLOAT,
    EVENT_BUBBLE_SHIELD,
    EVENT_BREW_BOLT4,

    /// Hozen Bouncer
    EVENT_SAY_OOK_KILLED,
    EVENT_BOUNCER_CHECK_PLAYERS,
    EVENT_RUN_AND_CRASH,

    /// Chen Stormstout / Auntie Stormstout intro event.
    EVENT_AUNTIE_ENTRANCE_SAY_1,
    EVENT_CHEN_ENTRANCE_SAY_1,
    EVENT_AUNTIE_ENTRANCE_SAY_2,
    EVENT_CHEN_ENTRANCE_SAY_2,
    EVENT_AUNTIE_ENTRANCE_SAY_3,
    EVENT_CHEN_ENTRANCE_SAY_3,
    EVENT_AUNTIE_ENTRANCE_SAY_4,
    EVENT_CHEN_ENTRANCE_SAY_4,
    EVENT_AUNTIE_ENTRANCE_SAY_5,
    EVENT_CHEN_ENTRANCE_SAY_5,
    EVENT_AUNTIE_ENTRANCE_SAY_6,
    EVENT_CHEN_ENTRANCE_SAY_6,
    EVENT_CHEN_ENTRANCE_SAY_7,

    /// NPC Dummy 45979
    EVENT_BREW_FINALE_EFFECT,
    EVENT_BREW_FINALE_SUMMON,
    EVENT_BREW_CHECK_PLAYERS
};

enum Actions
{
    ACTION_START_INTRO      = 0 ///< Chen Stormstout intro start.
};

enum Datas
{
    DATA_IS_IN_COMBAT       = 1
};

enum Equip
{
    EQUIP_PARTY_HOZEN   = 80580,
    EQUIP_DRUNKEN_HOZEN = 37924
};

Position const g_Wave1SummonPos[2] =
{
    { -701.170f, 1154.695f, 166.147f, 0.0f },
    { -705.623f, 1170.799f, 166.147f, 0.0f }
};

Position const g_Wave2SummonPos[9] =
{
    /// Back Line
    { -713.839f, 1168.498f, 166.147f, 0.0f },
    { -711.484f, 1160.516f, 166.147f, 0.0f },
    { -709.376f, 1152.371f, 166.147f, 0.0f },
    /// Middle Line
    { -705.623f, 1170.799f, 166.147f, 0.0f },
    { -703.452f, 1162.772f, 166.147f, 0.0f },
    { -701.170f, 1154.695f, 166.147f, 0.0f },
    /// Front Line
    { -697.545f, 1173.019f, 166.147f, 0.0f },
    { -695.321f, 1165.103f, 166.147f, 0.0f },
    { -693.034f, 1157.010f, 166.147f, 0.0f }
};

Position const g_Wave3SummonPos[4] =
{
    /// Back line
    { -708.536f, 1165.580f, 166.147f, 0.0f },
    { -706.338f, 1157.577f, 166.147f, 0.0f },
    /// Front Line
    { -700.357f, 1167.904f, 166.147f, 0.0f },
    { -698.245f, 1159.821f, 166.147f, 0.0f }
};

uint32 const g_BrewSpellId[3] = { SPELL_BREW_WHEAT_EFFECT, SPELL_BREW_MED_EFFECT, SPELL_BREW_DARK_EFFECT };

enum eWaveSummonNumber
{
    WAVE1_COUNT = 2,
    WAVE2_COUNT = 9,
    WAVE3_COUNT = 4,
};

// Instance Scripted events and dialogues.

// Set adds with p_Entry around p_Creature in combat in p_Dist radius. If p_Entry omitted, entries with same entry as p_Creature will EnterCombat
void SetOtherAddsInCombat(Creature* p_Creature, Unit* p_Target, uint32 p_Entry = 0, float p_Dist = 5.0f)
{
    if (!p_Creature || !p_Target || p_Target->GetTypeId() != TYPEID_PLAYER)
        return;

    p_Creature->AI()->AttackStart(p_Target);
    p_Creature->SetInCombatWith(p_Target);

    std::list<Creature*> l_CreatureList;
    GetCreatureListWithEntryInGrid(l_CreatureList, p_Creature, p_Entry ? p_Entry : p_Creature->GetEntry(), p_Dist);

    if (l_CreatureList.empty())
        return;

    for (Creature* l_Creature : l_CreatureList)
    {
        if (l_Creature != p_Creature && !l_Creature->AI()->GetData(DATA_IS_IN_COMBAT))
            l_Creature->AI()->EnterCombat(p_Target);

        l_Creature->AI()->AttackStart(p_Target);
        l_Creature->SetInCombatWith(p_Target);
    }
}

class at_stormstout_brewery_entrance : public AreaTriggerScript
{
    public:
        at_stormstout_brewery_entrance() : AreaTriggerScript("at_stormstout_brewery_entrance") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/, bool /*p_Enter*/)
        {
            if (player == nullptr)
                return false;

            InstanceScript* instance = player->GetInstanceScript();
            if (!instance)
                return true;

            if (instance->GetData(DATA_HOZEN_KILLED) < HOZEN_KILLS_NEEDED && instance->GetData(DATA_OOKOOK_EVENT) != DONE)
            {
                // Add the Hozen Counter aura.
                if (player->GetGroup())
                {
                    Player* l_Leader = ObjectAccessor::FindPlayer(player->GetGroup()->GetLeaderGUID());
                    Group*  l_Group  = l_Leader ? l_Leader->GetGroup() : nullptr;

                    if (l_Leader != nullptr && l_Group != nullptr)
                    {
                        for (GroupReference* itr = l_Group->GetFirstMember(); itr != NULL; itr = itr->next())
                        {
                            if (Player* member = itr->getSource())
                            {
                                if (!member->HasAura(SPELL_BANANA_BAR))
                                {
                                    member->AddAura(SPELL_BANANA_BAR, member);
                                    member->SetPower(POWER_ALTERNATE_POWER, instance->GetData(DATA_HOZEN_KILLED));
                                }
                            }
                        }
                    }
                }
                else
                {
                    player->AddAura(SPELL_BANANA_BAR, player);
                    player->SetPower(POWER_ALTERNATE_POWER, instance->GetData(DATA_HOZEN_KILLED));
                }

                // Make the intro event start, once.
                if (Creature* auntieStormstout = player->FindNearestCreature(NPC_AUNTIE_STORMSTOUT, 100.0f, true))
                {
                    if (!auntieStormstout->HasAura(SPELL_AUNTIE_VISUAL))
                    {
                        auntieStormstout->AddAura(SPELL_AUNTIE_VISUAL, auntieStormstout);
                        if (Creature* l_ChenStormstout = player->FindNearestCreature(NPC_CHEN_STORMSTOUT_ENTRANCE, 100.0f, true))
                        {
                            if (CreatureAI* l_AI = l_ChenStormstout->AI())
                                l_AI->DoAction(ACTION_START_INTRO);
                        }
                    }
                }
            }

            return true;
        }
};

// Chen Stormstout entrance 59704.
class npc_chen_stormstout_entrance : public CreatureScript
{
    public :
        npc_chen_stormstout_entrance() : CreatureScript("npc_chen_stormstout_entrance") { }

        struct npc_chen_stormstout_entrance_AI : public ScriptedAI
        {
            npc_chen_stormstout_entrance_AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;
            Creature* auntieStormstout;
            bool introStarted;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                auntieStormstout = NULL;
                introStarted = false;
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_START_INTRO && introStarted)
                    return;

                switch (action)
                {
                    case ACTION_START_INTRO:
                    {
                        if (Creature* auntie = me->FindNearestCreature(NPC_AUNTIE_STORMSTOUT, 100.0f, true))
                            auntieStormstout = auntie;
                        introStarted = true;
                        events.ScheduleEvent(EVENT_AUNTIE_ENTRANCE_SAY_1, 1000);
                        break;
                    }
                    default:
                        break;
                }
            };

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_AUNTIE_ENTRANCE_SAY_1:
                            auntieStormstout->AI()->Talk(AUNTIE_ENTRANCE_SAY_1);
                            events.ScheduleEvent(EVENT_CHEN_ENTRANCE_SAY_1, 8000);
                            break;
                        case EVENT_CHEN_ENTRANCE_SAY_1:
                            Talk(CHEN_ENTRANCE_SAY_1);
                            events.ScheduleEvent(EVENT_AUNTIE_ENTRANCE_SAY_2, 5000);
                            break;
                        case EVENT_AUNTIE_ENTRANCE_SAY_2:
                            auntieStormstout->AI()->Talk(AUNTIE_ENTRANCE_SAY_2);
                            events.ScheduleEvent(EVENT_CHEN_ENTRANCE_SAY_2, 6000);
                            break;
                        case EVENT_CHEN_ENTRANCE_SAY_2:
                            Talk(CHEN_ENTRANCE_SAY_2);
                            events.ScheduleEvent(EVENT_AUNTIE_ENTRANCE_SAY_3, 5000);
                            break;
                        case EVENT_AUNTIE_ENTRANCE_SAY_3:
                            auntieStormstout->AI()->Talk(AUNTIE_ENTRANCE_SAY_3);
                            events.ScheduleEvent(EVENT_CHEN_ENTRANCE_SAY_3, 5000);
                            break;
                        case EVENT_CHEN_ENTRANCE_SAY_3:
                            Talk(CHEN_ENTRANCE_SAY_3);
                            events.ScheduleEvent(EVENT_AUNTIE_ENTRANCE_SAY_4, 9000);
                            break;
                        case EVENT_AUNTIE_ENTRANCE_SAY_4:
                            auntieStormstout->AI()->Talk(AUNTIE_ENTRANCE_SAY_4);
                            events.ScheduleEvent(EVENT_CHEN_ENTRANCE_SAY_4, 5500);
                            break;
                        case EVENT_CHEN_ENTRANCE_SAY_4:
                            Talk(CHEN_ENTRANCE_SAY_4);
                            events.ScheduleEvent(EVENT_AUNTIE_ENTRANCE_SAY_5, 6500);
                            break;
                        case EVENT_AUNTIE_ENTRANCE_SAY_5:
                            auntieStormstout->AI()->Talk(AUNTIE_ENTRANCE_SAY_5);
                            events.ScheduleEvent(EVENT_CHEN_ENTRANCE_SAY_5, 11000);
                            break;
                        case EVENT_CHEN_ENTRANCE_SAY_5:
                            Talk(CHEN_ENTRANCE_SAY_5);
                            events.ScheduleEvent(EVENT_AUNTIE_ENTRANCE_SAY_6, 4500);
                            break;
                        case EVENT_AUNTIE_ENTRANCE_SAY_6:
                            auntieStormstout->AI()->Talk(AUNTIE_ENTRANCE_SAY_6);
                            events.ScheduleEvent(EVENT_CHEN_ENTRANCE_SAY_6, 4500);
                            break;
                        case EVENT_CHEN_ENTRANCE_SAY_6:
                            Talk(CHEN_ENTRANCE_SAY_6);
                            events.ScheduleEvent(EVENT_CHEN_ENTRANCE_SAY_7, 30000);
                            break;
                        case EVENT_CHEN_ENTRANCE_SAY_7:
                            Talk(CHEN_ENTRANCE_SAY_7);
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_chen_stormstout_entrance_AI(creature);
        }
};

// Instance trash scripts.

// Sodden Hozen Brawler 59605.
class npc_sodden_hozen_brawler : public CreatureScript
{
    public :
        npc_sodden_hozen_brawler() : CreatureScript("npc_sodden_hozen_brawler") { }

        struct npc_sodden_hozen_brawler_AI : public ScriptedAI
        {
            npc_sodden_hozen_brawler_AI(Creature* creature) : ScriptedAI(creature), summons(me)
            {
                m_SummonedFirstHelper = false;
            }

            EventMap events;
            SummonList summons;
            bool m_SummonedFirstHelper;
            bool m_HelperDead;
            bool m_IsInCombat;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();
                m_HelperDead = false;
                m_IsInCombat = false;

                // Summon the "helper".
                if (!m_SummonedFirstHelper)
                {
                    me->SummonCreature(NPC_AQUA_DANCER, me->GetPositionX(), me->GetPositionY() - 3.0f, me->GetPositionZ() + 8.0f, me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                    m_SummonedFirstHelper = true;
                }

                me->SetReactState(REACT_DEFENSIVE);
            }

            void EnterCombat(Unit* p_Who)
            {
                if (!m_IsInCombat && p_Who->GetTypeId() == TYPEID_PLAYER)
                {
                    m_IsInCombat = true;
                    me->SetReactState(REACT_AGGRESSIVE);
                    SetOtherAddsInCombat(me, p_Who);
                }
            }

            uint32 GetData(uint32 p_Data)
            {
                if (p_Data == DATA_IS_IN_COMBAT)
                    return m_IsInCombat ? 1 : 0;
                return 0;
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustReachedHome()
            {
                if (m_HelperDead)
                    me->SummonCreature(NPC_AQUA_DANCER, me->GetPositionX(), me->GetPositionY() - 3.0f, me->GetPositionZ() + 8.0f, me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
            }

            void JustDied(Unit* /*killer*/)
            {
                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->setActive(true);

		        if (me->isInCombat())
                    summon->SetInCombatWithZone();

                if (summon->GetEntry() == NPC_AQUA_DANCER)
                {
                    summon->SetCanFly(true);
                    summon->SetDisableGravity(true);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->CastSpell(me, SPELL_AQUATIC_ILLUSION, false);
                }
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
            {
                summons.Despawn(summon);
                me->RemoveAurasDueToSpell(SPELL_AQUATIC_ILLUSION);
                DoCast(me, SPELL_AQUAT_ILLUSION_R);
                m_HelperDead = true;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sodden_hozen_brawler_AI(creature);
        }
};

// Inflamed Hozen Brawler 56924.
class npc_inflamed_hozen_brawler : public CreatureScript
{
    public :
        npc_inflamed_hozen_brawler() : CreatureScript("npc_inflamed_hozen_brawler") { }

        struct npc_inflamed_hozen_brawler_AI : public ScriptedAI
        {
            npc_inflamed_hozen_brawler_AI(Creature* creature) : ScriptedAI(creature), summons(me)
            {
                m_SummonedFirstHelper = false;
            }

            EventMap events;
            SummonList summons;
            bool m_SummonedFirstHelper;
            bool m_HelperDead;
            bool m_IsInCombat;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();
                m_HelperDead = false;
                m_IsInCombat = false;

                // Summon the "helper".
                if (!m_SummonedFirstHelper)
                {
                    me->SummonCreature(NPC_FIERY_TRICKSTER, me->GetPositionX(), me->GetPositionY() - 3.0f, me->GetPositionZ() + 8.0f, me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                    m_SummonedFirstHelper = true;
                }

                me->SetReactState(REACT_DEFENSIVE);
            }

            void EnterCombat(Unit* p_Who)
            {
                if (!m_IsInCombat && p_Who->GetTypeId() == TYPEID_PLAYER)
                {
                    m_IsInCombat = true;
                    me->SetReactState(REACT_AGGRESSIVE);
                    SetOtherAddsInCombat(me, p_Who);
                }
            }

            uint32 GetData(uint32 p_Data)
            {
                if (p_Data == DATA_IS_IN_COMBAT)
                    return m_IsInCombat ? 1 : 0;
                return 0;
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustReachedHome()
            {
                if (m_HelperDead)
                    me->SummonCreature(NPC_FIERY_TRICKSTER, me->GetPositionX(), me->GetPositionY() - 3.0f, me->GetPositionZ() + 8.0f, me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
            }

            void JustDied(Unit* /*killer*/)
            {
                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->setActive(true);

		        if (me->isInCombat())
                    summon->SetInCombatWithZone();

                if (summon->GetEntry() == NPC_FIERY_TRICKSTER)
                {
                    summon->SetCanFly(true);
                    summon->SetDisableGravity(true);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->CastSpell(me, SPELL_FIERY_ILLUSION, false);
                }
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
            {
                summons.Despawn(summon);
                me->RemoveAurasDueToSpell(SPELL_FIERY_ILLUSION);
                DoCast(me, SPELL_FIERY_ILLUSION_R);
                m_HelperDead = true;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_inflamed_hozen_brawler_AI(creature);
        }
};

// Hozen Bouncer 56849.
class npc_hozen_bouncer : public CreatureScript
{
    public :
        npc_hozen_bouncer() : CreatureScript("npc_hozen_bouncer") { }

        struct npc_hozen_bouncer_AI : public ScriptedAI
        {
            npc_hozen_bouncer_AI(Creature* creature) : ScriptedAI(creature)
            {
                m_Instance = creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap events;
            bool m_IsInCombat;
            uint8 l_SayCount;

            void Reset()
            {
                events.Reset();
                m_IsInCombat = false;

                if (m_Instance)
                    m_IsInCombat = !(m_Instance->GetBossState(DATA_OOKOOK) == DONE);

                me->SetReactState(REACT_PASSIVE);
                l_SayCount = 0;

                if (m_IsInCombat)
                    events.ScheduleEvent(EVENT_BOUNCER_CHECK_PLAYERS, 300);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_OOK_OOK_TALK:
                    {
                        events.ScheduleEvent(EVENT_SAY_OOK_KILLED, 2000);
                        break;
                    }
                    case ACTION_OOK_OOK_DEAD:
                    {
                        m_IsInCombat = false;
                        me->SetReactState(REACT_PASSIVE);
                        me->RemoveAura(SPELL_HOZEN_DOORGUARD);
                        events.ScheduleEvent(EVENT_RUN_AND_CRASH, 18000);
                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterEvadeMode()
            {
                if (m_Instance)
                    m_IsInCombat = !(m_Instance->GetBossState(DATA_OOKOOK) == DONE);

                if (m_IsInCombat)
                    events.ScheduleEvent(EVENT_BOUNCER_CHECK_PLAYERS, 300);

                me->GetMotionMaster()->MoveTargetedHome();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (!me->isAlive() || type != POINT_MOTION_TYPE || id != 1)
                    return;

                me->CastSpell(me, SPELL_DANCE_PARTY_ANIM, true);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BOUNCER_CHECK_PLAYERS:
                            if (me->FindNearestPlayer(5.0) && m_IsInCombat)
                            {
                                if (!me->HasAura(SPELL_HOZEN_DOORGUARD))
                                    me->CastSpell(me, SPELL_HOZEN_DOORGUARD, true);
                                events.ScheduleEvent(EVENT_BOUNCER_CHECK_PLAYERS, 300);
                            }
                            else
                                me->RemoveAura(SPELL_HOZEN_DOORGUARD);
                            break;
                        case EVENT_SAY_OOK_KILLED:
                            Talk(SAY_OOK_KILLED + l_SayCount++);
                            if (l_SayCount < 3)
                                events.ScheduleEvent(EVENT_SAY_OOK_KILLED, 4000);
                            break;
                        case EVENT_RUN_AND_CRASH:
                            me->GetMotionMaster()->MovePoint(1, -747.929f, 1323.334f, 146.715f);
                            break;
                        default:
                            break;
                    }
                }

                if (m_IsInCombat)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_hozen_bouncer_AI(creature);
        }
};

// Sleepy Hozen Brawler 56863, Drunken Hozen Brawler 56862.
class npc_drunken_sleepy_hozen_brawler : public CreatureScript
{
    public :
        npc_drunken_sleepy_hozen_brawler() : CreatureScript("npc_drunken_sleepy_hozen_brawler") { }

        struct npc_drunken_sleepy_hozen_brawler_AI : public ScriptedAI
        {
            npc_drunken_sleepy_hozen_brawler_AI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool m_IsInCombat;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();

                m_IsInCombat = false;
                // Add Sleep cosmetic to the Sleepy Hozen Brawlers.
                if (me->GetEntry() == NPC_SLEEPY_HOZEN_BRAWLER && !me->HasAura(SPELL_COSMETIC_SLEEP))
                    me->AddAura(SPELL_COSMETIC_SLEEP, me);
                // Add weapon & fighting visual for Drunken Hozen Brawlers.
                else
                {
                    SetEquipmentSlots(false, EQUIP_DRUNKEN_HOZEN, 0, 0);
                    events.ScheduleEvent(EVENT_UPPERCUT, 2000);
                }

                me->SetReactState(REACT_DEFENSIVE);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& /*p_Damage*/)
            {
                if (m_IsInCombat)
                    return;
            }

            void EnterCombat(Unit* p_Who)
            {
                // Sleeping Hozen
                if (me->GetEntry() == NPC_SLEEPY_HOZEN_BRAWLER)
                {
                    // Remove Sleep cosmetic from the Sleepy Hozen Brawlers.
                    me->RemoveAurasDueToSpell(SPELL_COSMETIC_SLEEP);

                    // Check the Habanero Brew barrels.
                    if (Creature* habanero = me->FindNearestCreature(NPC_HABANERO_BREW, 10.0f))
                        habanero->CastSpell(habanero, SPELL_SPICY_EXPLOSION, false);
                }
                // Drunken Hozen
                else if (!m_IsInCombat && p_Who->GetTypeId() == TYPEID_PLAYER)
                {
                    m_IsInCombat = true;
                    me->SetReactState(REACT_AGGRESSIVE);
                    SetOtherAddsInCombat(me, p_Who);
                    SetOtherAddsInCombat(me, p_Who, NPC_HOZEN_PARTY_ANIMAL);
                }
            }

            uint32 GetData(uint32 p_Data)
            {
                if (p_Data == DATA_IS_IN_COMBAT)
                    return m_IsInCombat ? 1 : 0;
                return 0;
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustReachedHome()
            {
                // Add Sleep cosmetic to the Sleepy Hozen Brawlers.
                if (me->GetEntry() == NPC_SLEEPY_HOZEN_BRAWLER && !me->HasAura(SPELL_COSMETIC_SLEEP))
                    me->AddAura(SPELL_COSMETIC_SLEEP, me);
                else
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_ATTACK1H);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_UPPERCUT:
                        {
                            if (!m_IsInCombat)
                            {
                                std::list<Creature*> l_HozenList;
                                GetCreatureListWithEntryInGrid(l_HozenList, me, NPC_DRUNKEN_HOZEN_BRAWLER, 5.0f);
                                for (Creature* l_Hozen : l_HozenList)
                                {
                                    if (l_Hozen == me)
                                        continue;

                                    me->CastSpell(l_Hozen, SPELL_UPPERCUT, true);
                                }

                                events.ScheduleEvent(EVENT_UPPERCUT, 2000);
                            }

                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_drunken_sleepy_hozen_brawler_AI(creature);
        }
};

// 59684 - Hozen Party Animal
class npc_hozen_party_animal : public CreatureScript
{
    public:
        npc_hozen_party_animal() : CreatureScript("npc_hozen_party_animal") { }

        struct npc_hozen_party_animalAI : public ScriptedAI
        {
            npc_hozen_party_animalAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_IsInCombat;

            void Reset()
            {
                m_IsInCombat = false;
                me->SetReactState(REACT_DEFENSIVE);
                SetEquipmentSlots(false, EQUIP_PARTY_HOZEN, 0, 0);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (!m_IsInCombat && p_Attacker->GetTypeId() == TYPEID_PLAYER)
                {
                    m_IsInCombat = true;
                    me->SetReactState(REACT_AGGRESSIVE);
                    SetOtherAddsInCombat(me, p_Attacker);
                }
            }

            uint32 GetData(uint32 p_Data)
            {
                if (p_Data == DATA_IS_IN_COMBAT)
                    return m_IsInCombat ? 1 : 0;
                return 0;
            }

            void EnterEvadeMode()
            {
                Reset();
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE || p_Id != 2)
                    return;

                me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_hozen_party_animalAI(p_Creature);
        }
};

// Stout Brew Alemental 59519.
class npc_stout_brew_alemental : public CreatureScript
{
    public :
        npc_stout_brew_alemental() : CreatureScript("npc_stout_brew_alemental") { }

        struct npc_stout_brew_alemental_AI : public ScriptedAI
        {
            npc_stout_brew_alemental_AI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                events.ScheduleEvent(EVENT_BREW_BOLT, urand(2000, 6000));
                events.ScheduleEvent(EVENT_BLACKOUT_BREW, urand(12000, 17000));
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BREW_BOLT:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BREW_BOLT_STOUT);
                            events.ScheduleEvent(EVENT_BREW_BOLT, urand(7000, 11000));
                            break;
                        }
                        case EVENT_BLACKOUT_BREW:
                        {
                            DoCast(me, SPELL_BLACKOUT_BREW);
                            events.ScheduleEvent(EVENT_BLACKOUT_BREW, urand(18000, 23000));
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_stout_brew_alemental_AI(creature);
        }
};

// Sudsy Brew Alemental 59522.
class npc_sudsy_brew_alemental : public CreatureScript
{
    public :
        npc_sudsy_brew_alemental() : CreatureScript("npc_sudsy_brew_alemental") { }

        struct npc_sudsy_brew_alemental_AI : public ScriptedAI
        {
            npc_sudsy_brew_alemental_AI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                events.ScheduleEvent(EVENT_BREW_BOLT2, urand(2000, 6000));
                events.ScheduleEvent(EVENT_SUDS, urand(9000, 12000));
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BREW_BOLT2:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BREW_BOLT2);
                            events.ScheduleEvent(EVENT_BREW_BOLT2, urand(7000, 11000));
                            break;
                        }
                        case EVENT_SUDS:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_SUDS);
                            events.ScheduleEvent(EVENT_SUDS, urand(18000, 22000));
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sudsy_brew_alemental_AI(creature);
        }
};

// Pool of Suds 56748.
class npc_pool_of_suds : public CreatureScript
{
    public :
        npc_pool_of_suds() : CreatureScript("npc_pool_of_suds") { }

        struct npc_pool_of_suds_AI : public ScriptedAI
        {
            npc_pool_of_suds_AI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                Reset();
            }

            void Reset()
            {
                me->AddAura(AURA_SUDS, me);
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(const uint32 diff) { } // No melee.
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_pool_of_suds_AI(creature);
        }
};

// Unruly Alemental 56684.
class npc_unruly_alemental : public CreatureScript
{
    public :
        npc_unruly_alemental() : CreatureScript("npc_unruly_alemental") { }

        struct npc_unruly_alemental_AI : public ScriptedAI
        {
            npc_unruly_alemental_AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();

                if (!me->HasAura(SPELL_BREWHAHA))
                    me->AddAura(SPELL_BREWHAHA, me);
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                events.ScheduleEvent(EVENT_BREW_BOLT3, urand(2000, 6000));
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustReachedHome()
            {
                if (!me->HasAura(SPELL_BREWHAHA))
                    me->AddAura(SPELL_BREWHAHA, me);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BREW_BOLT3:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BREW_BOLT3);
                            events.ScheduleEvent(EVENT_BREW_BOLT3, urand(7000, 11000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_unruly_alemental_AI(creature);
        }
};

// Fizzy Brew Alemental 59520.
class npc_fizzy_brew_alemental : public CreatureScript
{
    public :
        npc_fizzy_brew_alemental() : CreatureScript("npc_fizzy_brew_alemental") { }

        struct npc_fizzy_brew_alemental_AI : public ScriptedAI
        {
            npc_fizzy_brew_alemental_AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                events.ScheduleEvent(EVENT_BREW_BOLT2, urand(2000, 6000));
                events.ScheduleEvent(EVENT_CARBONATION, urand(9000, 12000));
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BREW_BOLT2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BREW_BOLT2);
                            events.ScheduleEvent(EVENT_BREW_BOLT2, urand(7000, 11000));
                            break;
                        case EVENT_CARBONATION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_CARBONATION_M);
                            events.ScheduleEvent(EVENT_CARBONATION, urand(22000, 26000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_fizzy_brew_alemental_AI(creature);
        }
};

// Pool of Carbonation 56746.
class npc_pool_of_carbonation : public CreatureScript
{
    public :
        npc_pool_of_carbonation() : CreatureScript("npc_pool_of_carbonation") { }

        struct npc_pool_of_carbonation_AI : public ScriptedAI
        {
            npc_pool_of_carbonation_AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                Reset();
            }

            void Reset()
            {
                me->AddAura(AURA_CARBONATION, me);
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 const diff) { } // No melee.
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_pool_of_carbonation_AI(creature);
        }
};

// Bloated Brew Alemental 59518.
class npc_bloated_brew_alemental : public CreatureScript
{
    public :
        npc_bloated_brew_alemental() : CreatureScript("npc_bloated_brew_alemental") { }

        struct npc_bloated_brew_alemental_AI : public ScriptedAI
        {
            npc_bloated_brew_alemental_AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                events.ScheduleEvent(EVENT_BREW_BOLT, urand(2000, 6000));
                events.ScheduleEvent(EVENT_BLOAT, urand(10000, 14000));
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BREW_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BREW_BOLT_YEASTY);
                            events.ScheduleEvent(EVENT_BREW_BOLT, urand(7000, 11000));
                            break;
                        case EVENT_BLOAT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BLOAT);
                            events.ScheduleEvent(EVENT_BLOAT, urand(32000, 37000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_bloated_brew_alemental_AI(creature);
        }
};

// Bubbling Brew Alemental 59521.
class npc_bubbling_brew_alemental : public CreatureScript
{
    public :
        npc_bubbling_brew_alemental() : CreatureScript("npc_bubbling_brew_alemental") { }

        struct npc_bubbling_brew_alemental_AI : public ScriptedAI
        {
            npc_bubbling_brew_alemental_AI(Creature* creature) : ScriptedAI(creature), summons(me)
            {
            }

            EventMap events;
            SummonList summons;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();
            }

            void JustDied(Unit* p_Killer)
            {
                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                events.ScheduleEvent(EVENT_BREW_BOLT4, urand(2000, 6000));
                events.ScheduleEvent(EVENT_BUBBLE_SHIELD, urand(9000, 17000));
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->setActive(true);

		        if (me->isInCombat())
                    summon->SetInCombatWithZone();

                if (summon->GetEntry() == NPC_BUBBLE_SHIELD_TRASH)
                {
                    summon->SetReactState(REACT_PASSIVE);
                    summon->AddAura(SPELL_BUBBLE_SHIELD, me);
                }
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
            {
                summons.Despawn(summon);

				if (Aura* aura = me->GetAura(SPELL_BUBBLE_SHIELD))
                {
                    if (aura->GetStackAmount() > 1)
                        me->SetAuraStack(SPELL_BUBBLE_SHIELD, me, aura->GetStackAmount() - 1);
                    else
                        me->RemoveAurasDueToSpell(SPELL_BUBBLE_SHIELD);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BREW_BOLT4:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BREW_BOLT4);
                            events.ScheduleEvent(EVENT_BREW_BOLT4, urand(7000, 11000));
                            break;
                        case EVENT_BUBBLE_SHIELD:
                            for (uint8 i = 0; i < 4; i++)
                                me->SummonCreature(NPC_BUBBLE_SHIELD_TRASH, me->GetPositionX() + frand(3.0f, -3.0f), me->GetPositionX() + frand(3.0f, -3.0f), me->GetPositionZ() + 0.1f, 0, TEMPSUMMON_MANUAL_DESPAWN);
                            events.ScheduleEvent(EVENT_BUBBLE_SHIELD, urand(42000, 54000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_bubbling_brew_alemental_AI(creature);
        }
};

// Yeasty Brew Alemental 59494.
class npc_yeasty_brew_alemental : public CreatureScript
{
    public :
        npc_yeasty_brew_alemental() : CreatureScript("npc_yeasty_brew_alemental") { }

        struct npc_yeasty_brew_alemental_AI : public ScriptedAI
        {
            npc_yeasty_brew_alemental_AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void InitializeAI()
            {
                if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                events.ScheduleEvent(EVENT_BREW_BOLT4, urand(2000, 6000));
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BREW_BOLT4:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_BREW_BOLT4);
                            events.ScheduleEvent(EVENT_BREW_BOLT4, urand(7000, 11000));
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_yeasty_brew_alemental_AI(creature);
        }
};

// 45979 - General Purpose Bunny JMF - NPC already used in another dungeon with an aura...
class npc_general_purpose_bunny : public CreatureScript
{
    public:
        npc_general_purpose_bunny() : CreatureScript("npc_general_purpose_bunny") { }

        struct npc_general_purpose_bunnyAI : public ScriptedAI
        {
            npc_general_purpose_bunnyAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_IsInBrewery = p_Creature->GetMapId() == 961;
                m_Instance = p_Creature->GetInstanceScript();
            }

            bool m_IsInBrewery;
            bool m_IsCarrot;
            bool m_IsYanZhuSummoner;
            bool m_IsBrewFinaleMob;
            bool m_EventLaunched;
            uint8 m_SummonWaveCount;
            uint8 m_EffectCount;
            uint8 m_SummonsCount;
            uint64 m_YanZhuSummonerGuid;
            uint64 m_BrewLauncherGuid;
            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset()
            {
                m_Events.Reset();
                m_IsCarrot = false;
                m_IsYanZhuSummoner = false;
                m_IsBrewFinaleMob = false;
                m_EventLaunched = false;
                m_SummonWaveCount = 0;
                m_EffectCount = 0;
                m_YanZhuSummonerGuid = 0;
                m_BrewLauncherGuid = 0;

                if (m_IsInBrewery)
                {
                    me->RemoveAllAuras();
                    me->SetDisplayId(DISPLAYID_INVISIBLE);

                    // Carrot NPC (Hoptallus door)
                    if (me->GetPositionX() > -683.0f && me->GetPositionX() < -682.0f && me->GetPositionY() > 1249.0f && me->GetPositionY() < 1250.0f &&
                        me->GetPositionZ() > 162.0f  && me->GetPositionZ() < 163.0f)
                        m_IsCarrot = true;
                    // Yan Zhu Summoner (at center of Yan Zhu's room)
                    else if (me->GetPositionX() > -704.0f && me->GetPositionX() < -703.0f && me->GetPositionY() > 1162.0f && me->GetPositionY() < 1163.0f &&
                             me->GetPositionZ() > 166.0f  && me->GetPositionZ() < 167.0f)
                    {
                        std::list<Creature*> l_BunnyList;
                        GetCreatureListWithEntryInGrid(l_BunnyList, me, NPC_GENERAL_PURPOSE_BUNNY_B, 50.0f);
                        for (Creature* l_Bunny : l_BunnyList)
                        {
                            // Need brew launcher to make it cast Finale Brew spells on Yan Zhu Summoner
                            if (l_Bunny->AI()->GetData(DATA_YANZHU_EVENT_SPURT))
                                m_BrewLauncherGuid = l_Bunny->GetGUID();
                        }
                        m_IsYanZhuSummoner = true;
                        m_Events.ScheduleEvent(EVENT_BREW_CHECK_PLAYERS, 1000);
                    }
                    // Brew launcher (in the corner of Yan Zhu's room)
                    else if (me->GetPositionX() > -679.0f && me->GetPositionX() < -678.0f && me->GetPositionY() > 1197.0f && me->GetPositionY() < 1198.0f &&
                             me->GetPositionZ() > 166.0f  && me->GetPositionZ() < 167.0f)
                    {
                        std::list<Creature*> l_BunnyList;
                        GetCreatureListWithEntryInGrid(l_BunnyList, me, NPC_GENERAL_PURPOSE_BUNNY_B, 50.0f);
                        for (Creature* l_Bunny : l_BunnyList)
                        {
                            // Need Yan Zhu Summoner
                            if (l_Bunny->AI()->GetData(DATA_YANZHU_THE_UNCASKED_EVENT))
                                m_YanZhuSummonerGuid = l_Bunny->GetGUID();
                        }
                        m_IsBrewFinaleMob = true;
                    }
                }
            }

            void SummonedCreatureDies(Creature* p_Summon, Unit* p_Killer)
            {
                --m_SummonsCount;
                if (m_SummonsCount == 0)
                    DoAction(ACTION_SUMMON_YANZHU_WAVES);
            }

            uint32 GetData(uint32 p_Data)
            {
                if (p_Data == DATA_YANZHU_THE_UNCASKED_EVENT)
                    return m_IsYanZhuSummoner ? 1 : 0;

                if (p_Data == DATA_YANZHU_EVENT_SPURT)
                    return m_IsBrewFinaleMob ? 1 : 0;

                return 0;
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_SUMMON_YANZHU_WAVES)
                {
                    ++m_SummonWaveCount;
                    m_Events.ScheduleEvent(EVENT_BREW_FINALE_EFFECT, 1000);
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_BREW_CHECK_PLAYERS:
                    {
                        if (me->FindNearestPlayer(8.0f))
                        {
                            m_EventLaunched = true;
                            DoAction(ACTION_SUMMON_YANZHU_WAVES);
                        }
                        else
                            m_Events.ScheduleEvent(EVENT_BREW_CHECK_PLAYERS, 1000);
                        break;
                    }
                    case EVENT_BREW_FINALE_EFFECT:
                    {
                        if (Creature* l_BrewLauncher = Creature::GetCreature(*me, m_BrewLauncherGuid))
                        {
                            /// One effect per trash wave
                            if (m_SummonWaveCount < 3)
                            {
                                Position l_TargetPoint = GetTargetPoint();
                                l_BrewLauncher->CastSpell(l_TargetPoint.GetPositionX(), l_TargetPoint.GetPositionY(), l_TargetPoint.GetPositionZ(), g_BrewSpellId[m_SummonWaveCount - 1], true);
                            }
                            /// All effects for boss
                            else
                            {
                                for (uint8 l_Idx = 0; l_Idx < 3; ++l_Idx)
                                {
                                    Position l_TargetPoint = GetTargetPoint();
                                    l_BrewLauncher->CastSpell(l_TargetPoint.GetPositionX(), l_TargetPoint.GetPositionY(), l_TargetPoint.GetPositionZ(), g_BrewSpellId[l_Idx], true);
                                }
                            }
                        }

                        if (++m_EffectCount >= 10)
                            m_Events.ScheduleEvent(EVENT_BREW_FINALE_SUMMON, 2000);
                        else
                            m_Events.ScheduleEvent(EVENT_BREW_FINALE_EFFECT, 400);
                        break;
                    }
                    case EVENT_BREW_FINALE_SUMMON:
                    {
                        switch (m_SummonWaveCount)
                        {
                            case 1:
                                for (m_SummonsCount = 0; m_SummonsCount < WAVE1_COUNT; ++m_SummonsCount)
                                    me->SummonCreature(NPC_BLOATED_BREW_ALEMENTAL, g_Wave1SummonPos[m_SummonsCount]);
                                break;
                            case 2:
                                for (m_SummonsCount = 0; m_SummonsCount < WAVE2_COUNT; ++m_SummonsCount)
                                    me->SummonCreature(NPC_BUBBLING_BREW_ALEMENTAL, g_Wave2SummonPos[m_SummonsCount]);
                                break;
                            case 3:
                                for (m_SummonsCount = 0; m_SummonsCount < WAVE3_COUNT; ++m_SummonsCount)
                                    me->SummonCreature(NPC_FIZZY_BREW_ALEMENTAL, g_Wave3SummonPos[m_SummonsCount]);
                                break;
                            case 4:
                                me->SummonCreature(BOSS_YANZHU_THE_UNCASKED, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                                break;
                            default:
                                break;
                        }

                        m_EffectCount = 0;
                        break;
                    }
                    default:
                        break;
                }
            }

            Position GetTargetPoint()
            {
                float l_Dist = frand(0.0f, 8.0f);
                float l_Ori  = frand(0.0f, 2 * M_PI);
                float l_PosX = me->GetPositionX() + l_Dist * cos(l_Ori);
                float l_PosY = me->GetPositionY() + l_Dist * sin(l_Ori);

                return { l_PosX, l_PosY, me->GetPositionZ(), 0.0f };
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_general_purpose_bunnyAI(p_Creature);
        }
};

// Spicy Explosion 107205, triggered by SPELL_PROC_EXPLOSION.
class spell_stormstout_brewery_habanero_beer : public SpellScriptLoader
{
    public:
        spell_stormstout_brewery_habanero_beer() : SpellScriptLoader("spell_stormstout_brewery_habanero_beer") { }

        class spell_stormstout_brewery_habanero_beer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_stormstout_brewery_habanero_beer_SpellScript);

            void HandleInstaKill(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                std::list<Creature*> creatureList;
                GetCreatureListWithEntryInGrid(creatureList, GetCaster(), NPC_HABANERO_BREW, 10.0f);

                GetCaster()->RemoveAurasDueToSpell(SPELL_PROC_EXPLOSION);

                for (std::list<Creature*>::iterator barrel = creatureList.begin(); barrel != creatureList.end(); barrel++)
                {
                    if ((*barrel)->HasAura(SPELL_PROC_EXPLOSION))
                    {
                        (*barrel)->RemoveAurasDueToSpell(SPELL_PROC_EXPLOSION);
                        (*barrel)->CastSpell(*barrel, SPELL_SPICY_EXPLOSION, true);
                    }
                }
            }

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    if (caster->ToCreature())
                        caster->ToCreature()->DespawnOrUnsummon(1000);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_stormstout_brewery_habanero_beer_SpellScript::HandleInstaKill, EFFECT_1, SPELL_EFFECT_INSTAKILL);
                AfterCast += SpellCastFn(spell_stormstout_brewery_habanero_beer_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_stormstout_brewery_habanero_beer_SpellScript();
        }
};

/// Dance Party Animkit - 128271
class spell_stormstout_brewery_dance_party_animkit : public SpellScriptLoader
{
    public:
        spell_stormstout_brewery_dance_party_animkit() : SpellScriptLoader("spell_stormstout_brewery_dance_party_animkit") { }

        class spell_stormstout_brewery_dance_party_animkit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_stormstout_brewery_dance_party_animkit_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->GetTypeId() == TypeID::TYPEID_PLAYER)
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_stormstout_brewery_dance_party_animkit_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_stormstout_brewery_dance_party_animkit_SpellScript();
        }
};

void AddSC_stormstout_brewery()
{
    new at_stormstout_brewery_entrance();               ///< 7998
    new npc_chen_stormstout_entrance();                 ///< 59704
    new npc_sodden_hozen_brawler();                     ///< 59605
    new npc_inflamed_hozen_brawler();                   ///< 59624
    new npc_hozen_bouncer();                            ///< 56849
    new npc_drunken_sleepy_hozen_brawler();             ///< 56862 - 56863
    new npc_hozen_party_animal();                       ///< 56984
    new npc_stout_brew_alemental();                     ///< 59519
    new npc_sudsy_brew_alemental();                     ///< 59522
    new npc_pool_of_suds();                             ///< 56748
    new npc_unruly_alemental();                         ///< 56684
    new npc_fizzy_brew_alemental();                     ///< 59520
    new npc_pool_of_carbonation();                      ///< 56746
    new npc_bloated_brew_alemental();                   ///< 59518
    new npc_bubbling_brew_alemental();                  ///< 59521
    new npc_yeasty_brew_alemental();                    ///< 59494
    new npc_general_purpose_bunny();                    ///< 45979
    new spell_stormstout_brewery_habanero_beer();       ///< 107205
    new spell_stormstout_brewery_dance_party_animkit(); ///< 128271
}

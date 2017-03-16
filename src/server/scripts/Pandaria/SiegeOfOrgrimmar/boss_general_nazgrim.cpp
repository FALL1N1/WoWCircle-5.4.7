#include "siege_of_orgrimmar.hpp"

#define NAZGRIM_POWER_TYPE POWER_MANA
#define NAZGRIM_POWER_MAX 100

enum ScriptedTexts
{
    SAY_AGGRO_ALLIANCE  = 0,
    SAY_AGGRO_HORDE     = 1,
    SAY_DEATH_ALLIANCE  = 2,
    SAY_DEATH_GAMON_1   = 3,
    SAY_DEATH_GAMON_2   = 4,
    SAY_DEATH_HORDE_1   = 5,
    SAY_DEATH_HORDE_2   = 6,
    SAY_ADDS            = 7,
    SAY_ADDS_ALL        = 8,
    SAY_INTRO_ALLIANCE  = 9,

    SAY_IRON_JUGGERNAUT_INTRO   = 10,
    SAY_IRON_JUGGERNAUT_DEATH   = 11,
    SAY_IRON_JUGGERNAUT_OUTRO   = 12,
    
    SAY_INTRO_HORDE     = 13,
    SAY_WIPE            = 14,
};

enum Spells
{
    SPELL_ZERO_MANA_REGEN           = 96301,

    SPELL_BERSERK                   = 26662,

    SPELL_BATTLE_STANCE             = 143589,
    SPELL_BERSERKER_STANCE          = 143594,
    SPELL_DEFENSIVE_STANCE          = 143593,

    SPELL_GENERATE_RAGE             = 144278, // from defensive stance
    SPELL_GENERATE_RAGE_ENERGIZE    = 143597,

    SPELL_COOLING_OFF               = 143484,

    SPELL_ENEGRIZE_NAZGRIM_1        = 143590, // 3 points from banner
    SPELL_ENERGIZE_NAZGRIM_2        = 143876, // 3 points from aftershock
    SPELL_ENERGIZE_NAZGRIM_3        = 143877, // 5 points from ravager

    SPELL_SUNDERING_BLOW            = 143494,

    SPELL_BONECRACKER               = 143638,

    SPELL_HEROIC_SHOCKWAVE          = 143500,
    SPELL_HEROIC_SHOCKWAVE_DMG      = 143716,
    SPELL_HEROIC_SHOCKWAVE_FORCE    = 143713,
    SPELL_HEROIC_SHOCKWAVE_SUMMON   = 143715,

    SPELL_AFTERSHOCK                = 143712,

    SPELL_KORKRON_BANNER            = 143591,
    SPELL_KORKRON_BANNER_SUMMON     = 143501,
    SPELL_KORKRON_BANNER_AURA       = 143536,
    SPELL_KORKRON_BANNER_BUFF       = 143535,

    SPELL_WAR_SONG                  = 143503,

    SPELL_RAVAGER                   = 143872,
    SPELL_RAVAGER_AURA              = 143874,
    SPELL_RAVAGER_DMG               = 143873,

    // Kor'kron Ironblade
    SPELL_IRONSTORM                 = 143420,
    SPELL_IRONSTORM_DMG             = 143421,
    SPELL_LAST_STAND                = 143427,

    // Kor'kron Arcweaver
    SPELL_ARCANE_SHOCK              = 143432,
    SPELL_MAGICSTRIKE               = 143431,
    SPELL_UNSTABLE_BLINK            = 143433,

    // Kor'kron Assassin
    SPELL_ASSASSINS_MARK            = 143480,
    SPELL_BACKSTAB                  = 143481,
    SPELL_STEALTH                   = 118969,

    // Kor'kron Warshaman
    SPELL_EARTH_SHIELD              = 143475,
    SPELL_EARTH_SHIELD_HEAL         = 143479,
    SPELL_EMPOWERED_CHAIN_HEAL      = 143473,
    SPELL_HEALING_TIDE_TOTEM        = 143474,
    SPELL_HEALING_TIDE              = 143477,
    SPELL_HEALING_TIDE_HEAL         = 143478,

};

enum Adds
{
    NPC_KORKRON_BANNER              = 71626,
    NPC_GENERAL_NAZGRIM_AFTERSHOCK  = 71697,
    NPC_RAVAGER                     = 71762,

    NPC_ORGRIMMAR_FAITHFUL          = 71715,
    NPC_KORKRON_IRONBLADE           = 71516,
    NPC_KORKRON_ARCWEAVER           = 71517,
    NPC_KORKRON_ASSASSIN            = 71518,
    NPC_KORKRON_WARSHAMAN           = 71519,
    NPC_HEALING_TIDE_TOTEM          = 71610,
    NPC_KORKRON_SNIPER              = 71656,
};

enum Events
{
    EVENT_SWITCH_STANCE     = 1,
    EVENT_SUNDERING_BLOW,
    EVENT_BONECRACKER,
    EVENT_RAGE_SPELL,
    EVENT_BERSERK,
    EVENT_SUMMON_ADDS,

    // Kor'kron Warshaman
    EVENT_IRONSTORM,

    // Kor'kron Arcweaver
    EVENT_ARCANE_SHOCK,
    EVENT_MAGICSTRIKE,
    EVENT_UNSTABLE_BLINK,

    // Kor'kron Assassin
    EVENT_BACKSTAB,
    EVENT_ASSASSINS_MARK,

    // Kor'kron Warshaman
    EVENT_EARTH_SHIELD,
    EVENT_EMPOWERED_CHAIN_HEAL,
    EVENT_HEALING_TIDE_TOTEM,
};

enum Datas
{
    DATA_ADD_POSITION   = 1,
};

enum Actions
{

};

enum DisplayIds
{
    DISPLAY_ITEM_RAVAGER = 7717,
};

enum Timers
{
    TIMER_BERSERK               = 10 * MINUTE * IN_MILLISECONDS,

    TIMER_RAGE_SPELL            = 1 * IN_MILLISECONDS,

    TIMER_SWITCH_STANCE_FIRST   = 1 * IN_MILLISECONDS,
    TIMER_SWITCH_STANCE         = 60 * IN_MILLISECONDS,

    TIMER_SUNDERING_BLOW_FIRST  = 8 * IN_MILLISECONDS,
    TIMER_SUNDERING_BLOW        = 8 * IN_MILLISECONDS,

    TIMER_BONECRACKER_FIRST     = 20 * IN_MILLISECONDS,
    TIMER_BONECRACKER           = 30 * IN_MILLISECONDS,

    TIMER_SUMMON_ADDS_FIRST     = 45 * IN_MILLISECONDS,
    TIMER_SUMMON_ADDS           = 45 * IN_MILLISECONDS,

    // Kor'kron Ironblade
    TIMER_IRONSTORM_FIRST_MIN   = 6 * IN_MILLISECONDS,
    TIMER_IRONSTORM_FIRST_MAX   = 10 * IN_MILLISECONDS,

    // Kor'kron Arcweaver
    TIMER_ARCANE_SHOCK_FIRST    = 1 * IN_MILLISECONDS,
    TIMER_ARCANE_SHOCK          = 1500,
    TIMER_MAGICSTRIKE_FIRST_MIN = 5 * IN_MILLISECONDS,
    TIMER_MAGICSTRIKE_FIRST_MAX = 5 * IN_MILLISECONDS,
    TIMER_MAGICSTRIKE_MIN       = 15 * IN_MILLISECONDS,
    TIMER_MAGICSTRIKE_MAX       = 20 * IN_MILLISECONDS,
    TIMER_UNSTABLE_BLINK_MIN    = 20 * IN_MILLISECONDS,
    TIMER_UNSTABLE_BLINK_MAX    = 25 * IN_MILLISECONDS,

    // Kor'kron Assassin
    TIMER_BACKSTAB              = 2 * IN_MILLISECONDS,

    // Kor'kron Warshaman
    TIMER_EARTH_SHIELD_FIRST    = 5 * IN_MILLISECONDS,
    TIMER_EARTH_SHIELD          = 40 * IN_MILLISECONDS,
    TIMER_EMPOWERED_CHAIN_HEAL  = 15 * IN_MILLISECONDS,
    TIMER_HEALING_TIDE_TOTEM    = 20 * IN_MILLISECONDS,
};

enum MovementPoints : int
{
    POINT_ADDS_POSITION = 1,
};

enum Stances : int
{
    STANCE_NONE         = 0,
    BATTLE_STANCE       = 1,
    BERSERKER_STANCE    = 2,
    DEFENSIVE_STANCE    = 3,
};

#define ADDS_COUNT 2
#define ADD_POSITIONS_COUNT 5

const Position addSpawnPos[ADD_POSITIONS_COUNT][2] = 
{
    {
        {1584.02f, -4591.88f, -66.80f, 5.84f},
        {1588.61f, -4594.49f, -66.73f, 5.78f}
    },
    {
        {1591.88f, -4584.03f, -66.81f, 5.22f},
        {1594.81f, -4590.88f, -66.73f, 5.02f}
    },
    {
        {1559.00f, -4621.40f, -66.92f, 5.76f},
        {1564.27f, -4624.34f, -66.93f, 5.77f}
    },
    {
        {1558.65f, -4634.91f, -67.05f, 0.50f},
        {1563.88f, -4632.72f, -67.05f, 0.37f}
    },
    {
        {1568.02f, -4647.59f, -66.84f, 0.78f},
        {1572.80f, -4642.79f, -66.70f, 0.78f}
    }
};

class boss_general_nazgrim : public CreatureScript
{
    public:

        boss_general_nazgrim() : CreatureScript("boss_general_nazgrim") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_general_nazgrimAI(creature);
        }

        struct boss_general_nazgrimAI : public BossAI
        {
            boss_general_nazgrimAI(Creature* creature) : BossAI(creature, DATA_GENERAL_NAZGRIM),
                m_IsIntroDone(false), m_SceneHelper(creature->GetGUID(), creature->GetMap())
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                SetEquipmentSlots(false, DISPLAY_ITEM_RAVAGER);
                
            }

            void Reset()
            {
                _Reset();

                InitPowers();

                me->SetReactState(REACT_AGGRESSIVE);

                m_CurrentStance = STANCE_NONE;
                m_LastHeroicShockwavePosition = me->GetHomePosition();
            }

            void EnterCombat(Unit* who)
            {
                TalkAggro();

                events.ScheduleEvent(EVENT_BERSERK, TIMER_BERSERK);
                events.ScheduleEvent(EVENT_SWITCH_STANCE, TIMER_SWITCH_STANCE_FIRST);
                events.ScheduleEvent(EVENT_SUNDERING_BLOW, TIMER_SUNDERING_BLOW_FIRST);
                events.ScheduleEvent(EVENT_BONECRACKER, TIMER_BONECRACKER_FIRST);
                events.ScheduleEvent(EVENT_RAGE_SPELL, TIMER_RAGE_SPELL);
                events.ScheduleEvent(EVENT_SUMMON_ADDS, TIMER_SUMMON_ADDS_FIRST);

                DoZoneInCombat();
                instance->SetBossState(DATA_GENERAL_NAZGRIM, IN_PROGRESS);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!m_IsIntroDone && who->IsPlayer() && me->GetDistance(who) <= 80.0f)
                {
                    m_IsIntroDone = true;
                    
                    TalkIntro();
                }
            }

            void JustDied(Unit* who)
            {
                _JustDied();
            }

            void DoAction(const int32 action) override
            {
                if (action == ACTION_IRON_JUGGERNAUT_DEAD)
                {
                    m_SceneHelper.AddSceneActionTalk(SAY_IRON_JUGGERNAUT_DEATH, 4000);
                    m_SceneHelper.Activate();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                m_SceneHelper.Update(diff);

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    ExecuteEvent(eventId);
                }

                DoMeleeAttackIfReady();
            }

            void ExecuteEvent(const uint32 eventId) override
            {
                switch (eventId)
                {
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK);
                        break;
                    case EVENT_SWITCH_STANCE:
                        SwitchStance();
                        events.ScheduleEvent(EVENT_SWITCH_STANCE, TIMER_SWITCH_STANCE);
                        break;
                    case EVENT_SUNDERING_BLOW:
                        // set 'triggered', because spell is applied on next swing
                        DoCastVictim(SPELL_SUNDERING_BLOW, true);
                        events.ScheduleEvent(EVENT_SUNDERING_BLOW, TIMER_SUNDERING_BLOW);
                        break;
                    case EVENT_BONECRACKER:
                        DoCastAOE(SPELL_BONECRACKER);
                        events.ScheduleEvent(EVENT_BONECRACKER, TIMER_BONECRACKER);
                        break;
                    case EVENT_RAGE_SPELL:
                        DoRageSpell();
                        events.ScheduleEvent(EVENT_RAGE_SPELL, TIMER_RAGE_SPELL);
                        break;
                    case EVENT_SUMMON_ADDS:
                        SpawnAdds();
                        events.ScheduleEvent(EVENT_SUMMON_ADDS, TIMER_SUMMON_ADDS);
                        break;
                }
            }

            Position GetLastHeroicShockwavePosition()
            {
                return m_LastHeroicShockwavePosition;
            }

        private:

            void TalkIntro()
            {
                uint32 team = instance->GetData(DATA_TEAM_IN_INSTANCE);

                if (team == ALLIANCE)
                {
                    Talk(SAY_INTRO_ALLIANCE);
                }
                else if (team == HORDE)
                {
                    Talk(SAY_INTRO_HORDE);
                }
            }

            void TalkAggro()
            {
                uint32 team = instance->GetData(DATA_TEAM_IN_INSTANCE);
                if (team == ALLIANCE)
                {
                    Talk(SAY_AGGRO_ALLIANCE);
                }
                else if (team == HORDE)
                {
                    Talk(SAY_AGGRO_HORDE);
                }
            }

            void InitPowers()
            {
                me->AddAura(SPELL_ZERO_MANA_REGEN, me);

                me->setPowerType(NAZGRIM_POWER_TYPE);
                me->SetMaxPower(NAZGRIM_POWER_TYPE, NAZGRIM_POWER_MAX);
                me->SetPower(NAZGRIM_POWER_TYPE, 0);
            }

            void SwitchStance()
            {
                uint32 currentStanceSpellId = 0;
                uint32 nextStanceSpellId = 0;
                Stances nextStance = STANCE_NONE;

                switch (m_CurrentStance)
                {
                    case BATTLE_STANCE:
                        currentStanceSpellId = SPELL_BATTLE_STANCE;
                        nextStanceSpellId = SPELL_BERSERKER_STANCE;
                        nextStance = BERSERKER_STANCE;
                        break;
                    case BERSERKER_STANCE:
                        currentStanceSpellId = SPELL_BERSERKER_STANCE;
                        nextStanceSpellId = SPELL_DEFENSIVE_STANCE;
                        nextStance = DEFENSIVE_STANCE;
                        break;
                    case DEFENSIVE_STANCE:
                        currentStanceSpellId = SPELL_DEFENSIVE_STANCE;
                        nextStanceSpellId = SPELL_BATTLE_STANCE;
                        nextStance = BATTLE_STANCE;
                        break;
                    default:
                        currentStanceSpellId = 0;
                        nextStanceSpellId = SPELL_BATTLE_STANCE;
                        nextStance = BATTLE_STANCE;
                        break;
                }

                if (currentStanceSpellId)
                {
                    me->RemoveAura(currentStanceSpellId);
                }

                if (nextStanceSpellId)
                {
                    me->CastSpell(me, nextStanceSpellId, true);
                }

                m_CurrentStance = nextStance;
            }

            void DoRageSpell()
            {
                if (me->HasAura(SPELL_COOLING_OFF))
                    return;

                uint32 rageSpellId = 0;
                Unit* target = NULL;

                int32 currentPower = me->GetPower(NAZGRIM_POWER_TYPE);

                if (currentPower >= 100)
                {
                    rageSpellId = SPELL_RAVAGER;
                    target = me->getVictim();
                }
                else if (currentPower >= 70)
                {
                    rageSpellId = SPELL_WAR_SONG;
                    target = me;
                }
                else if (currentPower >= 50)
                {
                    rageSpellId = SPELL_KORKRON_BANNER;
                    target = me;
                }
                else if (currentPower >= 30)
                {
                    rageSpellId = SPELL_HEROIC_SHOCKWAVE;
                    target = SelectTarget(SELECT_TARGET_RANDOM, 0, -20.0f, true);
                    if (!target)
                        target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                    if (target)
                    {
                        target->GetPosition(&m_LastHeroicShockwavePosition);
                    }
                }

                if (rageSpellId && target)
                {
                    DoCast(target, rageSpellId);
                }
            }

            void SpawnAdds()
            {
                Talk(SAY_ADDS);

                std::list<uint32> addsEntries;
                addsEntries.push_back(NPC_KORKRON_IRONBLADE);
                addsEntries.push_back(NPC_KORKRON_ASSASSIN);
                addsEntries.push_back(NPC_KORKRON_ARCWEAVER);
                addsEntries.push_back(NPC_KORKRON_WARSHAMAN);
                JadeCore::Containers::RandomResizeList(addsEntries, ADDS_COUNT);

                std::list<uint32> addsPositions;
                addsPositions.push_back(0);
                addsPositions.push_back(1);
                addsPositions.push_back(2);
                addsPositions.push_back(3);
                addsPositions.push_back(4);
                JadeCore::Containers::RandomResizeList(addsPositions, ADDS_COUNT);

                for (auto creatureEntry : addsEntries)
                {
                    if (addsPositions.empty())
                        break;

                    uint32 positionIndex = addsPositions.front();
                    addsPositions.pop_front();

                    if (Creature* pCreature = me->SummonCreature(creatureEntry, addSpawnPos[positionIndex][0]))
                    {
                        pCreature->GetMotionMaster()->MovePoint(POINT_ADDS_POSITION, addSpawnPos[positionIndex][1]);
                    }
                }
            }

        private:

            bool m_IsIntroDone;
            Stances m_CurrentStance;
            Position m_LastHeroicShockwavePosition;
            SceneHelper m_SceneHelper;

        };
};

struct npc_nazgrim_korkron_addAI : public ScriptedAI
{
    public:

        npc_nazgrim_korkron_addAI(Creature* creature) : ScriptedAI(creature)
        {
            ApplyAllImmunities(true);

            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, false);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, false);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, false);

            me->setActive(true);

            me->SetReactState(REACT_PASSIVE);
        }

        void Reset()
        {
            events.Reset();
            summons.DespawnAll();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE && id == POINT_ADDS_POSITION)
            {
                me->GetMotionMaster()->MovementExpired(false);

                DoAggro();
            }
        }

        void JustDied(Unit* who)
        {
            events.Reset();
            summons.DespawnAll();

            me->DespawnOrUnsummon(3000);
        }

    protected:

        virtual void DoAggro() { }
};

class npc_general_nazgrim_korkron_ironblade : public CreatureScript
{
    public:
        npc_general_nazgrim_korkron_ironblade() : CreatureScript("npc_general_nazgrim_korkron_ironblade") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_general_nazgrim_korkron_ironbladeAI(creature);
        }

        struct npc_general_nazgrim_korkron_ironbladeAI : public npc_nazgrim_korkron_addAI
        {
            npc_general_nazgrim_korkron_ironbladeAI(Creature* creature) : npc_nazgrim_korkron_addAI(creature)
            {
            }

            void Reset()
            {
                npc_nazgrim_korkron_addAI::Reset();

                m_IsLastStandDone = false;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (CheckForLastStand())
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_IRONSTORM:
                            DoCast(me, SPELL_IRONSTORM);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        protected:

            virtual void DoAggro() override
            {
                me->SetReactState(REACT_AGGRESSIVE);

                events.ScheduleEvent(EVENT_IRONSTORM, urand(TIMER_IRONSTORM_FIRST_MIN, TIMER_IRONSTORM_FIRST_MAX));
            }

        private:

            bool CheckForLastStand()
            {
                if (!m_IsLastStandDone && me->GetHealthPct() <= 50.0f)
                {
                    m_IsLastStandDone = true;

                    DoCast(me, SPELL_LAST_STAND);

                    return true;
                }

                return false;
            }

        private:

            bool m_IsLastStandDone;

        };
};

class npc_general_nazgrim_korkron_arcweaver : public CreatureScript
{
    public:
        npc_general_nazgrim_korkron_arcweaver() : CreatureScript("npc_general_nazgrim_korkron_arcweaver") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_general_nazgrim_korkron_arcweaverAI(creature);
        }

        struct npc_general_nazgrim_korkron_arcweaverAI : public npc_nazgrim_korkron_addAI
        {
            npc_general_nazgrim_korkron_arcweaverAI(Creature* creature) : npc_nazgrim_korkron_addAI(creature)
            {
            }

            void Reset()
            {
                npc_nazgrim_korkron_addAI::Reset();
            }

            void AttackStart(Unit* who)
            {
                if (!who)
                    return;

                if (me->Attack(who, true))
                    DoStartNoMovement(who);
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
                        case EVENT_ARCANE_SHOCK:
                            DoCastVictim(SPELL_ARCANE_SHOCK);
                            events.ScheduleEvent(EVENT_ARCANE_SHOCK, TIMER_ARCANE_SHOCK);
                            break;
                        case EVENT_MAGICSTRIKE:
                            DoCastAOE(SPELL_MAGICSTRIKE);
                            events.ScheduleEvent(EVENT_MAGICSTRIKE, urand(TIMER_MAGICSTRIKE_MIN, TIMER_MAGICSTRIKE_MAX));
                            break;
                        case EVENT_UNSTABLE_BLINK:
                            DoCast(SPELL_UNSTABLE_BLINK);
                            events.ScheduleEvent(EVENT_UNSTABLE_BLINK, urand(TIMER_UNSTABLE_BLINK_MIN, TIMER_UNSTABLE_BLINK_MAX));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

         protected:

            virtual void DoAggro() override
            {
                me->SetReactState(REACT_AGGRESSIVE);

                events.ScheduleEvent(EVENT_ARCANE_SHOCK, TIMER_ARCANE_SHOCK_FIRST);
                events.ScheduleEvent(EVENT_MAGICSTRIKE, urand(TIMER_MAGICSTRIKE_FIRST_MIN, TIMER_MAGICSTRIKE_FIRST_MAX));
                // when npc blinks it can teleport into textures
                // don't use this spell now
                // events.ScheduleEvent(EVENT_UNSTABLE_BLINK, urand(TIMER_UNSTABLE_BLINK_MIN, TIMER_UNSTABLE_BLINK_MAX));
            }

        };
};

class npc_general_nazgrim_korkron_assassin : public CreatureScript
{
    public:
        npc_general_nazgrim_korkron_assassin() : CreatureScript("npc_general_nazgrim_korkron_assassin") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_general_nazgrim_korkron_assassinAI(creature);
        }

        struct npc_general_nazgrim_korkron_assassinAI : public npc_nazgrim_korkron_addAI
        {
            npc_general_nazgrim_korkron_assassinAI(Creature* creature) : npc_nazgrim_korkron_addAI(creature)
            {
            }

            void Reset()
            {
                npc_nazgrim_korkron_addAI::Reset();

                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
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
                        case EVENT_BACKSTAB:
                            if (Unit* target = me->getVictim())
                            {
                                if (target->isInBack(me))
                                {
                                    DoCast(target, SPELL_BACKSTAB);
                                }
                            }
                            events.ScheduleEvent(EVENT_BACKSTAB, TIMER_BACKSTAB);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        protected:

            virtual void DoAggro() override
            {
                me->SetReactState(REACT_AGGRESSIVE);

                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                {
                    me->AddAura(SPELL_ASSASSINS_MARK, target);
                    me->AddAura(SPELL_STEALTH, me);
                    me->AddThreat(target, 1000000.0f);
                    AttackStart(target);
                }

                events.ScheduleEvent(EVENT_BACKSTAB, TIMER_BACKSTAB);
            }

        };
};

class npc_general_nazgrim_korkron_warshaman : public CreatureScript
{
    public:
        npc_general_nazgrim_korkron_warshaman() : CreatureScript("npc_general_nazgrim_korkron_warshaman") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_general_nazgrim_korkron_warshamanAI(creature);
        }

        struct npc_general_nazgrim_korkron_warshamanAI : public npc_nazgrim_korkron_addAI
        {
            npc_general_nazgrim_korkron_warshamanAI(Creature* creature) : npc_nazgrim_korkron_addAI(creature)
            {
            }

            void Reset()
            {
                npc_nazgrim_korkron_addAI::Reset();
            }

            void EnterCombat(Unit* who)
            {
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
                        case EVENT_EARTH_SHIELD:
                            if (Unit* target = me->SelectNearbyAlly(me, 20.0f))
                            {
                                DoCast(target, SPELL_EARTH_SHIELD);
                            }
                            events.ScheduleEvent(EVENT_EARTH_SHIELD, TIMER_EARTH_SHIELD);
                            break;
                        case EVENT_EMPOWERED_CHAIN_HEAL:
                            DoCast(me, SPELL_EMPOWERED_CHAIN_HEAL);
                            events.ScheduleEvent(EVENT_EMPOWERED_CHAIN_HEAL, TIMER_EMPOWERED_CHAIN_HEAL);
                            break;
                        case EVENT_HEALING_TIDE_TOTEM:
                            DoCast(me, SPELL_HEALING_TIDE_TOTEM);
                            events.ScheduleEvent(EVENT_HEALING_TIDE_TOTEM, TIMER_HEALING_TIDE_TOTEM);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        protected:

            virtual void DoAggro() override
            {
                me->SetReactState(REACT_AGGRESSIVE);

                events.ScheduleEvent(EVENT_EARTH_SHIELD, TIMER_EARTH_SHIELD_FIRST);
                events.ScheduleEvent(EVENT_EMPOWERED_CHAIN_HEAL, TIMER_EMPOWERED_CHAIN_HEAL);
                events.ScheduleEvent(EVENT_HEALING_TIDE_TOTEM, TIMER_HEALING_TIDE_TOTEM);
            }

        };
};

class npc_general_nazgrim_healing_tide_totem : public CreatureScript
{
    public:
        npc_general_nazgrim_healing_tide_totem() : CreatureScript("npc_general_nazgrim_healing_tide_totem") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_general_nazgrim_healing_tide_totemAI(creature);
        }

        struct npc_general_nazgrim_healing_tide_totemAI : public Scripted_NoMovementAI
        {
            npc_general_nazgrim_healing_tide_totemAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);

                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                me->AddAura(SPELL_HEALING_TIDE, me);
            }

            void UpdateAI(const uint32 diff) override
            {

            }

        private:

            InstanceScript* pInstance;
        };
};

class npc_general_nazgrim_aftershock : public CreatureScript
{
    public:
        npc_general_nazgrim_aftershock() : CreatureScript("npc_general_nazgrim_aftershock") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_general_nazgrim_aftershockAI(creature);
        }

        struct npc_general_nazgrim_aftershockAI : public Scripted_NoMovementAI
        {
            npc_general_nazgrim_aftershockAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit* owner)
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (Creature* pNazgrim = pInstance->instance->GetCreature(pInstance->GetData64(DATA_GENERAL_NAZGRIM)))
                    {
                        if (boss_general_nazgrim::boss_general_nazgrimAI* nazgrimAI = CAST_AI(boss_general_nazgrim::boss_general_nazgrimAI, pNazgrim->GetAI()))
                        {
                            Position pos = nazgrimAI->GetLastHeroicShockwavePosition();

                            me->SetOrientation(me->GetAngle(&pos));
                            me->SetFacingTo(me->GetAngle(&pos));

                            DoCastAOE(SPELL_AFTERSHOCK);
                        }
                    }
                }
            }
        };
};

class npc_general_nazgrim_ravager : public CreatureScript
{
    public:
        npc_general_nazgrim_ravager() : CreatureScript("npc_general_nazgrim_ravager") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_general_nazgrim_ravagerAI(creature);
        }

        struct npc_general_nazgrim_ravagerAI : public ScriptedAI
        {
            npc_general_nazgrim_ravagerAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                me->AddAura(SPELL_RAVAGER_AURA, me);
            }
        };
};

class spell_general_nazgrim_sundering_blow : public SpellScriptLoader
{
    public:
        spell_general_nazgrim_sundering_blow() : SpellScriptLoader("spell_general_nazgrim_sundering_blow") { }

        class spell_general_nazgrim_sundering_blow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_general_nazgrim_sundering_blow_SpellScript);

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                PreventHitDefaultEffect(effIndex);

                uint8 stacks = 1;

                if (Unit* victim = GetCaster()->getVictim())
                {
                    if (auto aura = victim->GetAura(SPELL_SUNDERING_BLOW))
                    {
                        stacks = aura->GetStackAmount();
                    }
                }

                GetCaster()->EnergizeBySpell(GetCaster(), GetSpellInfo()->Id, 5 * stacks, NAZGRIM_POWER_TYPE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_general_nazgrim_sundering_blow_SpellScript::HandleHitTarget, EFFECT_2, SPELL_EFFECT_ENERGIZE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_general_nazgrim_sundering_blow_SpellScript();
        }
};

class spell_general_nazgrim_heroic_shockwave_force : public SpellScriptLoader
{
    public:
        spell_general_nazgrim_heroic_shockwave_force() : SpellScriptLoader("spell_general_nazgrim_heroic_shockwave_force") { }

        class spell_general_nazgrim_heroic_shockwave_force_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_general_nazgrim_heroic_shockwave_force_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (Creature* pNazgrim = GetCaster()->ToCreature())
                {
                    std::list<Unit*> rangeTargets;

                    pNazgrim->AI()->SelectTargetList(rangeTargets, 2, SELECT_TARGET_RANDOM, -15.0f, true);
                    
                    if (rangeTargets.size() < 2)
                    {
                        pNazgrim->AI()->SelectTargetList(rangeTargets, 2, SELECT_TARGET_RANDOM, 0.0f, true);
                    }

                    targets.clear();

                    for (auto target : rangeTargets)
                        targets.push_back(target);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_general_nazgrim_heroic_shockwave_force_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_general_nazgrim_heroic_shockwave_force_SpellScript();
        }
};

class spell_general_nazgrim_aftershock : public SpellScriptLoader
{
    public:
        spell_general_nazgrim_aftershock() : SpellScriptLoader("spell_general_nazgrim_aftershock") { }

        class spell_general_nazgrim_aftershock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_general_nazgrim_aftershock_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                Position endPos;
                GetCaster()->GetPosition(&endPos);
                GetCaster()->GetNearPoint2D(endPos.m_positionX, endPos.m_positionY, endPos.m_positionZ, 0.0f);

                for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
                {
                    if (!CheckForTarget((*itr), GetCaster(), 3.0f))
                    {
                        itr = targets.erase(itr);
                    }
                    else
                    {
                        ++itr;
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_general_nazgrim_aftershock_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_130);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_general_nazgrim_aftershock_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_130);
            }

        private:

            bool CheckForTarget(const WorldObject* target, const Position* sourcePosition, float lineSize) const
            {
                if (!sourcePosition->HasInArc(M_PI, target))
                    return false;

                float dist = sourcePosition->GetExactDist2d(target);
                if (dist >= 60.0f)
                    return false;

                float angle = sourcePosition->GetRelativeAngle(target);

                return fabs(sin(angle)) * sourcePosition->GetExactDist2d(target->GetPositionX(), target->GetPositionY()) < lineSize;
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_general_nazgrim_aftershock_SpellScript();
        }
};

void AddSC_boss_general_nazgrim()
{
    new boss_general_nazgrim();                         // 71515

    new npc_general_nazgrim_korkron_ironblade();        // 71516
    new npc_general_nazgrim_korkron_arcweaver();        // 71517
    new npc_general_nazgrim_korkron_assassin();         // 71518
    new npc_general_nazgrim_korkron_warshaman();        // 71519
    new npc_general_nazgrim_healing_tide_totem();       // 71610
    new npc_general_nazgrim_aftershock();               // 71697
    new npc_general_nazgrim_ravager();                  // 71762

    new spell_general_nazgrim_sundering_blow();         // 143494
    new spell_general_nazgrim_heroic_shockwave_force(); // 143713
    new spell_general_nazgrim_aftershock();             // 143712

}
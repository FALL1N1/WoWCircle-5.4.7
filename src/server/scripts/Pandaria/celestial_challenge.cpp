
#include "ScriptPCH.h"

enum ScriptedTextsXuen
{
    SAY_XUEN_AGGRO  = 0,
    SAY_XUEN_DEATH  = 1,
    SAY_XUEN_INTRO  = 2,
    SAY_XUEN_KILL   = 3,
    SAY_XUEN_SPELL  = 4,
};

enum ScriptedTextsChiJi
{
    SAY_CHI_JI_AGGRO    = 0,
    SAY_CHI_JI_DEATH    = 1,
    SAY_CHI_JI_INTRO    = 2,
    SAY_CHI_JI_KILL     = 3,
    SAY_CHI_JI_SPELL_1  = 4,
    SAY_CHI_JI_SPELL_2  = 5,
};

enum SpellsXuen
{
    SPELL_LEAP                          = 144640,

    SPELL_SPECTRAL_SWIPE                = 144638,

    SPELL_CRACKLING_LIGHTNING_AOE       = 144634,
    SPELL_CRACKLING_LIGHTNING_PERIODIC  = 144635,
    SPELL_CRACKLING_LIGHTNING_DMG       = 144633,

    SPELL_CHI_BARRAGE                   = 144642,
    SPELL_CHI_BARRAGE_MISSILE           = 144643,
    SPELL_CHI_BARRAGE_DMG               = 144644,

    SPELL_AGILITY                       = 144631,
};

enum SpellsYulon
{
    SPELL_JADEFLAME_BUFFET              = 144630,

    SPELL_JADEFIRE_BREATH               = 144530,

    SPELL_JADEFIRE_BOLT_AOE             = 144545,
    SPELL_JADEFIRE_BOLT_MISSILE         = 144532,
    SPELL_JADEFIRE_BOLT_SUMMON          = 144541,

    SPELL_JADEFIRE_BLAZE_AURA           = 144537,
    SPELL_JADEFIRE_BLAZE_DMG            = 144538,

    SPELL_JADEFIRE_WALL_AURA            = 144533,
    SPELL_JADEFIRE_WALL_DMG             = 144539,
};

enum SpellsChiJi
{
    SPELL_INSPIRING_SONG                = 144468,

    SPELL_FIRESTORM_SUMMON              = 144461,
    SPELL_FIRESTORM_AURA                = 144463,
    SPELL_FIRESTORM_DMG                 = 144462,

    SPELL_BEACON_OF_HOPE_SUMMON         = 144473,
    SPELL_BEACON_OF_HOPE_AURA           = 144474,
    SPELL_BEACON_OF_HOPE_BUFF           = 144475,

    SPELL_BLAZING_SONG                  = 144471,
    SPELL_BLAZING_SONG_DMG              = 144472,

    SPELL_CRANE_RUSH                    = 144470,
    SPELL_CRANE_RUSH_SUMMON             = 144495,

    SPELL_BLAZING_NOVA                  = 144493,
    SPELL_BLAZING_NOVA_DMG              = 144494,
};

enum SpellsNiuzao
{
    SPELL_HEADBUTT                      = 144610,

    SPELL_OXEN_FORTITUDE                = 144606,
    SPELL_OXEN_FORTITUDE_BUFF           = 144607,

    SPELL_MASSIVE_QUAKE                 = 144611,
    SPELL_MASSIVE_QUAKE_DMG             = 144612,

    SPELL_CHARGE                        = 144608,
    SPELL_CHARGE_DMG                    = 144609,
};

enum Creatures
{
    // Xuen
    NPC_XUEN            = 71953,

    // Yu'lon
    NPC_YULON           = 71955,

    // Chi-Ji
    NPC_CHI_JI          = 71952,
    NPC_FIRESTORM       = 71971,
    NPC_BEACON_OF_HOPE  = 71978,
    NPC_CHILD_OF_CHI_JI = 71990,

    // Niuzao
    NPC_NIUZAO          = 71954,
};

enum Events
{
    // Xuen
    EVENT_LEAP  = 1,
    EVENT_CRACKLING_LIGHTNING,
    EVENT_CHI_BARRAGE,
    EVENT_AGILITY,

    // Chi-Ji
    EVENT_INSPIRING_SONG,
    EVENT_FIRESTORM,
    EVENT_BEACON_OF_HOPE,
    EVENT_BLAZING_SONG,
    EVENT_CRANE_RUSH,
};

#define ARENA_MIN_POSITION_X -771.0f
#define ARENA_MAX_POSITION_X -518.0f
#define ARENA_MIN_POSITION_Y -5115.0f
#define ARENA_MAX_POSITION_Y -4926.0f

bool IsTargetOutOfArena(WorldObject const* target)
{
    if (target->GetPositionX() < ARENA_MIN_POSITION_X || 
        target->GetPositionX() > ARENA_MAX_POSITION_X ||
        target->GetPositionY() < ARENA_MIN_POSITION_Y || 
        target->GetPositionY() > ARENA_MAX_POSITION_Y)
    {
        return true;
    }

    return false;
}

struct npc_celestial_challengeAI : public ScriptedAI
{
    npc_celestial_challengeAI(Creature* pCreature) : ScriptedAI(pCreature),
        m_IsChallengeCompleted(false)
    {
        ApplyAllImmunities(true);

        me->setActive(true);

        me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 6.0f);
        me->SetFloatValue(UNIT_FIELD_COMBATREACH, 6.0f);

        me->SetReactState(REACT_DEFENSIVE);

        me->SetFixedAttackDistance(230.0f);
    }

    void Reset()
    {
        events.Reset();
        summons.DespawnAll();

        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    }

    void JustSummoned(Creature* summon) override
    {
        summons.Summon(summon);
    }

    void SummonedCreatureDespawn(Creature* summon) override
    {
        summons.Despawn(summon);
    }

    void EnterEvadeMode()
    {
        if (IsChallengeCompleted())
            return;

        ScriptedAI::EnterEvadeMode();
    }

    void DamageTaken(Unit* attacker, uint32& damage)
    {
        if (IsChallengeCompleted())
        {
            damage = 0;
            return;
        }

        if (damage >= me->GetHealth())
        {
            m_IsChallengeCompleted = true;

            damage = me->GetHealth() - 1;

            me->AttackStop();
            me->CombatStop();
            me->InterruptNonMeleeSpells(false);

            events.Reset();
            summons.DespawnAll();
            me->RemoveAllAreasTrigger();

            me->setFaction(35);

            CompleteChallenge();
        }
    }

protected:

    bool IsOutOfArena() const
    {
        if (IsTargetOutOfArena(me))
            return true;

        return false;            
    }

    bool IsChallengeCompleted() const
    {
        return m_IsChallengeCompleted;
    }

    Unit* SelectRandomRangeTargetOrAny(const float rangeDistance)
    {
        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -rangeDistance, true);
        if (!target)
            target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
        
        return target;
    }

    virtual void CompleteChallenge() { }

private:

    bool m_IsChallengeCompleted;
};

class npc_celestial_challenge_xuen : public CreatureScript
{
    public:
        npc_celestial_challenge_xuen() : CreatureScript("npc_celestial_challenge_xuen") 
		{
		}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_celestial_challenge_xuenAI(creature);
        }

        struct npc_celestial_challenge_xuenAI : public npc_celestial_challengeAI
        {
            npc_celestial_challenge_xuenAI(Creature* pCreature) : npc_celestial_challengeAI(pCreature)
            {

            }

            void Reset()
            {
                npc_celestial_challengeAI::Reset();
            }

            void EnterCombat(Unit* who)
            {
                Talk(SAY_XUEN_AGGRO);

                events.ScheduleEvent(EVENT_LEAP, urand(15000, 20000));
                events.ScheduleEvent(EVENT_AGILITY, urand(25000, 40000));
                events.ScheduleEvent(EVENT_CRACKLING_LIGHTNING, 38000);
                events.ScheduleEvent(EVENT_CHI_BARRAGE, 20000);
            }

            void KilledUnit(Unit* who)
            {
                if (who && who->IsPlayer())
                {
                    Talk(SAY_XUEN_KILL);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (IsOutOfArena())
                {
                    EnterEvadeMode();
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    ExecuteEvent(eventId);
                }

                DoMeleeAttackIfReady();
            }

            void ExecuteEvent(const uint32 eventId)
            {
                switch (eventId)
                {
                    case EVENT_LEAP:
                        if (Unit* target = SelectRandomRangeTargetOrAny(15.0f))
                        {
                            DoCast(target, SPELL_LEAP);
                        }
                        events.ScheduleEvent(EVENT_LEAP, urand(25000, 35000));
                        break;
                    case EVENT_CRACKLING_LIGHTNING:
                        DoCast(me, SPELL_CRACKLING_LIGHTNING_PERIODIC);
                        events.ScheduleEvent(EVENT_CRACKLING_LIGHTNING, 47000);
                        break;
                    case EVENT_CHI_BARRAGE:
                        DoCastAOE(SPELL_CHI_BARRAGE);
                        events.ScheduleEvent(EVENT_CHI_BARRAGE, 20000);
                        break;
                    case EVENT_AGILITY:
                        Talk(SAY_XUEN_SPELL);
                        DoCast(me, SPELL_AGILITY);
                        break;
                }
            }

        protected:

            virtual void CompleteChallenge()
            {
                Talk(SAY_XUEN_DEATH);

                me->DespawnOrUnsummon(5000);
            }
        };
};

class npc_celestial_challenge_chi_ji : public CreatureScript
{
    public:
        npc_celestial_challenge_chi_ji() : CreatureScript("npc_celestial_challenge_chi_ji") 
		{
		}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_celestial_challenge_chi_jiAI(creature);
        }

        struct npc_celestial_challenge_chi_jiAI : public npc_celestial_challengeAI
        {
            npc_celestial_challenge_chi_jiAI(Creature* pCreature) : npc_celestial_challengeAI(pCreature),
                m_NextSpellBlazingSong(false), m_FirstCraneRushDone(false), m_SecondCraneRushDone(false)
            {
                // enable to interrupt Inspiring Song
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, false);
            }

            void Reset()
            {
                npc_celestial_challengeAI::Reset();

                m_NextSpellBlazingSong = false;
                m_FirstCraneRushDone = false;
                m_SecondCraneRushDone = false;
            }

            void EnterCombat(Unit* who)
            {
                Talk(SAY_CHI_JI_AGGRO);

                events.ScheduleEvent(EVENT_INSPIRING_SONG, 20000);
                events.ScheduleEvent(EVENT_FIRESTORM, urand(15000, 25000));
                events.ScheduleEvent(EVENT_BEACON_OF_HOPE, urand(30000, 40000));
            }

            void KilledUnit(Unit* who)
            {
                if (who && who->IsPlayer())
                {
                    Talk(SAY_CHI_JI_KILL);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (IsOutOfArena())
                {
                    EnterEvadeMode();
                    return;
                }

                events.Update(diff);

                if (CraneRush())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    ExecuteEvent(eventId);
                }

                DoMeleeAttackIfReady();
            }

            void ExecuteEvent(const uint32 eventId)
            {
                if (m_NextSpellBlazingSong && eventId != EVENT_BLAZING_SONG)
                {
                    events.RescheduleEvent(eventId, urand(1000, 3000));
                    return;
                }

                switch (eventId)
                {
                    case EVENT_INSPIRING_SONG:
                        DoCast(SPELL_INSPIRING_SONG);
                        events.ScheduleEvent(EVENT_INSPIRING_SONG, 30000);
                        break;
                    case EVENT_FIRESTORM:
                        if (Unit* target = SelectRandomRangeTargetOrAny(15.0f))
                        {
                            DoCast(target, SPELL_FIRESTORM_SUMMON);
                        }
                        events.ScheduleEvent(EVENT_FIRESTORM, urand(25000, 30000));
                        break;
                    case EVENT_BEACON_OF_HOPE:
                        Talk(SAY_CHI_JI_SPELL_1);
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                        {
                            DoCast(target, SPELL_BEACON_OF_HOPE_SUMMON);
                        }
                        m_NextSpellBlazingSong = true;
                        events.ScheduleEvent(EVENT_BEACON_OF_HOPE, 60000);
                        events.ScheduleEvent(EVENT_BLAZING_SONG, 1);
                        break;
                    case EVENT_BLAZING_SONG:
                        m_NextSpellBlazingSong = false;
                        DoCast(me, SPELL_BLAZING_SONG);
                        break;
                }
            }

        protected:

            virtual void CompleteChallenge()
            {
                Talk(SAY_CHI_JI_DEATH);

                me->DespawnOrUnsummon(5000);
            }

        private:

            bool CraneRush()
            {
                if (!m_FirstCraneRushDone && me->GetHealthPct() <= 66.0f)
                {
                    m_FirstCraneRushDone = true;

                    Talk(SAY_CHI_JI_SPELL_2);

                    DoCast(SPELL_CRANE_RUSH);
                    return true;
                }

                if (!m_SecondCraneRushDone && me->GetHealthPct() <= 33.0f)
                {
                    m_SecondCraneRushDone = true;

                    Talk(SAY_CHI_JI_SPELL_2);

                    DoCast(SPELL_CRANE_RUSH);
                    return true;
                }

                return false;
            }

        private:

            bool m_NextSpellBlazingSong;
            bool m_FirstCraneRushDone;
            bool m_SecondCraneRushDone;
        };
};

class npc_celestial_challenge_chi_ji_firestorm : public CreatureScript
{
    public:
        npc_celestial_challenge_chi_ji_firestorm() : CreatureScript("npc_celestial_challenge_chi_ji_firestorm") 
		{
		}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_celestial_challenge_chi_ji_firestormAI(creature);
        }

        struct npc_celestial_challenge_chi_ji_firestormAI : public ScriptedAI
        {
            npc_celestial_challenge_chi_ji_firestormAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                me->AddAura(SPELL_FIRESTORM_AURA, me);
            }

            void IsSummonedBy(Unit* owner)
            {
                me->GetMotionMaster()->MoveRandom(5.0f);
            }

        protected:

        };
};

class npc_celestial_challenge_chi_ji_becon_of_hope : public CreatureScript
{
    public:
        npc_celestial_challenge_chi_ji_becon_of_hope() : CreatureScript("npc_celestial_challenge_chi_ji_becon_of_hope") 
		{
		}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_celestial_challenge_chi_ji_becon_of_hopeAI(creature);
        }

        struct npc_celestial_challenge_chi_ji_becon_of_hopeAI : public Scripted_NoMovementAI
        {
            npc_celestial_challenge_chi_ji_becon_of_hopeAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                me->AddAura(SPELL_BEACON_OF_HOPE_AURA, me);
            }

            // prevents evade mode
            // it happens because of the beacon has 35 faction and enters combat at players' side
            // maybe need to set 16 faction
            void EnterEvadeMode() { }

        protected:

        };
};

class npc_celestial_challenge_chi_ji_child : public CreatureScript
{
    public:
        npc_celestial_challenge_chi_ji_child() : CreatureScript("npc_celestial_challenge_chi_ji_child") 
		{
		}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_celestial_challenge_chi_ji_childAI(creature);
        }

        struct npc_celestial_challenge_chi_ji_childAI : public ScriptedAI
        {
            npc_celestial_challenge_chi_ji_childAI(Creature* pCreature) : ScriptedAI(pCreature)
            {

            }

            void Reset()
            {
                me->AddAura(SPELL_BLAZING_NOVA, me);
            }

            bool CanAIAttack(Unit const* l_Target) const override
            {
                if (IsTargetOutOfArena(l_Target))
                    return false;

                return true;
            }

        protected:

        };
};

class TargetOutOfArenaCheck
{
    public:
        TargetOutOfArenaCheck() {}

        bool operator()(WorldObject* unit)
        {
            if (!unit)
                return true;

            if (IsTargetOutOfArena(unit))
            {
                return true;
            }

            return false;
        }

    private:
        Unit* _caster;
        Unit* _mainTarget;
};

class spell_celestial_challenge_xuen_crackling_lightning: public SpellScriptLoader
{
    public:
        spell_celestial_challenge_xuen_crackling_lightning() : SpellScriptLoader("spell_celestial_challenge_xuen_crackling_lightning") { }

        class spell_celestial_challenge_xuen_crackling_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_celestial_challenge_xuen_crackling_lightning_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    Unit* target = pCreature->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, -15.0f, true);
                    if (!target)
                        target = pCreature->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                    if (target)
                    {
                        targets.clear();
                        targets.push_back(target);
                    }
                }

                if (targets.size() > 1)
                {
                    JadeCore::Containers::RandomResizeList(targets, 1);
                }
            }

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_CRACKLING_LIGHTNING_DMG, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_celestial_challenge_xuen_crackling_lightning_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_celestial_challenge_xuen_crackling_lightning_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_celestial_challenge_xuen_crackling_lightning_SpellScript();
        }
};

class spell_celestial_challenge_xuen_chi_barrage: public SpellScriptLoader
{
    public:
        spell_celestial_challenge_xuen_chi_barrage() : SpellScriptLoader("spell_celestial_challenge_xuen_chi_barrage") { }

        class spell_celestial_challenge_xuen_chi_barrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_celestial_challenge_xuen_chi_barrage_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    Unit* target = pCreature->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, -15.0f, true);
                    if (!target)
                        target = pCreature->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                    if (target)
                    {
                        targets.clear();
                        targets.push_back(target);
                    }
                }

                if (targets.size() > 1)
                {
                    JadeCore::Containers::RandomResizeList(targets, 1);
                }
            }

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_CHI_BARRAGE_MISSILE, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_celestial_challenge_xuen_chi_barrage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_celestial_challenge_xuen_chi_barrage_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_celestial_challenge_xuen_chi_barrage_SpellScript();
        }
};

class spell_celestial_challenge_chi_ji_blazing_song: public SpellScriptLoader
{
    public:
        spell_celestial_challenge_chi_ji_blazing_song() : SpellScriptLoader("spell_celestial_challenge_chi_ji_blazing_song") { }

        class spell_celestial_challenge_chi_ji_blazing_song_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_celestial_challenge_chi_ji_blazing_song_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                TargetOutOfArenaCheck check;
                targets.remove_if(check);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_celestial_challenge_chi_ji_blazing_song_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_celestial_challenge_chi_ji_blazing_song_SpellScript();
        }
};

void AddSC_celestial_challenge()
{
    new npc_celestial_challenge_xuen();                         // 71953
    new npc_celestial_challenge_chi_ji();                       // 71952
    new npc_celestial_challenge_chi_ji_firestorm();             // 71971
    new npc_celestial_challenge_chi_ji_becon_of_hope();         // 71978
    new npc_celestial_challenge_chi_ji_child();                 // 71990

    new spell_celestial_challenge_xuen_crackling_lightning();   // 144634
    new spell_celestial_challenge_xuen_chi_barrage();           // 144642
    new spell_celestial_challenge_chi_ji_blazing_song();        // 144472
}
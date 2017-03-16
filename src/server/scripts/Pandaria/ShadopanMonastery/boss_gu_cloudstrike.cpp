//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "shadopan_monastery.hpp"

enum eSpells
{
    // Gu
    SPELL_KILL_GUARDIANS            = 114927,

    SPELL_INVOKE_LIGHTNING          = 106984,
    SPELL_CHARGING_SOUL             = 110945,
    
    SPELL_OVERCHARGED_SOUL          = 110852,
    SPELL_OVERCHARGED_SOUL_DAMAGE   = 111129,

    // Azure Serpent
    SPELL_LIGHTNING_SHIELD          = 123496,
    SPELL_STATIC_FIELD              = 106923,

    SPELL_LIGHTNING_BREATH          = 102573,
    SPELL_MAGNETIC_SHROUD           = 107140, // TODO
};

enum eEvents
{
    // Gu
    EVENT_INVOKE_LIGHTNING  = 1,
    EVENT_OVERCHARGED_SOUL  = 2,

    // Azure Serpent
    EVENT_STATIC_FIELD      = 3,
    EVENT_LIGHTNING_BREATH  = 4,
    EVENT_MAGNETIC_SHROUD   = 5,
};

enum eActions
{
    // Gu
    ACTION_INTRO                = 1,
    ACTION_GU_P_3               = 2,
    
    // Azure Serpent
    ACTION_AZURE_SERPENT_P_1    = 3,
    ACTION_AZURE_SERPENT_P_2    = 4,
    ACTION_AZURE_SERPENT_RESET  = 5,
};

enum ePhases
{
    PHASE_ONE   = 1,
    PHASE_TWO   = 2,
    PHASE_THREE = 3
};

class boss_gu_cloudstrike : public CreatureScript
{
    public:
        boss_gu_cloudstrike() : CreatureScript("boss_gu_cloudstrike") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Aggro,
            SerpentMove,
            SerpentCalled,
            SerpentAttack,
            Phase2,
            MagneticShroud,
            Serpent60,
            Serpent20,
            SerpentDeath,
            Phase3,
            Death
        };

        struct boss_gu_cloudstrikeAI : public BossAI
        {
            boss_gu_cloudstrikeAI(Creature* p_Creature) : BossAI(p_Creature, DataTypes::DATA_GU_CLOUDSTRIKE)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            InstanceScript* m_Instance;
            bool m_IntroDone;

            uint8 m_Phase;

            void Reset() override
            {
                _Reset();

                me->RemoveAurasDueToSpell(SPELL_CHARGING_SOUL);

                if (Creature* l_AzureSerpent = GetAzureSerpent())
                {
                    if (l_AzureSerpent->IsAIEnabled)
                        l_AzureSerpent->AI()->DoAction(ACTION_AZURE_SERPENT_RESET);
                }

                m_Phase = 1;
                events.ScheduleEvent(EVENT_INVOKE_LIGHTNING, urand(5000, 10000), PHASE_ONE);
            }

            Creature* GetAzureSerpent() const
            {
                return m_Instance == nullptr ? nullptr : m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_AZURE_SERPENT));
            }

            void JustReachedHome() override
            {
                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MAGNETIC_SHROUD);
                    m_Instance->SetBossState(DATA_GU_CLOUDSTRIKE, FAIL);
                }

                summons.DespawnAll();

                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                events.Reset();
            }

            void JustDied(Unit* p_Killer) override
            {
                Talk(eTalks::Death);

                _JustDied();

                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MAGNETIC_SHROUD);
                    m_Instance->SetBossState(DATA_GU_CLOUDSTRIKE, DONE);
                }

                summons.DespawnAll();
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage) override
            {
                if (m_Phase == 1 && me->HealthBelowPctDamaged(50, p_Damage))
                {
                    Talk(eTalks::Phase2);

                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        Talk(eTalks::SerpentAttack);
                    });

                    m_Phase = 2;
                    events.CancelEventGroup(PHASE_ONE);

                    events.ScheduleEvent(EVENT_OVERCHARGED_SOUL, 2500, PHASE_TWO);
                    
                    me->SetReactState(REACT_PASSIVE);
                    me->CastSpell(me, SPELL_CHARGING_SOUL, false);

                    if (Creature* l_AzureSerpent = GetAzureSerpent())
                    {
                        if (l_AzureSerpent->IsAIEnabled)
                            l_AzureSerpent->AI()->DoAction(ACTION_AZURE_SERPENT_P_2);
                    }
                }
            }

            void DoAction(const int32 p_Action) override
            {
                switch (p_Action)
                {
                    case ACTION_INTRO:
                    {
                        if (!m_IntroDone)
                        {
                            Talk(eTalks::Intro1);

                            AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                Talk(eTalks::Intro2);
                            });

                            me->CastSpell(me, SPELL_KILL_GUARDIANS);
                            m_IntroDone = true;
                        }

                        break;
                    }
                    case ACTION_GU_P_3:
                    {
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->CastSpell(me, SPELL_OVERCHARGED_SOUL, true);
                        me->RemoveAurasDueToSpell(SPELL_CHARGING_SOUL);

                        AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Phase3); });

                        break;
                    }
                    default:
                        break;
                }
            }

            void JustSummoned(Creature* p_Summoned) override
            {
                summons.Summon(p_Summoned);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalks::Aggro);

                AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::SerpentMove);
                });

                AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    Talk(eTalks::SerpentCalled);
                });

                if (Creature* l_AzureSerpent = GetAzureSerpent())
                {
                    if (l_AzureSerpent->IsAIEnabled)
                        l_AzureSerpent->AI()->DoAction(ACTION_AZURE_SERPENT_P_1);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_INVOKE_LIGHTNING:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            me->CastSpell(l_Target, SPELL_INVOKE_LIGHTNING, false);
                        events.ScheduleEvent(EVENT_INVOKE_LIGHTNING, urand(5000, 10000), PHASE_ONE);
                        break;
                    case EVENT_OVERCHARGED_SOUL:
                        me->CastSpell(me, SPELL_OVERCHARGED_SOUL_DAMAGE, false);
                        events.ScheduleEvent(EVENT_OVERCHARGED_SOUL, 2500, PHASE_TWO);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_gu_cloudstrikeAI(p_Creature);
        }
};

Position const g_AzureSerpentPos[4] =
{
    { 3835.01f, 2906.63f, 753.33f },
    { 3850.37f, 2738.14f, 814.84f },
    { 3758.79f, 2692.08f, 778.60f },
    { 3736.37f, 2680.89f, 778.60f }
};

class npc_azure_serpent : public CreatureScript
{
    public:
        npc_azure_serpent() :  CreatureScript("npc_azure_serpent") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Aggro,
            SerpentMove,
            SerpentCalled,
            SerpentAttack,
            Phase2,
            MagneticShroud,
            Serpent60,
            Serpent20,
            SerpentDeath,
            Phase3,
            Death
        };

        struct npc_azure_serpentAI : public ScriptedAI
        {
            npc_azure_serpentAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            uint8 m_Phase;

            uint32 m_MoveTimer;
            uint8 m_LastMoveID;

            bool m_60PercentWarned;
            bool m_20PercentWarned;

            void Reset() override
            {
                m_Phase = 0;

                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                m_Events.Reset();

                me->AddAura(SPELL_LIGHTNING_SHIELD, me);

                m_MoveTimer = 0;
                m_LastMoveID = 0;

                me->SetSpeed(MOVE_FLIGHT, 5.0f);

                m_60PercentWarned = false;
                m_20PercentWarned = false;
            }

            Creature* GetGu() const
            {
                return m_Instance == nullptr ? nullptr : m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_GU_CLOUDSTRIKE));
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (Creature* l_GU = GetGu())
                {
                    if (l_GU->IsAIEnabled)
                    {
                        l_GU->AI()->DoAction(ACTION_GU_P_3);
                        l_GU->AI()->Talk(eTalks::SerpentDeath);
                    }
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case ACTION_AZURE_SERPENT_P_1:
                        m_MoveTimer = 100;
                        break;
                    case ACTION_AZURE_SERPENT_P_2:
                        m_Events.CancelEventGroup(PHASE_ONE);
                        m_Events.ScheduleEvent(EVENT_MAGNETIC_SHROUD, urand(10000, 15000), PHASE_TWO);
                        m_Events.ScheduleEvent(EVENT_LIGHTNING_BREATH, urand (2500, 7500), PHASE_TWO);

                        m_LastMoveID = 3;
                        me->GetMotionMaster()->MovePoint(m_LastMoveID, g_AzureSerpentPos[m_LastMoveID]);
                        me->RemoveAurasDueToSpell(SPELL_LIGHTNING_SHIELD);

                        DoZoneInCombat();
                        break;
                    case ACTION_AZURE_SERPENT_RESET:
                        if (!me->isAlive())
                            me->Respawn();

                        me->CombatStop();
                        Reset();
                        me->NearTeleportTo(3829.19f, 2927.29f, 705.71f, 0.0f);
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                if (p_ID < 2)
                {
                    m_LastMoveID = p_ID + 1;
                    m_MoveTimer = 500;
                }
                else if (p_ID == 2)
                {
                    /// Movement Finished, stop move start event
                    m_Phase = 1;
                    m_Events.ScheduleEvent(EVENT_STATIC_FIELD, 10000, PHASE_ONE);
                    DoZoneInCombat();
                }
                else if (p_ID == 3)
                {
                    /// Movement Finished, stop move start event
                    m_MoveTimer = 200;  ///< This prevent the boss from being out the line of sight

                    me->SetReactState(REACT_AGGRESSIVE);
                    me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage) override
            {
                if (me->HealthBelowPctDamaged(20, p_Damage) && !m_20PercentWarned)
                {
                    m_20PercentWarned = true;

                    if (Creature const* l_Gu = GetGu())
                    {
                        if (l_Gu->IsAIEnabled)
                            l_Gu->AI()->Talk(eTalks::Serpent20);
                    }
                }
                else if (me->HealthBelowPctDamaged(60, p_Damage) && !m_60PercentWarned)
                {
                    m_60PercentWarned = true;

                    if (Creature const* l_Gu = GetGu())
                    {
                        if (l_Gu->IsAIEnabled)
                            l_Gu->AI()->Talk(eTalks::Serpent60);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_MoveTimer != 0)
                {
                    if (m_MoveTimer <= p_Diff)
                    {
                        me->GetMotionMaster()->MovePoint(m_LastMoveID, g_AzureSerpentPos[m_LastMoveID]);
                        m_MoveTimer = 0;
                    }
                    else
                        m_MoveTimer -= p_Diff;
                }

                if (!m_Phase)
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_STATIC_FIELD:
                    {
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            me->CastSpell(l_Target, SPELL_STATIC_FIELD, false);
                        m_Events.ScheduleEvent(EVENT_STATIC_FIELD, 10000, PHASE_ONE);
                        break;
                    }
                    case EVENT_MAGNETIC_SHROUD:
                    {
                        if (Creature const* l_Gu = GetGu())
                        {
                            if (l_Gu->IsAIEnabled)
                                l_Gu->AI()->Talk(eTalks::MagneticShroud);
                        }

                        me->CastSpell(me, SPELL_MAGNETIC_SHROUD, false);
                        m_Events.ScheduleEvent(EVENT_MAGNETIC_SHROUD, urand(10000, 15000), PHASE_TWO);
                        break;
                    }
                    case EVENT_LIGHTNING_BREATH:
                    {
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 0, true))
                            me->CastSpell(l_Target, SPELL_LIGHTNING_BREATH, false);
                        m_Events.ScheduleEvent(EVENT_LIGHTNING_BREATH, urand(7500, 12500), PHASE_TWO);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_azure_serpentAI(creature);
        }
};

class AreaTrigger_at_gu_intro : public AreaTriggerScript
{
    public:
        AreaTrigger_at_gu_intro() : AreaTriggerScript("at_gu_intro") {}

        bool OnTrigger(Player* p_Player, AreaTriggerEntry const* /*p_Trigger*/, bool p_Enter) override
        {
            if (InstanceScript* l_InstanceScript = p_Player->GetInstanceScript())
            {
                if (Creature* l_Gu = l_InstanceScript->instance->GetCreature(l_InstanceScript->GetData64(NPC_GU_CLOUDSTRIKE)))
                {
                    if (l_Gu->IsAIEnabled)
                        l_Gu->AI()->DoAction(ACTION_INTRO);
                }
            }

            return false;
        }
};

class OnlyGuardianPredicate
{
    public:
        OnlyGuardianPredicate() {}

        bool operator()(WorldObject* p_Target)
        {
            return p_Target->GetEntry() != NPC_GUARDIAN;
        }
};

class spell_kill_guardians : public SpellScriptLoader
{
    public:
        spell_kill_guardians() : SpellScriptLoader("spell_kill_guardians") { }

        class spell_kill_guardians_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_kill_guardians_SpellScript);

            void SelectTarget(std::list<WorldObject*>& p_Targets)
            {
                p_Targets.remove_if(OnlyGuardianPredicate());
                m_Targets = p_Targets;
            }

            void KillTarget(std::list<WorldObject*>& p_Targets)
            {
                p_Targets = m_Targets;

                for (WorldObject* l_Iter : p_Targets)
                {
                    if (Creature* l_Target = l_Iter->ToCreature())
                        l_Target->DespawnOrUnsummon(2000);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_kill_guardians_SpellScript::SelectTarget, EFFECT_0, TARGET_SRC_CASTER);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_kill_guardians_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_kill_guardians_SpellScript::KillTarget, EFFECT_1, TARGET_SRC_CASTER);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_kill_guardians_SpellScript::KillTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            }

            std::list<WorldObject*> m_Targets;
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_kill_guardians_SpellScript();
        }
};

class spell_overcharged_soul_damage : public SpellScriptLoader
{
    public:
        spell_overcharged_soul_damage() : SpellScriptLoader("spell_overcharged_soul_damage") { }

        class spell_overcharged_soul_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_overcharged_soul_damage_SpellScript);

            void ChangeDamage(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                    SetHitDamage(25000 / l_Caster->GetHealthPct());
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_overcharged_soul_damage_SpellScript::ChangeDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_overcharged_soul_damage_SpellScript();
        }
};

void AddSC_boss_gu_cloudstrike()
{
    new boss_gu_cloudstrike();
    new npc_azure_serpent();
    new AreaTrigger_at_gu_intro();
    new spell_kill_guardians();
    new spell_overcharged_soul_damage();
}
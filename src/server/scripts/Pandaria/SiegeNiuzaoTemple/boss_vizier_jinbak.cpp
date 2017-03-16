//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "siege_of_the_niuzoa_temple.hpp"

enum eSpells
{
    SPELL_SUMMON_GLOBULE = 119990,
    SPELL_DETONATE = 120001,
    SPELL_DETONATE_ANIMATION = 120002,

    SPELL_SAP_PUDDLE = 119939,
    SPELL_VISUAL_SHIELD = 131628,
    SPELL_SAP_RESIDUE = 119941, // DOT
    SPELL_GROW = 120865,
};

enum eEvents
{
    EVENT_GROW = 1,
    EVENT_DEAL_DOT,
    EVENT_GLOBUES,
    EVENT_DETONATE,
};

enum Pre_Fight_Events
{
    EVENT_WALK = 100,
    EVENT_WALK2 = 101,
    EVENT_WALK3 = 103,
    EVENT_COSMETIC_LASER = 104,
};

#define paddle_size 0.05

class boss_jinbak : public CreatureScript
{
public:
    boss_jinbak() : CreatureScript("boss_jinbak") {}

    struct boss_jinbakAI : public BossAI
    {
        boss_jinbakAI(Creature* creature) : BossAI(creature, DATA_JINBAK),
            instance(creature->GetInstanceScript())// Is this needed here? please test
        {
        }

        InstanceScript* instance;
        bool start;
        bool canContinue;
        std::list<Creature*> Trigger_List;
        std::list<Creature*> Trigger_List2;
        bool Trigger_1IsDead;

        void Reset()
        {
            _Reset();
            events.Reset();

            start = true;
            canContinue = true;

            if (IsHeroic())
            {
                me->SetHealth(12647760);
                me->SetMaxHealth(12647760);
                me->SetLevel(93);
            }
            else
            {
                me->SetHealth(10188300);
                me->SetMaxHealth(10188300);
                me->SetLevel(91);
            }

            events.ScheduleEvent(EVENT_COSMETIC_LASER, 1000);
            events.ScheduleEvent(EVENT_WALK, 12000);

            if (!me->FindNearestCreature(95943, 10.0f, true))
                TempSummon* Creature = me->SummonCreature(95943, 1529.359497f, 5163.071289f, 158.893372f, 1.240147f);
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_GROW, 4000);
            events.ScheduleEvent(EVENT_DETONATE, 35000);
            events.ScheduleEvent(EVENT_GLOBUES, 25000);

            if (instance)
                instance->SetBossState(DATA_JINBAK, IN_PROGRESS);

            me->RemoveAllAuras();
            events.CancelEvent(EVENT_COSMETIC_LASER);
            DoPlaySoundToSet(me, 29390);

            me->MonsterYell("Ah-hah! The lot of you will soon wish you hadn't come this way..", LANG_UNIVERSAL, me->GetGUID()); // Upon Combat

            if (Creature* puddle = me->FindNearestCreature(95943, 100.0f, true))
            {
                puddle->RemoveAllAuras();
                puddle->CastSpell(puddle, 119939);
            }
        }

        void JustReachedHome()
        {
            if (instance)
                instance->SetBossState(DATA_JINBAK, FAIL);
            summons.DespawnAll();

            if (!me->FindNearestCreature(95943, 10.0f, true))
                TempSummon* Creature = me->SummonCreature(95943, 1529.359497f, 5163.071289f, 158.893372f, 1.240147f);
        }

        void DamageTaken(Unit* attacker, uint32& damage) { }

        void JustSummoned(Creature* summoned)
        {
            summons.Summon(summoned);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
            {
                int32 crand = urand(1, 2);

                if (crand == 1)
                {
                    DoPlaySoundToSet(me, 29393);
                    me->MonsterYell("Your carved bones, will decorate the weapons of our warriors!", LANG_UNIVERSAL, me->GetGUID());
                }
                else
                {
                    me->MonsterYell("Did you think you stood a chance?", LANG_UNIVERSAL, me->GetGUID());
                    DoPlaySoundToSet(me, 29394);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            if (UpdateVictim())
            {
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_GLOBUES:
                    {
                        me->MonsterTextEmote("Sap Globules begin to sprout from the tree!", me->GetGUID(), true);
                        // Summon Globe, each at a different location:
                        me->SummonCreature(61623, 1559.777100f, 5154.710938f, 161.472107f);
                        me->SummonCreature(61623, 1497.709595f, 5169.799316f, 161.033157f);
                        me->SummonCreature(61623, 1514.540039f, 5185.884766f, 160.270462f);
                        events.ScheduleEvent(EVENT_GLOBUES, 40000);
                        break;
                    }
                    case EVENT_DETONATE:
                    {
                        if (Creature* puddle = me->FindNearestCreature(95943, 100.0f, true))
                        {
                            me->CastSpell(puddle, 120001);
                            DoPlaySoundToSet(me, 29395);
                            me->MonsterYell("Allow me to show you the power of amber alchemy..", LANG_UNIVERSAL, me->GetGUID());
                        }
                        events.ScheduleEvent(EVENT_DETONATE, 35000);
                        break;
                    }
                    case EVENT_GROW:
                    {
                        if (Creature* puddle = me->FindNearestCreature(95943, 100.0f, true))
                        {
                            puddle->AddAura(120865, puddle);

                            if (puddle->HasAura(120865))
                            {
                                puddle->SetAuraStack(120865, puddle, puddle->GetAura(120865)->GetStackAmount() + 1);
                            }
                            events.ScheduleEvent(EVENT_GROW, 1000);
                        }
                        break;
                    }

                    }
                }
                DoMeleeAttackIfReady();
            }

            if (!UpdateVictim())
            {
                // Boss is set to faction 35 if all three packs aren't dead
                GetCreatureListWithEntryInGrid(Trigger_List, me, 61910, 25.0f);

                if (start && canContinue)
                    canContinue = false;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_COSMETIC_LASER:
                    {
                        if (Creature* puddle = me->FindNearestCreature(95943, 100.0f, true))
                        {
                            me->CastSpell(puddle, 120095);
                            events.ScheduleEvent(EVENT_COSMETIC_LASER, 10000);
                        }
                        break;
                    }
                    case EVENT_WALK:
                    {
                        if (MotionMaster* motion = me->GetMotionMaster())
                        {
                            me->SetFacingTo(5.948628f);
                            motion->MovePoint(1, 1523.044312f, 5166.077637f, 158.893372f);
                            events.ScheduleEvent(EVENT_WALK2, 18000);
                        }
                        break;
                    }
                    case EVENT_WALK2:
                    {
                        if (MotionMaster* motion = me->GetMotionMaster())
                        {
                            me->SetFacingTo(1.948595f);
                            motion->MovePoint(2, 1529.750000f, 5157.278809f, 158.893372f);
                            events.ScheduleEvent(EVENT_WALK3, 12000);

                        }
                        break;
                    }
                    case EVENT_WALK3:
                    {
                        if (MotionMaster* motion = me->GetMotionMaster())
                        {
                            me->SetFacingTo(3.604226f);
                            motion->MovePoint(3, 1536.063477f, 5166.399902f, 158.893372f);
                            events.ScheduleEvent(EVENT_WALK, 12000);
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();

            if (instance)
                instance->SetBossState(DATA_JINBAK, DONE);

            DoPlaySoundToSet(me, 29391);
            me->MonsterYell("But.. I was.. who will...?!", LANG_UNIVERSAL, me->GetGUID());
            summons.DespawnAll();

            if (GameObject* objg = me->FindNearestGameObject(213174, 600.0f))
            {
                //hardened door
                objg->Delete();
            }
            // Summon warden for boss 2
            if (!me->FindNearestCreature(62795, 300.0f, true))
                me->SummonCreature(62795, 1524.239990f, 5309.060059f, 185.227005f, 4.759360f, TEMPSUMMON_MANUAL_DESPAWN, 0);
        }

    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_jinbakAI(creature);
    }
};
class npc_globue : public CreatureScript
{
public:
    npc_globue() : CreatureScript("npc_globue") {}

    struct npc_sap_puddleAI : public ScriptedAI
    {
        npc_sap_puddleAI(Creature* creature) : ScriptedAI(creature)
        {
            me->AddUnitState(UNIT_STATE_CANNOT_AUTOATTACK);
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset()
        {
            if (MotionMaster* motion = me->GetMotionMaster())
                motion->MovePoint(1, 1529.432495f, 5163.758301f, 158.892502f);
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            if (Creature* paddle = me->FindNearestCreature(95943, 100, true))
            {
                // calculate radius
                float preradius = me->GetExactDist2d(me->GetPositionX(), me->GetPositionY());
                float width = 5.0f;

                float radius = width + preradius;
                float modifiedRadius = (me->GetObjectSize()) + radius;

                if (paddle->HasAura(SPELL_GROW))
                {
                    if (Aura* aura = paddle->GetAura(SPELL_GROW))
                    {
                        if (me->IsWithinDistInMap(paddle, modifiedRadius, true))
                        {
                            aura->SetStackAmount(aura->GetStackAmount() + 1);
                            me->DespawnOrUnsummon(100);
                        }
                    }
                }
            }
        }
    private:
        EventMap events;

    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_sap_puddleAI(creature);
    }
};
class detonate_after_Damage : public SpellScriptLoader
{
public:
    detonate_after_Damage() : SpellScriptLoader("detonate_after_Damage") { }

    class residue_after_damagespellscript : public SpellScript
    {
        PrepareSpellScript(residue_after_damagespellscript);

        void Unload()
        {
            if (!GetCaster())
                return;

            Creature* creature = GetCaster()->ToCreature();
            if (!creature)
                return;

            if (Unit* puddle = creature->FindNearestCreature(95943, 100.0f, true))
            {
                std::list<Unit*> PL_list;

                JadeCore::AnyUnitInObjectRangeCheck check(puddle, 30.0f);
                JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> searcher(puddle, PL_list, check);
                puddle->VisitNearbyObject(30.0f, searcher);

                for (std::list<Unit*>::const_iterator it = PL_list.begin(); it != PL_list.end(); ++it)
                {
                    if (!(*it))
                        return;

                    if ((*it)->IsHostileTo(creature))
                    {
                        if (puddle->HasAura(SPELL_GROW))
                        {
                            float damage = 2000;
                            float stacksize = puddle->GetAura(SPELL_GROW)->GetStackAmount();
                            int32 calc = damage + (damage * stacksize);

                            creature->CastCustomSpell((*it), SPELL_DETONATE_ANIMATION, &calc, NULL, NULL, true, NULL);
                        }
                    }

                    if (Creature* paddle = GetCaster()->FindNearestCreature(95943, 100.0f, true))
                    {
                        paddle->RemoveAura(SPELL_GROW);
                    }
                }

            }

            //player->SetMovement(MOVE_ROOT);
        }

        void Register()
        {
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new residue_after_damagespellscript();
    }
};
class npc_sap_puddle : public CreatureScript
{
public:
    npc_sap_puddle() : CreatureScript("npc_sap_puddle") { }

    struct SKULL_BANNER_AI : public CreatureAI
    {
        SKULL_BANNER_AI(Creature* creature) : CreatureAI(creature)
        {
            me->SetObjectScale(0.4f);
        }

        bool Used;
        // TODO -- CORRECT SPELL ID: 114206 (40 yard aura)

        void Reset()
        {
            events.Reset();
            Player_List.clear();

            Used = false;

            me->CastSpell(me, SPELL_SAP_PUDDLE, true);
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            if (Player* pl = me->SelectNearestPlayer(1.0))
            {
                if (pl->IsWithinDistInMap(me, 1.0f, true) && !Used)
                {
                    bool Used = true;
                    events.ScheduleEvent(EVENT_GROW, 2000);
                    events.ScheduleEvent(EVENT_CHARGE, 4000);
                }
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHARGE:
                    Used = false;
                    break;
                case EVENT_GROW:
                    if (me->HasAura(SPELL_GROW))
                    {
                        float stacks = me->GetAura(SPELL_GROW)->GetStackAmount() * 2;

                        if (Player* pl = me->SelectNearestPlayer(1.0 + stacks))
                        {
                            if (pl->IsWithinDistInMap(me, 1.0f + stacks, true) && !Used)
                            {
                                me->AddAura(119941, pl);
                                me->AddAura(120938, pl);
                                if (me->HasAura(SPELL_GROW))
                                {
                                    if (Aura* aura = me->GetAura(SPELL_GROW))
                                    {
                                        //aura->Remove();
                                        aura->Remove();
                                    }
                                }
                            }
                        }
                    }
                    break;
                }

            }
        }
    private:
        EventMap events;
        std::list<Player*> Player_List;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new SKULL_BANNER_AI(creature);
    }
};

class cosmetic_nonplayer_laser : public SpellScriptLoader
{
public:
    cosmetic_nonplayer_laser() : SpellScriptLoader("cosmetic_nonplayer_laser") { }

    class cosmetic_nonplayer_laserspellscript : public SpellScript
    {
        PrepareSpellScript(cosmetic_nonplayer_laserspellscript);

        SpellCastResult CheckTarget()
        {
            if (!GetCaster())
                return SPELL_FAILED_DONT_REPORT;

            if (GetExplTargetUnit())
            {
                if (GetExplTargetUnit()->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_CAST_OK;
            }
            else if (Creature* paddle = GetCaster()->FindNearestCreature(95943, 100.0f, true))
            {
                return SPELL_CAST_OK;
            }

            return SPELL_FAILED_DONT_REPORT;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(cosmetic_nonplayer_laserspellscript::CheckTarget);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new cosmetic_nonplayer_laserspellscript;
    }
};

void AddSC_boss_jinbak()
{
    new cosmetic_nonplayer_laser();
    new detonate_after_Damage();
    new npc_globue();
    new boss_jinbak();
    new npc_sap_puddle();
}
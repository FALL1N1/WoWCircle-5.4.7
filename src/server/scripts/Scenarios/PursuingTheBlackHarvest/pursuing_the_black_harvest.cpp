#include "ScriptPCH.h"
#include "pursuing_the_black_harvest.h"
#include "MoveSplineInit.h"

enum ScriptedTextsAshtongueWorker
{
    SAY_HELP    = 0,
};

enum Spells
{
   SPELL_SEARCHING_FOR_INTRUDERS    = 134110,
   SPELL_NETTED                     = 134111,
   SPELL_BLACKOUT                   = 134112,
};

enum Events
{

};

enum Actions
{

};

enum Equipments
{

};

class IntrudersCheck
{
    public:
        IntrudersCheck(WorldObject const* obj, float range) : _obj(obj), _range(range) {}
        bool operator()(Player* u)
        {
            if (!u->isAlive())
                return false;

            if (!_obj->IsWithinDistInMap(u, _range))
                return false;

            if (u->HasAura(SPELL_TRUSTED_BY_THE_ASHTONGUE))
                return false;

            if (u->HasAura(SPELL_NETTED) || u->HasAura(SPELL_BLACKOUT))
                return false;

            return true;
        }

    private:
        WorldObject const* _obj;
        float _range;
};

class npc_pursuing_the_black_harvest_ashtongue_primalist : public CreatureScript
{
    public:

        enum eTexts
        {
            SAY_YOU_ARE_NOT_WELCOME_HERE = 0
        };

        npc_pursuing_the_black_harvest_ashtongue_primalist() : CreatureScript("npc_pursuing_the_black_harvest_ashtongue_primalist") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_pursuing_the_black_harvest_ashtongue_primalistAI(pCreature);
        }

        struct npc_pursuing_the_black_harvest_ashtongue_primalistAI : public ScriptedAI
        {
            uint64 m_TargetGUID;

            npc_pursuing_the_black_harvest_ashtongue_primalistAI(Creature* creature) : ScriptedAI(creature)
            {
                checkIntruderTimer = 1000;
            }

            void Reset()
            {
                m_TargetGUID = 0;
                me->AddAura(SPELL_SEARCHING_FOR_INTRUDERS, me);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!m_TargetGUID)
                    TryToFindIntruder(diff);
            }

        private:

            Player* FindIntruder()
            {
                Player* player = NULL;
                IntrudersCheck check(me, 31.0f);
                JadeCore::PlayerSearcher<IntrudersCheck> searcher(me, player, check);
                me->VisitNearbyWorldObject(30.0f, searcher);

                return player;
            }

            void TryToFindIntruder(const uint32 diff)
            {
                if (checkIntruderTimer <= diff)
                {
                    checkIntruderTimer = 1000;

                    const float distance = 30.0f;

                    if (Player* intruder = FindIntruder())
                    {
                        AttackIntruder(intruder);
                    }
                }
                else
                {
                    checkIntruderTimer -= diff;
                }
            }

            void AttackIntruder(Player* player)
            {
                Talk(SAY_YOU_ARE_NOT_WELCOME_HERE);

                DoCast(player, SPELL_NETTED, true);


                Position pos;
                player->GetContactPoint(me, pos.m_positionX, pos.m_positionY, pos.m_positionZ);
                player->GetFirstCollisionPosition(pos, player->GetObjectSize(), player->GetRelativeAngle(me));

                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(0, pos);

                m_TargetGUID = player->GetGUID();
            }

            void MovementInform(uint32 p_Type, uint32 p_Id) override
            {
                if (p_Type != POINT_MOTION_TYPE || p_Id != 0)
                    return;

                if (Player* player = ObjectAccessor::FindPlayer(m_TargetGUID))
                {
                    DoCast(player, SPELL_BLACKOUT, true);
                    EnterEvadeMode();
                }
            }

        private:

            uint32 checkIntruderTimer;

        };
};

class npc_pursuing_the_black_harvest_ashtongue_shaman : public CreatureScript
{
    public:

        npc_pursuing_the_black_harvest_ashtongue_shaman() : CreatureScript("npc_pursuing_the_black_harvest_ashtongue_shaman") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_pursuing_the_black_harvest_ashtongue_shamanAI(pCreature);
        }

        struct npc_pursuing_the_black_harvest_ashtongue_shamanAI : public ScriptedAI
        {
            npc_pursuing_the_black_harvest_ashtongue_shamanAI(Creature* creature) : ScriptedAI(creature)
            {
                checkIntruderTimer = 1000;
            }

            void UpdateAI(const uint32 diff)
            {
                TryToFindIntruder(diff);
            }

        private:

            Player* FindIntruder()
            {
                Player* player = NULL;
                IntrudersCheck check(me, 31.0f);
                JadeCore::PlayerSearcher<IntrudersCheck> searcher(me, player, check);
                me->VisitNearbyWorldObject(30.0f, searcher);

                return player;
            }

            void TryToFindIntruder(const uint32 diff)
            {
                if (checkIntruderTimer <= diff)
                {
                    checkIntruderTimer = 1000;

                    const float distance = 30.0f;

                    if (Player* intruder = FindIntruder())
                    {
                        AttackIntruder(intruder);
                    }
                }
                else
                {
                    checkIntruderTimer -= diff;
                }
            }

            void AttackIntruder(Player* player)
            {
                DoCast(player, SPELL_BLACKOUT, true);
            }

        private:

            uint32 checkIntruderTimer;

        };
};

class npc_pursuing_the_black_harvest_ashtongue_worker : public CreatureScript
{
    public:

        npc_pursuing_the_black_harvest_ashtongue_worker() : CreatureScript("npc_pursuing_the_black_harvest_ashtongue_worker") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_pursuing_the_black_harvest_ashtongue_workerAI(pCreature);
        }

        struct npc_pursuing_the_black_harvest_ashtongue_workerAI : public ScriptedAI
        {
            npc_pursuing_the_black_harvest_ashtongue_workerAI(Creature* creature) : ScriptedAI(creature)
            {
                checkIntruderTimer = 1000;
                m_CalledForHelp = false;
            }

            void UpdateAI(const uint32 diff)
            {
                TryToFindIntruder(diff);
            }

        private:

            bool FindIntruder() const
            {
                Player* player = me->FindNearestPlayer(8.0f);

                return player != nullptr;
            }

            void TryToFindIntruder(const uint32 diff)
            {
                if (m_CalledForHelp)
                    return;

                if (checkIntruderTimer <= diff)
                {
                    checkIntruderTimer = 1000;

                    const float distance = 30.0f;

                    if (FindIntruder())
                    {
                        CallForHelp();
                    }
                }
                else
                {
                    checkIntruderTimer -= diff;
                }
            }

            Creature* FindNearestPrimalist()
            {
                Creature* pPrimalist = me->FindNearestCreature(NPC_ASHTONGUE_PRIMALIST, 150.0f);
                if (!pPrimalist || pPrimalist->isMoving())
                    return NULL;

                return pPrimalist;
            }

            void CallForHelp()
            {
                m_CalledForHelp = true;

                if (Creature* pPrimalist = FindNearestPrimalist())
                {
                    Talk(SAY_HELP);

                    pPrimalist->StopMoving();

                    Position pos;
                    me->GetRandomNearPosition(pos, 3.0f);

                    Movement::MoveSplineInit init(*pPrimalist);
                    init.MoveTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
                    init.SetWalk(false);
                    init.Launch();
                }
            }

        private:

            uint32 checkIntruderTimer;
            bool m_CalledForHelp;

        };
};

class npc_pursuing_the_black_harvest_demonic_soulwell : public CreatureScript
{
    public:

        npc_pursuing_the_black_harvest_demonic_soulwell() : CreatureScript("npc_pursuing_the_black_harvest_demonic_soulwell") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_pursuing_the_black_harvest_demonic_soulwellAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (!creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                return true;

            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                if (pInstance->GetData(DATA_SCENARIO_STEP) == STEP_9)
                {
                    pInstance->instance->SummonCreature(NPC_KANRETHAD_EBONLOCKE_AT_END, kanrethadEndPos);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            return true;
        }

        struct npc_pursuing_the_black_harvest_demonic_soulwellAI : public ScriptedAI
        {
            npc_pursuing_the_black_harvest_demonic_soulwellAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

        private:

        private:

        };
};

class npc_pursuing_the_black_harvest_kanrethad_demon : public CreatureScript
{
    public:

        npc_pursuing_the_black_harvest_kanrethad_demon() : CreatureScript("npc_pursuing_the_black_harvest_kanrethad_demon") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_pursuing_the_black_harvest_kanrethad_demonAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (!creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                return true;

            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                if (pInstance->GetData(DATA_SCENARIO_STEP) == STEP_9)
                {
                    player->KilledMonsterCredit(BLACK_TEMPLE_TOP_CREDIT, 0);
                    pInstance->SetData(DATA_KILL_KANRETHAD, DONE);

                    player->CastSpell(creature, SPELL_DRAIN_ENERGY);
                    
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            return true;
        }

        struct npc_pursuing_the_black_harvest_kanrethad_demonAI : public ScriptedAI
        {
            npc_pursuing_the_black_harvest_kanrethad_demonAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

        private:

        private:

        };
};

class spell_pursuing_the_black_harvest_nested : public SpellScriptLoader
{
    public:
        spell_pursuing_the_black_harvest_nested() : SpellScriptLoader("spell_pursuing_the_black_harvest_nested") { }

        class spell_pursuing_the_black_harvest_nested_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pursuing_the_black_harvest_nested_AuraScript);

            int32 m_Timer;

            enum Constants : int32
            {
                Delay = 10
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                m_Timer = Constants::Delay * IN_MILLISECONDS;
            }

            void OnUpdate(uint32 p_Diff, AuraEffect* p_AurEff)
            {
                // Just in case a problem occurred with the primalist
                m_Timer -= p_Diff;
                if (m_Timer <= 0)
                {
                    m_Timer = Constants::Delay * IN_MILLISECONDS;
                    if (Unit* l_Caster = GetUnitOwner())
                        l_Caster->CastSpell(l_Caster, SPELL_BLACKOUT, true);
                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_pursuing_the_black_harvest_nested_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectUpdate += AuraEffectUpdateFn(spell_pursuing_the_black_harvest_nested_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_STUN);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pursuing_the_black_harvest_nested_AuraScript();
        }
};

class spell_pursuing_the_black_harvest_blackout : public SpellScriptLoader
{
    public:
        spell_pursuing_the_black_harvest_blackout() : SpellScriptLoader("spell_pursuing_the_black_harvest_blackout") { }

        class spell_pursuing_the_black_harvest_blackout_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pursuing_the_black_harvest_blackout_AuraScript);

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetUnitOwner())
                    return;
                
                if (Player* player = GetUnitOwner()->ToPlayer())
                {
                    player->NearTeleportTo(entrancePos);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_pursuing_the_black_harvest_blackout_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pursuing_the_black_harvest_blackout_AuraScript();
        }
};

class spell_pursuing_the_black_harvest_drain_energy : public SpellScriptLoader
{
    public:
        spell_pursuing_the_black_harvest_drain_energy() : SpellScriptLoader("spell_pursuing_the_black_harvest_drain_energy") { }

        class spell_pursuing_the_black_harvest_drain_energy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pursuing_the_black_harvest_drain_energy_AuraScript);

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;
                
                if (Player* player = GetCaster()->ToPlayer())
                {
                    player->CastSpell(player, SPELL_FEL_ENERGY_9, true);

                    player->learnSpell(SPELL_CODEX_OF_XERRATH_1, false);
                    player->learnSpell(SPELL_CODEX_OF_XERRATH_2, false);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_pursuing_the_black_harvest_drain_energy_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pursuing_the_black_harvest_drain_energy_AuraScript();
        }
};

void AddSC_pursuing_the_black_harvest()
{
    new npc_pursuing_the_black_harvest_ashtongue_primalist();   // 68096
    new npc_pursuing_the_black_harvest_ashtongue_shaman();      // 68129
    new npc_pursuing_the_black_harvest_ashtongue_worker();      // 68098
    new npc_pursuing_the_black_harvest_demonic_soulwell();      // 69964
    new npc_pursuing_the_black_harvest_kanrethad_demon();       // 69964

    new spell_pursuing_the_black_harvest_nested();              // 134111
    new spell_pursuing_the_black_harvest_blackout();            // 134112
    new spell_pursuing_the_black_harvest_drain_energy();        // 139200
}

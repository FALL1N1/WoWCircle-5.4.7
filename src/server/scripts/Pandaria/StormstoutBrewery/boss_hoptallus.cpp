//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "stormstout_brewery.hpp"

enum Yells
{
    SAY_AGGRO          = 0, // Oh yeah!
    SAY_KILL           = 1, // You die!
    SAY_DEATH          = 2, // You have...turnip...for a head...
    SAY_FURLWIND       = 3, // Gonna spins around !
    SAY_CARROT_BREATH  = 4  // Urp...eats too many carrots...
};

# define ANN_BUNNIES "Hoptallus lets out a loud screech! The virmen are coming!"

enum Spells
{
    // Boss
    SPELL_CARROT_BREATH          = 112944, // Aura. Triggers 112945 dmg.
    SPELL_FURLWIND               = 112992, // Aura. Triggers 112993 dmg.
    SPELL_COSMETIC_EARTHQUAKE    = 114928,
    SPELL_HOPTALLUS_KEG_SCENE    = 114366,

    // NPCs
    SPELL_KEG_CARRY              = 131820, // Hopper carry keg.
    SPELL_BLINKING_EXPL_COSM     = 114287,
    SPELL_BLINKING_EXPL_FUSE     = 114290,
    SPELL_EXPLOSIVE_BREW         = 114291, // When Hopper reaches target.
    SPELL_HOPPER_ANIM_REPLACE    = 114284,
    SPELL_CARROT_DOOR            = 116005,

    SPELL_HAMMER_VISUAL          = 114530,
    SPELL_HAMMER_FALLING         = 114531,
    SPELL_HAMMER_ARROW           = 114533,
    SPELL_SMASH_AURA             = 111662, // Hammer aura.
    SPELL_SMASH                  = 111666, // Player cast spell on button click.
    SPELL_NIBBLER_VISUAL         = 128188
};

enum Events
{
    // Boss
    EVENT_CARROT_BREATH          = 1,
    EVENT_FURLWIND,
    EVENT_SUMMON_NPCS,
    EVENT_SET_COMBAT,

    // NPCs
    EVENT_EXPLOSIVE_BREW,
    EVENT_CHECK_PLAYERS
};

Position const bunnySummonPosition     = { -713.955f, 1254.574f, 164.790f, 0.387467f }; // Bunny adds summon position.
Position const hoptallusMovePosition   = { -696.136f, 1260.781f, 162.781f, 1.862440f }; // Hoptallus move position after summoned.
Position const nibblerSummonPosition   = { -692.167f, 1293.802f, 162.806f, 2.923863f }; // Position to summon Nibbler

uint32 const g_TrashEntries[3] = { NPC_HOPPLING, NPC_HOPPER, NPC_BOPPER };

/// Check if all trashes are done, so Hoptallus can appear
bool CheckTrashDone(Creature* p_Dead)
{
    for (uint8 l_Idx = 0; l_Idx < 3; ++l_Idx)
    {
        if (g_TrashEntries[l_Idx] == p_Dead->GetEntry())
        {
            std::list<Creature*> l_TrashList;
            GetCreatureListWithEntryInGrid(l_TrashList, p_Dead, g_TrashEntries[l_Idx], 50.0f);

            if (!l_TrashList.empty())
            {
                for (Creature* l_Trash : l_TrashList)
                {
                    // Found another trash with the same entry which is alive
                    if (l_Trash != p_Dead && l_Trash->isAlive())
                        return false;
                }
            }
        }
        else
        {
            // Found another trash alive
            if (GetClosestCreatureWithEntry(p_Dead, g_TrashEntries[l_Idx], 50.0f))
                return false;
        }
    }

    // No trash found
    return true;
}

// 56717 - Hoptallus
class boss_hoptallus : public CreatureScript
{
    public :
        boss_hoptallus() : CreatureScript("boss_hoptallus") { }

        struct boss_hoptallus_AI : public BossAI
        {
            boss_hoptallus_AI(Creature* p_Creature) : BossAI(p_Creature, DATA_HOPTALLUS_EVENT), m_Summons(me)
            {
                m_Instance = p_Creature->GetInstanceScript();
                p_Creature->ApplySpellImmune(0, IMMUNITY_ID, 111666, true); // Immune to Smash.
                m_IntroStarted = false;
                m_IntroDone    = false;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            SummonList m_Summons;
            bool m_IntroStarted;
            bool m_IntroDone;

            void Reset()
            {
                m_Events.Reset();
                m_Summons.DespawnAll();

                if (m_Instance)
                    m_Instance->SetData(DATA_HOPTALLUS_EVENT, NOT_STARTED);

                if (Creature* l_Bunny = GetClosestCreatureWithEntry(me, NPC_GENERAL_PURPOSE_BUNNY_B, 50.0f))
                    l_Bunny->AddAura(SPELL_CARROT_DOOR, l_Bunny);

                _Reset();

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);

                if (m_Instance)
                {
                    m_Instance->SetData(DATA_HOPTALLUS_EVENT, IN_PROGRESS);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me); // Add
                }

                // Carrot Breath handled through EVENT_FURLWIND and the other way around.
                m_Events.ScheduleEvent(EVENT_FURLWIND, urand(13000, 16000));
                m_Events.ScheduleEvent(EVENT_SUMMON_NPCS, urand(48000, 52000));

                _EnterCombat();
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_HOPTALLUS_APPEARS)
                {
                    m_IntroDone = true;

                    me->SetReactState(REACT_DEFENSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

                    if (m_Instance)
                    {
                        if (GameObject* l_Keg = m_Instance->instance->GetGameObject(m_Instance->GetData64(GAMEOBJECT_GIANT_BARREL)))
                            l_Keg->SetGoState(GO_STATE_ACTIVE);

                        me->GetMotionMaster()->MoveJump(hoptallusMovePosition.GetPositionX(), hoptallusMovePosition.GetPositionY(),
                                                        hoptallusMovePosition.GetPositionZ(), 20.0f, 5.0f, 10.0f, 1);
                    }

                    // Keg scene; should be played by spell 114366 but, doesn't work...
                    Map::PlayerList const &l_PlayerList = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                        l_Itr->getSource()->PlayScene(SCENE_HOPTALLUS_KEG, l_Itr->getSource());
                }
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (!me->isAlive() || type != EFFECT_MOTION_TYPE || id != 1)
                    return;

                me->SetHomePosition(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                me->SetFacingTo(hoptallusMovePosition.GetOrientation());
                me->SetOrientation(hoptallusMovePosition.GetOrientation());
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->GetMotionMaster()->MoveTargetedHome();

                if (m_Instance)
                {
                    m_Instance->SetData(DATA_HOPTALLUS_EVENT, FAIL);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove
                }

                _EnterEvadeMode();
            }

            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH);
                m_Summons.DespawnAll();

                if (m_Instance)
                {
                    m_Instance->SetData(DATA_HOPTALLUS_EVENT, DONE);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove

                    if (GameObject* l_CarrotDoor = m_Instance->instance->GetGameObject(m_Instance->GetData64(GAMEOBJECT_GIANT_CARROT)))
                        l_CarrotDoor->SetGoState(GO_STATE_ACTIVE);

                    // Todo : a NPC should come with a wheelbarrow, take the carrot, and go away with it
                    if (Creature* l_Dummy = GetClosestCreatureWithEntry(me, NPC_GENERAL_PURPOSE_BUNNY_B, 50.0f))
                        l_Dummy->RemoveAura(SPELL_CARROT_DOOR);
                }

                _JustDied();
            }

            void JustSummoned(Creature* summon)
            {
                m_Summons.Summon(summon);
                summon->setActive(true);

		        if (me->isInCombat())
                    summon->SetInCombatWithZone();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() && m_IntroDone)
                    return;

                m_Events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_FURLWIND:
                            Talk(SAY_FURLWIND);
                            DoCast(me, SPELL_FURLWIND);
                            m_Events.ScheduleEvent(EVENT_CARROT_BREATH, urand(17000, 19000)); // Always 18 seconds after Furlwind.
                            break;
                        case EVENT_CARROT_BREATH:
                            Talk(SAY_CARROT_BREATH);
                            DoCast(me, SPELL_CARROT_BREATH);
                            m_Events.ScheduleEvent(EVENT_FURLWIND, urand(24000, 26000)); // Always 25 seconds after Carrot Breath.
                            break;
                        case EVENT_SUMMON_NPCS:
                            me->MonsterTextEmote(ANN_BUNNIES, 0, true);
                            CallNPCS();
                            m_Events.ScheduleEvent(EVENT_SUMMON_NPCS, urand(48000, 52000)); // Constant.
                            break;
                        default:
                            break;
                    }
                }

                if (m_IntroDone && me->isInCombat())
                    DoMeleeAttackIfReady();
            }

        // Special functions here.
        private:
            void CallNPCS()
            {
                // 15 Hoppling, 7 Hopper, 5 Bopper.
                for (uint8 i = 0; i < 8; i++)
                    if (Creature* hoppling = me->SummonCreature(NPC_HOPPLING, bunnySummonPosition, TEMPSUMMON_MANUAL_DESPAWN))
                        hoppling->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 5.0f, 10.0f);

                for (uint8 i = 0; i < 4; i++)
                    if (Creature* hopper = me->SummonCreature(NPC_HOPPER, bunnySummonPosition, TEMPSUMMON_MANUAL_DESPAWN))
                        hopper->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 5.0f, 10.0f);

                for (uint8 i = 0; i < 2; i++)
                    if (Creature* bopper = me->SummonCreature(NPC_BOPPER, bunnySummonPosition, TEMPSUMMON_MANUAL_DESPAWN))
                        bopper->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 5.0f, 10.0f);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_hoptallus_AI(creature);
        }
};

// 60208 - Hopling
class npc_hopling : public CreatureScript
{
    public:
        npc_hopling() : CreatureScript("npc_hopling") { }

        struct npc_hoplingAI : public ScriptedAI
        {
            npc_hoplingAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void EnterEvadeMode()
            {
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                if (CheckTrashDone(me))
                {
                    if (m_Instance)
                    {
                        if (Creature* m_Hoptallus = m_Instance->instance->GetCreature(m_Instance->GetData64(BOSS_HOPTALLUS)))
                            m_Hoptallus->AI()->DoAction(ACTION_HOPTALLUS_APPEARS);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_hoplingAI(p_Creature);
        }
};

// 59464 - Hopper
class npc_hopper : public CreatureScript
{
    public :
        npc_hopper() : CreatureScript("npc_hopper") { }

        struct npc_hopper_AI : public ScriptedAI
        {
            npc_hopper_AI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit())
            {
                ASSERT(m_Vehicle);
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap events;
            Vehicle* m_Vehicle;

            void Reset()
            {
                events.Reset();

                if (m_Vehicle)
                {
                    if (!m_Vehicle->IsVehicleInUse())
                    {
                        if (Creature* l_Barrel = me->SummonCreature(NPC_EXPLOSIVE_BREW_BARREL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
                        {
                            l_Barrel->EnterVehicle(me, 0);
                            l_Barrel->CastSpell(me, SPELL_BLINKING_EXPL_COSM, true);
                            me->AddAura(SPELL_BLINKING_EXPL_FUSE, me);
                        }
                    }
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 1000);
            }

            void EnterEvadeMode()
            {
                events.Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->DespawnOrUnsummon();
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                if (CheckTrashDone(me))
                {
                    if (m_Instance)
                    {
                        if (Creature* m_Hoptallus = m_Instance->instance->GetCreature(m_Instance->GetData64(BOSS_HOPTALLUS)))
                            m_Hoptallus->AI()->DoAction(ACTION_HOPTALLUS_APPEARS);
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_CHECK_PLAYERS:
                        {
                            if (Player* nearPlayer = me->FindNearestPlayer(3.0f))
                            {
                                if (nearPlayer->IsWithinDistInMap(me, 3.0f))
                                    DoCast(me, SPELL_EXPLOSIVE_BREW);
                                else
                                    events.ScheduleEvent(EVENT_CHECK_PLAYERS, 1000);
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 1000);
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
            return new npc_hopper_AI(creature);
        }
};

// 59551 - Bopper
class npc_bopper : public CreatureScript
{
    public :
        npc_bopper() : CreatureScript("npc_bopper") { }

        struct npc_bopper_AI : public ScriptedAI
        {
            npc_bopper_AI(Creature* creature) : ScriptedAI(creature)
            {
                m_Instance = creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset()
            {
                me->AddAura(SPELL_HAMMER_VISUAL, me);
            }

            void EnterEvadeMode()
            {
                Reset();
                me->DeleteThreatList();
                me->CombatStop(false);
                me->DespawnOrUnsummon();
            }

            void JustDied(Unit* /*killer*/)
            {
                me->RemoveAura(SPELL_HAMMER_VISUAL);
                me->CastSpell(me, SPELL_HAMMER_FALLING, true);

                if (Unit* hammer = me->SummonCreature(NPC_BOPPER_HAMMER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000))
                {
                    hammer->ToCreature()->SetReactState(REACT_PASSIVE);
                    hammer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    hammer->AddAura(SPELL_HAMMER_VISUAL, hammer);
                    hammer->AddAura(SPELL_HAMMER_ARROW, hammer);
                }

                if (CheckTrashDone(me))
                {
                    if (m_Instance)
                    {
                        if (Creature* m_Hoptallus = m_Instance->instance->GetCreature(m_Instance->GetData64(BOSS_HOPTALLUS)))
                            m_Hoptallus->AI()->DoAction(ACTION_HOPTALLUS_APPEARS);
                    }
                }
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
            return new npc_bopper_AI(creature);
        }
};

// 59539 - Big Ol' Hammer
class npc_hoptallus_bopper_hammer : public CreatureScript
{
    public:
        npc_hoptallus_bopper_hammer() : CreatureScript("npc_hoptallus_bopper_hammer") { }

        struct npc_hoptallus_bopper_hammerAI : public PassiveAI
        {
            npc_hoptallus_bopper_hammerAI(Creature* creature) : PassiveAI(creature) { }

            void Reset() override
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            void OnSpellClick(Unit* clicker) override
            {
                clicker->AddAura(SPELL_SMASH_AURA, clicker);
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_hoptallus_bopper_hammerAI(creature);
        }
};

class PositionCheck : public std::unary_function<Unit*, bool>
{
    public:
        explicit PositionCheck(Unit* _caster) : caster(_caster) { }
        bool operator()(WorldObject* object)
        {
              return !caster->HasInArc(M_PI / 6, object);
        }

    private:
        Unit* caster;
};

class PlayerCheck : public std::unary_function<Unit*, bool>
{
    public:
        explicit PlayerCheck(Unit* _caster) : caster(_caster) { }
        bool operator()(WorldObject* object)
        {
            return object->GetTypeId() != TYPEID_PLAYER;
        }

    private:
        Unit* caster;
};

// Carrot Breath 112944
class spell_hoptallus_carrot_breath : public SpellScriptLoader
{
    public:
        spell_hoptallus_carrot_breath() : SpellScriptLoader("spell_hoptallus_carrot_breath") { }

        class spell_hoptallus_carrot_breath_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hoptallus_carrot_breath_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                PreventDefaultAction(); // Don't apply stupid dummy aura.
            }

            void Rotate(AuraEffect const* /*aurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    float l_Ori = l_Caster->GetOrientation() + M_PI / 12.0f;
                    l_Caster->SetOrientation(l_Ori);
                    l_Caster->SetFacingTo(l_Ori);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_hoptallus_carrot_breath_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hoptallus_carrot_breath_AuraScript::Rotate, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hoptallus_carrot_breath_AuraScript();
        }
};

// Hoptallus Carrot breath damage 112945.
class spell_hoptallus_carrot_breath_damage : public SpellScriptLoader
{
    public:
        spell_hoptallus_carrot_breath_damage() : SpellScriptLoader("spell_hoptallus_carrot_breath_damage") { }

        class spell_hoptallus_carrot_breath_damageSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hoptallus_carrot_breath_damageSpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (Unit* l_Caster = GetCaster())
                    targets.remove_if(PlayerCheck(l_Caster));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hoptallus_carrot_breath_damageSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hoptallus_carrot_breath_damageSpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_CONE_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hoptallus_carrot_breath_damageSpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_CONE_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hoptallus_carrot_breath_damageSpellScript();
        }
};

// Hoptallus Furlwind damage 112993.
class spell_hoptallus_furlwind_damage : public SpellScriptLoader
{
    public:
        spell_hoptallus_furlwind_damage() : SpellScriptLoader("spell_hoptallus_furlwind_damage") { }

        class spell_hoptallus_furlwind_damageSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hoptallus_furlwind_damageSpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (Unit* l_Caster = GetCaster())
                    targets.remove_if(PlayerCheck(l_Caster));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_hoptallus_furlwind_damageSpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_hoptallus_furlwind_damageSpellScript();
        }
};

void AddSC_boss_hoptallus()
{
    new boss_hoptallus();                       // 56717
    new npc_hopper();                           // 59464
    new npc_bopper();                           // 59551
    new npc_hoptallus_bopper_hammer();          // 59539
    new npc_hopling();                          // 60208
    new spell_hoptallus_carrot_breath();        // 112944
    new spell_hoptallus_carrot_breath_damage(); // 112945
    new spell_hoptallus_furlwind_damage();      // 112993
}

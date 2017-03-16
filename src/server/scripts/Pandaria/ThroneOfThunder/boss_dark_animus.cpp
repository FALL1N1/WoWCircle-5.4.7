#include "throne_of_thunder.hpp"

// 137453
// 138322 visual anima orb

enum Spells
{
    // Anima Orb
    SPELL_TRANSFUSION_AOE           = 138118,
    SPELL_TRANSFUSION_ENERGIZE      = 138399,

    // Anima Golem
    SPELL_EVASIVE                   = 140759,
    SPELL_ACCELERATION_LINK         = 138451,
    SPELL_ACCELERATION_LINK_DUMMY   = 138452,
    SPELL_ACCELERATION_LINK_AURA    = 138453,
    SPELL_ANIMA_CAPACITY_4          = 138384,
    SPELL_CRITICALLY_DAMAGED_1      = 138400,
    SPELL_CRITICALLY_DAMAGED_2      = 138409,

    // Large Anima Golem
    SPELL_CRIMSON_WAKE_AOE          = 138541, // targetting
    SPELL_CRIMSON_WAKE              = 138480,
    SPELL_CRIMSON_WAKE_SLOW         = 138482,
    SPELL_CRIMSON_WAKE_MEDIUM       = 138484,
    SPELL_CRIMSON_WAKE_FAST         = 138544,
    SPELL_CRIMSON_WAKE_DMG          = 138485,
    SPELL_FIXATED                   = 138486,
    SPELL_ANIMA_CAPACITY_8          = 138385,

    // Massive Anima Golem
    SPELL_MATTER_SWAP               = 138609,
    SPELL_MATTER_SWAP_DAMAGE        = 138618,
    SPELL_TARGETED_MATTER_SWAP      = 139919,
    SPELL_EXPLOSIVE_SLAM            = 138569,
    SPELL_ANIMA_CAPACITY_36         = 138386,

    // Dark Animus
    SPELL_ACTIVATION_SEQUENCE       = 139537,   // LFR only

    SPELL_SIPHON_ANIMA              = 138644,

    SPELL_TOUCH_OF_THE_ANIMUS       = 138659,

    SPELL_EMPOWER_GOLEM             = 138780,   // Heroic only

    SPELL_ANIMA_RING                = 136954,   // 25 Anima needed
    SPELL_ANIMA_RING_1              = 136955,
    SPELL_ANIMA_RING_2              = 136956,
    SPELL_ANIMA_RING_3              = 136957,
    SPELL_ANIMA_RING_4              = 136958,
    SPELL_ANIMA_RING_5              = 136959,
    SPELL_ANIMA_RING_6              = 136960,
    SPELL_ANIMA_RING_7              = 138671,
    SPELL_ANIMA_RING_8              = 138672,
    SPELL_ANIMA_RING_9              = 138673,
    SPELL_ANIMA_RING_10             = 138674,
    SPELL_ANIMA_RING_11             = 138675,
    SPELL_ANIMA_RING_12             = 138676,
    SPELL_ANIMA_RING_DEBUFF         = 136962,

    SPELL_ANIMA_FONT                = 138691,   // 50 Anima needed
    SPELL_ANIMA_FONT_MISSILE        = 138697,

    SPELL_INTERRUPTING_JOLT         = 138763,   // 75 Anima needed
    SPELL_FULL_POWER                = 138729,   // 100 Anima neded (soft enrage)
    SPELL_FULL_POWER_MISSILE        = 138749,

    SPELL_ANIMA_CAPACITY_100        = 137387,
    SPELL_BERSERK                   = 47008,

    // Misc
    SPELL_BLOOD_ORB_VISUAL          = 138116,
    SPELL_HOVER                     = 138092,
    SPELL_FIERY_HANDS               = 34223,
    SPELL_SHIELD_INTERRUPTION       = 140021,
    SPELL_RITUAL_TRANSFORM_1        = 137618,
    SPELL_RITUAL_TRANSFORM_2        = 137898,
    SPELL_RITUAL_TRANSFORM_3        = 137899,
    SPELL_RITUAL_TRANSFORM_4        = 137900,
    SPELL_RITUAL_TRANSFORM_5        = 137902,
    SPELL_RITUAL_TRANSFORM_6        = 137903,
    SPELL_RITUAL_TRANSFORM_7        = 137904,
    SPELL_RITUAL_TRANSFORM_8        = 137906,
    SPELL_RITUAL_TRANSFORM_9        = 137944,
    SPELL_RITUAL_TRANSFORM_10       = 137945,
    SPELL_RITUAL_TRANSFORM_11       = 137946,
    SPELL_RITUAL_TRANSFORM_12       = 137947,
    SPELL_RITUAL_TRANSFORM_13       = 137948,
    SPELL_RITUAL_TRANSFORM_14       = 137949,
    SPELL_RITUAL_TRANSFORM_15       = 137951,
    SPELL_RITUAL_TRANSFORM_16       = 137952,
    SPELL_RITUAL_TRANSFORM_17       = 137953,
    SPELL_RITUAL_TRANSFORM_18       = 137954,
    SPELL_RITUAL_TRANSFORM_19       = 137955,
    SPELL_RITUAL_TRANSFORM_20       = 137956,
    SPELL_RITUAL_TRANSFORM_21       = 137957,
    SPELL_RITUAL_TRANSFORM_22       = 137958,
    SPELL_RITUAL_TRANSFORM_23       = 137959,
    SPELL_RITUAL_TRANSFORM_24       = 137960,
    SPELL_ZERO_ENERGY               = 72242,
    SPELL_TURNED_OFF                = 138373,
    SPELL_ENERGIZE_SEARCHER         = 138378,
    SPELL_ENERGIZE_CUSTOM           = 138909,
    SPELL_FIRE_BLAST                = 138099,
    SPELL_SHADOW_NOVA               = 137998,
    SPELL_SHORT_RITUAL_LIGHTNING    = 137995,
    SPELL_LONG_RITUAL_LIGHTNING     = 137994
};

enum Adds
{
    NPC_DARK_RITUALIST              = 69702,
    NPC_ANIMA_GOLEM                 = 69701,
    NPC_LARGE_ANIMA_GOLEM           = 69700,
    NPC_MASSIVE_ANIMA_GOLEM         = 69699,
    NPC_CRIMSON_WAKE                = 69951,
    NPC_ANIMA_ORB                   = 69756,
};

enum Events
{
    EVENT_BERSERK   = 1,
    EVENT_FIRE_BLAST,
    EVENT_RITUAL_LIGHTNING,
    EVENT_SHADOW_NOVA,
    EVENT_CHECK_NEAR_GOLEM,
    EVENT_CRIMSON_WAKE,
    EVENT_EXPLOSIVE_SLAM,
    EVENT_MATTER_SWAP,
    EVENT_SIPHON_ANIMA,
    EVENT_TOUCH_OF_THE_ANIMUS,
    EVENT_ANIMA_RING,
    EVENT_ANIMA_FONT,
    EVENT_INTERRUPTING_JOLT,
    EVENT_FULL_POWER,
    EVENT_EMPOWER_GOLEM,
    EVENT_CRIMSON_WAKE_MOVE,
    EVENT_CRIMSON_WAKE_DESPAWN,
};

enum Says
{
    TALK_INTRO,
    TALK_AGGRO,
    TALK_DEATH
};

enum Actions
{
    ACTION_FIGHT_RITUALISTS = 1,
    ACTION_KILL_RITUALISTS,
    ACTION_ACTIVATE_GOLEM,
    ACTION_START_FIGHT,
    ACTION_SET_DAMAGED,
};

enum ScriptData
{
    DATA_DAMAGED    = 1,
    DATA_ENERGY,
};

uint32 const g_RitualistTransform[24] =
{
    SPELL_RITUAL_TRANSFORM_1,
    SPELL_RITUAL_TRANSFORM_2,
    SPELL_RITUAL_TRANSFORM_3,
    SPELL_RITUAL_TRANSFORM_4,
    SPELL_RITUAL_TRANSFORM_5,
    SPELL_RITUAL_TRANSFORM_6,
    SPELL_RITUAL_TRANSFORM_7,
    SPELL_RITUAL_TRANSFORM_8,
    SPELL_RITUAL_TRANSFORM_9,
    SPELL_RITUAL_TRANSFORM_10,
    SPELL_RITUAL_TRANSFORM_11,
    SPELL_RITUAL_TRANSFORM_12,
    SPELL_RITUAL_TRANSFORM_13,
    SPELL_RITUAL_TRANSFORM_14,
    SPELL_RITUAL_TRANSFORM_15,
    SPELL_RITUAL_TRANSFORM_16,
    SPELL_RITUAL_TRANSFORM_17,
    SPELL_RITUAL_TRANSFORM_18,
    SPELL_RITUAL_TRANSFORM_19,
    SPELL_RITUAL_TRANSFORM_20,
    SPELL_RITUAL_TRANSFORM_21,
    SPELL_RITUAL_TRANSFORM_22,
    SPELL_RITUAL_TRANSFORM_23,
    SPELL_RITUAL_TRANSFORM_24
};

#define MAX_ANIMA_RING_SPELLS 12
uint32 const animaRingSpells[MAX_ANIMA_RING_SPELLS] =
{
    SPELL_ANIMA_RING_1,
    SPELL_ANIMA_RING_2,
    SPELL_ANIMA_RING_3,
    SPELL_ANIMA_RING_4,
    SPELL_ANIMA_RING_5,
    SPELL_ANIMA_RING_6,
    SPELL_ANIMA_RING_7,
    SPELL_ANIMA_RING_8,
    SPELL_ANIMA_RING_9,
    SPELL_ANIMA_RING_10,
    SPELL_ANIMA_RING_11,
    SPELL_ANIMA_RING_12
};

const Position animaGolemPos[25] = 
{
    {5739.72f, 4774.27f, 75.3739f, 1.56583f},
    {5713.01f, 4766.28f, 77.6697f, 1.592f},
    {5707.22f, 4766.16f, 77.6697f, 1.592f},
    {5711.69f, 4848.08f, 77.6699f, 4.72574f},
    {5700.94f, 4766.02f, 77.6697f, 1.59505f},
    {5706.36f, 4848.2f, 77.6699f, 4.74712f},
    {5701.09f, 4847.81f, 77.6699f, 4.78988f},
    {5753.78f, 4850.66f, 77.6699f, 4.73796f},
    {5747.5f, 4850.58f, 77.6699f, 4.78683f},
    {5759.71f, 4850.82f, 77.6699f, 4.73796f},
    {5754.09f, 4764.69f, 77.6687f, 1.47507f},
    {5748.19f, 4764.79f, 77.6687f, 1.50256f},
    {5759.95f, 4764.35f, 77.6687f, 1.50867f},
    {5739.69f, 4839.37f, 75.3739f, 4.70611f},
    {5732.19f, 4839.05f, 75.3739f, 4.74581f},
    {5724.72f, 4774.3f, 75.3739f, 1.56888f},
    {5724.77f, 4839.0f, 75.3739f, 4.71832f},
    {5732.13f, 4774.28f, 75.3739f, 1.56888f},
    {5783.33f, 4829.07f, 77.6391f, 3.11155f},
    {5783.18f, 4824.22f, 77.6391f, 3.11155f},
    {5780.19f, 4786.08f, 77.6475f, 3.12943f},
    {5780.13f, 4781.37f, 77.6475f, 3.12943f},
    {5765.98f, 4813.97f, 75.3747f, 3.2852f},
    {5765.99f, 4805.92f, 75.3747f, 3.13816f},
    {5766.35f, 4798.31f, 75.3739f, 3.13816f}
};

const Position largeAnimaGolemPos[8] = 
{
    {5751.12f, 4856.66f, 77.663f, 4.76632f},
    {5759.69f, 4856.34f, 77.665f, 4.68386f},
    {5715.51f, 4854.11f, 77.6676f, 4.7384f},
    {5705.1f, 4853.84f, 77.6676f, 4.7384f},
    {5716.56f, 4757.37f, 77.6677f, 1.57411f},
    {5707.36f, 4757.34f, 77.6677f, 1.57106f},
    {5750.7f, 4757.17f, 77.667f, 1.587f},
    {5761.87f, 4757.26f, 77.6647f, 1.68474f}
};

const Position massiveAnimaGolemPos[2] = 
{
    {5786.09f, 4842.15f, 77.7136f, 3.14776f},
    {5786.0f, 4772.41f, 77.7091f, 3.16324f}
};

const Position animaOrbPos = {5732.17f, 4806.6f, 75.3752f, 3.15363f};

struct npc_dark_animus_golem_baseAI : public ScriptedAI
{
    public:

        npc_dark_animus_golem_baseAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            pInstance = me->GetInstanceScript();
        }

        void Reset()
        {
            events.Reset();
            summons.DespawnAll();

            me->SetReactState(REACT_PASSIVE);

            me->AddAura(SPELL_TURNED_OFF, me);
            me->AddAura(GetCapasitySpell(), me);
            me->AddAura(SPELL_ZERO_ENERGY, me);
            me->AddAura(SPELL_EVASIVE, me);

            me->setPowerType(POWER_ENERGY);
            me->SetMaxPower(POWER_ENERGY, GetMaxPower());
            me->SetPower(POWER_ENERGY, 0);

            me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6); // Sniffed value
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK5); // Sniffed value

            isDisabled = true;
            isDamaged = false;
            energyPower = 0;
        }

        void DamageTaken(Unit* who, uint32& damage)
        {
            if (isDisabled)
            {
                damage = 0;
                return;
            }

            // need to save power value before death
            // because power will be 0 if creature dies
            if (!energyPower && me->GetHealth() <= damage)
            {
                energyPower = me->GetPower(POWER_ENERGY);
            }
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_SET_DAMAGED:
                    SetDamaged(true);
                    break;
                case ACTION_ACTIVATE_GOLEM:
                    ActivateGolem();
                    break;
            }
        }

        uint32 GetData(uint32 type)
        {
            switch (type)
            {
                case DATA_DAMAGED:
                    return (isDamaged ? 1 : 0);
                case DATA_ENERGY:
                    return energyPower;
            }

            return 0;
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);

            if (me->isInCombat())
                DoZoneInCombat(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }

        void JustDied(Unit* p_Killer)
        {
            events.Reset();
            summons.DespawnAll();

            me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), SPELL_ENERGIZE_SEARCHER, true);
        }

    private:

        uint32 GetCapasitySpell()
        {
            switch (me->GetEntry())
            {
                case NPC_ANIMA_GOLEM:
                    return SPELL_ANIMA_CAPACITY_4;
                case NPC_LARGE_ANIMA_GOLEM:
                    return SPELL_ANIMA_CAPACITY_8;
                case NPC_MASSIVE_ANIMA_GOLEM:
                    return SPELL_ANIMA_CAPACITY_36;
                case NPC_DARK_ANIMUS:
                    return SPELL_ANIMA_CAPACITY_100;
            }

            return 0;
        }

        // there is mod_max_power auras (capacity spells)
        // but they are not implemented yet
        uint32 GetMaxPower()
        {
            switch (me->GetEntry())
            {
                case NPC_ANIMA_GOLEM:
                    return 4;
                case NPC_LARGE_ANIMA_GOLEM:
                    return 8;
                case NPC_MASSIVE_ANIMA_GOLEM:
                    return 36;
                case NPC_DARK_ANIMUS:
                    return 100;
            }

            return 0;
        }

        void SetDamaged(bool value)
        {
            if (value)
            {
                isDamaged = true;
                me->AddAura(SPELL_CRITICALLY_DAMAGED_1, me);
                me->AddAura(SPELL_CRITICALLY_DAMAGED_2, me);
            }
            else
            {
                isDamaged = false;
                me->RemoveAura(SPELL_CRITICALLY_DAMAGED_1);
                me->RemoveAura(SPELL_CRITICALLY_DAMAGED_2);
            }
            
        }

        void ActivateGolem()
        {
            if (!isDisabled)
                return;

            if (isDamaged)
                return;

            isDisabled = false;

            me->RemoveAura(SPELL_TURNED_OFF);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

            me->SetReactState(REACT_AGGRESSIVE);

            if (Player* target = me->FindNearestPlayer(100.f))
                AttackStart(target);
        }

    protected:

        InstanceScript* pInstance;
        bool isDisabled;
        bool isDamaged;
        int32 energyPower;
};

// Dark Animus - 69427
class boss_dark_animus : public CreatureScript
{
    public:
        boss_dark_animus() : CreatureScript("boss_dark_animus") { }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_dark_animusAI(p_Creature);
        }

        struct boss_dark_animusAI : public BossAI
        {
            boss_dark_animusAI(Creature* p_Creature) : BossAI(p_Creature, DATA_DARK_ANIMUS)
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                me->AddAura(SPELL_TURNED_OFF, me);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 15.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 15.0f);
            }

            void Reset()
            {
                _Reset();

                me->SetReactState(REACT_PASSIVE);

                me->AddAura(SPELL_ANIMA_CAPACITY_100, me);
                me->AddAura(SPELL_ZERO_ENERGY, me);

                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 0);

                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                //me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6); // Sniffed value
                //me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK5); // Sniffed value

                isDisabled = true;
                isAnimaRingEnabled = false;
                isAnimaFontEnabled = false;
                isInterruptingJoltEnabled = false;
                isFullPowerEnabled = false;

                RemoveInstanceAuras();

                SpawnGolems();
            }

            void JustReachedHome()
            {
                me->AddAura(SPELL_TURNED_OFF, me);
            }

            void KilledUnit(Unit* p_Who)
            {
            }

            void EnterCombat(Unit* p_Attacker)
            {
                instance->SetBossState(DATA_DARK_ANIMUS, IN_PROGRESS);
            }

            void DamageTaken(Unit* who, uint32& damage)
            {
                if (isDisabled)
                {
                    damage = 0;
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ACTIVATE_GOLEM:
                    {
                        ActivateGolem();
                        break;
                    }
                    case ACTION_START_FIGHT:
                    {
                        DoZoneInCombat();
                        break;
                    }
                    default:
                        break;
                }
            }

            void JustDied(Unit* killer)
            {
                _JustDied();

                RemoveInstanceAuras();
            }
            
            void UpdateAI(const uint32 p_Diff)
            {
                // UpdateVictim should be called first to enable evade mode
                if (!UpdateVictim() || isDisabled)
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                EnableNewAbilities();

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BERSERK:
                            me->CastSpell(me, SPELL_BERSERK, true);
                            break;
                        case EVENT_SIPHON_ANIMA:
                        {
                            //uint8 l_Rand = urand(0, 10);
                            DoCastAOE(SPELL_SIPHON_ANIMA);
                            //if (l_Rand)
                            //    me->MonsterTextEmote("Dark Animus drains power from the other golems with |cFFF00000|Hspell:138644|h[Siphon Anima]|h|r!", 0, true);
                            events.ScheduleEvent(EVENT_SIPHON_ANIMA, 6000);
                            break;
                        }
                        case EVENT_TOUCH_OF_THE_ANIMUS:
                            DoCastAOE(SPELL_TOUCH_OF_THE_ANIMUS);
                            events.ScheduleEvent(EVENT_TOUCH_OF_THE_ANIMUS, 11000);
                            break;
                        case EVENT_ANIMA_RING:
                            DoCastVictim(SPELL_ANIMA_RING);
                            events.ScheduleEvent(EVENT_ANIMA_RING, urand(12000, 24200));
                            break;
                        case EVENT_ANIMA_FONT:
                            DoCastAOE(SPELL_ANIMA_FONT);
                            events.ScheduleEvent(EVENT_ANIMA_FONT, 25000);
                            break;
                        case EVENT_INTERRUPTING_JOLT:
                            DoCastAOE(SPELL_INTERRUPTING_JOLT);
                            me->MonsterTextEmote("Stop casting! Dark Animus is generating an |cFFF00000|Hspell:138763|h[Interrupting Jolt]|h|r.", 0, true);
                            events.ScheduleEvent(EVENT_INTERRUPTING_JOLT, 21500);
                            break;
                        case EVENT_EMPOWER_GOLEM:
                            me->CastSpell(me, SPELL_EMPOWER_GOLEM, true);
                            events.ScheduleEvent(EVENT_EMPOWER_GOLEM, 16000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            void SpawnGolems()
            {
                // spawn anima orb
                me->SummonCreature(NPC_ANIMA_ORB, animaOrbPos);

                // spawn anima golems
                std::list<uint64> animaGolems;
                for (uint8 i = 0; i < 25; ++i)
                {
                    if (Creature* pGolem = me->SummonCreature(NPC_ANIMA_GOLEM, animaGolemPos[i]))
                    {
                        animaGolems.push_back(pGolem->GetGUID());
                    }
                }

                // Turn off 13 anima golems in 10ppl raids
                if (!Is25ManRaid())
                {
                    JadeCore::Containers::RandomResizeList(animaGolems, 13);
                    for (std::list<uint64>::const_iterator itr = animaGolems.begin(); itr != animaGolems.end(); ++itr)
                    {
                        if (Creature* pGolem = instance->instance->GetCreature((*itr)))
                        {
                            pGolem->AI()->DoAction(ACTION_SET_DAMAGED);
                        }
                    }
                }

                // spawn large anima golems
                std::list<uint64> largeAnimaGolems;
                for (uint8 i = 0; i < 8; ++i)
                { 
                    if (Creature* pGolem = me->SummonCreature(NPC_LARGE_ANIMA_GOLEM, largeAnimaGolemPos[i]))
                    {
                        largeAnimaGolems.push_back(pGolem->GetGUID());
                    }
                }

                // Turn off 3 large anima golems in 10ppl raids
                if (!Is25ManRaid())
                {
                    JadeCore::Containers::RandomResizeList(largeAnimaGolems, 3);
                    for (std::list<uint64>::const_iterator itr = largeAnimaGolems.begin(); itr != largeAnimaGolems.end(); ++itr)
                    {
                        if (Creature* pGolem = instance->instance->GetCreature((*itr)))
                        {
                            pGolem->AI()->DoAction(ACTION_SET_DAMAGED);
                        }
                    }
                }

                for (uint8 i = 0; i < 2; ++i)
                    me->SummonCreature(NPC_MASSIVE_ANIMA_GOLEM, massiveAnimaGolemPos[i]);
            }

            void RemoveInstanceAuras()
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MATTER_SWAP);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EXPLOSIVE_SLAM);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_THE_ANIMUS);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ANIMA_FONT);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ANIMA_RING_DEBUFF);
            }

            void EnableNewAbilities()
            {
                if (!isAnimaRingEnabled && me->GetPower(POWER_ENERGY) >= 25)
                {
                    isAnimaRingEnabled = true;
                    events.ScheduleEvent(EVENT_ANIMA_RING, urand(13000, 24000));
                }

                if (!isAnimaFontEnabled && me->GetPower(POWER_ENERGY) >= 50)
                {
                    isAnimaFontEnabled = true;
                    events.ScheduleEvent(EVENT_ANIMA_RING, urand(13000, 24000));
                }

                if (!isInterruptingJoltEnabled && me->GetPower(POWER_ENERGY) >= 75)
                {
                    isInterruptingJoltEnabled = true;
                    events.ScheduleEvent(EVENT_INTERRUPTING_JOLT, 18000);
                }

                if (!isFullPowerEnabled && me->GetPower(POWER_ENERGY) >= 100)
                {
                    isFullPowerEnabled = true;
                    events.ScheduleEvent(EVENT_FULL_POWER, urand(500, 1000));
                }
            }

            void ActivateGolem()
            {
                if (!isDisabled)
                    return;

                isDisabled = false;

                me->RemoveAura(SPELL_TURNED_OFF);
                            
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                
                me->SetReactState(REACT_AGGRESSIVE);

                events.ScheduleEvent(EVENT_BERSERK, 600000);  // 10 min
                events.ScheduleEvent(EVENT_SIPHON_ANIMA, 6000);
                events.ScheduleEvent(EVENT_TOUCH_OF_THE_ANIMUS, 11000);

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_EMPOWER_GOLEM, 16000);

                if (Player* target = me->FindNearestPlayer(50.f))
                    AttackStart(target);
            }

        private:

            bool isAnimaRingEnabled;
            bool isAnimaFontEnabled;
            bool isInterruptingJoltEnabled;
            bool isFullPowerEnabled;
            bool isDisabled;
            
        };
};

// Anima Orb - 69756
class npc_dark_animus_anima_orb : public CreatureScript
{
    public:
        npc_dark_animus_anima_orb() : CreatureScript("npc_dark_animus_anima_orb") { }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_dark_animus_anima_orbAI(p_Creature);
        }

        struct npc_dark_animus_anima_orbAI : public Scripted_NoMovementAI
        {
            npc_dark_animus_anima_orbAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                pInstance = p_Creature->GetInstanceScript();
                me->SetReactState(REACT_PASSIVE);

                isExploded = false;
            }

            void Reset()
            {
                me->AddAura(SPELL_BLOOD_ORB_VISUAL, me);

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                damage = 0;

                if (!isExploded)
                {
                    isExploded = true;

                    me->CastSpell(me, SPELL_TRANSFUSION_AOE, true);

                    if (pInstance)
                    {
                        if (Creature* pAnimus = pInstance->instance->GetCreature(pInstance->GetData64(DATA_DARK_ANIMUS)))
                        {
                            pAnimus->AI()->DoAction(ACTION_START_FIGHT);
                        }
                    }
                    
                    me->RemoveAura(SPELL_BLOOD_ORB_VISUAL);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                    me->DespawnOrUnsummon(10000);
                }
            }

            void JustDied(Unit* who)
            {

            }

        private:

            InstanceScript* pInstance;
            bool isExploded;
        };
};

// Dark Ritualist - 69702
class npc_dark_animus_dark_ritualist : public CreatureScript
{
    public:
        npc_dark_animus_dark_ritualist() : CreatureScript("npc_dark_animus_dark_ritualist") { }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_dark_animus_dark_ritualistAI(p_Creature);
        }

        struct npc_dark_animus_dark_ritualistAI : public ScriptedAI
        {
            npc_dark_animus_dark_ritualistAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset()
            {
                events.Reset();

                me->CastSpell(me, g_RitualistTransform[urand(0, 23)], true);
                me->CastSpell(me, SPELL_HOVER, true);
                me->CastSpell(me, SPELL_SHIELD_INTERRUPTION, true);

                if (urand(0, 1))
                    me->AddAura(SPELL_FIERY_HANDS, me);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (m_Instance)
                {
                    if (Creature* l_AnimaOrb = Creature::GetCreature(*me, m_Instance->GetData64(NPC_ANIMA_ORB)))
                        l_AnimaOrb->AI()->DoAction(ACTION_FIGHT_RITUALISTS);
                }

                me->RemoveAura(SPELL_HOVER);
                me->RemoveAura(SPELL_SHIELD_INTERRUPTION);

                events.ScheduleEvent(EVENT_FIRE_BLAST, urand(2000, 5000));
                events.ScheduleEvent(EVENT_SHADOW_NOVA, urand(3000, 6000));
                events.ScheduleEvent(EVENT_RITUAL_LIGHTNING, urand(5000, 10000));
            }

            void JustDied(Unit* p_Killer)
            {
                if (m_Instance)
                {
                    if (Creature* l_AnimaOrb = Creature::GetCreature(*me, m_Instance->GetData64(NPC_ANIMA_ORB)))
                        l_AnimaOrb->AI()->DoAction(ACTION_KILL_RITUALISTS);
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FIRE_BLAST:
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(l_Target, SPELL_FIRE_BLAST, true);
                            events.ScheduleEvent(EVENT_FIRE_BLAST, urand(12000, 15000));
                            break;
                        case EVENT_SHADOW_NOVA:
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(l_Target, SPELL_SHADOW_NOVA, true);
                            events.ScheduleEvent(EVENT_SHADOW_NOVA, urand(13000, 16000));
                            break;
                        case EVENT_RITUAL_LIGHTNING:
                            if (me->HasAura(SPELL_FIERY_HANDS))
                                me->CastSpell(me, SPELL_LONG_RITUAL_LIGHTNING, false);
                            else
                                me->CastSpell(me, SPELL_SHORT_RITUAL_LIGHTNING, false);
                            events.ScheduleEvent(EVENT_RITUAL_LIGHTNING, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };   
};

// Anima Golem - 69701
class npc_dark_animus_anima_golem : public CreatureScript
{
    public:
        npc_dark_animus_anima_golem() : CreatureScript("npc_dark_animus_anima_golem") { }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_dark_animus_anima_golemAI(p_Creature);
        }

        struct npc_dark_animus_anima_golemAI : public npc_dark_animus_golem_baseAI
        {
            npc_dark_animus_anima_golemAI(Creature* p_Creature) : npc_dark_animus_golem_baseAI(p_Creature)
            {
            }

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);

                me->AddAura(SPELL_TURNED_OFF, me);
                me->AddAura(SPELL_ANIMA_CAPACITY_4, me);
                me->AddAura(SPELL_ZERO_ENERGY, me);
                me->AddAura(SPELL_EVASIVE, me);

                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 4);
                me->SetPower(POWER_ENERGY, 0);

                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6); // Sniffed value
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK5); // Sniffed value

                isDisabled = true;
                isDamaged = false;
                energyPower = 0;
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_CHECK_NEAR_GOLEM, 1000);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim() || isDisabled)
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_NEAR_GOLEM:
                        {
                            UpdateAccelerationLink();
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            void UpdateAccelerationLink()
            {
                std::list<Creature*> golemList;
                me->GetCreatureListWithEntryInGrid(golemList, NPC_ANIMA_GOLEM, 2.f);

                golemList.remove(me);

                if (!golemList.empty())
                {
                    Aura* aur = me->GetAura(SPELL_ACCELERATION_LINK_AURA);
                    if (!aur)
                        aur = me->AddAura(SPELL_ACCELERATION_LINK_AURA, me);

                    aur->SetStackAmount(golemList.size());
                }
                else
                {
                    me->RemoveAura(SPELL_ACCELERATION_LINK_AURA);
                }
            }

        private:

        };
};

// Large Anima Golem - 69700
class npc_dark_animus_large_anima_golem : public CreatureScript
{
    public:
        npc_dark_animus_large_anima_golem() : CreatureScript("npc_dark_animus_large_anima_golem") { }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_dark_animus_large_anima_golemAI(p_Creature);
        }

        struct npc_dark_animus_large_anima_golemAI : public npc_dark_animus_golem_baseAI
        {
            npc_dark_animus_large_anima_golemAI(Creature* p_Creature) : npc_dark_animus_golem_baseAI(p_Creature)
            {
                pInstance = p_Creature->GetInstanceScript();
            }

            void Reset()
            {
                npc_dark_animus_golem_baseAI::Reset();

                DespawnCrimsonWakes();
            }

            void DoAction(int32 const action)
            {
                npc_dark_animus_golem_baseAI::DoAction(action);

                switch (action)
                {
                    case ACTION_ACTIVATE_GOLEM:
                    {
                        // it switch on in the base class
                        if (!isDisabled)
                        {
                            events.ScheduleEvent(EVENT_CRIMSON_WAKE, urand(3000, 8000));
                        }
                        break;
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim() || isDisabled)
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CRIMSON_WAKE:
                            DoCast(me, SPELL_CRIMSON_WAKE_AOE);
                            events.ScheduleEvent(EVENT_CRIMSON_WAKE, 30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            void DespawnCrimsonWakes()
            {
                std::list<Creature*> l_CrimsonWakesList;
                me->GetCreatureListWithEntryInGrid(l_CrimsonWakesList, NPC_CRIMSON_WAKE, 150.f);

                for (Creature* l_CrimsonWake : l_CrimsonWakesList)
                    l_CrimsonWake->DespawnOrUnsummon();
            }

        private:

        };
};

// Massive Anima Golem = 69699
class npc_dark_animus_massive_anima_golem : public CreatureScript
{
    public:
        npc_dark_animus_massive_anima_golem() : CreatureScript("npc_dark_animus_massive_anima_golem") { }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_dark_animus_massive_anima_golemAI(p_Creature);
        }

        struct npc_dark_animus_massive_anima_golemAI : public npc_dark_animus_golem_baseAI
        {
            npc_dark_animus_massive_anima_golemAI(Creature* p_Creature) : npc_dark_animus_golem_baseAI(p_Creature)
            {
            }            

            void Reset()
            {
                npc_dark_animus_golem_baseAI::Reset();

                me->RemoveAura(SPELL_EMPOWER_GOLEM);
            }

            void DoAction(int32 const action)
            {
                npc_dark_animus_golem_baseAI::DoAction(action);

                switch (action)
                {
                    case ACTION_ACTIVATE_GOLEM:
                    {
                        // it switch on in the base class
                        if (!isDisabled)
                        {
                            events.ScheduleEvent(EVENT_EXPLOSIVE_SLAM, urand(7000, 10000));
                            events.ScheduleEvent(EVENT_MATTER_SWAP, urand(10000, 15000));
                        }
                        break;
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim() || isDisabled)
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_EXPLOSIVE_SLAM:
                            DoCastVictim(SPELL_EXPLOSIVE_SLAM);
                            events.ScheduleEvent(EVENT_EXPLOSIVE_SLAM, urand(15000, 18000));
                            break;
                        case EVENT_MATTER_SWAP:
                            DoCastAOE(SPELL_MATTER_SWAP);
                            events.ScheduleEvent(EVENT_MATTER_SWAP, urand(18000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

        private:

        }; 
};

// Crimson Wake - 69951
class npc_dark_animus_crimson_wake : public CreatureScript
{
    public:
        npc_dark_animus_crimson_wake() : CreatureScript("npc_dark_animus_crimson_wake") { }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_dark_animus_crimson_wakeAI(p_Creature);
        }

        struct npc_dark_animus_crimson_wakeAI : public ScriptedAI
        {
            npc_dark_animus_crimson_wakeAI(Creature* p_Creature) : ScriptedAI(p_Creature) 
            { 
                isMoving = false;
                m_FollowedPlayerGUID = 0;
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            
                me->AddAura(SPELL_CRIMSON_WAKE_SLOW, me);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                if (Unit* l_Target = me->FindNearestPlayer(10.0f))
                {
                    m_FollowedPlayerGUID = l_Target->GetGUID();

                    me->CastSpell(l_Target, SPELL_FIXATED);
                    me->ClearUnitState(UNIT_STATE_CASTING);

                    events.ScheduleEvent(EVENT_CRIMSON_WAKE_MOVE, 3000);
                    events.ScheduleEvent(EVENT_CRIMSON_WAKE_DESPAWN, 24000);
                }
            }

            void DamageDealt(Unit* victim, uint32& damage, DamageEffectType damageType)
            {
                // we apply crimson wake aura in Reset
                // but it should make damage only after moving
                if (!isMoving)
                {
                    damage = 0;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CRIMSON_WAKE_MOVE:
                            BeginMoving();
                            break;
                        case EVENT_CRIMSON_WAKE_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                    }                        
                }
            }

        private:

            void BeginMoving()
            {
                if (!isMoving)
                {
                    isMoving = true;

                    if (Unit* target = sObjectAccessor->GetUnit(*me, m_FollowedPlayerGUID))
                    {
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f);
                    }
                }
            }

        private:

            uint64 m_FollowedPlayerGUID;
            bool isMoving;
        };
};

// Long Ritual Lightning - 137994
class spell_dark_animus_long_ritual_lightning: public SpellScriptLoader
{
    public:
        spell_dark_animus_long_ritual_lightning() : SpellScriptLoader("spell_dark_animus_long_ritual_lightning") { }

        class spell_dark_animus_long_ritual_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_long_ritual_lightning_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (!p_Object)
                        return true;

                    if (p_Object->GetDistance(GetCaster()) > 30.f)
                        return false;

                    return true;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_long_ritual_lightning_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_long_ritual_lightning_SpellScript();
        }
};

// Transfusion (AoE) - 138118
class spell_dark_animus_transfusion_aoe: public SpellScriptLoader
{
    public:
        spell_dark_animus_transfusion_aoe() : SpellScriptLoader("spell_dark_animus_transfusion_aoe") { }

        class spell_dark_animus_transfusion_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_transfusion_aoe_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.remove_if([this](WorldObject* object) -> bool
                {
                    if (!object)
                        return true;

                    Creature* pGolem = object->ToCreature();
                    if (!pGolem)
                        return true;

                    if (pGolem->GetEntry() != NPC_ANIMA_GOLEM ||
                        pGolem->AI()->GetData(DATA_DAMAGED))
                        return true;

                    return false;
                });
            }

            void HandleOnHit()
            {
                if (!GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetHitUnit(), SPELL_TRANSFUSION_ENERGIZE, true);

                if (Creature* pCreature = GetHitUnit()->ToCreature())
                {
                    pCreature->AI()->DoAction(ACTION_ACTIVATE_GOLEM);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_transfusion_aoe_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnHit += SpellHitFn(spell_dark_animus_transfusion_aoe_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_transfusion_aoe_SpellScript();
        }
};

// Transfusion (Searcher) - 138378
class spell_dark_animus_transfusion_searcher: public SpellScriptLoader
{
    public:
        spell_dark_animus_transfusion_searcher() : SpellScriptLoader("spell_dark_animus_transfusion_searcher") { }

        class spell_dark_animus_transfusion_searcher_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_transfusion_searcher_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (targets.empty())
                    return;

                Unit* caster = GetCaster();
                targets.remove_if([this, caster](WorldObject* object) -> bool
                {
                    if (!object)
                        return true;

                    Creature* golem = object->ToCreature();
                    if (!golem)
                        return true;

                    if (golem->GetPower(POWER_ENERGY) >= golem->GetMaxPower(POWER_ENERGY) ||
                        (golem->GetPower(POWER_ENERGY) + GetCaster()->GetAI()->GetData(DATA_ENERGY)) > uint32(golem->GetMaxPower(POWER_ENERGY)))
                        return true;

                    if (golem->AI()->GetData(DATA_DAMAGED))
                        return true;

                    if (golem->GetEntry() == NPC_ANIMA_GOLEM ||
                        golem->GetEntry() == NPC_LARGE_ANIMA_GOLEM ||
                        golem->GetEntry() == NPC_MASSIVE_ANIMA_GOLEM ||
                        golem->GetEntry() == NPC_DARK_ANIMUS)
                        return false;

                    return true;
                });

                if (targets.empty())
                    return;

                // In LFR difficulty, a random golem is chosen.
                targets.sort(JadeCore::DistanceCompareOrderPred(caster));
                WorldObject* target = (*targets.begin());

                targets.clear();
                targets.push_back(target);
            }

            void HandleOnHit()
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (Creature* pGolem = GetCaster()->ToCreature())
                {
                    int32 energy = pGolem->AI()->GetData(DATA_ENERGY);
                    GetHitUnit()->CastCustomSpell(GetHitUnit(), SPELL_ENERGIZE_CUSTOM, &energy, NULL, NULL, true);
                }

                if (Creature* pCreature = GetHitUnit()->ToCreature())
                {
                    pCreature->AI()->DoAction(ACTION_ACTIVATE_GOLEM);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_transfusion_searcher_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnHit += SpellHitFn(spell_dark_animus_transfusion_searcher_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_transfusion_searcher_SpellScript();
        }
};

// Crimson Wake (AOE) - 138541
class spell_dark_animus_crimson_wake_aoe: public SpellScriptLoader
{
    public:
        spell_dark_animus_crimson_wake_aoe() : SpellScriptLoader("spell_dark_animus_crimson_wake_aoe") { }

        class spell_dark_animus_crimson_wake_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_crimson_wake_aoe_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (targets.empty())
                    return;

                JadeCore::Containers::RandomResizeList(targets, 1);
            }

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastCustomSpell(GetHitUnit(), SPELL_CRIMSON_WAKE, NULL, NULL, NULL, NULL, NULL, NULL, true, NULL, NULL, GetCaster()->GetGUID());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_crimson_wake_aoe_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_dark_animus_crimson_wake_aoe_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_crimson_wake_aoe_SpellScript();
        }
};

// Crimson Wake (Slow) - 138482
class spell_dark_animus_crimson_wake_slow: public SpellScriptLoader
{
    public:
        spell_dark_animus_crimson_wake_slow() : SpellScriptLoader("spell_dark_animus_crimson_wake_slow") {}

        class spell_dark_animus_crimson_wake_slow_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dark_animus_crimson_wake_slow_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, SPELL_CRIMSON_WAKE_MEDIUM, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dark_animus_crimson_wake_slow_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dark_animus_crimson_wake_slow_AuraScript();
        }
};

// Crimson Wake (Medium) - 138484
class spell_dark_animus_crimson_wake_medium: public SpellScriptLoader
{
    public:
        spell_dark_animus_crimson_wake_medium() : SpellScriptLoader("spell_dark_animus_crimson_wake_medium") {}

        class spell_dark_animus_crimson_wake_medium_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dark_animus_crimson_wake_medium_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, SPELL_CRIMSON_WAKE_FAST, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dark_animus_crimson_wake_medium_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dark_animus_crimson_wake_medium_AuraScript();
        }
};

// Matter Swap - 138609
class spell_dark_animus_matter_swap: public SpellScriptLoader
{
    public:
        spell_dark_animus_matter_swap() : SpellScriptLoader("spell_dark_animus_matter_swap") { }

        class spell_dark_animus_matter_swap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_matter_swap_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_matter_swap_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_matter_swap_SpellScript();
        }

        class spell_dark_animus_matter_swap_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dark_animus_matter_swap_AuraScript);

            void HandleEffectUpdate(uint32 p_Diff, AuraEffect* p_AurEff)
            {
                Aura* l_MatterSwap = GetAura();
                if (!l_MatterSwap)
                    return;

                AuraEffect* aurEffToDamageOwner = l_MatterSwap->GetEffect(EFFECT_0);
                AuraEffect* aurEffToDamageTarget = p_AurEff;

                uint32 l_ElapsedTime = l_MatterSwap->GetMaxDuration() - l_MatterSwap->GetDuration();

                // See http://fr.wowhead.com/npc=69427#comments:id=1847317
                if (l_ElapsedTime >= 12000)
                {
                    aurEffToDamageOwner->SetAmount(0);
                    aurEffToDamageTarget->SetAmount(100);
                }
                if (l_ElapsedTime >= 9000)
                {
                    aurEffToDamageOwner->SetAmount(25);
                    aurEffToDamageTarget->SetAmount(75);
                }
                if (l_ElapsedTime >= 6000)
                {
                    aurEffToDamageOwner->SetAmount(50);
                    aurEffToDamageTarget->SetAmount(50);
                }
                if (l_ElapsedTime >= 3000)
                {
                    aurEffToDamageOwner->SetAmount(75);
                    aurEffToDamageTarget->SetAmount(25);
                }
                else
                {
                    aurEffToDamageOwner->SetAmount(100);
                    aurEffToDamageTarget->SetAmount(0);
                }
            }

            /*void HandleDispel(DispelInfo* p_DispelData)
            {
                if (Unit* l_MassiveGolem = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        std::list<Player*> l_PlayerList;
                        l_Target->GetPlayerListInGrid(l_PlayerList, 100.f);

                        if (l_PlayerList.empty())
                            return;

                        uint64 l_CasterGuid = l_MassiveGolem->GetGUID();
                        l_PlayerList.remove_if([this, l_CasterGuid](Player* p_Player) -> bool
                        {
                            if (!p_Player || !p_Player->HasAura(SPELL_TARGETED_MATTER_SWAP))
                                return true;

                            if (p_Player->GetAura(SPELL_TARGETED_MATTER_SWAP, l_CasterGuid) == nullptr)
                                return true;

                            return false;
                        });

                        if (l_PlayerList.empty())
                            return;

                        if (Player* l_Player = (*l_PlayerList.begin()))
                        {
                            int32 l_TargetDamage = l_Target->CountPctFromMaxHealth(GetEffect(EFFECT_0)->GetAmount());
                            int32 l_PlayerDamage = l_Player->CountPctFromMaxHealth(GetEffect(EFFECT_1)->GetAmount());

                            if (l_TargetDamage)
                                l_MassiveGolem->CastCustomSpell(l_Target, SPELL_MATTER_SWAP_DAMAGE, &l_TargetDamage, NULL, NULL, true);
                            if (l_PlayerDamage)
                                l_MassiveGolem->CastCustomSpell(l_Player, SPELL_MATTER_SWAP_DAMAGE, &l_PlayerDamage, NULL, NULL, true);

                            Position l_TargetPos, l_PlayerPos;
                            l_Player->GetPosition(&l_PlayerPos);
                            l_Target->GetPosition(&l_TargetPos);

                            l_Player->NearTeleportTo(l_TargetPos.m_positionX, l_TargetPos.m_positionY, l_TargetPos.m_positionZ, l_TargetPos.m_orientation);
                            l_Target->NearTeleportTo(l_PlayerPos.m_positionX, l_PlayerPos.m_positionY, l_PlayerPos.m_positionZ, l_PlayerPos.m_orientation);
                        }
                    }
                }
            }*/

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                Aura const* l_MatterSwap = GetAura();
                if (!l_MatterSwap)
                    return;

                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode != AURA_REMOVE_BY_EXPIRE && l_RemoveMode != AURA_REMOVE_BY_ENEMY_SPELL)
                    return;

                Unit* owner = GetUnitOwner();
                Unit* target = GetPlayerWithMatterSwapTarget(owner);
                if (!owner || !target)
                    return;

                Position ownerPosition, targetPosition;
                owner->GetPosition(&ownerPosition);
                target->GetPosition(&targetPosition);

                int32 damageToOwner = 0;
                int32 damageToTarget = 0;

                if (AuraEffect const* aurEffToDamageOwner = l_MatterSwap->GetEffect(EFFECT_0))
                {
                    damageToOwner = owner->CountPctFromMaxHealth(aurEffToDamageOwner->GetAmount());
                }

                if (AuraEffect const* aurEffToDamageTarget = p_AurEff)
                {
                    damageToTarget = owner->CountPctFromMaxHealth(aurEffToDamageTarget->GetAmount());
                }

                if (damageToOwner)
                {
                    GetCaster()->CastCustomSpell(owner, SPELL_MATTER_SWAP_DAMAGE, &damageToOwner, NULL, NULL, NULL, NULL, NULL, true);
                }

                if (damageToTarget)
                {
                    GetCaster()->CastCustomSpell(target, SPELL_MATTER_SWAP_DAMAGE, &damageToTarget, NULL, NULL, NULL, NULL, NULL, true);
                }

                if (owner->isAlive())
                {
                    owner->NearTeleportTo(targetPosition);
                }

                if (target->isAlive())
                {
                    target->NearTeleportTo(ownerPosition);
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_dark_animus_matter_swap_AuraScript::HandleEffectUpdate, EFFECT_1, SPELL_AURA_DUMMY);
                //OnDispel += AuraDispelFn(spell_dark_animus_matter_swap_AuraScript::HandleDispel);
                OnEffectRemove += AuraEffectRemoveFn(spell_dark_animus_matter_swap_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }

        private:

            Player* GetPlayerWithMatterSwapTarget(Unit* unit)
            {
                Player* player = NULL;
                PlayerMatterSwapCheck checker(unit);
                JadeCore::PlayerLastSearcher<PlayerMatterSwapCheck> searcher(unit, player, checker);
                unit->VisitNearbyWorldObject(100.0f, searcher);
                return player;
            }

            class PlayerMatterSwapCheck
            {
                public:
                    PlayerMatterSwapCheck(WorldObject const* obj) : _obj(obj) {}
                    bool operator()(Player* u)
                    {
                        if (!u->isAlive())
                            return false;

                        if (!_obj->IsWithinDistInMap(u, 100.0f))
                            return false;

                        if (!u->HasAura(SPELL_TARGETED_MATTER_SWAP))
                            return false;

                        return true;
                    }

                private:
                    WorldObject const* _obj;
            };
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dark_animus_matter_swap_AuraScript();
        }
};

// Targeted: Matter Swap - 139919
class spell_dark_animus_targeted_matter_swap: public SpellScriptLoader
{
    public:
        spell_dark_animus_targeted_matter_swap() : SpellScriptLoader("spell_dark_animus_targeted_matter_swap") { }

        class spell_dark_animus_targeted_matter_swap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_targeted_matter_swap_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.sort(JadeCore::DistanceOrderPred(GetCaster()));
                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_targeted_matter_swap_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_targeted_matter_swap_SpellScript();
        }
};

class spell_dark_animus_anima_ring_area_trigger: public SpellScriptLoader
{
    public:
        spell_dark_animus_anima_ring_area_trigger() : SpellScriptLoader("spell_dark_animus_anima_ring_area_trigger") { }

        class spell_dark_animus_anima_ring_area_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_anima_ring_area_trigger_SpellScript);

            void HandleOnHit(SpellEffIndex p_EffIndex)
            {
                if (!GetExplTargetDest())
                    return;

                int32 ringIndex = GetAnimaRingIndex(m_scriptSpellId);
                if (ringIndex < 0 || ringIndex >= MAX_ANIMA_RING_SPELLS)
                    return;

                WorldLocation summonPos = *GetExplTargetDest();

                float angle = (2 * M_PI / MAX_ANIMA_RING_SPELLS) * ringIndex;

                float newX = summonPos.GetPositionX() + ((5.f) * cos(angle));
                float newY = summonPos.GetPositionY() + ((5.f) * sin(angle));

                summonPos.Relocate(newX, newY);

                SetExplTargetDest(summonPos);

                if (Position* dest = GetHitDest())
                {
                    dest->Relocate(newX, newY);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_dark_animus_anima_ring_area_trigger_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_CREATE_AREATRIGGER);
            }

        private:

            int32 GetAnimaRingIndex(uint32 spellId)
            {
                for (uint8 i = 0; i < MAX_ANIMA_RING_SPELLS; ++i)
                {
                    if (animaRingSpells[i] == spellId)
                        return i;
                }

                return -1;
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_anima_ring_area_trigger_SpellScript();
        }
};

// Activation Sequence - 139537
class spell_dark_animus_activation_sequence: public SpellScriptLoader
{
    public:
        spell_dark_animus_activation_sequence() : SpellScriptLoader("spell_dark_animus_activation_sequence") {}

        class spell_dark_animus_activation_sequence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dark_animus_activation_sequence_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Creature* l_DarkAnimus = GetTarget()->ToCreature())
                    l_DarkAnimus->AI()->DoAction(ACTION_ACTIVATE_GOLEM);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_dark_animus_activation_sequence_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dark_animus_activation_sequence_AuraScript();
        }
};

// Siphon Anima - 138644
class spell_dark_animus_siphon_anima: public SpellScriptLoader
{
    public:
        spell_dark_animus_siphon_anima() : SpellScriptLoader("spell_dark_animus_siphon_anima") { }

        class spell_dark_animus_siphon_anima_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_siphon_anima_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (!p_Object)
                        return true;

                    Creature* pCreature = p_Object->ToCreature();
                    if (!pCreature)
                        return true;

                    if (GetCaster()->GetEntry() == pCreature->GetEntry())
                        return true;

                    if (pCreature->GetEntry() != NPC_ANIMA_GOLEM &&
                        pCreature->GetEntry() != NPC_MASSIVE_ANIMA_GOLEM &&
                        pCreature->GetEntry() != NPC_LARGE_ANIMA_GOLEM)
                        return true;

                    if (pCreature->AI()->GetData(DATA_DAMAGED))
                        return true;

                    if (pCreature->GetPower(POWER_ENERGY) <= 0)
                        return true;

                    return false;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_siphon_anima_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_siphon_anima_SpellScript();
        }
};

// Touch of the Animus - 138659
class spell_dark_animus_touch_of_the_animus: public SpellScriptLoader
{
    public:
        spell_dark_animus_touch_of_the_animus() : SpellScriptLoader("spell_dark_animus_touch_of_the_animus") { }

        class spell_dark_animus_touch_of_the_animus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_touch_of_the_animus_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_touch_of_the_animus_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_touch_of_the_animus_SpellScript();
        }
};

// Anima Font - 138691
class spell_dark_animus_anima_font: public SpellScriptLoader
{
    public:
        spell_dark_animus_anima_font() : SpellScriptLoader("spell_dark_animus_anima_font") { }

        class spell_dark_animus_anima_font_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_anima_font_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_anima_font_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_anima_font_SpellScript();
        }
};

// Anima Font (Dummy - trigger missile) - 138694
class spell_dark_animus_anima_font_dummy: public SpellScriptLoader
{
    public:
        spell_dark_animus_anima_font_dummy() : SpellScriptLoader("spell_dark_animus_anima_font_dummy") { }

        class spell_dark_animus_anima_font_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_anima_font_dummy_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, SPELL_ANIMA_FONT_MISSILE, true);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_anima_font_dummy_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_dark_animus_anima_font_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_anima_font_dummy_SpellScript();
        }
};

// FULL POWER - 138734
class spell_dark_animus_full_power: public SpellScriptLoader
{
    public:
        spell_dark_animus_full_power() : SpellScriptLoader("spell_dark_animus_full_power") { }

        class spell_dark_animus_full_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_full_power_SpellScript);

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, SPELL_FULL_POWER_MISSILE, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dark_animus_full_power_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_full_power_SpellScript();
        }
};

// Empower Golem - 138780
class spell_dark_animus_empower_golem: public SpellScriptLoader
{
    public:
        spell_dark_animus_empower_golem() : SpellScriptLoader("spell_dark_animus_empower_golem") { }

        class spell_dark_animus_empower_golem_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dark_animus_empower_golem_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (!p_Object || !p_Object->ToCreature())
                        return true;

                    if (!p_Object->ToCreature()->isAlive())
                        return true;

                    if (p_Object->GetEntry() == NPC_ANIMA_GOLEM ||
                        p_Object->GetEntry() == NPC_LARGE_ANIMA_GOLEM ||
                        p_Object->GetEntry() == NPC_MASSIVE_ANIMA_GOLEM)
                        return false;

                    return true;
                });

                if (p_Targets.empty())
                    return;

                std::list<Unit*> l_TempList;
                for (WorldObject* l_Object : p_Targets)
                    l_TempList.push_back(l_Object->ToUnit());

                l_TempList.sort(JadeCore::HealthPctOrderPred());
                WorldObject* l_Object = (*l_TempList.begin());
                p_Targets.clear();
                p_Targets.push_back(l_Object);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_empower_golem_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_empower_golem_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dark_animus_empower_golem_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dark_animus_empower_golem_SpellScript();
        }
};

class spell_area_dark_animus_anima_ring: public SpellAreaTriggerScript
{
    public:
        spell_area_dark_animus_anima_ring() : SpellAreaTriggerScript("spell_area_dark_animus_anima_ring") {}

        bool OnAddTarget(AreaTrigger* trigger, Unit* target)
        {
            target->AddAura(SPELL_ANIMA_RING_DEBUFF, target);

            trigger->Remove(false);

            return true;
        }
};

void AddSC_boss_dark_animus()
{
    new boss_dark_animus();
    new npc_dark_animus_anima_orb();
    new npc_dark_animus_dark_ritualist();
    new npc_dark_animus_anima_golem();
    new npc_dark_animus_large_anima_golem();
    new npc_dark_animus_massive_anima_golem();
    new npc_dark_animus_crimson_wake();

    new spell_dark_animus_long_ritual_lightning();
    new spell_dark_animus_transfusion_aoe();
    new spell_dark_animus_transfusion_searcher();
    new spell_dark_animus_crimson_wake_aoe();       // 138541
    new spell_dark_animus_crimson_wake_slow();
    new spell_dark_animus_crimson_wake_medium();
    new spell_dark_animus_matter_swap();
    new spell_dark_animus_targeted_matter_swap();
    new spell_dark_animus_anima_ring_area_trigger(); // 136955, 136956, 136957, 136958, 136959, 136960, 138671, 138672, 138673, 138674, 138675, 138676
    new spell_dark_animus_activation_sequence();
    new spell_dark_animus_siphon_anima();
    new spell_dark_animus_touch_of_the_animus();
    new spell_dark_animus_anima_font();
    new spell_dark_animus_anima_font_dummy();
    new spell_dark_animus_full_power();
    new spell_dark_animus_empower_golem();

    new spell_area_dark_animus_anima_ring();        // 136955, 136956, 136957, 136958, 136959, 136960, 138671, 138672, 138673, 138674, 138675, 138676
}

#include "siege_of_orgrimmar.hpp"

enum ScriptedTextRookStonetoe
{
    SAY_ROOK_AGGRO       = 0,
    SAY_ROOK_DEATH       = 1,
    SAY_ROOK_INTRO_1     = 2,
    SAY_ROOK_INTRO_2     = 3,
    SAY_ROOK_INTRO_3     = 4,
    SAY_ROOK_KILL        = 5,
    SAY_ROOK_KICK        = 6,
    SAY_ROOK_MEASURES    = 7,
    SAY_ROOK_LOTUS       = 8,
};

enum ScriptedTextHeSoftfoot
{
    SAY_HE_AGGRO       = 0,
    SAY_HE_MEASURES    = 1,
    SAY_HE_LOTUS       = 2,
};

enum ScriptedTextSunTenderHeart
{
    SAY_SUN_AGGRO       = 0,
    SAY_SUN_DEATH       = 1,
    SAY_SUN_INTRO_1     = 2,
    SAY_SUN_KILL        = 3,
    SAY_SUN_RESET       = 4,
    SAY_SUN_CALAMITY    = 5,
    SAY_SUN_MEASURES    = 6,
    SAY_SUN_LOTUS       = 7,
};

enum Spells
{
    SPELL_ZERO_POWER                    = 96301,
    SPELL_BOND_OF_THE_GOLDEN_LOTUS      = 143497,

    // Rook Stonetoe
    SPELL_VENGEFUL_STRIKES              = 144396,
    SPELL_VENGEFUL_STRIKES_DMG          = 144397,

    SPELL_CORRUPTED_BREW_AOE            = 143019,
    SPELL_CORRUPTED_BREW_MISSILE_1      = 143021,
    SPELL_CORRUPTED_BREW_MISSILE_2      = 145608,
    SPELL_CORRUPTED_BREW_MISSILE_3      = 145609,
    SPELL_CORRUPTED_BREW_MISSILE_4      = 145610,
    SPELL_CORRUPTED_BREW_MISSILE_5      = 145611,
    SPELL_CORRUPTED_BREW_MISSILE_6      = 145612,
    SPELL_CORRUPTED_BREW_MISSILE_7      = 145615,
    SPELL_CORRUPTED_BREW_MISSILE_8      = 145617,
    SPELL_CORRUPTED_BREW_DMG            = 143023,
    SPELL_CORRUPTED_BREW_DUMMY          = 145605, // ?

    SPELL_CLASH_AOE                     = 143027,
    SPELL_CLASH_JUMP                    = 143028, // creature
    SPELL_CLASH_CHARGE                  = 143030, // player

    SPELL_CORRUPTION_KICK               = 143007,
    SPELL_CORRUPTION_DMG                = 143009,
    SPELL_CORRUPTION_PERIODIC           = 143010,

    SPELL_MISERY_SORROW_AND_GLOOM       = 143955,
    SPELL_MISERY_SORROW_AND_GLOOM_JUMP_1    = 143946,
    SPELL_MISERY_SORROW_AND_GLOOM_JUMP_2    = 143948,
    SPELL_MISERY_SORROW_AND_GLOOM_JUMP_3    = 143949,

    SPELL_SHARED_TORMENT                = 145655,

    SPELL_DEFILED_GROUND                = 143961,
    SPELL_DEFILED_GROUND_AREATRIGGER    = 143960, // spell does not exists
    SPELL_DEFILED_GROUND_AURA           = 143959,
    SPELL_DEFILED_GROUND_DMG            = 144357,

    SPELL_INFERNO_STRIKE                = 143962,

    SPELL_CORRUPTION_SHOCK_AOE          = 143958,
    SPELL_CORRUPTION_SHOCK_MISSILE      = 144020,
    SPELL_CORRUPTION_SHOCK_DMG          = 144018,

    // He StoneFoot
    SPELL_SHADOWSTEP_AOE                = 143050,
    SPELL_SHADOWSTEP                    = 143048,
    SPELL_GARROTE                       = 143198,

    SPELL_GOUGE                         = 143330,
    SPELL_GOUGE_DMG                     = 143301,
    SPELL_GOUGE_KNOCKBACK               = 143331,

    SPELL_NOXIOUS_POISON                = 143225,
    SPELL_NOXIOUS_POISON_AREATRIGGER    = 143235,
    SPELL_NOXIOUS_POISON_MISSILE_1      = 143245,
    SPELL_NOXIOUS_POISON_MISSILE_2      = 143276,
    SPELL_NOXIOUS_POISON_AURA           = 143239, // dmg
    SPELL_NOXIOUS_POISON_DMG            = 144367,

    SPELL_INSTANT_POISON                = 143210,
    SPELL_INSTANT_POISON_DMG            = 143224,

    SPELL_MARK_OF_ANGUISH_JUMP          = 143808,
    SPELL_MARK_OF_ANGUISH_AURA_1        = 143812, // on boss
    SPELL_MARK_OF_ANGUISH_AOE           = 143822,
    SPELL_MARK_OF_ANGUISH_AURA_2        = 143840, // on players
    SPELL_MARK_OF_ANGUISH_DUMMY         = 143842,
    SPELL_MARK_OF_ANGUISH_DMG           = 144365,
    SPELL_SHADOW_WEAKNESS               = 144079,
    SPELL_SHADOW_WEAKNESS_AOE           = 144081, // ?
    SPELL_SHADOW_WEAKNESS_DEBUFF        = 144176,
    SPELL_DEBILITATION                  = 147383,

    // Sun TenderHeart
    SPELL_SHA_SEAR                      = 143423,
    SPELL_SHA_SEAR_DMG                  = 143424,

    SPELL_SHADOW_WORD_BANE_AOE_1        = 143446,
    SPELL_SHADOW_WORD_BANE_AOE_2        = 143438,
    SPELL_SHADOW_WORD_BANE_DUMMY_1      = 143443,
    SPELL_SHADOW_WORD_BANE_DUMMY_2      = 143879,
    SPELL_SHADOW_WORD_BANE_DMG          = 143434,

    SPELL_CALAMITY                      = 143491,
    SPELL_CALAMITY_DMG                  = 143493,
    SPELL_CALAMITY_DUMMY                = 143544, // ?

    SPELL_DARK_MEDITATION_AREATRIGGER   = 143546,
    SPELL_MEDITATIVE_FIELD              = 143564,
    SPELL_DARK_MEDITATION_DUMMY_1       = 143649, // ? no visual
    SPELL_DARK_MEDITATION_JUMP_1        = 143728, // ? for adds ?
    SPELL_DARK_MEDITATION_JUMP_2        = 143730, // ? for adds ?
    SPELL_DARK_MEDITATION_PERIODIC      = 143745,
    SPELL_DARK_MEDITATION_DUMMY_2       = 143843, // for boss
    SPELL_DARK_MEDITATION_DMG           = 143559,

    SPELL_MANIFEST_DESPAIR              = 143746,
    SPELL_MANIFEST_DESPERATION          = 144504,
};

enum Adds
{
    NPC_EMBODIED_MISERY         = 71476,
    NPC_EMBODIED_SORROW         = 71481,
    NPC_EMBODIED_GLOOM          = 71477,

    NPC_EMBODIED_ANGUISH        = 71478,

    NPC_EMBODIED_DESPAIR        = 71474,
    NPC_EMBODIED_DESPERATION    = 71482,
    NPC_DESPAIR_SPAWN           = 71712,
    NPC_DESPERATION_SPAWN       = 71993,
};

enum Events
{
    EVENT_BOND_OF_GOLDEN_LOTUS_END  = 1,

    EVENT_VENGEFUL_STRIKES,
    EVENT_CORRUPTED_BREW,
    EVENT_CLASH,

    EVENT_AGGRO,
    EVENT_SYNC_HEALTH,
    EVENT_DEFILED_GROUND,
    EVENT_INFERNO_STRIKE,
    EVENT_CORRUPTION_SHOCK,

    EVENT_GOUGE,
    EVENT_SHADOWSTEP,
    EVENT_POISON,

    EVENT_MARK_OF_ANGUISH,
    EVENT_MARK_OF_ANGUISH_TARGET_CHANGED,
    EVENT_CHECK_MARK_OF_ANGUISH_TARGET,

    EVENT_SHA_SEAR,
    EVENT_CALAMITY,
    EVENT_SHADOW_WORD_BANE,
    EVENT_DARK_MEDITATION,
    EVENT_DARK_MEDITATION_DMG,
    EVENT_MANIFEST,
};

enum Actions
{
    DATA_CASTING_LOW_HEALTH = 1,
    GUID_MARK_OF_ANGUISH,
};

const Position embodiedStonetoePos[6] = 
{
    {1208.065918f, 1020.084290f, 443.482819f, 6.25f}, // in air
    {1224.071533f, 1030.121948f, 442.441803f, 1.01f}, // in air
    {1216.354248f, 1041.308716f, 442.246277f, 2.62f}, // in air
    {1224.833130f, 1041.106079f, 417.891165f, 3.38f}, // on ground
    {1199.534790f, 1033.528931f, 418.082357f, 3.96f}, // on ground
    {1218.366211f, 1011.913879f, 418.074331f, 5.65f}  // on ground
};

const Position embodiedDespairPos = {1218.507202f, 1056.175659f, 417.490377f, 4.20f};
const Position embodiedDesperationPos = {1209.596313f, 1002.381042f, 418.073019f, 1.54f};

struct fallen_protectorAI : public BossAI
{
    fallen_protectorAI(Creature* creature) : BossAI(creature, DATA_FALLEN_PROTECTORS)
    {
        ApplyAllImmunities(true);

        me->setActive(true);
    }

    void Reset() override
    {
        _Reset();

        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_DEFENSIVE);

        m_IsCastingLowHealth = false;
        m_SummonedNpcsForMeasures.clear();
        m_Phase = 0;
        m_IsInMeasure = false;
    }

    void EnterEvadeMode() override
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

        BossAI::EnterEvadeMode();
    }

    void EnterCombat(Unit* who) override
    {
        bool isFirstAggro = true;

        std::list<Creature*> otherBosses;
        GetOtherBosses(otherBosses);

        for (Creature* pBoss : otherBosses)
        {
            if (pBoss && !pBoss->isInCombat())
            {
                DoZoneInCombat(pBoss);
            }
            else
            {
                isFirstAggro = false;
            }
        }

        if (isFirstAggro)
        {
            DoFirstAggro();
        }

        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
    }

    uint32 GetData(uint32 type) override
    {
        if (type == DATA_CASTING_LOW_HEALTH)
        {
            return m_IsCastingLowHealth ? 1 : 0;
        }

        return 0;
    }

    void DamageTaken(Unit* who, uint32 & damage) override
    {
        if (me->GetHealth() <= damage)
        {
            if (IsCastingLowHealth())
            {
                damage = me->GetHealth() - 1;
            }
            else 
            {
                if (!IsOthersCastingLowHealth())
                {
                    DoBoundOfGoldenLotus();

                    SetCastingLowHealth(true);

                    damage = me->GetHealth() - 1;

                    me->CastStop();

                    DoCast(me, SPELL_BOND_OF_THE_GOLDEN_LOTUS);

                    events.ScheduleEvent(EVENT_BOND_OF_GOLDEN_LOTUS_END, 15000);
                }
                else
                {
                    KillOthers();
                }
            }
        }
    }

    void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
    {
        if (IsInMeasures())
        {
            RemoveMeasureNpc(summon->GetEntry());

            if (!IsAnyMeasureNpc())
            {
                EndMeasures();
            }
        }
    }

    void JustDied(Unit* who) override
    {
        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

        _JustDied();
    }

    bool CheckPhase()
    {
        if ((m_Phase == 0 && me->GetHealthPct() < 66.0f) ||
            (m_Phase == 2 && me->GetHealthPct() < 33.0f))
        {
            ++m_Phase;

            BeginMeasures();

            return true;
        }

        return false;
    }

protected:

    virtual void DoFirstAggro() = 0;
    virtual void DoBoundOfGoldenLotus() = 0;
    virtual void DoBeginMeasures() = 0;
    virtual void DoEndMeasures() = 0;

    bool IsCastingLowHealth() const { return m_IsCastingLowHealth; }
    void SetCastingLowHealth(bool value) { m_IsCastingLowHealth = value; }

    void AddMeasureNpc(uint32 entry) { m_SummonedNpcsForMeasures.insert(entry); }
    void RemoveMeasureNpc(uint32 entry) { m_SummonedNpcsForMeasures.erase(entry); }
    bool IsAnyMeasureNpc() const { return !m_SummonedNpcsForMeasures.empty(); }

    bool IsInMeasures() const { return m_IsInMeasure; }
    void SetInMeasures(bool value) { m_IsInMeasure = value; }

private:

    Creature* GetRookStoneToe()
    {
        return instance->instance->GetCreature(instance->GetData64(DATA_ROOK_STONETOE));
    }

    Creature* GetHeSoftFoot()
    {
        return instance->instance->GetCreature(instance->GetData64(DATA_HE_SOFTFOOT));
    }

    Creature* GetSunTenderHeart()
    {
        return instance->instance->GetCreature(instance->GetData64(DATA_SUN_TENDERHEART));
    }

    void GetOtherBosses(std::list<Creature*>& creatures)
    {
        Creature* pRook = GetRookStoneToe();
        Creature* pHe = GetHeSoftFoot();
        Creature* pSun = GetSunTenderHeart();

        if (pRook && pRook->GetEntry() != me->GetEntry())
            creatures.push_back(pRook);

        if (pHe && pHe->GetEntry() != me->GetEntry())
            creatures.push_back(pHe);

        if (pSun && pSun->GetEntry() != me->GetEntry())
            creatures.push_back(pSun);
    }

    bool IsOthersCastingLowHealth()
    {
        if (Creature* pHe = GetHeSoftFoot())
        {
            if (pHe->AI()->GetData(DATA_CASTING_LOW_HEALTH) == 0)
            {
                return false;
            }
        }

        if (Creature* pSun = GetSunTenderHeart())
        {
            if (pSun->AI()->GetData(DATA_CASTING_LOW_HEALTH) == 0)
            {
                return false;
            }
        }

        return true;
    }

    void KillOthers()
    {
        std::list<Creature*> otherBosses;
        GetOtherBosses(otherBosses);

        for (Creature* bBoss : otherBosses)
        {
            if (bBoss)
            {
                if (Unit* victim = bBoss->getVictim())
                {
                    victim->Kill(bBoss);
                }
            }
        }
    }

    void BeginMeasures()
    {
        SetInMeasures(true);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->AttackStop();
        me->SetReactState(REACT_PASSIVE);

        DoBeginMeasures();
    }

    void EndMeasures()
    {
        SetInMeasures(false);

        ++m_Phase;
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_AGGRESSIVE);

        DoEndMeasures();
    }

private:

    bool m_IsCastingLowHealth;
    std::set<uint32 /*entry*/> m_SummonedNpcsForMeasures;
    uint8 m_Phase;
    bool m_IsInMeasure;

};

class boss_rook_stonetoe : public CreatureScript
{
    public:
        boss_rook_stonetoe() : CreatureScript("boss_rook_stonetoe") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_rook_stonetoeAI(creature);
        }

        struct boss_rook_stonetoeAI : public fallen_protectorAI
        {
            boss_rook_stonetoeAI(Creature* creature) : fallen_protectorAI(creature)
            {
            }

            void Reset() override
            {
                fallen_protectorAI::Reset();
            }

            void EnterCombat(Unit* attacker) override
            {
                fallen_protectorAI::EnterCombat(attacker);

                events.ScheduleEvent(EVENT_VENGEFUL_STRIKES, 8000);
                events.ScheduleEvent(EVENT_CORRUPTED_BREW, 18000);
                events.ScheduleEvent(EVENT_CLASH, 45000);

                instance->SetBossState(DATA_FALLEN_PROTECTORS, IN_PROGRESS);
                DoZoneInCombat();
            }

            void KilledUnit(Unit* who) override
            {
                if (who->IsPlayer())
                {
                    Talk(SAY_ROOK_KILL);
                }
            }

            void JustDied(Unit* who) override
            {
                fallen_protectorAI::JustDied(who);

                Talk(SAY_ROOK_DEATH);
            }

            void MovementInform(uint32 type, uint32 id) override
            {
                if (id == EVENT_JUMP)
                {
                    Talk(SAY_ROOK_KICK);

                    me->GetMotionMaster()->MovementExpired(false);
                    DoCast(me, SPELL_CORRUPTION_KICK);

                    // reenable moving to the target
                    me->ClearUnitState(UNIT_STATE_MELEE_ATTACKING);
                }
            }

            void UpdateAI(const uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (CheckPhase())
                {
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOND_OF_GOLDEN_LOTUS_END:
                            SetCastingLowHealth(false);
                            break;
                        case EVENT_VENGEFUL_STRIKES:
                            DoCastVictim(SPELL_VENGEFUL_STRIKES);
                            events.ScheduleEvent(EVENT_VENGEFUL_STRIKES, 43000);
                            break;
                        case EVENT_CORRUPTED_BREW:
                            DoCastAOE(SPELL_CORRUPTED_BREW_AOE);
                            events.ScheduleEvent(EVENT_CORRUPTED_BREW, 11000);
                            break;
                        case EVENT_CLASH:
                            DoCastAOE(SPELL_CLASH_AOE);
                            events.ScheduleEvent(EVENT_CLASH, 46000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        protected:

            virtual void DoFirstAggro() override
            {
                Talk(SAY_ROOK_AGGRO);
            }

            virtual void DoBoundOfGoldenLotus() override
            {
                Talk(SAY_ROOK_LOTUS);
            }

            virtual void DoBeginMeasures() override
            {
                Talk(SAY_ROOK_MEASURES);

                events.CancelEvent(EVENT_VENGEFUL_STRIKES);
                events.CancelEvent(EVENT_CORRUPTED_BREW);
                events.CancelEvent(EVENT_CLASH);

                me->CastSpell(me, SPELL_MISERY_SORROW_AND_GLOOM, true);

                if (Creature* pMisery = me->SummonCreature(NPC_EMBODIED_MISERY, embodiedStonetoePos[0]))
                {
                    AddMeasureNpc(pMisery->GetEntry());
                    pMisery->GetMotionMaster()->MoveJump(embodiedStonetoePos[3].GetPositionX(), embodiedStonetoePos[3].GetPositionY(), embodiedStonetoePos[3].GetPositionZ() + 0.1f, 40.0f, 20.0f);
                }
                if (Creature* pSorrow = me->SummonCreature(NPC_EMBODIED_SORROW, embodiedStonetoePos[1]))
                {
                    AddMeasureNpc(pSorrow->GetEntry());
                    pSorrow->GetMotionMaster()->MoveJump(embodiedStonetoePos[4].GetPositionX(), embodiedStonetoePos[4].GetPositionY(), embodiedStonetoePos[4].GetPositionZ() + 0.1f, 40.0f, 20.0f);
                }
                if (Creature* pGloom = me->SummonCreature(NPC_EMBODIED_GLOOM, embodiedStonetoePos[2]))
                {
                    AddMeasureNpc(pGloom->GetEntry());
                    pGloom->GetMotionMaster()->MoveJump(embodiedStonetoePos[5].GetPositionX(), embodiedStonetoePos[5].GetPositionY(), embodiedStonetoePos[5].GetPositionZ() + 0.1f, 40.0f, 20.0f);
                }
            }

            virtual void DoEndMeasures() override
            {
                me->RemoveAura(SPELL_MISERY_SORROW_AND_GLOOM);

                events.ScheduleEvent(EVENT_VENGEFUL_STRIKES, 7500);
                events.ScheduleEvent(EVENT_CORRUPTED_BREW, 17000);
                events.ScheduleEvent(EVENT_CLASH, 45000);
            }
        };  
};

class boss_he_softfoot : public CreatureScript
{
    public:
        boss_he_softfoot() : CreatureScript("boss_he_softfoot") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_he_softfootAI(creature);
        }

        struct boss_he_softfootAI : public fallen_protectorAI
        {
            boss_he_softfootAI(Creature* creature) : fallen_protectorAI(creature)
            {

            }

            void Reset() override
            {
                fallen_protectorAI::Reset();

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GARROTE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_OF_ANGUISH_AURA_2);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SHADOW_WEAKNESS_DEBUFF);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEBILITATION);

                me->RemoveAllAreasTrigger();
            }

            void EnterCombat(Unit* attacker) override
            {
                fallen_protectorAI::EnterCombat(attacker);

                events.ScheduleEvent(EVENT_GOUGE, 25000);
                events.ScheduleEvent(EVENT_SHADOWSTEP, 9000);
                events.ScheduleEvent(EVENT_POISON, urand(5000, 15000));

                instance->SetBossState(DATA_FALLEN_PROTECTORS, IN_PROGRESS);
                DoZoneInCombat();
            }

            void JustDied(Unit* who) override
            {
                fallen_protectorAI::JustDied(who);

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GARROTE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_OF_ANGUISH_AURA_2);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SHADOW_WEAKNESS_DEBUFF);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEBILITATION);

                me->RemoveAllAreasTrigger();
            }

            void UpdateAI(const uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (CheckPhase())
                {
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOND_OF_GOLDEN_LOTUS_END:
                            SetCastingLowHealth(false);
                            break;
                        case EVENT_GOUGE:
                            DoCastVictim(SPELL_GOUGE);
                            events.ScheduleEvent(EVENT_GOUGE, 30000);
                            break;
                        case EVENT_SHADOWSTEP:
                            DoCastAOE(SPELL_SHADOWSTEP_AOE);
                            events.ScheduleEvent(EVENT_SHADOWSTEP, 30000);
                            break;
                        case EVENT_POISON:
                        {
                            uint32 nextPoisonSpell = urand(0, 1) ? SPELL_INSTANT_POISON : SPELL_NOXIOUS_POISON;
                            uint32 previousPoisonSpell = nextPoisonSpell == SPELL_INSTANT_POISON ? SPELL_NOXIOUS_POISON : SPELL_INSTANT_POISON;

                            DoCast(me, nextPoisonSpell);
                            me->RemoveAura(previousPoisonSpell);
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        protected:

            virtual void DoFirstAggro() override
            {
                Talk(SAY_HE_AGGRO);
            }

            virtual void DoBoundOfGoldenLotus() override
            {
                Talk(SAY_HE_LOTUS);
            }

            virtual void DoBeginMeasures() override
            {
                Talk(SAY_HE_MEASURES);

                me->RemoveAreaTrigger(SPELL_NOXIOUS_POISON_AREATRIGGER);

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GARROTE);

                me->RemoveAura(SPELL_NOXIOUS_POISON);
                me->RemoveAura(SPELL_INSTANT_POISON);

                events.CancelEvent(EVENT_GOUGE);
                events.CancelEvent(EVENT_SHADOWSTEP);
                events.CancelEvent(EVENT_POISON);

                me->CastSpell(me, SPELL_MARK_OF_ANGUISH_AURA_1, true);

                if (Creature* pAnguish = me->SummonCreature(NPC_EMBODIED_ANGUISH, embodiedStonetoePos[0]))
                {
                    AddMeasureNpc(pAnguish->GetEntry());
                    pAnguish->GetMotionMaster()->MoveJump(embodiedStonetoePos[3].GetPositionX(), embodiedStonetoePos[3].GetPositionY(), embodiedStonetoePos[3].GetPositionZ() + 0.1f, 40.0f, 20.0f);
                }
            }

            virtual void DoEndMeasures() override
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_OF_ANGUISH_AURA_2);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SHADOW_WEAKNESS_DEBUFF);

                me->RemoveAura(SPELL_MARK_OF_ANGUISH_AURA_1);

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_AGGRESSIVE);

                events.ScheduleEvent(EVENT_SHADOWSTEP, 800);
                events.ScheduleEvent(EVENT_GOUGE, 23000);
                events.ScheduleEvent(EVENT_POISON, urand(5000, 15000));
            }
        };  
};

class boss_sun_tenderheart : public CreatureScript
{
    public:
        boss_sun_tenderheart() : CreatureScript("boss_sun_tenderheart") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sun_tenderheartAI(creature);
        }

        struct boss_sun_tenderheartAI : public fallen_protectorAI
        {
            boss_sun_tenderheartAI(Creature* creature) : fallen_protectorAI(creature)
            {
                // Enable to interrupt Sha Sear
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, false);
            }

            void Reset() override
            {
                fallen_protectorAI::Reset();

                DespawnCreaturesInArea(NPC_DESPAIR_SPAWN, me);
                DespawnCreaturesInArea(NPC_DESPERATION_SPAWN, me);

                me->RemoveAllAreasTrigger();
            }

            void EnterCombat(Unit* attacker) override
            {
                fallen_protectorAI::EnterCombat(attacker);

                events.ScheduleEvent(EVENT_SHA_SEAR, urand(500, 2000));
                events.ScheduleEvent(EVENT_SHADOW_WORD_BANE, 15000);
                events.ScheduleEvent(EVENT_CALAMITY, 31000);

                instance->SetBossState(DATA_FALLEN_PROTECTORS, IN_PROGRESS);
                DoZoneInCombat();
            }

            void AttackStart(Unit* who) override
            {
                if (me->Attack(who, true))
                    DoStartNoMovement(who);
            }

            void EnterEvadeMode() override
            {
                fallen_protectorAI::EnterEvadeMode();

                Talk(SAY_SUN_RESET);
            }

            void KilledUnit(Unit* who) override
            {
                if (who->IsPlayer())
                {
                    Talk(SAY_SUN_KILL);
                }
            }

            void JustDied(Unit* who) override
            {
                fallen_protectorAI::JustDied(who);

                Talk(SAY_SUN_DEATH);

                DespawnCreaturesInArea(NPC_DESPAIR_SPAWN, me);
                DespawnCreaturesInArea(NPC_DESPERATION_SPAWN, me);

                me->RemoveAllAreasTrigger();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (CheckPhase())
                {
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOND_OF_GOLDEN_LOTUS_END:
                            SetCastingLowHealth(false);
                            break;
                        case EVENT_SHA_SEAR:
                            DoCastAOE(SPELL_SHA_SEAR);
                            events.ScheduleEvent(EVENT_SHA_SEAR, urand(5000, 7000));
                            break;
                        case EVENT_SHADOW_WORD_BANE:
                            DoCastAOE(SPELL_SHADOW_WORD_BANE_AOE_1);
                            events.ScheduleEvent(EVENT_SHADOW_WORD_BANE, 25000);
                            break;
                        case EVENT_CALAMITY:
                            Talk(SAY_SUN_CALAMITY);
                            DoCastAOE(SPELL_CALAMITY);
                            events.ScheduleEvent(EVENT_CALAMITY, 39000);
                            break;
                        case EVENT_DARK_MEDITATION:
                            DarkMeditation();
                            break;
                        case EVENT_DARK_MEDITATION_DMG:
                            DoCastAOE(SPELL_DARK_MEDITATION_DMG, true);
                            events.ScheduleEvent(EVENT_DARK_MEDITATION_DMG, 500);
                            break;
                        default:
                            break;
                    }
                }
            }

        protected:

            virtual void DoFirstAggro() override
            {
                Talk(SAY_SUN_AGGRO);
            }

            virtual void DoBoundOfGoldenLotus() override
            {
                Talk(SAY_SUN_LOTUS);
            }

            virtual void DoBeginMeasures() override
            {
                events.CancelEvent(EVENT_SHA_SEAR);
                events.CancelEvent(EVENT_CALAMITY);
                events.CancelEvent(EVENT_SHADOW_WORD_BANE);

                events.ScheduleEvent(EVENT_DARK_MEDITATION, 1);
            }

            virtual void DoEndMeasures() override
            {
                me->RemoveAura(SPELL_DARK_MEDITATION_DUMMY_2);
                me->RemoveAura(SPELL_DARK_MEDITATION_AREATRIGGER);
                me->RemoveAreaTrigger(SPELL_DARK_MEDITATION_AREATRIGGER);

                me->SetReactState(REACT_AGGRESSIVE);

                events.ScheduleEvent(EVENT_SHA_SEAR, urand(1000, 3000));
                events.ScheduleEvent(EVENT_SHADOW_WORD_BANE, 10000);
                events.ScheduleEvent(EVENT_CALAMITY, urand(2000, 39000));
            }

            void DarkMeditation()
            {
                Talk(SAY_SUN_MEASURES);

                me->AddAura(SPELL_DARK_MEDITATION_DUMMY_2, me);
                DoCast(me, SPELL_DARK_MEDITATION_AREATRIGGER);

                if (Creature* pDespair = me->SummonCreature(NPC_EMBODIED_DESPAIR, embodiedDespairPos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000))
                {
                    AddMeasureNpc(pDespair->GetEntry());
                }
                
                if (Creature* pDesperation = me->SummonCreature(NPC_EMBODIED_DESPERATION, embodiedDesperationPos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000))
                {
                    AddMeasureNpc(pDesperation->GetEntry());
                }
            
                events.ScheduleEvent(EVENT_DARK_MEDITATION_DMG, 500);
            }
        };  
};

class npc_rook_stonetoe_embodied_misery : public CreatureScript
{
    public:
        npc_rook_stonetoe_embodied_misery() : CreatureScript("npc_rook_stonetoe_embodied_misery") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_rook_stonetoe_embodied_miseryAI(creature);
        }

        struct npc_rook_stonetoe_embodied_miseryAI : public ScriptedAI
        {
            npc_rook_stonetoe_embodied_miseryAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    me->AddAura(SPELL_SHARED_TORMENT, me);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_AGGRO, 2000);
                events.ScheduleEvent(EVENT_DEFILED_GROUND, 9000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SYNC_HEALTH, 2000);
            }

            void JustDied(Unit* killer)
            {
                events.Reset();

                me->DespawnOrUnsummon(1000);
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
                        case EVENT_AGGRO:
                            me->SetReactState(REACT_AGGRESSIVE);
                            break;
                        case EVENT_SYNC_HEALTH:
                            SyncHealth();
                            events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
                            break;
                        case EVENT_DEFILED_GROUND:
                            DoCastVictim(SPELL_DEFILED_GROUND);
                            events.ScheduleEvent(EVENT_DEFILED_GROUND, 10500);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            void SyncHealth()
            {
                Creature* pSorrow = me->FindNearestCreature(NPC_EMBODIED_SORROW, 200.0f);
                Creature* pGloom = me->FindNearestCreature(NPC_EMBODIED_GLOOM, 200.0f);

                uint32 health = me->GetHealth();
                if (pSorrow && pSorrow->isAlive())
                    health = std::min(health, pSorrow->GetHealth());

                if (pGloom && pGloom->isAlive())
                    health = std::min(health, pGloom->GetHealth());

                if (health > 0 && me->GetHealth() != health)
                {
                    me->SetHealth(health);
                }
            }
        };
};

class npc_rook_stonetoe_embodied_sorrow : public CreatureScript
{
    public:
        npc_rook_stonetoe_embodied_sorrow() : CreatureScript("npc_rook_stonetoe_embodied_sorrow") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_rook_stonetoe_embodied_sorrowAI(creature);
        }

        struct npc_rook_stonetoe_embodied_sorrowAI : public ScriptedAI
        {
            npc_rook_stonetoe_embodied_sorrowAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    me->AddAura(SPELL_SHARED_TORMENT, me);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_AGGRO, 2000);
                events.ScheduleEvent(EVENT_INFERNO_STRIKE, 7000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SYNC_HEALTH, 2000);
            }

            void JustDied(Unit* killer)
            {
                events.Reset();

                me->DespawnOrUnsummon(1000);
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
                        case EVENT_AGGRO:
                            me->SetReactState(REACT_AGGRESSIVE);
                            break;
                        case EVENT_SYNC_HEALTH:
                            SyncHealth();
                            events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
                            break;
                        case EVENT_INFERNO_STRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                DoCast(target, SPELL_INFERNO_STRIKE);
                            }
                            events.ScheduleEvent(EVENT_INFERNO_STRIKE, 10000);
                            break;
                    }
                }
            }

        private:

            void SyncHealth()
            {
                Creature* pMisery = me->FindNearestCreature(NPC_EMBODIED_MISERY, 200.0f);
                Creature* pGloom = me->FindNearestCreature(NPC_EMBODIED_GLOOM, 200.0f);

                uint32 health = me->GetHealth();
                if (pMisery && pMisery->isAlive())
                    health = std::min(health, pMisery->GetHealth());

                if (pGloom && pGloom->isAlive())
                    health = std::min(health, pGloom->GetHealth());

                if (health > 0 && me->GetHealth() != health)
                {
                    me->SetHealth(health);
                }
            }
        };
};

class npc_rook_stonetoe_embodied_gloom : public CreatureScript
{
    public:
        npc_rook_stonetoe_embodied_gloom() : CreatureScript("npc_rook_stonetoe_embodied_gloom") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_rook_stonetoe_embodied_gloomAI(creature);
        }

        struct npc_rook_stonetoe_embodied_gloomAI : public ScriptedAI
        {
            npc_rook_stonetoe_embodied_gloomAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, false);

                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                events.Reset();

                if (IsHeroic())
                    me->AddAura(SPELL_SHARED_TORMENT, me);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_AGGRO, 2000);
                events.ScheduleEvent(EVENT_CORRUPTION_SHOCK, 1000);
                if (IsHeroic())
                    events.ScheduleEvent(EVENT_SYNC_HEALTH, 2000);
            }

            void JustDied(Unit* killer)
            {
                events.Reset();

                me->DespawnOrUnsummon(1000);
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
                        case EVENT_AGGRO:
                            me->SetReactState(REACT_AGGRESSIVE);
                            break;
                        case EVENT_SYNC_HEALTH:
                            SyncHealth();
                            events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
                            break;
                        case EVENT_CORRUPTION_SHOCK:
                            DoCastAOE(SPELL_CORRUPTION_SHOCK_AOE);
                            events.ScheduleEvent(EVENT_CORRUPTION_SHOCK, urand(3000, 5000));
                            break;
                    }
                }
            }

        private:

            void SyncHealth()
            {
                Creature* pMisery = me->FindNearestCreature(NPC_EMBODIED_MISERY, 200.0f);
                Creature* pSorrow = me->FindNearestCreature(NPC_EMBODIED_SORROW, 200.0f);

                uint32 health = me->GetHealth();
                if (pMisery && pMisery->isAlive())
                    health = std::min(health, pMisery->GetHealth());

                if (pSorrow && pSorrow->isAlive())
                    health = std::min(health, pSorrow->GetHealth());

                if (health > 0 && me->GetHealth() != health)
                {
                    me->SetHealth(health);
                }
            }
        };
};

class npc_he_softfoot_embodied_anguish : public CreatureScript
{
    public:
        npc_he_softfoot_embodied_anguish() : CreatureScript("npc_he_softfoot_embodied_anguish") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_he_softfoot_embodied_anguishAI(creature);
        }

        struct npc_he_softfoot_embodied_anguishAI : public ScriptedAI
        {
            npc_he_softfoot_embodied_anguishAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
            }

            void Reset()
            {
                events.Reset();

                targetGUID = 0;
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MARK_OF_ANGUISH, 1000);
            }

            void SetGUID(uint64 guid, int32 id)
            {
                if (id == GUID_MARK_OF_ANGUISH)
                {
                    // if it is not first targetting
                    if (targetGUID != 0)
                    {
                        DoCastAOE(SPELL_SHADOW_WEAKNESS_AOE, true);
                    }

                    targetGUID = guid;
                    
                    events.CancelEvent(EVENT_CHECK_MARK_OF_ANGUISH_TARGET);
                    events.ScheduleEvent(EVENT_MARK_OF_ANGUISH_TARGET_CHANGED, 500);
                }
            }

            void JustDied(Unit* killer)
            {
                events.Reset();

                me->DespawnOrUnsummon(1000);
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
                        case EVENT_MARK_OF_ANGUISH:
                            DoCastAOE(SPELL_MARK_OF_ANGUISH_AOE);
                            break;
                        case EVENT_MARK_OF_ANGUISH_TARGET_CHANGED:
                        {
                            me->CastStop();
                            DoResetThreat();

                            if (Player* target = sObjectAccessor->GetPlayer(*me, targetGUID))
                            {
                                me->AddThreat(target, 10000000.0f);
                                me->NearTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), me->GetAngle(target));
                            }

                            events.ScheduleEvent(EVENT_CHECK_MARK_OF_ANGUISH_TARGET, 1000);
                            break;
                        }
                        case EVENT_CHECK_MARK_OF_ANGUISH_TARGET:
                        {
                            if (Player* target = sObjectAccessor->GetPlayer(*me, targetGUID))
                            {
                                DoCast(target, SPELL_MARK_OF_ANGUISH_AURA_2);
                                me->ClearUnitState(UNIT_STATE_CASTING);
                            }
                            break;
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            uint64 targetGUID;
        };
};

class npc_sun_tenderheart_embodied_despair : public CreatureScript
{
    public:
        npc_sun_tenderheart_embodied_despair() : CreatureScript("npc_sun_tenderheart_embodied_despair") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sun_tenderheart_embodied_despairAI(creature);
        }

        struct npc_sun_tenderheart_embodied_despairAI : public Scripted_NoMovementAI
        {
            npc_sun_tenderheart_embodied_despairAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                ApplyAllImmunities(true);

                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                events.Reset();
            }

            void IsSummonedBy(Unit* owner)
            {
                events.ScheduleEvent(EVENT_MANIFEST, 1000);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
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

            void JustDied(Unit* who)
            {
                events.Reset();

                me->DespawnOrUnsummon(3000);
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
                        case EVENT_MANIFEST:
                            DoCast(me, SPELL_MANIFEST_DESPAIR);
                            events.ScheduleEvent(EVENT_MANIFEST, 9000);
                            break;
                        case EVENT_SYNC_HEALTH:
                            SyncHealth();
                            events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
                            break;
                    }
                }
            }

        private:

            void SyncHealth()
            {
                uint32 health = me->GetHealth();

                std::list<Creature*> creatures;
                me->GetCreatureListWithEntryInGrid(creatures, NPC_DESPAIR_SPAWN, 200.0f);// TODO: adjuste searche range

                for (std::list<Creature*>::const_iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* pSpawn = (*itr);

                    if (pSpawn && pSpawn->isAlive())
                        health = std::min(health, pSpawn->GetHealth());
                }

                if (health > 0 && me->GetHealth() != health)
                {
                    me->SetHealth(health);
                }
            }
        };
};

class npc_sun_tenderheart_embodied_desperation : public CreatureScript
{
    public:
        npc_sun_tenderheart_embodied_desperation() : CreatureScript("npc_sun_tenderheart_embodied_desperation") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sun_tenderheart_embodied_desperationAI(creature);
        }

        struct npc_sun_tenderheart_embodied_desperationAI : public Scripted_NoMovementAI
        {
            npc_sun_tenderheart_embodied_desperationAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                ApplyAllImmunities(true);

                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                events.Reset();
            }

            void IsSummonedBy(Unit* owner)
            {
                events.ScheduleEvent(EVENT_MANIFEST, 1000);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
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

            void JustDied(Unit* who)
            {
                events.Reset();
                summons.DespawnAll();

                me->DespawnOrUnsummon(3000);
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
                        case EVENT_MANIFEST:
                            DoCast(me, SPELL_MANIFEST_DESPERATION);
                            events.ScheduleEvent(EVENT_MANIFEST, 9000);
                            break;
                        case EVENT_SYNC_HEALTH:
                            SyncHealth();
                            events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
                            break;
                    }
                }
            }

        private:

            void SyncHealth()
            {
                uint32 health = me->GetHealth();

                std::list<Creature*> creatures;
                me->GetCreatureListWithEntryInGrid(creatures, NPC_DESPERATION_SPAWN, 200.0f);// TODO: adjuste searche range

                for (std::list<Creature*>::const_iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* pSpawn = (*itr);

                    if (pSpawn && pSpawn->isAlive())
                        health = std::min(health, pSpawn->GetHealth());
                }

                if (health > 0 && me->GetHealth() != health)
                {
                    me->SetHealth(health);
                }
            }
        };
};

class npc_sun_tenderheart_despair_spawn : public CreatureScript
{
    public:
        npc_sun_tenderheart_despair_spawn() : CreatureScript("npc_sun_tenderheart_despair_spawn") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sun_tenderheart_despair_spawnAI(creature);
        }

        struct npc_sun_tenderheart_despair_spawnAI : public Scripted_NoMovementAI
        {
            npc_sun_tenderheart_despair_spawnAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                ApplyAllImmunities(true);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
            }

            void JustDied(Unit* who)
            {
                events.Reset();
                summons.DespawnAll();

                me->DespawnOrUnsummon(3000);
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
                        case EVENT_SYNC_HEALTH:
                            SyncHealth();
                            events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
                            break;
                    }
                }
            }

        private:

            void SyncHealth()
            {
                Creature* pDespair = me->FindNearestCreature(NPC_EMBODIED_DESPAIR, 200.0f);
                std::list<Creature*> creatures;
                me->GetCreatureListWithEntryInGrid(creatures, NPC_DESPAIR_SPAWN, 200.0f);// TODO: adjuste searche range

                uint32 health = me->GetHealth();
                if (pDespair && pDespair->isAlive())
                    health = std::min(health, pDespair->GetHealth());

                for (std::list<Creature*>::const_iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* pSpawn = (*itr);

                    if (pSpawn && pSpawn->isAlive())
                        health = std::min(health, pSpawn->GetHealth());
                }

                if (health > 0 && me->GetHealth() != health)
                {
                    me->SetHealth(health);
                }
            }
        };
};

class npc_sun_tenderheart_desperation_spawn : public CreatureScript
{
    public:
        npc_sun_tenderheart_desperation_spawn() : CreatureScript("npc_sun_tenderheart_desperation_spawn") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sun_tenderheart_desperation_spawnAI(creature);
        }

        struct npc_sun_tenderheart_desperation_spawnAI : public Scripted_NoMovementAI
        {
            npc_sun_tenderheart_desperation_spawnAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                ApplyAllImmunities(true);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                me->DespawnOrUnsummon(3000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SYNC_HEALTH:
                            SyncHealth();
                            events.ScheduleEvent(EVENT_SYNC_HEALTH, 1000);
                            break;
                    }
                }
            }

        private:

            void SyncHealth()
            {
                Creature* pDesperation = me->FindNearestCreature(NPC_EMBODIED_DESPERATION, 200.0f);
                std::list<Creature*> creatures;
                me->GetCreatureListWithEntryInGrid(creatures, NPC_DESPERATION_SPAWN, 200.0f); // TODO: adjuste searche range

                uint32 health = me->GetHealth();
                if (pDesperation && pDesperation->isAlive())
                    health = std::min(health, pDesperation->GetHealth());

                for (std::list<Creature*>::const_iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* pSpawn = (*itr);

                    if (pSpawn && pSpawn->isAlive())
                        health = std::min(health, pSpawn->GetHealth());
                }

                if (health > 0 && me->GetHealth() != health)
                {
                    me->SetHealth(health);
                }
            }
        };
};

class spell_sun_tenderheart_calamity : public SpellScriptLoader
{
    public:
        spell_sun_tenderheart_calamity() : SpellScriptLoader("spell_sun_tenderheart_calamity") { }

        class spell_sun_tenderheart_calamity_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sun_tenderheart_calamity_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->RemoveAura(SPELL_SHADOW_WORD_BANE_DMG);
                GetCaster()->CastSpell(GetHitUnit(), SPELL_CALAMITY_DMG, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sun_tenderheart_calamity_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sun_tenderheart_calamity_SpellScript();
        }
};

class spell_sun_tenderheart_shadow_word_bane_aoe_1 : public SpellScriptLoader
{
    public:
        spell_sun_tenderheart_shadow_word_bane_aoe_1() : SpellScriptLoader("spell_sun_tenderheart_shadow_word_bane_aoe_1") { }

        class spell_sun_tenderheart_shadow_word_bane_aoe_1_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sun_tenderheart_shadow_word_bane_aoe_1_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHADOW_WORD_BANE_DUMMY_1, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sun_tenderheart_shadow_word_bane_aoe_1_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sun_tenderheart_shadow_word_bane_aoe_1_SpellScript();
        }
};

class spell_sun_tenderheart_shadow_word_bane_aoe_2 : public SpellScriptLoader
{
    public:
        spell_sun_tenderheart_shadow_word_bane_aoe_2() : SpellScriptLoader("spell_sun_tenderheart_shadow_word_bane_aoe_2") { }

        class spell_sun_tenderheart_shadow_word_bane_aoe_2_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sun_tenderheart_shadow_word_bane_aoe_2_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHADOW_WORD_BANE_DUMMY_2, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sun_tenderheart_shadow_word_bane_aoe_2_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sun_tenderheart_shadow_word_bane_aoe_2_SpellScript();
        }
};

class spell_sun_tenderheart_shadow_word_bane_dummy_1 : public SpellScriptLoader
{
    public:
        spell_sun_tenderheart_shadow_word_bane_dummy_1() : SpellScriptLoader("spell_sun_tenderheart_shadow_word_bane_dummy_1") { }

        class spell_sun_tenderheart_shadow_word_bane_dummy_1_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sun_tenderheart_shadow_word_bane_dummy_1_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHADOW_WORD_BANE_DMG, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sun_tenderheart_shadow_word_bane_dummy_1_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sun_tenderheart_shadow_word_bane_dummy_1_SpellScript();
        }
};

class spell_sun_tenderheart_shadow_word_bane_dummy_2 : public SpellScriptLoader
{
    public:
        spell_sun_tenderheart_shadow_word_bane_dummy_2() : SpellScriptLoader("spell_sun_tenderheart_shadow_word_bane_dummy_2") { }

        class spell_sun_tenderheart_shadow_word_bane_dummy_2_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sun_tenderheart_shadow_word_bane_dummy_2_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHADOW_WORD_BANE_DMG, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sun_tenderheart_shadow_word_bane_dummy_2_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sun_tenderheart_shadow_word_bane_dummy_2_SpellScript();
        }
};

class spell_sun_tenderheart_shadow_word_bane_dmg : public SpellScriptLoader
{
    public:
        spell_sun_tenderheart_shadow_word_bane_dmg() : SpellScriptLoader("spell_sun_tenderheart_shadow_word_bane_dmg") { }

        class spell_sun_tenderheart_shadow_word_bane_dmg_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sun_tenderheart_shadow_word_bane_dmg_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                uint32 tick = aurEff->GetTickNumber();
                if (tick < 3)
                {
                    GetCaster()->CastSpell(GetUnitOwner(), SPELL_SHADOW_WORD_BANE_AOE_2, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sun_tenderheart_shadow_word_bane_dmg_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sun_tenderheart_shadow_word_bane_dmg_AuraScript();
        }
};

class spell_rook_stonetoe_corrupted_brew_aoe : public SpellScriptLoader
{
    public:
        spell_rook_stonetoe_corrupted_brew_aoe() : SpellScriptLoader("spell_rook_stonetoe_corrupted_brew_aoe") { }

        class spell_rook_stonetoe_corrupted_brew_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rook_stonetoe_corrupted_brew_aoe_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_CORRUPTED_BREW_MISSILE_1, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rook_stonetoe_corrupted_brew_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rook_stonetoe_corrupted_brew_aoe_SpellScript();
        }
};

class spell_rook_stonetoe_inferno_strike : public SpellScriptLoader
{
    public:
        spell_rook_stonetoe_inferno_strike() : SpellScriptLoader("spell_rook_stonetoe_inferno_strike") { }

        class spell_rook_stonetoe_inferno_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rook_stonetoe_inferno_strike_SpellScript);

            void HandleDamage(SpellEffIndex effIndex)
            {
                int32 multiplier = GetSpellInfo()->Effects[EFFECT_0].BasePoints;

                SetHitDamage(GetHitDamage() * multiplier);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rook_stonetoe_inferno_strike_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rook_stonetoe_inferno_strike_SpellScript();
        }
};

class spell_rook_stonetoe_defiled_ground : public SpellScriptLoader
{
    public:
        spell_rook_stonetoe_defiled_ground() : SpellScriptLoader("spell_rook_stonetoe_defiled_ground") { }

        class spell_rook_stonetoe_defiled_ground_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rook_stonetoe_defiled_ground_SpellScript);

            void HandleSpell(SpellEffIndex effIndex)
            {
                if (!GetCaster())
                    return;

                // TODO: find spellvisual
                /*Position pos;
                GetCaster()->GetPosition(&pos);

                uint32 triggerEntry = 1053;

                AreaTrigger* areaTrigger = new AreaTrigger;
                if (!areaTrigger->CreateAreaTrigger(
                    SPELL_DEFILED_GROUND_AREATRIGGER,
                    map->GenerateLowGuid<HighGuid::AreaTrigger>(),
                    GetCaster()->GetPhaseMask(),
                    GetSpellInfo()->SpellVisual[0],
                    pos,
                    10000,
                    GetCaster()->GetMap()))
                {
                    delete areaTrigger;
                    return;
                }*/
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_rook_stonetoe_defiled_ground_SpellScript::HandleSpell, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rook_stonetoe_defiled_ground_SpellScript();
        }
};

class spell_he_stonefoot_gouge : public SpellScriptLoader
{
    public:
        spell_he_stonefoot_gouge() : SpellScriptLoader("spell_he_stonefoot_gouge") { }

        class spell_he_stonefoot_gouge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_he_stonefoot_gouge_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (GetHitUnit()->isInBack(GetCaster()))
                {
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_GOUGE_KNOCKBACK, true);
                }
                else
                {
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_GOUGE_DMG, true);
                    GetCaster()->getThreatManager().modifyThreatPercent(GetHitUnit(), -100);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_he_stonefoot_gouge_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_he_stonefoot_gouge_SpellScript();
        }
};

class spell_he_stonefoot_shadowstep_aoe : public SpellScriptLoader
{
    public:
        spell_he_stonefoot_shadowstep_aoe() : SpellScriptLoader("spell_he_stonefoot_shadowstep_aoe") { }

        class spell_he_stonefoot_shadowstep_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_he_stonefoot_shadowstep_aoe_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHADOWSTEP, true);

                // reenable moving to the target
                GetCaster()->ClearUnitState(UNIT_STATE_MELEE_ATTACKING);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_he_stonefoot_shadowstep_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_he_stonefoot_shadowstep_aoe_SpellScript();
        }
};

class spell_rook_stonetoe_clash_aoe : public SpellScriptLoader
{
    public:
        spell_rook_stonetoe_clash_aoe() : SpellScriptLoader("spell_rook_stonetoe_clash_aoe") { }

        class spell_rook_stonetoe_clash_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rook_stonetoe_clash_aoe_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                float distance = GetCaster()->GetDistance(GetHitUnit());
                Position centerPos;
                GetCaster()->GetNearPoint(GetCaster(), centerPos.m_positionX, centerPos.m_positionY, centerPos.m_positionZ, 0.0f, distance / 2.0f, GetCaster()->GetAngle(GetHitUnit()));
                centerPos.m_positionZ += 0.1f;

                GetCaster()->CastSpell(centerPos.GetPositionX(), centerPos.GetPositionY(), centerPos.GetPositionZ(), SPELL_CLASH_JUMP, true);
                GetHitUnit()->CastSpell(centerPos.GetPositionX(), centerPos.GetPositionY(), centerPos.GetPositionZ(), SPELL_CLASH_CHARGE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rook_stonetoe_clash_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rook_stonetoe_clash_aoe_SpellScript();
        }
};

class spell_rook_stonetoe_corruption_shock_aoe : public SpellScriptLoader
{
    public:
        spell_rook_stonetoe_corruption_shock_aoe() : SpellScriptLoader("spell_rook_stonetoe_corruption_shock_aoe") { }

        class spell_rook_stonetoe_corruption_shock_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rook_stonetoe_corruption_shock_aoe_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_CORRUPTION_SHOCK_MISSILE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rook_stonetoe_corruption_shock_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rook_stonetoe_corruption_shock_aoe_SpellScript();
        }
};

class spell_he_softfoot_mark_of_anguish_aoe : public SpellScriptLoader
{
    public:
        spell_he_softfoot_mark_of_anguish_aoe() : SpellScriptLoader("spell_he_softfoot_mark_of_anguish_aoe") { }

        class spell_he_softfoot_mark_of_anguish_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_he_softfoot_mark_of_anguish_aoe_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    pCreature->AI()->SetGUID(GetHitUnit()->GetGUID(), GUID_MARK_OF_ANGUISH);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_he_softfoot_mark_of_anguish_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_he_softfoot_mark_of_anguish_aoe_SpellScript();
        }
};

class spell_he_softfoot_mark_of_anguish_dummy : public SpellScriptLoader
{
    public:
        spell_he_softfoot_mark_of_anguish_dummy() : SpellScriptLoader("spell_he_softfoot_mark_of_anguish_dummy") { }

        class spell_he_softfoot_mark_of_anguish_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_he_softfoot_mark_of_anguish_dummy_SpellScript);

            void HandleDummy()
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->RemoveAura(SPELL_MARK_OF_ANGUISH_AURA_2);
                GetHitUnit()->AddAura(SPELL_DEBILITATION, GetHitUnit());
                
                if (Creature* pCreature = GetCaster()->FindNearestCreature(NPC_EMBODIED_ANGUISH, 200.0f))
                {
                    pCreature->AI()->SetGUID(GetHitUnit()->GetGUID(), GUID_MARK_OF_ANGUISH);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_he_softfoot_mark_of_anguish_dummy_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_he_softfoot_mark_of_anguish_dummy_SpellScript();
        }
};

class spell_he_softfoot_shadow_weakness_aoe : public SpellScriptLoader
{
    public:
        spell_he_softfoot_shadow_weakness_aoe() : SpellScriptLoader("spell_he_softfoot_shadow_weakness_aoe") { }

        class spell_he_softfoot_shadow_weakness_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_he_softfoot_shadow_weakness_aoe_SpellScript);

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->AddAura(SPELL_SHADOW_WEAKNESS_DEBUFF, GetHitUnit());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_he_softfoot_shadow_weakness_aoe_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_he_softfoot_shadow_weakness_aoe_SpellScript();
        }
};

class spell_he_softfoot_mark_of_anguish_aura_2 : public SpellScriptLoader
{
    public:
        spell_he_softfoot_mark_of_anguish_aura_2() : SpellScriptLoader("spell_he_softfoot_mark_of_anguish_aura_2") { }

        class spell_he_softfoot_mark_of_anguish_aura_2_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_he_softfoot_mark_of_anguish_aura_2_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_MARK_OF_ANGUISH_DMG, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_he_softfoot_mark_of_anguish_aura_2_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_he_softfoot_mark_of_anguish_aura_2_AuraScript();
        }
};

class spell_he_softfoot_instant_poison : public SpellScriptLoader
{
    public:
        spell_he_softfoot_instant_poison() : SpellScriptLoader("spell_he_softfoot_instant_poison") { }

        class spell_he_softfoot_instant_poison_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_he_softfoot_instant_poison_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo & p_EventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (Creature* pHe = GetCaster()->ToCreature())
                {
                    if (pHe->HasSpellCooldown(SPELL_INSTANT_POISON_DMG))
                        return;

                    if (Unit* target = pHe->getVictim())
                    {
                        pHe->CastSpell(target, SPELL_INSTANT_POISON_DMG, true);
                        pHe->_AddCreatureSpellCooldown(SPELL_INSTANT_POISON_DMG, time(NULL) + 3);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_he_softfoot_instant_poison_AuraScript::OnProc, EFFECT_1, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_he_softfoot_instant_poison_AuraScript();
        }
};

class spell_area_sun_tenderheart_dark_meditation : public SpellAreaTriggerScript
{
    public:
        spell_area_sun_tenderheart_dark_meditation() : SpellAreaTriggerScript("spell_area_sun_tenderheart_dark_meditation") {}

        bool OnAddTarget(AreaTrigger* trigger, Unit* target)
        {
            if (!trigger->GetCaster())
                return false;

            trigger->GetCaster()->AddAura(SPELL_MEDITATIVE_FIELD, target);

            return true;
        }

        bool OnRemoveTarget(AreaTrigger* trigger, Unit* target)
        {
            target->RemoveAura(SPELL_MEDITATIVE_FIELD);

            return true;
        }
};

class spell_area_he_softfoot_noxious_poison : public SpellAreaTriggerScript
{
    public:
        spell_area_he_softfoot_noxious_poison() : SpellAreaTriggerScript("spell_area_he_softfoot_noxious_poison") {}

        bool OnAddTarget(AreaTrigger* trigger, Unit* target)
        {
            if (!trigger->GetCaster())
                return false;

            trigger->GetCaster()->AddAura(SPELL_NOXIOUS_POISON_AURA, target);

            return true;
        }

        bool OnRemoveTarget(AreaTrigger* trigger, Unit* target)
        {
            target->RemoveAura(SPELL_NOXIOUS_POISON_AURA);

            return true;
        }
};

void AddSC_boss_fallen_protectors()
{
    new boss_rook_stonetoe();                               // 71475
    new boss_he_softfoot();                                 // 71479
    new boss_sun_tenderheart();                             // 71480

    new npc_rook_stonetoe_embodied_misery();                // 71476
    new npc_rook_stonetoe_embodied_sorrow();                // 71481
    new npc_rook_stonetoe_embodied_gloom();                 // 71477
    new npc_he_softfoot_embodied_anguish();                 // 71478
    new npc_sun_tenderheart_embodied_despair();             // 71474
    new npc_sun_tenderheart_embodied_desperation();         // 71482
    new npc_sun_tenderheart_despair_spawn();                // 71712
    new npc_sun_tenderheart_desperation_spawn();            // 71993

    new spell_sun_tenderheart_calamity();                   // 143491
    new spell_sun_tenderheart_shadow_word_bane_aoe_1();     // 143446
    new spell_sun_tenderheart_shadow_word_bane_aoe_2();     // 143438
    new spell_sun_tenderheart_shadow_word_bane_dummy_1();   // 143443
    new spell_sun_tenderheart_shadow_word_bane_dummy_2();   // 143879
    new spell_sun_tenderheart_shadow_word_bane_dmg();       // 143434
    new spell_rook_stonetoe_corrupted_brew_aoe();           // 143019
    new spell_rook_stonetoe_clash_aoe();                    // 143027
    new spell_rook_stonetoe_inferno_strike();               // 143962
    new spell_rook_stonetoe_defiled_ground();               // 143959
    new spell_rook_stonetoe_corruption_shock_aoe();         // 143958
    new spell_he_stonefoot_gouge();                         // 143330
    new spell_he_stonefoot_shadowstep_aoe();                // 143050
    new spell_he_softfoot_mark_of_anguish_aoe();            // 143822
    new spell_he_softfoot_mark_of_anguish_dummy();          // 143842
    new spell_he_softfoot_shadow_weakness_aoe();            // 144081
    new spell_he_softfoot_mark_of_anguish_aura_2();         // 143840
    new spell_he_softfoot_instant_poison();                 // 143210

    new spell_area_sun_tenderheart_dark_meditation();       // 143546
    new spell_area_he_softfoot_noxious_poison();            // 143235
}
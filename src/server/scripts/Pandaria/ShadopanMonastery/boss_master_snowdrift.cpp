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
    // Snowdrift
    SPELL_CHASE_DOWN            = 118961,
    SPELL_FIST_OF_FURY          = 106853,
    SPELL_PARRY_STANCE          = 106454,
    SPELL_QUIVERING_PALM        = 106422,
    SPELL_TORNADO_KICK          = 106434,

    SPELL_SMOKE_BOMB            = 110444,

    // Novices

    // Flagrant Lotus
    SPELL_BALL_OF_FIRE          = 113760,
    SPELL_BALL_OF_FIRE_DAMAGE   = 106470,
    SPELL_FLYING_KICK           = 106439,

    SPELL_CALL_STAFF            = 106681,
    SPELL_RELEASE_STAFF         = 106680,

    // Flying Snow
    SPELL_WHIRLING_STEEL_FOCUS  = 106699,
    SPELL_WHIRLING_STEEL_DAMAGE = 106646,

    // Both
    SPELL_FLARE                 = 132951,
};

enum eEvents
{
    // Snowdrift
    EVENT_FIRST_EVENT           = 1,
    EVENT_CHECK_WIPE            = 2,

    EVENT_CHASE_DOWN            = 3,
    EVENT_FIST_OF_FURY          = 4,
    EVENT_PARRY_STANCE          = 5,
    EVENT_QUIVERING_PALM        = 6,
    EVENT_TORNADO_KICK          = 7,
    EVENT_FIREBALL              = 8,
    EVENT_PHASE_3               = 9,

    EVENT_DISAPPEAR             = 10,
    EVENT_DISAPPEAR_TWO         = 11,

    // Miniboss
    EVENT_BALL_OF_FIRE          = 12,
    EVENT_FLYING_KICK           = 13,
    EVENT_CALL_STAFF            = 14,
    EVENT_RELEASE_STAFF         = 15,
    
    EVENT_WHIRLING_STEEL_FOCUS  = 16,
    EVENT_WHIRLING_STEEL_CHANGE = 17,
    EVENT_WHIRLING_STEEL_STOP   = 18
};

enum ePhases
{
    PHASE_FIRST_EVENT   = 1,
    PHASE_FIGHT_1       = 2,
    PHASE_FIGHT_2       = 3,
    PHASE_FIGHT_3       = 4
};

enum ePoints
{
    POINT_BEGIN_EVENT               = 1,
    POINT_PHASE_FIGHT               = 2,

    POINT_NOVICE_JUMP               = 3,
    POINT_NOVICE_DEFEATED           = 4,
    POINT_NOVICE_DEFEATED_SECOND    = 5,

    POINT_MINIBOSS_JUMP             = 6,
    POINT_MINIBOSS_DEFEATED         = 7
};

Position const g_SnowdriftPos[2] =
{
    { 3680.56f, 3045.27f, 816.20f },
    { 3713.60f, 3091.87f, 817.31f }
};

Position const g_InitiateSpawnPos[5] =
{
    { 3708.56f, 3039.60f, 816.28f },
    { 3699.19f, 3049.62f, 816.28f },
    { 3688.03f, 3055.68f, 816.28f },
    { 3675.10f, 3066.98f, 816.28f },
    { 3668.95f, 3070.81f, 816.28f }
};

Position const g_MinibossSpawnPos[2] =
{
    { 3683.72f, 3053.94f, 816.28f },
    { 3687.77f, 3051.30f, 816.28f }
};

Position const g_ClonePos[3] =
{
    { 3683.37f, 3087.65f, 815.70f, 0.0f  },
    { 3719.41f, 3062.31f, 815.70f, 1.88f },
    { 3718.04f, 3097.97f, 817.40f, 4.06f }
};

#define defaultrand urand(1000, 10000)

class boss_master_snowdrift : public CreatureScript
{
    public:
        boss_master_snowdrift() : CreatureScript("boss_master_snowdrift") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            WaveStart,
            WaveCurrent1,
            WaveCurrent2,
            MiniBoss,
            MiniBossStart,
            MiniBossEnd1,
            MiniBossEnd2,
            MiniBossEnd3,
            Aggro,
            Vanish,
            Phase3,
            Phase3Warn,
            Death
        };

        struct boss_master_snowdriftAI : public BossAI
        {
            boss_master_snowdriftAI(Creature* p_Creature) : BossAI(p_Creature, DATA_MASTER_SNOWDRIFT)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            bool m_IntroStarted;

            uint8 m_Phase;
            uint8 m_EventPhase;

            EventMap m_CosmeticEvents;

            void Reset() override
            {
                _Reset();

                me->RemoveFlag(EUnitFields::UNIT_NPC_FLAGS, NPCFlags::UNIT_NPC_FLAG_QUESTGIVER);

                m_IntroStarted = false;
                m_Phase = PHASE_FIRST_EVENT;
                m_EventPhase = 0;

                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(POINT_BEGIN_EVENT, g_SnowdriftPos[POINT_BEGIN_EVENT - 1]);

                me->setFaction(35);
                me->SetReactState(REACT_PASSIVE);

                SetCanSeeEvenInPassiveMode(true);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (m_Instance)
                    m_Instance->SetBossState(DATA_MASTER_SNOWDRIFT, IN_PROGRESS);

                InitDefaultEvents();

                Talk(eTalks::Aggro);
            }

            void InitDefaultEvents()
            {
                events.CancelEventGroup(PHASE_FIGHT_1);
                events.CancelEventGroup(PHASE_FIGHT_2);
                events.CancelEventGroup(PHASE_FIGHT_3);

                switch (m_Phase)
                {
                    case PHASE_FIGHT_1:
                        events.ScheduleEvent(EVENT_TORNADO_KICK, urand(7500, 12500), PHASE_FIGHT_1);
                        events.ScheduleEvent(EVENT_FIST_OF_FURY, urand(5000, 10000), PHASE_FIGHT_1);
                        events.ScheduleEvent(EVENT_CHASE_DOWN, defaultrand, PHASE_FIGHT_1);
                        break;
                    case PHASE_FIGHT_2:
                        events.ScheduleEvent(EVENT_FIREBALL, urand(2500, 5000), PHASE_FIGHT_2);
                        events.ScheduleEvent(EVENT_PHASE_3, urand(10000, 15000), PHASE_FIGHT_2);
                        break;
                    case PHASE_FIGHT_3:
                        events.ScheduleEvent(EVENT_PARRY_STANCE, urand(10000, 15000), PHASE_FIGHT_3);
                        events.ScheduleEvent(EVENT_QUIVERING_PALM, urand(5000, 10000), PHASE_FIGHT_3);
                        break;
                    default:
                        break;
                }
            }

            void MoveInLineOfSight(Unit* p_Mover) override
            {
                if (p_Mover->ToPlayer())
                {
                    if (p_Mover->ToPlayer()->isGameMaster())
                        return;

                    if (me->GetDistance(p_Mover) < 45.0f && !m_IntroStarted)
                    {
                        m_IntroStarted = true;
                        m_Instance->SetBossState(DATA_MASTER_SNOWDRIFT, SPECIAL);

                        m_CosmeticEvents.ScheduleEvent(EVENT_FIRST_EVENT, 1000);
                        m_CosmeticEvents.ScheduleEvent(EVENT_CHECK_WIPE, 1000);

                        Talk(eTalks::Intro1);

                        AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro2); });
                    }
                }
            }

            void JustReachedHome() override
            {
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(POINT_BEGIN_EVENT, g_SnowdriftPos[POINT_BEGIN_EVENT - 1]);

                if (m_Instance)
                    m_Instance->SetBossState(DATA_MASTER_SNOWDRIFT, FAIL);

                summons.DespawnAll();
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == ACTION_NOVICE_DONE)
                {
                    std::list<Creature*> l_NoviceList;
                    GetCreatureListWithEntryInGrid(l_NoviceList, me, NPC_NOVICE, 100.0f);

                    for (Creature* l_Novice : l_NoviceList)
                    {
                        if (Creature* l_Position = m_Instance->instance->GetCreature(m_Instance->GetData64(DATA_RANDOM_SECOND_POS)))
                            l_Novice->GetMotionMaster()->MoveJump(*l_Position, 20.0f, 30.0f, 10.0f, POINT_NOVICE_DEFEATED_SECOND);
                    }

                    Talk(eTalks::MiniBoss);

                    ++m_EventPhase;
                    m_CosmeticEvents.ScheduleEvent(EVENT_FIRST_EVENT, urand(4000, 6000));
                }
                else if (p_Action == ACTION_MINIBOSS_DONE)
                {
                    Talk(eTalks::MiniBossEnd1);

                    AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::MiniBossEnd2); });
                    AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::MiniBossEnd3); });

                    ++m_EventPhase;
                    m_CosmeticEvents.ScheduleEvent(EVENT_FIRST_EVENT, urand(16000, 20000));
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case POINT_PHASE_FIGHT:
                        me->setFaction(14);
                        me->SetReactState(REACT_AGGRESSIVE);
                    case POINT_BEGIN_EVENT:
                        me->SetFacingTo(me->GetAngle(3659.08f, 3015.38f));
                        break;
                    default:
                        break;
                }
            }

            void DoEvent()
            {
                switch (m_EventPhase)
                {
                    case 0:
                    case 1:
                    case 2:
                        //Talk(eventPhase);
                        ++m_EventPhase;
                        m_CosmeticEvents.ScheduleEvent(EVENT_FIRST_EVENT, 5000);
                        break;
                    // There is five novices waves
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    {
                        if (m_EventPhase == 3)
                            Talk(eTalks::WaveStart);
                        else if (m_EventPhase == 5)
                            Talk(eTalks::WaveCurrent1);
                        else if (m_EventPhase == 7)
                            Talk(eTalks::WaveCurrent2);

                        // Last wave have 4 initiates (for a total of 24)
                        uint8 l_MaxInitiativePerWave = m_EventPhase == 7 ? 4 : 5;

                        for (uint8 l_I = 0; l_I < l_MaxInitiativePerWave; ++l_I)
                            me->SummonCreature(NPC_NOVICE, g_InitiateSpawnPos[l_I]);

                        // When last wave, we wait for all initiate to be defeated
                        if (m_EventPhase != 7)
                        {
                            ++m_EventPhase;
                            m_CosmeticEvents.ScheduleEvent(EVENT_FIRST_EVENT, urand(12500, 17500));
                        }

                        break;
                    }
                    case 8:
                    {
                        Talk(eTalks::MiniBossStart);
                        me->SummonCreature(NPC_FLAGRANT_LOTUS, g_MinibossSpawnPos[0]);
                        me->SummonCreature(NPC_FLYING_SNOW,    g_MinibossSpawnPos[1]);
                        break;
                    }
                    case 9:
                    {
                        me->GetMotionMaster()->MovePoint(POINT_PHASE_FIGHT, g_SnowdriftPos[POINT_PHASE_FIGHT - 1]);
                        m_Phase = PHASE_FIGHT_1;
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage) override
            {
                switch (m_Phase)
                {
                    case PHASE_FIGHT_1:
                    {
                        if (me->HealthBelowPctDamaged(70, p_Damage))
                        {
                            me->CombatStop();
                            me->SetReactState(REACT_PASSIVE);

                            me->CastSpell(me, SPELL_SMOKE_BOMB, true);
                            me->SetVisible(false); // We set this for two seconds, time to let client erase players' focus from the boss

                            m_Phase = PHASE_FIGHT_2;
                            events.Reset();
                            events.ScheduleEvent(EVENT_DISAPPEAR, 2000);
                        }

                        break;
                    }
                    case PHASE_FIGHT_3:
                    {
                        if (p_Damage >= me->GetHealth())
                        {
                            p_Damage = 0;

                            me->RemoveAllAuras();

                            me->setFaction(35);
                            me->AttackStop();
                            me->CombatStop();
                            me->SetFlag(EUnitFields::UNIT_NPC_FLAGS, NPCFlags::UNIT_NPC_FLAG_QUESTGIVER);
                            me->SetReactState(ReactStates::REACT_PASSIVE);

                            Talk(eTalks::Death);

                            if (m_Instance)
                                m_Instance->SetBossState(DataTypes::DATA_MASTER_SNOWDRIFT, EncounterState::DONE);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
                    // Event Script
                    case EVENT_FIRST_EVENT:
                        DoEvent();
                        break;
                    case EVENT_CHECK_WIPE:
                        if (m_Instance && m_Instance->IsWipe())
                            Reset();
                        m_CosmeticEvents.ScheduleEvent(EVENT_CHECK_WIPE, defaultrand);
                        break;
                    default:
                        break;
                }

                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (uint32 l_EventID = events.ExecuteEvent())
                {
                    // Combat Script
                    /// Phase 1
                    case EVENT_TORNADO_KICK:
                        me->CastSpell(me, SPELL_TORNADO_KICK, false);
                        events.ScheduleEvent(EVENT_TORNADO_KICK, urand(7500, 12500));
                        break;
                    case EVENT_FIST_OF_FURY:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_FIST_OF_FURY, false);

                        events.ScheduleEvent(EVENT_FIST_OF_FURY, urand(5000, 10000));
                        break;
                    case EVENT_CHASE_DOWN:
                        // Todo
                        events.ScheduleEvent(l_EventID, defaultrand);
                        break;
                    /// Phase 2
                    case EVENT_DISAPPEAR:
                    {
                        std::vector<uint8> l_RandIndex;

                        for (int l_I = 0; l_I < 3; ++l_I)
                            l_RandIndex.push_back(l_I); // 0 1 2

                        std::random_shuffle(l_RandIndex.begin(), l_RandIndex.end());

                        bool l_IsBoss = true;

                        for (uint8 l_Index : l_RandIndex)
                        {
                            // The first random pos is for the boss, the two others are for his clones
                            if (l_IsBoss)
                            {
                                me->NearTeleportTo(g_ClonePos[l_Index]);
                                me->SetVisible(true);
                                me->CastSpell(me, SPELL_SMOKE_BOMB, true);
                                l_IsBoss = false;
                            }
                            else
                            {
                                if (Creature* l_Clone = me->SummonCreature(NPC_SNOWDRIFT_CLONE, g_ClonePos[l_Index]))
                                    l_Clone->CastSpell(l_Clone, SPELL_SMOKE_BOMB, true);
                            }
                        }

                        Talk(eTalks::Vanish);
                        InitDefaultEvents();
                        break;
                    }
                    case EVENT_FIREBALL:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_BALL_OF_FIRE, false);
                        events.ScheduleEvent(EVENT_FIREBALL, urand(2500, 5000));
                        break;
                    /// Phase 3
                    case EVENT_PHASE_3:
                        me->SetReactState(REACT_AGGRESSIVE);
                        m_Phase = PHASE_FIGHT_3;
                        InitDefaultEvents();
                        Talk(eTalks::Phase3);
                        Talk(eTalks::Phase3Warn);
                        break;
                    case EVENT_PARRY_STANCE:
                        me->CastSpell(me, SPELL_PARRY_STANCE, false);
                        events.ScheduleEvent(EVENT_PARRY_STANCE, urand(10000, 15000));
                        break;
                    case EVENT_QUIVERING_PALM:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_QUIVERING_PALM, false);
                        events.ScheduleEvent(EVENT_QUIVERING_PALM, urand(5000, 10000));
                        break;
                    default:
                        break;
                }

                if (m_Phase == PHASE_FIGHT_1 || m_Phase == PHASE_FIGHT_3)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_master_snowdriftAI(creature);
        }
};

class npc_snowdrift_novice : public CreatureScript
{
    public:
        npc_snowdrift_novice() :  CreatureScript("npc_snowdrift_novice") { }

        struct npc_snowdrift_noviceAI : public ScriptedAI
        {
            npc_snowdrift_noviceAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_JumpDone = false;
                m_StillInFight = true;
            }

            InstanceScript* m_Instance;
            EventMap events;
            bool m_JumpDone;
            bool m_StillInFight;

            void Reset() override
            {
                events.Reset();

                if (!m_JumpDone)
                {
                    float l_X, l_Y;
                    GetPositionWithDistInOrientation(me, 40.0f, 4.0f, l_X, l_Y);
                    me->GetMotionMaster()->MoveJump(l_X, l_Y, me->GetMap()->GetHeight(l_X, l_Y, me->GetPositionZ()), 20, 10, POINT_NOVICE_JUMP);
                    m_JumpDone = true;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != EFFECT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case POINT_NOVICE_JUMP:
                    {
                        if (!m_StillInFight)
                            break;

                        if (Player* l_Target = me->SelectNearestPlayerNotGM(100.0f))
                            AttackStart(l_Target);

                        break;
                    }
                    case POINT_NOVICE_DEFEATED:
                    {
                        if (m_Instance)
                            m_Instance->SetData(DATA_DEFEATED_NOVICE, 1);

                        AddTimedDelayedOperation(100, [this]() -> void
                        {
                            me->SetFacingTo(me->GetAngle(3659.08f, 3015.38f));
                            me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                        });

                        break;
                    }
                    case POINT_NOVICE_DEFEATED_SECOND:
                    {
                        AddTimedDelayedOperation(100, [this]() -> void
                        {
                            me->SetFacingTo(me->GetAngle(3659.08f, 3015.38f));
                            me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage) override
            {
                if (!m_StillInFight)
                    return;

                if (me->HasReactState(ReactStates::REACT_PASSIVE))
                {
                    p_Damage = 0;
                    return;
                }

                if (p_Damage >= me->GetHealth())
                {
                    p_Damage = 0;
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    me->setFaction(35);
                    me->AttackStop();
                    me->CombatStop();
                    m_StillInFight = false;
                    events.Reset();

                    if (m_Instance)
                    {
                        if (Creature* l_Position = m_Instance->instance->GetCreature(m_Instance->GetData64(DATA_RANDOM_FIRST_POS)))
                        {
                            me->GetMotionMaster()->MoveJump(*l_Position, 20.0f, 10.0f, 10.0f, POINT_NOVICE_DEFEATED);
                            me->SetHomePosition(*l_Position);
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_snowdrift_noviceAI(p_Creature);
        }
};

class npc_snowdrift_miniboss : public CreatureScript
{
    public:
        npc_snowdrift_miniboss() :  CreatureScript("npc_snowdrift_miniboss") { }

        struct npc_snowdrift_minibossAI : public ScriptedAI
        {
            npc_snowdrift_minibossAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                jumpDone = false;
                stillInFight = true;
            }

            InstanceScript* pInstance;
            EventMap events;
            bool jumpDone;
            bool stillInFight;
            uint8 whirlwindProgress;

            void Reset()
            {
                events.Reset();
                whirlwindProgress = 0;

                if (!jumpDone)
                {
                    float x, y;
                    GetPositionWithDistInOrientation(me, 30.0f, 4.23f, x, y);
                    me->GetMotionMaster()->MoveJump(x, y, me->GetMap()->GetHeight(x, y, me->GetPositionZ()), 20, 10, POINT_MINIBOSS_JUMP);
                    jumpDone = true;

                    if (me->GetEntry() == NPC_FLAGRANT_LOTUS)
                    {
                        events.ScheduleEvent(EVENT_BALL_OF_FIRE,    1000);
                        events.ScheduleEvent(EVENT_FLYING_KICK,     1000);
                        events.ScheduleEvent(EVENT_CALL_STAFF,      1000);
                        events.ScheduleEvent(EVENT_RELEASE_STAFF,   1000);
                    }
                    else
                    {
                        events.ScheduleEvent(EVENT_WHIRLING_STEEL_FOCUS,  1000);
                    }
                }
            }

            void MovementInform(uint32 uiType, uint32 id)
            {
                if (uiType != POINT_MOTION_TYPE)
                    return;

                switch (id)
                {
                    case POINT_MINIBOSS_DEFEATED:
                        me->SetFacingTo(me->GetAngle(3659.08f, 3015.38f));
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (!stillInFight)
                    return;

                if (damage >= me->GetHealth())
                {
                    damage = 0;
                    me->setFaction(35);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                    me->CombatStop();
                    events.Reset();
                    stillInFight = false;

                    if (Creature* position = pInstance->instance->GetCreature(pInstance->GetData64(DATA_RANDOM_MINIBOSS_POS)))
                    {
                        me->GetMotionMaster()->MovePoint(POINT_MINIBOSS_DEFEATED, position->GetPositionX(), position->GetPositionY(), position->GetPositionZ());
                        me->SetHomePosition(position->GetPositionX(), position->GetPositionY(), position->GetPositionZ(), position->GetOrientation());
                    }

                    if (pInstance)
                        pInstance->SetData(DATA_DEFEATED_MINIBOSS, 1);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!stillInFight)
                    return;

                events.Update(diff);

                switch(events.ExecuteEvent())
                {
                    case EVENT_BALL_OF_FIRE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_BALL_OF_FIRE, false);

                        events.ScheduleEvent(EVENT_BALL_OF_FIRE, 10000);
                        break;
                    case EVENT_FLYING_KICK:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_FLYING_KICK, false);

                        events.ScheduleEvent(EVENT_FLYING_KICK, 10000);
                        break;
                    case EVENT_CALL_STAFF:
                        //me->CastSpell(me, SPELL_CALL_STAFF, false);
                        events.ScheduleEvent(EVENT_CALL_STAFF, 10000);
                        break;
                    case EVENT_RELEASE_STAFF:
                        if (me->GetVehicleKit())
                            if (me->GetVehicleKit()->GetPassenger(0))
                                me->GetVehicleKit()->GetPassenger(0)->ExitVehicle();
                        events.ScheduleEvent(EVENT_CALL_STAFF, 10000);
                        break;
                    case EVENT_WHIRLING_STEEL_FOCUS:
                        me->AddAura(SPELL_WHIRLING_STEEL_DAMAGE, me);
                        me->SetReactState(REACT_PASSIVE);
                        // no break
                    case EVENT_WHIRLING_STEEL_CHANGE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            me->AddAura(SPELL_WHIRLING_STEEL_FOCUS, target);
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MoveChase(target);
                        }
                        if (++whirlwindProgress >= 5)
                            events.ScheduleEvent(EVENT_WHIRLING_STEEL_CHANGE, 2000);
                        else
                            events.ScheduleEvent(EVENT_WHIRLING_STEEL_STOP, 2000);
                        break;
                    case EVENT_WHIRLING_STEEL_STOP:
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->GetMotionMaster()->Clear();
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            AttackStart(target);
                        events.ScheduleEvent(EVENT_WHIRLING_STEEL_FOCUS, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_snowdrift_minibossAI(creature);
        }
};

class npc_snowdrift_clone : public CreatureScript
{
    public:
        npc_snowdrift_clone() :  CreatureScript("npc_snowdrift_clone") { }

        struct npc_snowdrift_cloneAI : public ScriptedAI
        {
            npc_snowdrift_cloneAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 fireBallTimer;

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->setFaction(14);
                DoZoneInCombat();

                fireBallTimer = 500;
            }

            void UpdateAI(const uint32 diff)
            {
                if (fireBallTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                        me->CastSpell(target, SPELL_BALL_OF_FIRE, true);

                    fireBallTimer = 500;
                }
                else fireBallTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_snowdrift_cloneAI(creature);
        }
};

class npc_snowdrift_fireball : public CreatureScript
{
    public:
        npc_snowdrift_fireball() :  CreatureScript("npc_snowdrift_fireball") { }

        struct npc_snowdrift_fireballAI : public ScriptedAI
        {
            npc_snowdrift_fireballAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 damageTimer;

            void Reset()
            {
                float x, y;
                GetPositionWithDistInOrientation(me, 100.0f, me->GetOrientation(), x, y);
                me->GetMotionMaster()->MovePoint(0, x, y, me->GetPositionZ());

                me->SetReactState(REACT_PASSIVE);
                me->setFaction(14);

                damageTimer = 500;
            }

            void UpdateAI(const uint32 diff)
            {
                if (damageTimer <= diff)
                {
                    if (Unit* target = me->SelectNearestTarget())
                        if (me->GetDistance(target) <= 1.0f)
                            me->CastSpell(target, SPELL_BALL_OF_FIRE_DAMAGE, true);

                    damageTimer = 500;
                }
                else damageTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_snowdrift_fireballAI(creature);
        }
};

void AddSC_boss_master_snowdrift()
{
    new boss_master_snowdrift();
    new npc_snowdrift_novice();
    new npc_snowdrift_miniboss();
    new npc_snowdrift_clone();
    new npc_snowdrift_fireball();
}

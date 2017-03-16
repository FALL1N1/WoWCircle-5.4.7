#include "siege_of_orgrimmar.hpp"

enum ScriptedTexts
{
    SAY_SIEGE_MODE      = 0,
    SAY_ASSAULT_MODE    = 1,
};

enum Spells
{
    SPELL_BERSERK                   = 26662,

    // TODO: We should use this spells to handle energy regeneration/degeneration
    // but there is 1000 msec periodic in spells
    // Don't know how to use it correctly
    SPELL_REGENERATION              = 146359,
    SPELL_DEPLETION                 = 146360,

    SPELL_BORER_DRILL               = 144209,
    SPELL_BORER_DRILL_AURA          = 144221, // npc 'borer drill' has it
    SPELL_BORER_DRILL_AREATRIGGER   = 144220,
    SPELL_BORER_DRLL_DMG            = 144218,
    SPELL_BORER_DRILL_VISUAL_1      = 144296,
    SPELL_BORER_DRILL_VISUAL_2      = 148823,

    SPELL_SCATTER_LASER_FORCE       = 144460, // main spell
    SPELL_LASER_STRIKE              = 144456,
    SPELL_SCATTER_LASER_FORCE_2     = 144458, // on players
    SPELL_LASER_BURN                = 144459,

    SPELL_MORTAR_BLAST_FORCE        = 145407, // main spell
    SPELL_MORTAR_BLAST_AOE          = 144315, // select targets
    SPELL_MORTAR_BLAST_MISSILE      = 146027,
    SPELL_MORTAR_BLAST_DMG          = 144316,

    SPELL_MORTAR_BLAST_FORCE_2      = 144555, // heroic
    SPELL_MORTAR_BLAST_PERIODIC     = 144554, // heroic
    SPELL_MORTAR_BLAST_MISSILE_2    = 144553, // heroic
    SPELL_MORTAR_BLAST_DMG_2        = 144556, // heroic

    SPELL_FLAME_VENTS               = 144464,
    SPELL_IGNITE_ARMOR              = 144467,

    SPELL_CRAWLER_MINE_AOE          = 144673, // select targets
    SPELL_CRAWLER_MINE_SUMMON       = 144705,
    SPELL_CRAWLER_MINE_IMMUNITY     = 149836,
    SPELL_CRAWLER_MINE_JUMP_3       = 149837, // remove immunities
    SPELL_SUPERHEATED_CRAWLER_MINE  = 146342, // created when laser touchs crawler mine
    SPELL_DETONATION_SEQUENCE       = 144718,
    SPELL_CRAWLER_MINE_BLAST        = 144766, // dmg to all players

    SPELL_GROUND_POUND              = 144776, // player jumps on crawler mines
    SPELL_ENGULFED_EXPLOSION_DMG    = 144791,

    SPELL_SEISMIC_ACTIVITY          = 144483,
    SPELL_SEISMIC_ACTIVITY_DMG      = 144484,
    SPELL_SEISMIC_ACTITITY_DEST     = 144557,

    SPELL_SHOCK_PULSE               = 144485,

    SPELL_DEMOLISHER_CANNONS_PERIODIC   = 144199,
    SPELL_DEMOLISHER_CANNONS_AOE        = 144198, // select targets
    SPELL_DEMOLISHER_CANNONS_MISSILE    = 144153,
    SPELL_DEMOLISHER_CANNONS_DMG    = 144154,

    SPELL_CUTTER_LASER_AOE          = 144573, // casted by 'cutter laser' dummy
    SPELL_CUTTER_LASER_RAY          = 144576,
    SPELL_CUTTER_LASER_FORCE        = 145190, // triggers script ?? 
    SPELL_CUTTER_LASER_SCRIPT       = 144575, // ??
    SPELL_CUTTER_LASER_TARGET       = 146325,
    SPELL_CUTTER_LASER_DMG          = 144918,

    SPELL_EXPLOSIVE_TAR_PERIODIC    = 144492,
    SPELL_EXPLOSIVE_TAR_MISSILE     = 144496,
    SPELL_EXPLOSIVE_TAR_SUMMON      = 144497,
    SPELL_EXPLOSIVE_TAR_VISUAL      = 146191,
    SPELL_EXPLOSIVE_TAR_DMG         = 144498,
    // 144525 explosive tar unknown (aura without visuals)
    SPELL_TAR_EXPLOSION             = 144919,
};

enum Adds
{
    NPC_BORER_DRILL     = 71906,

    NPC_CANNON_LEFT     = 71468,
    NPC_CANNON_RIGHT    = 71913,
    NPC_TOP_CANNON      = 71484,
    NPC_TAIL_GUN        = 71914,

    NPC_SAWBLADE_1      = 71469, // basic
    NPC_SAWBLADE_2      = 71532, // when the sawblade moves
    NPC_SAWBLADE_3      = 71945, // invisible ?

    NPC_CRAWLER_MINE    = 72050,
    NPC_CRAWLER_MINE_2  = 72060, // invisible, summoned by 144705 (I think it's target for real mine jumping)

    NPC_CUTTER_LASER    = 72026,

    NPC_EXPLOSIVE_TAR   = 71950,
};

enum Events
{
    EVENT_BERSERK = 1,
    EVENT_SIEGE_MODE_PREPARE,
    EVENT_ASSAULT_MODE_PREPARE,
    EVENT_FLAME_VENTS,
    EVENT_BORER_DRILL,
    EVENT_SCATTER_LASER,
    EVENT_MORTAR_BLAST,
    EVENT_CRAWLER_MINE,

    EVENT_SHOCK_PULSE,
    EVENT_DEMOLISHER_CANNONS,
    EVENT_CUTTER_LASER,
    EVENT_EXPLOSIVE_TAR,

};

enum Actions
{
    ACTION_DEMOLISHER_CANNONS   = 1,
    ACTION_CUTTER_LASER,
};

enum eData
{
    DATA_CRAWLER_MINE_DUMMY = 1,
    DATA_CUTTER_LASER_GUID,
};

enum VehicleSeatPositions
{
    TOP_CANNON_SEAT     = 0,
    SAWBLADE_SEAT       = 1,
    RIGHT_CANNON_SEAT   = 2,
    LEFT_CANNON_SEAT    = 3,
    TAIL_GUN_SEAT       = 4,
};

enum Phases
{
    PHASE_NONE      = 0,
    PHASE_ASSAULT   = 1,
    PHASE_SIEGE     = 2,
};

enum Timers
{
    TIMER_BERSERK                   = 10 * MINUTE * IN_MILLISECONDS,
    TIMER_FLAME_VENTS_FIRST         = 9 * IN_MILLISECONDS,
    TIMER_FLAME_VENTS               = 10 * IN_MILLISECONDS,
    TIMER_BORER_DRILL               = 17 * IN_MILLISECONDS,
    TIMER_MORTAR_BLAST              = 4 * IN_MILLISECONDS,
    TIMER_SCATTER_LASER             = 11500,
    TIMER_CRAWLER_MINE              = 30 * IN_MILLISECONDS,

    TIMER_SHOCK_PULSE               = 16500,
    TIMER_DEMOLISHER_CANNONS        = 8500,
    TIMER_CUTTER_LASER              = 25 * IN_MILLISECONDS,
    TIMER_EXPLOSIVE_TAR_FIRST       = 7000,
    TIMER_EXPLOSIVE_TAR             = 30000,
    TIMER_SIEGE_MODE_PREPARE        = 1 * IN_MILLISECONDS,
    TIMER_ASSAULT_MODE_PREPARE      = 1 * IN_MILLISECONDS,
    TIMER_ASSAULT_MODE_REGENERATE   = 1200,
    TIMER_SIEGE_MODE_REGENERATE     = 600,
};

#define MAX_POWER_ENERGY 100
#define POWER_TYPE POWER_ENERGY
#define MAX_CRAWLER_MINES 3
#define CUTTER_LASER_RADIUS 2.0f
#define EXPLOSIVE_TAR_RADIUS 4.0f
#define BOSS_COMBAT_REACH_SIZE 15.0f
#define DEFAULT_RANGE_TARGETS_DIST 15.0f
#define BOSS_RANGE_TARGETS_DIST (BOSS_COMBAT_REACH_SIZE - DEFAULT_RANGE_TARGETS_DIST)

class boss_iron_juggernaut : public CreatureScript
{
    public:
        boss_iron_juggernaut() : CreatureScript("boss_iron_juggernaut") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_iron_juggernautAI(creature);
        }

        struct boss_iron_juggernautAI : public BossAI
        {
            boss_iron_juggernautAI(Creature* creature) : BossAI(creature, DATA_IRON_JUGGERNAUT)
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, BOSS_COMBAT_REACH_SIZE);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, BOSS_COMBAT_REACH_SIZE);
            }

            void Reset()
            {
                _Reset();

                InitPowers();

                InitVehicles();

                me->SetReactState(REACT_DEFENSIVE);

                m_CurrentPhase = PHASE_NONE;
            }

            void EnterCombat(Unit* who)
            {
                SendIronJuggernautStart();

                m_CurrentPhase = PHASE_ASSAULT;

                events.ScheduleEvent(EVENT_BERSERK, TIMER_BERSERK);
                events.ScheduleEvent(EVENT_FLAME_VENTS, TIMER_FLAME_VENTS_FIRST);
                events.ScheduleEvent(EVENT_BORER_DRILL, TIMER_BORER_DRILL);
                events.ScheduleEvent(EVENT_MORTAR_BLAST, TIMER_MORTAR_BLAST);
                events.ScheduleEvent(EVENT_SCATTER_LASER, TIMER_SCATTER_LASER);
                events.ScheduleEvent(EVENT_CRAWLER_MINE, TIMER_CRAWLER_MINE);

                instance->SetBossState(DATA_IRON_JUGGERNAUT, IN_PROGRESS);
                DoZoneInCombat();
            }

            void RegeneratePower(Powers power, int32& value) override
            {
                value = 0;
            }

            void SetGUID(uint64 guid, int32 id) override
            {
                if (id == DATA_CRAWLER_MINE_DUMMY)
                {
                    SpawnCrawlerMine(guid);
                }
            }

            void JustDied(Unit* who)
            {
                SendIronJuggernautDead();

                _JustDied();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (IsFarFromHomePosition())
                {
                    EnterEvadeMode();
                    return;
                }

                RegenerateEnergy(diff);

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
                    case EVENT_FLAME_VENTS:
                        DoCastVictim(SPELL_FLAME_VENTS);
                        events.ScheduleEvent(EVENT_FLAME_VENTS, TIMER_FLAME_VENTS);
                        break;
                    case EVENT_BORER_DRILL:
                        DoBorerDrill();
                        events.ScheduleEvent(EVENT_BORER_DRILL, TIMER_BORER_DRILL);
                        break;
                    case EVENT_MORTAR_BLAST:
                        DoCastAOE(SPELL_MORTAR_BLAST_FORCE);
                        events.ScheduleEvent(EVENT_MORTAR_BLAST, TIMER_MORTAR_BLAST);
                        break;
                    case EVENT_SCATTER_LASER:
                        DoCastAOE(SPELL_SCATTER_LASER_FORCE);
                        events.ScheduleEvent(EVENT_SCATTER_LASER, TIMER_SCATTER_LASER);
                        break;
                    case EVENT_CRAWLER_MINE:
                        DoCastAOE(SPELL_CRAWLER_MINE_AOE);
                        events.ScheduleEvent(EVENT_CRAWLER_MINE, TIMER_CRAWLER_MINE);
                        break;
                    case EVENT_SHOCK_PULSE:
                        DoCastAOE(SPELL_SHOCK_PULSE);
                        events.ScheduleEvent(EVENT_SHOCK_PULSE, TIMER_SHOCK_PULSE);
                        break;
                    case EVENT_DEMOLISHER_CANNONS:
                        DemolisherCannons();
                        events.ScheduleEvent(EVENT_DEMOLISHER_CANNONS, TIMER_DEMOLISHER_CANNONS);
                        break;
                    case EVENT_CUTTER_LASER:
                        CutterLaser();
                        break;
                    case EVENT_EXPLOSIVE_TAR:
                        DoCast(me, SPELL_EXPLOSIVE_TAR_PERIODIC);
                        events.ScheduleEvent(EVENT_EXPLOSIVE_TAR, TIMER_EXPLOSIVE_TAR);
                        break;
                    case EVENT_SIEGE_MODE_PREPARE:
                        ShiftIntoSiegeMode();
                        break;
                    case EVENT_ASSAULT_MODE_PREPARE:
                        ShiftIntoAssaultMode();
                        break;
                    default:
                        break;
                }
            }

        private:

            bool IsInAssaultPhase() const
            {
                return m_CurrentPhase == PHASE_ASSAULT;
            }

            bool IsInSiegePhase() const
            {
                return m_CurrentPhase == PHASE_SIEGE;
            }

            void InitPowers()
            {
                me->setPowerType(POWER_TYPE);
                me->SetMaxPower(POWER_TYPE, MAX_POWER_ENERGY);
                me->SetPower(POWER_TYPE, 0);

                regenerateTimer = TIMER_ASSAULT_MODE_REGENERATE;
            }

            void RegenerateEnergy(const uint32 diff)
            {
                if (!IsInAssaultPhase() && !IsInSiegePhase())
                    return;

                if (regenerateTimer <= diff)
                {
                    int32 currentEnergy = me->GetPower(POWER_TYPE);

                    if (IsInAssaultPhase())
                    {
                        if (currentEnergy < MAX_POWER_ENERGY)
                        {
                            int32 newEnergy = currentEnergy + 1;
                            me->SetPower(POWER_TYPE, newEnergy);

                            if (newEnergy >= MAX_POWER_ENERGY)
                            {
                                Talk(SAY_SIEGE_MODE);
                                events.ScheduleEvent(EVENT_SIEGE_MODE_PREPARE, TIMER_SIEGE_MODE_PREPARE);
                            }
                        }

                        regenerateTimer = TIMER_ASSAULT_MODE_REGENERATE;
                    }
                    else if (IsInSiegePhase())
                    {
                        if (currentEnergy > 0)
                        {
                            int32 newEnergy = currentEnergy - 1;
                            me->SetPower(POWER_TYPE, newEnergy);

                            if (newEnergy == 0)
                            {
                                Talk(SAY_ASSAULT_MODE);
                                events.ScheduleEvent(EVENT_ASSAULT_MODE_PREPARE, TIMER_ASSAULT_MODE_PREPARE);
                            }
                        }

                        regenerateTimer = TIMER_SIEGE_MODE_REGENERATE;
                    }
                }
                else
                {
                    regenerateTimer -= diff;
                }
            }

            void InitVehicles()
            {
                if (Creature* pCannon = me->SummonCreature(NPC_TOP_CANNON, *me))
                {
                    pCannon->EnterVehicle(me, TOP_CANNON_SEAT);
                }

                if (Creature* pCannon = me->SummonCreature(NPC_SAWBLADE_1, *me))
                {
                    pCannon->EnterVehicle(me, SAWBLADE_SEAT);
                }

                if (Creature* pCannon = me->SummonCreature(NPC_CANNON_RIGHT, *me))
                {
                    pCannon->EnterVehicle(me, RIGHT_CANNON_SEAT);
                }

                if (Creature* pCannon = me->SummonCreature(NPC_CANNON_LEFT, *me))
                {
                    pCannon->EnterVehicle(me, LEFT_CANNON_SEAT);
                }

                if (Creature* pCannon = me->SummonCreature(NPC_TAIL_GUN, *me))
                {
                    pCannon->EnterVehicle(me, TAIL_GUN_SEAT);
                }
            }

            void DoBorerDrill()
            {
                Position pos;
                me->GetNearPosition(pos, 13.0f, -0.3f);

                DoCast(me, SPELL_BORER_DRILL_VISUAL_1, true);
                me->CastSpell(pos, SPELL_BORER_DRILL, false);
            }

            void SpawnCrawlerMine(uint64 targetCreatureGuid)
            {
                if (Unit* target = Unit::GetUnit(*me, targetCreatureGuid))
                {
                    if (Creature* pMine = me->SummonCreature(NPC_CRAWLER_MINE, *me, TEMPSUMMON_TIMED_DESPAWN, 60000))
                    {
                        pMine->GetMotionMaster()->MoveJump(*target, 20.0f, 10.0f);
                    }
                }
            }

            void DemolisherCannons()
            {
                uint32 cannonEntry = urand(0, 1) ? NPC_CANNON_LEFT : NPC_CANNON_RIGHT;

                EntryCheckPredicate pred(cannonEntry);
                summons.DoAction(ACTION_DEMOLISHER_CANNONS, pred);
            }

            void CutterLaser()
            {
                EntryCheckPredicate pred(NPC_TAIL_GUN);
                summons.DoAction(ACTION_CUTTER_LASER, pred);
            }

            void ShiftIntoSiegeMode()
            {
                m_CurrentPhase = PHASE_SIEGE;

                me->AttackStop();
                me->SetReactState(REACT_PASSIVE);

                me->RemoveAura(SPELL_REGENERATION);

                events.CancelEvent(EVENT_FLAME_VENTS);
                events.CancelEvent(EVENT_BORER_DRILL);
                events.CancelEvent(EVENT_SCATTER_LASER);
                events.CancelEvent(EVENT_MORTAR_BLAST);
                events.CancelEvent(EVENT_CRAWLER_MINE);

                DoCast(me, SPELL_SEISMIC_ACTIVITY);

                events.ScheduleEvent(EVENT_SHOCK_PULSE, TIMER_SHOCK_PULSE);
                events.ScheduleEvent(EVENT_DEMOLISHER_CANNONS, TIMER_DEMOLISHER_CANNONS);
                events.ScheduleEvent(EVENT_CUTTER_LASER, TIMER_CUTTER_LASER);
                events.ScheduleEvent(EVENT_EXPLOSIVE_TAR, TIMER_EXPLOSIVE_TAR_FIRST);
            }

            void ShiftIntoAssaultMode()
            {
                m_CurrentPhase = PHASE_ASSAULT;

                me->SetReactState(REACT_AGGRESSIVE);
                AttackStart(me->getVictim());

                me->RemoveAura(SPELL_DEPLETION);
                me->RemoveAura(SPELL_SEISMIC_ACTIVITY);

                events.CancelEvent(EVENT_SHOCK_PULSE);
                events.CancelEvent(EVENT_DEMOLISHER_CANNONS);
                events.CancelEvent(EVENT_CUTTER_LASER);
                events.CancelEvent(EVENT_EXPLOSIVE_TAR);

                events.ScheduleEvent(EVENT_FLAME_VENTS, TIMER_FLAME_VENTS_FIRST);
                events.ScheduleEvent(EVENT_BORER_DRILL, TIMER_BORER_DRILL);
                events.ScheduleEvent(EVENT_MORTAR_BLAST, TIMER_MORTAR_BLAST);
                events.ScheduleEvent(EVENT_SCATTER_LASER, TIMER_SCATTER_LASER);
                events.ScheduleEvent(EVENT_CRAWLER_MINE, TIMER_CRAWLER_MINE);
            }

            bool IsFarFromHomePosition() const
            {
                Position const& pos = me->GetHomePosition();
                if (me->GetDistance(pos) > 60.0f)
                {
                    return true;
                }

                return false;
            }

            void SendIronJuggernautStart()
            {
                instance->SetData(DATA_IRON_JUGGERNAUT_START, DONE);
            }

            void SendIronJuggernautDead()
            {
                instance->SetData(DATA_IRON_JUGGERNAUT_DEAD, DONE);
            }

        private:

            Phases m_CurrentPhase;
            uint32 regenerateTimer;
        };
};

class npc_iron_juggernaut_cannons : public CreatureScript
{
    public:
        npc_iron_juggernaut_cannons() : CreatureScript("npc_iron_juggernaut_cannons") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_juggernaut_cannonsAI(creature);
        }

        struct npc_iron_juggernaut_cannonsAI : public Scripted_NoMovementAI
        {
            npc_iron_juggernaut_cannonsAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            
                pInstance = creature->GetInstanceScript();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_DEMOLISHER_CANNONS)
                {
                    DemolisherCannons();
                }
                else if (action == ACTION_CUTTER_LASER)
                {
                    CutterLaser();
                }
            }

        private:

            Creature* GetIronJuggernaut()
            {
                if (pInstance)
                {
                    return pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_JUGGERNAUT));
                }

                return NULL;
            }

            void DemolisherCannons()
            {
                if (me->GetEntry() != NPC_CANNON_LEFT &&
                    me->GetEntry() != NPC_CANNON_RIGHT)
                    return;

                DoCast(me, SPELL_DEMOLISHER_CANNONS_PERIODIC);
            }

            void CutterLaser()
            {
                if (me->GetEntry() != NPC_TAIL_GUN)
                    return;

                Creature* pJuggernaut = GetIronJuggernaut();
                if (!pJuggernaut)
                    return;

                // boss's size is 16.f, so -1.0f is like -17.0f
                Unit* target = pJuggernaut->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, -BOSS_RANGE_TARGETS_DIST, true);
                if (!target)
                    target = pJuggernaut->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                if (target)
                {
                    Position pos;
                    target->GetRandomNearPosition(pos, 3.0f);

                    if (Creature* pLaser = pJuggernaut->SummonCreature(NPC_CUTTER_LASER, pos, TEMPSUMMON_TIMED_DESPAWN, 10000))
                    {
                        target->AddAura(SPELL_CUTTER_LASER_TARGET, target);

                        pLaser->AI()->SetGUID(target->GetGUID(), DATA_CUTTER_LASER_GUID);

                        DoCast(pLaser, SPELL_CUTTER_LASER_RAY);
                    }
                }
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_iron_juggernaut_borer_drill : public CreatureScript
{
    public:
        npc_iron_juggernaut_borer_drill() : CreatureScript("npc_iron_juggernaut_borer_drill") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_juggernaut_borer_drillAI(creature);
        }

        struct npc_iron_juggernaut_borer_drillAI : public ScriptedAI
        {
            npc_iron_juggernaut_borer_drillAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void Reset()
            {
                me->AddAura(SPELL_BORER_DRILL_AURA, me);

                me->SetSpeed(MOVE_RUN, 0.4f, true);

                moveTimer = 3000;
                isMoved = false;
            }

            void UpdateAI(const uint32 diff)
            {
                UpdateMove(diff);
            }

        private:

            void UpdateMove(const uint32 diff)
            {
                if (isMoved)
                    return;

                if (moveTimer <= diff)
                {
                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -20.0f, true);
                    if (!target)
                        target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                    if (target)
                    {
                        me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f);
                    }

                    isMoved = true;
                }
                else
                {
                    moveTimer -= diff;
                }
            }

        private:

            uint32 moveTimer;
            bool isMoved;
        };
};

class npc_iron_juggernaut_crawler_mine_dummy : public CreatureScript
{
    public:
        npc_iron_juggernaut_crawler_mine_dummy() : CreatureScript("npc_iron_juggernaut_crawler_mine_dummy") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_juggernaut_crawler_mine_dummyAI(creature);
        }

        struct npc_iron_juggernaut_crawler_mine_dummyAI : public ScriptedAI
        {
            npc_iron_juggernaut_crawler_mine_dummyAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void IsSummonedBy(Unit* owner)
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (Creature* pJuggernaut = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_JUGGERNAUT)))
                    {
                        pJuggernaut->AI()->SetGUID(me->GetGUID(), DATA_CRAWLER_MINE_DUMMY);
                    }
                }
            }

        private:

        private:

        };
};

class npc_iron_juggernaut_crawler_mine : public CreatureScript
{
    public:
        npc_iron_juggernaut_crawler_mine() : CreatureScript("npc_iron_juggernaut_crawler_mine") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_juggernaut_crawler_mineAI(creature);
        }

        struct npc_iron_juggernaut_crawler_mineAI : public ScriptedAI
        {
            npc_iron_juggernaut_crawler_mineAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_CLIENT_CANNOT_TARGET);

                isExploded = false;
                beforeExplodeTimer = 14000;
                me->setFaction(35);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (id == EVENT_JUMP)
                {
                    me->GetMotionMaster()->MovementExpired(false);
                    DoCast(SPELL_DETONATION_SEQUENCE);

                    // reinit timer
                    beforeExplodeTimer = 14000;
                }
            }

            void OnSpellClick(Unit* clicker)
            {
                if (isExploded)
                    return;

                isExploded = true;

                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

                clicker->CastSpell(me, SPELL_GROUND_POUND, true);

                me->DespawnOrUnsummon(1000);
            }

            void UpdateAI(const uint32 diff)
            {
                UpdateBeforeExplode(diff);
            }

        private:

            void UpdateBeforeExplode(const uint32 diff)
            {
                if (isExploded)
                    return;

                if (beforeExplodeTimer <= diff)
                {
                    me->setFaction(14);
                    isExploded = true;
                }
                else
                {
                    beforeExplodeTimer -= diff;
                }
            }

        private:

            uint32 beforeExplodeTimer;
            bool isExploded;

        };
};

class npc_iron_juggernaut_cutter_laser : public CreatureScript
{
    public:
        npc_iron_juggernaut_cutter_laser() : CreatureScript("npc_iron_juggernaut_cutter_laser") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_juggernaut_cutter_laserAI(creature);
        }

        struct npc_iron_juggernaut_cutter_laserAI : public ScriptedAI
        {
            npc_iron_juggernaut_cutter_laserAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

                targetGuid = 0;

                damageTimer = 2000;
            }

            void Reset()
            {
                me->SetSpeed(MOVE_RUN, 0.5f, true);

                moveTimer = 2000;
                isMoved = false;
            }

            void SetGUID(uint64 guid, int32 id) override
            {
                if (id == DATA_CUTTER_LASER_GUID)
                {
                    targetGuid = guid;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                UpdateMove(diff);

                UpdateNearestPlayers(diff);
            }

        private:

            void UpdateMove(const uint32 diff)
            {
                if (isMoved)
                    return;

                if (moveTimer <= diff)
                {
                    if (Unit* target = me->GetUnit(*me, targetGuid))
                    {
                        me->GetMotionMaster()->MoveFollowExact(target, 0.0f, 0.0f);
                    }

                    isMoved = true;
                }
                else
                {
                    moveTimer -= diff;
                }
            }

            void UpdateNearestPlayers(const uint32 diff)
            {
                if (!isMoved)
                    return;

                if (damageTimer <= diff)
                {
                    damageTimer = 2000;

                    DoDamageToNearestPlayers();
                }
                else
                {
                    damageTimer -= diff;
                }
            }

            void DoDamageToNearestPlayers()
            {
                std::list<Player*> players;
                me->GetPlayerListInGrid(players, CUTTER_LASER_RADIUS);

                for (Player* player : players)
                {
                    if (!player->HasAura(SPELL_CUTTER_LASER_DMG))
                        me->AddAura(SPELL_CUTTER_LASER_DMG, player);
                }
            }

        private:

            uint64 targetGuid;
            uint32 moveTimer;
            uint32 damageTimer;
            bool isMoved;
        };
};

class npc_iron_juggernaut_explosive_tar : public CreatureScript
{
    public:
        npc_iron_juggernaut_explosive_tar() : CreatureScript("npc_iron_juggernaut_explosive_tar") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_juggernaut_explosive_tarAI(creature);
        }

        struct npc_iron_juggernaut_explosive_tarAI : public Scripted_NoMovementAI
        {
            npc_iron_juggernaut_explosive_tarAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

                damageTimer = 2000;
                checkLaserTimer = 2000;
                isExploded = false;
            }

            void Reset()
            {
                me->AddAura(SPELL_EXPLOSIVE_TAR_VISUAL, me);
            }

            void UpdateAI(const uint32 diff)
            {
                UpdateNearestPlayers(diff);

                CheckNearestCutterLaser(diff);
            }

        private:

            void UpdateNearestPlayers(const uint32 diff)
            {
                if (damageTimer <= diff)
                {
                    damageTimer = 2000;

                    DoDamageToNearestPlayers();
                }
                else
                {
                    damageTimer -= diff;
                }
            }

            void DoDamageToNearestPlayers()
            {
                std::list<Player*> players;
                me->GetPlayerListInGrid(players, EXPLOSIVE_TAR_RADIUS);

                for (Player* player : players)
                {
                    if (!player->HasAura(SPELL_EXPLOSIVE_TAR_DMG))
                        me->AddAura(SPELL_EXPLOSIVE_TAR_DMG, player);
                }
            }

            void CheckNearestCutterLaser(const uint32 diff)
            {
                if (isExploded)
                    return;

                if (checkLaserTimer <= diff)
                {
                    checkLaserTimer = 1000;

                    if (me->FindNearestCreature(NPC_CUTTER_LASER, EXPLOSIVE_TAR_RADIUS))
                    {
                        isExploded = true;
                        DoCastAOE(SPELL_TAR_EXPLOSION, true);
                        me->DespawnOrUnsummon(500);
                    }
                }
                else
                {
                    checkLaserTimer -= diff;
                }
            }

        private:

            uint32 damageTimer;
            uint32 checkLaserTimer;
            bool isExploded;
        };
};

class EntryCheck
{
    public:
        
        EntryCheck(uint32 entry) : m_Entry(entry) { }

        bool operator()(WorldObject* object) const
        {
            if (object->GetEntry() == m_Entry)
                return false;

            return true;
        }

    private:

        uint32 m_Entry;
};

class spell_iron_juggernaut_borer_drill : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_borer_drill() : SpellScriptLoader("spell_iron_juggernaut_borer_drill") { }

        class spell_iron_juggernaut_borer_drill_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_juggernaut_borer_drill_SpellScript);

            void HandleHit(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                if (WorldLocation const* worldLocation = GetHitDest())
                {
                    Position pos;
                    worldLocation->GetPosition(&pos);
                    GetCaster()->SummonCreature(NPC_BORER_DRILL, pos, TEMPSUMMON_TIMED_DESPAWN, 20000);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_iron_juggernaut_borer_drill_SpellScript::HandleHit, EFFECT_1, SPELL_EFFECT_PERSISTENT_AREA_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_juggernaut_borer_drill_SpellScript();
        }
};

class spell_iron_juggernaut_mortar_blast_force : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_mortar_blast_force() : SpellScriptLoader("spell_iron_juggernaut_mortar_blast_force") { }

        class spell_iron_juggernaut_mortar_blast_force_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_juggernaut_mortar_blast_force_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(EntryCheck(NPC_TOP_CANNON));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_juggernaut_mortar_blast_force_SpellScript::FilterTargets, EFFECT_0,  TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_juggernaut_mortar_blast_force_SpellScript();
        }
};

class spell_iron_juggernaut_mortar_blast_aoe : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_mortar_blast_aoe() : SpellScriptLoader("spell_iron_juggernaut_mortar_blast_aoe") { }

        class spell_iron_juggernaut_mortar_blast_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_juggernaut_mortar_blast_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                std::list<WorldObject*> rangeTargets;

                for (auto target : targets)
                {
                    if (GetCaster()->GetDistance(target) >= DEFAULT_RANGE_TARGETS_DIST)
                    {
                        rangeTargets.push_back(target);
                    }
                }

                WorldObject* target = NULL;

                if (!rangeTargets.empty())
                {
                    target = JadeCore::Containers::SelectRandomContainerElement(rangeTargets);
                }
                else if (!targets.empty())
                {
                    target = JadeCore::Containers::SelectRandomContainerElement(targets);
                }

                if (target)
                {
                    targets.clear();
                    targets.push_back(target);
                }
            }

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_MORTAR_BLAST_MISSILE, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_juggernaut_mortar_blast_aoe_SpellScript::FilterTargets, EFFECT_0,  TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_iron_juggernaut_mortar_blast_aoe_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_juggernaut_mortar_blast_aoe_SpellScript();
        }
};

class spell_iron_juggernaut_scatter_laser_force : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_scatter_laser_force() : SpellScriptLoader("spell_iron_juggernaut_scatter_laser_force") { }

        class spell_iron_juggernaut_scatter_laser_force_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_juggernaut_scatter_laser_force_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(EntryCheck(NPC_TAIL_GUN));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_juggernaut_scatter_laser_force_SpellScript::FilterTargets, EFFECT_0,  TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_juggernaut_scatter_laser_force_SpellScript();
        }
};

class spell_iron_juggernaut_crawler_mine_aoe : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_crawler_mine_aoe() : SpellScriptLoader("spell_iron_juggernaut_crawler_mine_aoe") { }

        class spell_iron_juggernaut_crawler_mine_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_juggernaut_crawler_mine_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (Creature* pJaggernaut = GetCaster()->ToCreature())
                {
                    std::list<Unit*> rangeTargets;
                    // boss's size is 16.f, so -1.0f is like -17.0f
                    pJaggernaut->AI()->SelectTargetList(rangeTargets, MAX_CRAWLER_MINES, SELECT_TARGET_RANDOM, -BOSS_RANGE_TARGETS_DIST, true);
                    
                    if (rangeTargets.size() < MAX_CRAWLER_MINES)
                    {
                        pJaggernaut->AI()->SelectTargetList(rangeTargets, MAX_CRAWLER_MINES, SELECT_TARGET_RANDOM, 15.0f, true);
                    }

                    targets.clear();
                    for (auto target : rangeTargets)
                        targets.push_back(target);
                }
            }

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetHitUnit(), SPELL_CRAWLER_MINE_SUMMON, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_juggernaut_crawler_mine_aoe_SpellScript::FilterTargets, EFFECT_0,  TARGET_UNIT_DEST_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_iron_juggernaut_crawler_mine_aoe_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_juggernaut_crawler_mine_aoe_SpellScript();
        }
};

class spell_iron_juggernaut_detonation_sequence : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_detonation_sequence() : SpellScriptLoader("spell_iron_juggernaut_detonation_sequence") { }

        class spell_iron_juggernaut_detonation_sequence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_iron_juggernaut_detonation_sequence_AuraScript);

            void HandlePeriodicTick(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_CRAWLER_MINE_BLAST, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_iron_juggernaut_detonation_sequence_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_iron_juggernaut_detonation_sequence_AuraScript();
        }
};

class spell_iron_juggernaut_demolisher_cannons_aoe : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_demolisher_cannons_aoe() : SpellScriptLoader("spell_iron_juggernaut_demolisher_cannons_aoe") { }

        class spell_iron_juggernaut_demolisher_cannons_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_juggernaut_demolisher_cannons_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                InstanceScript* pInstance = GetCaster()->GetInstanceScript();
                if (!pInstance)
                    return;

                Creature* pJaggernaut = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_JUGGERNAUT));
                if (!pJaggernaut)
                    return;

                uint32 maxTargetsCount = pJaggernaut->GetMap()->Is25ManRaid() ? 5 : 3;

                std::list<Unit*> rangeTargets;
                // boss's size is 16.f, so -1.0f is like -17.0f
                pJaggernaut->AI()->SelectTargetList(rangeTargets, maxTargetsCount, SELECT_TARGET_RANDOM, -BOSS_RANGE_TARGETS_DIST, true);
                    
                if (rangeTargets.size() < maxTargetsCount)
                {
                    pJaggernaut->AI()->SelectTargetList(rangeTargets, maxTargetsCount, SELECT_TARGET_RANDOM, 15.0f, true);
                }

                targets.clear();
                for (auto target : rangeTargets)
                    targets.push_back(target); 
            }

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_DEMOLISHER_CANNONS_MISSILE, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_juggernaut_demolisher_cannons_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_iron_juggernaut_demolisher_cannons_aoe_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_juggernaut_demolisher_cannons_aoe_SpellScript();
        }
};

class spell_iron_juggernaut_cutter_laser_aoe : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_cutter_laser_aoe() : SpellScriptLoader("spell_iron_juggernaut_cutter_laser_aoe") { }

        class spell_iron_juggernaut_cutter_laser_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_juggernaut_cutter_laser_aoe_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;
                
                targets.remove_if(EntryCheck(NPC_TAIL_GUN));
            }

            void HandleHitTarget(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_CUTTER_LASER_RAY, false);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_juggernaut_cutter_laser_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnEffectHitTarget += SpellEffectFn(spell_iron_juggernaut_cutter_laser_aoe_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_juggernaut_cutter_laser_aoe_SpellScript();
        }
};

class spell_iron_juggernaut_cutter_laser_dmg : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_cutter_laser_dmg() : SpellScriptLoader("spell_iron_juggernaut_cutter_laser_dmg") { }

        class spell_iron_juggernaut_cutter_laser_dmg_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_iron_juggernaut_cutter_laser_dmg_AuraScript);

            void HandlePeriodicTick(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (!GetUnitOwner()->FindNearestCreature(NPC_CUTTER_LASER, CUTTER_LASER_RADIUS + 1.0f))
                {
                    Remove();
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_iron_juggernaut_cutter_laser_dmg_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_iron_juggernaut_cutter_laser_dmg_AuraScript();
        }
};

class spell_iron_juggernaut_explosive_tar_periodic : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_explosive_tar_periodic() : SpellScriptLoader("spell_iron_juggernaut_explosive_tar_periodic") { }

        class spell_iron_juggernaut_explosive_tar_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_iron_juggernaut_explosive_tar_periodic_AuraScript);

            void HandlePeriodicTick(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                const uint32 maxTarTargets = 5;

                if (Creature* pJuggernaut = GetUnitOwner()->ToCreature())
                {
                    Position tarPos;

                    // boss's size is 16.f, so -1.0f is like -17.0f
                    if (Unit* target = pJuggernaut->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, -BOSS_RANGE_TARGETS_DIST, true))
                    {
                        target->GetPosition(&tarPos);
                    }
                    else
                    {
                        pJuggernaut->GetRandomNearPosition(tarPos, 25.0f);
                    }

                    pJuggernaut->CastSpell(tarPos, SPELL_EXPLOSIVE_TAR_MISSILE, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_iron_juggernaut_explosive_tar_periodic_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_iron_juggernaut_explosive_tar_periodic_AuraScript();
        }
};

class spell_iron_juggernaut_explosive_tar_dmg : public SpellScriptLoader
{
    public:
        spell_iron_juggernaut_explosive_tar_dmg() : SpellScriptLoader("spell_iron_juggernaut_explosive_tar_dmg") { }

        class spell_iron_juggernaut_explosive_tar_dmg_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_iron_juggernaut_explosive_tar_dmg_AuraScript);

            void HandlePeriodicTick(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (!GetUnitOwner()->FindNearestCreature(NPC_EXPLOSIVE_TAR, EXPLOSIVE_TAR_RADIUS + 1.0f))
                {
                    Remove();
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_iron_juggernaut_explosive_tar_dmg_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_iron_juggernaut_explosive_tar_dmg_AuraScript();
        }
};

void AddSC_boss_iron_juggernaut()
{
    new boss_iron_juggernaut();                         // 71466
    new npc_iron_juggernaut_cannons();                  // 71484
    new spell_iron_juggernaut_borer_drill();            // 71906 71914
    new npc_iron_juggernaut_crawler_mine_dummy();       // 72060
    new npc_iron_juggernaut_crawler_mine();             // 72050
    new npc_iron_juggernaut_cutter_laser();             // 72026
    new npc_iron_juggernaut_explosive_tar();            // 71950

    new spell_iron_juggernaut_borer_drill();            // 144209
    new spell_iron_juggernaut_mortar_blast_force();     // 145407
    new spell_iron_juggernaut_mortar_blast_aoe();       // 144315
    new spell_iron_juggernaut_scatter_laser_force();    // 144460
    new spell_iron_juggernaut_crawler_mine_aoe();       // 144673
    new spell_iron_juggernaut_detonation_sequence();    // 144718
    new spell_iron_juggernaut_demolisher_cannons_aoe(); // 144198
    new spell_iron_juggernaut_cutter_laser_aoe();       // 144573
    new spell_iron_juggernaut_cutter_laser_dmg();       // 144918
    new spell_iron_juggernaut_explosive_tar_periodic(); // 144492
    new spell_iron_juggernaut_explosive_tar_dmg();      // 144498
}
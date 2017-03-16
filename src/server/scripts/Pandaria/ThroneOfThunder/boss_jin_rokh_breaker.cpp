#include "throne_of_thunder.hpp"

enum Yells
{
    SAY_AGGRO               = 0,
    SAY_BERSERK             = 1,
    SAY_DEATH               = 2,
    SAY_INTRO               = 3,
    SAY_KILL                = 4,
    SAY_STATIC_BURST        = 5,
    SAY_THUNDERING_THROW    = 6,
    SAY_LIGHTNING_STORM     = 7,
    SAY_FOCUSED_LIGHTNING   = 8,

    ANN_THUNDERING_THROW    = 9,
    ANN_FOCUSED_LIGHTNING   = 10,
    ANN_LIGHTNING_STORM     = 11,
};

enum Spells
{
    // Boss

    // Thundering Throw.
    SPELL_RIDE_VEHICLE          = 43671,  // Grab spell (Control Vehicle).

    SPELL_THUNDERING_THROW      = 137180, // Main cast time spell, dummy effect 0 for SPELL_THUNDERING_THROW_GRAB.
    SPELL_THUNDERING_THROW_GRAB = 137161, // Silence, pacify, eff. 0 script effect for SPELL_RIDE_VEHICLE.
    SPELL_THUNDERING_THROW_JMP  = 137173, // Jump behind target spell cast on Statue npc.
    SPELL_THUNDERING_THROW_PLR  = 137370, // Dmg dealed to player on impact.
    SPELL_THUNDERING_THROW_AOE  = 137167, // Additional 190000 to 210000 Nature damage to other players within 8 yards of the point of impact.
    SPELL_THUNDERING_THROW_STUN = 137371, // Stun for 5 seconds on other players within 8 yards of the point of impact.

    // Conductive Waters.
    SPELL_CONDUCTIVE_WATER_SUMM = 137145, // Summons Conductive Waters npc NPC_CONDUCTIVE_WATER.
    SPELL_CONDUCTIVE_VISUAL     = 137277, // Water effect visual ground.
    SPELL_CONDUCTIVE_VISUAL_2   = 137340, // Mogu fountain visual shooted by statue (orientation!).
    SPELL_CONDUCTIVE_VISUAL_3   = 137978, // Electrified waters visual ground.
    SPELL_CONDUCTIVE_WATER_GROW = 137676, // Aura size increase 137694 11 % each 100 ms.
    SPELL_CONDUCTIVE_WATER      = 138470, // Debuff, + 80% Nature damage.
    SPELL_FLUIDITY              = 138002, // Buff 40% damage dealt.

    // Static burst + Wound.
    SPELL_STATIC_BURST          = 137162, // Does 95000 to 105000 Nature damage to all players. After 3 sec., expires and applies 10 Static Wound stacks to current target (eff 1).

    SPELL_STATIC_WOUND          = 138349, // Aura. Inflicts 20000 Nature damage / stack when taking melee damage. - 1 stack / 3 secs (eff 1).
    SPELL_STATIC_WOUND_COND     = 138375, // Static Wound Conduction damage, inflicting 20000 / 3 Nature damage / stack to all players in the Conductive Water. If tank is in it.
    SPELL_STATIC_WOUND_DMG      = 138389, // Inflicts 20000 / 3 Nature damage / stack to all players when taking melee damage. On SPELL_STATIC_WOUND.

    // Focused Lightning + Lightning Fissure left after FL explosion (If the Focused Lightning detonates outside of Conductive Water, a Lightning Fissure forms).
    SPELL_FOCUSED_LIGHTNING     = 137399, // Cast time + dummy on eff 0 (to summon Focused Lightning npc NPC_FOCUSED_LIGHTNING).
    SPELL_FOCUSED_LIGHTNING_VIS = 137425, // Focused Lightning visual.
    SPELL_FOCUSED_LIGHTNING_SPD = 137389, // Aura, each 500 ms increase movement speed (triggers 137437).
    SPELL_FOCUSED_LIGHTNING_DMG = 137423, // Pulses 38000 to 42000 Nature damage every half second to other players within 5 yards.
    SPELL_FOCUSED_LIGHTNING_FIX = 137422, // Fixated aura (player).
    SPELL_FOCUSED_LIGHTNING_DET = 137374, // Focused Lightning Detonation damage, 170625 to 179375 Nature damage to players in 8 yards of fixated victim.
    SPELL_FOCUSED_LIGHTNING_DMG_1 = 137530, // Focused Lightning Conduction damage, 170625 to 179375 Nature damage to all players standing in Conductive Water.
    SPELL_FOCUSED_LIGHTNING_DMG_2 = 138990, // If touches ELECTRIFIED Conductive Water.

    SPELL_LIGHTNING_FISSURE_SUM = 137479, // Summons Lightning Fissure npc NPC_LIGHTNING_FISSURE.
    SPELL_LIGHTNING_FISSURE_VIS = 137480, // Fissure visual.
    SPELL_LIGHTNING_FISSURE_DMG = 137484, // Aura.
    SPELL_LIGHTNING_FISS_COND   = 138133, // Contacts Conductive Water, removing Lightning Fissure + 71250 - 78750 dmg to players standing in Conductive Water.
    SPELL_IMPLOSION             = 137507, // LF implodes upon contact with another Focused Lightning, 237500 - 262500 dmg to all players + 10% damage taken from Implosion. 

    // Lightning Storm.
    SPELL_LIGHTNING_STORM       = 137313, // Aura.
    SPELL_LIGHTNING_STORM_VIS   = 140555, // Visual.
    SPELL_LIGHTNING_STRIKE_VIS  = 140507, // Visual too.
    SPELL_LIGHTNING_STORM_VIS2  = 140775, // Visual 2.
    SPELL_LIGHTNING_STORM_DMG   = 137261, // Effect 0 damage from aura.
    SPELL_LIGHTNING_STORM_DUMMY = 140819, // Effect 1 dummy from aura.
    SPELL_LIGHTNING_STORM_H_DMG = 137647, // Heroic, triggered on SPELL_LIGHTNING_STORM_DUMMY.
    SPELL_LIGHTNING_DIFFUSION   = 137905, // Each Lightning Strike diffuses, 195000 to 205000 Nature damage to all players in a line. On SPELL_LIGHTNING_STORM_H_DMG.
    SPELL_ELECTRIFIED_WATERS    = 138006, // Removes Fluidity, 35000 Nature damage / sec. to players in Conductive Water. Electrifies the water (visual change).

    // Ionization - Heroic only.
    SPELL_IONIZATION_AURA       = 138732, // Dummy on effect 1 for SPELL_IONIZATION_DMG when dispelled or duration expires.
    SPELL_IONIZATION_DMG        = 138733, // HEROIC. Inflicts 450000 Nature damage to players within 8 yards.
    SPELL_IONIZATION_CONDUCTION = 138743, // Ionization damage travels through Conductive Water, inflicting 450000 Nature damage to players standing in Conductive Water.

    SPELL_BERSERK               = 144369, // Berserk, Enrage, Bye - Bye or, simply put, a wipe. :)

    SPELL_ACHIEVEMENT           = 139163,
};

enum Npcs
{
    NPC_STATUE_TRIGGER          = 69467,
    NPC_CONDUCTIVE_WATER        = 69469,
	NPC_FOCUSED_LIGHTNING       = 69593,
    NPC_LIGHTNING_FISSURE       = 69609
};

enum Events
{
    // Boss
    EVENT_THUNDERING_THROW      = 1,
    EVENT_THUNDERING_THROW_TOSS,
    EVENT_THUNDERING_THROW_DMG,
    EVENT_STATIC_BURST,
    EVENT_FOCUSED_LIGHTNING,
    EVENT_LIGHTNING_STORM_JUMP,
    EVENT_LIGHTNING_STORM,
    EVENT_IONIZATION,

    EVENT_BERSERK,

    // Focused Lightning   
    EVENT_FOCUSED_LIGHTNING_DMG,
    EVENT_START_MOVE,
    EVENT_CHECK_TARGETS,

    // Lightning Fissure
    EVENT_FISSURE_SEARCH,

    // Conductive    
    EVENT_BUFF_DEBUFF
};

enum Actions
{
    // Statue trigger
    ACTION_STATUE_WATER         = 1 // Break Mogu statue and form Conductive Water.
};

enum Guids
{
    GUID_WATER  = 1,
    GUID_PLAYER = 2,
    GUID_THROW_TARGET,
};

enum Timers
{
    TIMER_THUNDERING_THROW_F    = 28000, // 30 seconds into the fight counted with below.
    TIMER_THUNDERING_THROW_S    = 90000, // Every 90 seconds after that.

    TIMER_THUNDERING_THROW_TOSS = 2000,  // Actual throwing timer.
    TIMER_THUNDERING_THROW_DMG  = 1500,  // Player damage timer.

    TIMER_BUFF_DEBUFF           = 1100,  // Conductive Water buff / debuff refresh timer.

    TIMER_STATIC_BURST_F        = 15000, // 15 seconds into the fight.
    TIMER_STATIC_BURST_S        = 22000, // Every 22 seconds after that.

    TIMER_FOCUSED_LIGHTNING_F   = 10000, // 10 seconds into the fight.
    TIMER_FOCUSED_LIGHTNING_S   = 15000, // Every 15 seconds after that.

    TIMER_FOCUSED_LIGHTNING_DMG = 500,   // Focused Lightning periodic damage.

    TIMER_LIGHTNING_STORM_JUMPF = 88500, // Jump timer before Storm phase.
    TIMER_LIGHTNING_STORM_JUMPS = 88500, // Jump timer before Storm phase.

    TIMER_LIGHTNING_STORM       = 1500,  // Every 90sec counted with above.

    TIMER_IONIZATION_F          = 60000, // HEROIC, once per normal phase.
    TIMER_IONIZATION_S          = 90000,

    TIMER_BERSERK               = 540000,
};

Position const middlePos        = { 5891.60f, 6263.18f, 124.11f };

#define MAX_MOGU_STATUES 4
Position const statueTriggerPlace[MAX_MOGU_STATUES] =
{
    { 5841.659f, 6313.456f, 162.212f, 5.38f }, // South-east
    { 5838.741f, 6210.887f, 159.103f, 0.74f }, // North-east
    { 5943.230f, 6210.887f, 161.288f, 2.30f }, // North-west
    { 5943.678f, 6315.564f, 163.020f, 3.88f }, // South-west
};

const uint32 statueFontainDataList[MAX_MOGU_STATUES] =
{
    DATA_MOGU_FOUNTAIN_SE,
    DATA_MOGU_FOUNTAIN_NE,
    DATA_MOGU_FOUNTAIN_NW,
    DATA_MOGU_FOUNTAIN_SW
};

Position const statueThrowPlace[MAX_MOGU_STATUES] =
{
    { 5855.366f,  6301.588f, 157.095f }, // Jump South-east 2.39f O
    { 5853.612f,  6227.119f, 156.869f }, // Jump North-east 3.95f O
    { 5927.603f,  6224.533f, 158.019f }, // Jump North-west 5.69f O
    { 5928.242f,  6301.532f, 156.285f }, // Jump South-west 0.72f O
};

Position const waterSummonPlace[MAX_MOGU_STATUES] =
{
    { 5861.975f, 6293.226f, 124.035f }, // South-east
    { 5860.412f, 6232.429f, 124.035f }, // North-east
    { 5922.794f, 6237.926f, 124.035f }, // North-west
    { 5921.287f, 6288.865f, 124.035f }, // South-west
};

struct StatueData
{
    public:

        StatueData(uint64 triggerGuid, uint64 fontainGuid, Position throwDest, Position waterDest)
        {
            m_TriggerGuid = triggerGuid;
            m_FontainGuid = fontainGuid;
            m_PlayerThrowDest = throwDest;
            m_WaterSummonDest = waterDest;
            m_IsActivated = false;
        }

        uint64 GetTriggerGuid() const
        {
            return m_TriggerGuid;
        }

        uint64 GetFontainGuid() const
        {
            return m_FontainGuid;
        }

        Position GetThrowDest() const
        {
            return m_PlayerThrowDest;
        }

        Position GetWaterSummonDest() const
        {
            return m_WaterSummonDest;
        }

        bool IsActivated() const
        {
            return m_IsActivated;
        }

        void Activate()
        {
            m_IsActivated = true;
        }

    private:

        bool m_IsActivated;
        uint64 m_TriggerGuid;
        uint64 m_FontainGuid;
        Position m_PlayerThrowDest;
        Position m_WaterSummonDest;
};


class StatueController
{
    typedef std::set<StatueData*> StatuesSet;

    public:

        StatueController(Unit* owner)
        {
            m_Owner = owner;
            m_LastActivatedStatue = 0;
        }

        void SpawnStatues()
        {
            ASSERT(m_Owner->GetInstanceScript());

            for (uint8 i = 0; i < MAX_MOGU_STATUES; i++)
            {
                if (Creature* pStatueTrigger = m_Owner->SummonCreature(NPC_STATUE_TRIGGER, statueTriggerPlace[i], TEMPSUMMON_MANUAL_DESPAWN))
                {
                    uint64 fontainGuid = m_Owner->GetInstanceScript()->GetData64(statueFontainDataList[i]);
                    Position throwDest = statueThrowPlace[i];
                    Position waterDest = waterSummonPlace[i];
                    m_Statues.insert(new StatueData(pStatueTrigger->GetGUID(), fontainGuid, throwDest, waterDest));
                }
            }
        }

        void DespawnStatues()
        {
            for (StatuesSet::const_iterator itr = m_Statues.begin(); itr != m_Statues.end(); ++itr)
            {
                if (Creature* pStatueTrigger = sObjectAccessor->GetCreature(*m_Owner, (*itr)->GetTriggerGuid()))
                {
                    pStatueTrigger->DespawnOrUnsummon();
                }

                if (GameObject* pFontain = sObjectAccessor->GetGameObject(*m_Owner, (*itr)->GetFontainGuid()))
                {
                    pFontain->SetGoState(GO_STATE_READY);
                }

                delete (*itr);
            }

            m_Statues.clear();
            m_LastActivatedStatue = 0;
        }

        void ActivateNextStatueFor(Unit* target)
        {
            if (m_Statues.empty())
                return;

            StatueData* statue = GetRandomNotActivatedStatue();
            if (!statue)
                return;

            statue->Activate();

            Creature* pStatueTrigger = sObjectAccessor->GetCreature(*m_Owner, statue->GetTriggerGuid());
            if (pStatueTrigger)
            {
                pStatueTrigger->AI()->DoAction(ACTION_STATUE_WATER);
                m_LastActivatedStatue = pStatueTrigger->GetGUID();
            }

            if (GameObject* pFontain = sObjectAccessor->GetGameObject(*m_Owner, statue->GetFontainGuid()))
            {
                pFontain->SetGoState(GO_STATE_ACTIVE);
            }

            // Spell doesn't work. Throw player at statue.
            target->GetMotionMaster()->MoveJump(statue->GetThrowDest().GetPositionX(), statue->GetThrowDest().GetPositionY(), statue->GetThrowDest().GetPositionZ(), 25.0f, 25.0f);

            // Summon Conductive Water (by position, not spell).
            m_Owner->SummonCreature(NPC_CONDUCTIVE_WATER, statue->GetWaterSummonDest(), TEMPSUMMON_MANUAL_DESPAWN);
        }

        Creature* GetLastActivatedStatue() const
        {
            return sObjectAccessor->GetCreature(*m_Owner, m_LastActivatedStatue);
        }

    private:

        StatueData* GetRandomNotActivatedStatue()
        {
            std::list<StatueData*> notActivatedStatues;
            for (StatuesSet::const_iterator itr = m_Statues.begin(); itr != m_Statues.end(); ++itr)
            {
                if (!(*itr)->IsActivated())
                    notActivatedStatues.push_back(*itr);
            }

            if (notActivatedStatues.empty())
                return NULL;

            return JadeCore::Containers::SelectRandomContainerElement(notActivatedStatues);
        }

    private:

        Unit* m_Owner;
        std::set<StatueData*> m_Statues;
        uint64 m_LastActivatedStatue;
};

class boss_jinrokh_the_breaker : public CreatureScript
{
    public:
        boss_jinrokh_the_breaker() : CreatureScript("boss_jinrokh_the_breaker") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_jinrokh_the_breakerAI(creature);
        }

        struct boss_jinrokh_the_breakerAI : public BossAI
        {
            boss_jinrokh_the_breakerAI(Creature* creature) : BossAI(creature, DATA_JIN_ROKH_BREAKER), m_StatueController(me)
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                introDone = false;
                
                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 10);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 10);
            }

            void Reset()
            {
                _Reset();

                m_StatueController.DespawnStatues();
                
                SetThunderingThrowTarget(NULL);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FOCUSED_LIGHTNING_FIX);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FLUIDITY);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONDUCTIVE_WATER);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ELECTRIFIED_WATERS);

                DespawnCreaturesInArea(NPC_FOCUSED_LIGHTNING, me);
                DespawnCreaturesInArea(NPC_LIGHTNING_FISSURE, me);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!introDone && me->IsWithinDistInMap(who, 100.0f) && who->GetTypeId() == TYPEID_PLAYER)
                {
                    Talk(SAY_INTRO);
                    introDone = true;
                }

                BossAI::MoveInLineOfSight(who);
            }

            void EnterCombat(Unit* who)
            {
                Talk(SAY_AGGRO);

                m_StatueController.SpawnStatues();

				events.ScheduleEvent(EVENT_THUNDERING_THROW, TIMER_THUNDERING_THROW_F);
				events.ScheduleEvent(EVENT_STATIC_BURST, TIMER_STATIC_BURST_F);
				events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, TIMER_FOCUSED_LIGHTNING_F);
				events.ScheduleEvent(EVENT_LIGHTNING_STORM_JUMP, TIMER_LIGHTNING_STORM_JUMPF);

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_IONIZATION, TIMER_IONIZATION_F);

				events.ScheduleEvent(EVENT_BERSERK, TIMER_BERSERK);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                DoZoneInCombat();
                instance->SetBossState(DATA_JIN_ROKH_BREAKER, IN_PROGRESS);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            uint64 GetGUID(int32 id)
            {
                if (id == GUID_THROW_TARGET)
                {
                    return thunderingThrowTargetGuid;
                }

                return 0;
            }

            void JustDied(Unit* killer)
            {
                _JustDied();

                Talk(SAY_DEATH);

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FOCUSED_LIGHTNING_FIX);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FLUIDITY);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONDUCTIVE_WATER);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ELECTRIFIED_WATERS);

                DespawnCreaturesInArea(NPC_FOCUSED_LIGHTNING, me);
                DespawnCreaturesInArea(NPC_LIGHTNING_FISSURE, me);

                SetThunderingThrowTarget(NULL);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (!CheckForOutOfRange())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_THUNDERING_THROW:
                            Talk(SAY_THUNDERING_THROW);
                            Talk(ANN_THUNDERING_THROW);
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 0.0f, true))
                            {
                                DoCast(target, SPELL_THUNDERING_THROW);
                                SetThunderingThrowTarget(target);
                            }
				            events.ScheduleEvent(EVENT_THUNDERING_THROW_TOSS, TIMER_THUNDERING_THROW_TOSS);
				            events.ScheduleEvent(EVENT_THUNDERING_THROW, TIMER_THUNDERING_THROW_S);
                            break;

                        case EVENT_THUNDERING_THROW_TOSS:
                        {
                            if (Unit* target = GetThunderingThrowTarget())
                            {
                                target->RemoveAura(SPELL_RIDE_VEHICLE);
                                target->RemoveAura(SPELL_THUNDERING_THROW_GRAB);
                                me->RemoveAura(SPELL_RIDE_VEHICLE);

                                DoModifyThreatPercent(target, -100);

                                m_StatueController.ActivateNextStatueFor(target);

                                events.ScheduleEvent(EVENT_THUNDERING_THROW_DMG, TIMER_THUNDERING_THROW_DMG);
                            }
                            break;
                        }

                        case EVENT_THUNDERING_THROW_DMG:
                        {
                            Unit* throwTarget = GetThunderingThrowTarget();
                            Unit* statueTrigger = m_StatueController.GetLastActivatedStatue();
                            if (throwTarget && statueTrigger)
                            {
                                statueTrigger->CastSpell(throwTarget, SPELL_THUNDERING_THROW_PLR, true);
                                statueTrigger->CastSpell(throwTarget, SPELL_THUNDERING_THROW_AOE, true);
                            }

                            SetThunderingThrowTarget(NULL);
                            break;
                        }
                        case EVENT_STATIC_BURST:
                            Talk(SAY_STATIC_BURST);
                            DoCastVictim(SPELL_STATIC_BURST);
				            events.ScheduleEvent(EVENT_STATIC_BURST, TIMER_STATIC_BURST_S);
                            break;

                        case EVENT_FOCUSED_LIGHTNING:
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 1, 0.0f, true);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                            if (target)
                            {
                                Talk(SAY_FOCUSED_LIGHTNING);
                                DoCast(target, SPELL_FOCUSED_LIGHTNING);
                            }
				            events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, TIMER_FOCUSED_LIGHTNING_S);
                            break;
                        }
                        case EVENT_LIGHTNING_STORM_JUMP:
                            Talk(SAY_LIGHTNING_STORM);
                            Talk(ANN_LIGHTNING_STORM);
                            me->GetMotionMaster()->MoveJump(middlePos.GetPositionX(), middlePos.GetPositionY(), middlePos.GetPositionZ(), 15.0f, 15.0f);
				            events.ScheduleEvent(EVENT_LIGHTNING_STORM, TIMER_LIGHTNING_STORM);
				            events.ScheduleEvent(EVENT_LIGHTNING_STORM_JUMP, TIMER_LIGHTNING_STORM_JUMPS);
                            break;

                        case EVENT_LIGHTNING_STORM:
                        {
                            // Remove Pouring Water visual from triggers.
                            std::list<Creature*> triggerList;
                            GetCreatureListWithEntryInGrid(triggerList, me, NPC_STATUE_TRIGGER, 200.0f); // TODO: adjuste searche range
                            if (!triggerList.empty())
                            for (std::list<Creature*>::iterator statueTrigger = triggerList.begin(); statueTrigger != triggerList.end(); statueTrigger++)
                                if ((*statueTrigger)->HasAura(SPELL_CONDUCTIVE_VISUAL_2))
                                    (*statueTrigger)->RemoveAurasDueToSpell(SPELL_CONDUCTIVE_VISUAL_2);

                            // Remove Conductive Water visual from the npc's and add the Electrified one.
                            std::list<Creature*> waterList;
                            GetCreatureListWithEntryInGrid(waterList, me, NPC_CONDUCTIVE_WATER, 200.0f); // TODO: adjuste searche range
                            if (!waterList.empty())
                            for (std::list<Creature*>::iterator water = waterList.begin(); water != waterList.end(); water++)
                            {
                                if ((*water)->HasAura(SPELL_CONDUCTIVE_VISUAL))
                                {
                                    (*water)->RemoveAurasDueToSpell(SPELL_CONDUCTIVE_VISUAL);
                                    (*water)->AddAura(SPELL_CONDUCTIVE_VISUAL_3, (*water));
                                }
                            }
                            me->AddAura(SPELL_LIGHTNING_STORM_VIS, me);
                            if (Aura* visual = me->GetAura(SPELL_LIGHTNING_STORM_VIS))
                                visual->SetMaxDuration(15000);
                            me->AddAura(SPELL_LIGHTNING_STORM_VIS2, me);
                            if (Aura* visual2 = me->GetAura(SPELL_LIGHTNING_STORM_VIS2))
                                visual2->SetMaxDuration(15000);
                            DoCast(me, SPELL_LIGHTNING_STORM);
                            break;
                        }

                        case EVENT_IONIZATION:
                        {
                            DoCastAOE(SPELL_IONIZATION_AURA);
                            events.ScheduleEvent(EVENT_IONIZATION, TIMER_IONIZATION_S);
                            break;
                        }

                        case EVENT_BERSERK:
                            Talk(SAY_BERSERK);
				            DoCast(me, SPELL_BERSERK);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            bool CheckForOutOfRange()
            {
                if (me->GetDistance(me->GetHomePosition()) > 70.0f)
                {
                    EnterEvadeMode();
                    return false;
                }

                return true;
            }

            void SetThunderingThrowTarget(Unit* target)
            {
                thunderingThrowTargetGuid = target ? target->GetGUID() : 0;
            }

            Unit* GetThunderingThrowTarget() const
            {
                return sObjectAccessor->GetUnit(*me, thunderingThrowTargetGuid);
            }

        private:

            StatueController m_StatueController;
            uint64 thunderingThrowTargetGuid;
            bool introDone;
        };
};

class npc_mogu_statue_trigger : public CreatureScript
{
    public:
        npc_mogu_statue_trigger() : CreatureScript("npc_mogu_statue_trigger") { }

        struct npc_mogu_statue_trigger_AI : public ScriptedAI
        {
            npc_mogu_statue_trigger_AI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                Reset();
            }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                me->SetReactState(REACT_PASSIVE);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_STATUE_WATER:
                        me->AddAura(SPELL_CONDUCTIVE_VISUAL_2, me);
                        break;

                    default: break;
                }
            };

            void JustSummoned(Creature* summon)
            {
				summon->setActive(true);

				if (me->isInCombat())
					summon->SetInCombatWithZone();
            }

            void UpdateAI(uint32 const diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mogu_statue_trigger_AI(creature);
        }
};

class npc_jinrokh_the_breaker_conductive_water : public CreatureScript
{
    public:
        npc_jinrokh_the_breaker_conductive_water() : CreatureScript("npc_jinrokh_the_breaker_conductive_water") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_jinrokh_the_breaker_conductive_water_AI(creature);
        }

        struct npc_jinrokh_the_breaker_conductive_water_AI : public ScriptedAI
        {
            npc_jinrokh_the_breaker_conductive_water_AI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                me->AddAura(SPELL_CONDUCTIVE_VISUAL, me);
                me->AddAura(SPELL_CONDUCTIVE_WATER_GROW, me);
                me->SetReactState(REACT_PASSIVE);
                waterRadius = me->GetObjectScale() * 4.0f; // 4 yards original radius.
                events.ScheduleEvent(EVENT_BUFF_DEBUFF, TIMER_BUFF_DEBUFF);
            }

            float GetWaterRadius() const
            {
                return waterRadius;
            }

            void UpdateAI(uint32 const diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_BUFF_DEBUFF:
                        {
                            waterRadius = me->GetObjectScale() * 4.0f; 

                            std::list<Player*> players;
                            me->GetPlayerListInGrid(players, waterRadius);
                            if (!players.empty())
                            {
                                bool damageAura = me->HasAura(SPELL_CONDUCTIVE_VISUAL_3);

                                for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                                {
                                    me->AddAura(SPELL_CONDUCTIVE_WATER, (*itr));
                                    if (damageAura)
                                    {
                                        me->AddAura(SPELL_ELECTRIFIED_WATERS, (*itr));
                                    }
                                    else
                                    {
                                        me->AddAura(SPELL_FLUIDITY, (*itr));
                                    }
                                }
                            }

                            events.ScheduleEvent(EVENT_BUFF_DEBUFF, TIMER_BUFF_DEBUFF);
                            break;
                        }

                        default: break;
                    }
                }
            }
        private:
            float waterRadius;
        };        
};

typedef npc_jinrokh_the_breaker_conductive_water::npc_jinrokh_the_breaker_conductive_water_AI WaterAI;

class npc_jinrokh_the_breaker_focused_lightning : public CreatureScript
{
    public:
        npc_jinrokh_the_breaker_focused_lightning() : CreatureScript("npc_jinrokh_the_breaker_focused_lightning") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_jinrokh_the_breaker_focused_lightningAI(creature);
        }

        struct npc_jinrokh_the_breaker_focused_lightningAI : public ScriptedAI
        {
            npc_jinrokh_the_breaker_focused_lightningAI(Creature* creature) : ScriptedAI(creature) 
            { 
                me->SetReactState(REACT_PASSIVE);
                exploded = false;
                playerGuid = 0;
                waterGuid = 0;
                me->SetSpeed(MOVE_RUN, 0.1f, true);
                me->SetSpeed(MOVE_WALK, 0.1f, true);
            }

            uint64 GetGUID(int32 id)
            {
                switch (id)
                {
                    case GUID_PLAYER: return playerGuid;
                    case GUID_WATER: return waterGuid;
                }
                return 0;
            }

            void IsSummonedBy(Unit* summoner)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->AddAura(SPELL_FOCUSED_LIGHTNING_SPD, me);
                me->AddAura(SPELL_FOCUSED_LIGHTNING_VIS, me);
                
                if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 150.0f, true))
                {
                    summoner->ToCreature()->AI()->Talk(ANN_FOCUSED_LIGHTNING, target->GetGUID());
                    DoCast(target, SPELL_FOCUSED_LIGHTNING_FIX, true);
                    playerGuid = target->GetGUID();
                }

                events.ScheduleEvent(EVENT_START_MOVE, 2000);
            }

            void JustSummoned(Creature* summon)
            {
				summon->setActive(true);

				if (me->isInCombat())
					summon->SetInCombatWithZone();
            }

            void UpdateAI(uint32 const diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_START_MOVE:
                            me->ClearUnitState(UNIT_STATE_CASTING);
                            if (Player* player = me->GetPlayer(*me, playerGuid))
                            {
                                me->GetMotionMaster()->MoveFollowExact(player, 0, 0.0f);
                            }
                            events.ScheduleEvent(EVENT_CHECK_TARGETS, 100);
                            events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING_DMG, TIMER_FOCUSED_LIGHTNING_DMG);
                            break;
                        case EVENT_CHECK_TARGETS:
                        {
                            CheckAchievement();

                            if (DoWaterExplosion())
                            {
                                exploded = true;
                                me->DespawnOrUnsummon(200);
                                events.Reset();
                                return;
                            }
                            if (DoFissureExplosion())
                            {
                                exploded = true;
                                me->DespawnOrUnsummon(200);
                                events.Reset();
                                return;
                            }
                            if (DoPlayerExplosion())
                            {
                                exploded = true;
                                me->DespawnOrUnsummon(200);
                                events.Reset();
                                return;
                            }
                            events.ScheduleEvent(EVENT_CHECK_TARGETS, 200);
                            break;
                        }
                        case EVENT_FOCUSED_LIGHTNING_DMG:
                            DoCast(me, SPELL_FOCUSED_LIGHTNING_DMG);
                            events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING_DMG, TIMER_FOCUSED_LIGHTNING_DMG);
                            break;
                    }
                }
            }

        private:

            uint64 playerGuid;
            uint64 waterGuid;
            bool exploded;

        private:

            bool DoWaterExplosion()
            {
                if (Creature* pWater = me->FindNearestCreature(NPC_CONDUCTIVE_WATER, 50.0f))
                {
                    float radius = CAST_AI(WaterAI, pWater->GetAI())->GetWaterRadius();
                    if (me->IsWithinDist(pWater, radius))
                    {
                        if (pWater->HasAura(SPELL_CONDUCTIVE_VISUAL_3))
                        {
                            // aoe 8 yards
                            DoCastAOE(SPELL_FOCUSED_LIGHTNING_DMG_2, true);
                        }
                        else
                        {
                            // aoe onlu for players standing in water
                            waterGuid = pWater->GetGUID();
                            DoCastAOE(SPELL_FOCUSED_LIGHTNING_DMG_1, true);
                        }
                        return true;
                    }
                }
                return false;
            }

            bool DoFissureExplosion()
            {
                if (Creature* pFissure = me->FindNearestCreature(NPC_LIGHTNING_FISSURE, 2.0f))
                {
                    DoCastAOE(SPELL_IMPLOSION);
                    pFissure->DespawnOrUnsummon(200);
                    return true;
                }
                return false;
            }

            bool DoPlayerExplosion()
            {
                Player* target = me->GetPlayer(*me, playerGuid);
                if (!target)
                    return true;

                if (target->IsWithinDistInMap(me, 3.0f))
                {
                    // if player is in conductive water
                    target->RemoveAura(SPELL_FOCUSED_LIGHTNING_FIX);
                    me->CastSpell(target, SPELL_FOCUSED_LIGHTNING_DET, true);
                    me->CastSpell(me, SPELL_LIGHTNING_FISSURE_SUM, true);
                    return true;
                }

                return false;
            }

            void CheckAchievement()
            {
                Creature* creature = NULL;
                FocusedLightningCheck checker(me);
                JadeCore::CreatureLastSearcher<FocusedLightningCheck> searcher(me, creature, checker);
                me->VisitNearbyObject(2.0f, searcher);
                if (creature)
                {
                    if (InstanceScript* pInstance = me->GetInstanceScript())
                    {
                        pInstance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT, 0, 0, me);
                    }
                }
            }

        private:

            class FocusedLightningCheck
            {
                public:
                    FocusedLightningCheck(Unit* source) : i_source(source)
                    {
                    }
                    bool operator()(Creature* u)
                    {
                        if (u == i_source)
                            return false;

                        if (u->GetEntry() != NPC_FOCUSED_LIGHTNING)
                            return false;

                        if (i_source->GetDistance(u) > 2.0f)
                            return false;

                        return true;
                    }
                private:
                    Unit* const i_source;
            };
        };
};

class npc_jinrokh_the_breaker_lightning_fissure : public CreatureScript
{
    public:
        npc_jinrokh_the_breaker_lightning_fissure() : CreatureScript("npc_jinrokh_the_breaker_lightning_fissure") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_jinrokh_the_breaker_lightning_fissureAI(creature);
        }

        struct npc_jinrokh_the_breaker_lightning_fissureAI : public Scripted_NoMovementAI
        {
            npc_jinrokh_the_breaker_lightning_fissureAI(Creature* creature) : Scripted_NoMovementAI(creature) 
            { 
                
            }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->AddAura(SPELL_LIGHTNING_FISSURE_VIS, me);
                me->AddAura(SPELL_LIGHTNING_FISSURE_DMG, me);
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
            }
        };
};

// Thundering Throw (main cast spell) - 137180
class spell_jinrokh_the_breaker_thundering_throw : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_thundering_throw() : SpellScriptLoader("spell_jinrokh_the_breaker_thundering_throw") { }

        class spell_jinrokh_the_breaker_thundering_throw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jinrokh_the_breaker_thundering_throw_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                caster->CastSpell(target, SPELL_THUNDERING_THROW_GRAB, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_jinrokh_the_breaker_thundering_throw_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_jinrokh_the_breaker_thundering_throw_SpellScript();
        }
};

// Thundering Throw (vehicle grab spell) - 137161
class spell_jinrokh_the_breaker_thundering_throw_grab : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_thundering_throw_grab() : SpellScriptLoader("spell_jinrokh_the_breaker_thundering_throw_grab") { }

        class spell_jinrokh_the_breaker_thundering_throw_grab_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jinrokh_the_breaker_thundering_throw_grab_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                //target->CastSpell(caster, SPELL_RIDE_VEHICLE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_jinrokh_the_breaker_thundering_throw_grab_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_jinrokh_the_breaker_thundering_throw_grab_SpellScript();
        }
};

// Thundering Throw (AOE) - 137167
class spell_jinrokh_the_breaker_thundering_throw_aoe : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_thundering_throw_aoe() : SpellScriptLoader("spell_jinrokh_the_breaker_thundering_throw_aoe") { }

        class spell_jinrokh_the_breaker_thundering_throw_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jinrokh_the_breaker_thundering_throw_aoe_SpellScript);

            void HandleHit(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    if (GetHitUnit()->GetGUID() != pCreature->AI()->GetGUID(GUID_THROW_TARGET))
                    {
                        pCreature->AddAura(SPELL_THUNDERING_THROW_STUN, GetHitUnit());
                    }
                }
            }

            void CalculateDamage(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    if (GetHitUnit()->GetGUID() == pCreature->AI()->GetGUID(GUID_THROW_TARGET))
                    {
                        SetHitDamage(0);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_jinrokh_the_breaker_thundering_throw_aoe_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                OnEffectHitTarget += SpellEffectFn(spell_jinrokh_the_breaker_thundering_throw_aoe_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_jinrokh_the_breaker_thundering_throw_aoe_SpellScript();
        }
};

// Static Burst 137162.
class spell_jinrokh_the_breaker_static_burst : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_static_burst() : SpellScriptLoader("spell_jinrokh_the_breaker_static_burst") { }

        class spell_jinrokh_the_breaker_static_burst_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_jinrokh_the_breaker_static_burst_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();

                if (!caster || !caster->getVictim())
                    return;

                caster->CastCustomSpell(SPELL_STATIC_WOUND, SPELLVALUE_AURA_STACK, 10, caster->getVictim());
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_jinrokh_the_breaker_static_burst_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_jinrokh_the_breaker_static_burst_AuraScript();
        }
};

// Static Wound 138349.
class spell_jinrokh_the_breaker_static_wound : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_static_wound() : SpellScriptLoader("spell_jinrokh_the_breaker_static_wound") { }

        class spell_jinrokh_the_breaker_static_wound_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_jinrokh_the_breaker_static_wound_AuraScript)

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo & eventInfo)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                Aura* aur = GetAura();
                if (!aur)
                    return;

                //uint8 stacks = aur->GetStackAmount();
                // GetAmount returns amount with stacks
                int32 damage = aurEff->GetAmount();

                GetUnitOwner()->CastCustomSpell(GetUnitOwner(), SPELL_STATIC_WOUND_DMG, &damage, 0, 0, 0, 0, 0, true);
            }

            void HandlePeriodic(AuraEffect const* /*aurEff*/)
            {
                Aura* aur = GetAura();
                if (!aur)
                    return;

                aur->ModStackAmount(-1);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_jinrokh_the_breaker_static_wound_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_jinrokh_the_breaker_static_wound_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_jinrokh_the_breaker_static_wound_AuraScript();
        }
};

class spell_jinrokh_the_breaker_static_wound_dmg : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_static_wound_dmg() : SpellScriptLoader("spell_jinrokh_the_breaker_static_wound_dmg") { }

        class spell_jinrokh_the_breaker_static_wound_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jinrokh_the_breaker_static_wound_dmg_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (GetCaster()->GetGUID() != GetHitUnit()->GetGUID())
                {
                    SetHitDamage(GetHitDamage() / 3);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_jinrokh_the_breaker_static_wound_dmg_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_jinrokh_the_breaker_static_wound_dmg_SpellScript();
        }
};

// Focus Lightning (main cast spell) - 137399.
class spell_jinrokh_the_breaker_focused_lightning : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_focused_lightning() : SpellScriptLoader("spell_jinrokh_the_breaker_focused_lightning") { }

        class spell_jinrokh_the_breaker_focused_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jinrokh_the_breaker_focused_lightning_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->SummonCreature(NPC_FOCUSED_LIGHTNING, GetHitUnit()->GetPositionX() + 5.0f, GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_jinrokh_the_breaker_focused_lightning_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_jinrokh_the_breaker_focused_lightning_SpellScript();
        }
};

// Lightning Storm (Heroic dummy damage) - 140819
class spell_jinrokh_the_breaker_lightning_storm_heroic_dummy : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_lightning_storm_heroic_dummy() : SpellScriptLoader("spell_jinrokh_the_breaker_lightning_storm_heroic_dummy") { }

        class spell_jinrokh_the_breaker_lightning_storm_heroic_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jinrokh_the_breaker_lightning_storm_heroic_dummy_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();

                if (!target)
                    return;

                Unit* caster = target->FindNearestCreature(NPC_JIN_ROKH_BREAKER, 200.0f, true) ? target->FindNearestCreature(NPC_JIN_ROKH_BREAKER, 200.0f, true)->ToUnit() : NULL;

                if (!caster)
                    return;

                caster->CastSpell(caster, SPELL_LIGHTNING_STRIKE_VIS, true);

                // Only happens on Heroic difficulties.
                if (!caster->GetMap()->IsHeroic())
                    return;

                if (urand(1, 4) == 1) // 25 % chance means 1 in 4 ticks and should comply with the restrictions, we don't want to kill everybody :).
                {
                    caster->CastSpell(target, SPELL_LIGHTNING_STORM_H_DMG, true);

                    // Cast Lightning Diffusion on players in the back of the target in a line.
                    Map::PlayerList const &PlayerList = caster->GetMap()->GetPlayers();
                    if (!PlayerList.isEmpty())
                    {
                        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        {
                            if (Player* player = i->getSource())
                            {
                                if (player->ToUnit()->isInBack(target))
                                {
                                    caster->CastSpell(player, SPELL_LIGHTNING_DIFFUSION, true);
                                    break; // break the loop, the spell is chained.
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_jinrokh_the_breaker_lightning_storm_heroic_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_jinrokh_the_breaker_lightning_storm_heroic_dummy_SpellScript();
        }
};

// Ionization aura 138732.
class spell_jinrokh_the_breaker_ionization : public SpellScriptLoader
{
    public:
        spell_jinrokh_the_breaker_ionization() : SpellScriptLoader("spell_jinrokh_the_breaker_ionization") { }

        class spell_jinrokh_the_breaker_ionization_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jinrokh_the_breaker_ionization_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                targets.remove_if(TanksCheck());

                // in 25 heroic
                if (targets.size() > 13)
                {
                    JadeCore::Containers::RandomResizeList(targets, 13);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_jinrokh_the_breaker_ionization_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }

        private:

            class TanksCheck
            {
                public:
                    TanksCheck() {}

                    bool operator()(WorldObject* unit) const
                    {
                        if (!unit->IsPlayer())
                            return true;

                        if (unit->ToPlayer()->GetRoleForGroup() == ROLES_TANK)
                            return true;

                        return false;
                    }

                private:
                    uint64 m_WaterGuid;
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_jinrokh_the_breaker_ionization_SpellScript();
        }

        class spell_jinrokh_the_breaker_ionization_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_jinrokh_the_breaker_ionization_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                int32 damage = aurEff->GetAmount();
                
                GetCaster()->CastCustomSpell(GetUnitOwner(), SPELL_IONIZATION_DMG, &damage, NULL, NULL, NULL, NULL, NULL, true);
                
                if (GetUnitOwner()->HasAura(SPELL_CONDUCTIVE_WATER))
                {
                    GetUnitOwner()->CastCustomSpell(GetUnitOwner(), SPELL_IONIZATION_CONDUCTION, &damage, NULL, NULL, NULL, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_jinrokh_the_breaker_ionization_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_jinrokh_the_breaker_ionization_AuraScript();
        }
};

// TODO: move to better place
class WaterGuidCheck
{
    public:
        WaterGuidCheck(uint64 waterGuid) : m_WaterGuid(waterGuid) {}

        bool operator()(WorldObject* unit) const
        {
            if (!unit->ToUnit())
                return true;

            if (!unit->ToUnit()->HasAura(SPELL_CONDUCTIVE_WATER, m_WaterGuid))
                return true;

            return false;
        }

    private:
        uint64 m_WaterGuid;
};

class spell_jinrokh_the_breaker_focused_lightning_conduction : public SpellScriptLoader
{
public:
    spell_jinrokh_the_breaker_focused_lightning_conduction() : SpellScriptLoader("spell_jinrokh_the_breaker_focused_lightning_conduction") { }

    class spell_jinrokh_the_breaker_focused_lightning_conduction_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_jinrokh_the_breaker_focused_lightning_conduction_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            Creature* pLightning = GetCaster()->ToCreature();
            if (!pLightning)
                return;

            uint64 waterGuid = pLightning->AI()->GetGUID(GUID_WATER);
            if (!waterGuid)
            {
                targets.clear();
                return;
            }

            targets.remove_if(WaterGuidCheck(waterGuid));
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_jinrokh_the_breaker_focused_lightning_conduction_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_jinrokh_the_breaker_focused_lightning_conduction_SpellScript();
    }
};

class spell_jinrokh_the_breaker_ionization_conduction : public SpellScriptLoader
{
public:
    spell_jinrokh_the_breaker_ionization_conduction() : SpellScriptLoader("spell_jinrokh_the_breaker_ionization_conduction") { }

    class spell_jinrokh_the_breaker_ionization_conduction_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_jinrokh_the_breaker_ionization_conduction_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            Aura const* aurWater = GetCaster()->GetAura(SPELL_CONDUCTIVE_WATER);
            if (!aurWater)
            {
                targets.clear();
                return;
            }

            uint64 waterGuid = aurWater->GetCasterGUID();
            if (!waterGuid)
            {
                targets.clear();
                return;
            }

            targets.remove_if(WaterGuidCheck(waterGuid));
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_jinrokh_the_breaker_ionization_conduction_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_jinrokh_the_breaker_ionization_conduction_SpellScript();
    }
};

void AddSC_boss_jinrokh_the_breaker()
{
    new boss_jinrokh_the_breaker();
    new npc_mogu_statue_trigger();
    new npc_jinrokh_the_breaker_conductive_water();
    new npc_jinrokh_the_breaker_focused_lightning();
    new npc_jinrokh_the_breaker_lightning_fissure();
    new spell_jinrokh_the_breaker_thundering_throw();
    new spell_jinrokh_the_breaker_thundering_throw_grab();
    new spell_jinrokh_the_breaker_thundering_throw_aoe();
    new spell_jinrokh_the_breaker_static_burst();
    new spell_jinrokh_the_breaker_static_wound();
    new spell_jinrokh_the_breaker_static_wound_dmg();               // 138389
    new spell_jinrokh_the_breaker_focused_lightning();
    new spell_jinrokh_the_breaker_lightning_storm_heroic_dummy();
    new spell_jinrokh_the_breaker_ionization();                     // 138732
    new spell_jinrokh_the_breaker_focused_lightning_conduction();
    new spell_jinrokh_the_breaker_ionization_conduction();          // 138743
}

#include "throne_of_thunder.hpp"
#include "Vehicle.h"

enum ScriptedTexts
{
    SAY_ROSHAK_DIED     = 0,
    SAY_QUETZAL_DIED    = 1,
    SAY_DAMREN_DIED     = 2,
    SAY_AGGRO           = 3,
    SAY_DAMREN          = 4,
    SAY_QUETZAL         = 5,
    SAY_ROSHAK          = 6,
    SAY_DEATH           = 7,
    SAY_INTRO_1         = 8,
    SAY_INTRO_2         = 9,
    SAY_KILL            = 10,
    SAY_MANCHU          = 11,
    SAY_MINION_DIED_1   = 12,
    SAY_MINION_DIED_2   = 13,
    SAY_PHASE_2         = 14,
    SAY_WIPE            = 15,
    SAY_WEISHENG        = 16,
};

enum Spells
{
    SPELL_BERSERK               = 137697,
    SPELL_EJECT_ALL_PASSENGERS  = 50630,
    SPELL_ZERO_POWER            = 96301,

    // Iron Qon

    SPELL_THROW_SPEAR_AOE       = 136176, // aoe targetting
    SPELL_THROW_SPEAR           = 134925,
    SPELL_THROW_SPEAR_DMG       = 134926,
    
    SPELL_DISARM_SELF           = 135104,

    SPELL_IRON_QON_SPEAR        = 134927,

    SPELL_RETURN_SPEAR_SCRIPT   = 137712,
    SPELL_RETURN_SPEAR_DUMMY    = 137713,
    SPELL_RETURN_SPEAR_JUMP     = 137714,

    SPELL_IMPALE                    = 134691,

    SPELL_FIST_SMASH                = 136146,
    SPELL_FIST_SMASH_DMG            = 136147,

    SPELL_RISING_ANGER              = 136324,
    SPELL_RISING_ANGER_AURA         = 136323,

    SPELL_IGNITE_CYCLONE            = 136139,
    SPELL_FREEZE_CYCLONE            = 136153,

    // rushing wind 136161 136590
    SPELL_BURNING_BLAST_DMG         = 136925,

    // Ro'shak

    SPELL_MOLTEN_INFERNO            = 134664,
    SPELL_MOLTEN_OVERLOAD           = 137221,

    SPELL_UNLEASHED_FLAME_FORCE     = 139111,
    SPELL_UNLEASHED_FLAME_DUMMY_1   = 139112, // triggered by 139111
    SPELL_UNLEASHED_FLAME_PERIODIC  = 137991,
    SPELL_UNLEASHED_FLAME_DUMMY_2   = 134621, // triggered by 137991
    SPELL_UNLEASHED_FLAME_DUMMY_3   = 139140, // aoe which targets players, has basepoints 139138
    SPELL_UNLEASHED_FLAME_MISSILE_1 = 134611,
    SPELL_UNLEASHED_FLAME_MISSILE_2 = 139138,
    SPELL_UNLEASHED_FLAME_DMG       = 134628,

    SPELL_SCORCHED                  = 134647, // triggered by 134611 and 139138

    SPELL_BURNING_CINDERS_AOE       = 136330, // missile flies to the spear
    SPELL_BURNING_CINDERS           = 134758,
    SPELL_BURNING_CINDERS_AURA      = 135052,
    SPELL_BURNING_CINDERS_AURA_DMG  = 137668, // triggered by 137052

    SPELL_GROUND_RUPTURE            = 134759, // triggered by 134758

    // Quetzal

    SPELL_STORM_CLOUD_AOE           = 136419, // missile flies to the spear
    SPELL_STORM_CLOUD               = 136421,
    SPELL_STORM_CLOUD_AURA          = 136425,
    SPELL_STORM_CLOUD_AURA_DMG      = 137669, // triggered by 136425
    SPELL_ELECTRIFIED               = 136615,

    SPELL_STORM_SURGE               = 136498, // triggered by 136421
    

    SPELL_ARC_LIGHTNING_FORCE       = 139106,
    SPELL_LIGHTNING_STORM           = 136192, // main debuff, triggered by 139106
    SPELL_ARC_LIGHTNING_DUMMY       = 138473, // with basepoints 136193
    SPELL_ARC_LIGHTNING_AURA_DMG    = 136193,
    SPELL_ARC_LIGHTNING_SUMMON      = 136198,
    SPELL_ARC_LIGHTNING_REMOVE      = 136199,
    SPELL_ARC_LIGHTNING_PERIODIC_1  = 136224, // for visual
    SPELL_ARC_LIGHTNING_DUMMY_1     = 136226,
    SPELL_ARC_LIGHTNING_PERIODIC_2  = 138434,
    SPELL_ARC_LIGHTNING_DUMMY_2     = 138435,

    SPELL_RUSHING_WINDS_AREATRIGGER = 136574,
    SPELL_RUSHING_WINDS_AOE         = 136591,
    SPELL_RUSHING_WINDS_JUMP        = 136592,
    SPELL_RUSHING_WINDS_AURA        = 136654, // if player is hitted by a tornado
    SPELL_WINDSTORM_AURA            = 136577,
    // 137656
    // 137480

    // Dam'ren

    SPELL_FROZEN_BLOOD_AOE          = 136449,
    SPELL_FROZEN_BLOOD              = 136451,
    SPELL_FROZEN_BLOOD_DMG          = 136520, // triggered by 136451
    SPELL_FROZEN_BLOOD_DEBUFF       = 136452,
    SPELL_FROZEN_BLOOD_SLOW         = 137664, // triggered by 136452

    SPELL_DEAD_ZONE_LEFT            = 135199,
    SPELL_DEAD_ZONE_RIGHT           = 135223,
    SPELL_DEAD_ZONE_BACK            = 135225,
    SPELL_DEAD_ZONE_FRONT           = 135361,

    SPELL_DEAD_ZONE_FRONT_RIGHT     = 137226,
    SPELL_DEAD_ZONE_LEFT_RIGHT      = 137227,
    SPELL_DEAD_ZONE_FRONT_LEFT      = 137228,
    SPELL_DEAD_ZONE_FRONT_BACK      = 137229,
    SPELL_DEAD_ZONE_LEFT_BACK       = 137230,
    SPELL_DEAD_ZONE_BACK_RIGHT      = 137231,
    SPELL_DEAD_ZONE_SCRIPT          = 137232,

    SPELL_FROZEN_RESILIENCE         = 135144,
    SPELL_FROZEN_RESILIENCE_DMG     = 135142,
};

enum Adds
{
    NPC_IRON_QON_SPEAR_1    = 68480,
    NPC_IRON_QON_SPEAR_2    = 69159,
    NPC_LIGHTNING_CLOUD     = 70660,
};

enum Events
{
    EVENT_BERSERK       = 1,
    EVENT_THROW_SPEAR,
    EVENT_RETURN_SPEAR,
    EVENT_IMPALE,
    EVENT_SIT_ON_QUETZAL,
    EVENT_SIT_ON_DAMREN,

    EVENT_MOLTEN_INFERNO,
    EVENT_UNLEASHED_FLAME,
    EVENT_BURNING_CINDERS,
    EVENT_REGENERATE,
    EVENT_CHECK_PLAYERS,

    EVENT_ARC_LIGHTNING,
    EVENT_RUSHING_WINDS_PREPARE,
    EVENT_RUSHING_WINDS_BEGIN,
    EVENT_RUSHING_WINDS_END,

    EVENT_DEAD_ZONE,
    EVENT_DEAD_ZONE_ENDS,

    EVENT_FIST_SMASH,
};

enum Actions
{
    ACTION_START_COMBAT = 1,
    ACTION_ROSHAK_DIED,
    ACTION_QUETZAL_DIED,
    ACTION_DAMREN_DIED,
    ACTION_THROWN_SPEAR,
    ACTION_IGNITE_SPEAR,
    ACTION_STRIKE_SPEAR,
    ACTION_ACTIVATE_QUETZAL,
    ACTION_ACTIVATE_DAMREN,
    ACTION_FREEZE_SPEAR,
    ACTION_RUSHING_WINDS_START,
    ACTION_RUSHING_WINDS_END,

    ACTION_RELEASE_TARGET,
};

enum Points
{
    POINT_RUSHING_WINDS = 1,
    POINT_RUSHING_WINDS_END,
};

enum Quilens
{
    ROSHAK  = 1,
    QUETZAL,
    DAMREN,
};

enum Phases
{
    PHASE_NONE  = 0,
    PHASE_ROSHAK,
    PHASE_QUETZAL,
    PHASE_DAMREN,
    PHASE_LAST,
};

const Position ironQonPos[3] = 
{
    {6045.43f, 4598.80f, 146.51f, 1.60f}, // iron qon,  center
    {6065.55f, 4606.11f, 146.55f, 1.60f}, // frost add, right from the boss
    {6025.83f, 4605.50f, 146.55f, 1.60f}  // air add, left from boss
};

const Position centerRoomPos = {6045.66f, 4671.58f, 153.0f, 1.72f};

#define MAX_DEAD_ZONE_SPELLS 6
const uint32 deadZoneSpells[MAX_DEAD_ZONE_SPELLS] = 
{
    SPELL_DEAD_ZONE_FRONT_RIGHT,
    SPELL_DEAD_ZONE_LEFT_RIGHT,
    SPELL_DEAD_ZONE_FRONT_LEFT,
    SPELL_DEAD_ZONE_FRONT_BACK,
    SPELL_DEAD_ZONE_LEFT_BACK,
    SPELL_DEAD_ZONE_BACK_RIGHT,
};

class boss_iron_qon : public CreatureScript
{
    public:
        boss_iron_qon() : CreatureScript("boss_iron_qon") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_iron_qonAI(creature);
        }

        struct boss_iron_qonAI : public BossAI
        {
            boss_iron_qonAI(Creature* creature) : BossAI(creature, DATA_IRON_QON)
            {
                ApplyAllImmunities(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 8);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 8);

                me->setActive(true);
            }

            void Reset()
            {
                _Reset();

                m_RoshakGUID = 0;
                m_QuetzalGUID = 0;
                m_DamrenGUID = 0;
                m_Phase = PHASE_NONE;

                DespawnCreaturesInArea(NPC_LIGHTNING_CLOUD, me);

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                //me->ExitVehicle(&ironQonPos[0]);
                SpawnQuilens();
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_START_COMBAT:
                        DoZoneInCombat();
                        break;
                    case ACTION_ROSHAK_DIED:
                        Talk(SAY_ROSHAK_DIED);

                        if (Creature* pQuilen = GetQuilen(ROSHAK))
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pQuilen);

                        CancelCommonEvents();

                        me->_ExitVehicle();

                        m_Phase = PHASE_QUETZAL;
                        events.ScheduleEvent(EVENT_SIT_ON_QUETZAL, 3000);
                        break;
                    case ACTION_QUETZAL_DIED:
                        Talk(SAY_QUETZAL_DIED);

                        if (Creature* pQuilen = GetQuilen(QUETZAL))
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pQuilen);

                        CancelCommonEvents();

                        me->_ExitVehicle();

                        m_Phase = PHASE_DAMREN;
                        events.ScheduleEvent(EVENT_SIT_ON_DAMREN, 3000);
                        break;
                    case ACTION_DAMREN_DIED:
                        Talk(SAY_DAMREN_DIED);

                        if (Creature* pQuilen = GetQuilen(DAMREN))
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, pQuilen);

                        CancelCommonEvents();

                        me->_ExitVehicle();

                        m_Phase = PHASE_LAST;

                        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                        me->SetReactState(REACT_AGGRESSIVE);
                        me->AddAura(SPELL_RISING_ANGER, me);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                        events.ScheduleEvent(EVENT_FIST_SMASH, 22500);
                        break;
                    case ACTION_RUSHING_WINDS_START:
                        CancelCommonEvents();
                        break;
                    case ACTION_RUSHING_WINDS_END:
                        StartCommonEvents();
                        break;
                }
            }

            void EnterCombat(Unit* attacker)
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_BERSERK, 12 * MINUTE * IN_MILLISECONDS);
                
                m_Phase = PHASE_ROSHAK;

                if (Creature* pQuilen = GetQuilen(ROSHAK))
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, pQuilen);

                instance->SetBossState(DATA_IRON_QON, IN_PROGRESS);
                //DoZoneInCombat();
            }

            void EnterEvadeMode()
            {
                Talk(SAY_WIPE);

                me->_ExitVehicle();

                BossAI::EnterEvadeMode();
            }

            void JustDied(Unit* killer)
            {
                _JustDied();

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
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
                        case EVENT_BERSERK:
                        {
                            if (Creature* pQuilen = GetQuilen(ROSHAK))
                                me->AddAura(SPELL_BERSERK, pQuilen);

                            if (Creature* pQuilen = GetQuilen(QUETZAL))
                                me->AddAura(SPELL_BERSERK, pQuilen);

                            if (Creature* pQuilen = GetQuilen(DAMREN))
                                me->AddAura(SPELL_BERSERK, pQuilen);

                            me->AddAura(SPELL_BERSERK, me);

                            break;
                        }

                        case EVENT_IMPALE:
                        {
                            Unit* target = NULL;
                            if (Creature* pQuilen = me->GetVehicleCreatureBase())
                            {
                                target = pQuilen->getVictim();
                            }
                            else
                            {
                                target = me->getVictim();
                            }

                            if (target)
                                DoCast(target, SPELL_IMPALE);

                            events.ScheduleEvent(EVENT_IMPALE, 20000);

                            break;
                        }

                        case EVENT_THROW_SPEAR:
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, -20.0f, true);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);

                            if (target)
                            {
                                DoCast(target, SPELL_THROW_SPEAR);

                                SendActionThrownSpear();

                                events.ScheduleEvent(EVENT_RETURN_SPEAR, 7000);
                            }

                            events.ScheduleEvent(EVENT_THROW_SPEAR, 30000);
                            break;
                        }

                        case EVENT_RETURN_SPEAR:
                            if (Creature* pSpear = me->FindNearestCreature(NPC_IRON_QON_SPEAR_1, 100.0f))
                            {
                                DoCast(pSpear, SPELL_RETURN_SPEAR_SCRIPT, true);
                            }
                            break;
                        case EVENT_SIT_ON_QUETZAL:
                            if (Creature* pQuetzal = GetQuilen(QUETZAL))
                            {
                                pQuetzal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                                pQuetzal->AI()->DoAction(ACTION_ACTIVATE_QUETZAL);
                                me->EnterVehicle(pQuetzal);

                                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, pQuetzal);
                            }
                            StartCommonEvents();
                            break;
                        case EVENT_SIT_ON_DAMREN:
                            if (Creature* pDamren = GetQuilen(DAMREN))
                            {
                                pDamren->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                                pDamren->AI()->DoAction(ACTION_ACTIVATE_DAMREN);
                                me->EnterVehicle(pDamren);

                                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, pDamren);
                            }
                            StartCommonEvents();
                            break;
                        case EVENT_FIST_SMASH:
                            DoCast(me, SPELL_FIST_SMASH);
                            events.ScheduleEvent(EVENT_FIST_SMASH, 20000);
                            break;
                    }
                }

                if (m_Phase == PHASE_LAST)
                    DoMeleeAttackIfReady();
            }

        private:

            uint64 m_RoshakGUID;
            uint64 m_QuetzalGUID;
            uint64 m_DamrenGUID;
            Phases m_Phase;

        private:

            void StartCommonEvents()
            {
                events.ScheduleEvent(EVENT_IMPALE, urand(15000, 20000));
                events.ScheduleEvent(EVENT_THROW_SPEAR, 10000);
            }

            void CancelCommonEvents()
            {
                events.CancelEvent(EVENT_IMPALE);
                events.CancelEvent(EVENT_THROW_SPEAR);
            }

        private:

            void SpawnQuilens()
            {
                if (Creature* pRoshak = me->SummonCreature(NPC_ROSHAK, ironQonPos[0]))
                {
                    m_RoshakGUID = pRoshak->GetGUID();
                }

                if (Creature* pQuetzal = me->SummonCreature(NPC_QUETZAL, ironQonPos[2]))
                {
                    m_QuetzalGUID = pQuetzal->GetGUID();
                }

                if (Creature* pDamren = me->SummonCreature(NPC_DAMREN, ironQonPos[1]))
                {
                    m_DamrenGUID = pDamren->GetGUID();
                }
            }

            Creature* GetQuilen(Quilens quilen)
            {
                switch (quilen)
                {
                    case ROSHAK:
                        return instance->instance->GetCreature(m_RoshakGUID);
                    case QUETZAL:
                        return instance->instance->GetCreature(m_QuetzalGUID);
                    case DAMREN:
                        return instance->instance->GetCreature(m_DamrenGUID);
                }

                return NULL;
            }

            void SendActionThrownSpear()
            {
                Creature* pQuilen = NULL;

                switch (m_Phase)
                {
                    case PHASE_ROSHAK: pQuilen = GetQuilen(ROSHAK); break;
                    case PHASE_QUETZAL: pQuilen = GetQuilen(QUETZAL); break;
                    case PHASE_DAMREN: pQuilen = GetQuilen(DAMREN); break;
                }

                if (pQuilen)
                    pQuilen->AI()->DoAction(ACTION_THROWN_SPEAR);
            }

        };
};

class npc_iron_qon_roshak : public CreatureScript
{
    public:
        npc_iron_qon_roshak() : CreatureScript("npc_iron_qon_roshak") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_qon_roshakAI(creature);
        }

        struct npc_iron_qon_roshakAI : public ScriptedAI
        {
            npc_iron_qon_roshakAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 5);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 5);

                me->setActive(true);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_DEFENSIVE);
                //me->SetReactState(REACT_PASSIVE);

                me->setPowerType(POWER_MANA);
                me->SetMaxPower(POWER_MANA, 100);
                me->SetPower(POWER_MANA, 0);

                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);

                me->AddAura(SPELL_ZERO_POWER, me);
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_REGENERATE, 500);
                events.ScheduleEvent(EVENT_MOLTEN_INFERNO, urand(1000, 5000));
                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 2000);

                SetBossVehicle();

                SendStartCombat();
            }

            void KilledUnit(Unit* target)
            {
                SendKilledUnit(target);
            }

            void JustDied(Unit* killer)
            {
                if (Vehicle* pVehicle = me->GetVehicleKit())
                {
                    pVehicle->RemoveAllPassengers();

                    SendDied();
                }
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_THROWN_SPEAR)
                {
                    events.ScheduleEvent(EVENT_THROW_SPEAR, 3000);
                }
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
                        case EVENT_REGENERATE:
                        {
                            int32 power = me->GetPower(POWER_MANA);
                            if (power >= 100)
                            {
                                DoCast(me, SPELL_MOLTEN_OVERLOAD);
                            }
                            else
                            {
                                me->SetPower(POWER_MANA, power + 5);
                            }
                            events.ScheduleEvent(EVENT_REGENERATE, 1000);
                            break;
                        }

                        case EVENT_MOLTEN_INFERNO:
                        {
                            if (me->GetPower(POWER_MANA) < 10)
                            {
                                events.ScheduleEvent(EVENT_MOLTEN_INFERNO, 2000);
                            }
                            else
                            {
                                DoCastAOE(SPELL_MOLTEN_INFERNO);
                                events.ScheduleEvent(EVENT_MOLTEN_INFERNO, urand(6000, 7000));
                            }
                            break;
                        }

                        case EVENT_CHECK_PLAYERS:
                        {
                            int32 power = me->GetPower(POWER_MANA);
                            if (power >= 30)
                            {
                                std::list<Unit*> targets;
                                SelectTargetList(targets, 25, SELECT_TARGET_RANDOM, 0.0f, true);

                                for (std::list<Unit*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                                {
                                    Unit* target = (*itr);

                                    std::list<Player*> countingTargets;
                                    target->GetPlayerListInGrid(countingTargets, 5.0f);

                                    if (countingTargets.size() >= uint8(RAID_MODE(3, 5)))
                                    {
                                        me->SetPower(POWER_MANA, power - 30);
                                        DoCast(target, SPELL_UNLEASHED_FLAME_MISSILE_1);
                                    }
                                }
                            }

                            events.ScheduleEvent(EVENT_CHECK_PLAYERS, 3000);
                            break;
                        }

                        case EVENT_THROW_SPEAR:
                            DoCastAOE(SPELL_BURNING_CINDERS_AOE, true);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        private:

            void SetBossVehicle()
            {
                if (pInstance)
                {
                    if (Creature* pIronQon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_QON)))
                    {
                        pIronQon->EnterVehicle(me, -1, true);
                    }
                }
            }

            void SendStartCombat()
            {
                if (pInstance)
                {
                    if (Creature* pIronQon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_QON)))
                    {
                        pIronQon->AI()->DoAction(ACTION_START_COMBAT);
                    }
                }
            }

            void SendKilledUnit(Unit* target)
            {
                if (pInstance)
                {
                    if (Creature* pIronQon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_QON)))
                    {
                        pIronQon->AI()->KilledUnit(target);
                    }
                }
            }

            void SendDied()
            {
                if (pInstance)
                {
                    if (Creature* pIronQon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_QON)))
                    {
                        pIronQon->AI()->DoAction(ACTION_ROSHAK_DIED);
                    }
                }
            }

        };
};

class npc_iron_qon_quetzal : public CreatureScript
{
    public:
        npc_iron_qon_quetzal() : CreatureScript("npc_iron_qon_quetzal") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_qon_quetzalAI(creature);
        }

        struct npc_iron_qon_quetzalAI : public ScriptedAI
        {
            npc_iron_qon_quetzalAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 5);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 5);

                me->setActive(true);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void IsSummonedBy(Unit* owner)
            {
                me->SetStandState(UNIT_STAND_STATE_SIT);
            }

            void KilledUnit(Unit* target)
            {
                SendKilledUnit(target);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_ACTIVATE_QUETZAL)
                {
                    me->ClearUnitState(UNIT_STAND_STATE_SIT);

                    events.ScheduleEvent(EVENT_ARC_LIGHTNING, 20000);
                    //events.ScheduleEvent(EVENT_RUSHING_WINDS_PREPARE, 10000);
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoZoneInCombat();

                }
                else if (action == ACTION_THROWN_SPEAR)
                {
                    events.ScheduleEvent(EVENT_THROW_SPEAR, 3000);
                }
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (id == POINT_RUSHING_WINDS)
                {
                    events.ScheduleEvent(EVENT_RUSHING_WINDS_BEGIN, 1000);
                }
            }

            void JustDied(Unit* killer)
            {
                if (Vehicle* pVehicle = me->GetVehicleKit())
                {
                    pVehicle->RemoveAllPassengers();

                    SendDied();
                }
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
                        case EVENT_ARC_LIGHTNING:
                            DoCastAOE(SPELL_LIGHTNING_STORM);
                            events.ScheduleEvent(EVENT_ARC_LIGHTNING, 20000);
                            break;
                        case EVENT_THROW_SPEAR:
                            DoCastAOE(SPELL_STORM_CLOUD_AOE, true);
                            break;
                        case EVENT_RUSHING_WINDS_PREPARE:
                            events.CancelEvent(EVENT_ARC_LIGHTNING);

                            me->SetCanFly(true);
                            me->SetDisableGravity(true);

                            me->RemoveAllAuras();
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                            me->AttackStop();
                            me->SetReactState(REACT_PASSIVE);

                            me->GetMotionMaster()->MovePoint(POINT_RUSHING_WINDS, centerRoomPos);
                            
                            events.ScheduleEvent(EVENT_RUSHING_WINDS_END, 20000);
                            break;
                        case EVENT_RUSHING_WINDS_BEGIN:
                            
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        private:

            void SendKilledUnit(Unit* target)
            {
                if (pInstance)
                {
                    if (Creature* pIronQon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_QON)))
                    {
                        pIronQon->AI()->KilledUnit(target);
                    }
                }
            }

            void SendDied()
            {
                if (pInstance)
                {
                    if (Creature* pIronQon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_QON)))
                    {
                        pIronQon->AI()->DoAction(ACTION_QUETZAL_DIED);
                    }
                }
            }
        };
};

class npc_iron_qon_damren : public CreatureScript
{
    public:
        npc_iron_qon_damren() : CreatureScript("npc_iron_qon_damren") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_qon_damrenAI(creature);
        }

        struct npc_iron_qon_damrenAI : public ScriptedAI
        {
            npc_iron_qon_damrenAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 5);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 5);

                me->setActive(true);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();

                lastDeadZoneSpell = 0;

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void IsSummonedBy(Unit* owner)
            {
                me->SetStandState(UNIT_STAND_STATE_SIT);
            }

            void KilledUnit(Unit* target)
            {
                SendKilledUnit(target);
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if (DeadZoneDamage(attacker))
                {
                    damage = 0;
                }
            }

            void JustDied(Unit* killer)
            {
                if (Vehicle* pVehicle = me->GetVehicleKit())
                {
                    pVehicle->RemoveAllPassengers();

                    SendDied();
                }
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_ACTIVATE_DAMREN)
                {
                    me->ClearUnitState(UNIT_STAND_STATE_SIT);

                    events.ScheduleEvent(EVENT_DEAD_ZONE, 8500);

                    me->SetReactState(REACT_AGGRESSIVE);

                    me->AddAura(SPELL_FROZEN_RESILIENCE, me);

                    DoZoneInCombat();

                }
                else if (action == ACTION_THROWN_SPEAR)
                {
                    events.ScheduleEvent(EVENT_THROW_SPEAR, 3000);
                }
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
                        case EVENT_THROW_SPEAR:
                            DoCastAOE(SPELL_FROZEN_BLOOD_AOE, true);
                            break;
                        case EVENT_DEAD_ZONE:
                        {
                            uint8 i = urand(0, MAX_DEAD_ZONE_SPELLS - 1);

                            lastDeadZoneSpell = deadZoneSpells[i];

                            DoCast(me, lastDeadZoneSpell);
                            
                            events.ScheduleEvent(EVENT_DEAD_ZONE_ENDS, 6000);
                            events.ScheduleEvent(EVENT_DEAD_ZONE, 20000);
                            break;
                        }
                        case EVENT_DEAD_ZONE_ENDS:
                            lastDeadZoneSpell = 0;
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;
            EventMap events;
            uint32 lastDeadZoneSpell; // cache aura spell, don't use 'HasAura' later

        private:

            void SendKilledUnit(Unit* target)
            {
                if (pInstance)
                {
                    if (Creature* pIronQon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_QON)))
                    {
                        pIronQon->AI()->KilledUnit(target);
                    }
                }
            }

            void SendDied()
            {
                if (pInstance)
                {
                    if (Creature* pIronQon = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IRON_QON)))
                    {
                        pIronQon->AI()->DoAction(ACTION_DAMREN_DIED);
                    }
                }
            }

            bool DeadZoneDamage(Unit* attacker)
            {
                if (lastDeadZoneSpell == SPELL_DEAD_ZONE_FRONT_RIGHT)
                {
                    bool isInFront = attacker->isInBack(me, M_PI / 2);
                    bool isInRight = HasInRight(attacker, M_PI / 2);
                    return isInFront || isInRight;
                }
                else if (lastDeadZoneSpell == SPELL_DEAD_ZONE_LEFT_RIGHT)
                {
                    bool isInLeft = HasInLeft(attacker, M_PI / 2);
                    bool isInRight = HasInRight(attacker, M_PI / 2);
                    return isInLeft || isInRight;
                }
                else if (lastDeadZoneSpell == SPELL_DEAD_ZONE_FRONT_LEFT)
                {
                    bool isInFront = attacker->isInBack(me, M_PI / 2);
                    bool isInLeft = HasInLeft(attacker, M_PI / 2);
                    return isInFront || isInLeft;
                }
                else if (lastDeadZoneSpell == SPELL_DEAD_ZONE_FRONT_BACK)
                {
                    bool isInFront = attacker->isInBack(me, M_PI / 2);
                    bool isInBack = me->isInBack(attacker, M_PI / 2);
                    return isInFront || isInBack;
                }
                else if (lastDeadZoneSpell == SPELL_DEAD_ZONE_LEFT_BACK)
                {
                    bool isInBack = me->isInBack(attacker, M_PI / 2);
                    bool isInLeft = HasInLeft(attacker, M_PI / 2);
                    return isInBack || isInLeft;
                }
                else if (lastDeadZoneSpell == SPELL_DEAD_ZONE_BACK_RIGHT)
                {
                    bool isInBack = me->isInBack(attacker, M_PI / 2);
                    bool isInRight = HasInRight(attacker, M_PI / 2);
                    return isInBack || isInRight;
                }

                return false;
            }

            bool HasInLeft(const Position* obj, float arc) const
            {
                // move arc to range 0.. 2*pi
                arc = me->NormalizeOrientation(arc);

                float angle = me->GetAngle(obj);
                angle -= me->GetOrientation();

                // move angle to range -pi ... +pi
                angle = me->NormalizeOrientation(angle);
                if (angle > M_PI)
                    angle -= 2.0f*M_PI;

                float lborder = -1 * (arc/2.0f) + (M_PI / 2);                        
                float rborder = (arc/2.0f) + (M_PI / 2);                            
                return ((angle >= lborder) && (angle <= rborder));
            }

            bool HasInRight(const Position* obj, float arc) const
            {
                // move arc to range 0.. 2*pi
                arc = me->NormalizeOrientation(arc);

                float angle = me->GetAngle(obj);
                angle -= me->GetOrientation();

                // move angle to range -pi ... +pi
                angle = me->NormalizeOrientation(angle);
                if (angle > M_PI)
                    angle -= 2.0f*M_PI;

                float lborder = -1 * (arc/2.0f) - (M_PI / 2);                        
                float rborder = (arc/2.0f) - (M_PI / 2);                            
                return ((angle >= lborder) && (angle <= rborder));
            }
        };
};

class npc_iron_qon_spear : public CreatureScript
{
    public:
        npc_iron_qon_spear() : CreatureScript("npc_iron_qon_spear") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_qon_spearAI(creature);
        }

        struct npc_iron_qon_spearAI : public Scripted_NoMovementAI
        {
            npc_iron_qon_spearAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void Reset()
            {
                me->AddAura(SPELL_IRON_QON_SPEAR, me);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (id == EVENT_JUMP)
                {
                    me->DespawnOrUnsummon(100);
                }
            }

        };
};

class npc_iron_qon_spear_2 : public CreatureScript
{
    public:
        npc_iron_qon_spear_2() : CreatureScript("npc_iron_qon_spear_2") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_qon_spear_2AI(creature);
        }

        struct npc_iron_qon_spear_2AI : public Scripted_NoMovementAI
        {
            npc_iron_qon_spear_2AI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                
                canSpawnTriggers = false;
                spawnTimer = 200;
                currentTick = 0;
                m_IronQonGuid = 0;

                m_SpellInfo = NULL;
                m_MiscValue = 0;
            }

            void Reset()
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                    m_IronQonGuid = pInstance->GetData64(DATA_IRON_QON);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_IGNITE_SPEAR)
                {
                    canSpawnTriggers = true;

                    m_SpellInfo = const_cast<SpellInfo*>(sSpellMgr->GetSpellInfo(SPELL_BURNING_CINDERS));

                    if (m_SpellInfo->Effects[EFFECT_1].Effect == SPELL_EFFECT_CREATE_AREATRIGGER)
                        m_MiscValue = m_SpellInfo->Effects[EFFECT_1].MiscValue;

                }
                else if (action == ACTION_STRIKE_SPEAR)
                {
                    canSpawnTriggers = true;

                    m_SpellInfo = const_cast<SpellInfo*>(sSpellMgr->GetSpellInfo(SPELL_STORM_CLOUD));
                    if (m_SpellInfo->Effects[EFFECT_1].Effect == SPELL_EFFECT_CREATE_AREATRIGGER)
                        m_MiscValue = m_SpellInfo->Effects[EFFECT_1].MiscValue;
                }
                else if (action == ACTION_FREEZE_SPEAR)
                {
                    canSpawnTriggers = true;

                    m_SpellInfo = const_cast<SpellInfo*>(sSpellMgr->GetSpellInfo(SPELL_FROZEN_BLOOD));
                    if (m_SpellInfo->Effects[EFFECT_1].Effect == SPELL_EFFECT_CREATE_AREATRIGGER)
                        m_MiscValue = m_SpellInfo->Effects[EFFECT_1].MiscValue;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (canSpawnTriggers)
                {
                    if (spawnTimer <= diff)
                    {
                        if (!m_SpellInfo)
                        {
                            canSpawnTriggers = false;
                            return;
                        }

                        Creature* pIronQon = sObjectAccessor->GetCreature(*me, m_IronQonGuid);
                        if (!pIronQon)
                        {
                            canSpawnTriggers = false;
                            return;
                        }

                        spawnTimer = 200;

                        currentTick++;
                        if (currentTick >= 20)
                        {
                            canSpawnTriggers = false;
                            return;
                        }

                        for (uint8 i = 0; i < 4; ++i)
                        {
                            Position pos;

                            me->GetNearPosition(pos, currentTick * 3, (float)i * (M_PI / 2.0f));

                            AreaTrigger* areaTrigger = new AreaTrigger;
                            if (!areaTrigger->CreateAreaTriggerFromSpell(me->GetMap()->GenerateLowGuid<HighGuid::AreaTrigger>(), pIronQon, m_SpellInfo, EFFECT_1, pos, pos))
                                delete areaTrigger;
                        }
                    }
                    else
                    {
                        spawnTimer -= diff;
                    }
                }
            }

        private:

            uint64 m_IronQonGuid;
            SpellInfo* m_SpellInfo;
            uint32 m_MiscValue;
            bool canSpawnTriggers;
            uint32 spawnTimer;
            uint32 currentTick;
        };
};

class npc_iron_qon_lightning_cloud : public CreatureScript
{
    public:
        npc_iron_qon_lightning_cloud() : CreatureScript("npc_iron_qon_lightning_cloud") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_qon_lightning_cloudAI(creature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            creature->AI()->DoAction(ACTION_RELEASE_TARGET);

            return true;
        }

        struct npc_iron_qon_lightning_cloudAI : public Scripted_NoMovementAI
        {
            npc_iron_qon_lightning_cloudAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);    

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                targetGuid = 0;
            }

            void SetGUID(uint64 guid, int32 id)
            {
                targetGuid = guid;
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_RELEASE_TARGET)
                {
                    if (Unit* target = me->GetUnit(*me, targetGuid))
                    {
                        target->RemoveAura(SPELL_LIGHTNING_STORM);
                    }

                    me->DespawnOrUnsummon(100);
                }
            }

        private:

            uint64 targetGuid;
        };
};

class spell_iron_qon_return_spear_script: public SpellScriptLoader
{
    public:
        spell_iron_qon_return_spear_script() : SpellScriptLoader("spell_iron_qon_return_spear_script") { }

        class spell_iron_qon_return_spear_script_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_qon_return_spear_script_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetCaster(), SPELL_RETURN_SPEAR_JUMP, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_iron_qon_return_spear_script_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_qon_return_spear_script_SpellScript();
        }
};

class spell_iron_qon_molten_inferno: public SpellScriptLoader
{
    public:
        spell_iron_qon_molten_inferno() : SpellScriptLoader("spell_iron_qon_molten_inferno") { }

        class spell_iron_qon_molten_inferno_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_qon_molten_inferno_SpellScript);

            void HandleAfterCast()
            {
                if (!GetCaster())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    int32 power = pCreature->GetPower(POWER_MANA);
                    if (power >= 10)
                        pCreature->SetPower(POWER_MANA, power - 10);
                    else
                        pCreature->SetPower(POWER_MANA, 0);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_iron_qon_molten_inferno_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_qon_molten_inferno_SpellScript();
        }
};

class spell_iron_qon_burning_cinders_aoe: public SpellScriptLoader
{
    public:
        spell_iron_qon_burning_cinders_aoe() : SpellScriptLoader("spell_iron_qon_burning_cinders_aoe") { }

        class spell_iron_qon_burning_cinders_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_qon_burning_cinders_aoe_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (Creature* pSpear = GetCaster()->SummonCreature(NPC_IRON_QON_SPEAR_2, GetHitUnit()->GetPositionX(), GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 50000))
                {
                    pSpear->CastSpell(GetHitUnit()->GetPositionX(), GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), SPELL_BURNING_CINDERS, true);
                    pSpear->AI()->DoAction(ACTION_IGNITE_SPEAR);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_iron_qon_burning_cinders_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_qon_burning_cinders_aoe_SpellScript();
        }
};

class spell_iron_qon_ligntning_storm: public SpellScriptLoader
{
    public:
        spell_iron_qon_ligntning_storm() : SpellScriptLoader("spell_iron_qon_ligntning_storm") { }

        class spell_iron_qon_ligntning_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_qon_ligntning_storm_SpellScript);

            void FilterTargets(std::list<WorldObject*> &targets)
            {
                if (!GetCaster())
                    return; 

                Unit* victim = GetCaster()->getVictim();

                if (targets.size() > 1)
                {
                    if (victim)
                        targets.remove(victim);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_iron_qon_ligntning_storm_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        class spell_iron_qon_ligntning_storm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_iron_qon_ligntning_storm_AuraScript);

            void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                //GetUnitOwner()->AddAura(SPELL_ARC_LIGHTNING_PERIODIC_1, GetUnitOwner());

                if (Creature* pCloud = GetCaster()->SummonCreature(NPC_LIGHTNING_CLOUD, GetUnitOwner()->GetPositionX(), GetUnitOwner()->GetPositionY(), GetUnitOwner()->GetPositionZ(), 0.0f))
                {
                    pCloud->AI()->SetGUID(GetUnitOwner()->GetGUID());
                }
            }

            /*void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetUnitOwner())
                    return;

                GetUnitOwner()->RemoveAura(SPELL_ARC_LIGHTNING_PERIODIC_1);
            }*/

            void HandleAbsorb(AuraEffect* aurEff, int32& remainingHeal, uint32& absorbAmount)
            {
                absorbAmount = remainingHeal;
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_iron_qon_ligntning_storm_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                //AfterEffectRemove += AuraEffectRemoveFn(spell_iron_qon_ligntning_storm_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                OnEffectAbsorbHeal += AuraEffectAbsorbHealFn(spell_iron_qon_ligntning_storm_AuraScript::HandleAbsorb, EFFECT_4);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_qon_ligntning_storm_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_iron_qon_ligntning_storm_AuraScript();
        }
};

class spell_iron_qon_storm_cloud_aoe: public SpellScriptLoader
{
    public:
        spell_iron_qon_storm_cloud_aoe() : SpellScriptLoader("spell_iron_qon_storm_cloud_aoe") { }

        class spell_iron_qon_storm_cloud_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_qon_storm_cloud_aoe_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (Creature* pSpear = GetCaster()->SummonCreature(NPC_IRON_QON_SPEAR_2, GetHitUnit()->GetPositionX(), GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 50000))
                {
                    pSpear->CastSpell(GetHitUnit()->GetPositionX(), GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), SPELL_STORM_CLOUD, true);
                    pSpear->AI()->DoAction(ACTION_STRIKE_SPEAR);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_iron_qon_storm_cloud_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_qon_storm_cloud_aoe_SpellScript();
        }
};

class spell_iron_qon_storm_cloud: public SpellScriptLoader
{
    public:
        spell_iron_qon_storm_cloud() : SpellScriptLoader("spell_iron_qon_storm_cloud") { }

        class spell_iron_qon_storm_cloud_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_iron_qon_storm_cloud_AuraScript);

            void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetUnitOwner())
                    return;

                Aura const* aur = aurEff->GetBase();
                if (aur->GetStackAmount() >= 3)
                {
                    GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_ELECTRIFIED, true);
                    Remove();
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_iron_qon_storm_cloud_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_iron_qon_storm_cloud_AuraScript();
        }
};

class spell_iron_qon_frozen_blood_aoe: public SpellScriptLoader
{
    public:
        spell_iron_qon_frozen_blood_aoe() : SpellScriptLoader("spell_iron_qon_frozen_blood_aoe") { }

        class spell_iron_qon_frozen_blood_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_qon_frozen_blood_aoe_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (Creature* pSpear = GetCaster()->SummonCreature(NPC_IRON_QON_SPEAR_2, GetHitUnit()->GetPositionX(), GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 50000))
                {
                    pSpear->CastSpell(GetHitUnit()->GetPositionX(), GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), SPELL_BURNING_CINDERS, true);
                    pSpear->AI()->DoAction(ACTION_FREEZE_SPEAR);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_iron_qon_frozen_blood_aoe_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_qon_frozen_blood_aoe_SpellScript();
        }
};

class spell_iron_qon_rushing_winds_aoe: public SpellScriptLoader
{
    public:
        spell_iron_qon_rushing_winds_aoe() : SpellScriptLoader("spell_iron_qon_rushing_winds_aoe") { }

        class spell_iron_qon_rushing_winds_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_iron_qon_rushing_winds_aoe_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetCaster(), SPELL_RUSHING_WINDS_JUMP, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_iron_qon_rushing_winds_aoe_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_iron_qon_rushing_winds_aoe_SpellScript();
        }
};

class spell_iron_qon_frozen_resilience : public SpellScriptLoader
{
    public:
        spell_iron_qon_frozen_resilience() : SpellScriptLoader("spell_iron_qon_frozen_resilience") { }

        class spell_iron_qon_frozen_resilience_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_iron_qon_frozen_resilience_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetUnitOwner())
                    return;

                Unit* attacker = eventInfo.GetActor();
                if (!attacker)
                    return;

                int32 damage = int32(0.1f * eventInfo.GetDamageInfo()->GetDamage());

                GetUnitOwner()->CastCustomSpell(attacker, SPELL_FROZEN_RESILIENCE_DMG, &damage, NULL, NULL, NULL, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_iron_qon_frozen_resilience_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_iron_qon_frozen_resilience_AuraScript();
        }
};

class spell_area_iron_qon_burning_cinders : public SpellAreaTriggerScript
{
    public:
        spell_area_iron_qon_burning_cinders() : SpellAreaTriggerScript("spell_area_iron_qon_burning_cinders") {}

        bool OnAddTarget(AreaTrigger* trigger, Unit* target)
        {
            if (!trigger->GetCaster())
                return false;

            trigger->GetCaster()->AddAura(SPELL_BURNING_CINDERS_AURA, target);

            return true;
        }

        bool OnRemoveTarget(AreaTrigger* trigger, Unit* target)
        {
            target->RemoveAura(SPELL_BURNING_CINDERS_AURA);

            return true;
        }
};

class spell_area_iron_qon_storm_cloud : public SpellAreaTriggerScript
{
    public:
        spell_area_iron_qon_storm_cloud() : SpellAreaTriggerScript("spell_area_iron_qon_storm_cloud") {}

        bool OnAddTarget(AreaTrigger* trigger, Unit* target)
        {
            if (!trigger->GetCaster())
                return false;

            trigger->GetCaster()->AddAura(SPELL_STORM_CLOUD_AURA, target);

            return true;
        }

        bool OnRemoveTarget(AreaTrigger* trigger, Unit* target)
        {
            target->RemoveAura(SPELL_STORM_CLOUD_AURA);

            return true;
        }
};

class spell_area_iron_qon_frozen_blood : public SpellAreaTriggerScript
{
    public:
        spell_area_iron_qon_frozen_blood() : SpellAreaTriggerScript("spell_area_iron_qon_frozen_blood") {}

        bool OnAddTarget(AreaTrigger* trigger, Unit* target)
        {
            if (!trigger->GetCaster())
                return false;

            trigger->GetCaster()->AddAura(SPELL_FROZEN_BLOOD_DEBUFF, target);

            return true;
        }

        bool OnRemoveTarget(AreaTrigger* trigger, Unit* target)
        {
            target->RemoveAura(SPELL_FROZEN_BLOOD_DEBUFF);

            return true;
        }
};

void AddSC_boss_iron_qon()
{
    new boss_iron_qon();                        // 68078
    new npc_iron_qon_roshak();                  // 68079
    new npc_iron_qon_quetzal();                 // 68080
    new npc_iron_qon_damren();                  // 68081
    new npc_iron_qon_spear();                   // 68480
    new npc_iron_qon_spear_2();                 // 69159
    new npc_iron_qon_lightning_cloud();         // 70660

    new spell_iron_qon_return_spear_script();   // 137712
    new spell_iron_qon_molten_inferno();        // 134664
    new spell_iron_qon_burning_cinders_aoe();   // 136330
    new spell_iron_qon_ligntning_storm();       // 136192
    new spell_iron_qon_storm_cloud_aoe();       // 136419
    new spell_iron_qon_storm_cloud();           // 137669
    new spell_iron_qon_frozen_blood_aoe();      // 136449
    new spell_iron_qon_rushing_winds_aoe();     // 136591
    new spell_iron_qon_frozen_resilience();     // 135144

    new spell_area_iron_qon_burning_cinders();  // 134758
    new spell_area_iron_qon_storm_cloud();      // 136421
    new spell_area_iron_qon_frozen_blood();     // 136451
}

#include "siege_of_orgrimmar.hpp"
#include "GameObjectAI.h"

#define MAX_SHA_POWER 100
#define SHA_POWER_PERIODIC 600

#define DEFAULT_PRIDE_INCREASE 5

#define MAX_LINGERING_CORRUPTIONS 10

#define GIFT_OF_TITANS_COUNT_10 2
#define GIFT_OF_TITANS_COUNT_25 5

enum ScriptedTextNorushen
{
    SAY_GIFT_OF_THE_TITANS  = 0,
    SAY_FINAL_GIFT          = 1,
    SAY_NORUSHEN_INTRO_1    = 2,
    SAY_NORUSHEN_INTRO_2    = 3,
};

#define SOUND_DEATH 38897

enum ScriptedTextShaOfPride
{
    SAY_AGGRO               = 0,
    SAY_DEATH               = 1,
    SAY_UNLEASHED           = 2,
    SAY_INTRO               = 3,
    SAY_KILL                = 4,
    SAY_SELF_REFLECTION     = 5,
    SAY_SWELLING_PRIDE      = 6,
    SAY_CORRUPTED_PRISON    = 7,
};

enum ScriptedTextJaina
{
    SAY_JAINA_INTRO_1   = 0,
    SAY_JAINA_INTRO_2   = 1,
    SAY_JAINA_INTRO_3   = 2,
    SAY_JAINA_INTRO_4   = 3,
    SAY_JAINA_INTRO_5   = 4,
};

enum ScriptedTextLorthemar
{
    SAY_LORTHEMAR_INTRO_1 = 0,
    SAY_LORTHEMAR_INTRO_2 = 1,
    SAY_LORTHEMAR_INTRO_3 = 2,
    SAY_LORTHEMAR_INTRO_4 = 3,
    SAY_LORTHEMAR_INTRO_5 = 4,
};

enum Spells
{
    // Intro

    SPELL_SHA_VORTEX                = 146024,

    // Lingering Corruption
    SPELL_CORRUPTED_TOUCH           = 149207,
    SPELL_CORRUPTED_TOUCH_DMG       = 149209,

    // Boss

    SPELL_ZERO_POWER                = 72242,

    SPELL_PRIDE                     = 144343,

    SPELL_SWELLING_PRIDE            = 144400,
    SPELL_BURSTING_PRIDE            = 144910, // 25-49 pride
    SPELL_BURSTING_PRIDE_DMG        = 144911, 
    SPELL_PROJECTION_1              = 144952, // 50-74 pride
    SPELL_PROJECTION_2              = 145526, // explode visual (without dmg)
    SPELL_PROJECTION_MISSILE        = 145066, // triggers non-existed spell 145324
    SPELL_PROJECTION_DMG            = 145320,
    SPELL_PROJECTION_MARK           = 146822,
    SPELL_AURA_OF_PRIDE             = 146817, // 75-99 pride
    SPELL_AURA_OF_PRIDE_DMG         = 146818,
    SPELL_OVERCOME_1                = 144843, // buff
    SPELL_OVERCOME_2                = 144863, // debuff

    SPELL_GIFT_OF_THE_TITANS        = 146595, // casted by norushen
    SPELL_GIFT_OF_THE_TITANS_1      = 144359, // 1 player
    SPELL_GIFT_OF_THE_TITANS_2      = 146594, // 2 players
    SPELL_GIFT_OF_THE_TITANS_AOE    = 144363, // search 8 yards
    SPELL_POWER_OF_THE_TITANS       = 144364,
    SPELL_FINAL_GIFT                = 144854,

    SPELL_MARK_OF_ARROGANCE         = 144351,
    
    SPELL_WOUNDED_PRIDE             = 144358,

    SPELL_SELF_REFLECTION           = 144800,
    SPELL_SELF_REFLECTION_SPAWN     = 144784,
    SPELL_SELF_REFLECTION_DMG       = 144788,

    SPELL_REACHING_ATTACK           = 144774,

    SPELL_UNLEASHED                 = 144832,
    SPELL_UNLEASHED_DMG             = 144836,

    SPELL_CORRUPTED_PRISON_1        = 144574,
    SPELL_CORRUPTED_PRISON_2        = 144636,
    SPELL_CORRUPTED_PRISON_3        = 144683,
    SPELL_CORRUPTED_PRISON_4        = 144684,
    SPELL_CORRUPTED_PRISON_DMG      = 144615, // knockback players
    SPELL_CORRUPTED_PRISON_OBJECT_1 = 144550, // ?
    SPELL_CORRUPTED_PRISON_OBJECT_2 = 144552, // ?
    SPELL_CORRUPTED_PRISON_OBJECT_3 = 144559, // ?

    // heroic spells
    SPELL_BANISHMENT_AOE_1          = 146823, // targets player
    SPELL_BANISHMENT_AOE_2          = 147028, // ?
    SPELL_BANISHMENT_TELEPORT       = 148705,
    SPELL_BANISHMENT                = 145215,
    SPELL_BANISHMENT_AREATRIGGERS   = 145217, // creates triggers
    SPELL_BANISHMENT_SCALE          = 145684,
    SPELL_BANISHMENT_STUN           = 146623,
    
    // Manifestation of Pride
    SPELL_MANIFESTATION_SPAWN       = 144778,
    SPELL_MOCKING_BLAST             = 144379,
    SPELL_LAST_WORD                 = 144370,
};

enum Adds
{
    NPC_MANIFESTATION_OF_PRIDE  = 71946,
    NPC_REFLECTION              = 72172,
    NPC_SHADOW_PRISON           = 72017,
    NPC_SHA_VORTEX              = 72239,
};

enum Events
{
    EVENT_SPAWN_SHA_OF_PRIDE     = 1,
    EVENT_GIFT_OF_THE_TITANS,
    EVENT_FINAL_GIFT,

    EVENT_REACHING_ATTACK,
    EVENT_MARK_OF_ARROGANCE,
    EVENT_SHADOW_PRISON,
    EVENT_SWELLING_PRIDE,
    EVENT_WOUNDED_PRIDE,
    EVENT_MANIFESTATION,
    EVENT_SELF_REFLECTION,

    EVENT_REFLECTION_DMG,

    EVENT_MOVE,
    EVENT_MOCKING_BLAST,
};

enum Actions
{
    ACTION_LINGERING_CORRUPTION_DIED    = 1,
    ACTION_UNLEASHED,
    ACTION_RESET,
    ACTION_COMPLETED,
    ACTION_COMBAT_START,
};

enum eData
{
    DATA_TRAPPED_PLAYER = 1,
    DATA_TRAP_USED,
    DATA_TRAP_DEACTIVATED,
};

enum PrideLevels
{
    PRIDE_MAX   = 100,
};

const uint32 prisonSpells[MAX_SHADOW_PRISONS] = 
{
    SPELL_CORRUPTED_PRISON_1,
    SPELL_CORRUPTED_PRISON_2,
    SPELL_CORRUPTED_PRISON_3,
    SPELL_CORRUPTED_PRISON_4
};

const Position shaVortexPos = {747.83f, 1112.96f, 356.08f, 4.96f};

const Position manifestationPos = {735.34f, 1173.69f, 356.08f, 4.88f};

const Position prisonPos[MAX_SHADOW_PRISONS] = 
{
    {764.30f, 1137.92f, 356.08f, 4.17f},
    {772.71f, 1096.25f, 356.08f, 2.53f},
    {731.98f, 1087.30f, 356.08f, 0.93f},
    {722.91f, 1128.89f, 356.08f, 5.65f}
};

inline void TryIncreasePrideOnUnit(Unit* target, int32 prideToAdd)
{
    int32 currentPride = target->GetPower(POWER_ALTERNATE_POWER);
    if (currentPride >= PRIDE_MAX)
        return;

    if (target->HasAura(SPELL_GIFT_OF_THE_TITANS_1) ||
        target->HasAura(SPELL_GIFT_OF_THE_TITANS_2))
        return;

    int32 newPride = std::min(currentPride + prideToAdd, (int)PRIDE_MAX);

    target->SetPower(POWER_ALTERNATE_POWER, newPride);
}

class PrisonData
{
    public:

        PrisonData()
        {
            Clear();
        }

        bool HasPlayer() const
        {
            return m_TargetGuid != 0;
        }

        bool HasTraps() const
        {
            return !m_TrapGuids.empty();
        }

        bool RemoveTrapGuid(uint64 trapGuid)
        {
            std::set<uint64>::iterator itr = m_TrapGuids.find(trapGuid);
            if (itr == m_TrapGuids.end())
                return false;

            m_TrapGuids.erase(itr);

            return true;
        }

        void SetTargetGuid(uint64 targetGuid)
        {
            m_TargetGuid = targetGuid;
        }

        uint64 GetTargetGuid() const
        {
            return m_TargetGuid;
        }

        void SetPrisonGuid(uint64 prisonGuid)
        {
            m_PrisonGuid = prisonGuid;
        }

        uint64 GetPrisonGuid() const
        {
            return m_PrisonGuid;
        }

        void SetPrison(uint8 prison)
        {
            m_Prison = prison;
        }

        uint8 GetPrison() const
        {
            return m_Prison;
        }

        void SetSpellId(uint32 spellId)
        {
            m_SpellId = spellId;
        }

        uint32 GetSpellId() const
        {
            return m_SpellId;
        }

        void AddTrapGuid(uint64 trapGuid)
        {
            m_TrapGuids.insert(trapGuid);
        }

        std::set<uint64>& GetTrapGuids()
        {
            return m_TrapGuids;
        }

        void Clear()
        {
            m_TargetGuid = 0;
            m_PrisonGuid = 0;
            m_Prison = 0;
            m_TrapGuids.clear();
            m_SpellId = 0;
        }

    private:

    private:

        uint64 m_TargetGuid;
        uint64 m_PrisonGuid;
        uint8 m_Prison;
        std::set<uint64> m_TrapGuids;
        uint32 m_SpellId;
};

class PrisonController
{
    public:

        PrisonController(InstanceScript* pInstance)
        {
            m_pInstance = pInstance;
        }

        bool CatchPlayer(Unit* catcher, uint64 playerGuid)
        {
            if (!HasFreePrison())
                return false;

            Player* player = sObjectAccessor->FindPlayer(playerGuid);
            if (!player)
                return false;

            uint8 prison = GetFreePrisonIndex();

            uint32 spellId = GetSpellIdForPrison(prison);

            if (Creature* pPrisonTrigger = catcher->SummonCreature(NPC_SHADOW_PRISON, prisonPos[prison]))
            {
                pPrisonTrigger->CastSpell(pPrisonTrigger, SPELL_CORRUPTED_PRISON_DMG, true);
                pPrisonTrigger->DespawnOrUnsummon(1000);
            }

            player->InterruptNonMeleeSpells(false);
            catcher->CastSpell(player, spellId, true);

            ActivatePrisonAndTraps(prison, playerGuid, spellId);

            return true;
        }

        void TrapUsed(uint64 trapGuid)
        {
            if (PrisonData* prisonData = GetPrisonDataByTrap(trapGuid))
            {
                if (prisonData->RemoveTrapGuid(trapGuid))
                {
                    if (!prisonData->HasTraps())
                        DeactivatePrisonAndTraps(prisonData->GetPrison());
                }
            }
        }

        bool HasFreePrison() const
        {
            for (uint8 i = 0; i < MAX_SHADOW_PRISONS; ++i)
            {
                if (!m_Prisons[i].HasPlayer())
                    return true;
            }

            return false;
        }

        void DeactivateAllPrisons()
        {
            for (uint8 i = 0; i < MAX_SHADOW_PRISONS; ++i)
            {
                DeactivatePrisonAndTraps(i);
            }
        }

        void FreePlayer(uint64 playerGuid)
        {
            if (PrisonData* prisonData = GetPrisonDataByPlayer(playerGuid))
            {
                DeactivatePrisonAndTraps(prisonData->GetPrison());
            }
        }

    public:

        static uint32 GetSpellIdForPrisonTrapEntry(uint32 entry)
        {
            switch (entry)
            {
                case GO_WEST_SHADOW_PRISON_A:
                case GO_WEST_SHADOW_PRISON_B:
                case GO_WEST_SHADOW_PRISON_C:
                    return SPELL_CORRUPTED_PRISON_1;
                case GO_NORTH_SHADOW_PRISON_A:
                case GO_NORTH_SHADOW_PRISON_B:
                case GO_NORTH_SHADOW_PRISON_C:
                    return SPELL_CORRUPTED_PRISON_2;
                case GO_EAST_SHADOW_PRISON_A:
                case GO_EAST_SHADOW_PRISON_B:
                case GO_EAST_SHADOW_PRISON_C:
                    return SPELL_CORRUPTED_PRISON_3;
                case GO_SOUTH_SHADOW_PRISON_A:
                case GO_SOUTH_SHADOW_PRISON_B:
                case GO_SOUTH_SHADOW_PRISON_C:
                    return SPELL_CORRUPTED_PRISON_4;
            }

            return 0;
        }

    private:

        GameObject* GetPrison(uint8 prison)
        {
            if (!m_pInstance)
                return NULL;

            switch (prison)
            {
                case SHADOW_PRISON_WEST: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_WEST_SHADOW_PRISON));
                case SHADOW_PRISON_NORTH: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_NORTH_SHADOW_PRISON));
                case SHADOW_PRISON_EAST: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_EAST_SHADOW_PRISON));
                case SHADOW_PRISON_SOUTH: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SOUTH_SHADOW_PRISON));
            }

            return NULL;
        }

        GameObject* GetPrisonTrap(uint8 prison, uint8 prisonTrap)
        {
            switch (prison)
            {
                case SHADOW_PRISON_WEST:
                    switch (prisonTrap)
                    {
                        case SHADOW_PRISON_TRAP_A: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_WEST_SHADOW_PRISON_A));
                        case SHADOW_PRISON_TRAP_B: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_WEST_SHADOW_PRISON_B));
                        case SHADOW_PRISON_TRAP_C: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_WEST_SHADOW_PRISON_C));
                    }
                    break;
                case SHADOW_PRISON_NORTH:
                    switch (prisonTrap)
                    {
                        case SHADOW_PRISON_TRAP_A: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_NORTH_SHADOW_PRISON_A));
                        case SHADOW_PRISON_TRAP_B: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_NORTH_SHADOW_PRISON_B));
                        case SHADOW_PRISON_TRAP_C: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_NORTH_SHADOW_PRISON_C));
                    }
                    break;
                case SHADOW_PRISON_EAST:
                    switch (prisonTrap)
                    {
                        case SHADOW_PRISON_TRAP_A: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_EAST_SHADOW_PRISON_A));
                        case SHADOW_PRISON_TRAP_B: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_EAST_SHADOW_PRISON_B));
                        case SHADOW_PRISON_TRAP_C: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_EAST_SHADOW_PRISON_C));
                    }
                    break;
                case SHADOW_PRISON_SOUTH:
                    switch (prisonTrap)
                    {
                        case SHADOW_PRISON_TRAP_A: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SOUTH_SHADOW_PRISON_A));
                        case SHADOW_PRISON_TRAP_B: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SOUTH_SHADOW_PRISON_B));
                        case SHADOW_PRISON_TRAP_C: return m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SOUTH_SHADOW_PRISON_C));
                    }
                    break;
            }

            return NULL;
        }

        int8 GetFreePrisonIndex() const
        {
            for (uint8 i = 0; i < MAX_SHADOW_PRISONS; ++i)
            {
                if (m_Prisons[i].GetTargetGuid() == 0)
                    return i;
            }

            return -1;
        }

        PrisonData* GetPrisonDataByTrap(uint64 trapGuid)
        {
            for (uint8 i = 0; i < MAX_SHADOW_PRISONS; ++i)
            {
                if (m_Prisons[i].HasPlayer() && 
                    (m_Prisons[i].GetTrapGuids().find(trapGuid) != m_Prisons[i].GetTrapGuids().end()))
                    return &m_Prisons[i];
            }

            return NULL;
        }

        PrisonData* GetPrisonDataByPlayer(uint64 playerGuid)
        {
            for (uint8 i = 0; i < MAX_SHADOW_PRISONS; ++i)
            {
                if (m_Prisons[i].HasPlayer() && m_Prisons[i].GetTargetGuid() == playerGuid)
                    return &m_Prisons[i];
            }

            return NULL;
        }

        void ActivatePrisonAndTraps(uint8 prison, uint64 playerGuid, uint32 spellId)
        {
            PrisonData& prisonData = m_Prisons[prison];

            if (GameObject* pGo = GetPrison(prison))
            {
                pGo->SetGoState(GO_STATE_ACTIVE);
                prisonData.SetPrisonGuid(pGo->GetGUID());
            }

            uint8 skipTrap = urand(0, MAX_SHADOW_PRISON_TRAPS - 1);

            for (uint8 j = 0; j < MAX_SHADOW_PRISON_TRAPS; ++j)
            {
                if (j == skipTrap)
                    continue;

                if (GameObject* prisonTrap = GetPrisonTrap(prison, j))
                {
                    prisonTrap->SetGoState(GO_STATE_READY);
                    prisonTrap->AI()->SetGUID(playerGuid, DATA_TRAPPED_PLAYER);
                    prisonData.AddTrapGuid(prisonTrap->GetGUID());
                }
            }

            prisonData.SetPrison(prison);
            prisonData.SetTargetGuid(playerGuid);
            prisonData.SetSpellId(spellId);
        }

        void DeactivatePrisonAndTraps(uint8 prison)
        {
            FreePrisonPlayer(prison);

            if (GameObject* pGo = GetPrison(prison))
                pGo->SetGoState(GO_STATE_READY);

            for (uint8 j = 0; j < MAX_SHADOW_PRISON_TRAPS; ++j)
            {
                if (GameObject* prisonTrap = GetPrisonTrap(prison, j))
                {
                    prisonTrap->SetGoState(GO_STATE_ACTIVE);
                    //prisonTrap->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                }
            }
        }

        void FreePrisonPlayer(uint8 prison)
        {
            if (prison >= MAX_SHADOW_PRISONS)
                return;

            if (m_Prisons[prison].HasPlayer())
                if (Player* player = sObjectAccessor->FindPlayer(m_Prisons[prison].GetTargetGuid()))
                    player->RemoveAura(m_Prisons[prison].GetSpellId());

            m_Prisons[prison].Clear();
        }

        static uint32 GetSpellIdForPrison(uint8 prison)
        {
            ASSERT(prison < MAX_SHADOW_PRISONS);

            return prisonSpells[prison];
        }

    private:

        InstanceScript* m_pInstance;
        PrisonData m_Prisons[MAX_SHADOW_PRISONS];
};

class boss_sha_of_pride : public CreatureScript
{
    public:
        boss_sha_of_pride() : CreatureScript("boss_sha_of_pride") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sha_of_prideAI(creature);
        }

        struct boss_sha_of_prideAI : public BossAI
        {
            boss_sha_of_prideAI(Creature* creature) : BossAI(creature, DATA_SHA_OF_PRIDE), m_PrisonController(instance)
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 21.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 21.0f);

                energyTimer = SHA_POWER_PERIODIC;
                isUnleashed = false;
            }

            void Reset()
            {
                _Reset();

                me->SetReactState(REACT_DEFENSIVE);

                m_PrisonController.DeactivateAllPrisons();

                InitPowers();

                KillOvercomePlayers();
                RemoveInstanceAuras();

                SendResetToNorushen();

                energyTimer = SHA_POWER_PERIODIC;
                isUnleashed = false;
            }

            void AttackStart(Unit* who)
            {
                if (!who)
                    return;

                if (me->Attack(who, true))
                    DoStartNoMovement(who);
            }

            void EnterCombat(Unit* who)
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_REACHING_ATTACK, 2000);
                events.ScheduleEvent(EVENT_MARK_OF_ARROGANCE, 20500);
                events.ScheduleEvent(EVENT_SELF_REFLECTION, 20500);
                events.ScheduleEvent(EVENT_SHADOW_PRISON, 42000);
                events.ScheduleEvent(EVENT_WOUNDED_PRIDE, 10000);
                events.ScheduleEvent(EVENT_MANIFESTATION, 48000);

                ApplyPrideOnPlayers();

                SendStartToNorushen();

                instance->SetBossState(DATA_SHA_OF_PRIDE, IN_PROGRESS);
                DoZoneInCombat();
            }

            void SetGUID(uint64 guid, int32 type)
            {
                if (type == DATA_TRAP_USED)
                {
                    m_PrisonController.TrapUsed(guid);
                }
                else if (type == DATA_TRAP_DEACTIVATED)
                {
                    m_PrisonController.FreePlayer(guid);
                }
            }

            void KilledUnit(Unit* victim)
            {
                if (!victim || !victim->IsPlayer())
                    return;

                Talk(SAY_KILL);

                m_PrisonController.FreePlayer(victim->GetGUID());
            }

            void JustDied(Unit* who)
            {
                _JustDied();

                DoPlaySoundToSet(me, SOUND_DEATH);

                KillOvercomePlayers();
                RemoveInstanceAuras();

                m_PrisonController.DeactivateAllPrisons();

                switch (GetDifficulty())
                {
                    case MAN10_DIFFICULTY:
                        instance->DoRespawnGameObject(instance->GetData64(DATA_VAULT_OF_FORBIDDEN_TREASURES_10), DAY);
                        break;
                    case MAN25_DIFFICULTY:
                        instance->DoRespawnGameObject(instance->GetData64(DATA_VAULT_OF_FORBIDDEN_TREASURES_25), DAY);
                        break;
                    case MAN10_HEROIC_DIFFICULTY:
                        instance->DoRespawnGameObject(instance->GetData64(DATA_VAULT_OF_FORBIDDEN_TREASURES_10H), DAY);
                        break;
                    case MAN25_HEROIC_DIFFICULTY:
                        instance->DoRespawnGameObject(instance->GetData64(DATA_VAULT_OF_FORBIDDEN_TREASURES_25H), DAY);
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                UpdateEnergy(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (CheckUnleashed())
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_REACHING_ATTACK:
                            if (!me->IsWithinMeleeRange(me->getVictim()))
                                DoCastVictim(SPELL_REACHING_ATTACK);
                            events.ScheduleEvent(EVENT_REACHING_ATTACK, 2000);
                            break;
                        case EVENT_MARK_OF_ARROGANCE:
                            DoCastAOE(SPELL_MARK_OF_ARROGANCE);
                            events.ScheduleEvent(EVENT_MARK_OF_ARROGANCE, 20500);
                            break;
                        case EVENT_SWELLING_PRIDE:
                            Talk(SAY_SWELLING_PRIDE);

                            DoCastAOE(SPELL_SWELLING_PRIDE);

                            events.RescheduleEvent(EVENT_WOUNDED_PRIDE, 11000);
                            events.RescheduleEvent(EVENT_MANIFESTATION, 48000);
                            events.RescheduleEvent(EVENT_SELF_REFLECTION, 20500);
                            events.RescheduleEvent(EVENT_SHADOW_PRISON, 42000);
                            break;
                        case EVENT_SHADOW_PRISON:
                        {
                            Talk(SAY_CORRUPTED_PRISON);

                            std::list<Unit*> targets;
                            SelectTargetList(targets, ShadowPrisonTargetSelector(me), RAID_MODE(2, 4), SELECT_TARGET_RANDOM);

                            if (targets.size() > 1)
                            {
                                targets.remove(me->getVictim());
                            }
                            else if (targets.empty())
                            {
                                targets.push_back(me->getVictim());
                            }

                            for (std::list<Unit*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                            { 
                                m_PrisonController.CatchPlayer(me, (*itr)->GetGUID());
                            }

                            events.ScheduleEvent(EVENT_SHADOW_PRISON, 42000);
                            break;
                        }
                        case EVENT_WOUNDED_PRIDE:
                            DoCastVictim(SPELL_WOUNDED_PRIDE);
                            events.ScheduleEvent(EVENT_WOUNDED_PRIDE, 26000);
                            break;
                        case EVENT_MANIFESTATION:
                            SpawnManifestation();
                            break;
                        case EVENT_SELF_REFLECTION:
                            Talk(SAY_SELF_REFLECTION);
                            DoCast(me, SPELL_SELF_REFLECTION);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            void InitPowers()
            {
                me->AddAura(SPELL_ZERO_POWER, me);

                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, MAX_SHA_POWER);
                me->SetPower(POWER_ENERGY, 0);
            }

            void UpdateEnergy(const uint32 diff)
            {
                if (energyTimer <= diff)
                {
                    energyTimer = SHA_POWER_PERIODIC;

                    int32 curEnergy = me->GetPower(POWER_ENERGY);
                    if (curEnergy < MAX_SHA_POWER)
                    {
                        int32 newEnergy = curEnergy + 1;
                        if (newEnergy >= MAX_SHA_POWER)
                        {
                            events.ScheduleEvent(EVENT_SWELLING_PRIDE, 1);
                        }

                        me->SetPower(POWER_ENERGY, curEnergy + 1);
                    }
                }
                else
                {
                    energyTimer -= diff;
                }
            }

            void ApplyPrideOnPlayers()
            {
                instance->DoAddAuraOnPlayers(SPELL_PRIDE);
            }

            void RemoveInstanceAuras()
            {
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_PRISON_1);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_PRISON_2);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_PRISON_3);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTED_PRISON_4);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PRIDE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERCOME_1);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERCOME_2);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_OF_ARROGANCE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_GIFT_OF_THE_TITANS_1);
            }

            void KillOvercomePlayers()
            {
                Map::PlayerList const& PlayerList = instance->instance->GetPlayers();
                if (!PlayerList.isEmpty())
                {
                    for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                    {
                        if (Player* player = itr->getSource())
                        {
                            // kill only charmed targets
                            if (player->HasAura(SPELL_OVERCOME_2))
                            {
                                me->Kill(player);
                            }
                        }
                    }
                }
            }

            void SpawnManifestation()
            {
                me->SummonCreature(NPC_MANIFESTATION_OF_PRIDE, manifestationPos);
            }

            bool CheckUnleashed()
            {
                if (isUnleashed)
                    return false;

                if (me->GetHealthPct() > 30.0f)
                    return false;

                isUnleashed = true;

                Talk(SAY_UNLEASHED);

                DoCast(me, SPELL_UNLEASHED);

                SendUnleashedToNorushen();

                return true;
            }

            void SendStartToNorushen()
            {
                if (Creature* pNorushen = Creature::GetCreature(*me, instance->GetData64(DATA_SHA_OF_PRIDE_NORUSHEN)))
                {
                    if (pNorushen->AI())
                        pNorushen->AI()->DoAction(ACTION_COMBAT_START);
                }
            }

            void SendResetToNorushen()
            {
                if (Creature* pNorushen = instance->instance->GetCreature(instance->GetData64(DATA_SHA_OF_PRIDE_NORUSHEN)))
                {
                    pNorushen->AI()->DoAction(ACTION_RESET);
                }
            }

            void SendUnleashedToNorushen()
            {
                if (Creature* pNorushen = instance->instance->GetCreature(instance->GetData64(DATA_SHA_OF_PRIDE_NORUSHEN)))
                {
                    pNorushen->AI()->DoAction(ACTION_UNLEASHED);
                }
            }

            void SendDoneToNorushen()
            {
                if (Creature* pNorushen = instance->instance->GetCreature(instance->GetData64(DATA_SHA_OF_PRIDE_NORUSHEN)))
                {
                    pNorushen->AI()->DoAction(ACTION_COMPLETED);
                }
            }

        private:

            PrisonController m_PrisonController;
            uint32 energyTimer;
            bool isUnleashed;

        private:

            struct ShadowPrisonTargetSelector : public std::unary_function<Unit*, bool>
            {
                const Unit* me;
                float m_dist;
                bool m_playerOnly;
                int32 m_aura;

                ShadowPrisonTargetSelector(Unit const* unit) : me(unit) {}

                bool operator()(Unit const* target) const
                {
                    if (!me)
                        return false;

                    if (!target)
                        return false;

                    if (!target->IsPlayer())
                        return false;

                    Unit* _target = (Unit*)target;

                    if (Player* player = _target->ToPlayer())
                    {
                        if (player->GetRoleForGroup() == ROLES_TANK ||
                            player->GetRoleForGroup() == ROLES_HEALER)
                            return false;
                    }

                    return true;
                }
            };

        };
};

class npc_sha_of_pride_norushen : public CreatureScript
{
    public:
        npc_sha_of_pride_norushen() : CreatureScript("npc_sha_of_pride_norushen") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sha_of_pride_norushenAI(creature);
        }

        struct npc_sha_of_pride_norushenAI : public Scripted_NoMovementAI
        {
            npc_sha_of_pride_norushenAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);
                SetCanSeeEvenInPassiveMode(true);

                pInstance = me->GetInstanceScript();
                introDone = false;
                shaSpawned = false;
                lingeringCorruptionCount = 0;
                shaVortexGuid = 0;
                isFinalGiftDone = false;

                SpawnVortexAndLingeringCorruption();
            }

            void Reset()
            {
                introDone = false;
                shaSpawned = false;
                lingeringCorruptionCount = 0;
                isFinalGiftDone = false;
                //shaVortexGuid = 0; do not do it
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (introDone)
                    return;

                if (who->IsPlayer())
                {
                    if (me->GetDistance(who) <= 10.0f)
                    {
                        introDone = true;

                        Talk(SAY_NORUSHEN_INTRO_1);
                    }
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_LINGERING_CORRUPTION_DIED:
                        UpdateLingeringCorruption(1);
                        break;
                    case ACTION_UNLEASHED:
                        DelayedFinalGift();
                        //FinalGift();
                        break;
                    case ACTION_RESET:
                        isFinalGiftDone = false;
                        break;
                    case ACTION_COMPLETED:
                        events.CancelEvent(EVENT_FINAL_GIFT);
                        break;
                    case ACTION_COMBAT_START:
                        events.ScheduleEvent(EVENT_GIFT_OF_THE_TITANS, 20000);
                        break;
                }
            }

            void JustDied(Unit* /*killer*/) override
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SPAWN_SHA_OF_PRIDE:
                            SpawnShaOfPride();
                            break;
                        case EVENT_GIFT_OF_THE_TITANS:
                            Talk(SAY_GIFT_OF_THE_TITANS);
                            me->CastSpell(me, SPELL_GIFT_OF_THE_TITANS, false);
                            events.ScheduleEvent(EVENT_GIFT_OF_THE_TITANS, 40000);
                            break;
                        case EVENT_FINAL_GIFT:
                            FinalGift();
                            break;
                        default:
                            break;
                    }
                }
            }

        private:

            void SpawnVortexAndLingeringCorruption()
            {
                if (pInstance)
                {
                    if (pInstance->GetBossState(DATA_SHA_OF_PRIDE) == DONE)
                        return;
                }

                if (Creature* pCreature = me->SummonCreature(NPC_SHA_VORTEX, shaVortexPos))
                {
                    shaVortexGuid = pCreature->GetGUID();

                    pCreature->AddAura(SPELL_SHA_VORTEX, pCreature);

                    for (uint8 i = 0; i < MAX_LINGERING_CORRUPTIONS; ++i)
                    {
                        Position pos;
                        pCreature->GetRandomNearPosition(pos, frand(20.0f, 60.0f));
                        pCreature->SummonCreature(NPC_LINGERING_CORRUPTION, pos);
                    }
                }
            }

            void UpdateLingeringCorruption(int32 addCount)
            {
                if (shaSpawned)
                    return;

                lingeringCorruptionCount += addCount;

                if (!introDone)
                {
                    introDone = true;

                    Talk(SAY_NORUSHEN_INTRO_1);
                }

                if (lingeringCorruptionCount >= MAX_LINGERING_CORRUPTIONS)
                {
                    shaSpawned = true;

                    Talk(SAY_NORUSHEN_INTRO_2);

                    if (Creature* pShaVortex = me->GetCreature(*me, shaVortexGuid))
                        pShaVortex->DespawnOrUnsummon();

                    DelayedSpawnShaOfPride();
                }
            }

            bool CanSpawnShaOfPride()
            {
                if (!pInstance)
                    return false;

                if (pInstance->GetData64(DATA_SHA_OF_PRIDE) != 0)
                    return false;

                if (pInstance->GetBossState(DATA_SHA_OF_PRIDE) == IN_PROGRESS ||
                    pInstance->GetBossState(DATA_SHA_OF_PRIDE) == DONE)
                    return false;

                return true;
            }

            void DelayedSpawnShaOfPride()
            {
                if (!CanSpawnShaOfPride())
                    return;

                events.ScheduleEvent(EVENT_SPAWN_SHA_OF_PRIDE, 10000);
            }

            void SpawnShaOfPride()
            {
                if (!CanSpawnShaOfPride())
                    return;

                if (Creature* pSha = me->SummonCreature(NPC_SHA_OF_PRIDE, shaVortexPos))
                {
                    pSha->AI()->Talk(SAY_INTRO);
                }
            }

            void DelayedFinalGift()
            {
                events.CancelEvent(EVENT_GIFT_OF_THE_TITANS);
                events.ScheduleEvent(EVENT_FINAL_GIFT, 9000);
            }

            void FinalGift()
            {
                if (isFinalGiftDone)
                    return;

                isFinalGiftDone = true;

                events.CancelEvent(EVENT_GIFT_OF_THE_TITANS);

                Talk(SAY_FINAL_GIFT);

                DoCastAOE(SPELL_FINAL_GIFT);
            }

        private:

            InstanceScript* pInstance;
            uint64 shaVortexGuid;
            bool introDone;
            uint32 lingeringCorruptionCount;
            bool shaSpawned;
            bool isFinalGiftDone;

        };
};

class npc_sha_of_pride_lingering_corruption : public CreatureScript
{
    public:
        npc_sha_of_pride_lingering_corruption() : CreatureScript("npc_sha_of_pride_lingering_corruption") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sha_of_pride_lingering_corruptionAI(creature);
        }

        struct npc_sha_of_pride_lingering_corruptionAI : public ScriptedAI
        {
            npc_sha_of_pride_lingering_corruptionAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                
            }

            void JustDied(Unit* who)
            {
                DoCastAOE(SPELL_CORRUPTED_TOUCH, true);

                SendLingeringCorruptionDied();

                me->DespawnOrUnsummon(2000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:

            void SendLingeringCorruptionDied()
            {
                if (pInstance)
                {
                    if (Creature* pNorushen = pInstance->instance->GetCreature(pInstance->GetData64(DATA_SHA_OF_PRIDE_NORUSHEN)))
                    {
                        pNorushen->AI()->DoAction(ACTION_LINGERING_CORRUPTION_DIED);
                    }
                }
            }

        private:

            InstanceScript* pInstance;
        };
};

class npc_sha_of_pride_manifestation_of_pride : public CreatureScript
{
    public:
        npc_sha_of_pride_manifestation_of_pride() : CreatureScript("npc_sha_of_pride_manifestation_of_pride") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sha_of_pride_manifestation_of_prideAI(creature);
        }

        struct npc_sha_of_pride_manifestation_of_prideAI : public Scripted_NoMovementAI
        {
            npc_sha_of_pride_manifestation_of_prideAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                
            }

            void IsSummonedBy(Unit* owner)
            {
                DoCast(me, SPELL_MANIFESTATION_SPAWN, true);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MOVE, 3000);
            }

            void JustDied(Unit* who)
            {
                DoCastAOE(SPELL_LAST_WORD, true);

                me->DespawnOrUnsummon(2000);
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
                        case EVENT_MOVE:
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveAura(SPELL_MANIFESTATION_SPAWN);
                            events.ScheduleEvent(EVENT_MOCKING_BLAST, urand(1000, 3000));
                            break;
                        case EVENT_MOCKING_BLAST:
                            DoCastAOE(SPELL_MOCKING_BLAST);
                            events.ScheduleEvent(EVENT_MOCKING_BLAST, 3000);
                            break;
                    }
                }
            }

        private:

        private:

            InstanceScript* pInstance;
        };
};

class npc_sha_of_pride_reflection : public CreatureScript
{
    public:
        npc_sha_of_pride_reflection() : CreatureScript("npc_sha_of_pride_reflection") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sha_of_pride_reflectionAI(creature);
        }

        struct npc_sha_of_pride_reflectionAI : public ScriptedAI
        {
            npc_sha_of_pride_reflectionAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                
            }

            void IsSummonedBy(Unit* owner)
            {
                DoCast(me, SPELL_SELF_REFLECTION_SPAWN, true);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_REFLECTION_DMG, 3000);
                events.ScheduleEvent(EVENT_MOVE, 4000);
            }

            void JustDied(Unit* who)
            {
                me->DespawnOrUnsummon(2000);
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
                        case EVENT_MOVE:
                            me->SetReactState(REACT_AGGRESSIVE);
                            break;
                        case EVENT_REFLECTION_DMG:
                            DoCastAOE(SPELL_SELF_REFLECTION_DMG);
                            me->RemoveAura(SPELL_SELF_REFLECTION_SPAWN);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

        private:

            InstanceScript* pInstance;
        };
};

class go_sha_of_pride_shadow_prison_trap : public GameObjectScript
{
    public:
        go_sha_of_pride_shadow_prison_trap() : GameObjectScript("go_sha_of_pride_shadow_prison_trap") { }

        GameObjectAI* GetAI(GameObject* pGo) const
        {
            return new go_sha_of_pride_shadow_prison_trapAI(pGo);
        }

        struct go_sha_of_pride_shadow_prison_trapAI : public GameObjectAI
        {
            go_sha_of_pride_shadow_prison_trapAI(GameObject* p_Go) : GameObjectAI(p_Go) 
            { 
                playersTimer = 2000;
                catchedPlayerGuid = 0;
            }

            void SetGUID(const uint64& guid, int32 id) override
            {
                if (id == DATA_TRAPPED_PLAYER)
                {
                    catchedPlayerGuid = guid;
                }
            }

            void UpdateAI(uint32 diff)
            {
                UpdatePlayers(diff);
            }

        private:

            void UpdatePlayers(const uint32 diff)
            {
                if (go->GetGoState() != GO_STATE_READY)
                    return;

                if (playersTimer <= diff)
                {
                    playersTimer = 1000;

                    if (PlayersCount(10.0f) >= 2)
                    {
                        SendFreePlayer();
                    }
                }
                else
                {
                    playersTimer-= diff;
                }
            }

            void UpdatePlayersOld(const uint32 diff)
            {
                if (go->GetGoState() != GO_STATE_READY)
                    return;

                if (playersTimer <= diff)
                {
                    playersTimer = 2000;

                    Player* player = GetPlayer(10.0f);
                    if (!player)
                        return;

                    float dist = go->GetDistance2d(player);

                    if (dist < 5.0f || dist > 7.5f)
                        return;

                    //               _______A
                    //        /\               
                    //       /  \              
                    //      /    \   _______B 
                    //     /@@@@@@\                  
                    //    /@@@@@@@@\ _______C         
                    //
                    //   1<--------->2      1 and 2 - angles, A-C - radius, B-C - height for area
                    // 
                    //
                    // A-B = 5.0f 
                    // A-C > 7.5f
                    //
                    // so area is between 5.0f and 7.5f height and 1 and 2 angles
                    //
                    // I've calculated 3 basic constants: 
                    // orientation of trap (GameObject) and 1 and 2 angles we need

                    // if orientation of the trap is defOri
                    // then player's angle should be between minAngle and maxAngle
                    const float defOri = 5.335980f;
                    const float minAngle = 1.868661f;
                    const float maxAngle = 3.25126290f;

                    float diff = defOri - go->GetOrientation();
                    float normalizedDiff = go->NormalizeOrientation(diff);
                    float plrAngle = go->GetAngle(player);
                    float normalizedAngleWithDiff = go->NormalizeOrientation(plrAngle + normalizedDiff);

                    if (normalizedAngleWithDiff > minAngle && normalizedAngleWithDiff < maxAngle)
                    {
                        go->SetGoState(GO_STATE_ACTIVE);
                        SendTrapUsed();
                    }                    
                }
                else
                {
                    playersTimer -= diff;
                }
            }

            void SendTrapUsed()
            {
                if (InstanceScript* pInstance = go->GetInstanceScript())
                {
                    if (Creature* pSha = pInstance->instance->GetCreature(pInstance->GetData64(DATA_SHA_OF_PRIDE)))
                    {
                        pSha->AI()->SetGUID(go->GetGUID(), DATA_TRAP_USED);                    
                    }
                }
            }

            void SendFreePlayer()
            {
                if (InstanceScript* pInstance = go->GetInstanceScript())
                {
                    if (Creature* pSha = pInstance->instance->GetCreature(pInstance->GetData64(DATA_SHA_OF_PRIDE)))
                    {
                        pSha->AI()->SetGUID(catchedPlayerGuid, DATA_TRAP_DEACTIVATED); 
                    }
                }
            }

            // use this function instead of basic 'GetPlayerListInGrid'
            Player* GetPlayer(float range)
            {
                uint32 auraId = PrisonController::GetSpellIdForPrisonTrapEntry(go->GetEntry());
                if (!auraId)
                    return NULL;

                Player* player = NULL;
                PlayerTrapActivatorCheck check(go, range, auraId);
                JadeCore::PlayerSearcher<PlayerTrapActivatorCheck> searcher(go, player, check);
                go->VisitNearbyWorldObject(range, searcher);
                return player;
            }

            uint32 PlayersCount(float range)
            {
                uint32 auraId = PrisonController::GetSpellIdForPrisonTrapEntry(go->GetEntry());
                if (!auraId)
                    return 0;

                std::list<Player*> players;
                PlayerTrapActivatorCheckNew check(go, 5.0f, 7.5f, auraId);
                JadeCore::PlayerListSearcher<PlayerTrapActivatorCheckNew> searcher(go, players, check);
                go->VisitNearbyWorldObject(range, searcher);
                return players.size();
            }

        private:

            uint32 playersTimer;
            uint64 catchedPlayerGuid;

        private:

            class PlayerTrapActivatorCheck
            {
                public:
                    PlayerTrapActivatorCheck(WorldObject const* obj, float range, uint32 skipAuraId) : _obj(obj), _range(range), _skipAuraId(skipAuraId) {}
                    bool operator()(Player* u)
                    {
                        if (!u->isAlive())
                            return false;

                        if (_obj->GetDistance2d(u) > _range)
                            return false;

                        if (u->HasAura(_skipAuraId))
                            return false;

                        return true;
                    }

                private:
                    WorldObject const* _obj;
                    float _range;
                    uint32 _skipAuraId;
            };

            class PlayerTrapActivatorCheckNew
            {
                public:
                    PlayerTrapActivatorCheckNew(WorldObject const* obj, float rangeMin, float rangeMax, uint32 skipAuraId) : 
                        _obj(obj), _rangeMax(rangeMax), _rangeMin(rangeMin), _skipAuraId(skipAuraId) {}
                    bool operator()(Player* u)
                    {
                        if (!u->isAlive())
                            return false;

                        if (_obj->GetDistance2d(u) < _rangeMin)
                            return false;

                        if (_obj->GetDistance2d(u) > _rangeMax)
                            return false;

                        if (u->HasAura(_skipAuraId))
                            return false;

                        return true;
                    }

                private:
                    WorldObject const* _obj;
                    float _rangeMax;
                    float _rangeMin;
                    uint32 _skipAuraId;
            };

        };
};

class npc_sha_of_pride_jaina_proudmoore : public CreatureScript
{
    public:
        npc_sha_of_pride_jaina_proudmoore() : CreatureScript("npc_sha_of_pride_jaina_proudmoore") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sha_of_pride_jaina_proudmooreAI(creature);
        }

        struct npc_sha_of_pride_jaina_proudmooreAI : public ScriptedAI
        {
            npc_sha_of_pride_jaina_proudmooreAI(Creature* creature) : ScriptedAI(creature), m_SceneHelper(creature->GetGUID(), creature->GetMap())
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {

            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_SHA_OF_PRIDE_JAINA)
                {
                    m_SceneHelper.Clear();

                    m_SceneHelper.AddSceneActionTalk(SAY_JAINA_INTRO_1, 1000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JAINA_INTRO_2, 14000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JAINA_INTRO_3, 6000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JAINA_INTRO_4, 13000);
                    m_SceneHelper.AddSceneSummonCreature(NPC_PORTAL_TO_ORGRIMMAR_1, npcAfterShaOfPridePos[3], 18000);
                    m_SceneHelper.AddSceneActionMove(npcAfterShaOfPridePos[4], true, 1);
                    if (IsAllianceInstance())
                    {
                        m_SceneHelper.AddSceneActionTalk(SAY_JAINA_INTRO_5, 1);
                    }

                    m_SceneHelper.Activate();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                m_SceneHelper.Update(diff);
            }

        private:

            bool IsAllianceInstance()
            {
                return (pInstance && pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE);
            }

        private:

            InstanceScript* pInstance;
            SceneHelper m_SceneHelper;
        };
};

class npc_sha_of_pride_lorthemar_theron : public CreatureScript
{
    public:
        npc_sha_of_pride_lorthemar_theron() : CreatureScript("npc_sha_of_pride_lorthemar_theron") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sha_of_pride_lorthemar_theronAI(creature);
        }

        struct npc_sha_of_pride_lorthemar_theronAI : public ScriptedAI
        {
            npc_sha_of_pride_lorthemar_theronAI(Creature* creature) : ScriptedAI(creature), m_SceneHelper(creature->GetGUID(), creature->GetMap())
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {

            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_SHA_OF_PRIDE_LORTHEMAR)
                {
                    m_SceneHelper.Clear();

                    m_SceneHelper.AddSceneActionTalk(SAY_LORTHEMAR_INTRO_1, 11000);
                    m_SceneHelper.AddSceneActionTalk(SAY_LORTHEMAR_INTRO_2, 6000);
                    m_SceneHelper.AddSceneActionTalk(SAY_LORTHEMAR_INTRO_3, 11000);
                    m_SceneHelper.AddSceneActionTalk(SAY_LORTHEMAR_INTRO_4, 18000);
                    m_SceneHelper.AddSceneSummonCreature(NPC_PORTAL_TO_ORGRIMMAR_1, npcAfterShaOfPridePos[5], 5000);
                    m_SceneHelper.AddSceneActionMove(npcAfterShaOfPridePos[6], true, 1);
                    if (IsHordeInstance())
                    {
                        m_SceneHelper.AddSceneActionTalk(SAY_LORTHEMAR_INTRO_5, 1);
                    }

                    m_SceneHelper.Activate();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                m_SceneHelper.Update(diff);
            }

        private:

            bool IsHordeInstance()
            {
                return (pInstance && pInstance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE);
            }

        private:

            InstanceScript* pInstance;
            SceneHelper m_SceneHelper;
        };
};

class npc_sha_of_pride_portal_to_orgrimmar : public CreatureScript
{
    public:
        npc_sha_of_pride_portal_to_orgrimmar() : CreatureScript("npc_sha_of_pride_portal_to_orgrimmar") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sha_of_pride_portal_to_orgrimmarAI(creature);
        }

        struct npc_sha_of_pride_portal_to_orgrimmarAI : public Scripted_NoMovementAI
        {
            npc_sha_of_pride_portal_to_orgrimmarAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);
                me->SetDisplayId(51795);
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_CLIENT_CANNOT_TARGET);

                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {

            }

            void UpdateAI(const uint32 diff)
            {
                
            }

            void sGossipHello(Player* player) override
            {
                if (IsAllianceInstance())
                {
                    player->NearTeleportTo(galakrasPlayersPos[0]);
                }
                else if (IsHordeInstance())
                {
                    player->NearTeleportTo(galakrasPlayersPos[1]);
                }
                
                SendPlayerTeleported();
            }

        private:

            bool IsAllianceInstance()
            {
                return (pInstance && pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE);
            }

            bool IsHordeInstance()
            {
                return (pInstance && pInstance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE);
            }

            void SendPlayerTeleported()
            {
                if (pInstance)
                {
                    pInstance->SetData(DATA_FIRST_GALAKRAS_INTRO, DONE);
                }
            }

        private:

            InstanceScript* pInstance;
        };
};

class spell_sha_of_pride_corrupted_touch : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_corrupted_touch() : SpellScriptLoader("spell_sha_of_pride_corrupted_touch") { }

        class spell_sha_of_pride_corrupted_touch_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_pride_corrupted_touch_AuraScript);

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (!GetUnitOwner())
                    return;

                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_CORRUPTED_TOUCH_DMG, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectApplyFn(spell_sha_of_pride_corrupted_touch_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_of_pride_corrupted_touch_AuraScript();
        }
};

class spell_sha_of_pride_swelling_pride : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_swelling_pride() : SpellScriptLoader("spell_sha_of_pride_swelling_pride") { }

        class spell_sha_of_pride_swelling_pride_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_swelling_pride_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                DoUniqueEffectsOnUnit(GetHitUnit());

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_swelling_pride_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }

        private:

            void DoUniqueEffectsOnUnit(Unit* target)
            {
                Unit* caster = GetCaster();

                int32 currentPride = target->GetPower(POWER_ALTERNATE_POWER);

                if (currentPride >= 25 && currentPride < 50)
                {
                    caster->CastSpell(target, SPELL_BURSTING_PRIDE, true);
                }
                else if (currentPride >= 50 && currentPride < 75)
                {
                    caster->AddAura(SPELL_PROJECTION_MARK, target);
                    caster->CastSpell(target, SPELL_PROJECTION_1, true);

                    // HACK:
                    // We need to save target guid to handle it later
                    // There are no correct ways to do it now
                    // So save target guid as original caster
                    caster->CastSpell(target, SPELL_PROJECTION_MISSILE, true, NULL, NULL, target->GetGUID());
                }
                else if (currentPride >= 75 && currentPride < 100)
                {
                    caster->CastSpell(target, SPELL_AURA_OF_PRIDE, true);
                }
                else if (currentPride >= 100)
                {
                    if (target->HasAura(SPELL_OVERCOME_1))
                    {
                        caster->CastSpell(target, SPELL_OVERCOME_2, true);
                    }
                }
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_swelling_pride_SpellScript();
        }
};

class spell_sha_of_pride_mark_of_arrogance : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_mark_of_arrogance() : SpellScriptLoader("spell_sha_of_pride_mark_of_arrogance") { }

        class spell_sha_of_pride_mark_of_arrogance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_pride_mark_of_arrogance_AuraScript);

            void HandleDispel(DispelInfo* dispelInfo)
            {
                if (Unit* dispeller = dispelInfo->GetDispeller())
                    TryIncreasePrideOnUnit(dispeller, DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                AfterDispel += AuraDispelFn(spell_sha_of_pride_mark_of_arrogance_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_of_pride_mark_of_arrogance_AuraScript();
        }
};

class spell_sha_of_pride_bursting_pride_dmg : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_bursting_pride_dmg() : SpellScriptLoader("spell_sha_of_pride_bursting_pride_dmg") { }

        class spell_sha_of_pride_bursting_pride_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_bursting_pride_dmg_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_bursting_pride_dmg_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_bursting_pride_dmg_SpellScript();
        }
};

class spell_sha_of_pride_projection_missile : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_projection_missile() : SpellScriptLoader("spell_sha_of_pride_projection_missile") { }

        class spell_sha_of_pride_projection_missile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_projection_missile_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetCaster())
                    return;

                WorldLocation const* loc = GetHitDest();

                bool doDamage = true;

                if (Unit* targetedPlayer = GetOriginalCaster())
                {
                    float dist = loc->GetExactDist2d(targetedPlayer);
                    if (dist < 4.0f)
                    {
                        doDamage = false;
                    }
                }

                if (doDamage)
                {
                    GetCaster()->CastSpell(loc->GetPositionX(), loc->GetPositionY(), loc->GetPositionZ(), SPELL_PROJECTION_DMG, true);
                }
                else
                {
                    GetCaster()->CastSpell(loc->GetPositionX(), loc->GetPositionY(), loc->GetPositionZ(), SPELL_PROJECTION_2, true);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_sha_of_pride_projection_missile_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_projection_missile_SpellScript();
        }
};

class spell_sha_of_pride_projection_dmg : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_projection_dmg() : SpellScriptLoader("spell_sha_of_pride_projection_dmg") { }

        class spell_sha_of_pride_projection_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_projection_dmg_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_projection_dmg_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_projection_dmg_SpellScript();
        }
};

class spell_sha_of_pride_aura_of_pride_dmg : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_aura_of_pride_dmg() : SpellScriptLoader("spell_sha_of_pride_aura_of_pride_dmg") { }

        class spell_sha_of_pride_aura_of_pride_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_aura_of_pride_dmg_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_aura_of_pride_dmg_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_aura_of_pride_dmg_SpellScript();
        }
};

class spell_sha_of_pride_pride : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_pride() : SpellScriptLoader("spell_sha_of_pride_pride") { }

        class spell_sha_of_pride_pride_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_pride_pride_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                int32 power = GetUnitOwner()->GetPower(POWER_ALTERNATE_POWER);

                if (power >= 100 && !GetUnitOwner()->HasAura(SPELL_OVERCOME_1))
                {
                    GetUnitOwner()->AddAura(SPELL_OVERCOME_1, GetUnitOwner());
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_of_pride_pride_AuraScript::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_of_pride_pride_AuraScript();
        }
};

class spell_sha_of_pride_wounded_pride : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_wounded_pride() : SpellScriptLoader("spell_sha_of_pride_wounded_pride") { }

        class spell_sha_of_pride_wounded_pride_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_pride_wounded_pride_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetUnitOwner())
                    return;

                Unit* attacker = eventInfo.GetActor();
                if (!attacker)
                    return;

                if (attacker->GetEntry() != NPC_SHA_OF_PRIDE)
                    return;

                TryIncreasePrideOnUnit(GetUnitOwner(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_of_pride_wounded_pride_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_of_pride_wounded_pride_AuraScript();
        }
};

class spell_sha_of_pride_mocking_blast : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_mocking_blast() : SpellScriptLoader("spell_sha_of_pride_mocking_blast") { }

        class spell_sha_of_pride_mocking_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_mocking_blast_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_mocking_blast_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_mocking_blast_SpellScript();
        }
};

class spell_sha_of_pride_last_word : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_last_word() : SpellScriptLoader("spell_sha_of_pride_last_word") { }

        class spell_sha_of_pride_last_word_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_last_word_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();

                if (!GetCaster())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    std::list<Player*> unitTargets;

                    pCreature->GetPlayerListInGrid(unitTargets, 100.0f);

                    if (!unitTargets.empty())
                    {
                        unitTargets.sort(JadeCore::DistanceOrderPred(pCreature));
                        unitTargets.resize(2);

                        for (std::list<Player*>::const_iterator itr = unitTargets.begin(); itr != unitTargets.end(); ++itr)
                        {
                            targets.push_back((*itr));
                        }
                    }
                }
            }

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_of_pride_last_word_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_last_word_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_last_word_SpellScript();
        }
};

class spell_sha_of_pride_self_reflection : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_self_reflection() : SpellScriptLoader("spell_sha_of_pride_self_reflection") { }

        class spell_sha_of_pride_self_reflection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_pride_self_reflection_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (Creature* pCreature = GetUnitOwner()->ToCreature())
                {
                    // TODO:
                    // Creature should target 5 players with highest pride, each player per tick
                    if (Unit* target = pCreature->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    {
                        pCreature->SummonCreature(NPC_REFLECTION, *target);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_of_pride_self_reflection_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_of_pride_self_reflection_AuraScript();
        }
};

class spell_sha_of_pride_self_reflection_dmg : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_self_reflection_dmg() : SpellScriptLoader("spell_sha_of_pride_self_reflection_dmg") { }

        class spell_sha_of_pride_self_reflection_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_self_reflection_dmg_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_self_reflection_dmg_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_self_reflection_dmg_SpellScript();
        }
};

class spell_sha_of_pride_corrupted_prison_periodic : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_corrupted_prison_periodic() : SpellScriptLoader("spell_sha_of_pride_corrupted_prison_periodic") { }

        class spell_sha_of_pride_corrupted_prison_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_pride_corrupted_prison_periodic_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                TryIncreasePrideOnUnit(GetUnitOwner(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_of_pride_corrupted_prison_periodic_AuraScript::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_of_pride_corrupted_prison_periodic_AuraScript();
        }
};

class spell_sha_of_pride_corrupted_prison_dmg : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_corrupted_prison_dmg() : SpellScriptLoader("spell_sha_of_pride_corrupted_prison_dmg") { }

        class spell_sha_of_pride_corrupted_prison_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_corrupted_prison_dmg_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_corrupted_prison_dmg_SpellScript::HandleHit, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_corrupted_prison_dmg_SpellScript();
        }
};

class spell_sha_of_pride_unleashed : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_unleashed() : SpellScriptLoader("spell_sha_of_pride_unleashed") { }

        class spell_sha_of_pride_unleashed_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_unleashed_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                PreventHitDefaultEffect(effIndex);

                if (Creature* pNorushen = GetHitUnit()->ToCreature())
                {
                    pNorushen->AI()->DoAction(ACTION_UNLEASHED);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_unleashed_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_INSTAKILL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_unleashed_SpellScript();
        }
};

class spell_sha_of_pride_unleashed_dmg : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_unleashed_dmg() : SpellScriptLoader("spell_sha_of_pride_unleashed_dmg") { }

        class spell_sha_of_pride_unleashed_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_unleashed_dmg_SpellScript);

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetHitUnit())
                    return;

                TryIncreasePrideOnUnit(GetHitUnit(), DEFAULT_PRIDE_INCREASE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_of_pride_unleashed_dmg_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_of_pride_unleashed_dmg_SpellScript();
        }
};

class spell_sha_of_pride_gift_titans : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_gift_titans() : SpellScriptLoader("spell_sha_of_pride_gift_titans") { }

        class spell_sha_of_pride_gift_titans_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_of_pride_gift_titans_SpellScript);

            void HandleCast()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                std::list<Player*> playerList;
                caster->GetPlayerListInGrid(playerList, 100.0f);
                playerList.remove_if(JadeCore::UnitAuraCheck(true, SPELL_OVERCOME_1));

                uint32 targetsCount = caster->GetMap()->Is25ManRaid() ? GIFT_OF_TITANS_COUNT_25 : GIFT_OF_TITANS_COUNT_10;

                if (playerList.size() > targetsCount)
                    JadeCore::RandomResizeList(playerList, targetsCount);

                for (Player* player : playerList)
                {
                    caster->AddAura(SPELL_GIFT_OF_THE_TITANS_1, player);
                }
            }

            void Register() override
            {
                OnCast += SpellCastFn(spell_sha_of_pride_gift_titans_SpellScript::HandleCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_sha_of_pride_gift_titans_SpellScript();
        }
};

class spell_sha_of_pride_gift_of_the_titans_aura : public SpellScriptLoader
{
    public:
        spell_sha_of_pride_gift_of_the_titans_aura() : SpellScriptLoader("spell_sha_of_pride_gift_of_the_titans_aura") { }

        class spell_sha_of_pride_gift_of_the_titans_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_pride_gift_of_the_titans_aura_AuraScript);

            void HandlePeriodicTick(AuraEffect const* aurEff)
            {
                PreventDefaultAction();

                Unit* owner = GetUnitOwner();
                if (!owner)
                    return;

                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, owner, 8.f);

                playerList.remove_if(JadeCore::UnitAuraCheck(false, SPELL_GIFT_OF_THE_TITANS_1));

                uint8 neededCount = owner->GetMap()->Is25ManRaid() ? 7 : 2;
                if (playerList.size() >= neededCount)
                {
                    if (!owner->HasAura(SPELL_POWER_OF_THE_TITANS))
                        owner->AddAura(SPELL_POWER_OF_THE_TITANS, owner);
                }
                else
                {
                    if (owner->HasAura(SPELL_POWER_OF_THE_TITANS))
                        owner->RemoveAura(SPELL_POWER_OF_THE_TITANS);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_of_pride_gift_of_the_titans_aura_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_sha_of_pride_gift_of_the_titans_aura_AuraScript();
        }
};

void AddSC_boss_sha_of_pride()
{
    new boss_sha_of_pride();                        // 71734
    new npc_sha_of_pride_norushen();                // 71965
    new npc_sha_of_pride_lingering_corruption();    // 72791
    new npc_sha_of_pride_manifestation_of_pride();  // 71946
    new npc_sha_of_pride_reflection();              // 72172
    new go_sha_of_pride_shadow_prison_trap();       // 221758, 221759, 221757, 221755, 221750, 221754, 221753, 221751, 221752, 221761, 221760, 221756
    new npc_sha_of_pride_jaina_proudmoore();        // 73598
    new npc_sha_of_pride_lorthemar_theron();        // 73605
    new npc_sha_of_pride_portal_to_orgrimmar();     // 73535 73536

    new spell_sha_of_pride_corrupted_touch();       // 149207
    new spell_sha_of_pride_swelling_pride();        // 144400
    new spell_sha_of_pride_mark_of_arrogance();     // 144351
    new spell_sha_of_pride_bursting_pride_dmg();    // 144911
    new spell_sha_of_pride_projection_missile();    // 145066
    new spell_sha_of_pride_projection_dmg();        // 145320
    new spell_sha_of_pride_aura_of_pride_dmg();     // 146818
    new spell_sha_of_pride_pride();                 // 144343
    new spell_sha_of_pride_wounded_pride();         // 144358
    new spell_sha_of_pride_mocking_blast();         // 144379
    new spell_sha_of_pride_last_word();             // 144370
    new spell_sha_of_pride_self_reflection();       // 144800
    new spell_sha_of_pride_self_reflection_dmg();   // 144788
    new spell_sha_of_pride_corrupted_prison_periodic(); // 144574, 144636, 144683, 144684
    new spell_sha_of_pride_corrupted_prison_dmg();  // 144615
    new spell_sha_of_pride_unleashed();             // 144832
    new spell_sha_of_pride_unleashed_dmg();         // 144836
    new spell_sha_of_pride_gift_titans();           // 146595
    new spell_sha_of_pride_gift_of_the_titans_aura();   // 144359
}
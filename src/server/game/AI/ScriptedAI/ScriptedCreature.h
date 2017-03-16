/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCRIPTEDCREATURE_H_
#define SCRIPTEDCREATURE_H_

#include "Creature.h"
#include "CreatureAI.h"
#include "CreatureAIImpl.h"
#include "InstanceScript.h"
#include "LFGMgr.h"

#define CAST_PLR(a)     (dynamic_cast<Player*>(a))
#define CAST_CRE(a)     (dynamic_cast<Creature*>(a))
#define CAST_AI(a, b)   (dynamic_cast<a*>(b))
#define ENSURE_AI(a,b)  (EnsureAI<a>(b))

template<class T, class U>
T* EnsureAI(U* ai)
{
    T* cast_ai = dynamic_cast<T*>(ai);
    ASSERT(cast_ai);
    return cast_ai;
};

#define GET_SPELL(a)    (const_cast<SpellInfo*>(sSpellMgr->GetSpellInfo(a)))

class InstanceScript;

class SummonList : public std::list<uint64>
{
    public:
        explicit SummonList(Creature* creature) : me(creature) {}
        void Summon(Creature* summon) { push_back(summon->GetGUID()); }
        void Despawn(Creature* summon) { remove(summon->GetGUID()); }
        void DespawnEntry(uint32 entry);
        void DespawnAll();

        template <class Predicate> void DoAction(int32 info, Predicate& predicate, uint16 max = 0)
        {
            // We need to use a copy of SummonList here, otherwise original SummonList would be modified
            std::list<uint64> listCopy = *this;
            JadeCore::Containers::RandomResizeList<uint64, Predicate>(listCopy, predicate, max);
            for (iterator i = listCopy.begin(); i != listCopy.end(); )
            {
                Creature* summon = Unit::GetCreature(*me, *i++);
                if (summon && summon->IsAIEnabled)
                    summon->AI()->DoAction(info);
            }
        }

        void DoZoneInCombat(uint32 entry = 0);
        void RemoveNotExisting();
        bool HasEntry(uint32 entry);
    private:
        Creature* me;
};

class EntryCheckPredicate
{
    public:
        EntryCheckPredicate(uint32 entry) : _entry(entry) {}
        bool operator()(uint64 guid) { return GUID_ENPART(guid) == _entry; }

    private:
        uint32 _entry;
};

class DummyEntryCheckPredicate
{
    public:
        bool operator()(uint64) { return true; }
};

struct ScriptedAI : public CreatureAI
{
    explicit ScriptedAI(Creature* creature);
    virtual ~ScriptedAI() {}

    // *************
    //CreatureAI Functions
    // *************

    void AttackStartNoMove(Unit* target);

    // Called at any Damage from any attacker (before damage apply)
    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/) {}

    //Called at World update tick
    virtual void UpdateAI(uint32 const p_Diff);
    void UpdateOperations(uint32 const p_Diff);

    //Called at creature death
    void JustDied(Unit* /*killer*/) {}

    //Called at creature killing another unit
    void KilledUnit(Unit* /*victim*/) {}

    // Called when the creature summon successfully other creature
    void JustSummoned(Creature* /*summon*/) {}

    // Called when a summoned creature is despawned
    void SummonedCreatureDespawn(Creature* /*summon*/) {}

    // Called when hit by a spell
    void SpellHit(Unit* /*caster*/, SpellInfo const* /*spell*/) {}

    // Called when spell hits a target
    void SpellHitTarget(Unit* /*target*/, SpellInfo const* /*spell*/) {}

    //Called at waypoint reached or PointMovement end
    void MovementInform(uint32 /*type*/, uint32 /*id*/) {}

    // Called when AI is temporarily replaced or put back when possess is applied or removed
    void OnPossess(bool /*apply*/) {}

    // Called at any threat added from any attacker (before threat apply)
    void OnAddThreat(Unit* /*victim*/, float& /*fThreat*/, SpellSchoolMask /*schoolMask*/, SpellInfo const* /*threatSpell*/) {}

    // *************
    // Variables
    // *************

    //Pointer to creature we are manipulating
    Creature* me;

    //For fleeing
    bool IsFleeing;

    SummonList summons;
    EventMap events;

    // *************
    //Pure virtual functions
    // *************

    //Called at creature reset either by death or evade
    void Reset() {}

    //Called at creature aggro either by MoveInLOS or Attack Start
    void EnterCombat(Unit* /*victim*/) {}

    // *************
    //AI Helper Functions
    // *************

    //Start movement toward victim
    void DoStartMovement(Unit* target, float distance = 0.0f, float angle = 0.0f);

    //Start no movement on victim
    void DoStartNoMovement(Unit* target);

    //Stop attack of current victim
    void DoStopAttack();

    //Cast spell by spell info
    void DoCastSpell(Unit* target, SpellInfo const* spellInfo, bool triggered = false);

    //Plays a sound to all nearby players
    void DoPlaySoundToSet(WorldObject* source, uint32 soundId);

    //Drops all threat to 0%. Does not remove players from the threat list
    void DoResetThreat();

    float DoGetThreat(Unit* unit);
    void DoModifyThreatPercent(Unit* unit, int32 pct);

    void DoTeleportTo(float x, float y, float z, uint32 time = 0);
    void DoTeleportTo(float const pos[4]);

    //Teleports a player without dropping threat (only teleports to same map)
    void DoTeleportPlayer(Unit* unit, float x, float y, float z, float o);
    void DoTeleportAll(float x, float y, float z, float o);

    //Returns friendly unit with the most amount of hp missing from max hp
    Unit* DoSelectLowestHpFriendly(float range, uint32 minHPDiff = 1);

    void DespawnCreaturesInArea(uint32 entry, WorldObject* object);

    void DespawnGameObjectsInArea(uint32 entry, WorldObject* object);

    void ApplyAllImmunities(bool apply);

    //Returns a list of friendly CC'd units within range
    std::list<Creature*> DoFindFriendlyCC(float range);

    //Returns a list of all friendly units missing a specific buff within range
    std::list<Creature*> DoFindFriendlyMissingBuff(float range, uint32 spellId);

    //Return a player with at least minimumRange from me
    Player* GetPlayerAtMinimumRange(float minRange);

    //Spawns a creature relative to me
    Creature* DoSpawnCreature(uint32 entry, float offsetX, float offsetY, float offsetZ, float angle, uint32 type, uint32 despawntime);

    bool HealthBelowPct(uint32 pct) const { return me->HealthBelowPct(pct); }
    bool HealthAbovePct(uint32 pct) const { return me->HealthAbovePct(pct); }

    //Returns spells that meet the specified criteria from the creatures spell list
    SpellInfo const* SelectSpell(Unit* target, uint32 school, uint32 mechanic, SelectTargetType targets, uint32 powerCostMin, uint32 powerCostMax, float rangeMin, float rangeMax, SelectEffect effect);

    void SetEquipmentSlots(bool loadDefault, int32 mainHand = EQUIP_NO_CHANGE, int32 offHand = EQUIP_NO_CHANGE, int32 ranged = EQUIP_NO_CHANGE);

    //Generally used to control if MoveChase() is to be used or not in AttackStart(). Some creatures does not chase victims
    void SetCombatMovement(bool allowMovement);
    bool IsCombatMovementAllowed() const { return _isCombatMovementAllowed; }

    bool EnterEvadeIfOutOfCombatArea(uint32 const diff);

    /// Add timed delayed operation
    /// @p_Timeout  : Delay time
    /// @p_Function : Callback function
    void AddTimedDelayedOperation(uint32 p_Timeout, std::function<void()> && p_Function)
    {
        m_EmptyWarned = false;
        m_TimedDelayedOperations.push_back(std::pair<uint32, std::function<void()>>(p_Timeout, p_Function));
    }

    /// Called after last delayed operation was deleted
    /// Do whatever you want
    virtual void LastOperationCalled() { }

    std::vector<std::pair<int32, std::function<void()>>>    m_TimedDelayedOperations;   ///< Delayed operations
    bool                                                    m_EmptyWarned;              ///< Warning when there are no more delayed operations

    // return true for heroic mode. i.e.
    //   - for dungeon in mode 10-heroic,
    //   - for raid in mode 10-Heroic
    //   - for raid in mode 25-heroic
    // DO NOT USE to check raid in mode 25-normal.
    bool IsHeroic() const { return _isHeroic; }
    bool IsLFR() const { return _difficulty == RAID_TOOL_DIFFICULTY; }

    // return the dungeon or raid difficulty
    Difficulty GetDifficulty() const { return _difficulty; }

    // return true for 25 man or 25 man heroic mode
    bool Is25ManRaid() const { return (_difficulty == MAN25_DIFFICULTY || _difficulty == MAN25_HEROIC_DIFFICULTY); }

    template<class T> inline
    const T& DUNGEON_MODE(const T& normal5, const T& heroic10) const
    {
        switch (_difficulty)
        {
            case REGULAR_DIFFICULTY:
                return normal5;
            case HEROIC_DIFFICULTY:
                return heroic10;
            default:
                break;
        }

        return heroic10;
    }

    template<class T> inline
    const T& RAID_MODE(const T& normal10, const T& normal25) const
    {
        switch (_difficulty)
        {
            case MAN10_DIFFICULTY:
                return normal10;
            case MAN25_DIFFICULTY:
                return normal25;
            default:
                break;
        }

        return normal25;
    }

    template<class T> inline
    const T& RAID_MODE(const T& normal10, const T& normal25, const T& heroic10, const T& heroic25) const
    {
        switch (_difficulty)
        {
            case MAN10_DIFFICULTY:
                return normal10;
            case MAN25_DIFFICULTY:
                return normal25;
            case MAN10_HEROIC_DIFFICULTY:
                return heroic10;
            case MAN25_HEROIC_DIFFICULTY:
                return heroic25;
            default:
                break;
        }

        return heroic25;
    }

    private:
        Difficulty _difficulty;
        uint32 _evadeCheckCooldown;
        bool _isCombatMovementAllowed;
        bool _isHeroic;
};

struct Scripted_NoMovementAI : public ScriptedAI
{
    Scripted_NoMovementAI(Creature* creature) : ScriptedAI(creature) {}
    virtual ~Scripted_NoMovementAI() {}

    //Called at each attack of me by any victim
    void AttackStart(Unit* target);
};

class BossAI : public ScriptedAI
{
    public:
        BossAI(Creature* creature, uint32 bossId);
        virtual ~BossAI() {}

        InstanceScript* const instance;
        BossBoundaryMap const* GetBoundary() const { return _boundary; }

        void JustSummoned(Creature* summon);
        void SummonedCreatureDespawn(Creature* summon);

        virtual void UpdateAI(uint32 const diff);

        // Hook used to execute events scheduled into EventMap without the need
        // to override UpdateAI
        // note: You must re-schedule the event within this method if the event
        // is supposed to run more than once
        virtual void ExecuteEvent(uint32 const /*eventId*/) { }

        void Reset() { _Reset(); }
        void EnterCombat(Unit* /*who*/) { _EnterCombat(); }
        void JustDied(Unit* /*killer*/) { _JustDied(); }
        void JustReachedHome() { _JustReachedHome(); }

    protected:
        void _Reset();
        void _EnterCombat();
        void _JustDied();
        void _JustReachedHome() { me->setActive(false); }

        bool CheckInRoom()
        {
            if (CheckBoundary(me))
                return true;

            EnterEvadeMode();
            return false;
        }

        bool CheckInArea(const uint32 diff, uint32 areaId)
        {
            if (_checkareaTimer <= diff)
                _checkareaTimer = 3000;
            else
            {
                _checkareaTimer -= diff;
                return true;
            }

            if (me->GetAreaId() != areaId)
            {
                EnterEvadeMode();
                return false;
            }

            return true;
        }

        bool CheckBoundary(Unit* who);
        void TeleportCheaters();

        void ProcessCompletedEncounter();

        EventMap events;
        SummonList summons;

    private:
        BossBoundaryMap const* const _boundary;
        uint32 const _bossId;
        uint32 _checkareaTimer;
        bool m_IsCompletedEncounterProcessed;
};

class WorldBossAI : public ScriptedAI
{
    public:
        WorldBossAI(Creature* creature);
        virtual ~WorldBossAI() {}

        void JustSummoned(Creature* summon);
        void SummonedCreatureDespawn(Creature* summon);

        virtual void UpdateAI(uint32 const diff);

        // Hook used to execute events scheduled into EventMap without the need
        // to override UpdateAI
        // note: You must re-schedule the event within this method if the event
        // is supposed to run more than once
        virtual void ExecuteEvent(uint32 const /*eventId*/) { }

        void Reset() { _Reset(); }
        void EnterCombat(Unit* /*who*/) { _EnterCombat(); }
        void JustDied(Unit* /*killer*/) { _JustDied(); }

    protected:
        void _Reset();
        void _EnterCombat();
        void _JustDied();

        EventMap events;
        SummonList summons;
};

class SceneHelper
{
    private:

        enum ActionTypes
        {
            ACTION_TALK,
            ACTION_MOVE,
            ACTION_SUMMON,
        };

        class SceneAction
        {
            public:

                SceneAction(uint64 actorGuid, Map* actionMap, ActionTypes actionType, uint32 actionTimer) :
                    m_ActorGuid(actorGuid), m_ActionMap(actionMap), m_ActionType(actionType), m_ActionTimer(actionTimer) { }

                ActionTypes GetActionType() const { return m_ActionType; }
                uint32 GetActionTimer() const { return m_ActionTimer; }
                virtual void DoAction() = 0;

            protected:

                Creature* GetActor()
                {
                    return m_ActionMap->GetCreature(m_ActorGuid);
                }

                uint64 GetActorGuid()
                {
                    return m_ActorGuid;
                }

                Map* GetActionMap()
                {
                    return m_ActionMap;
                }

            protected:

                uint64 m_ActorGuid;
                Map* m_ActionMap;
                ActionTypes m_ActionType;
                uint32 m_ActionTimer;
        };

        class SceneActionTalk : public SceneAction
        {
            public:

                SceneActionTalk(uint64 actorGuid, Map* actionMap, uint32 talkIndex, uint32 actionTimer) : 
                    SceneAction(actorGuid, actionMap, ACTION_TALK, actionTimer), m_TalkIndex(talkIndex)
                {
                    
                }

                void DoAction();

        private:

            uint32 m_TalkIndex;

        };

        class SceneActionMove : public SceneAction
        {
            public:

                SceneActionMove(uint64 actorGuid, Map* actionMap, const Position& movePosition, bool isWalk, uint32 actionTimer) : 
                    SceneAction(actorGuid, actionMap, ACTION_MOVE, actionTimer), m_IsWalk(isWalk), m_IsManyPoints(false)
                {
                    m_MovePosition = movePosition;
                }

                SceneActionMove(uint64 actorGuid, Map* actionMap, std::vector<Position>& movePositions, bool isWalk, uint32 actionTimer) : 
                    SceneAction(actorGuid, actionMap, ACTION_MOVE, actionTimer), m_IsWalk(isWalk), m_IsManyPoints(true)
                {
                    m_MovePositions = movePositions;
                }

                void DoAction();

        private:

            Position m_MovePosition;
            std::vector<Position> m_MovePositions;
            bool m_IsWalk;
            bool m_IsManyPoints;

        };

        class SceneActionSummon : public SceneAction
        {
            public:

                SceneActionSummon(uint64 actorGuid, Map* actionMap, uint32 summonEntry, const Position& summonPosition, uint32 actionTimer) : 
                    SceneAction(actorGuid, actionMap, ACTION_MOVE, actionTimer), m_SummonEntry(summonEntry)
                {
                    m_SummonPosition = summonPosition;
                }

                void DoAction();

        private:

            uint32 m_SummonEntry;
            Position m_SummonPosition;
        };

        class SceneActionCast : public SceneAction
        {
            public:

                SceneActionCast(uint64 actorGuid, Map* actionMap, uint32 spellEntry, uint32 actionTimer) : 
                    SceneAction(actorGuid, actionMap, ACTION_MOVE, actionTimer), m_SpellEntry(spellEntry)
                {

                }

                void DoAction();

        private:

            uint32 m_SpellEntry;
        };

    public:

        SceneHelper(uint64 actorGuid, Map* actionMap) : m_DefaultActorGuid(actorGuid), m_ActionMap(actionMap), 
            m_IsActive(false), m_SceneTimer(1000)
        {
        }

        SceneHelper(Map* actionMap) : m_DefaultActorGuid(0), m_ActionMap(actionMap), 
            m_IsActive(false), m_SceneTimer(1000)
        {
        }

        ~SceneHelper()
        {
            Clear();
        }

        void SetDefaultActorGuid(uint64 actorGuid) { m_DefaultActorGuid = actorGuid; }

        bool IsActive() const { return m_IsActive; }
        bool IsStopped() const { return !m_IsActive; }
        bool IsEmpty() const { return m_SceneActions.empty(); }

        void Activate() 
        {
            if (IsEmpty())
                return;

            m_SceneTimer = GetCurrentAction()->GetActionTimer();

            m_IsActive = true; 
        }
        void Stop() { m_IsActive = false; }

        void AddSceneActionTalk(uint32 talkIndex, uint32 actionTimer = 1000)
        {
            m_SceneActions.push_back(new SceneActionTalk(m_DefaultActorGuid, m_ActionMap, talkIndex, actionTimer));
        }

        void AddSceneActionMove(const Position& movePosition, bool isWalk, uint32 actionTimer = 1000)
        {
            m_SceneActions.push_back(new SceneActionMove(m_DefaultActorGuid, m_ActionMap, movePosition, isWalk, actionTimer));
        }

        void AddSceneActionMove(std::vector<Position>& positions, bool isWalk, uint32 actionTimer = 1000)
        {
            m_SceneActions.push_back(new SceneActionMove(m_DefaultActorGuid, m_ActionMap, positions, isWalk, actionTimer));
        }

        void AddSceneSummonCreature(uint32 summonEntry, const Position& summonPosition, uint32 actionTimer = 1000)
        {
            m_SceneActions.push_back(new SceneActionSummon(m_DefaultActorGuid, m_ActionMap, summonEntry, summonPosition, actionTimer));
        }

        void AddSceneCast(uint32 spellEntry, uint32 actionTimer = 1000)
        {
            m_SceneActions.push_back(new SceneActionCast(m_DefaultActorGuid, m_ActionMap, spellEntry, actionTimer));
        }

        void Clear()
        {
            while (!m_SceneActions.empty())
            {
                delete m_SceneActions.front();
                m_SceneActions.pop_front();
            }
        }

        void Update(const uint32 diff);

    private:

        SceneAction* GetCurrentAction()
        {
            if (m_SceneActions.empty())
                return NULL;

            return m_SceneActions.front();
        }

    private:

        bool m_IsActive;
        uint64 m_DefaultActorGuid;
        Map* m_ActionMap;
        uint32 m_SceneTimer;
        std::list<SceneAction*> m_SceneActions;
};

// SD2 grid searchers.
Creature* GetClosestCreatureWithEntry(WorldObject* source, uint32 entry, float maxSearchRange, bool alive = true);
GameObject* GetClosestGameObjectWithEntry(WorldObject* source, uint32 entry, float maxSearchRange);
void GetCreatureListWithEntryInGrid(std::list<Creature*>& list, WorldObject* source, uint32 entry, float maxSearchRange);
void GetGameObjectListWithEntryInGrid(std::list<GameObject*>& list, WorldObject* source, uint32 entry, float maxSearchRange);
void GetPlayerListInGrid(std::list<Player*>& list, WorldObject* source, float maxSearchRange);

void GetPositionWithDistInOrientation(Unit* pUnit, float dist, float orientation, float& x, float& y);
void GetRandPosFromCenterInDist(float centerX, float centerY, float dist, float& x, float& y);

#endif // SCRIPTEDCREATURE_H_

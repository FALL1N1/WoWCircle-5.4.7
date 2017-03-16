/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef TRINITY_INSTANCE_DATA_H
#define TRINITY_INSTANCE_DATA_H

#include "ZoneScript.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Group.h"
#include "Guild.h"
//#include "GameObject.h"
//#include "Map.h"

#define OUT_SAVE_INST_DATA             sLog->outDebug(LOG_FILTER_TSCR, "Saving Instance Data for Instance %s (Map %d, Instance Id %d)", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_SAVE_INST_DATA_COMPLETE    sLog->outDebug(LOG_FILTER_TSCR, "Saving Instance Data for Instance %s (Map %d, Instance Id %d) completed.", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA(a)          sLog->outDebug(LOG_FILTER_TSCR, "Loading Instance Data for Instance %s (Map %d, Instance Id %d). Input is '%s'", instance->GetMapName(), instance->GetId(), instance->GetInstanceId(), a)
#define OUT_LOAD_INST_DATA_COMPLETE    sLog->outDebug(LOG_FILTER_TSCR, "Instance Data Load for Instance %s (Map %d, Instance Id: %d) is complete.", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())
#define OUT_LOAD_INST_DATA_FAIL        sLog->outError(LOG_FILTER_TSCR, "Unable to load Instance Data for Instance %s (Map %d, Instance Id: %d).", instance->GetMapName(), instance->GetId(), instance->GetInstanceId())

class Map;
class Unit;
class Player;
class GameObject;
class Creature;

typedef std::set<GameObject*> DoorSet;
typedef std::set<Creature*> MinionSet;

enum EncounterFrameType
{
    ENCOUNTER_FRAME_SET_COMBAT_RES_LIMIT    = 0,
    ENCOUNTER_FRAME_RESET_COMBAT_RES_LIMIT  = 1,
    ENCOUNTER_FRAME_ENGAGE                  = 2,
    ENCOUNTER_FRAME_DISENGAGE               = 3,
    ENCOUNTER_FRAME_UPDATE_PRIORITY         = 4,
    ENCOUNTER_FRAME_ADD_TIMER               = 5,
    ENCOUNTER_FRAME_ENABLE_OBJECTIVE        = 6,
    ENCOUNTER_FRAME_UPDATE_OBJECTIVE        = 7,
    ENCOUNTER_FRAME_DISABLE_OBJECTIVE       = 8,
    ENCOUNTER_FRAME_UNK7                    = 9,    // Seems to have something to do with sorting the encounter units
    ENCOUNTER_FRAME_ADD_COMBAT_RES_LIMIT    = 10,
};

enum EncounterState
{
    NOT_STARTED   = 0,
    IN_PROGRESS   = 1,
    FAIL          = 2,
    DONE          = 3,
    SPECIAL       = 4,
    TO_BE_DECIDED = 5,
};

enum DoorType
{
    DOOR_TYPE_ROOM          = 0,    // Door can open if encounter is not in progress
    DOOR_TYPE_PASSAGE       = 1,    // Door can open if encounter is done
    DOOR_TYPE_SPAWN_HOLE    = 2,    // Door can open if encounter is in progress, typically used for spawning places
    MAX_DOOR_TYPES,
};

enum BoundaryType
{
    BOUNDARY_NONE = 0,
    BOUNDARY_N,
    BOUNDARY_S,
    BOUNDARY_E,
    BOUNDARY_W,
    BOUNDARY_NE,
    BOUNDARY_NW,
    BOUNDARY_SE,
    BOUNDARY_SW,
    BOUNDARY_MAX_X = BOUNDARY_N,
    BOUNDARY_MIN_X = BOUNDARY_S,
    BOUNDARY_MAX_Y = BOUNDARY_W,
    BOUNDARY_MIN_Y = BOUNDARY_E,
};

typedef std::map<BoundaryType, float> BossBoundaryMap;

struct DoorData
{
    uint32 entry, bossId;
    DoorType type;
    uint32 boundary;
};

struct MinionData
{
    uint32 entry, bossId;
};

struct BossInfo
{
    BossInfo() : state(TO_BE_DECIDED) {}
    EncounterState state;
    DoorSet door[MAX_DOOR_TYPES];
    MinionSet minion;
    BossBoundaryMap boundary;
};

struct DoorInfo
{
    explicit DoorInfo(BossInfo* _bossInfo, DoorType _type, BoundaryType _boundary)
        : bossInfo(_bossInfo), type(_type), boundary(_boundary) {}
    BossInfo* bossInfo;
    DoorType type;
    BoundaryType boundary;
};

struct MinionInfo
{
    explicit MinionInfo(BossInfo* _bossInfo) : bossInfo(_bossInfo) {}
    BossInfo* bossInfo;
};

typedef std::multimap<uint32 /*entry*/, DoorInfo> DoorInfoMap;
typedef std::map<uint32 /*entry*/, MinionInfo> MinionInfoMap;

struct BossScenarios
{
    BossScenarios()
    {
        m_BossID = 0;
        m_ScenarioID = 0;
    }

    BossScenarios(uint32 p_ID, uint32 p_Scenario)
    {
        m_BossID = p_ID;
        m_ScenarioID = p_Scenario;
    }

    uint32 m_BossID;
    uint32 m_ScenarioID;
};

#define GOB_CHALLENGE_START_DOOR 211989

enum eChallengeMedals
{
    MedalTypeNone,
    MedalTypeBronze,
    MedalTypeSilver,
    MedalTypeGold,
    MaxMedalType
};

enum eInstanceSpells
{
    SpellDetermination          = 139068,
    /// Heroism, Bloodlust...
    ShamanSated                 = 57724,
    HunterInsanity              = 95809,
    MageTemporalDisplacement    = 80354,
    ShamanExhaustion            = 57723,
    /// Battle resurrection spells
    Rebirth                     = 20484,
    Soulstone                   = 20707,
    RaiseAlly                   = 61999,
    EternalGuardian             = 126393,
    GiftOfChiJi                 = 159931,
    DustOfLife                  = 159956,
    MaxBattleResSpells          = 6
};

uint32 const g_BattleResSpells[eInstanceSpells::MaxBattleResSpells] =
{
    eInstanceSpells::Rebirth,
    eInstanceSpells::Soulstone,
    eInstanceSpells::RaiseAlly,
    eInstanceSpells::EternalGuardian,
    eInstanceSpells::GiftOfChiJi,
    eInstanceSpells::DustOfLife
};



class InstanceScript : public ZoneScript
{
    public:
        explicit InstanceScript(Map* p_Map);

        virtual ~InstanceScript() {}

        Map* instance;

        /// On creation, NOT load.
        virtual void Initialize() {}

        /// On load
        virtual void Load(char const* data) { LoadBossState(data); }

        /// When save is needed, this function generates the data
        virtual std::string GetSaveData() { return GetBossSaveData(); }

        void SaveToDB();

        virtual void Update(uint32 diff) { UpdateOperations(diff); }
        void UpdateOperations(uint32 const diff);

        /// Used by the map's CanEnter function.
        /// This is to prevent players from entering during boss encounters.
        virtual bool IsEncounterInProgress() const;

        /// Called when a player begins to enter the instance.
        virtual void BeforePlayerEnter(Player* /*player*/) {}

        /// Called when a player successfully enters the instance.
        virtual void OnPlayerEnter(Player* p_Player);

        /// Called when a player successfully exits the instance
        virtual void OnPlayerExit(Player* p_Player);

        /// Handle open / close objects
        /// use HandleGameObject(0, boolen, GO); in OnObjectCreate in instance scripts
        /// use HandleGameObject(GUID, boolen, NULL); in any other script
        void HandleGameObject(uint64 guid, bool open, GameObject* go = NULL);

        /// change active state of doors or buttons
        void DoUseDoorOrButton(uint64 guid, uint32 withRestoreTime = 0, bool useAlternativeState = false);

        /// Respawns a GO having negative spawntimesecs in gameobject-table
        void DoRespawnGameObject(uint64 guid, uint32 timeToDespawn = MINUTE);

        /// sends world state update to all players in instance
        void DoUpdateWorldState(uint32 worldstateId, uint32 worldstateValue);

        /// Send Notify to all players in instance
        void DoSendNotifyToInstance(char const* format, ...);

        /// Reset Achievement Criteria
        void DoResetAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 = 0, uint64 miscValue2 = 0, bool evenIfCriteriaComplete = false);

        /// Complete Achievement for all players in instance
	    void DoCompleteAchievement(uint32 achievement);

        /// Update Achievement Criteria for all players in instance
        void DoUpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 = 0, uint32 miscValue2 = 0, uint32 miscValue3 = 0, Unit* unit = NULL);

        /// Start/Stop Timed Achievement Criteria for all players in instance
        void DoStartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);
        void DoStopTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);

        /// Remove Auras due to Spell on all players in instance
        void DoRemoveAurasDueToSpellOnPlayers(uint32 spell);

        /// Remove aura from stack on all players in instance
        void DoRemoveAuraFromStackOnPlayers(uint32 spell, uint64 casterGUID = 0, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT, uint32 num = 1);

        /// Cast spell on all players in instance
        void DoCastSpellOnPlayers(uint32 spell);

        /// Add aura on all players in instance
        void DoAddAuraOnPlayers(uint32 spell);

        /// Remove cooldown for spell on all players in instance
        void DoRemoveSpellCooldownOnPlayers(uint32 p_SpellID);

        void DoSetAlternatePowerOnPlayers(int32 value);

        void DoModifyPlayerCurrencies(uint32 id, int32 value);

        void DoNearTeleportPlayers(const Position pos, bool casting = false);

        void DoStartMovie(uint32 movieId);

        void DoKilledMonsterKredit(uint32 questId, uint32 entry, uint64 guid = 0);

        /// Return wether server allow two side groups or not
        bool ServerAllowsTwoSideGroups() { return sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP); }

        virtual bool SetBossState(uint32 id, EncounterState state);
        EncounterState GetBossState(uint32 id) const { return id < m_Bosses.size() ? m_Bosses[id].state : TO_BE_DECIDED; }
        BossBoundaryMap const* GetBossBoundary(uint32 id) const { return id < m_Bosses.size() ? &m_Bosses[id].boundary : NULL; }

        /// Achievement criteria additional requirements check
        /// NOTE: not use this if same can be checked existed requirement types from AchievementCriteriaRequirementType
        virtual bool CheckAchievementCriteriaMeet(uint32 /*criteria_id*/, Player const* /*source*/, Unit const* /*target*/ = NULL, uint32 /*miscvalue1*/ = 0);

        /// Checks boss requirements (one boss required to kill other)
        virtual bool CheckRequiredBosses(uint32 /*bossId*/, Player const* /*player*/ = NULL) const;

        /// Checks encounter state at kill/spellcast
        void UpdateEncounterState(EncounterCreditType type, uint32 creditEntry, Unit* source);

        /// Used only during loading
        void SetCompletedEncountersMask(uint32 newMask) { completedEncounters = newMask; }

        /// Returns completed encounters mask for packets
        uint32 GetCompletedEncounterMask() const { return completedEncounters; }

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

        struct CriteriaProgressData
        {
            CriteriaProgressData(uint32 p_ID, uint64 p_Quantity, uint64 p_Guid, uint32 p_Date, uint32 p_StartTime, uint8 p_Flags)
            {
                m_ID                = p_ID;
                m_Quantity          = p_Quantity;
                m_Guid              = p_Guid;
                m_Date              = p_Date;
                m_TimeFromStart     = p_StartTime;
                m_TimeFromCreate    = p_StartTime;
                m_Flags             = p_Flags;
            }

            CriteriaProgressData()
            {
                m_ID                = 0;
                m_Quantity          = 0;
                m_Guid              = 0;
                m_Date              = 0;
                m_TimeFromStart     = 0;
                m_TimeFromCreate    = 0;
                m_Flags             = 0;
            }

            uint32 m_ID;
            uint64 m_Quantity;
            uint64 m_Guid;
            uint32 m_Date;
            uint32 m_TimeFromStart;
            uint32 m_TimeFromCreate;
            uint8  m_Flags;
        };

        struct ScenarioData
        {
            ScenarioData(uint32 p_ScenarioID, uint32 p_StepID, uint32 p_CurrWave, uint32 p_MaxWave, uint32 p_Timer, uint32 p_CriteriaCount,
                bool p_Complete)
            {
                m_ScenarioID        = p_ScenarioID;
                m_StepID            = p_StepID;
                m_WaveCurrent       = p_CurrWave;
                m_WaveMax           = p_MaxWave;
                m_TimerDuration     = p_Timer;
                m_CriteriaCount     = p_CriteriaCount;
                m_ScenarioComplete  = p_Complete;

                m_CriteriaProgress.resize(m_CriteriaCount);
            }

            ScenarioData(uint32 p_ScenarioID, uint32 p_StepID)
            {
                m_ScenarioID        = p_ScenarioID;
                m_StepID            = p_StepID;
                m_WaveCurrent       = 0;
                m_WaveMax           = 0;
                m_TimerDuration     = 0;
                m_CriteriaCount     = 0;
                m_ScenarioComplete  = false;

                m_CriteriaProgress.clear();
            }

            ScenarioData()
            {
                m_ScenarioID        = 0;
                m_StepID            = 0;
                m_WaveCurrent       = 0;
                m_WaveMax           = 0;
                m_TimerDuration     = 0;
                m_CriteriaCount     = 0;
                m_ScenarioComplete  = false;

                m_CriteriaProgress.clear();
            }

            void AddCriteriaProgress(CriteriaProgressData p_Data)
            {
                m_CriteriaProgress.push_back(p_Data);
            }

            uint32 m_ScenarioID;
            uint32 m_StepID;
            uint32 m_WaveCurrent;
            uint32 m_WaveMax;
            uint32 m_TimerDuration;
            uint32 m_CriteriaCount;
            bool m_ScenarioComplete;

            std::vector<CriteriaProgressData> m_CriteriaProgress;
        };

        void SendScenarioState(ScenarioData p_Data, Player* p_Player = nullptr);
        void SendScenarioProgressUpdate(CriteriaProgressData p_Data, Player* p_Player = nullptr);

        //////////////////////////////////////////////////////////////////////////
        /// ChallengesMode
        void SendChallengeStartTimer(uint32 p_Time);
        void SendChallengeStartElapsedTimer(uint32 p_TimerID, uint32 p_Time);
        void SendChallengeStopElapsedTimer(uint32 p_TimerID, bool p_KeepTimer = false);
        void SetChallengeModeStarted();
        bool IsChallengeModeStarted() const { return m_ChallengeStarted; }
        void ScheduleChallengeStartup(uint32 p_Diff);
        void ScheduleChallengeTimeUpdate(uint32 p_Diff);
        void ScheduleBeginningTimeUpdate(uint32 p_Diff);
        void SendChallengeNewPlayerRecord();
        void SendChallengeModeComplete(uint32 p_Money, uint32 p_Valor);
        void SaveChallengeDatasIfNeeded();
        void SaveNewGroupChallenge(uint32 p_GuildID = 0);
        void RewardChallengers(uint32& p_Money, uint32& p_Valor);
        void RewardNewRealmRecord(RealmCompletedChallenge* p_OldChallenge = nullptr);

        bool   m_ChallengeStarted;
        bool   m_ConditionCompleted;
        uint32 m_StartChallengeTime;
        uint64 m_ChallengeDoorGuid;
        uint32 m_ChallengeTime;
        uint8  m_MedalType;
        uint64 m_InstanceGuid;
        uint32 m_BeginningTime;
        uint32 m_ScenarioID;
        uint8  m_ScenarioStep;
        //////////////////////////////////////////////////////////////////////////

        /// Called when a creature is killed by a player
        virtual void OnCreatureKilled(Creature* p_Creature, Player* p_Player) { }

        void SendEncounterUnit(uint32 type, Unit* unit = NULL, uint8 param1 = 0, uint8 param2 = 0);

        /// Check if all players are dead (except gamemasters)
        virtual bool IsWipe();

        virtual void FillInitialWorldStates(WorldPacket& /*data*/) {}

        void UpdatePhasing();

        //////////////////////////////////////////////////////////////////////////
        /// Combat Resurrection - http://wow.gamepedia.com/Resurrect#Combat_resurrection
        void ResetCombatResurrection();
        void StartCombatResurrection();
        bool CanUseCombatResurrection() const;
        void ConsumeCombatResurrectionCharge();

        uint32 m_InCombatResCount;
        //////////////////////////////////////////////////////////////////////////

    protected:
        void SetBossNumber(uint32 p_Number);
        void LoadDoorData(DoorData const* data);
        void LoadScenariosInfos(BossScenarios const* p_Scenarios, uint32 p_ScenarioID);
        void LoadMinionData(MinionData const* data);

        void AddDoor(GameObject* door, bool add);
        void AddMinion(Creature* minion, bool add);

        void UpdateDoorState(GameObject* door);
        void UpdateMinionState(Creature* minion, EncounterState state);

        std::string LoadBossState(char const* data);
        std::string GetBossSaveData();
    private:
        std::vector<BossInfo> m_Bosses;
        std::vector<BossScenarios> m_BossesScenarios;
        DoorInfoMap doors;
        MinionInfoMap minions;
        uint32 completedEncounters; ///< Completed encounter mask, bit indexes are DungeonEncounter.dbc boss numbers, used for packets
        uint32 m_EncounterTime;
};

enum ScenarioSteps
{
    STEP_1      = 0,
    STEP_2      = 1,
    STEP_3      = 2,
    STEP_4      = 3,
    STEP_5      = 4,
    STEP_6      = 5,
    STEP_7      = 6,
    STEP_8      = 7,
    STEP_9      = 8,
    STEP_10     = 9,
    STEP_11     = 10,
    STEP_12     = 11,
    STEP_13     = 12,
    STEP_14     = 13,
    STEP_15     = 14,
};

struct ScenarioStepCriteria
{
    public:

        ScenarioStepCriteria() {}

        ScenarioStepCriteria(uint32 criteriaId, uint32 totalCount) : 
            CriteriaId(criteriaId), CurrentCount(0), TotalCount(totalCount) {}

        bool IsCompleted() const { return CurrentCount >= TotalCount; }

        uint32 CriteriaId;
        uint32 CurrentCount;
        uint32 TotalCount;
};

typedef std::map<uint32 /*CriteriaId*/, ScenarioStepCriteria> ScenarioCriteriaMap;

struct ScenarioStep
{
    public:

        bool AddCriteria(uint32 criteriaId, uint32 totalCount);

        bool UpdateCriteria(uint32 criteriaId, uint32 count = 1);

        uint32 GetCriteriaCount(uint32 criteriaId);

        bool IsCompleted() const;

    private:

        uint32 m_Step;
        ScenarioCriteriaMap m_Criterias;
};

typedef std::map<uint32 /*step*/, ScenarioStep> ScenarioStepMap;

class ScenarioController
{
    public:

        explicit ScenarioController(Map* map, uint32 scenarioId, uint32 maxStep);

        ScenarioStep& GetStep(uint32 step);

        void UpdateCurrentStepCriteria(uint32 criteriaId, uint32 count = 1);
        uint32 GetCurrentStepCriteriaCount(uint32 criteriaId);

        void SetCurrentStep(uint32 step) { m_CurrentStep = step; }
        uint32 GetCurrentStep() const { return m_CurrentStep; }
        bool IsCurrentStep(uint32 step) const { return m_CurrentStep == step; }

        uint32 GetScenarioId() const { return m_ScenarioId; }
        uint32 GetMaxStep() const { return m_MaxStep; }

        bool IsCompleted();

        void SendScenarioProgressToAll(uint32 criteriaId);
        void SendScenarioProgressToAll(uint32 criteriaId, uint32 count);
        void SendScenarioState(Player* player);
        void SendScenarioStateToAll();
        void SendScenarioPOI(uint32 criteriaTreeId, Player* player);
        void SendScenarioPOI(std::list<uint32>& criteriaTrees, Player* player);

    private:

        Map* m_Map;

        uint32 m_ScenarioId;

        uint32 m_CurrentStep;
        uint32 m_MaxStep;
        ScenarioStepMap m_Steps; 
};

#endif

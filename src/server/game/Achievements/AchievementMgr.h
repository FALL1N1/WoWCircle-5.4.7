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
#ifndef __TRINITY_ACHIEVEMENTMGR_H
#define __TRINITY_ACHIEVEMENTMGR_H

#include <map>
#include <string>

#include "Common.h"
#include <ace/Singleton.h>
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "DBCStores.h"
#include <LockedMap.h>
#include "MapUpdater.h"

typedef std::list<CriteriaEntry const*> CriteriaEntryList;
typedef std::list<CriteriaTreeEntry const*> CriteriaTreeEntryList;
typedef std::list<AchievementEntry const*>         AchievementEntryList;

typedef ACE_Based::LockedMap<uint32, CriteriaTreeEntryList> AchievementCriteriaListByAchievement;
typedef ACE_Based::LockedMap<uint32, AchievementEntryList>         AchievementListByReferencedId;

struct CriteriaProgress
{
    uint64 counter;
    uint64 CompletedGUID;                                   // GUID of the player that completed this criteria (guild achievements)
    time_t date;                                            // latest update time.
    bool changed;
};

enum AchievementCriteriaDataType
{                                                           // value1         value2        comment
    ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE                = 0, // 0              0
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE          = 1, // creature_id    0
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE = 2, // class_id       race_id
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH= 3, // health_percent 0
    ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA              = 5, // spell_id       effect_idx
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA              = 7, // spell_id       effect_idx
    ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE               = 8, // minvalue                     value provided with achievement update must be not less that limit
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL             = 9, // minlevel                     minlevel of target
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER            = 10, // gender                       0=male; 1=female
    ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT              = 11, // scripted requirement
    // REUSE
    ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT    = 13, // count                        "with less than %u people in the zone"
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM              = 14, // team                         HORDE(67), ALLIANCE(469)
    ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK             = 15, // drunken_state  0             (enum DrunkenState) of player
    ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY             = 16, // holiday_id     0             event in holiday time
    ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE  = 17, // min_score      max_score     player's team win bg and opposition team have team score in range
    ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT          = 18, // 0              0             maker instance script call for check current criteria requirements fit
    ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM      = 19, // item_level     item_quality  for equipped item in slot to check item level and quality
    ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE = 21, // class_id       race_id
};

#define MAX_ACHIEVEMENT_CRITERIA_DATA_TYPE               22 // maximum value in AchievementCriteriaDataType enum
class Player;
class Unit;
class WorldPacket;

struct AchievementCriteriaData
{
    AchievementCriteriaDataType dataType;
    union
    {
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE              = 0 (no data)
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE        = 1
        struct
        {
            uint32 id;
        } creature;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE = 2
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE = 21
        struct
        {
            uint32 class_id;
            uint32 race_id;
        } classRace;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH = 3
        struct
        {
            uint32 percent;
        } health;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA            = 5
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA            = 7
        struct
        {
            uint32 spell_id;
            uint32 effect_idx;
        } aura;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE             = 8
        struct
        {
            uint32 minvalue;
        } value;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL           = 9
        struct
        {
            uint32 minlevel;
        } level;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER          = 10
        struct
        {
            uint32 gender;
        } gender;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT            = 11 (no data)
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT  = 13
        struct
        {
            uint32 maxcount;
        } map_players;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM            = 14
        struct
        {
            uint32 team;
        } team;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK           = 15
        struct
        {
            uint32 state;
        } drunk;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY           = 16
        struct
        {
            uint32 id;
        } holiday;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE= 17
        struct
        {
            uint32 min_score;
            uint32 max_score;
        } bg_loss_team_score;
        // ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT        = 18 (no data)
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM    = 19
        struct
        {
            uint32 item_level;
            uint32 item_quality;
        } equipped_item;
        // raw
        struct
        {
            uint32 value1;
            uint32 value2;
        } raw;
    };
    uint32 ScriptId;

    AchievementCriteriaData() : dataType(ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE)
    {
        raw.value1 = 0;
        raw.value2 = 0;
        ScriptId = 0;
    }

    AchievementCriteriaData(uint32 _dataType, uint32 _value1, uint32 _value2, uint32 _scriptId) : dataType(AchievementCriteriaDataType(_dataType))
    {
        raw.value1 = _value1;
        raw.value2 = _value2;
        ScriptId = _scriptId;
    }

    bool IsValid(CriteriaEntry const* criteria);
    bool Meets(uint32 criteria_id, Player const* source, Unit const* target, uint32 miscvalue1 = 0) const;
};

struct AchievementCriteriaDataSet
{
        AchievementCriteriaDataSet() : criteria_id(0) {}
        typedef std::vector<AchievementCriteriaData> Storage;
        void Add(AchievementCriteriaData const& data) { storage.push_back(data); }
        bool Meets(Player const* source, Unit const* target, uint32 miscvalue = 0) const;
        void SetCriteriaId(uint32 id) {criteria_id = id;}
    private:
        uint32 criteria_id;
        Storage storage;
};

typedef ACE_Based::LockedMap<uint32, AchievementCriteriaDataSet> AchievementCriteriaDataMap;

struct AchievementReward
{
    uint32 titleId[2];
    uint32 itemId;
    uint32 sender;
    std::string subject;
    std::string text;
};

typedef ACE_Based::LockedMap<uint32, AchievementReward> AchievementRewards;

struct AchievementRewardLocale
{
    StringVector subject;
    StringVector text;
};

typedef ACE_Based::LockedMap<uint32, AchievementRewardLocale> AchievementRewardLocales;

struct CompletedAchievementData
{
    time_t date;
    std::set<uint64> guids;
    uint64 first_guid;
    bool completedByThisCharacter;
    bool changed;
};

typedef ACE_Based::LockedMap<uint32, CriteriaProgress> CriteriaProgressMap;
typedef ACE_Based::LockedMap<uint32, CompletedAchievementData> CompletedAchievementMap;

template<class T>
class AchievementMgr
{
    public:
        AchievementMgr(T* owner);
        ~AchievementMgr();

        void Reset();
        static void DeleteFromDB(uint32 lowguid, uint32 accountId = 0);
        void LoadFromDB(PreparedQueryResult achievementResult, PreparedQueryResult criteriaResult, PreparedQueryResult achievementAccountResult = NULL, PreparedQueryResult criteriaAccountResult = NULL);
        void LoadFromDB2(PreparedQueryResult criteriaResult, PreparedQueryResult criteriaAccountResult = NULL);
        void LoadAfterInterRealm(QueryResult achievementResult, QueryResult criteriaResult);
        void LoadAfterInterRealm2(QueryResult criteriaResult);
        void SaveToDB(SQLTransaction& trans);
        void ResetAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 = 0, uint64 miscValue2 = 0, bool evenIfCriteriaComplete = false);
        void UpdateAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 = 0, uint64 miscValue2 = 0, uint64 miscValue3 = 0, Unit const* unit = NULL, Player* referencePlayer = NULL, bool p_LoginCheck = false);
        bool IsCompletedAchievement(AchievementEntry const* entry, Player* referencePlayer = nullptr, uint64* completedCriterias = nullptr);
        void CompletedAchievement(AchievementEntry const* entry, Player* referencePlayer, bool p_LoginCheck = false);
        void CheckAllAchievementCriteria(Player* referencePlayer, bool fix);
        void SendAllAchievementData(Player* receiver);
        void SendAchievementInfo(Player* receiver, uint32 achievementId = 0);
        bool HasAchieved(uint32 achievementId) const;
        bool HasAccountAchieved(uint32 achievementId) const;
        uint64 GetFirstAchievedCharacterOnAccount(uint32 achievementId) const;
        T* GetOwner() const { return _owner; }

        void UpdateTimedAchievements(uint32 timeDiff);
        void StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry, uint32 timeLost = 0);
        void RemoveTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);   // used for quest and scripted timed achievements
        uint32 GetAchievementPoints() const { return _achievementPoints; }
        time_t GetAchievementCompletedDate(AchievementEntry const* p_Entry) const;

        CompletedAchievementMap const& GetCompletedAchivements() const { return m_completedAchievements; }
        ACE_Thread_Mutex& GetCompletedAchievementLock() { return m_CompletedAchievementsLock; }

        CompletedAchievementMap& GetCompletedAchievements() { return m_completedAchievements; }
        CriteriaProgressMap& GetCriteriaProgress() { return m_criteriaProgress; }

    private:
        enum ProgressType { PROGRESS_SET, PROGRESS_ACCUMULATE, PROGRESS_HIGHEST };
        void SendAchievementEarned(AchievementEntry const* achievement) const;
        void SendCriteriaUpdate(CriteriaEntry const* entry, CriteriaProgress const* progress, uint32 timeElapsed, bool timedCompleted) const;
        CriteriaProgressMap* GetCriteriaProgressMap();
        CriteriaProgress* GetCriteriaProgress(uint32 entry);
        void SetCriteriaProgress(CriteriaTreeEntry const* tree, uint64 changeValue, Player* referencePlayer, ProgressType ptype = PROGRESS_SET);
        void RemoveCriteriaProgress(CriteriaTreeEntry const* entry);
        void CompletedCriteriaFor(AchievementEntry const* achievement, Player* referencePlayer, bool p_LoginCheck = false);
        bool CanCompleteCriteria(AchievementEntry const* achievement);
        bool IsCompletedCriteria(CriteriaTreeEntry const* tree, AchievementEntry const* achievement);
        bool CanUpdateCriteria(CriteriaTreeEntry const* tree, AchievementEntry const* achievement, uint64 miscValue1, uint64 miscValue2, uint64 miscValue3, Unit const* unit, Player* referencePlayer);
        void SendPacket(WorldPacket* data) const;

        bool ConditionsSatisfied(CriteriaEntry const *criteria, Player* referencePlayer) const;
        bool RequirementsSatisfied(CriteriaTreeEntry const* tree, uint64 miscValue1, uint64 miscValue2, uint64 miscValue3, Unit const* unit, Player* referencePlayer) const;
        bool AdditionalRequirementsSatisfied(CriteriaTreeEntry const* tree, uint64 miscValue1, uint64 miscValue2, Unit const* unit, Player* referencePlayer) const;

        bool CheckForOtherFactionCriteria(CriteriaTreeEntry const* tree);

        T* _owner;
        CriteriaProgressMap m_criteriaProgress;
        CompletedAchievementMap m_completedAchievements;
        mutable ACE_Thread_Mutex m_CompletedAchievementsLock;
        typedef ACE_Based::LockedMap<uint32, uint32> TimedAchievementMap;
        TimedAchievementMap m_timedAchievements;      // Criteria id/time left in MS
        uint32 _achievementPoints;
};

struct AchievementCriteriaUpdateTask
{
    uint64 PlayerGUID;
    uint64 UnitGUID;
    std::function<void(uint64, uint64)> Task;
};

using LockedAchievementCriteriaTaskQueue   = ACE_Based::LockedQueue<AchievementCriteriaUpdateTask, ACE_Thread_Mutex>;
using LockedPlayersAchievementCriteriaTask = ACE_Based::LockedMap<uint32, LockedAchievementCriteriaTaskQueue>;

using AchievementCriteriaTaskQueue   = std::queue<AchievementCriteriaUpdateTask>;
using PlayersAchievementCriteriaTask = std::map<uint32, AchievementCriteriaTaskQueue>;

class AchievementGlobalMgr
{
        friend class ACE_Singleton<AchievementGlobalMgr, ACE_Null_Mutex>;
        AchievementGlobalMgr() {}
        ~AchievementGlobalMgr() {}

    public:
        static char const* GetCriteriaTypeString(AchievementCriteriaTypes type);
        static char const* GetCriteriaTypeString(uint32 type);

        CriteriaTreeEntryList const& GetAchievementCriteriaByType(AchievementCriteriaTypes type, bool guild = false) const
        {
            return guild ? m_GuildAchievementCriteriasByType[type] : m_AchievementCriteriasByType[type];
        }

        CriteriaTreeEntryList const& GetTimedAchievementCriteriaByType(AchievementCriteriaTimedTypes type) const
        {
            return m_AchievementCriteriasByTimedType[type];
        }

        CriteriaTreeEntryList const* GetAchievementCriteriaByAchievement(uint32 id) const
        {
            AchievementCriteriaListByAchievement::const_iterator itr = m_AchievementCriteriaListByAchievement.find(id);
            return itr != m_AchievementCriteriaListByAchievement.end() ? &itr->second : NULL;
        }

        AchievementEntryList const* GetAchievementByReferencedId(uint32 id) const
        {
            AchievementListByReferencedId::const_iterator itr = m_AchievementListByReferencedId.find(id);
            return itr != m_AchievementListByReferencedId.end() ? &itr->second : NULL;
        }

        AchievementReward const* GetAchievementReward(AchievementEntry const* achievement) const
        {
            AchievementRewards::const_iterator iter = m_achievementRewards.find(achievement->ID);
            return iter != m_achievementRewards.end() ? &iter->second : NULL;
        }

        AchievementRewardLocale const* GetAchievementRewardLocale(AchievementEntry const* achievement) const
        {
            AchievementRewardLocales::const_iterator iter = m_achievementRewardLocales.find(achievement->ID);
            return iter != m_achievementRewardLocales.end() ? &iter->second : NULL;
        }

        AchievementCriteriaDataSet const* GetCriteriaDataSet(uint32 criteriaId) const
        {
            AchievementCriteriaDataMap::const_iterator iter = m_criteriaDataMap.find(criteriaId);
            return iter != m_criteriaDataMap.end() ? &iter->second : NULL;
        }

        bool IsRealmCompleted(AchievementEntry const* achievement) const
        {
            return m_allCompletedAchievements.find(achievement->ID) != m_allCompletedAchievements.end();
        }

        void SetRealmCompleted(AchievementEntry const* achievement)
        {
            m_allCompletedAchievements.insert(std::make_pair(achievement->ID, true));
        }

        bool IsGroupCriteriaType(AchievementCriteriaTypes type) const
        {
            switch (type)
            {
                case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
                case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
                case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:         // NYI
                case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
                case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:        // NYI
                case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND:  // NYI
                    return true;
                default:
                    break;
            }

            return false;
        }

        void LoadAchievementCriteriaList();
        void LoadAchievementCriteriaData();
        void LoadAchievementReferenceList();
        void LoadCompletedAchievements();
        void LoadRewards();
        void LoadRewardLocales();
        AchievementEntry const* GetAchievement(uint32 achievementId) const;
        AchievementEntry const* GetAchievement(CriteriaTreeEntry const* tree) const;
        std::vector<ModifierTreeEntry const*> const* GetModifiers(uint32 modifierId) const;

        void PrepareCriteriaUpdateTaskThread();

        void AddCriteriaUpdateTask(AchievementCriteriaUpdateTask const& p_Task)
        {
            m_LockedPlayersAchievementCriteriaTask[p_Task.PlayerGUID].add(p_Task);
        }

        PlayersAchievementCriteriaTask const& GetPlayersCriteriaTask() const
        {
            return m_PlayersAchievementCriteriaTask;
        }

        void ClearPlayersCriteriaTask()
        {
            m_PlayersAchievementCriteriaTask.clear();
        }

    private:
        AchievementCriteriaDataMap m_criteriaDataMap;

        // store achievement criterias by type to speed up lookup
        CriteriaTreeEntryList m_AchievementCriteriasByType[ACHIEVEMENT_CRITERIA_TYPE_TOTAL];
        CriteriaTreeEntryList m_GuildAchievementCriteriasByType[ACHIEVEMENT_CRITERIA_TYPE_TOTAL];

        CriteriaTreeEntryList m_AchievementCriteriasByTimedType[ACHIEVEMENT_TIMED_TYPE_MAX];

        // store achievement criterias by achievement to speed up lookup
        AchievementCriteriaListByAchievement m_AchievementCriteriaListByAchievement;

        // store achievements by referenced achievement id to speed up lookup
        AchievementListByReferencedId m_AchievementListByReferencedId;

        typedef ACE_Based::LockedMap<uint32, bool> AllCompletedAchievements;
        AllCompletedAchievements m_allCompletedAchievements;

        AchievementRewards m_achievementRewards;
        AchievementRewardLocales m_achievementRewardLocales;

        LockedPlayersAchievementCriteriaTask m_LockedPlayersAchievementCriteriaTask;  ///< All criteria update task are first storing here
        PlayersAchievementCriteriaTask       m_PlayersAchievementCriteriaTask;        ///< Before thread process, all task stored will be move here
};

#define sAchievementMgr ACE_Singleton<AchievementGlobalMgr, ACE_Null_Mutex>::instance()

class MapUpdater;
class AchievementCriteriaUpdateRequest : public MapUpdaterTask
{
    public:
        AchievementCriteriaUpdateRequest(MapUpdater* p_Updater, AchievementCriteriaTaskQueue p_TaskQueue);
        virtual void call() override;

    private:
        AchievementCriteriaTaskQueue m_CriteriaUpdateTasks;

};

#endif

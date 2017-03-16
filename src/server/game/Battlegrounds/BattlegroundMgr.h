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

#ifndef __BATTLEGROUNDMGR_H
#define __BATTLEGROUNDMGR_H

#include "Common.h"
#include "DBCEnums.h"
#include "Battleground.h"
#include "BattlegroundQueue.h"
#include "Object.h"
#include <ace/Singleton.h>

typedef std::map<uint32, Battleground*> BattlegroundSet;
typedef std::map<BattlegroundTypeId, BattlegroundSet> BattlegroundSetMap;
typedef UNORDERED_MAP<uint32, BattlegroundTypeId> BattleMastersMap;

const extern BattlegroundTypeId battlegroundTypes[26];

#define WS_CURRENCY_RESET_TIME 20001                    // Custom worldstate

struct CreateBattlegroundData
{
    BattlegroundTypeId bgTypeId;
    bool IsArena;
    uint32 MinPlayersPerTeam;
    uint32 MaxPlayersPerTeam;
    uint32 LevelMin;
    uint32 LevelMax;
    std::string BattlegroundName;
    uint32 MapID;
    float Team1StartLocX;
    float Team1StartLocY;
    float Team1StartLocZ;
    float Team1StartLocO;
    float Team2StartLocX;
    float Team2StartLocY;
    float Team2StartLocZ;
    float Team2StartLocO;
    float StartMaxDist;
    uint32 holiday;
    uint32 scriptId;
};

struct QueueSchedulerItem
{
    QueueSchedulerItem(uint32 arenaMMRating,
        uint8 arenaType,
        BattlegroundQueueTypeId bgQueueTypeId,
        BattlegroundTypeId bgTypeId,
        BattlegroundBracketId bracketid,
        uint32 const* disabledBGs)
        : _arenaMMRating (arenaMMRating),
        _arenaType (arenaType),
        _bgQueueTypeId (bgQueueTypeId),
        _bgTypeId (bgTypeId),
        _bracket_id (bracketid)
    {
        if (disabledBGs)
        {
            _disabledBGs.push_back(disabledBGs[0]);
            _disabledBGs.push_back(disabledBGs[1]);
        }
    }

    const uint32 _arenaMMRating;
    const uint8 _arenaType;
    const BattlegroundQueueTypeId _bgQueueTypeId;
    const BattlegroundTypeId _bgTypeId;
    const BattlegroundBracketId _bracket_id;
    std::vector<uint32> _disabledBGs;
};

typedef std::map<uint64, std::string> SpectatorDataStore;
typedef std::map<BattlegroundTypeId, bool /*holiday state*/> BattlegroundHolidayContainer;

class BattlegroundMgr
{
    friend class ACE_Singleton<BattlegroundMgr, ACE_Null_Mutex>;

    private:
        BattlegroundMgr();
        ~BattlegroundMgr();

    public:
        void Update(uint32 diff);

        /* Packet Building */
        void BuildPlayerJoinedBattlegroundPacket(WorldPacket* data, uint64 guid);
        void BuildPlayerLeftBattlegroundPacket(WorldPacket* data, uint64 guid);
        void BuildBattlegroundListPacket(WorldPacket* data, ObjectGuid guid, Player* player, BattlegroundTypeId bgTypeId);
        void BuildStatusFailedPacket(WorldPacket* data, Battleground* bg, Player* player, uint8 QueueSlot, GroupJoinBattlegroundResult result);
        void BuildUpdateWorldStatePacket(WorldPacket* data, uint32 field, uint32 value);
        void BuildPvpLogDataPacket(WorldPacket* data, Battleground* bg);
        void BuildBattlegroundStatusPacket(WorldPacket* data, Battleground* bg, Player * pPlayer, uint8 QueueSlot, uint8 StatusID, uint32 Time1, uint32 Time2, uint8 arenatype, uint8 uiFrame = 1);
        void SendAreaSpiritHealerQueryOpcode(Player* player, Battleground* bg, uint64 guid);
        void SendCheckWargameEntry(Player* p_Requester, Player* p_Target, uint64 p_QueueID);

        /* Battlegrounds */
        Battleground* GetBattlegroundThroughClientInstance(uint32 instanceId, BattlegroundTypeId bgTypeId);
        Battleground* GetBattleground(uint32 InstanceID, BattlegroundTypeId bgTypeId); //there must be uint32 because MAX_BATTLEGROUND_TYPE_ID means unknown
        BattlegroundSet GetBattlegroundsByType(BattlegroundTypeId bgTypeId);
        Battleground* GetBattlegroundTemplate(BattlegroundTypeId bgTypeId);
        Battleground* CreateNewBattleground(BattlegroundTypeId bgTypeId, PvPDifficultyEntry const* bracketEntry, uint8 arenaType, bool isRated, uint32 const* disabledBGs = nullptr, bool isWargame = false);

        uint32 CreateBattleground(CreateBattlegroundData& data);

        void AddBattleground(uint32 InstanceID, BattlegroundTypeId bgTypeId, Battleground* BG)
        {
            m_Battlegrounds[bgTypeId][InstanceID] = BG;
        };

        void RemoveBattleground(uint32 instanceID, BattlegroundTypeId bgTypeId) { m_Battlegrounds[bgTypeId].erase(instanceID); }
        uint32 CreateClientVisibleInstanceId(BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id);

        void CreateInitialBattlegrounds();
        void InitAutomaticArenaPointDistribution();
        void DeleteAllBattlegrounds();

        void SendToBattleground(Player* player, uint32 InstanceID, BattlegroundTypeId bgTypeId);

        /* Battleground queues */
        //these queues are instantiated when creating BattlegroundMrg
        BattlegroundQueue& GetBattlegroundQueue(BattlegroundQueueTypeId bgQueueTypeId) { return m_BattlegroundQueues[bgQueueTypeId]; }
        BattlegroundQueue m_BattlegroundQueues[MAX_BATTLEGROUND_QUEUE_TYPES]; // public, because we need to access them in BG handler code

        std::map<uint32, BGFreeSlotQueueType> BGFreeSlotQueue;

        void ScheduleQueueUpdate(uint32 arenaMatchmakerRating, uint8 arenaType, BattlegroundQueueTypeId bgQueueTypeId, BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id, uint32 const* disabledBGs = nullptr);
        uint32 GetMaxRatingDifference() const;
        uint32 GetRatingDiscardTimer()  const;
        uint32 GetPrematureFinishTime() const;

        void ToggleArenaTesting();
        void ToggleTesting();

        void SetHolidayWeekends(std::list<uint32> activeHolidayId);
        void LoadBattleMastersEntry();
        BattlegroundTypeId GetBattleMasterBG(uint32 entry) const
        {
            BattleMastersMap::const_iterator itr = mBattleMastersMap.find(entry);
            if (itr != mBattleMastersMap.end())
                return itr->second;
            return BATTLEGROUND_WS;
        }

        void InitBattlegroundQueues();

        bool isArenaTesting() const { return m_ArenaTesting; }
        bool isTesting() const { return m_Testing; }

        static bool IsArenaType(BattlegroundTypeId bgTypeId);
        static bool IsBattlegroundType(BattlegroundTypeId bgTypeId) { return !IsArenaType(bgTypeId); }
        static BattlegroundQueueTypeId BGQueueTypeId(BattlegroundTypeId bgTypeId, uint8 arenaType);
        static BattlegroundTypeId BGTemplateId(BattlegroundQueueTypeId bgQueueTypeId);
        static uint8 BGArenaType(BattlegroundQueueTypeId bgQueueTypeId);

        static HolidayIds BGTypeToWeekendHolidayId(BattlegroundTypeId bgTypeId);
        static BattlegroundTypeId WeekendHolidayIdToBGType(HolidayIds holiday);
        static bool IsBGWeekend(BattlegroundTypeId bgTypeId);

        bool IsBattlegroundHoliday(BattlegroundTypeId) const;
        BattlegroundHolidayContainer& GetBattlegroundHolidays() { return m_BattlegroundHolidays; }

        bool HaveSpectatorData() const { return m_spectatorData.size(); }
        void ClearSpectatorData() { m_spectatorData.clear(); }
        void AddSpectatorData(uint64 const &guid, std::string const &data) { m_spectatorData[guid] = data; }
        SpectatorDataStore &GetSpectatorDataStore() { return m_spectatorData; }

        void InitWargame(Group* p_First, Group* p_Second, BattlegroundTypeId p_BgTypeId);

    private:

        SpectatorDataStore m_spectatorData;
        BattlegroundHolidayContainer m_BattlegroundHolidays;

        BattleMastersMap    mBattleMastersMap;

        typedef std::map<BattlegroundTypeId, uint8> BattlegroundSelectionWeightMap; // TypeId and its selectionWeight
        /* Battlegrounds */
        BattlegroundSetMap m_Battlegrounds;
        BattlegroundSelectionWeightMap m_ArenaSelectionWeights;
        BattlegroundSelectionWeightMap m_BGSelectionWeights;
        BattlegroundSelectionWeightMap m_RatedBGSelectionWeights;
        std::vector<QueueSchedulerItem*> m_QueueUpdateScheduler;
        std::set<uint32> m_ClientBattlegroundIds[MAX_BATTLEGROUND_TYPE_ID][MAX_BATTLEGROUND_BRACKETS]; //the instanceids just visible for the client
        uint32 m_NextRatedArenaUpdate;
        bool   m_ArenaTesting;
        bool   m_Testing;
};

#define sBattlegroundMgr ACE_Singleton<BattlegroundMgr, ACE_Null_Mutex>::instance()
#endif


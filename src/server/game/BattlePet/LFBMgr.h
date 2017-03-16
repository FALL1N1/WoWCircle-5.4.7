#pragma once

#include "Common.h"
#include <ace/Singleton.h>
#include "LFB.h"
#include "LockedMap.h"
#include "LFBPlayerScript.h"

class Player;

enum LFBenum
{
    LFB_TIME_PROPOSAL                            = 2*MINUTE,
    LFB_QUEUEUPDATE_INTERVAL                     = 15*IN_MILLISECONDS,
    LFB_OFFLINE_REMOVE_INTERVAL                  = 1*MINUTE*IN_MILLISECONDS,
};

struct LFBQueueInfo;
struct LFBJoinResult;
struct LFBUpdateData;
struct LFBProposal;

typedef ACE_Based::LockedMap<uint64, LFBQueueInfo*> LFBQueueMap;
typedef ACE_Based::LockedMap<uint64, LFBState> LFBStateMap;
typedef ACE_Based::LockedMap<uint32 /*proposalId*/, LFBProposal*> LFBProposalMap;

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif
struct LFBJoinResult
{
    LFBJoinResult(time_t joinTime) : m_joinStatus(LFB_NONE), m_joinTime(joinTime)
    {
    }

    time_t m_joinTime;
    LFBUpdateStatus m_joinStatus;
    std::vector<BattlePetStatus> m_petStatus;
};

struct LFBQueueInfo
{
    LFBQueueInfo()
        : m_joinTime(0)
        , m_petsWeight(0)
        , m_proposalId(0)
        , m_team(TEAM_ALLIANCE)
        , m_status(LFB_NONE)
        , m_isDeleted(false)
    {
    }

    LFBQueueInfo(LFBJoinResult const& result, uint32 team, int32 weight)
        : m_status(result.m_joinStatus)
        , m_joinTime(result.m_joinTime)
        , m_queuedTime(0)
        , m_proposalId(0)
        , m_petsWeight(weight)
        , m_team(team)
        , m_isDeleted(false)
    {

    }

    int32 m_petsWeight;
    uint32 m_proposalId;
    uint32 m_team;
    time_t m_queuedTime;
    time_t m_joinTime;
    LFBUpdateStatus m_status;
    bool m_isDeleted;
};

struct LFBUpdateData
{
    LFBUpdateData()
        : m_status(LFB_NONE)
    {
    }

    LFBUpdateData(LFBUpdateStatus status)
        : m_status(status)
    {
    }

    LFBUpdateStatus m_status;
};

struct LFBProposalItem
{
    LFBProposalItem(ObjectGuid guid, LfbAnswer answer)
        : m_player(guid)
        , m_answer(answer)
    {

    }

    LFBProposalItem()
        : m_player(0LL)
        , m_answer(LfbAnswer::LFB_ANSWER_PENDING)
    {

    }

    ObjectGuid m_player;
    LfbAnswer m_answer;
};

struct LFBProposal
{
    LFBProposal(ObjectGuid first, ObjectGuid second)
        : m_cancelTime(getMSTime() + LFB_TIME_PROPOSAL*IN_MILLISECONDS)
        , m_state(LfbProposalState::LFB_PROPOSAL_STATE_INITIATING)
    {
        m_players.push_back(LFBProposalItem(first, LfbAnswer::LFB_ANSWER_DENY));
        m_players.push_back(LFBProposalItem(second, LfbAnswer::LFB_ANSWER_DENY));
    }

    LFBProposal()
        : m_cancelTime(getMSTime() + LFB_TIME_PROPOSAL*IN_MILLISECONDS)
        , m_state(LfbProposalState::LFB_PROPOSAL_STATE_INITIATING)
    {

    }

    std::vector<LFBProposalItem> m_players;
    time_t m_cancelTime;
    LfbProposalState m_state;
};
#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

class LFBMgr
{
    friend class ACE_Singleton<LFBMgr, ACE_Null_Mutex>;

    private:
        LFBMgr();
        ~LFBMgr();

    public:
        void Update(uint32 diff);

        void Join(Player* player);
        void Leave(Player* player);
        
        void SendJoinResult(Player* player, LFBJoinResult const& result);
        void SendUpdateStatus(uint64 guid, LFBUpdateData const& result);
        void SendUpdateStatus(Player* player, LFBUpdateData const& result);

        void SetOnline(Player* player, bool isOnline);

        void LoadFromDB(uint64 guid);
        void SaveToDB(uint64 guid);

        void UpdateProposal(uint64 guid, bool agree);
        void FinishBattle(Player* player);
    private:
        void TeleportPlayers(LFBProposal* proposal);
        void CheckJoin(Player* player, LFBJoinResult* joinResult);
        void CheckPetJoin(Player* player, LFBJoinResult* joinResult);
        uint32 GeneratePetsWeight(Player* player);

        void SelectOpponent(LFBQueueMap::iterator itr);
        void RemoveProposal(LFBProposalMap::iterator itr, LFBUpdateStatus status);

        void RemoveFromQueue(uint64 guid)
        {
            LFBQueueMap::iterator itr = m_queue.find(guid);
            if (itr == m_queue.end())
                return;

            delete itr->second;
            m_queue.erase(itr);
        }

        void RemoveFromQueue(LFBQueueMap::iterator& itr)
        {
            if (itr == m_queue.end())
                return;

            delete itr->second;
            m_queue.erase(itr++);
        }

        void MarkForDelete(uint64 guid)
        {
            LFBQueueMap::iterator itr = m_queue.find(guid);
            if (itr == m_queue.end())
                return;

            itr->second->m_isDeleted = true;
        }

        void MarkForDelete(LFBQueueMap::iterator itr)
        {
            if (itr == m_queue.end())
                return;

            itr->second->m_isDeleted = true;
        }

        void SetState(uint64 guid, LFBState state)
        {
            m_state[guid] = state;
        }

        LFBState GetState(uint64 guid)
        {
            return m_state[guid];
        }

        LFBQueueInfo const* GetLFBQueueInfo(uint64 guid) const
        {
            LFBQueueMap::const_iterator itr = m_queue.find(guid);
            if (itr != m_queue.end())
                return itr->second;

            return nullptr;
        }

        LFBQueueInfo* GetLFBQueueInfo(uint64 guid)
        {
            LFBQueueMap::iterator itr = m_queue.find(guid);
            if (itr != m_queue.end())
                return itr->second;

            return nullptr;
        }

        void ClearState(uint64 guid)
        {
            LFBStateMap::iterator itr = m_state.find(guid);
            if (itr == m_state.end())
                return;

            m_state.erase(itr);
        }

        bool IsInQueue(uint64 guid) const
        {
            return GetLFBQueueInfo(guid) != nullptr;
        }

        uint32 const GenerateProposalId()
        {
            return m_nextProposalId++;
        }

        // General variables
        uint32 m_QueueTimer;                               ///< used to check interval of update
        int32 m_WaitTimeAvg;                               ///< Average wait time to find a group queuing as multiple roles
        uint32 m_NumWaitTimeAvg;                           ///< Num of players used to calc avs wait time
        LFBQueueMap m_queue;
        LFBStateMap m_state;
        LFBProposalMap m_proposal;

        uint32 m_nextProposalId;
};

#define sLFBMgr ACE_Singleton<LFBMgr, ACE_Null_Mutex>::instance()
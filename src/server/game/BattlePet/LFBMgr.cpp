#include "LFBMgr.h"
#include "PetJournal/PetJounral.h"
#include "Battle/PetBattle.h"
#include "ObjectMgr.h"

LFBMgr::LFBMgr()
    : m_QueueTimer(0)
    , m_WaitTimeAvg(0)
    , m_NumWaitTimeAvg(0)
    , m_nextProposalId(0)
{
    new LFBPlayerScript();
}

LFBMgr::~LFBMgr()
{
    for (auto const& kvp : m_queue)
    {
        delete kvp.second;
    }

    for (auto const& kvp : m_proposal)
    {
        delete kvp.second;
    }

    m_queue.clear();
    m_proposal.clear();
}

void LFBMgr::Join(Player* player)
{
    if (!player || !player->GetSession())
        return;

    LFBJoinResult result(getMSTime());
    CheckJoin(player, &result);

    SendJoinResult(player, result);
    if (result.m_joinStatus == LFB_JOIN_QUEUE)
    {
        m_queue[player->GetGUID()] = new LFBQueueInfo(result, player->GetTeam(), GeneratePetsWeight(player));

        SetState(player->GetGUID(), LFB_STATE_QUEUED);
    }
}

void LFBMgr::Leave(Player* player)
{
    if (!player)
        return;

    LFBQueueInfo* info = GetLFBQueueInfo(player->GetGUID());
    if (info == nullptr || info->m_isDeleted)
        return;

    LFBState state = GetState(player->GetGUID());
    switch (state)
    {
        case LFBState::LFB_STATE_PROPOSAL:
        {
            LFBProposalMap::iterator itr = m_proposal.find(info->m_proposalId);
            if (itr != m_proposal.end())
                RemoveProposal(itr, LFBUpdateStatus::LFB_PROPOSAL_DECLINED);
        }
        case LFBState::LFB_STATE_FINISHED:
        case LFBState::LFB_STATE_IN_COMBAT:
        case LFBState::LFB_STATE_QUEUED:
        {
            LFBUpdateData updateData(LFB_LEAVE_QUEUE);
            SendUpdateStatus(player, updateData);
            ClearState(player->GetGUID());

            MarkForDelete(player->GetGUID());
            break;
        }
    }
}

void LFBMgr::CheckJoin(Player* player, LFBJoinResult* joinResult)
{
    if (!player || !joinResult)
        return;

    if (player->GetPetBattle())
    {
        joinResult->m_joinStatus = LFB_PET_BATTLE_IS_STARTED;
    }
    else if (IsInQueue(player->GetGUID()) || GetState(player->GetGUID()) == LFB_STATE_QUEUED)
    {
        joinResult->m_joinStatus = LFB_ALREADY_QUEUED;
    }
    else if (player->getRace() == RACE_PANDAREN_NEUTRAL || player->GetTeam() == TEAM_NEUTRAL)
    {
        joinResult->m_joinStatus = LFB_CANT_JOIN_DUE_TO_UNSELECTED_FACTION;
    }
    else
    {
        CheckPetJoin(player, joinResult);
    }

    if (joinResult->m_joinStatus == LFB_NONE)
        joinResult->m_joinStatus = LFB_JOIN_QUEUE;
}

void LFBMgr::CheckPetJoin(Player* player, LFBJoinResult* joinResult)
{
    if (!joinResult || joinResult->m_joinStatus != LFB_NONE)
        return;

    auto const journal = player->GetPetJournal();

    for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
    {
        BattlePetStatus status = BATTLE_PET_STATUS_OK;

        uint64 id = journal->GetLoadoutSlot(slot);
        if (id != 0LL)
        {
            BattlePet::PetTemplate const* proto = journal->GetBattlePet(id);
            if (proto)
            {
                if (proto->GetCurrentHealth() != 0)
                {
                    if (proto->IsPetAvailableForBattle())
                    {
                    }
                    else
                        status = BATTLE_PET_STATUS_PET_CANT_FIGHT;
                }
                else
                    status = BATTLE_PET_STATUS_PET_IS_DIED;
            }
            else
                status = BATTLE_PET_STATUS_NOT_BATTLE_PET_IN_SLOT;
        }
        else
            status = BATTLE_PET_STATUS_NOT_BATTLE_PET_IN_SLOT;

        joinResult->m_petStatus.push_back(status);
        /*TODO: BATTLE_PET_STATUS_PET_NOT_AVAILABLE_FOR_COMBAT*/
    }

    for (BattlePetStatus status : joinResult->m_petStatus)
    {
        if (status != BATTLE_PET_STATUS_OK)
        {
            joinResult->m_joinStatus = LFB_CANT_JOINT_QUEUE_DUE_TO_PET_STATUS;
            return;
        }
    }
}

void LFBMgr::SendJoinResult(Player* player, LFBJoinResult const& result)
{
    if (!player || !player->GetSession())
        return;

    ObjectGuid guid = player->GetGUID();

    WorldPacket data(SMSG_BATTLE_PET_QUEUE_STATUS, 50);
    data.WriteBit(guid[5]);
    if (result.m_joinStatus == LFB_CANT_JOINT_QUEUE_DUE_TO_PET_STATUS)
        data.WriteBits(BATTLE_PET_MAX_LOADOUT_SLOTS, 22);
    else
        data.WriteBits(0, 22);
    data.WriteBit(true); // Average wait time, 0 at start
    data.WriteBit(guid[7]);
    data.WriteBit(true); // Time in queue, 0 at start
    data.WriteBit(guid[2]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[3]);
    data << uint32(result.m_joinTime);
    data.WriteByteSeq(guid[4]);
    data << uint32(result.m_joinStatus);
    data << uint32(0);
    data.WriteByteSeq(guid[3]);
    data << uint32(6); // Unk
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[1]);
    data << uint32(0);
    if (result.m_joinStatus == LFB_CANT_JOINT_QUEUE_DUE_TO_PET_STATUS)
    {
        for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
        {
            data << uint32(result.m_petStatus[slot]);
        }
    }
    data << uint32(m_queue.size());
    data.WriteByteSeq(guid[6]);

    player->GetSession()->SendPacket(&data);
}

void LFBMgr::SendUpdateStatus(uint64 guid, LFBUpdateData const& result)
{
    Player* player = sObjectAccessor->FindPlayer(guid);
    if (player)
        SendUpdateStatus(player, result);
}

void LFBMgr::SendUpdateStatus(Player* player, LFBUpdateData const& result)
{
    if (!player || !player->GetSession())
        return;

    if (!player->IsInWorld())
        return;

    LFBQueueInfo const* info = GetLFBQueueInfo(player->GetGUID());
    if (!info)
        return;

    ObjectGuid guid = player->GetGUID();

    WorldPacket data(SMSG_BATTLE_PET_QUEUE_STATUS, 50);
    data.WriteBit(guid[5]);
    data.WriteBits(0, 22);
    data.WriteBit(true); // Average wait time, 0 at start
    data.WriteBit(guid[7]);
    data.WriteBit(true); // Time in queue, 0 at start
    data.WriteBit(guid[2]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[3]);
    data << uint32(info->m_joinTime);
    data.WriteByteSeq(guid[4]);
    data << uint32(result.m_status);
    if (result.m_status != LFB_LEAVE_QUEUE)
        data << uint32(info->m_queuedTime);
    else
        data << uint32(0);
    data.WriteByteSeq(guid[3]);
    data << uint32(6); // Unk
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[1]);
    if (result.m_status != LFB_LEAVE_QUEUE)
        data << uint32(m_WaitTimeAvg / 1000);
    else
        data << uint32(0);
    data << uint32(m_queue.size());
    data.WriteByteSeq(guid[6]);

    player->GetSession()->SendPacket(&data);
}

void LFBMgr::Update(uint32 diff)
{
    time_t currTime = getMSTime();

    for (LFBQueueMap::iterator itr = m_queue.begin(); itr != m_queue.end();)
    {
        LFBQueueInfo* info = itr->second;
        if (info->m_isDeleted)
        {
            RemoveFromQueue(itr);
        }
        else
            ++itr;
    }

    for (LFBProposalMap::iterator itr = m_proposal.begin(); itr != m_proposal.end();)
    {
        LFBProposalMap::iterator itrRemove = itr++;
        if (itrRemove->second->m_cancelTime < currTime)
        {
            RemoveProposal(itrRemove, LFBUpdateStatus::LFB_PROPOSAL_FAILED);
        }
    }

    for (LFBQueueMap::iterator itr = m_queue.begin(); itr != m_queue.end(); ++itr)
    {
        LFBState state = GetState(itr->first);
        LFBQueueInfo* info = itr->second;
        if (info->m_isDeleted)
            continue;

        if (state != LFBState::LFB_STATE_QUEUED)
            continue;

        SelectOpponent(itr);
    }

    if (m_QueueTimer >= LFB_QUEUEUPDATE_INTERVAL)
    {
        for (LFBQueueMap::iterator itr = m_queue.begin(); itr != m_queue.end(); ++itr)
        {
            uint64 guid = itr->first;
            LFBQueueInfo* info = itr->second;
            if (info->m_isDeleted)
                continue;

            uint32 queuedTime = uint32(currTime - info->m_joinTime);

            uint32 old_number = m_NumWaitTimeAvg++;
            m_WaitTimeAvg = int32((m_WaitTimeAvg * old_number + queuedTime) / m_NumWaitTimeAvg);

            info->m_queuedTime = queuedTime / 1000;

            LFBState state = GetState(guid);
            if (state == LFB_STATE_QUEUED)
            {
                if (Player* player = sObjectAccessor->FindPlayer(guid))
                {
                    LFBUpdateData updateData(LFB_UPDATE_STATUS);
                    SendUpdateStatus(player, updateData);
                }
                else // remove from queue
                {
                    MarkForDelete(itr);
                    ClearState(guid);
                }
            }
        }
        m_QueueTimer = 0;
    }
    else
        m_QueueTimer += diff;
}

void LFBMgr::SelectOpponent(LFBQueueMap::iterator itr)
{
    uint64 guid = itr->first;
    LFBQueueInfo* pinfo = itr->second;

    ++itr;

    if (itr == m_queue.end())
        return;

    Player* player = sObjectAccessor->FindPlayer(guid);
    if (!player)
        return;

    int32 petsWeight = pinfo->m_petsWeight;

    for (; itr != m_queue.end(); ++itr)
    {
        uint64 nguid = itr->first;
        if (GetState(nguid) != LFBState::LFB_STATE_QUEUED)
            continue;

        LFBQueueInfo* info = itr->second;
        if (info->m_isDeleted)
            continue;

        if (pinfo->m_team != info->m_team)
            continue;

        int32 opponentPetsWeight = info->m_petsWeight;
        if (abs(petsWeight - opponentPetsWeight) > (int32)sWorld->getIntConfig(CONFIG_PET_BATTLE_LEVEL_DIFF))
            continue;

        if (Player* opponent = sObjectAccessor->FindPlayer(nguid))
        {
            uint32 const proposalId = GenerateProposalId();
            m_proposal[proposalId] = new LFBProposal(guid, nguid);

            info->m_proposalId = pinfo->m_proposalId = proposalId;

            LFBUpdateData updateData(LFB_PROPOSAL_BEGIN);
            SendUpdateStatus(player, updateData);
            SendUpdateStatus(opponent, updateData);

            SetState(guid, LFBState::LFB_STATE_PROPOSAL);
            SetState(nguid, LFBState::LFB_STATE_PROPOSAL);

            WorldPacket data(SMSG_PET_BATTLE_GAME_FOUNDED);
            player->SendDirectMessage(&data);
            opponent->SendDirectMessage(&data);
            return;
        }
    }
}

void LFBMgr::RemoveProposal(LFBProposalMap::iterator itr, LFBUpdateStatus status)
{
    LFBProposal* proposal = itr->second;

    if (status == LFBUpdateStatus::LFB_PROPOSAL_FAILED)
    {
        for (LFBProposalItem& item : proposal->m_players)
        {
            item.m_answer = LfbAnswer::LFB_ANSWER_DENY;
        }
    }

    std::set<uint64> toRemove;
    for (LFBProposalItem& item : proposal->m_players)
    {
        if (item.m_answer == LfbAnswer::LFB_ANSWER_AGREE)
            continue;

        if (item.m_answer == LfbAnswer::LFB_ANSWER_DENY || status == LFBUpdateStatus::LFB_PROPOSAL_FAILED)
        {
            item.m_answer = LfbAnswer::LFB_ANSWER_DENY;
            toRemove.insert(item.m_player);
        }
    }

    if (!toRemove.empty())
    {
        for (LFBProposalItem& item : proposal->m_players)
        {
            Player* player = ObjectAccessor::FindPlayer(item.m_player);
            if (!player)
                continue;

            uint64 guid = player->GetGUID();
            if (toRemove.find(guid) != toRemove.end())
            {
                LFBUpdateData updateData(LFB_LEAVE_QUEUE);
                SendUpdateStatus(player, updateData);
                ClearState(player->GetGUID());

                MarkForDelete(player->GetGUID());
            }
            else
            {
                /*Re-join queue*/
                SetState(guid, LFBState::LFB_STATE_QUEUED);
                LFBQueueInfo* info = GetLFBQueueInfo(player->GetGUID());
                LFBJoinResult result(info->m_joinTime);
                result.m_joinStatus = LFB_JOIN_QUEUE;
                SendJoinResult(player, result);
            }
        }
    }

    delete proposal;
    m_proposal.erase(itr);
}

void LFBMgr::UpdateProposal(uint64 guid, bool agree)
{
    LFBQueueInfo const* info = GetLFBQueueInfo(guid);
    if (info == nullptr)
        return;

    LFBProposalMap::iterator itr = m_proposal.find(info->m_proposalId);
    if (itr == m_proposal.end())
        return;

    LFBProposal* proposal = itr->second;
    for (LFBProposalItem& item : proposal->m_players)
    {
        if (item.m_player != guid)
            continue;

        item.m_answer = agree ? LfbAnswer::LFB_ANSWER_AGREE : LfbAnswer::LFB_ANSWER_DENY;

        switch (item.m_answer)
        {
            case LfbAnswer::LFB_ANSWER_AGREE:
                break;
            case LfbAnswer::LFB_ANSWER_DENY:
            {
                LFBUpdateData data(LFBUpdateStatus::LFB_PROPOSAL_DECLINED);
                SendUpdateStatus(guid, data);

                for (LFBProposalItem& item2 : proposal->m_players)
                {
                    if (item2.m_player == guid)
                        continue;

                    LFBUpdateData data2(LFBUpdateStatus::LFB_OPPONENT_PROPOSAL_DECLINED);
                    SendUpdateStatus(item2.m_player, data2);
                }

                RemoveProposal(itr, LFBUpdateStatus::LFB_PROPOSAL_DECLINED);
                return;
            }
        }
    }

    uint8 answered = 0;
    for (LFBProposalItem& item : proposal->m_players)
    {
        if (item.m_answer == LfbAnswer::LFB_ANSWER_AGREE)
            ++answered;
    }

    if (answered != 2)
        return;

    TeleportPlayers(proposal);

    delete itr->second;
    m_proposal.erase(itr);
}

struct PetBattleMembersPositions
{
    PetBattleMembersPositions(uint32 map, uint32 team, G3D::Vector3 first, G3D::Vector3 second)
        : m_map(map)
        , m_team(team)
    {
        m_pos[0] = first;
        m_pos[1] = second;
    }

    uint32 m_map;
    uint32 m_team;
    G3D::Vector3 m_pos[2];
};

const static PetBattleMembersPositions _battlePositions[7] = 
{

    PetBattleMembersPositions(0, ALLIANCE, G3D::Vector3(-9502.376f, 114.492f, 59.822f), G3D::Vector3(-9493.934f, 119.854f, 58.459f)),
    PetBattleMembersPositions(0, ALLIANCE, G3D::Vector3(-10048.859f, 1231.028f, 40.881f), G3D::Vector3(-10054.330f, 1239.399f, 40.894f)),
    PetBattleMembersPositions(0, ALLIANCE, G3D::Vector3(-10909.911f, -362.280f, 39.643f), G3D::Vector3(-10899.923f, -362.773f, 39.265f)),
    PetBattleMembersPositions(0, ALLIANCE, G3D::Vector3(-10439.142f, -1939.163f, 104.313f), G3D::Vector3(-10439.306f, -1949.162f, 103.763f)),
    
    PetBattleMembersPositions(1, HORDE, G3D::Vector3(-954.766f, -3255.210f, 95.645f), G3D::Vector3(-958.212f, -3264.597f, 95.837f)),
    PetBattleMembersPositions(1, HORDE, G3D::Vector3(-2285.038f, -2155.838f, 95.843f), G3D::Vector3(-2281.738f, -2146.397f, 95.843f)),
    //PetBattleMembersPositions(1, HORDE, G3D::Vector3(-1369.247f, -2716.736f, 253.246f), G3D::Vector3(-1359.747f, -2713.613f, 253.390f)),
    PetBattleMembersPositions(1, HORDE, G3D::Vector3(-127.255f, -4959.972f, 20.903f), G3D::Vector3(-129.017f, -4950.128f, 21.378f))
};

void LFBMgr::TeleportPlayers(LFBProposal* proposal)
{
    Player* players[2] = { 0 };

    uint8 index = 0;
    for (LFBProposalItem& item : proposal->m_players)
    {
        uint64 guid = item.m_player;
        Player* player = sObjectAccessor->FindPlayer(guid);
        if (player == nullptr)
        {
            sLog->outError(LOG_FILTER_PETS, "LFBMgr::TeleportPlayers error, one of the players are not exist");
            return;
        }

        players[index++] = player;
    }

    ASSERT(players[0]->GetTeam() == players[1]->GetTeam());

    uint32 team = players[0]->GetTeam();

    std::vector<PetBattleMembersPositions> positions;
    for (PetBattleMembersPositions const& data : _battlePositions)
    {
        if (data.m_team != team)
            continue;

        positions.push_back(data);
    }

    PetBattleMembersPositions location = positions[urand(0, positions.size()-1)];

    BattlePet::Battle::CreateBattle(players[0], players[1], false, true);

    index = 0;
    for (Player* player : players)
    {
        G3D::Vector3 const& pos = location.m_pos[index];

        if (BattlePet::Battle* battle = player->GetPetBattle())
            battle->SetPosition(pos, PBOID(PBOID::PAD_0 + index));

        player->SetBattlegroundEntryPoint();
        player->ScheduleDelayedOperation(DELAYED_PET_BATTLE_INITIAL);

        ++index;
    }

    index = 0;
    for (Player* player : players)
    {
        G3D::Vector3 const& pos = location.m_pos[index++];
        player->TeleportTo(location.m_map, pos.x + 0.01f, pos.y + 0.01f, pos.z + 0.01f, player->GetOrientation());
        SetState(player->GetGUID(), LFBState::LFB_STATE_IN_COMBAT);
    }
}

void LFBMgr::FinishBattle(Player* player)
{
    LFBState state = GetState(player->GetGUID());
    if (state != LFBState::LFB_STATE_IN_COMBAT)
        return;

    SetState(player->GetGUID(), LFBState::LFB_STATE_FINISHED);

    Leave(player);
    player->TeleportToBGEntryPoint();
}

void LFBMgr::SetOnline(Player* player, bool isOnline)
{
    if (!player)
        return;

    Leave(player);
    return;

    LFBQueueInfo* info = GetLFBQueueInfo(player->GetGUID());
    if (!info)
        return;

    LFBState state = GetState(player->GetGUID());
    switch (state)
    {
    case LFB_STATE_QUEUED:
    {
        if (isOnline)
        {
            LFBJoinResult result(info->m_joinTime);
            result.m_joinStatus = LFB_JOIN_QUEUE;
            SendJoinResult(player, result);
        }
        break;
    }
    case LFB_STATE_PROPOSAL:
    {
        if (!isOnline)
        {
            // Remove proposal
        }
        break;
    }
    case LFB_STATE_IN_COMBAT:
    {
        if (!isOnline)
        {
            // Stop combat
            return;
        }
        break;
    }
    case LFB_STATE_FINISHED:
    case LFB_STATE_NONE:
        ASSERT(false); // QueueInfo should be removed before swith state to FINISHED
    default:
        ASSERT(false);
    }

    if (!isOnline)
    {
        LFBUpdateData updateData(LFB_LEAVE_QUEUE);
        SendUpdateStatus(player, updateData);
    }
}

void LFBMgr::LoadFromDB(uint64 guid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_LFB_DATA_BY_GUID);
    stmt->setUInt64(0, guid);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);
    if (!result)
        return;

    Field* fields = result->Fetch();

    LFBState state = LFBState(fields[0].GetUInt8());
    switch (state)
    {
        case LFB_STATE_NONE:
        case LFB_STATE_FINISHED:
            ASSERT(false);
            break;
        case LFB_STATE_QUEUED:
            // add back to queue
            break;
        case LFB_STATE_PROPOSAL:
        case LFB_STATE_IN_COMBAT:
            break;
    }
}

void LFBMgr::SaveToDB(uint64 guid)
{
    LFBState state = GetState(guid);
    if (state == LFB_STATE_NONE || state == LFB_STATE_FINISHED)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_LFB_DATA_BY_GUID);
    stmt->setUInt64(0, guid);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_LFB_DATA_BY_GUID);
    stmt->setUInt64(0, guid);
    stmt->setUInt8(1, state);

    CharacterDatabase.Execute(stmt);
}

uint32 LFBMgr::GeneratePetsWeight(Player* player)
{
    uint32 weight = 0;

    auto const journal = player->GetPetJournal();

    for (uint8 slot = 0; slot < BATTLE_PET_MAX_LOADOUT_SLOTS; ++slot)
    {
        uint64 id = journal->GetLoadoutSlot(slot);
        if (!journal->LoadoutSlotIsUnlocked(slot) || id == 0LL)
            continue;

        BattlePet::PetTemplate const* proto = journal->GetBattlePet(id);
        if (!proto)
            continue;

        weight += proto->GetLevel();
    }

    return weight;
}
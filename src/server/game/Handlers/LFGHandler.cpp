/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "WorldSession.h"
#include "WorldPacket.h"
#include "DBCStores.h"
#include "Player.h"
#include "Group.h"
#include "LFGMgr.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "GameEventMgr.h"
#include "InstanceScript.h"

void WorldSession::HandleLfgJoinOpcode(WorldPacket& recvData)
{
    uint32 numDungeons;
    uint32 dungeon;
    uint32 roles;
    uint8 length = 0;
    uint8 unk8 = 0;
    bool unkbit = false;

    recvData >> unk8;

    for (int i = 0; i < 3; ++i)
        recvData.read_skip<uint32>();

    recvData >> roles;

    numDungeons = recvData.ReadBits(22);
    length = recvData.ReadBits(8);
    unkbit = recvData.ReadBit();

    if (!numDungeons)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_JOIN [" UI64FMTD "] no dungeons selected", GetPlayer()->GetGUID());
        recvData.rfinish();
        return;
    }

    std::string comment = recvData.ReadString(length);

    LfgDungeonSet newDungeons;
    for (uint32 i = 0; i < numDungeons; ++i)
    {
        recvData >> dungeon;
        dungeon &= 0xFFFFFF;
        newDungeons.insert(dungeon);       // remove the type from the dungeon entry
    }

    const LFGDungeonEntry* entry = sLFGDungeonStore.LookupEntry(*newDungeons.begin() & 0xFFFFFF);
    uint8 l_Category = LFG_CATEGORIE_DUNGEON;
    uint8 maxGroupSize = 5;
    if (entry != NULL)
        l_Category = entry->category;

    if (l_Category == LFG_CATEGORIE_RAID)
        maxGroupSize = 25;
    if (l_Category == LFG_CATEGORIE_SCENARIO)
        maxGroupSize = 3;

    if (!sWorld->getBoolConfig(CONFIG_DUNGEON_FINDER_ENABLE) ||
        (GetPlayer()->GetGroup() && GetPlayer()->GetGroup()->GetLeaderGUID() != GetPlayer()->GetGUID() &&
        (GetPlayer()->GetGroup()->GetMembersCount() == maxGroupSize || !GetPlayer()->GetGroup()->isLFGGroup())))
    {
        recvData.rfinish();
        return;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_JOIN [" UI64FMTD "] roles: %u, Dungeons: %u, Comment: %s", GetPlayer()->GetGUID(), roles, uint8(newDungeons.size()), comment.c_str());
    sLFGMgr->Join(GetPlayer(), uint8(roles), newDungeons, comment);
}

void WorldSession::HandleLfgLeaveOpcode(WorldPacket&  /*recvData*/)
{
    Group* grp = GetPlayer()->GetGroup();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_LEAVE [" UI64FMTD "] in group: %u", GetPlayer()->GetGUID(), grp ? 1 : 0);

    // Check cheating - only leader can leave the queue
    if (!grp || grp->GetLeaderGUID() == GetPlayer()->GetGUID())
        sLFGMgr->Leave(GetPlayer(), grp);
}

void WorldSession::HandleLfgProposalResultOpcode(WorldPacket& recvData)
{
    uint32 lfgGroupID;                                     // Internal lfgGroupID
    bool accept;                                           // Accept to join?

    recvData >> lfgGroupID;                                // ProposalId
    recvData.read_skip<uint32>();                          // Time
    recvData.read_skip<uint32>();                          // Const flag 3
    recvData.read_skip<uint32>();                          // QueueId
    

    ObjectGuid guid1;
    ObjectGuid guid2;

    guid2[2] = recvData.ReadBit();
    guid1[6] = recvData.ReadBit();
    guid2[4] = recvData.ReadBit();
    guid2[3] = recvData.ReadBit();
    guid1[2] = recvData.ReadBit();
    guid2[5] = recvData.ReadBit();
    guid2[6] = recvData.ReadBit();
    guid2[1] = recvData.ReadBit();
    guid1[5] = recvData.ReadBit();
    guid1[0] = recvData.ReadBit();
    accept = recvData.ReadBit();
    guid1[1] = recvData.ReadBit();
    guid1[4] = recvData.ReadBit();
    guid1[3] = recvData.ReadBit();
    guid1[7] = recvData.ReadBit();
    guid2[0] = recvData.ReadBit();
    guid2[7] = recvData.ReadBit();

    recvData.ReadByteSeq(guid2[0]);
    recvData.ReadByteSeq(guid2[7]);
    recvData.ReadByteSeq(guid2[3]);
    recvData.ReadByteSeq(guid1[1]);
    recvData.ReadByteSeq(guid1[4]);
    recvData.ReadByteSeq(guid1[0]);
    recvData.ReadByteSeq(guid2[1]);
    recvData.ReadByteSeq(guid1[5]);
    recvData.ReadByteSeq(guid1[7]);
    recvData.ReadByteSeq(guid1[3]);
    recvData.ReadByteSeq(guid1[6]);
    recvData.ReadByteSeq(guid2[4]);
    recvData.ReadByteSeq(guid2[2]);
    recvData.ReadByteSeq(guid2[5]);
    recvData.ReadByteSeq(guid2[6]);
    recvData.ReadByteSeq(guid1[2]);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_PROPOSAL_RESULT [" UI64FMTD "] proposal: %u accept: %u", GetPlayer()->GetGUID(), lfgGroupID, accept ? 1 : 0);
    sLFGMgr->UpdateProposal(lfgGroupID, GetPlayer()->GetGUID(), accept);
}

void WorldSession::HandleLfgSetRolesOpcode(WorldPacket& recvData)
{
    uint32 roles;
    uint8 unk;

    recvData >> roles;                                    // Player Group Roles
    recvData >> unk;

    uint64 guid = GetPlayer()->GetGUID();
    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_SET_ROLES [" UI64FMTD "] Not in group", guid);
        return;
    }
    uint64 gguid = grp->GetGUID();
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_SET_ROLES: Group [" UI64FMTD "], Player [" UI64FMTD "], Roles: %u", gguid, guid, roles);
    sLFGMgr->UpdateRoleCheck(gguid, guid, roles);
}

void WorldSession::HandleLfgSetCommentOpcode(WorldPacket&  recvData)
{
    std::string comment;
    recvData >> comment;
    uint64 guid = GetPlayer()->GetGUID();
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SET_LFG_COMMENT [" UI64FMTD "] comment: %s", guid, comment.c_str());

    sLFGMgr->SetComment(guid, comment);
}

void WorldSession::HandleLfgSetBootVoteOpcode(WorldPacket& recvData)
{
    bool agree;                                            // Agree to kick player
    agree = recvData.ReadBit();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_SET_BOOT_VOTE [" UI64FMTD "] agree: %u", GetPlayer()->GetGUID(), agree ? 1 : 0);
    sLFGMgr->UpdateBoot(GetPlayer(), agree);
}

void WorldSession::HandleLfgTeleportOpcode(WorldPacket& recvData)
{
    bool out;
    out = recvData.ReadBit();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_LFG_TELEPORT [" UI64FMTD "] out: %u", GetPlayer()->GetGUID(), out ? 1 : 0);
    sLFGMgr->TeleportPlayer(GetPlayer(), out, true);
}

void WorldSession::HandleLfgLockInfoRequestOpcode(WorldPacket& p_RecvData)
{
    uint8 l_Value;
    bool l_GroupPacket;

    p_RecvData >> l_Value;
    l_GroupPacket = p_RecvData.ReadBit();

    ObjectGuid l_Guid = GetPlayer()->GetGUID();

    // Get Random dungeons that can be done at a certain level and expansion
    LfgDungeonSet l_RandomDungeons;
    uint8 l_Level = GetPlayer()->getLevel();
    uint8 l_Expansion = GetPlayer()->GetSession()->Expansion();
    for (uint32 l_I = 0; l_I < sLFGDungeonStore.GetNumRows(); ++l_I)
    {
        LFGDungeonEntry const* l_Dungeon = sLFGDungeonStore.LookupEntry(l_I);
        if (l_Dungeon && l_Dungeon->expansion <= l_Expansion && l_Dungeon->minlevel <= l_Level && l_Level <= l_Dungeon->maxlevel)
        {
            if (l_Dungeon->flags & LFG_FLAG_SEASONAL)
            {
                if (HolidayIds holiday = sLFGMgr->GetDungeonSeason(l_Dungeon->ID))
                    if (holiday == HOLIDAY_WOTLK_LAUNCH || !IsHolidayActive(holiday))
                        continue;
            }
            else if (l_Dungeon->type != LfgType::TYPEID_RANDOM_DUNGEON && l_Dungeon->category != TYPEID_DUNGEON)
                continue;

            /// Some scenarios are alliance or horde only
            if (l_Dungeon->faction != -1)
            {
                if (l_Dungeon->faction == 1 && GetPlayer()->GetTeamId() != TEAM_ALLIANCE)
                    continue;

                if (l_Dungeon->faction == 0 && GetPlayer()->GetTeamId() != TEAM_HORDE)
                    continue;
            }

            l_RandomDungeons.insert(l_Dungeon->Entry());
        }
    }

    float l_QuestXpRate = GetPlayer()->GetRate<float>(RateType::XpQuest);
    if (float l_PersonalRate = GetPlayer()->GetPersonnalXpRate())
        l_QuestXpRate = l_PersonalRate;

    if (IsPremium())
        l_QuestXpRate *= sWorld->getRate(RATE_XP_QUEST_PREMIUM);

    // Get player locked Dungeons
    LfgLockMap l_Lock = sLFGMgr->GetLockedDungeons(l_Guid);

    uint32 l_Rsize = uint32(l_RandomDungeons.size());
    uint32 l_Lsize = uint32(l_Lock.size());

    WorldPacket l_Data(SMSG_LFG_PLAYER_INFO, 1 + l_Rsize * (4 + 1 + 4 + 4 + 4 + 4 + 1 + 4 + 4 + 4) + 4 + l_Lsize * (1 + 4 + 4 + 4 + 4 + 1 + 4 + 4 + 4));

    bool l_HasGuid = true;
    l_Data.WriteBit(l_HasGuid);
    if (l_HasGuid)
    {
        uint8 l_BitOrder[8] = { 0, 6, 7, 5, 2, 4, 1, 3 };
        l_Data.WriteBitInOrder(l_Guid, l_BitOrder);
    }

    l_Data.WriteBits(l_RandomDungeons.size(), 17);

    for (LfgDungeonSet::const_iterator l_It = l_RandomDungeons.begin(); l_It != l_RandomDungeons.end(); ++l_It)
    {
        LfgReward const* l_Reward = sLFGMgr->GetRandomDungeonReward(*l_It, l_Level);
        Quest const* l_QRew = NULL;

        bool l_Done = false;
        if (l_Reward)
        {
            l_QRew = sObjectMgr->GetQuestTemplate(l_Reward->reward[0].questId);
            if (l_QRew)
            {
                l_Done = !GetPlayer()->CanRewardQuest(l_QRew, false);
                if (l_Done)
                    l_QRew = sObjectMgr->GetQuestTemplate(l_Reward->reward[1].questId);
            }
        }

        l_Data.WriteBits(0, 21); // Unk count
        l_Data.WriteBits(l_QRew ? l_QRew->GetRewCurrencyCount() : 0, 21);
        l_Data.WriteBits(0, 19); // Unk count 2 - related to call to Arms

        l_Data.WriteBit(!l_Done);
        l_Data.WriteBit((time(nullptr) - GetPlayer()->m_bonusFactionLastTime) >= DAY); // reputation reward
        l_Data.WriteBits(l_QRew ? l_QRew->GetRewItemsCount() : 0, 20);
    }

    l_Data.WriteBits(l_Lock.size(), 20);

    for (LfgDungeonSet::const_iterator l_It = l_RandomDungeons.begin(); l_It != l_RandomDungeons.end(); ++l_It)
    {
        LfgReward const* l_Reward = sLFGMgr->GetRandomDungeonReward(*l_It, l_Level);
        Quest const* l_QRew = NULL;
        uint8 l_Done = 0;
        if (l_Reward)
        {
            l_QRew = sObjectMgr->GetQuestTemplate(l_Reward->reward[0].questId);
            if (l_QRew)
            {
                l_Done = !GetPlayer()->CanRewardQuest(l_QRew, false);
                if (l_Done)
                    l_QRew = sObjectMgr->GetQuestTemplate(l_Reward->reward[1].questId);
            }
        }

         uint32 l_CompletedEncouterMask = 0;
         LFGDungeonEntry const* l_Dungeon = sLFGDungeonStore.LookupEntry((*l_It) & 0x00FFFFFF);
         DungeonEncounterList const* l_Encounters = sObjectMgr->GetDungeonEncounterList(l_Dungeon ? l_Dungeon->map : 0, Difficulty(l_Dungeon->difficulty));
         if (l_Encounters && l_Encounters->size())
         {
             for (DungeonEncounterList::const_iterator l_Iter = l_Encounters->begin(); l_Iter != l_Encounters->end(); ++l_Iter)
             {
                 for (uint64 l_Value : _player->GetBossLooted())
                 {
                     uint32 l_Entry = l_Value & 0xFFFFFFFF;
                     uint32 l_DisplayID = l_Value >> 32;

                     if (l_DisplayID == (*l_Iter)->dbcEntry->CreatureDisplayId)
                         l_CompletedEncouterMask |= 1 << (*l_Iter)->dbcEntry->encounterIndex;
                 }
             }
         }

        l_Data << uint32(0); // 11
        l_Data << uint32(0); // 12

        l_Data << uint32(0); // 15
        l_Data << uint32(0); // 16
        l_Data << uint32(0); // 17

        if (l_QRew)
        {
            if (l_QRew->GetRewItemsCount())
            {
                ItemTemplate const* l_IProto = NULL;
                for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
                {
                    if (!l_QRew->RewardItemId[i])
                        continue;

                    l_IProto = sObjectMgr->GetItemTemplate(l_QRew->RewardItemId[i]);

                    l_Data << uint32(l_IProto ? l_IProto->DisplayInfoID : 0);
                    l_Data << uint32(l_QRew->RewardItemIdCount[i]);
                    l_Data << uint32(l_QRew->RewardItemId[i]);
                }
            }

            if (l_QRew->GetRewCurrencyCount())
            {
                for (uint8 l_I = 0; l_I < QUEST_REWARDS_COUNT; ++l_I)
                {
                    if (!l_QRew->RewardCurrencyId[l_I])
                        continue;

                    l_Data << uint32(l_QRew->RewardCurrencyId[l_I]);
                    l_Data << uint32(l_QRew->RewardCurrencyCount[l_I]);
                }
            }

            l_Data << uint32(1); // 23
            l_Data << uint32(l_CompletedEncouterMask);
            l_Data << uint32(0); // 25
            l_Data << uint32(1); // 26
            l_Data << uint32(l_QRew->XPValue(GetPlayer()) * l_QuestXpRate);
            l_Data << uint32(0); // 28
            l_Data << uint32(0); // 29
            l_Data << uint32(0); // 30
            l_Data << uint32(0); // 31
            l_Data << uint32(0); // 32
            l_Data << uint32(l_QRew->GetRewOrReqMoney());
            l_Data << uint32(*l_It);

        }
        else
        {
            l_Data << uint32(0); // 23
            l_Data << uint32(0); // 24
            l_Data << uint32(0); // 25
            l_Data << uint32(0); // 26
            l_Data << uint32(0); // 27
            l_Data << uint32(0); // 28
            l_Data << uint32(0); // 29
            l_Data << uint32(0); // 30
            l_Data << uint32(0); // 31
            l_Data << uint32(0); // 32
            l_Data << uint32(0); // 33
            l_Data << uint32(0); // 34
        }
    }

    if (l_HasGuid)
    {
        uint8 byteOrder[8] = { 6, 3, 0, 4, 5, 1, 2, 7 };
        l_Data.WriteBytesSeq(l_Guid, byteOrder);
    }

    for (LfgLockMap::const_iterator l_It = l_Lock.begin(); l_It != l_Lock.end(); ++l_It)
    {
        auto lockData = l_It->second;
        l_Data << uint32(l_It->first);                         // Dungeon entry (id + type)
        l_Data << uint32(lockData.itemLevel);
        l_Data << uint32(lockData.lockstatus);              // Lock status
        l_Data << uint32(GetPlayer()->GetAverageItemLevel());
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleLfrSearchOpcode(WorldPacket& recvData)
{
    uint32 entry;                                          // Raid id to search
    recvData >> entry;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SEARCH_LFG_JOIN [" UI64FMTD "] dungeon entry: %u", GetPlayer()->GetGUID(), entry);
    //SendLfrUpdateListOpcode(entry);
}

void WorldSession::HandleLfrLeaveOpcode(WorldPacket& recvData)
{
    uint32 dungeonId;                                      // Raid id queue to leave
    recvData >> dungeonId;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SEARCH_LFG_LEAVE [" UI64FMTD "] dungeonId: %u", GetPlayer()->GetGUID(), dungeonId);
    //sLFGMgr->LeaveLfr(GetPlayer(), dungeonId);
}
void WorldSession::HandleLfgGetStatus(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_LFG, "CMSG_LFG_GET_STATUS %s", GetPlayer()->GetGUID());

    uint64 guid = GetPlayer()->GetGUID();
    LfgUpdateData updateData = sLFGMgr->GetLfgStatus(guid);

    if (GetPlayer()->GetGroup())
    {
        sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);
        updateData.dungeons.clear();
        sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);
    }
    else
    {
        sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);
        updateData.dungeons.clear();
        sLFGMgr->SendUpdateStatus(GetPlayer(), updateData);
    }
}

void WorldSession::SendLfgRoleChosen(ObjectGuid guid, uint8 roles)
{
    uint8 bitOrder[8] = { 5, 3, 1, 0, 7, 6, 2, 4 };

    WorldPacket data(SMSG_LFG_ROLE_CHOSEN, 12);

    data.WriteBit(roles > 0);
    data.WriteBitInOrder(guid, bitOrder);
    data.WriteByteSeq(guid[0]);
    data << uint32(roles);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[4]);

    SendPacket(&data);
}

void WorldSession::SendLfgRoleCheckUpdate(const LfgRoleCheck* pRoleCheck)
{
    ASSERT(pRoleCheck);
    LfgDungeonSet dungeons;
    if (pRoleCheck->rDungeonId)
        dungeons.insert(pRoleCheck->rDungeonId);
    else
        dungeons = pRoleCheck->dungeons;

    ObjectGuid unkGuid = 0;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_ROLE_CHECK_UPDATE [" UI64FMTD "]", GetPlayer()->GetGUID());
    
    WorldPacket data(SMSG_LFG_ROLE_CHECK_UPDATE, 4 + 1 + 1 + dungeons.size() * 4 + 1 + pRoleCheck->roles.size() * (8 + 1 + 4 + 1));
    ByteBuffer dataBuffer;

    data.WriteBit(unkGuid[0]);
    data.WriteBit(unkGuid[1]);
    data.WriteBit(unkGuid[6]);
    data.WriteBits(pRoleCheck->roles.size(), 21);
    if (!pRoleCheck->roles.empty())
    {
        ObjectGuid guid = pRoleCheck->leader;
        uint8 roles = pRoleCheck->roles.find(guid)->second;
        Player* player = ObjectAccessor::FindPlayer(guid);

        data.WriteBit(guid[4]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[2]);
        data.WriteBit(guid[6]);
        data.WriteBit(roles > 0);
        data.WriteBit(guid[5]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[3]);

        dataBuffer << uint32(roles);                                   // Roles
        dataBuffer.WriteByteSeq(guid[0]);
        dataBuffer.WriteByteSeq(guid[2]);
        dataBuffer.WriteByteSeq(guid[5]);
        dataBuffer.WriteByteSeq(guid[4]);
        dataBuffer.WriteByteSeq(guid[7]);
        dataBuffer.WriteByteSeq(guid[6]);
        dataBuffer.WriteByteSeq(guid[1]);
        dataBuffer.WriteByteSeq(guid[3]);
        dataBuffer << uint8(player ? player->getLevel() : 0);          // Level

        for (LfgRolesMap::const_reverse_iterator it = pRoleCheck->roles.rbegin(); it != pRoleCheck->roles.rend(); ++it)
        {
            if (it->first == pRoleCheck->leader)
                continue;

            guid = it->first;
            roles = it->second;
            player = ObjectAccessor::FindPlayer(guid);

            data.WriteBit(guid[4]);
            data.WriteBit(guid[1]);
            data.WriteBit(guid[2]);
            data.WriteBit(guid[6]);
            data.WriteBit(roles > 0);
            data.WriteBit(guid[5]);
            data.WriteBit(guid[7]);
            data.WriteBit(guid[0]);
            data.WriteBit(guid[3]);

            dataBuffer << uint32(roles);                                   // Roles
            dataBuffer.WriteByteSeq(guid[0]);
            dataBuffer.WriteByteSeq(guid[2]);
            dataBuffer.WriteByteSeq(guid[5]);
            dataBuffer.WriteByteSeq(guid[4]);
            dataBuffer.WriteByteSeq(guid[7]);
            dataBuffer.WriteByteSeq(guid[6]);
            dataBuffer.WriteByteSeq(guid[1]);
            dataBuffer.WriteByteSeq(guid[3]);
            dataBuffer << uint8(player ? player->getLevel() : 0);          // Level
        }
    }

    data.WriteBit(unkGuid[7]);
    data.WriteBits(dungeons.size(), 22);
    data.WriteBit(pRoleCheck->state == LFG_ROLECHECK_INITIALITING);
    data.WriteBit(unkGuid[3]);
    data.WriteBit(unkGuid[5]);
    data.WriteBit(unkGuid[4]);
    data.WriteBit(unkGuid[2]);
    data.append(dataBuffer);
    data.WriteByteSeq(unkGuid[4]);
    if (!dungeons.empty())
    {
        for (LfgDungeonSet::iterator it = dungeons.begin(); it != dungeons.end(); ++it)
        {
            LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(*it);
            data << uint32(dungeon ? dungeon->Entry() : 0); // Dungeon
        }
    }
    data.WriteByteSeq(unkGuid[6]);
    data.WriteByteSeq(unkGuid[7]);
    data.WriteByteSeq(unkGuid[0]);
    data.WriteByteSeq(unkGuid[3]);
    data << uint8(1);
    data.WriteByteSeq(unkGuid[5]);
    data << uint8(pRoleCheck->state);
    data.WriteByteSeq(unkGuid[2]);
    data.WriteByteSeq(unkGuid[1]);

    SendPacket(&data);
}

void WorldSession::SendLfgJoinResult(uint64 guid_, const LfgJoinResultData& joinData)
{
    ObjectGuid guid = guid_;

    uint32 size = 0;
    for (LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
        size += 8 + 4 + uint32(it->second.size()) * (4 + 4 + 4 + 4);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_JOIN_RESULT [" UI64FMTD "] checkResult: %u checkValue: %u", GetPlayer()->GetGUID(), joinData.result, joinData.state);

    WorldPacket data(SMSG_LFG_JOIN_RESULT, 4 + 4 + size);

    data << uint32(3);                                    // Unk
    data << uint8(joinData.result);                       // Check Result
    data << uint8(joinData.state);                        // Check Value
    data << uint32(getMSTime());                          // Time
    data << uint32(0);                                    // Queue Id

    data.WriteBit(guid[4]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[1]);
    data.WriteBits(joinData.lockmap.size(), 22);
    data.WriteBit(guid[3]);

    for (LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
    {
        ObjectGuid guid1 = it->first;

        data.WriteBit(guid1[7]);
        data.WriteBit(guid1[0]);
        data.WriteBit(guid1[1]);
        data.WriteBit(guid1[6]);
        data.WriteBit(guid1[2]);
        data.WriteBit(guid1[4]);
        data.WriteBits(it->second.size(), 20);
        data.WriteBit(guid1[3]);
        data.WriteBit(guid1[5]);
    }

    data.WriteBit(guid[5]);
    data.WriteByteSeq(guid[5]);

    for (LfgLockPartyMap::const_iterator it = joinData.lockmap.begin(); it != joinData.lockmap.end(); ++it)
    {
        LfgLockMap second = it->second;
        for (LfgLockMap::const_iterator itr = second.begin(); itr != second.end(); ++itr)
        {
            auto lockData = itr->second;
            data << uint32(lockData.itemLevel);                 // Lock status
            data << uint32(lockData.lockstatus);
            data << uint32(itr->first);                         // Dungeon entry (id + type)
            data << uint32(GetPlayer()->GetAverageItemLevel());
        }

        ObjectGuid guid1 = it->first;

        uint8 byteOrder[8] = {1, 4, 7, 6, 3, 2, 0, 5};
        data.WriteBytesSeq(guid1, byteOrder);
    }

    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[1]);

    SendPacket(&data);
}

void WorldSession::SendLfgQueueStatus(uint32 dungeon, int32 waitTime, int32 avgWaitTime, int32 waitTimeTanks, int32 waitTimeHealer, int32 waitTimeDps, uint32 queuedTime, uint8 tanks, uint8 healers, uint8 dps)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_QUEUE_STATUS [" UI64FMTD "] dungeon: %u - waitTime: %d - avgWaitTime: %d - waitTimeTanks: %d - waitTimeHealer: %d - waitTimeDps: %d - queuedTime: %u - tanks: %u - healers: %u - dps: %u", GetPlayer()->GetGUID(), dungeon, waitTime, avgWaitTime, waitTimeTanks, waitTimeHealer, waitTimeDps, queuedTime, tanks, healers, dps);
    ObjectGuid guid = GetPlayer()->GetGUID();

    LfgQueueInfo* info = sLFGMgr->GetLfgQueueInfo(GetPlayer()->GetGroup() ? GetPlayer()->GetGroup()->GetGUID() : GetPlayer()->GetGUID());
    if (!info)
        return;

    WorldPacket data(SMSG_LFG_QUEUE_STATUS, 4 + 4 + 4 + 4 + 4 +4 + 1 + 1 + 1 + 4);
    data << uint32(info->joinTime);
    data << uint32(0);                                      // queue id
    data << int32(waitTime);
    data << uint32(dungeon);
    data << uint8(tanks);                                  // Tanks needed
    data << int32(waitTimeTanks);                          // Wait Tanks
    data << uint8(healers);                                // Healers needed
    data << int32(waitTimeHealer);                         // Wait Healers
    data << uint8(dps);                                    // Dps needed
    data << int32(waitTimeDps);                            // Wait Dps
    data << uint32(3);                                     // Some Flags
    data << int32(queuedTime);
    data << uint32(avgWaitTime);
   
    uint8 bitOrder[8] = { 2, 0, 6, 5, 1, 4, 7, 3 };
    data.WriteBitInOrder(guid, bitOrder);

    uint8 byteOrder[8] = { 6, 1, 2, 4, 7, 3, 5, 0 };
    data.WriteBytesSeq(guid, byteOrder);
    
    SendPacket(&data);
}

void WorldSession::SendLfgPlayerReward(uint32 rdungeonEntry, uint32 sdungeonEntry, uint8 done, const LfgReward* reward, const Quest* qRew)
{
    if (!rdungeonEntry || !sdungeonEntry || !qRew)
        return;

    uint8 itemNum = uint8(qRew ? qRew->GetRewItemsCount() + qRew->GetRewCurrencyCount()  : 0);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_PLAYER_REWARD [" UI64FMTD "] rdungeonEntry: %u - sdungeonEntry: %u - done: %u", GetPlayer()->GetGUID(), rdungeonEntry, sdungeonEntry, done);

    ByteBuffer bytereward;
    WorldPacket data(SMSG_LFG_PLAYER_REWARD, 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 1 + itemNum * (4 + 4 + 4));
    
    data.WriteBits(itemNum, 20);

    if (qRew && qRew->GetRewItemsCount())
    {
        ItemTemplate const* iProto = NULL;
        for (uint8 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        {
            if (!qRew->RewardItemId[i])
                continue;

            data.WriteBit(0);

            iProto = sObjectMgr->GetItemTemplate(qRew->RewardItemId[i]);

            bytereward << uint32(iProto ? iProto->DisplayInfoID : 0);
            bytereward << uint32(qRew->RewardItemIdCount[i]);
            bytereward << uint32(0);
            bytereward << uint32(qRew->RewardItemId[i]);
        }
    }
    if (qRew && qRew->GetRewCurrencyCount())
    {
        for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        {
            if (!qRew->RewardCurrencyId[i])
                continue;

            data.WriteBit(1);

            bytereward << uint32(0);
            bytereward << uint32(qRew->RewardCurrencyCount[i]);
            bytereward << uint32(0);
            bytereward << uint32(qRew->RewardCurrencyId[i]);
        }
    }
    data.append(bytereward);

    float QuestXpRate = GetPlayer()->GetRate<float>(RateType::XpQuest);
    if (float personalRate = GetPlayer()->GetPersonnalXpRate())
        QuestXpRate = personalRate;

    uint32 XP = qRew->XPValue(GetPlayer()) * QuestXpRate;
    if (IsPremium())
        XP *= sWorld->getRate(RATE_XP_QUEST_PREMIUM);

    data << uint32(XP);
    data << uint32(rdungeonEntry);                         // Random Dungeon Finished
    data << uint32(sdungeonEntry);                         // Dungeon Finished
    data << uint32(qRew->GetRewOrReqMoney());
    

    SendPacket(&data);
}

void WorldSession::SendLfgBootPlayer(const LfgPlayerBoot* pBoot)
{
    uint64 guid = GetPlayer()->GetGUID();
    LfgAnswer playerVote = pBoot->votes.find(guid)->second;
    uint8 votesNum = 0;
    uint8 agreeNum = 0;
    uint32 secsleft = uint8((pBoot->cancelTime - time(NULL)) / 1000);
    for (LfgAnswerMap::const_iterator it = pBoot->votes.begin(); it != pBoot->votes.end(); ++it)
    {
        if (it->second != LFG_ANSWER_PENDING)
        {
            ++votesNum;
            if (it->second == LFG_ANSWER_AGREE)
                ++agreeNum;
        }
    }
    
    ObjectGuid victimGuid = ObjectGuid(pBoot->victim);
    
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_BOOT_PROPOSAL_UPDATE [" UI64FMTD "] inProgress: %u - didVote: %u - agree: %u - victim: [" UI64FMTD "] votes: %u - agrees: %u - left: %u - needed: %u - reason %s",
        guid, uint8(pBoot->inProgress), uint8(playerVote != LFG_ANSWER_PENDING), uint8(playerVote == LFG_ANSWER_AGREE), pBoot->victim, votesNum, agreeNum, secsleft, pBoot->votedNeeded, pBoot->reason.c_str());
    WorldPacket data(SMSG_LFG_BOOT_PROPOSAL_UPDATE, 1 + 1 + 1 + 8 + 4 + 4 + 4 + 4 + pBoot->reason.length());

    data.WriteBit(victimGuid[2]);
    data.WriteBit(victimGuid[3]);
    data.WriteBit(false);
    data.WriteBit(pBoot->inProgress);                      // Vote in progress
    data.WriteBit(playerVote != LFG_ANSWER_PENDING);       // Did Vote
    data.WriteBit(victimGuid[7]);
    data.WriteBit(victimGuid[0]);
    data.WriteBit(victimGuid[5]);
    data.WriteBit(pBoot->reason.empty());
    data.WriteBit(victimGuid[4]);
    data.WriteBit(victimGuid[1]);
    data.WriteBit(victimGuid[6]);
    data.WriteBit(playerVote == LFG_ANSWER_AGREE);         // Agree
    
    if (!pBoot->reason.empty())
        data.WriteBits(pBoot->reason.size(), 8);

    data.WriteString(pBoot->reason);                       // Kick reason
    
    data.WriteByteSeq(victimGuid[3]);
    data.WriteByteSeq(victimGuid[0]);
    data.WriteByteSeq(victimGuid[6]);
    data.WriteByteSeq(victimGuid[1]);
    data << uint32(secsleft);                              // Time Left
    data << uint32(pBoot->votedNeeded);                    // Needed Votes
    data.WriteByteSeq(victimGuid[2]);
    data << uint32(votesNum);                              // Total Votes
    data.WriteByteSeq(victimGuid[7]);
    data.WriteByteSeq(victimGuid[5]);
    data.WriteByteSeq(victimGuid[4]);
    data << uint32(agreeNum);                              // Agree Count
    SendPacket(&data);
}

void WorldSession::SendLfgUpdateProposal(uint32 p_ProposalId, const LfgProposal* p_Proposal)
{
    if (!p_Proposal)
        return;

    uint64 l_Guid = GetPlayer()->GetGUID();
    LfgProposalPlayerMap::const_iterator l_Player = p_Proposal->players.find(l_Guid);
    if (l_Player == p_Proposal->players.end())                  // Player MUST be in the proposal
         return;

    LfgProposalPlayer* l_ProposalPlayer = l_Player->second;
    uint32 l_PlayerProposalGroupGUID    = l_ProposalPlayer->groupLowGuid;
    uint32 l_GroupProposalGroupGUID     = p_Proposal->groupLowGuid;
    uint32 l_DungeonId                  = p_Proposal->dungeonId;

    bool l_CompletedMask = false;
    bool l_Silent        = false;

    Group* l_Group = l_GroupProposalGroupGUID ? sGroupMgr->GetGroupByGUID(l_GroupProposalGroupGUID) : NULL;
    uint32 l_CompletedEncounters = 0;

    if (l_Group)
    {
        uint64 l_GroupGUID = l_Group->GetGUID();
        l_Silent = l_Group->isLFGGroup() && sLFGMgr->GetState(l_GroupGUID) != LFG_STATE_FINISHED_DUNGEON && _player->GetMap()->IsDungeon();
    }

    WorldPacket l_Data(SMSG_LFG_PROPOSAL_UPDATE, 4 + 1 + 4 + 4 + 1 + 1 + p_Proposal->players.size() * (4 + 1 + 1 + 1 + 1 +1));

    if (!l_Silent)                                       // Only show proposal dungeon if it's continue
    {
        LfgDungeonSet l_PlayerDungeons = sLFGMgr->GetSelectedDungeons(l_Guid);
        if (l_PlayerDungeons.size() == 1)
            l_DungeonId = (*l_PlayerDungeons.begin());
    }

    if (LFGDungeonEntry const* l_Dungeon = sLFGDungeonStore.LookupEntry(l_DungeonId))
    {
        l_DungeonId = l_Dungeon->Entry();

        // Select a player inside to be get completed encounters from
        if (l_Group)
        {
            for (GroupReference* l_Itr = l_Group->GetFirstMember(); l_Itr != NULL; l_Itr = l_Itr->next())
            {
                Player* l_GroupMember = l_Itr->getSource();
                if (l_GroupMember && l_GroupMember->GetMapId() == uint32(l_Dungeon->map))
                {
                    if (InstanceScript* l_InstanceScript = l_GroupMember->GetInstanceScript())
                    {
                        l_CompletedEncounters = l_InstanceScript->GetCompletedEncounterMask();
                        l_CompletedMask = true;
                    }
                    break;
                }
            }
        }
    }

    ObjectGuid l_PlayerGUID = l_Guid;
    ObjectGuid l_InstanceSaveGUID = MAKE_NEW_GUID(l_DungeonId, 0, HighGuid::Instance);

    l_Data << uint32(0);
    l_Data << uint32(l_DungeonId);
    l_Data << uint32(l_CompletedEncounters);
    l_Data << uint32(0x03);
    l_Data << uint32(p_ProposalId);
    l_Data << uint32(getMSTime());
    l_Data << uint8(p_Proposal->state);

    l_Data.WriteBit(l_PlayerGUID[4]);
    l_Data.WriteBit(l_CompletedMask);
    l_Data.WriteBit(l_PlayerGUID[2]);
    l_Data.WriteBit(l_PlayerGUID[0]);
    l_Data.WriteBits(p_Proposal->players.size(), 21);

    for (l_Player = p_Proposal->players.begin(); l_Player != p_Proposal->players.end(); ++l_Player)
    {
        bool l_InDungeon = false;
        bool l_InSameGroup = false;

        if (l_Player->second->groupLowGuid)
        {
            l_InDungeon = l_Player->second->groupLowGuid == l_GroupProposalGroupGUID;
            l_InSameGroup = l_Player->second->groupLowGuid == l_PlayerProposalGroupGUID;
        }

        l_Data.WriteBit(l_Player->first == l_Guid);                         // Self player
        l_Data.WriteBit(l_Player->second->accept != LFG_ANSWER_PENDING);   // Answered
        l_Data.WriteBit(l_InDungeon);                                       // In dungeon (silent)
        l_Data.WriteBit(l_InSameGroup);                                     // Same Group than player
        l_Data.WriteBit(l_Player->second->accept == LFG_ANSWER_AGREE);    // Accepted
    }
    l_Data.WriteBit(l_PlayerGUID[5]);
    l_Data.WriteBit(l_InstanceSaveGUID[7]);
    l_Data.WriteBit(l_InstanceSaveGUID[4]);
    l_Data.WriteBit(l_PlayerGUID[1]);
    l_Data.WriteBit(l_InstanceSaveGUID[6]);
    l_Data.WriteBit(l_InstanceSaveGUID[0]);
    l_Data.WriteBit(l_InstanceSaveGUID[1]);
    l_Data.WriteBit(l_PlayerGUID[6]);
    l_Data.WriteBit(l_PlayerGUID[3]);
    l_Data.WriteBit(l_InstanceSaveGUID[3]);
    l_Data.WriteBit(l_InstanceSaveGUID[2]);
    l_Data.WriteBit(l_Silent);
    l_Data.WriteBit(l_PlayerGUID[7]);
    l_Data.WriteBit(l_InstanceSaveGUID[5]);

    l_Data.WriteByteSeq(l_InstanceSaveGUID[3]);
    l_Data.WriteByteSeq(l_InstanceSaveGUID[0]);
    l_Data.WriteByteSeq(l_PlayerGUID[2]);
    l_Data.WriteByteSeq(l_PlayerGUID[3]);
    l_Data.WriteByteSeq(l_PlayerGUID[1]);
    l_Data.WriteByteSeq(l_PlayerGUID[4]);
    l_Data.WriteByteSeq(l_InstanceSaveGUID[6]);
    l_Data.WriteByteSeq(l_InstanceSaveGUID[4]);
    l_Data.WriteByteSeq(l_InstanceSaveGUID[1]);
    l_Data.WriteByteSeq(l_InstanceSaveGUID[5]);
    l_Data.WriteByteSeq(l_PlayerGUID[6]);

    for (l_Player = p_Proposal->players.begin(); l_Player != p_Proposal->players.end(); ++l_Player)
        l_Data << uint32(l_Player->second->role);

    l_Data.WriteByteSeq(l_InstanceSaveGUID[2]);
    l_Data.WriteByteSeq(l_PlayerGUID[7]);
    l_Data.WriteByteSeq(l_PlayerGUID[0]);
    l_Data.WriteByteSeq(l_InstanceSaveGUID[7]);
    l_Data.WriteByteSeq(l_PlayerGUID[5]);

    SendPacket(&l_Data);
}

void WorldSession::SendLfgUpdateSearch(bool update)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_UPDATE_SEARCH [" UI64FMTD "] update: %u", GetPlayer()->GetGUID(), update ? 1 : 0);
    WorldPacket data(SMSG_LFG_UPDATE_SEARCH, 1);
    data << uint8(update);                                 // In Lfg Queue?
    SendPacket(&data);
}

void WorldSession::SendLfgDisabled()
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_DISABLED [" UI64FMTD "]", GetPlayer()->GetGUID());
    WorldPacket data(SMSG_LFG_DISABLED, 0);
    SendPacket(&data);
}

void WorldSession::SendLfgOfferContinue(uint32 dungeonEntry)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_OFFER_CONTINUE [" UI64FMTD "] dungeon entry: %u", GetPlayer()->GetGUID(), dungeonEntry);
    WorldPacket data(SMSG_LFG_OFFER_CONTINUE, 4);
    data << uint32(dungeonEntry);
    SendPacket(&data);
}

void WorldSession::SendLfgTeleportError(uint8 err)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_LFG_TELEPORT_DENIED [" UI64FMTD "] reason: %u", GetPlayer()->GetGUID(), err);
    WorldPacket data(SMSG_LFG_TELEPORT_DENIED, 4);
    //Not sure it is no 4bits.
    data.WriteBits(err, 4);                                   // Error
    SendPacket(&data);
}

/*
void WorldSession::SendLfrUpdateListOpcode(uint32 dungeonEntry)
{
    sLog->outDebug(LOG_FILTER_PACKETIO, "SMSG_LFG_UPDATE_LIST [" UI64FMTD "] dungeon entry: %u", GetPlayer()->GetGUID(), dungeonEntry);
    WorldPacket data(SMSG_LFG_UPDATE_LIST);
    SendPacket(&data);
}
*/

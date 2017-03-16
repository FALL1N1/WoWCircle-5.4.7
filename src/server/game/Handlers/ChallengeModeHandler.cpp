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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include <list>
#include <vector>
#include <utility>

void WorldSession::HandleGetChallengeModeRewards(WorldPacket& /*p_RecvData*/)
{
    WorldPacket l_Data(Opcodes::SMSG_CHALLENGE_MODE_REWARDS);
    ChallengeRewardsMap l_ChallengeRewards = sObjectMgr->GetChallengeRewards();
    ByteBuffer l_Buffer;

    l_Data.WriteBits(l_ChallengeRewards.size(), 20);
    l_Data.WriteBits(0, 21);   ///< TierRewards

    /// RewardsCount
    {
        for (auto l_Reward : l_ChallengeRewards)
        {
            uint32 l_RewardsPerMedalCount = 4;
            l_Data.WriteBits(l_RewardsPerMedalCount, 20);

            ChallengeReward l_ChallengeReward = l_Reward.second;
            for (uint32 l_J = 0; l_J < l_RewardsPerMedalCount; ++l_J)
            {
                uint32 l_ItemRewards = 0;
                uint32 l_CurrencyRewards = 0;

                l_Data.WriteBits(l_ItemRewards, 21);
                l_Data.WriteBits(l_CurrencyRewards, 20);

                l_Buffer << uint32(l_ChallengeReward.MoneyReward[l_J]);
            }
        }
    }

    if (l_Buffer.size())
        l_Data.append(l_Buffer);

    /// Anything else is needed to be sent
    SendPacket(&l_Data);
}

void WorldSession::HandleChallengeModeRequestLeaders(WorldPacket& p_RecvData)
{
    uint32 l_MapID = p_RecvData.read<uint32>();
    p_RecvData.read_skip<uint32>(); ///< Last Guild update
    p_RecvData.read_skip<uint32>(); ///< Last Realm update

    RealmCompletedChallenge* l_GroupChallenge = sObjectMgr->GetGroupCompletedChallengeForMap(l_MapID);
    RealmCompletedChallenge* l_GuildChallenge = sObjectMgr->GetGuildCompletedChallengeForMap(l_MapID);

    WorldPacket l_Data(Opcodes::SMSG_CHALLENGE_MODE_REQUEST_LEADERS_RESULT);

    uint32 l_RealmLeaders = l_GroupChallenge != nullptr;
    uint32 l_GuildLeaders = 0;

    if (l_GuildChallenge != nullptr && l_GuildChallenge->m_GuildID == _player->GetGuildId())
        l_GuildLeaders = 1;

    if (l_GuildChallenge != nullptr && l_GroupChallenge == nullptr)
        l_GroupChallenge = l_GuildChallenge;

    l_Data.WriteBits(l_RealmLeaders, 19);

    /// RealmLeaders
    ByteBuffer l_RealmBuffer;

    if (l_RealmLeaders > 0)
    {
        uint32 l_Members = l_GroupChallenge->m_MembersCount;
        l_Data.WriteBits(l_Members, 20);

        for (uint32 l_J = 0; l_J < l_Members; ++l_J)
        {
            ObjectGuid l_Guid = l_GroupChallenge->m_Members[l_J].m_Guid;

            uint8 l_BitsOrder[8] = { 4, 6, 0, 1, 2, 5, 7, 3 };
            l_Data.WriteBitInOrder(l_Guid, l_BitsOrder);

            l_RealmBuffer.WriteByteSeq(l_Guid[4]);
            l_RealmBuffer.WriteByteSeq(l_Guid[0]);
            l_RealmBuffer.WriteByteSeq(l_Guid[7]);
            l_RealmBuffer.WriteByteSeq(l_Guid[2]);
            l_RealmBuffer << uint32(realmID);
            l_RealmBuffer.WriteByteSeq(l_Guid[6]);
            l_RealmBuffer << uint32(l_GroupChallenge->m_Members[l_J].m_SpecID);
            l_RealmBuffer.WriteByteSeq(l_Guid[3]);
            l_RealmBuffer << uint32(realmID);
            l_RealmBuffer.WriteByteSeq(l_Guid[1]);
            l_RealmBuffer.WriteByteSeq(l_Guid[5]);
        }

        l_RealmBuffer << uint32(l_GroupChallenge->m_CompletionTime);
        l_RealmBuffer << uint32(secsToTimeBitFields(l_GroupChallenge->m_CompletionDate));
        l_RealmBuffer << uint32(l_GroupChallenge->m_AttemptID);
        l_RealmBuffer << uint32(realmID);
        l_RealmBuffer << uint32(l_GroupChallenge->m_MedalEarned);
    }

    l_Data.WriteBits(l_GuildLeaders, 19);

    /// GuildLeaders
    ByteBuffer l_GuildBuffer;

    if (l_GuildLeaders > 0)
    {
        uint32 l_Members = l_GuildChallenge->m_MembersCount;
        l_Data.WriteBits(l_Members, 20);

        for (uint32 l_J = 0; l_J < l_Members; ++l_J)
        {
            ObjectGuid l_Guid = l_GuildChallenge->m_Members[l_J].m_Guid;

            uint8 l_BitsOrder[8] = { 0, 2, 6, 4, 3, 5, 7, 1 };
            l_Data.WriteBitInOrder(l_Guid, l_BitsOrder);

            l_GuildBuffer.WriteByteSeq(l_Guid[6]);
            l_GuildBuffer.WriteByteSeq(l_Guid[5]);
            l_GuildBuffer << uint32(realmID);
            l_GuildBuffer.WriteByteSeq(l_Guid[4]);
            l_GuildBuffer.WriteByteSeq(l_Guid[2]);
            l_GuildBuffer.WriteByteSeq(l_Guid[7]);
            l_GuildBuffer << uint32(l_GuildChallenge->m_Members[l_J].m_SpecID);
            l_GuildBuffer << uint32(realmID);
            l_GuildBuffer.WriteByteSeq(l_Guid[1]);
            l_GuildBuffer.WriteByteSeq(l_Guid[3]);
            l_GuildBuffer.WriteByteSeq(l_Guid[0]);
        }

        l_GuildBuffer << uint32(realmID);
        l_GuildBuffer << uint32(l_GuildChallenge->m_AttemptID);
        l_GuildBuffer << uint32(l_GuildChallenge->m_MedalEarned);
        l_GuildBuffer << uint32(secsToTimeBitFields(l_GuildChallenge->m_CompletionDate));
        l_GuildBuffer << uint32(l_GuildChallenge->m_CompletionTime);
    }

    if (l_GuildBuffer.size())
        l_Data.append(l_GuildBuffer);

    if (l_RealmBuffer.size())
        l_Data.append(l_RealmBuffer);

    l_Data << uint32(time(nullptr));
    l_Data << uint32(l_MapID);
    l_Data << uint32(time(nullptr));

    SendPacket(&l_Data);
}

void WorldSession::HandleChallengeModeRequestMapStats(WorldPacket& /*p_RecvData*/)
{
    uint32 l_MapsCount = _player->m_CompletedChallenges.size();

    WorldPacket l_Data(Opcodes::SMSG_CHALLENGE_MODE_ALL_MAP_STATS);
    ByteBuffer l_DataBuffer;

    l_Data.WriteBits(l_MapsCount, 19);

    /// MapData
    if (l_MapsCount > 0)
    {
        for (auto l_ChallengeData : _player->m_CompletedChallenges)
        {
            CompletedChallenge l_CompletedChallenge = l_ChallengeData.second;

            l_DataBuffer << int32(l_CompletedChallenge.m_BestMedal);
            l_DataBuffer << int32(l_ChallengeData.first);

            uint32 l_SpecCount = 0;
            l_Data.WriteBits(l_SpecCount, 23);

            /// BestSpecs - Useless part ?
            {
                for (uint32 l_J = 0; l_J < l_SpecCount; ++l_J)
                    l_DataBuffer << uint16(0);    ///< SpecID
            }

            l_DataBuffer << int32(l_CompletedChallenge.m_BestTime);
            l_DataBuffer << int32(l_CompletedChallenge.m_LastTime);
            l_DataBuffer << uint32(secsToTimeBitFields(l_CompletedChallenge.m_BestMedalDate));
        }
    }

    if (l_DataBuffer.size())
        l_Data.append(l_DataBuffer);

    SendPacket(&l_Data);
}

void WorldSession::SendMapChallengeModeDBReply(uint32 p_Entry)
{
    MapChallengeModeHotfix* l_HotFix = sObjectMgr->GetMapChallengeModeHotfix(p_Entry);
    if (l_HotFix == nullptr)
        return;

    WorldPacket l_Data(Opcodes::SMSG_DB_REPLY, 56);
    l_Data << int32(p_Entry);
    l_Data << uint32(DB2Types::DB2_REPLY_MAP_CHALLENGE_MODE);
    l_Data << uint32(time(NULL));
    l_Data << uint32(40);   ///< Size of MapChallengeMode.db2

    /// MapChallengeMode.db2
    {
        l_Data << uint32(l_HotFix->m_ID);
        l_Data << uint32(l_HotFix->m_MapID);
        l_Data << uint32(l_HotFix->m_Field2);
        l_Data << uint32(l_HotFix->m_Field3);
        l_Data << uint32(l_HotFix->m_Field4);
        l_Data << uint32(l_HotFix->m_BronzeTime);
        l_Data << uint32(l_HotFix->m_SilverTime);
        l_Data << uint32(l_HotFix->m_GoldTime);
        l_Data << uint32(l_HotFix->m_Field8);
        l_Data << uint32(l_HotFix->m_Field9);
    }

    SendPacket(&l_Data);
}

void WorldSession::SendChallengeModeMapStatsUpdate(uint32 p_MapID)
{
    WorldPacket l_Data(Opcodes::SMSG_CHALLENGE_MODE_MAP_STATS_UPDATE);

    for (auto l_ChallengeData : _player->m_CompletedChallenges)
    {
        if (l_ChallengeData.first == p_MapID)
        {
            CompletedChallenge l_CompletedChallenge = l_ChallengeData.second;

            l_Data << int32(l_CompletedChallenge.m_BestMedal);
            l_Data << int32(l_CompletedChallenge.m_LastTime);
            l_Data << uint32(secsToTimeBitFields(l_CompletedChallenge.m_BestMedalDate));
            l_Data << int32(l_CompletedChallenge.m_BestTime);
            l_Data << int32(l_ChallengeData.first);
        }
    }

    uint32 l_SpecCount = 0;
    l_Data.WriteBits(l_SpecCount, 23);

    /// BestSpecs - Useless part ?
    {
        for (uint32 l_J = 0; l_J < l_SpecCount; ++l_J)
            l_Data << uint16(0);    ///< SpecID
    }

    SendPacket(&l_Data);
}
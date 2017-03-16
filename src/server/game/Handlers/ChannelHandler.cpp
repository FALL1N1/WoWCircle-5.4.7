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

#include "ObjectMgr.h"                                      // for normalizePlayerName
#include "ChannelMgr.h"
#include "Chat.h"

void WorldSession::HandleJoinChannel(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());

    uint32 channelId;
    uint32 channelLength = 0;
    uint32 passLength = 0;
    std::string channelName, pass;

    recvPacket >> channelId;
    passLength = recvPacket.ReadBits(7);
    uint8 unknown1 = recvPacket.ReadBit();                  // unknown bit
    channelLength = recvPacket.ReadBits(7);
    uint8 unknown2 = recvPacket.ReadBit();                  // unknown bit
    pass = recvPacket.ReadString(passLength);
    channelName = recvPacket.ReadString(channelLength);

    if (channelId)
    {
        ChatChannelsEntry const* channel = sChatChannelsStore.LookupEntry(channelId);
        if (!channel)
            return;

        AreaTableEntry const* current_zone = GetAreaEntryByAreaID(_player->GetZoneId());
        if (!current_zone)
            return;

        if (!_player->CanJoinConstantChannelInZone(channel, current_zone))
            return;
    }

    if (channelName.empty())
        return;

    if (!ChatHandler(this).isValidChatMessage(channelName.c_str()))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        cMgr->team = _player->GetTeam();
        if (Channel* chn = cMgr->GetJoinChannel(channelName, channelId))
            chn->Join(_player->GetGUID(), pass.c_str());
    }
}

void WorldSession::HandleLeaveChannel(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());

    uint32 unk;
    std::string channelname;
    recvPacket >> unk;                                      // channel id?
    uint32 length = recvPacket.ReadBits(7);
    channelname = recvPacket.ReadString(length);

    if (channelname.empty())
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->Leave(_player->GetGUID(), true);
        cMgr->LeftChannel(channelname);
    }
}

void WorldSession::HandleChannelList(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());

    uint32 length = recvPacket.ReadBits(7);
    std::string channelname = recvPacket.ReadString(length);

    if (!ChatHandler(this).isValidChatMessage(channelname.c_str()))
        return;

    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
            chn->List(_player);
}

void WorldSession::HandleChannelPassword(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelPassCommand < 5)
        return;
    else
        timeLastChannelPassCommand = l_Now;

    uint32 l_PassLen = p_RecvData.ReadBits(7);
    uint32 l_NameLen = p_RecvData.ReadBits(7);

    std::string l_Pass = p_RecvData.ReadString(l_PassLen);
    std::string l_ChannName = p_RecvData.ReadString(l_NameLen);

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->Password(_player->GetGUID(), l_Pass.c_str());
    }
}

void WorldSession::HandleChannelSetOwner(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelSetownerCommand < 5)
        return;
    else
       timeLastChannelSetownerCommand = l_Now;

    uint32 l_ChannLen = p_RecvData.ReadBits(7);
    uint32 l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();

    std::string l_PlrName = p_RecvData.ReadString(l_NameLen);
    std::string l_ChannName = p_RecvData.ReadString(l_ChannLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->SetOwner(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelOwner(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelOwnerCommand < 5)
        return;
    else
       timeLastChannelOwnerCommand = l_Now;

    uint32 l_Len = p_RecvData.ReadBits(7);
    std::string l_Name = p_RecvData.ReadString(l_Len);

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_Name, _player))
            l_Channel->SendWhoOwner(_player->GetGUID());
    }
}

void WorldSession::HandleChannelModerator(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelModerCommand < 5)
        return;
    else
       timeLastChannelModerCommand = l_Now;

    uint32 l_ChannLen = p_RecvData.ReadBits(7);
    uint32 l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();

    std::string l_ChannName = p_RecvData.ReadString(l_ChannLen);
    std::string l_PlrName = p_RecvData.ReadString(l_NameLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->SetModerator(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelUnmoderator(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelUnmoderCommand < 5)
        return;
    else
       timeLastChannelUnmoderCommand = l_Now;

    uint32 l_ChannLen = p_RecvData.ReadBits(7);
    uint32 l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();

    std::string l_PlrName = p_RecvData.ReadString(l_NameLen);
    std::string l_ChannName = p_RecvData.ReadString(l_ChannLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->UnsetModerator(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelMute(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelMuteCommand < 5)
        return;
    else
        timeLastChannelMuteCommand = l_Now;

    uint32 l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();

    uint32 l_ChannLen = p_RecvData.ReadBits(7);

    std::string l_PlrName = p_RecvData.ReadString(l_NameLen);
    std::string l_ChannName = p_RecvData.ReadString(l_ChannLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->SetMute(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelUnmute(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelUnmuteCommand < 5)
        return;
    else
       timeLastChannelUnmuteCommand = l_Now;

    uint32 l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();

    uint32 l_ChannLen = p_RecvData.ReadBits(7);

    std::string l_PlrName = p_RecvData.ReadString(l_NameLen);
    std::string l_ChannName = p_RecvData.ReadString(l_ChannLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->UnsetMute(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelInvite(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelInviteCommand < 5)
        return;
    else
       timeLastChannelInviteCommand = l_Now;

    uint32 l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();

    uint32 l_ChannLen = p_RecvData.ReadBits(7);

    std::string l_ChannName = p_RecvData.ReadString(l_ChannLen);
    std::string l_PlrName = p_RecvData.ReadString(l_NameLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (!ChatHandler(this).isValidChatMessage(l_ChannName.c_str()))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->Invite(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelKick(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelKickCommand < 5)
        return;
    else
       timeLastChannelKickCommand = l_Now;

    uint32 l_ChannLen = p_RecvData.ReadBits(7);
    uint32 l_NameLen = p_RecvData.ReadBits(8) * 2;

    l_NameLen += p_RecvData.ReadBit();

    std::string l_PlrName = p_RecvData.ReadString(l_NameLen);
    std::string l_ChannName = p_RecvData.ReadString(l_ChannLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->Kick(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelBan(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelBanCommand < 5)
        return;
    else
       timeLastChannelBanCommand = l_Now;

    uint32 l_ChannLen, l_NameLen;
    std::string l_ChannName, l_PlrName;

    l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();

    l_ChannLen = p_RecvData.ReadBits(7);

    l_PlrName = p_RecvData.ReadString(l_NameLen);
    l_ChannName = p_RecvData.ReadString(l_ChannLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->Ban(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelUnban(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelUnbanCommand < 5)
        return;
    else
       timeLastChannelUnbanCommand = l_Now;

    uint32 l_ChannLen, l_NameLen;
    std::string l_ChannName, l_PlrName;

    l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();

    l_ChannLen = p_RecvData.ReadBits(7);

    l_PlrName = p_RecvData.ReadString(l_NameLen);
    l_ChannName = p_RecvData.ReadString(l_ChannLen);

    if (!normalizePlayerName(l_PlrName))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->UnBan(_player->GetGUID(), l_PlrName.c_str());
    }
}

void WorldSession::HandleChannelAnnouncements(WorldPacket& p_RecvData)
{
    time_t l_Now = time(nullptr);
    if (l_Now - timeLastChannelAnnounceCommand < 5)
        return;
    else
       timeLastChannelAnnounceCommand = l_Now;

    uint32 l_Len = p_RecvData.ReadBits(7);
    std::string l_ChannName = p_RecvData.ReadString(l_Len);

    if (ChannelMgr* l_ChannelMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannName, _player))
            l_Channel->Announce(_player->GetGUID());
    }
}

void WorldSession::HandleChannelDisplayListQuery(WorldPacket& p_RecvData)
{
    /// This should be OK because the 2 function _were_ the same
    HandleChannelList(p_RecvData);
}

void WorldSession::HandleGetChannelMemberCount(WorldPacket &recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    std::string channelname;
    recvPacket >> channelname;
    if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
    {
        if (Channel* chn = cMgr->GetChannel(channelname, _player))
        {
            WorldPacket data(SMSG_CHANNEL_MEMBER_COUNT, chn->GetName().size()+1+1+4);
            data << chn->GetName();
            data << uint8(chn->GetFlags());
            data << uint32(chn->GetNumPlayers());
            SendPacket(&data);
        }
    }
}

void WorldSession::HandleSetChannelWatch(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Opcode %u", recvPacket.GetOpcode());
    std::string channelname;
    recvPacket >> channelname;
    /*if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
        if (Channel* chn = cMgr->GetChannel(channelName, _player))
            chn->JoinNotify(_player->GetGUID());*/
}
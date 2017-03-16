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

#include "Language.h"
#include "WorldPacket.h"
#include "Common.h"
#include "ObjectMgr.h"
#include "TicketMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldSession.h"
#include "Util.h"

void WorldSession::HandleGMTicketCreateOpcode(WorldPacket& p_RecvData)
{
    /// Don't accept tickets if the ticket queue is disabled. (Ticket UI is greyed out but not fully dependable)
    if (sTicketMgr->GetStatus() == GMTICKET_QUEUE_STATUS_DISABLED)
        return;

    if (_player->getLevel() < sWorld->getIntConfig(CONFIG_TICKET_LEVEL_REQ))
    {
        SendNotification(GetTrinityString(LANG_TICKET_REQ), sWorld->getIntConfig(CONFIG_TICKET_LEVEL_REQ));
        return;
    }

    GMTicketResponse l_Response = GMTICKET_RESPONSE_CREATE_ERROR;

    /// Player must not have opened ticket
    if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(_player->GetGUID()))
    {
        if (l_Ticket->IsCompleted())
        {
            sTicketMgr->CloseTicket(l_Ticket->GetId(), _player->GetGUID());
            sTicketMgr->SendTicket(this, NULL);

            WorldPacket l_Data(SMSG_GM_RESPONSE_STATUS_UPDATE, 1);
            l_Data << uint8(GMTICKET_RESPONSE_TICKET_DELETED);
            SendPacket(&l_Data);

            GmTicket* l_NewTicket = new GmTicket(_player, p_RecvData);
            sTicketMgr->AddTicket(l_NewTicket);
            sTicketMgr->UpdateLastChange();

            sWorld->SendGMText(LANG_COMMAND_TICKETNEW, _player->GetName(), l_NewTicket->GetId());

            sTicketMgr->SendTicket(this, l_NewTicket);

            l_Response = GMTICKET_RESPONSE_CREATE_SUCCESS;
        }
        else
            l_Response = GMTICKET_RESPONSE_ALREADY_EXIST;
    }
    else
    {
        GmTicket* l_NewTicket = new GmTicket(_player, p_RecvData);
        sTicketMgr->AddTicket(l_NewTicket);
        sTicketMgr->UpdateLastChange();

        sWorld->SendGMText(LANG_COMMAND_TICKETNEW, _player->GetName(), l_NewTicket->GetId());

        sTicketMgr->SendTicket(this, l_NewTicket);

        l_Response = GMTICKET_RESPONSE_CREATE_SUCCESS;
    }

    WorldPacket l_Data(SMSG_GM_RESPONSE_STATUS_UPDATE, 1);
    l_Data << uint8(l_Response);
    SendPacket(&l_Data);
}

void WorldSession::HandleGMTicketUpdateOpcode(WorldPacket& p_RecvData)
{
    std::string l_Message;
    uint32 l_StrLen = p_RecvData.ReadBits(11);
    l_Message = p_RecvData.ReadString(l_StrLen);

    GMTicketResponse l_Response = GMTICKET_RESPONSE_UPDATE_ERROR;
    if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(_player->GetGUID()))
    {
        SQLTransaction l_Trans = SQLTransaction(NULL);
        l_Ticket->SetMessage(l_Message);
        l_Ticket->SaveToDB(l_Trans);

        sWorld->SendGMText(LANG_COMMAND_TICKETUPDATED, _player->GetName(), l_Ticket->GetId());

        sTicketMgr->SendTicket(this, l_Ticket);
        l_Response = GMTICKET_RESPONSE_UPDATE_SUCCESS;
    }

    WorldPacket l_Data(SMSG_GM_RESPONSE_STATUS_UPDATE, 1);
    l_Data << uint8(l_Response);
    SendPacket(&l_Data);
}

void WorldSession::HandleGMTicketDeleteOpcode(WorldPacket& /*p_RecvData*/)
{
    if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(_player->GetGUID()))
    {
        WorldPacket l_Data(SMSG_GM_RESPONSE_STATUS_UPDATE, 1);
        l_Data << uint8(GMTICKET_RESPONSE_TICKET_DELETED);
        SendPacket(&l_Data);

        sWorld->SendGMText(LANG_COMMAND_TICKETPLAYERABANDON, _player->GetName(), l_Ticket->GetId());

        sTicketMgr->CloseTicket(l_Ticket->GetId(), _player->GetGUID());
        sTicketMgr->SendTicket(this, nullptr);
    }
}

void WorldSession::HandleGMTicketGetTicketOpcode(WorldPacket& /*p_RecvData*/)
{
    SendQueryTimeResponse();

    if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(_player->GetGUID()))
    {
        if (l_Ticket->IsCompleted())
            l_Ticket->SendResponse(this);
        else
            sTicketMgr->SendTicket(this, l_Ticket);
    }
    else
        sTicketMgr->SendTicket(this, nullptr);
}

void WorldSession::HandleGMTicketGetWebTicketOpcode(WorldPacket& /*p_RecvData*/)
{
}

void WorldSession::HandleGMTicketSystemStatusOpcode(WorldPacket& /*p_RecvData*/)
{
    /// Note: This only disables the ticket UI at client side and is not fully reliable
    /// Are we sure this is a uint32? Should ask Zor
    WorldPacket l_Data(SMSG_GM_TICKET_SYSTEM_STATUS, 4);
    l_Data << uint32(sTicketMgr->GetStatus() ? GMTICKET_QUEUE_STATUS_ENABLED : GMTICKET_QUEUE_STATUS_DISABLED);
    SendPacket(&l_Data);
}

void WorldSession::HandleGMSurveySubmit(WorldPacket& recvData)
{
    uint32 nextSurveyID = sTicketMgr->GetNextSurveyID();
    // just put the survey into the database
    uint32 mainSurvey; // GMSurveyCurrentSurvey.dbc, column 1 (all 9) ref to GMSurveySurveys.dbc
    recvData >> mainSurvey;

    // sub_survey1, r1, comment1, sub_survey2, r2, comment2, sub_survey3, r3, comment3, sub_survey4, r4, comment4, sub_survey5, r5, comment5, sub_survey6, r6, comment6, sub_survey7, r7, comment7, sub_survey8, r8, comment8, sub_survey9, r9, comment9, sub_survey10, r10, comment10,
    for (uint8 i = 0; i < 10; i++)
    {
        uint32 subSurveyId; // ref to i'th GMSurveySurveys.dbc field (all fields in that dbc point to fields in GMSurveyQuestions.dbc)
        recvData >> subSurveyId;
        if (!subSurveyId)
            break;

        uint8 rank; // probably some sort of ref to GMSurveyAnswers.dbc
        recvData >> rank;
        std::string comment; // comment ("Usage: GMSurveyAnswerSubmit(question, rank, comment)")
        recvData >> comment;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GM_SUBSURVEY);
        stmt->setUInt32(0, nextSurveyID);
        stmt->setUInt32(1, subSurveyId);
        stmt->setUInt32(2, rank);
        stmt->setString(3, comment);
        CharacterDatabase.Execute(stmt);
    }

    std::string comment; // just a guess
    recvData >> comment;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GM_SURVEY);
    stmt->setUInt32(0, GUID_LOPART(GetPlayer()->GetGUID()));
    stmt->setUInt32(1, nextSurveyID);
    stmt->setUInt32(2, mainSurvey);
    stmt->setString(3, comment);

    CharacterDatabase.Execute(stmt);
}

void WorldSession::HandleReportLag(WorldPacket& recvData)
{
    // just put the lag report into the database...
    // can't think of anything else to do with it
    uint32 lagType, mapId;
    recvData >> lagType;
    recvData >> mapId;
    float x, y, z;
    recvData >> x;
    recvData >> y;
    recvData >> z;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_LAG_REPORT);
    stmt->setUInt32(0, GUID_LOPART(GetPlayer()->GetGUID()));
    stmt->setUInt8 (1, lagType);
    stmt->setUInt16(2, mapId);
    stmt->setFloat (3, x);
    stmt->setFloat (4, y);
    stmt->setFloat (5, z);
    stmt->setUInt32(6, GetLatency());
    stmt->setUInt32(7, time(NULL));
    CharacterDatabase.Execute(stmt);
}

void WorldSession::HandleGMResponseResolve(WorldPacket& /*p_RecvData*/)
{
    /// Empty packet
    if (GmTicket* l_Ticket = sTicketMgr->GetTicketByPlayer(_player->GetGUID()))
    {
        WorldPacket l_Data(SMSG_GM_RESPONSE_STATUS_UPDATE, 1);
        l_Data << uint8(GMTICKET_RESPONSE_TICKET_DELETED);
        SendPacket(&l_Data);

        sTicketMgr->CloseTicket(l_Ticket->GetId(), _player->GetGUID());
        sTicketMgr->SendTicket(this, nullptr);
    }
}
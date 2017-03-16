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

#ifndef _TICKETMGR_H
#define _TICKETMGR_H

#include <string>
#include <ace/Singleton.h>

#include "ObjectMgr.h"

class ChatHandler;

/// From blizzard lua
enum GMTicketSystemStatus
{
    GMTICKET_QUEUE_STATUS_DISABLED          = 0,
    GMTICKET_QUEUE_STATUS_ENABLED           = 1
};

enum GMTicketStatus
{
    GMTICKET_STATUS_HASTEXT                 = 0x06,
    GMTICKET_STATUS_DEFAULT                 = 0x0A
};

enum GMTicketResponse : uint8
{
    GMTICKET_RESPONSE_ALREADY_EXIST         = 1,
    GMTICKET_RESPONSE_CREATE_SUCCESS        = 2,
    GMTICKET_RESPONSE_CREATE_ERROR          = 3,
    GMTICKET_RESPONSE_UPDATE_SUCCESS        = 4,
    GMTICKET_RESPONSE_UPDATE_ERROR          = 5,
    GMTICKET_RESPONSE_TICKET_DELETED        = 9
};

/// From Blizzard LUA:
/// GMTICKET_ASSIGNEDTOGM_STATUS_NOT_ASSIGNED = 0;    -- ticket is not currently assigned to a gm
/// GMTICKET_ASSIGNEDTOGM_STATUS_ASSIGNED = 1;        -- ticket is assigned to a normal gm
/// GMTICKET_ASSIGNEDTOGM_STATUS_ESCALATED = 2;        -- ticket is in the escalation queue
/// 3 is a custom value and should never actually be sent
enum GMTicketEscalationStatus
{
    TICKET_UNASSIGNED                       = 0,
    TICKET_ASSIGNED                         = 1,
    TICKET_IN_ESCALATION_QUEUE              = 2,
    TICKET_ESCALATED_ASSIGNED               = 3
};

/// From blizzard lua
enum GMTicketOpenedByGMStatus
{
    GMTICKET_OPENEDBYGM_STATUS_NOT_OPENED   = 0,    ///< Ticket has never been opened by a gm
    GMTICKET_OPENEDBYGM_STATUS_OPENED       = 1     ///< Ticket has been opened by a gm
};

enum LagReportType
{
    LAG_REPORT_TYPE_LOOT                    = 1,
    LAG_REPORT_TYPE_AUCTION_HOUSE           = 2,
    LAG_REPORT_TYPE_MAIL                    = 3,
    LAG_REPORT_TYPE_CHAT                    = 4,
    LAG_REPORT_TYPE_MOVEMENT                = 5,
    LAG_REPORT_TYPE_SPELL                   = 6
};

class GmTicket
{
    public:
        GmTicket();
        explicit GmTicket(Player* p_Player, WorldPacket& p_RecvData);
        ~GmTicket();

        bool IsClosed() const { return m_ClosedBy; }
        bool IsCompleted() const { return m_Completed; }
        bool IsFromPlayer(uint64 p_Guid) const { return p_Guid == m_PlayerGUID; }
        bool IsAssigned() const { return m_AssignedTo != 0; }
        bool IsAssignedTo(uint64 p_Guid) const { return p_Guid == m_AssignedTo; }
        bool IsAssignedNotTo(uint64 p_Guid) const { return IsAssigned() && !IsAssignedTo(p_Guid); }

        uint32 GetId() const { return m_ID; }
        Player* GetPlayer() const { return ObjectAccessor::FindPlayer(m_PlayerGUID); }
        std::string GetPlayerName() const { return m_PlayerName; }
        std::string GetMessage() const { return m_Message; }
        Player* GetAssignedPlayer() const { return ObjectAccessor::FindPlayer(m_AssignedTo); }
        uint64 GetAssignedToGUID() const { return m_AssignedTo; }
        std::string GetAssignedToName() const
        {
            std::string l_Name;
            // save queries if ticket is not assigned
            if (m_AssignedTo)
                sObjectMgr->GetPlayerNameByGUID(m_AssignedTo, l_Name);

            return l_Name;
        }

        uint64 GetLastModifiedTime() const { return m_LastModifiedTime; }
        GMTicketEscalationStatus GetEscalatedStatus() const { return m_EscalatedStatus; }

        void SetEscalatedStatus(GMTicketEscalationStatus p_EscalatedStatus) { m_EscalatedStatus = p_EscalatedStatus; }
        void SetAssignedTo(uint64 p_Guid, bool p_IsAdmin)
        {
            m_AssignedTo = p_Guid;
            if (p_IsAdmin && m_EscalatedStatus == TICKET_IN_ESCALATION_QUEUE)
                m_EscalatedStatus = TICKET_ESCALATED_ASSIGNED;
            else if (m_EscalatedStatus == TICKET_UNASSIGNED)
                m_EscalatedStatus = TICKET_ASSIGNED;
        }
        void SetClosedBy(int64 const& p_Value) { m_ClosedBy = p_Value; }
        void SetMessage(std::string const& p_Message)
        {
            m_Message = p_Message;
            m_LastModifiedTime = uint64(time(nullptr));
        }
        void SetComment(std::string const& p_Comment) { m_Comment = p_Comment; }
        void SetViewed() { m_Viewed = true; }
        void SetUnassigned();
        void SetCompleted(bool p_Completed) { m_Completed = p_Completed; }

        void AppendResponse(std::string const& p_Response) { m_Response += p_Response; }

        bool LoadFromDB(Field* p_Fields);
        void SaveToDB(SQLTransaction& p_Trans) const;
        void DeleteFromDB();

        void WritePacket(WorldPacket& p_Data) const;
        void SendResponse(WorldSession* p_Session) const;

        void TeleportTo(Player* p_Player) const;
        std::string FormatMessageString(ChatHandler& p_Handler, bool p_Detailed = false) const;
        std::string FormatMessageString(ChatHandler& p_Handler, char const* p_ClosedName, char const* p_AssignedToName, char const* p_UnassignedName, char const* p_DeteledName) const;

    private:
        uint32 m_ID;
        uint64 m_PlayerGUID;
        std::string m_PlayerName;
        float m_PosX;
        float m_PosY;
        float m_PosZ;
        uint16 m_MapID;
        std::string m_Message;
        uint64 m_CreateTime;
        uint64 m_LastModifiedTime;
        int64 m_ClosedBy; // 0 = Open, -1 = Console, playerGuid = player abandoned ticket, other = GM who closed it.
        uint64 m_AssignedTo;
        std::string m_Comment;
        bool m_Completed;
        GMTicketEscalationStatus m_EscalatedStatus;
        bool m_Viewed;
        bool m_NeedResponse; // TODO: find out the use of this, and then store it in DB
        bool m_HaveTicket;
        std::string m_Response;
};

typedef std::map<uint32, GmTicket*> GmTicketList;

class TicketMgr
{
    friend class ACE_Singleton<TicketMgr, ACE_Null_Mutex>;

    private:
        TicketMgr();
        ~TicketMgr();

    public:
        void LoadTickets();
        void LoadSurveys();

        GmTicket* GetTicket(uint32 p_TicketID)
        {
            GmTicketList::iterator l_Iter = m_TicketList.find(p_TicketID);
            if (l_Iter != m_TicketList.end())
                return l_Iter->second;

            return nullptr;
        }

        GmTicket* GetTicketByPlayer(uint64 p_PlayerGuid)
        {
            for (GmTicketList::const_iterator l_Iter = m_TicketList.begin(); l_Iter != m_TicketList.end(); ++l_Iter)
            {
                if (l_Iter->second && l_Iter->second->IsFromPlayer(p_PlayerGuid) && !l_Iter->second->IsClosed())
                    return l_Iter->second;
            }

            return nullptr;
        }

        GmTicket* GetOldestOpenTicket()
        {
            for (GmTicketList::const_iterator l_Iter = m_TicketList.begin(); l_Iter != m_TicketList.end(); ++l_Iter)
            {
                if (l_Iter->second && !l_Iter->second->IsClosed() && !l_Iter->second->IsCompleted())
                    return l_Iter->second;
            }

            return nullptr;
        }

        void AddTicket(GmTicket* p_Ticket);
        void CloseTicket(uint32 p_TicketID, int64 p_Source = -1);
        void RemoveTicket(uint32 p_TicketID);

        bool GetStatus() const { return m_Status; }
        void SetStatus(bool p_Status) { m_Status = p_Status; }

        uint64 GetLastChange() const { return m_LastChange; }
        void UpdateLastChange() { m_LastChange = uint64(time(nullptr)); }

        uint32 GenerateTicketId() { return ++m_LastTicketID; }
        uint32 GetOpenTicketCount() const { return m_OpenTicketCount; }
        uint32 GetNextSurveyID() { return ++m_LastSurveyID; }

        void Initialize();
        void ResetTickets();

        void ShowList(ChatHandler& p_Handler, bool p_OnlineOnly) const;
        void ShowClosedList(ChatHandler& p_Handler) const;
        void ShowEscalatedList(ChatHandler& p_Handler) const;

        void SendTicket(WorldSession* p_Session, GmTicket* p_Ticket) const;

    protected:
        void _RemoveTicket(uint32 p_TicketID, int64 p_Source = -1, bool p_Permanently = false);

        GmTicketList m_TicketList;

        bool   m_Status;
        uint32 m_LastTicketID;
        uint32 m_LastSurveyID;
        uint32 m_OpenTicketCount;
        uint64 m_LastChange;
};

#define sTicketMgr ACE_Singleton<TicketMgr, ACE_Null_Mutex>::instance()

#endif // _TICKETMGR_H

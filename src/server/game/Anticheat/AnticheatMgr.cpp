/*
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

#include "AnticheatMgr.h"
#include "AnticheatScripts.h"
#include "MapManager.h"
#include "AccountMgr.h"

#define CLIMB_ANGLE 1.9f

AnticheatMgr::AnticheatMgr()
{
    m_dataDeleteTimer = 0;
}

AnticheatMgr::~AnticheatMgr()
{
    m_Players.clear();
}

void AnticheatMgr::JumpHackDetection(Player* player, MovementInfo const& /* movementInfo */,uint32 opcode)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & JUMP_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();

    if (m_Players[key].GetLastOpcode() == CMSG_MOVE_JUMP && opcode == CMSG_MOVE_JUMP)
    {
        BuildReport(player,JUMP_HACK_REPORT);
        //TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Jump-Hack detected player GUID (low) %u",player->GetGUIDLow());
    }
}

void AnticheatMgr::WalkOnWaterHackDetection(Player* player, MovementInfo const& /* movementInfo */)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & WALK_WATER_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();
    if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
        return;

    // if we are a ghost we can walk on water
    if (!player->isAlive())
        return;

    if (player->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
        player->HasAuraType(SPELL_AURA_SAFE_FALL) ||
        player->HasAuraType(SPELL_AURA_WATER_WALK))
        return;

    //TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Walk on Water - Hack detected player GUID (low) %u",player->GetGUIDLow());
    BuildReport(player,WALK_WATER_HACK_REPORT);

}

void AnticheatMgr::FlyHackDetection(Player* player, MovementInfo const& /* movementInfo */)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & FLY_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();
    if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_FLYING))
        return;

    if (player->HasAuraType(SPELL_AURA_FLY) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED))
        return;

    if (player->isDead() && player->HasAura(55164))
        return;

    if (player->HasAura(34480) || player->HasAura(44226))   // Kael'Thas encounters (in The Eye and Magister's Terrace)
        return;

    //TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Fly-Hack detected player GUID (low) %u",player->GetGUIDLow());
    BuildReport(player,FLY_HACK_REPORT);
}

void AnticheatMgr::TeleportPlaneHackDetection(Player* player, MovementInfo const& movementInfo)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & TELEPORT_PLANE_HACK_DETECTION) == 0)
        return;

    auto const& data = m_Players[player->GetGUIDLow()];
    if (movementInfo.GetMovementFlags() != data.GetLastMovementInfo().GetMovementFlags())
        return;

    uint32 distance2D = (uint32)movementInfo.pos.GetExactDist2d(&data.GetLastMovementInfo().pos);
    if (distance2D > sWorld->getIntConfig(CONFIG_ANTICHEAT_TELEPORTHACKDETECTION_DISTANCE) && !player->GetTeleportFlagForAnticheat())
        BuildReport(player, TELEPORT_PLANE_HACK_REPORT);

    player->SetTeleportFlagForAnticheat(false);

    /*if (m_Players[key].GetLastMovementInfo().pos.GetPositionZ() != 0 ||
        movementInfo.pos.GetPositionZ() != 0)
        return;

    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING))
        return;

    //DEAD_FALLING was deprecated
    //if (player->getDeathState() == DEAD_FALLING)
    //    return;
    float x, y, z;
    player->GetPosition(x, y, z);
    float ground_Z = player->GetMap()->GetHeight(x, y, z);
    float z_diff = fabs(ground_Z - z);

    // we are not really walking there
    if (z_diff > 1.0f)
    {
        //TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Teleport To Plane - Hack detected player GUID (low) %u",player->GetGUIDLow());
        BuildReport(player,TELEPORT_PLANE_HACK_REPORT);
    }*/
}

void AnticheatMgr::StartHackDetection(Player* player, MovementInfo const& movementInfo, uint32 opcode)
{
    if (!sWorld->getBoolConfig(CONFIG_ANTICHEAT_ENABLE))
        return;

    if (player->isGameMaster() || player->GetSession()->GetSecurity() > SEC_PLAYER)
        return;
    
    if (!sWorld->getBoolConfig(CONFIG_ANTICHEAT_GAMEMASTER_REPORT) && player->GetSession())
    {
        if (player->GetSession()->GetSecurity() >= SEC_MODERATOR)
            return;
    }

    // Deeprun Tram
    if (player->GetMapId() == 369)
        return;

    // Ulduar tram
    Position const& pos = movementInfo.pos;
    if (player->GetMapId() == 603 &&
        pos.GetPositionX() > 2260 && pos.GetPositionX() < 2360 &&
        pos.GetPositionY() > 270 && pos.GetPositionX() < 2590 &&
        pos.GetPositionZ() > 415 && pos.GetPositionZ() < 430)
        return;

    switch (player->GetZoneId(true))
    {
        case 3430: // Eversong Woods
        case 3433: // Ghostlands
        case 3525: // Bloodmyst Isle
        case 6757: // Timeless Isle
            return;
        default:
            break;
    }
    
    if (player->HasAura(148537) // Falling Flame
        || player->HasAura(126389))
        return;

    auto& data = m_Players[player->GetGUIDLow()];

    if (player->isInFlight() || player->GetTransport() || player->GetVehicle())
    {
        data.SetLastMovementInfo(movementInfo);
        data.SetLastOpcode(opcode);
        return;
    }

    SpeedHackDetection(player, movementInfo);
    FlyHackDetection(player, movementInfo);
    TeleportPlaneHackDetection(player, movementInfo);

    // Not need checks
    //JumpHackDetection(player,movementInfo,opcode);
    //WalkOnWaterHackDetection(player,movementInfo);
    //ClimbHackDetection(player,movementInfo,opcode);

    data.SetLastMovementInfo(movementInfo);
    data.SetLastOpcode(opcode);
}

// basic detection
void AnticheatMgr::ClimbHackDetection(Player *player, MovementInfo const& movementInfo, uint32 opcode)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & CLIMB_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();

    if (opcode != CMSG_MOVE_HEARTBEAT ||
        m_Players[key].GetLastOpcode() != CMSG_MOVE_HEARTBEAT)
        return;

    // in this case we don't care if they are "legal" flags, they are handled in another parts of the Anticheat Manager.
    if (player->IsInWater() ||
        player->IsFlying() ||
        player->IsFalling())
        return;

    Position playerPos;
    player->GetPosition(&playerPos);

    float deltaZ = fabs(playerPos.GetPositionZ() - movementInfo.pos.GetPositionZ());
    float deltaXY = movementInfo.pos.GetExactDist2d(&playerPos);

    float angle = player->NormalizeOrientation(tan(deltaZ/deltaXY));

    if (angle > CLIMB_ANGLE)
    {
        //TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Climb-Hack detected player GUID (low) %u", player->GetGUIDLow());
        BuildReport(player,CLIMB_HACK_REPORT);
    }
}

void AnticheatMgr::SpeedHackDetection(Player* player,MovementInfo const& movementInfo)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & SPEED_HACK_DETECTION) == 0)
        return;

    auto& data = m_Players[player->GetGUIDLow()];

    // We also must check the map because the movementFlag can be modified by the client.
    // If we just check the flag, they could always add that flag and always skip the speed hacking detection.
    // 369 == DEEPRUN TRAM
    if (player->GetMapId() == 369 || !m_Players.count(player->GetGUIDLow()))
        return;

    if (movementInfo.GetMovementFlags() != data.GetLastMovementInfo().GetMovementFlags())
        return;

    uint32 distance2D = (uint32)movementInfo.pos.GetExactDist2d(&data.GetLastMovementInfo().pos);
    uint8 moveType = 0;

    // we need to know HOW is the player moving
    if (player->HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
        moveType = MOVE_SWIM;
    else if (player->IsFlying())
        moveType = MOVE_FLIGHT;
    else if (player->HasUnitMovementFlag(MOVEMENTFLAG_WALKING))
        moveType = MOVE_WALK;
    else
        moveType = MOVE_RUN;

    // how many yards the player can do in one sec.
    uint32 speedRate = (uint32)(player->GetSpeed(UnitMoveType(moveType)) + movementInfo.jump.xyspeed);

    // how long the player took to move to here.
    uint32 timeDiff = getMSTimeDiff(data.GetLastMovementInfo().time, movementInfo.time);

    if (!timeDiff)
        timeDiff = 1;

    // this is the distance doable by the player in 1 sec, using the time done to move to this point.
    uint32 clientSpeedRate = distance2D * 1000 / timeDiff;

    // we did the (uint32) cast to accept a margin of tolerance
    if (clientSpeedRate > speedRate)
    {
        BuildReport(player,SPEED_HACK_REPORT);
        //TC_LOG_DEBUG("entities.player.character", "AnticheatMgr:: Speed-Hack detected player GUID (low) %u",player->GetGUIDLow());
    }
}

void AnticheatMgr::StartScripts()
{
    new AnticheatScripts();
}

void AnticheatMgr::HandlePlayerLogin(Player* player)
{
    // we must delete this to prevent errors in case of crash
    CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u",player->GetGUIDLow());
    // we initialize the pos of lastMovementPosition var.
    m_Players[player->GetGUIDLow()].SetPosition(player->GetPositionX(),player->GetPositionY(),player->GetPositionZ(),player->GetOrientation());
    QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM daily_players_reports WHERE guid=%u;",player->GetGUIDLow());

    if (resultDB)
        m_Players[player->GetGUIDLow()].SetDailyReportState(true);
}

void AnticheatMgr::HandlePlayerLogout(Player* player)
{
    // TO-DO Make a table that stores the cheaters of the day, with more detailed information.

    // We must also delete it at logout to prevent have data of offline players in the db when we query the database (IE: The GM Command)
    CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u",player->GetGUIDLow());

    m_Players[player->GetGUIDLow()].SetLogOutTime(getMSTime());
}

void AnticheatMgr::SavePlayerData(Player* player)
{
    //CharacterDatabase.PExecute("REPLACE INTO players_reports_status (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u);",player->GetGUIDLow(),m_Players[player->GetGUIDLow()].GetAverage(),m_Players[player->GetGUIDLow()].GetTotalReports(), m_Players[player->GetGUIDLow()].GetTypeReports(SPEED_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(FLY_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(JUMP_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(WALK_WATER_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(CLIMB_HACK_REPORT),m_Players[player->GetGUIDLow()].GetCreationTime());
}

uint32 AnticheatMgr::GetTotalReports(uint32 lowGUID)
{
    return m_Players[lowGUID].GetTotalReports();
}

float AnticheatMgr::GetAverage(uint32 lowGUID)
{
    return m_Players[lowGUID].GetAverage();
}

uint32 AnticheatMgr::GetTypeReports(uint32 lowGUID, uint8 type)
{
    return m_Players[lowGUID].GetTypeReports(type);
}

bool AnticheatMgr::MustCheckTempReports(uint8 type)
{
    if (type == JUMP_HACK_REPORT || type == TELEPORT_PLANE_HACK_REPORT)
        return false;

    return true;
}

void AnticheatMgr::BuildReport(Player* player,uint8 reportType)
{
    uint32 actualTime = getMSTime();

    auto& data = m_Players[player->GetGUIDLow()];

    if (MustCheckTempReports(reportType))
    {
        if (!data.GetTempReportsTimer(reportType))
            data.SetTempReportsTimer(actualTime,reportType);

        if (getMSTimeDiff(data.GetTempReportsTimer(reportType),actualTime) < 3000)
        {
            data.SetTempReports(data.GetTempReports(reportType)+1,reportType);

            if (data.GetTempReports(reportType) < 3)
                return;
        }
        else
        {
            data.SetTempReportsTimer(actualTime,reportType);
            data.SetTempReports(1,reportType);
            return;
        }
    }

    // generating creationTime for average calculation
    if (!data.GetTotalReports())
        data.SetCreationTime(actualTime);

    // increasing total_reports
    data.SetTotalReports(data.GetTotalReports()+1);
    // increasing specific cheat report
    data.SetTypeReports(reportType,data.GetTypeReports(reportType)+1);

    // diff time for average calculation
    uint32 diffTime = getMSTimeDiff(data.GetCreationTime(), actualTime) / IN_MILLISECONDS;

    if (diffTime > 0)
    {
        // Average == Reports per second
        float average = float(data.GetTotalReports()) / float(diffTime);
        data.SetAverage(average);
    }

    if (sWorld->getIntConfig(CONFIG_ANTICHEAT_MAX_REPORTS_FOR_DAILY_REPORT) < data.GetTotalReports())
    {
        if (!data.GetDailyReportState())
        {
            CharacterDatabase.PExecute("REPLACE INTO daily_players_reports (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u);",player->GetGUIDLow(),m_Players[player->GetGUIDLow()].GetAverage(),m_Players[player->GetGUIDLow()].GetTotalReports(), m_Players[player->GetGUIDLow()].GetTypeReports(SPEED_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(FLY_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(JUMP_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(WALK_WATER_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(CLIMB_HACK_REPORT),m_Players[player->GetGUIDLow()].GetCreationTime());
            data.SetDailyReportState(true);
        }
    }

    std::string cheat_type;
    switch (reportType)
    {
        case SPEED_HACK_REPORT:             cheat_type = "SPEED HACK"; break;
        case FLY_HACK_REPORT:               cheat_type = "FLY HACK"; break;
        case TELEPORT_PLANE_HACK_REPORT:    cheat_type = "TELEPORT HACK"; break;
        default:                            cheat_type = ""; break;
    }

    if ((data.GetTotalReports() > sWorld->getIntConfig(CONFIG_ANTICHEAT_REPORTS_INGAME_NOTIFICATION))
        || (reportType == TELEPORT_PLANE_HACK_REPORT && data.GetTypeReports(reportType) > sWorld->getIntConfig(CONFIG_ANTICHEAT_TELEPORTHACKDETECTION_REPORTS_INGAME_NOTIFICATION)))
    {
        // display warning at the center of the screen, hacky way?
        std::string str = "";
        str = "|[[ANTICHEAT]|cFF00FFFF[|cFF60FF00" + std::string(player->GetName()) + "|cFF00FFFF] found cheating! " + cheat_type + " detected!";
        WorldPacket data(SMSG_NOTIFICATION, (str.size()+1));
        data << str;
        sWorld->SendGlobalGMMessage(&data);
        sWorld->SendGMText(LANG_ANTICHEAT, player->GetName(), player->GetName(), cheat_type.c_str());
    }

    WorldSession* session = player->GetSession();
    if (sWorld->getBoolConfig(CONFIG_ANTICHEAT_AUTOBAN_ENABLE) && session)
    {
        if (((data.GetTotalReports() > sWorld->getIntConfig(CONFIG_ANTICHEAT_AUTOBAN_TOTAL_REPORTS) && data.GetAverage() > sWorld->getFloatConfig(CONFIG_ANTICHEAT_AUTOBAN_AVERAGE_REPORTS)) ||
            (data.GetTypeReports(TELEPORT_PLANE_HACK_REPORT) > sWorld->getIntConfig(CONFIG_ANTICHEAT_AUTOBAN_TELEPORT_HACK_TOTAL_REPORTS))) &&
            session->GetLatency() < sWorld->getIntConfig(CONFIG_ANTICHEAT_AUTOBAN_MAX_PING) &&
            player->isAlive())
        {
            uint32 account_id = session->GetAccountId();
            uint32 duration = sWorld->getIntConfig(CONFIG_ANTICHEAT_AUTOBAN_DURATION);
            LoginDatabase.PExecute("UPDATE account_banned SET active = 0 WHERE id = %u", account_id);
            LoginDatabase.PExecute("INSERT INTO account_banned (id, bandate, unbandate, bannedby, banreason, active) VALUES (%u, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+%u, 'Anticheat MoP', 'Autoban. Last detected cheat type: %s', 1)", account_id, duration, cheat_type.c_str());
            session->KickPlayer();
        }
    }
}

void AnticheatMgr::AnticheatGlobalCommand(ChatHandler* handler)
{
    // MySQL will sort all for us, anyway this is not the best way we must only save the anticheat data not whole player's data!.
    sObjectAccessor->SaveAllPlayers();

    QueryResult resultDB = CharacterDatabase.Query("SELECT guid,average,total_reports FROM players_reports_status WHERE total_reports != 0 ORDER BY average ASC LIMIT 3;");
    if (!resultDB)
    {
        handler->PSendSysMessage("No players found.");
        return;
    } else
    {
        handler->SendSysMessage("=============================");
        handler->PSendSysMessage("Players with the lowest averages:");
        do
        {
            Field *fieldsDB = resultDB->Fetch();

            uint32 guid = fieldsDB[0].GetUInt32();
            float average = fieldsDB[1].GetFloat();
            uint32 total_reports = fieldsDB[2].GetUInt32();

            if (Player* player = sObjectMgr->GetPlayerByLowGUID(guid))
                handler->PSendSysMessage("Player: %s Average: %f Total Reports: %u",player->GetName(),average,total_reports);

        } while (resultDB->NextRow());
    }

    resultDB = CharacterDatabase.Query("SELECT guid,average,total_reports FROM players_reports_status WHERE total_reports != 0 ORDER BY total_reports DESC LIMIT 3;");

    // this should never happen
    if (!resultDB)
    {
        handler->PSendSysMessage("No players found.");
        return;
    } else
    {
        handler->SendSysMessage("=============================");
        handler->PSendSysMessage("Players with the more reports:");
        do
        {
            Field *fieldsDB = resultDB->Fetch();

            uint32 guid = fieldsDB[0].GetUInt32();
            float average = fieldsDB[1].GetFloat();
            uint32 total_reports = fieldsDB[2].GetUInt32();

            if (Player* player = sObjectMgr->GetPlayerByLowGUID(guid))
                handler->PSendSysMessage("Player: %s Total Reports: %u Average: %f",player->GetName(),total_reports,average);

        } while (resultDB->NextRow());
    }
}

void AnticheatMgr::AnticheatDeleteCommand(uint32 guid)
{
    if (!guid)
    {
        for (AnticheatPlayersDataMap::iterator it = m_Players.begin(); it != m_Players.end(); ++it)
        {
            auto& data = (*it).second;
            data.SetLogOutTime(0);
            data.SetTotalReports(0);
            data.SetAverage(0);
            data.SetCreationTime(0);
            for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
            {
                data.SetTempReports(0, i);
                data.SetTempReportsTimer(0, i);
                data.SetTypeReports(i, 0);
            }
        }
        CharacterDatabase.PExecute("DELETE FROM players_reports_status;");
    }
    else
    {
        auto& data = m_Players[guid];
        data.SetLogOutTime(0);
        data.SetTotalReports(0);
        data.SetAverage(0);
        data.SetCreationTime(0);
        for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
        {
            data.SetTempReports(0, i);
            data.SetTempReportsTimer(0, i);
            data.SetTypeReports(i, 0);
        }
        CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u;",guid);
    }
}

void AnticheatMgr::ResetDailyReportStates()
{
     for (AnticheatPlayersDataMap::iterator it = m_Players.begin(); it != m_Players.end(); ++it)
         m_Players[(*it).first].SetDailyReportState(false);
}

void AnticheatMgr::Update(uint32 diff)
{
    const uint32 s_DeleteReportPeriod = 1 * HOUR * IN_MILLISECONDS;

    if (m_dataDeleteTimer >= 5000)
    {
        for (auto itr = m_Players.begin(); itr != m_Players.end();)
        {
            auto& data = itr->second;

            if (Player* player = sObjectAccessor->FindPlayer(MAKE_NEW_GUID(itr->first, 0, HighGuid::Player)))
                data.SetLogOutTime(0); // if player is online again

            if (data.GetLogOutTime() && GetMSTimeDiffToNow(data.GetLogOutTime()) >= s_DeleteReportPeriod)
            {
                m_Players.erase(itr++);
                continue;
            }

            ++itr;
        }

        m_dataDeleteTimer = 0;
    }
    else
        m_dataDeleteTimer += diff;
}

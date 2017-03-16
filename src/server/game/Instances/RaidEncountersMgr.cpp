
#include "Common.h"
#include "Player.h"
#include "GridNotifiers.h"
#include "Log.h"
#include "GridStates.h"
#include "CellImpl.h"
#include "Map.h"
#include "MapManager.h"
#include "MapInstanced.h"
#include "Timer.h"
#include "GridNotifiersImpl.h"
#include "Config.h"
#include "Transport.h"
#include "ObjectMgr.h"
#include "World.h"
#include "Group.h"
#include "InstanceScript.h"
#include "RaidEncountersMgr.h"

RaidEncounter::RaidEncounter(uint32 playerGuidLow, uint16 MapId, uint32 encounterMask)
: m_playerGuidLow(playerGuidLow), m_mapid(MapId), m_encounterMask(encounterMask),
m_IsBinded(false)
{
}

RaidEncounter::~RaidEncounter()
{
}

void RaidEncounter::SaveToDB(SQLTransaction* trans /*=NULL*/)
{
    if (trans && !trans->null())
    {
        (*trans)->PAppend("REPLACE INTO character_raid (guid, mapid, encounterMask, data) VALUES ('%u', '%u', '%u', '%s')", m_playerGuidLow, m_mapid, m_encounterMask, data.c_str());
    }
    else
    {
        SQLTransaction newTrans = CharacterDatabase.BeginTransaction();
        newTrans->PAppend("REPLACE INTO character_raid (guid, mapid, encounterMask, data) VALUES ('%u', '%u', '%u', '%s')", m_playerGuidLow, m_mapid, m_encounterMask, data.c_str());
        CharacterDatabase.CommitTransaction(newTrans);
    }

    m_IsBinded = true;
}

void RaidEncounter::LoadRespawnTimes()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CREATURE_RESPAWNS_FOR_RAID);
    stmt->setUInt16(0, GetMapId());
    stmt->setUInt32(1, m_playerGuidLow);
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        do
        {
            Field* fields      = result->Fetch();
            uint32 loguid      = fields[0].GetUInt32();
            uint32 respawnTime = fields[1].GetUInt32();

            m_creatureRespawnTimes[loguid] = time_t(respawnTime);
        }
        while (result->NextRow());
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GO_RESPAWNS_FOR_RAID);
    stmt->setUInt16(0, GetMapId());
    stmt->setUInt32(1, m_playerGuidLow);
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        do
        {
            Field* fields      = result->Fetch();
            uint32 loguid      = fields[0].GetUInt32();
            uint32 respawnTime = fields[1].GetUInt32();

            m_goRespawnTimes[loguid] = time_t(respawnTime);
        }
        while (result->NextRow());
    }
}

void RaidEncounter::SaveCreatureRespawnTime(uint32 creatureDbGuid, uint32 respawnTime)
{
    m_creatureRespawnTimes[creatureDbGuid] = respawnTime;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CREATURE_RESPAWN_FOR_RAID);
    stmt->setUInt32(0, m_playerGuidLow);
    stmt->setUInt32(1, respawnTime);
    stmt->setUInt32(2, creatureDbGuid);
    stmt->setUInt32(3, m_mapid);
    CharacterDatabase.Execute(stmt);
}

void RaidEncounter::SaveGameObjectRespawnTime(uint32 gameobjectDbGuid, uint32 respawnTime)
{
    m_goRespawnTimes[gameobjectDbGuid] = respawnTime;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GO_RESPAWN_FOR_RAID);
    stmt->setUInt32(0, gameobjectDbGuid);
    stmt->setUInt32(1, uint32(respawnTime));
    stmt->setUInt16(2, m_mapid);
    stmt->setUInt32(3, m_playerGuidLow);
    CharacterDatabase.Execute(stmt);
}

void RaidEncounter::RemoveCreatureRespawnTime(uint32 creatureGuid)
{
    m_creatureRespawnTimes.erase(creatureGuid);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN_FOR_RAID);
    stmt->setUInt32(0, creatureGuid);
    stmt->setUInt16(1, GetMapId());
    stmt->setUInt32(2, m_playerGuidLow);
    CharacterDatabase.Execute(stmt);
}

void RaidEncounter::RemoveGameObjectRespawnTime(uint32 gameobjectGuid)
{
    m_goRespawnTimes.erase(gameobjectGuid);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GO_RESPAWN_FOR_RAID);
    stmt->setUInt32(0, gameobjectGuid);
    stmt->setUInt16(1, GetMapId());
    stmt->setUInt32(2, m_playerGuidLow);
    CharacterDatabase.Execute(stmt);
}

time_t RaidEncounter::GetCreatureRespawnTime(uint32 creatureGuid)
{
    ACE_Based::LockedMap<uint32 /*dbGUID*/, time_t>::const_iterator itr = m_creatureRespawnTimes.find(creatureGuid);
    if (itr != m_creatureRespawnTimes.end())
        return itr->second;

    return time_t(0);
}

time_t RaidEncounter::GetGameObjectRespawnTime(uint32 gameobjectGuid)
{
    ACE_Based::LockedMap<uint32 /*dbGUID*/, time_t>::const_iterator itr = m_goRespawnTimes.find(gameobjectGuid);
    if (itr != m_goRespawnTimes.end())
        return itr->second;

    return time_t(0);
}

void RaidEncounter::DeleteRespawnTimes()
{
    m_creatureRespawnTimes.clear();
    m_goRespawnTimes.clear();

    DeleteRespawnTimesInDB(m_playerGuidLow, GetMapId());
}

void RaidEncounter::DeleteRespawnTimesInDB(uint32 playerGuid, uint32 mapId)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN_BY_ENCOUNTER);
    stmt->setUInt16(0, mapId);
    stmt->setUInt32(1, playerGuid);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GO_RESPAWN_BY_ENCOUNTER);
    stmt->setUInt16(0, mapId);
    stmt->setUInt32(1, playerGuid);
    CharacterDatabase.Execute(stmt);
}

void RaidEncounter::SwapRespawnTimesFromEncounter(RaidEncounter* encounter)
{
    if (this == encounter)
        return;

    m_creatureRespawnTimes.clear();
    m_goRespawnTimes.clear();

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN_BY_ENCOUNTER);
    stmt->setUInt16(0, GetMapId());
    stmt->setUInt32(1, m_playerGuidLow);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GO_RESPAWN_BY_ENCOUNTER);
    stmt->setUInt16(0, GetMapId());
    stmt->setUInt32(1, m_playerGuidLow);
    trans->Append(stmt);

    ACE_Based::LockedMap<uint32 /*dbGUID*/, time_t>* creatureRespawnTimes = encounter->GetCreatureRespawnTimes();
    if (creatureRespawnTimes->size() > 0)
    {
        for (ACE_Based::LockedMap<uint32, time_t>::const_iterator itr = creatureRespawnTimes->begin(); itr != creatureRespawnTimes->end(); ++itr)
        {
            m_creatureRespawnTimes[itr->first] = itr->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CREATURE_RESPAWN_FOR_RAID);
            stmt->setUInt32(0, m_playerGuidLow);
            stmt->setUInt32(1, itr->second);
            stmt->setUInt32(2, itr->first);
            stmt->setUInt32(3, m_mapid);
            trans->Append(stmt);
        }
    }

    ACE_Based::LockedMap<uint32 /*dbGUID*/, time_t>* gameobjectRespawnTimes = encounter->GetGameObjectRespawnTimes();
    if (gameobjectRespawnTimes->size() > 0)
    {
        for (ACE_Based::LockedMap<uint32, time_t>::const_iterator itr = gameobjectRespawnTimes->begin(); itr != gameobjectRespawnTimes->end(); ++itr)
        {
            m_goRespawnTimes[itr->first] = itr->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GO_RESPAWN_FOR_RAID);
            stmt->setUInt32(0, itr->first);
            stmt->setUInt32(1, itr->second);
            stmt->setUInt16(2, m_mapid);
            stmt->setUInt32(3, m_playerGuidLow);
            trans->Append(stmt);
        }
    }

    CharacterDatabase.CommitTransaction(trans);
}

RaidEncounterManager::RaidEncounterManager()
{

}

RaidEncounterManager::~RaidEncounterManager()
{

}

void RaidEncounterManager::LoadFromDB()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.PQuery("SELECT guid, mapId, encounterMask, data FROM character_raid");
    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 guid = fields[0].GetUInt32();
        uint32 mapId = fields[1].GetUInt16();
        uint32 encounterMask = fields[2].GetUInt32();
        std::string data = fields[3].GetString();

        RaidEncountersMap& r_map = m_RaidEncounters[guid];
        RaidEncounter& encounter = r_map[mapId];

        encounter.m_playerGuidLow = guid;
        encounter.m_mapid = mapId;
        encounter.m_encounterMask = encounterMask;
        encounter.data = data;

        encounter.LoadRespawnTimes();

        encounter.SetBinded(true);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded instances in %u ms", GetMSTimeDiffToNow(oldMSTime));

}

RaidEncounter*  RaidEncounterManager::AddEncounterForPlayer(uint32 guidLow, uint16 mapId, uint32 encounterMask, std::string& data)
{
    RaidEncountersMap& r_map = m_RaidEncounters[guidLow];
    RaidEncounter& encounter = r_map[mapId];
    
    encounter.m_playerGuidLow = guidLow;
    encounter.m_mapid = mapId;
    encounter.m_encounterMask = encounterMask;
    encounter.data = data;
    return &encounter;
}

void  RaidEncounterManager::AddEncounterForPlayerOrGroup(uint32 guidLow, uint16 mapId, uint32 encounterMask, std::string& data)
{
    Player* player = sObjectAccessor->FindPlayer(MAKE_NEW_GUID(guidLow, 0, HighGuid::Player));

    if (player)
    {
        Group* group = player->GetGroup();
        if (group)
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* member = itr->getSource();
                if (!member)
                    continue;

                RaidEncountersMap& r_map = m_RaidEncounters[member->GetGUIDLow()];
                RaidEncounter& encounter = r_map[mapId];
    
                encounter.m_playerGuidLow = guidLow;
                encounter.m_mapid = mapId;
                encounter.m_encounterMask = encounterMask;
                encounter.data = data;
            }
        }
    }
}

RaidEncountersMap* RaidEncounterManager::GetEncountersForPlayer(uint32 guidLow)
{
    RaidEncountersHashMap::iterator itr = m_RaidEncounters.find(guidLow);
    if (itr == m_RaidEncounters.end())
        return NULL;

    return &itr->second;
}

RaidEncounter* RaidEncounterManager::GetEncounterForPlayerAndMap(uint32 guidLow, uint16 mapId)
{
    RaidEncountersHashMap::iterator itr = m_RaidEncounters.find(guidLow);
    if (itr == m_RaidEncounters.end())
        return NULL;

    RaidEncountersMap::iterator itr2 = itr->second.find(mapId);
    if (itr2 == itr->second.end())
        return NULL;

    return &itr2->second;
}

void RaidEncounterManager::ResetEncounters()
{
    CharacterDatabase.Execute("DELETE FROM character_raid");
    CharacterDatabase.Execute("DELETE FROM creature_raid_respawn");
    CharacterDatabase.Execute("DELETE FROM gameobject_raid_respawn");

    m_RaidEncounters.clear();
}
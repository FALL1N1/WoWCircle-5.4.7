
#ifndef _RAIDENCOUNTERSMGR_H
#define _RAIDENCOUNTERSMGR_H

#include "Define.h"
#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>
#include <list>
#include <map>
#include "UnorderedMap.h"
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "ObjectDefines.h"

struct MapEntry;
class Player;
class Group;

class RaidEncounter
{
    public:

        RaidEncounter() : m_IsBinded(false) {}
        RaidEncounter(uint32 playerGuidLow, uint16 MapId, uint32 encounterMask = 0);

        ~RaidEncounter();

        uint32 GetMapId() const { return m_mapid; }

        void SaveToDB(SQLTransaction* trans = NULL);

        void LoadRespawnTimes();
        void DeleteRespawnTimes();
        static void DeleteRespawnTimesInDB(uint32 playerGuid, uint32 mapId);
        void SaveCreatureRespawnTime(uint32 creatureGuid, uint32 respawnTime);
        void SaveGameObjectRespawnTime(uint32 gameobjectGuid, uint32 respawnTime);
        void RemoveCreatureRespawnTime(uint32 creatureGuid);
        void RemoveGameObjectRespawnTime(uint32 gameobjectGuid);
        time_t GetCreatureRespawnTime(uint32 creatureGuid);
        time_t GetGameObjectRespawnTime(uint32 gameobjectGuid);
        ACE_Based::LockedMap<uint32 /*dbGUID*/, time_t>* GetCreatureRespawnTimes() { return &m_creatureRespawnTimes; }
        ACE_Based::LockedMap<uint32 /*dbGUID*/, time_t>* GetGameObjectRespawnTimes() { return &m_goRespawnTimes; }
        void SwapRespawnTimesFromEncounter(RaidEncounter* encounter);

        void DeleteFromDB();

        MapEntry const* GetMapEntry();

        uint32 GetEncounterMask() const { return m_encounterMask; }

        void SetBinded(bool value) { m_IsBinded = value; }
        bool IsBinded() const { return m_IsBinded; }

    public:

        uint32 m_playerGuidLow;
        uint32 m_mapid;
        uint32 m_encounterMask;
        std::string data;
        bool m_IsBinded;

        ACE_Based::LockedMap<uint32 /*dbGUID*/, time_t> m_creatureRespawnTimes;
        ACE_Based::LockedMap<uint32 /*dbGUID*/, time_t> m_goRespawnTimes;
};

typedef std::map<uint32 /*mapId*/, RaidEncounter> RaidEncountersMap;
typedef std::map<uint32 /*playerGuid*/, RaidEncountersMap> RaidEncountersHashMap;

class RaidEncounterManager
{
    friend class ACE_Singleton<RaidEncounterManager, ACE_Thread_Mutex>;

    private:
        RaidEncounterManager();
        ~RaidEncounterManager();

    public:

        void LoadFromDB();

        void AddEncounterForPlayerOrGroup(uint32 guidLow, uint16 mapId, uint32 encounterMask, std::string& data);
        RaidEncounter* AddEncounterForPlayer(uint32 guidLow, uint16 mapId, uint32 encounterMask, std::string& data);

        RaidEncountersMap* GetEncountersForPlayer(uint32 guidLow);
        RaidEncounter* GetEncounterForPlayerAndMap(uint32 guidLow, uint16 mapId);

        void ResetEncounters();

    private:

        RaidEncountersHashMap m_RaidEncounters;
};

#define sRaidEncountersMgr ACE_Singleton<RaidEncounterManager, ACE_Thread_Mutex>::instance()

#endif

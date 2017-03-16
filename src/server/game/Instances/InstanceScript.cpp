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

#include "InstanceScript.h"
#include "DatabaseEnv.h"
#include "Map.h"
#include "Player.h"
#include "GameObject.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Log.h"
#include "LFGMgr.h"
#include "RaidEncountersMgr.h"

InstanceScript::InstanceScript(Map* p_Map)
{
    instance = p_Map;
    completedEncounters = 0;
    m_ChallengeStarted = false;
    m_ConditionCompleted = false;
    m_StartChallengeTime = 0;
    m_ChallengeDoorGuid = 0;
    m_ChallengeTime = 0;
    m_MedalType = eChallengeMedals::MedalTypeNone;

    m_InstanceGuid = MAKE_NEW_GUID(p_Map->GetId(), 0, HighGuid::Instance);
    m_BeginningTime = 0;
    m_ScenarioID = 0;
    m_ScenarioStep = 0;
    m_EncounterTime = 0;

    m_InCombatResCount = 0;

    m_EncounterTime = 0;
}

void InstanceScript::SaveToDB()
{
    std::string data = GetSaveData();
    if (data.empty())
        return;

    if (RaidEncounter* mapEncounter = instance->GetRaidEncounter())
    {
        /// Raids don't use completedEncounters returned by GetCompletedEncounterMask, so it would always return here for raids, without saving...
        /// That's why we also need to check that instance isn't a raid.
        /// /\ 
        /// ||
        /// ???
        /// Raids use completedEncounters, just fill 'encounters' table
        /// And RaidEncounter is for raids, so IsRaid is wrong check
        if (!GetCompletedEncounterMask()/** && !instance->IsRaid()*/)
            return;

        /// Sync RaidEncounter CompletedMask / data with InstanceScript Data
        mapEncounter->data = data;
        mapEncounter->m_encounterMask = GetCompletedEncounterMask();

        Map::PlayerList const& lPlayers = instance->GetPlayers();

        if (!lPlayers.isEmpty())
        {
            SQLTransaction trans = CharacterDatabase.BeginTransaction();

            for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                if (Player* player = itr->getSource())
                {
                    RaidEncounter* playerEncounter = sRaidEncountersMgr->AddEncounterForPlayer(
                        player->GetGUIDLow(), instance->GetId(), GetCompletedEncounterMask(), data);

                    playerEncounter->SaveToDB(&trans);
                    playerEncounter->SwapRespawnTimesFromEncounter(mapEncounter);
                }
            }

            CharacterDatabase.CommitTransaction(trans);
        }
    }
    else
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_INSTANCE_DATA);
        stmt->setUInt32(0, GetCompletedEncounterMask());
        stmt->setString(1, data);
        stmt->setUInt32(2, instance->GetInstanceId());
        CharacterDatabase.Execute(stmt);
    }
}

void InstanceScript::HandleGameObject(uint64 GUID, bool open, GameObject* go)
{
    if (!go)
        go = instance->GetGameObject(GUID);
    if (go)
        go->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
    else
        sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript: HandleGameObject failed");
}

void InstanceScript::UpdateOperations(uint32 const p_Diff)
{
    for (auto l_It = m_TimedDelayedOperations.begin(); l_It != m_TimedDelayedOperations.end(); l_It++)
    {
        l_It->first -= p_Diff;

        if (l_It->first < 0)
        {
            l_It->second();
            l_It->second = nullptr;
        }
    }

    uint32 l_TimedDelayedOperationCountToRemove = std::count_if(std::begin(m_TimedDelayedOperations), std::end(m_TimedDelayedOperations), [](const std::pair<int32, std::function<void()>> & p_Pair) -> bool
    {
        return p_Pair.second == nullptr;
    });

    for (uint32 l_I = 0; l_I < l_TimedDelayedOperationCountToRemove; l_I++)
    {
        auto l_It = std::find_if(std::begin(m_TimedDelayedOperations), std::end(m_TimedDelayedOperations), [](const std::pair<int32, std::function<void()>> & p_Pair) -> bool
        {
            return p_Pair.second == nullptr;
        });

        if (l_It != std::end(m_TimedDelayedOperations))
            m_TimedDelayedOperations.erase(l_It);
    }

    if (m_TimedDelayedOperations.empty() && !m_EmptyWarned)
    {
        m_EmptyWarned = true;
        LastOperationCalled();
    }
}

bool InstanceScript::IsEncounterInProgress() const
{
    for (std::vector<BossInfo>::const_iterator itr = m_Bosses.begin(); itr != m_Bosses.end(); ++itr)
        if (itr->state == IN_PROGRESS)
            return true;

    return false;
}

void InstanceScript::OnPlayerEnter(Player* p_Player)
{
    SendScenarioState(ScenarioData(m_ScenarioID, m_ScenarioStep), p_Player);
}

void InstanceScript::OnPlayerExit(Player* p_Player)
{
    p_Player->RemoveAura(eInstanceSpells::SpellDetermination);
    p_Player->RemoveAurasByType(AuraType::SPELL_AURA_TRIGGER_BONUS_LOOT);
}

void InstanceScript::LoadMinionData(const MinionData* data)
{
    while (data->entry)
    {
        if (data->bossId < m_Bosses.size())
            minions.insert(std::make_pair(data->entry, MinionInfo(&m_Bosses[data->bossId])));

        ++data;
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadMinionData: " UI64FMTD " minions loaded.", uint64(minions.size()));
}

void InstanceScript::SetBossNumber(uint32 p_Number)
{
    m_Bosses.resize(p_Number);
    m_BossesScenarios.resize(p_Number);
}

void InstanceScript::LoadDoorData(const DoorData* data)
{
    while (data->entry)
    {
        if (data->bossId < m_Bosses.size())
            doors.insert(std::make_pair(data->entry, DoorInfo(&m_Bosses[data->bossId], data->type, BoundaryType(data->boundary))));

        ++data;
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadDoorData: " UI64FMTD " doors loaded.", uint64(doors.size()));
}

void InstanceScript::LoadScenariosInfos(BossScenarios const* p_Scenarios, uint32 p_ScenarioID)
{
    while (p_Scenarios->m_ScenarioID)
    {
        if (p_Scenarios->m_BossID < m_Bosses.size())
            m_BossesScenarios[p_Scenarios->m_BossID] = BossScenarios(p_Scenarios->m_BossID, p_Scenarios->m_ScenarioID);

        ++p_Scenarios;
    }

    m_ScenarioID = p_ScenarioID;
}

void InstanceScript::UpdateMinionState(Creature* minion, EncounterState state)
{
    switch (state)
    {
        case NOT_STARTED:
            if (!minion->isAlive())
                minion->Respawn();
            else if (minion->isInCombat())
                minion->AI()->EnterEvadeMode();
            break;
        case IN_PROGRESS:
            if (!minion->isAlive())
                minion->Respawn();
            else if (!minion->getVictim())
                minion->AI()->DoZoneInCombat();
            break;
        default:
            break;
    }
}

void InstanceScript::UpdateDoorState(GameObject* door)
{
    if (door == nullptr)
        return;

    DoorInfoMap::iterator lower = doors.lower_bound(door->GetEntry());
    DoorInfoMap::iterator upper = doors.upper_bound(door->GetEntry());
    if (lower == upper)
        return;

    bool open = true;
    for (DoorInfoMap::iterator itr = lower; itr != upper && open; ++itr)
    {
        switch (itr->second.type)
        {
            case DOOR_TYPE_ROOM:
                open = (itr->second.bossInfo->state != IN_PROGRESS);
                break;
            case DOOR_TYPE_PASSAGE:
                open = (itr->second.bossInfo->state == DONE);
                break;
            case DOOR_TYPE_SPAWN_HOLE:
                open = (itr->second.bossInfo->state == IN_PROGRESS);
                break;
            default:
                break;
        }
    }

    door->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
}

void InstanceScript::AddDoor(GameObject* door, bool add)
{
    if (door == nullptr)
        return;

    DoorInfoMap::iterator lower = doors.lower_bound(door->GetEntry());
    DoorInfoMap::iterator upper = doors.upper_bound(door->GetEntry());
    if (lower == upper)
        return;

    for (DoorInfoMap::iterator itr = lower; itr != upper; ++itr)
    {
        DoorInfo const& data = itr->second;

        if (add)
        {
            data.bossInfo->door[data.type].insert(door);
            switch (data.boundary)
            {
                default:
                case BOUNDARY_NONE:
                    break;
                case BOUNDARY_N:
                case BOUNDARY_S:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX();
                    break;
                case BOUNDARY_E:
                case BOUNDARY_W:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionY();
                    break;
                case BOUNDARY_NW:
                case BOUNDARY_SE:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX() + door->GetPositionY();
                    break;
                case BOUNDARY_NE:
                case BOUNDARY_SW:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX() - door->GetPositionY();
                    break;
            }
        }
        else
            data.bossInfo->door[data.type].erase(door);
    }

    if (add)
        UpdateDoorState(door);
}

void InstanceScript::AddMinion(Creature* minion, bool add)
{
    MinionInfoMap::iterator itr = minions.find(minion->GetEntry());
    if (itr == minions.end())
        return;

    if (add)
        itr->second.bossInfo->minion.insert(minion);
    else
        itr->second.bossInfo->minion.erase(minion);
}

bool InstanceScript::SetBossState(uint32 p_ID, EncounterState p_State)
{
    if (p_ID < m_Bosses.size())
    {
        BossInfo* l_BossInfo = &m_Bosses[p_ID];
        BossScenarios* l_BossScenario = &m_BossesScenarios[p_ID];

        if (l_BossInfo->state == TO_BE_DECIDED) // loading
        {
            l_BossInfo->state = p_State;
            return false;
        }
        else
        {
            if (l_BossInfo->state == p_State)
                return false;

            switch (p_State)
            {
                case EncounterState::DONE:
                {
                    for (MinionSet::iterator i = l_BossInfo->minion.begin(); i != l_BossInfo->minion.end(); ++i)
                    {
                        if ((*i)->isWorldBoss() && (*i)->isAlive())
                            return false;
                    }

                    SendScenarioProgressUpdate(CriteriaProgressData(l_BossScenario->m_ScenarioID, 1, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

                    /// This buff disappears immediately after killing the boss
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::SpellDetermination);

                    /// End of challenge
                    if (p_ID == (m_Bosses.size() - 1))
                    {
                        SendChallengeStopElapsedTimer(1);

                        if (instance->IsChallengeMode() && m_ChallengeStarted && m_ConditionCompleted)
                        {
                            m_ChallengeStarted = false;

                            SendChallengeNewPlayerRecord();

                            uint32 l_Money = 0;
                            uint32 l_Valor = 0;
                            RewardChallengers(l_Money, l_Valor);
                            SendChallengeModeComplete(l_Money, l_Valor);

                            SaveChallengeDatasIfNeeded();

                            DoUpdateAchievementCriteria(AchievementCriteriaTypes::ACHIEVEMENT_CRITERIA_TYPE_WIN_CHALLENGE_DUNGEON, instance->GetId(), m_MedalType);
                        }

                        /// End of dungeon for Dungeon Finding rewards
                        {
                            Map::PlayerList const& l_PlayerList = instance->GetPlayers();
                            if (!l_PlayerList.isEmpty())
                            {
                                for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                                {
                                    if (Player* l_Player = l_Itr->getSource())
                                    {
                                        uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                                        if (l_DungeonID)
                                            sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);
                                    }
                                }
                            }
                        }

                        SendScenarioState(ScenarioData(m_ScenarioID, ++m_ScenarioStep));
                    }

                    /// Handle Guild challenges
                    {
                        InstanceMap::PlayerList const& l_PlayersMap = instance->GetPlayers();
                        for (InstanceMap::PlayerList::const_iterator l_Itr = l_PlayersMap.begin(); l_Itr != l_PlayersMap.end(); ++l_Itr)
                        {
                            if (Player* l_Player = l_Itr->getSource())
                            {
                                if (l_Player->GetGroup() == nullptr
                                    || l_Player->GetMap() == nullptr
                                    || !l_Player->GetGroup()->IsGuildGroup(0, true, true))
                                    continue;

                                if (Guild* l_Guild = l_Player->GetGuild())
                                {
                                    if (instance->IsRaid())
                                        l_Guild->CompleteGuildChallenge(GuildChallengeType::ChallengeRaid);
                                    else if (instance->IsChallengeMode())
                                        l_Guild->CompleteGuildChallenge(GuildChallengeType::ChallengeDungeonChallenge);
                                    else if (instance->IsDungeon())
                                        l_Guild->CompleteGuildChallenge(GuildChallengeType::ChallengeDungeon);
                                }
                            }
                        }
                    }

                    m_EncounterTime = 0;

                    ResetCombatResurrection();

                    /// Bloodlust, Heroism, Temporal Displacement and Insanity debuffs are removed at the end of an encounter
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::HunterInsanity);
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::MageTemporalDisplacement);
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::ShamanExhaustion);
                    DoRemoveAurasDueToSpellOnPlayers(eInstanceSpells::ShamanSated);
                    break;
                }
                case EncounterState::IN_PROGRESS:
                {
                    m_EncounterTime = time(nullptr);
                    StartCombatResurrection();
                    break;
                }
                case EncounterState::FAIL:
                {
                    ResetCombatResurrection();

                    /// Now you have to fight for at least 3mins to get a stack.
                    /// It was nerfed due to people intentionally reseting the boss to gain max stack to kill the boss faster.
                    if (m_EncounterTime && instance->IsLFR() && (time(nullptr) - m_EncounterTime) >= 3 * TimeConstants::MINUTE)
                        DoCastSpellOnPlayers(eInstanceSpells::SpellDetermination);
                    break;
                }
                case EncounterState::NOT_STARTED:
                    ResetCombatResurrection();
                    break;
                default:
                    break;
            }

            l_BossInfo->state = p_State;
            SaveToDB();
        }

        for (uint32 l_Type = 0; l_Type < MAX_DOOR_TYPES; ++l_Type)
        {
            for (DoorSet::iterator l_Iter = l_BossInfo->door[l_Type].begin(); l_Iter != l_BossInfo->door[l_Type].end(); ++l_Iter)
                UpdateDoorState(*l_Iter);
        }

        for (MinionSet::iterator l_Iter = l_BossInfo->minion.begin(); l_Iter != l_BossInfo->minion.end(); ++l_Iter)
            UpdateMinionState(*l_Iter, p_State);

        return true;
    }

    return false;
}

std::string InstanceScript::LoadBossState(const char * data)
{
    if (!data)
        return NULL;
    std::istringstream loadStream(data);
    uint32 buff;
    uint32 bossId = 0;
    for (std::vector<BossInfo>::iterator i = m_Bosses.begin(); i != m_Bosses.end(); ++i, ++bossId)
    {
        loadStream >> buff;
        if (buff < TO_BE_DECIDED)
            SetBossState(bossId, (EncounterState)buff);
    }
    return loadStream.str();
}

std::string InstanceScript::GetBossSaveData()
{
    std::ostringstream saveStream;
    for (std::vector<BossInfo>::iterator i = m_Bosses.begin(); i != m_Bosses.end(); ++i)
        saveStream << (uint32)i->state << ' ';
    return saveStream.str();
}

void InstanceScript::DoUseDoorOrButton(uint64 uiGuid, uint32 uiWithRestoreTime, bool bUseAlternativeState)
{
    if (!uiGuid)
        return;

    GameObject* go = instance->GetGameObject(uiGuid);

    if (go)
    {
        if (go->GetGoType() == GAMEOBJECT_TYPE_DOOR || go->GetGoType() == GAMEOBJECT_TYPE_BUTTON)
        {
            if (go->getLootState() == GO_READY)
                go->UseDoorOrButton(uiWithRestoreTime, bUseAlternativeState);
            else if (go->getLootState() == GO_ACTIVATED)
                go->ResetDoorOrButton();
        }
        else
            sLog->outError(LOG_FILTER_GENERAL, "SD2: Script call DoUseDoorOrButton, but gameobject entry %u is type %u.", go->GetEntry(), go->GetGoType());
    }
}

void InstanceScript::DoRespawnGameObject(uint64 uiGuid, uint32 uiTimeToDespawn)
{
    if (GameObject* go = instance->GetGameObject(uiGuid))
    {
        //not expect any of these should ever be handled
        if (go->GetGoType() == GAMEOBJECT_TYPE_FISHINGNODE || go->GetGoType() == GAMEOBJECT_TYPE_DOOR ||
            go->GetGoType() == GAMEOBJECT_TYPE_BUTTON || go->GetGoType() == GAMEOBJECT_TYPE_TRAP)
            return;

        if (go->isSpawned())
            return;

        go->SetRespawnTime(uiTimeToDespawn);
        go->UpdateObjectVisibility();
    }
}

void InstanceScript::DoUpdateWorldState(uint32 uiStateId, uint32 uiStateData)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            if (Player* player = itr->getSource())
                player->SendUpdateWorldState(uiStateId, uiStateData);
    }
    else
        sLog->outDebug(LOG_FILTER_TSCR, "DoUpdateWorldState attempt send data but no players in map.");
}

// Send Notify to all players in instance
void InstanceScript::DoSendNotifyToInstance(char const* format, ...)
{
    InstanceMap::PlayerList const& players = instance->GetPlayers();

    if (!players.isEmpty())
    {
        va_list ap;
        va_start(ap, format);
        char buff[1024];
        vsnprintf(buff, 1024, format, ap);
        va_end(ap);
        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
            if (Player* player = i->getSource())
                if (WorldSession* session = player->GetSession())
                    session->SendNotification("%s", buff);
    }
}

// Reset Achievement Criteria for all players in instance
void InstanceScript::DoResetAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 /*= 0*/, uint64 miscValue2 /*= 0*/, bool evenIfCriteriaComplete /*= false*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->GetAchievementMgr().ResetAchievementCriteria(type, miscValue1, miscValue2, evenIfCriteriaComplete);
}

// Complete Achievement for all players in instance
void InstanceScript::DoCompleteAchievement(uint32 achievement)
{
  AchievementEntry const* pAE = sAchievementStore.LookupEntry(achievement);
  Map::PlayerList const &plrList = instance->GetPlayers();
  if (!pAE)
      return;

  if (!plrList.isEmpty())
      for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
          if (Player *pPlayer = i->getSource())
              pPlayer->CompletedAchievement(pAE);
}

// Update Achievement Criteria for all players in instance
void InstanceScript::DoUpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 /*= 0*/, uint32 miscValue2 /*= 0*/, uint32 miscValue3 /*=0*/, Unit* unit /*= NULL*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->UpdateAchievementCriteria(type, miscValue1, miscValue2, miscValue3, unit);
}

// Start timed achievement for all players in instance
void InstanceScript::DoStartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().StartTimedAchievement(type, entry);
}

// Stop timed achievement for all players in instance
void InstanceScript::DoStopTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().RemoveTimedAchievement(type, entry);
}

// Remove Auras due to Spell on all players in instance
void InstanceScript::DoRemoveAurasDueToSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();
    if (!PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            if (Player* player = itr->getSource())
            {
                player->RemoveAurasDueToSpell(spell);
                if (Pet* pet = player->GetPet())
                    pet->RemoveAurasDueToSpell(spell);
            }
        }
    }
}

// Remove aura from stack on all players in instance
void InstanceScript::DoRemoveAuraFromStackOnPlayers(uint32 spell, uint64 casterGUID, AuraRemoveMode mode, uint32 num)
{
    Map::PlayerList const& plrList = instance->GetPlayers();
    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator itr = plrList.begin(); itr != plrList.end(); ++itr)
            if (Player* pPlayer = itr->getSource())
                pPlayer->RemoveAuraFromStack(spell, casterGUID, mode, num);
}

// Cast spell on all players in instance
void InstanceScript::DoCastSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->CastSpell(player, spell, true);
}

// Add aura on all players in instance
void InstanceScript::DoAddAuraOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->AddAura(spell, player);
}

void InstanceScript::DoSetAlternatePowerOnPlayers(int32 value)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->SetPower(POWER_ALTERNATE_POWER, value);
}

void InstanceScript::DoModifyPlayerCurrencies(uint32 id, int32 value)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->ModifyCurrency(id, value);
}

void InstanceScript::DoNearTeleportPlayers(const Position pos, bool casting /*=false*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), casting);
}

void InstanceScript::DoStartMovie(uint32 movieId)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->SendMovieStart(movieId);

}

void InstanceScript::DoKilledMonsterKredit(uint32 questId, uint32 entry, uint64 guid/* =0*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();
    
    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                if (pPlayer->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->KilledMonsterCredit(entry, guid);
}

void InstanceScript::DoRemoveSpellCooldownOnPlayers(uint32 p_SpellID)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();

    if (!l_PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
        {
            if (Player* l_Player = l_Iter->getSource())
            {
                if (l_Player->HasSpellCooldown(p_SpellID))
                    l_Player->RemoveSpellCooldown(p_SpellID, true);
            }
        }
    }
}

bool InstanceScript::CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* /*source*/, Unit const* /*target*/ /*= NULL*/, uint32 /*miscvalue1*/ /*= 0*/)
{
    sLog->outError(LOG_FILTER_GENERAL, "Achievement system call InstanceScript::CheckAchievementCriteriaMeet but instance script for map %u not have implementation for achievement criteria %u",
        instance->GetId(), criteria_id);
    return false;
}

bool InstanceScript::CheckRequiredBosses(uint32 /*bossId*/, Player const* player) const
{
    if (player && player->isGameMaster())
        return true;

    if (instance->GetPlayersCountExceptGMs() > instance->ToInstanceMap()->GetMaxPlayers())
        return false;

    return true;
}

void InstanceScript::SendEncounterUnit(uint32 type, Unit* unit /*= NULL*/, uint8 param1 /*= 0*/, uint8 param2 /*= 0*/)
{
    // size of this packet is at most 15 (usually less)
    WorldPacket data(SMSG_UPDATE_INSTANCE_ENCOUNTER_UNIT, 15);
    data << uint32(type);

    switch (type)
    {
        case ENCOUNTER_FRAME_ENGAGE:
        case ENCOUNTER_FRAME_DISENGAGE:
        case ENCOUNTER_FRAME_UPDATE_PRIORITY:
            if (!unit)
                return;
            data.append(unit->GetPackGUID());
            data << uint8(param1);
            break;
        case ENCOUNTER_FRAME_ADD_TIMER:
        case ENCOUNTER_FRAME_ENABLE_OBJECTIVE:
        case ENCOUNTER_FRAME_DISABLE_OBJECTIVE:
        case ENCOUNTER_FRAME_SET_COMBAT_RES_LIMIT:
            data << uint8(param1);
            break;
        case ENCOUNTER_FRAME_UPDATE_OBJECTIVE:
            data << uint8(param1);
            data << uint8(param2);
            break;
        case ENCOUNTER_FRAME_UNK7:
        case ENCOUNTER_FRAME_ADD_COMBAT_RES_LIMIT:
        case ENCOUNTER_FRAME_RESET_COMBAT_RES_LIMIT:
        default:
            break;
    }

    instance->SendToPlayers(&data);
}

void InstanceScript::SendScenarioState(ScenarioData p_Data, Player* p_Player /*= nullptr*/)
{
    WorldPacket l_Data(Opcodes::SMSG_SCENARIO_STATE);

    l_Data << int32(0);
    l_Data << int32(p_Data.m_StepID);
    l_Data << int32(p_Data.m_ScenarioID);
    l_Data << int32(0);
    l_Data << int32(0);
    l_Data << int32(0);
    l_Data << int32(0);

    l_Data.WriteBit(false);
    l_Data.WriteBits(p_Data.m_CriteriaCount, 21);

    ByteBuffer l_ByteBuffer;

    for (CriteriaProgressData l_ProgressData : p_Data.m_CriteriaProgress)
    {
        ObjectGuid l_Guid = l_ProgressData.m_Guid;
        ObjectGuid l_Quantity = l_ProgressData.m_Quantity;

        l_Data.WriteBit(l_Guid[2]);
        l_Data.WriteBits(l_ProgressData.m_Flags, 4);
        l_Data.WriteBit(l_Guid[6]);
        l_Data.WriteBit(l_Quantity[2]);
        l_Data.WriteBit(l_Quantity[7]);
        l_Data.WriteBit(l_Guid[1]);
        l_Data.WriteBit(l_Quantity[3]);
        l_Data.WriteBit(l_Quantity[5]);
        l_Data.WriteBit(l_Guid[7]);
        l_Data.WriteBit(l_Guid[3]);
        l_Data.WriteBit(l_Guid[5]);
        l_Data.WriteBit(l_Quantity[4]);
        l_Data.WriteBit(l_Quantity[0]);
        l_Data.WriteBit(l_Quantity[1]);
        l_Data.WriteBit(l_Guid[4]);
        l_Data.WriteBit(l_Quantity[6]);
        l_Data.WriteBit(l_Guid[0]);

        l_ByteBuffer.WriteByteSeq(l_Guid[6]);
        l_ByteBuffer.WriteByteSeq(l_Quantity[3]);
        l_ByteBuffer.WriteByteSeq(l_Guid[3]);
        l_ByteBuffer << uint32(0);
        l_ByteBuffer << uint32(0);
        l_ByteBuffer.WriteByteSeq(l_Guid[0]);
        l_ByteBuffer.WriteByteSeq(l_Quantity[7]);
        l_ByteBuffer.WriteByteSeq(l_Guid[2]);
        l_ByteBuffer.WriteByteSeq(l_Quantity[1]);
        l_ByteBuffer.WriteByteSeq(l_Quantity[6]);
        l_ByteBuffer.WriteByteSeq(l_Quantity[0]);
        l_ByteBuffer.WriteByteSeq(l_Guid[1]);
        l_ByteBuffer.WriteByteSeq(l_Quantity[2]);
        l_ByteBuffer.WriteByteSeq(l_Guid[5]);
        l_ByteBuffer.WriteByteSeq(l_Quantity[4]);
        l_ByteBuffer << uint32(0);
        l_ByteBuffer.WriteByteSeq(l_Guid[4]);
        l_ByteBuffer.WriteByteSeq(l_Quantity[5]);
        l_ByteBuffer.WriteByteSeq(l_Guid[7]);
    }

    l_Data.WriteBit(false);

    if (l_ByteBuffer.size())
        l_Data.append(l_ByteBuffer);

    if (p_Player == nullptr)
        instance->SendToPlayers(&l_Data);
    else
        p_Player->SendDirectMessage(&l_Data);
}

void InstanceScript::SendScenarioProgressUpdate(CriteriaProgressData p_Data, Player* p_Player /*= nullptr*/)
{
    ObjectGuid l_Guid = p_Data.m_Guid;
    ObjectGuid l_Quantity = p_Data.m_Quantity;
    WorldPacket l_Data(Opcodes::SMSG_SCENARIO_PROGRESS_UPDATE);

    l_Data.WriteBit(l_Guid[7]);
    l_Data.WriteBit(l_Guid[0]);
    l_Data.WriteBit(l_Guid[4]);
    l_Data.WriteBits(p_Data.m_Flags, 4);
    l_Data.WriteBit(l_Quantity[3]);
    l_Data.WriteBit(l_Quantity[4]);
    l_Data.WriteBit(l_Quantity[0]);
    l_Data.WriteBit(l_Guid[6]);
    l_Data.WriteBit(l_Quantity[2]);
    l_Data.WriteBit(l_Guid[3]);
    l_Data.WriteBit(l_Quantity[7]);
    l_Data.WriteBit(l_Guid[5]);
    l_Data.WriteBit(l_Quantity[6]);
    l_Data.WriteBit(l_Quantity[5]);
    l_Data.WriteBit(l_Quantity[1]);
    l_Data.WriteBit(l_Guid[2]);
    l_Data.WriteBit(l_Guid[1]);

    l_Data.WriteByteSeq(l_Guid[5]);
    l_Data.WriteByteSeq(l_Quantity[2]);
    l_Data.WriteByteSeq(l_Quantity[6]);
    l_Data.WriteByteSeq(l_Guid[4]);
    l_Data.WriteByteSeq(l_Quantity[4]);
    l_Data.WriteByteSeq(l_Guid[6]);
    l_Data.WriteByteSeq(l_Quantity[3]);
    l_Data << uint32(secsToTimeBitFields(p_Data.m_Date));
    l_Data.WriteByteSeq(l_Guid[7]);
    l_Data.WriteByteSeq(l_Quantity[5]);
    l_Data.WriteByteSeq(l_Quantity[7]);
    l_Data.WriteByteSeq(l_Quantity[0]);
    l_Data.WriteByteSeq(l_Guid[1]);
    l_Data.WriteByteSeq(l_Quantity[1]);
    l_Data.WriteByteSeq(l_Guid[2]);
    l_Data.WriteByteSeq(l_Guid[3]);
    l_Data << int32(p_Data.m_TimeFromStart);
    l_Data << int32(p_Data.m_TimeFromCreate);
    l_Data << int32(p_Data.m_ID);
    l_Data.WriteByteSeq(l_Guid[0]);

    if (p_Player == nullptr)
        instance->SendToPlayers(&l_Data);
    else
        p_Player->SendDirectMessage(&l_Data);
}

//////////////////////////////////////////////////////////////////////////
/// ChallengesMode
void InstanceScript::SendChallengeStartTimer(uint32 p_Time)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            l_Player->SendStartTimer(p_Time, p_Time, CHALLENGE_TIMER);
    }
}

void InstanceScript::SendChallengeStartElapsedTimer(uint32 p_TimerID, uint32 p_Time)
{
    WorldPacket l_Data(Opcodes::SMSG_START_ELAPSED_TIMER, 8);
    l_Data << uint32(p_Time);
    l_Data << uint32(p_TimerID);
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SendChallengeStopElapsedTimer(uint32 p_TimerID, bool p_KeepTimer /*= false*/)
{
    WorldPacket l_Data(Opcodes::SMSG_STOP_ELAPSED_TIMER, 5);
    l_Data.WriteBit(p_KeepTimer);
    l_Data << uint32(p_TimerID);
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SetChallengeModeStarted()
{
    if (m_ChallengeStarted || !instance->IsChallengeMode())
        return;

    m_ChallengeStarted = true;

    /// Challenge mode start timer is 5 seconds
    SendChallengeStartTimer(5);
    m_StartChallengeTime = 5000;
}

void InstanceScript::ScheduleChallengeStartup(uint32 p_Diff)
{
    if (m_StartChallengeTime)
    {
        if (m_StartChallengeTime <= p_Diff)
        {
            m_StartChallengeTime = 0;

            if (GameObject* l_ChallengeDoor = instance->GetGameObject(m_ChallengeDoorGuid))
                l_ChallengeDoor->SetGoState(GO_STATE_ACTIVE);

            SendChallengeStartElapsedTimer(1, 0);

            m_MedalType = eChallengeMedals::MedalTypeGold;
        }
        else
            m_StartChallengeTime -= p_Diff;
    }
}

void InstanceScript::ScheduleChallengeTimeUpdate(uint32 p_Diff)
{
    if (!m_ChallengeStarted || m_StartChallengeTime)
        return;

    m_ChallengeTime += p_Diff;

    MapChallengeModeEntry const* l_ChallengeEntry = nullptr;
    for (uint32 l_I = 0; l_I < sMapChallengeModeStore.GetNumRows(); ++l_I)
    {
        if (MapChallengeModeEntry const* l_Challenge = sMapChallengeModeStore.LookupEntry(l_I))
        {
            if (l_Challenge->MapID == instance->GetId())
            {
                l_ChallengeEntry = l_Challenge;
                break;
            }
        }
    }

    /// Should not happens
    if (l_ChallengeEntry == nullptr)
        return;

    uint32 l_Times[eChallengeMedals::MedalTypeGold];
    MapChallengeModeHotfix* l_ChallengeHotfix = sObjectMgr->GetMapChallengeModeHotfix(l_ChallengeEntry->ID);
    if (l_ChallengeHotfix != nullptr)
    {
        l_Times[eChallengeMedals::MedalTypeBronze - 1] = l_ChallengeHotfix->m_BronzeTime * TimeConstants::IN_MILLISECONDS;
        l_Times[eChallengeMedals::MedalTypeSilver - 1] = l_ChallengeHotfix->m_SilverTime * TimeConstants::IN_MILLISECONDS;
        l_Times[eChallengeMedals::MedalTypeGold - 1] = l_ChallengeHotfix->m_GoldTime * TimeConstants::IN_MILLISECONDS;
    }
    else
    {
        l_Times[eChallengeMedals::MedalTypeBronze - 1] = l_ChallengeEntry->BronzeTime * TimeConstants::IN_MILLISECONDS;
        l_Times[eChallengeMedals::MedalTypeSilver - 1] = l_ChallengeEntry->SilverTime * TimeConstants::IN_MILLISECONDS;
        l_Times[eChallengeMedals::MedalTypeGold - 1] = l_ChallengeEntry->GoldTime * TimeConstants::IN_MILLISECONDS;
    }

    /// Downgrade Medal if needed
    if (m_MedalType > eChallengeMedals::MedalTypeNone && m_ChallengeTime > l_Times[m_MedalType - 1])
        --m_MedalType;
}

void InstanceScript::ScheduleBeginningTimeUpdate(uint32 p_Diff)
{
    m_BeginningTime += p_Diff;
}

void InstanceScript::SendChallengeNewPlayerRecord()
{
    uint32 l_MapID = instance->GetId();

    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (l_Player->HasChallengeCompleted(l_MapID))
            {
                CompletedChallenge* l_Challenge = l_Player->GetCompletedChallenge(l_MapID);
                /// Should not happens
                if (l_Challenge == nullptr)
                    continue;

                bool l_NewBestTime = m_ChallengeTime < l_Challenge->m_BestTime;
                bool l_NewBestMedal = m_MedalType > l_Challenge->m_BestMedal;

                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_COMPLETED_CHALLENGE);
                l_Statement->setUInt32(0, l_NewBestTime ? m_ChallengeTime : l_Challenge->m_BestTime);
                l_Statement->setUInt32(1, m_ChallengeTime);
                l_Statement->setUInt8(2, l_NewBestMedal ? m_MedalType : l_Challenge->m_BestMedal);
                l_Statement->setUInt32(3, l_NewBestMedal ? time(NULL) : l_Challenge->m_BestMedalDate);
                l_Statement->setUInt32(4, l_Player->GetGUIDLow());
                l_Statement->setUInt32(5, l_MapID);
                CharacterDatabase.Execute(l_Statement);

                if (l_NewBestMedal)
                {
                    l_Challenge->m_BestMedal = m_MedalType;
                    l_Challenge->m_BestMedalDate = time(nullptr);
                }

                if (l_NewBestTime)
                    l_Challenge->m_BestTime = m_ChallengeTime;

                l_Challenge->m_LastTime = m_ChallengeTime;

                /// Send new record only for new best time
                if (!l_NewBestTime)
                    continue;
            }
            else
            {
                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_COMPLETED_CHALLENGE);
                l_Statement->setUInt32(0, l_Player->GetGUIDLow());
                l_Statement->setUInt32(1, l_MapID);
                l_Statement->setUInt32(2, m_ChallengeTime);
                l_Statement->setUInt32(3, m_ChallengeTime);
                l_Statement->setUInt8(4, m_MedalType);
                l_Statement->setUInt32(5, time(nullptr));
                CharacterDatabase.Execute(l_Statement);

                CompletedChallenge l_Challenge;
                l_Challenge.m_BestMedal = m_MedalType;
                l_Challenge.m_BestMedalDate = time(nullptr);
                l_Challenge.m_BestTime = m_ChallengeTime;
                l_Challenge.m_LastTime = m_ChallengeTime;

                l_Player->AddCompletedChallenge(l_MapID, l_Challenge);
                l_Player->GetSession()->SendChallengeModeMapStatsUpdate(l_MapID);
            }

            WorldPacket l_Data(Opcodes::SMSG_CHALLENGE_MODE_NEW_PLAYER_RECORD, 12);
            l_Data << uint32(m_MedalType);
            l_Data << uint32(m_ChallengeTime);
            l_Data << uint32(l_MapID);
            l_Player->SendDirectMessage(&l_Data);
        }
    }
}

void InstanceScript::SendChallengeModeComplete(uint32 p_Money, uint32 p_Valor)
{
    uint32 l_ItemRewards = 0;
    uint32 l_CurrencyRewards = p_Valor != 0;
    WorldPacket l_Data(Opcodes::SMSG_CHALLENGE_MODE_COMPLETE, 4 * 4 + (l_ItemRewards * 12) + (l_CurrencyRewards * 8));

    /// Reward
    {
        l_Data.WriteBits(l_CurrencyRewards, 21);
        l_Data.WriteBits(l_ItemRewards, 20);

        /// ItemReward
        for (uint32 l_I = 0; l_I < l_ItemRewards; ++l_I)
        {
            /// ItemStruct
            {
                l_Data << int32(0);     ///< RandomPropertiesID
                l_Data << int32(0);     ///< RandomPropertiesSeed
                l_Data << int32(0);     ///< ItemEntry
            }
        }

        l_Data << uint32(m_MedalType);
        l_Data << uint32(p_Money);
        l_Data << uint32(m_ChallengeTime);

        /// CurrencyReward
        for (uint32 l_I = 0; l_I < l_CurrencyRewards; ++l_I)
        {
            l_Data << uint32(p_Valor);
            l_Data << uint32(CurrencyTypes::CURRENCY_TYPE_VALOR_POINTS);
        }

        l_Data << uint32(instance->GetId());
    }

    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SaveChallengeDatasIfNeeded()
{
    uint32 l_MapID = instance->GetId();
    RealmCompletedChallenge* l_GroupChallenge = sObjectMgr->GetGroupCompletedChallengeForMap(l_MapID);
    RealmCompletedChallenge* l_GuildChallenge = sObjectMgr->GetGuildCompletedChallengeForMap(l_MapID);

    SaveNewGroupChallenge();

    /// Delete old group record if it's a new realm-best time (or if it's the first), and reward titles/achievements
    if (l_GroupChallenge == nullptr || (l_GroupChallenge != nullptr && l_GroupChallenge->m_CompletionTime > m_ChallengeTime))
    {
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_CHALLENGE);
        l_Statement->setUInt32(0, l_MapID);
        CharacterDatabase.Execute(l_Statement);

        RewardNewRealmRecord(l_GroupChallenge);
    }

    bool l_GuildGroup = false;
    uint32 l_GuildID = 0;
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (Group* l_Group = l_Player->GetGroup())
            {
                if (!l_Group->IsGuildGroup(l_Player->GetGuildId(), true, true))
                    continue;

                l_GuildGroup = true;
                l_GuildID = l_Player->GetGuildId();
            }
        }
    }

    /// New best time for the guild
    if (l_GuildGroup)
    {
        SaveNewGroupChallenge(l_GuildID);

        /// Delete old guild record if it's a new realm-best time
        if (l_GuildChallenge != nullptr && l_GuildChallenge->m_CompletionTime > m_ChallengeTime)
        {
            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_CHALLENGE);
            l_Statement->setUInt32(0, l_MapID);
            l_Statement->setUInt32(1, l_GuildID);
            CharacterDatabase.Execute(l_Statement);
        }
    }
}

void InstanceScript::SaveNewGroupChallenge(uint32 p_GuildID /*= 0*/)
{
    uint32 l_Index = 0;
    uint32 l_MapID = instance->GetId();
    uint32 l_Date  = time(nullptr);

    RealmCompletedChallenge l_NewGroup;
    l_NewGroup.m_AttemptID      = 0;
    l_NewGroup.m_CompletionDate = l_Date;
    l_NewGroup.m_CompletionTime = m_ChallengeTime;
    l_NewGroup.m_GuildID        = p_GuildID;
    l_NewGroup.m_MedalEarned    = m_MedalType;

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(p_GuildID ? CHAR_INS_GUILD_CHALLENGE : CHAR_INS_GROUP_CHALLENGE);

    l_Statement->setUInt32(l_Index++, instance->GetId());

    if (p_GuildID)
        l_Statement->setUInt32(l_Index++, p_GuildID);

    l_Statement->setUInt32(l_Index++, 0);   ///< AttemptID
    l_Statement->setUInt32(l_Index++, m_ChallengeTime);
    l_Statement->setUInt32(l_Index++, l_Date);
    l_Statement->setUInt8(l_Index++, m_MedalType);

    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    uint32 l_Count = 0;
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            ++l_Count;
    }

    l_Statement->setUInt8(l_Index++, l_Count);
    l_NewGroup.m_MembersCount = l_Count;

    uint8 l_J = 0;
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end() && l_J < 5; ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            l_NewGroup.m_Members[l_J].m_Guid = MAKE_NEW_GUID(l_Player->GetGUIDLow(), 0, HighGuid::Player);
            l_NewGroup.m_Members[l_J].m_SpecID = l_Player->GetSpecializationId(l_Player->GetActiveSpec());
            ++l_J;

            l_Statement->setUInt32(l_Index++, l_Player->GetGUIDLow());
            l_Statement->setUInt32(l_Index++, l_Player->GetSpecializationId(l_Player->GetActiveSpec()));
        }
    }

    if (l_Count < 5)
    {
        for (uint8 l_I = 0; l_I < (5 - l_Count); ++l_I)
        {
            l_NewGroup.m_Members[l_J].m_Guid = 0;
            l_NewGroup.m_Members[l_J].m_SpecID = 0;
            ++l_J;

            l_Statement->setUInt32(l_Index++, 0);
            l_Statement->setUInt32(l_Index++, 0);
        }
    }

    CharacterDatabase.Execute(l_Statement);

    sObjectMgr->AddGroupCompletedChallenge(l_MapID, l_NewGroup);

    if (p_GuildID)
        sObjectMgr->AddGuildCompletedChallenge(l_MapID, l_NewGroup);
}

void InstanceScript::RewardChallengers(uint32& p_Money, uint32& p_Valor)
{
    ChallengeReward* l_Reward = sObjectMgr->GetChallengeRewardsForMap(instance->GetId());
    if (l_Reward == nullptr)
        return;

    if (m_MedalType < eChallengeMedals::MaxMedalType)
    {
        p_Money = l_Reward->MoneyReward[m_MedalType];
        p_Valor = l_Reward->ValorReward[m_MedalType];

        Map::PlayerList const& l_PlayerList = instance->GetPlayers();
        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
        {
            if (Player* l_Player = l_Iter->getSource())
            {
                if (p_Money)
                    l_Player->ModifyMoney(int64(p_Money));

                if (p_Valor)
                    l_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_VALOR_POINTS, p_Valor, true);
            }
        }

        return;
    }

    return;
}

void InstanceScript::RewardNewRealmRecord(RealmCompletedChallenge* p_OldChallenge /*= nullptr*/)
{
    ChallengeReward* l_Reward = sObjectMgr->GetChallengeRewardsForMap(instance->GetId());
    if (l_Reward == nullptr)
        return;

    CharTitlesEntry const* l_Title = sCharTitlesStore.LookupEntry(l_Reward->TitleID);
    if (l_Title == nullptr)
        return;

    AchievementEntry const* l_Achievement = sAchievementStore.LookupEntry(l_Reward->AchievementID);
    if (l_Achievement == nullptr)
        return;

    /// Remove title to previous challengers - Achievement will stay
    if (p_OldChallenge != nullptr)
    {
        SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

        for (uint8 l_I = 0; l_I < 5; ++l_I)
        {
            /// Check if player is online, then remove the title
            if (Player* l_Player = HashMapHolder<Player>::Find(p_OldChallenge->m_Members[l_I].m_Guid))
                l_Player->SetTitle(l_Title, true);
            else
            {
                uint32 l_Index = l_Title->bit_index / 32;
                uint32 l_Flag = 1 << (l_Title->bit_index % 32);
                uint32 l_LowGuid = GUID_LOPART(p_OldChallenge->m_Members[l_I].m_Guid);

                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_TITLES);
                l_Statement->setUInt32(0, l_LowGuid);

                if (PreparedQueryResult l_Result = CharacterDatabase.Query(l_Statement))
                {
                    Field* l_Fields = l_Result->Fetch();
                    char const* l_KnownTitlesStr = l_Fields[0].GetCString();

                    /// Title removal
                    if (l_KnownTitlesStr)
                    {
                        uint32 const l_TitleSize = KNOWN_TITLES_SIZE * 2;
                        uint32 l_KnownTitles[l_TitleSize];
                        Tokenizer l_Tokens(l_KnownTitlesStr, ' ', l_TitleSize);

                        uint32 l_ActualSize = l_Tokens.size();

                        for (uint32 l_J = 0; l_J < l_TitleSize; ++l_J)
                        {
                            if (l_J < l_ActualSize)
                                l_KnownTitles[l_J] = atol(l_Tokens[l_J]);
                            else
                                l_KnownTitles[l_J] = 0;
                        }

                        l_KnownTitles[l_Index] &= ~l_Flag;

                        std::ostringstream l_Stream;
                        for (uint32 l_J = 0; l_J < l_TitleSize; ++l_J)
                            l_Stream << l_KnownTitles[l_J] << ' ';

                        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TITLES_FACTION_CHANGE);
                        l_Statement->setString(0, l_Stream.str().c_str());
                        l_Statement->setUInt32(1, l_LowGuid);
                        l_Transaction->Append(l_Statement);

                        /// Unset any currently chosen title
                        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_RES_CHAR_TITLES_FACTION_CHANGE);
                        l_Statement->setUInt32(0, l_LowGuid);
                        l_Transaction->Append(l_Statement);
                    }
                }
            }
        }

        CharacterDatabase.CommitTransaction(l_Transaction);
    }

    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            l_Player->SetTitle(l_Title);
            l_Player->CompletedAchievement(l_Achievement);
        }
    }
}
//////////////////////////////////////////////////////////////////////////

bool InstanceScript::IsWipe()
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();

    if (PlayerList.isEmpty())
        return true;

    for (Map::PlayerList::const_iterator Itr = PlayerList.begin(); Itr != PlayerList.end(); ++Itr)
    {
        Player* player = Itr->getSource();

        if (!player)
            continue;

        if (player->isAlive() && !player->isGameMaster())
            return false;
    }

    return true;
}

void InstanceScript::UpdateEncounterState(EncounterCreditType type, uint32 creditEntry, Unit* source)
{
    DungeonEncounterList const* encounters = sObjectMgr->GetDungeonEncounterList(instance->GetId(), instance->GetDifficulty());
    if (!encounters)
        encounters = sObjectMgr->GetDungeonEncounterList(instance->GetId(), Difficulty(0));

    if (!encounters)
        return;

    for (DungeonEncounterList::const_iterator itr = encounters->begin(); itr != encounters->end(); ++itr)
    {
        if ((*itr)->creditType == type && (*itr)->creditEntry == creditEntry)
        {
            completedEncounters |= 1 << (*itr)->dbcEntry->encounterIndex;
            sLog->outDebug(LOG_FILTER_TSCR, "Instance %s (instanceId %u) completed encounter %s", instance->GetMapName(), instance->GetInstanceId(), (*itr)->dbcEntry->encounterName);
            if (uint32 dungeonId = (*itr)->lastEncounterDungeon)
            {
                Map::PlayerList const& players = instance->GetPlayers();
                if (!players.isEmpty())
                    for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                        if (Player* player = i->getSource())
                            if (!source || player->IsAtGroupRewardDistance(source))
                                sLFGMgr->RewardDungeonDoneFor(dungeonId, player);
            }
        }
    }
}

void InstanceScript::UpdatePhasing()
{
    PhaseUpdateData phaseUdateData;
    phaseUdateData.AddConditionType(CONDITION_INSTANCE_DATA);

    Map::PlayerList const& players = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        if (Player* player = itr->getSource())
            player->GetPhaseMgr().NotifyConditionChanged(phaseUdateData);
}


bool ScenarioStep::AddCriteria(uint32 criteriaId, uint32 totalCount)
{
    if (m_Criterias.find(criteriaId) != m_Criterias.end())
        return false;

    ScenarioStepCriteria& criteria = m_Criterias[criteriaId];
    criteria.CriteriaId = criteriaId;
    criteria.CurrentCount = 0;
    criteria.TotalCount = totalCount;

    return true;
}

bool ScenarioStep::UpdateCriteria(uint32 criteriaId, uint32 count)
{
    if (m_Criterias.find(criteriaId) == m_Criterias.end())
        return false;

    m_Criterias[criteriaId].CurrentCount += count;

    return true;
}

uint32 ScenarioStep::GetCriteriaCount(uint32 criteriaId)
{
    if (m_Criterias.find(criteriaId) == m_Criterias.end())
        return 0;

    return m_Criterias[criteriaId].CurrentCount;
}

bool ScenarioStep::IsCompleted() const
{
    for (ScenarioCriteriaMap::const_iterator itr = m_Criterias.begin(); itr != m_Criterias.end(); ++itr)
    {
        if (!itr->second.IsCompleted())
            return false;
    }

    return true;
}

ScenarioController::ScenarioController(Map* map, uint32 scenarioId, uint32 maxStep) :
m_Map(map), m_ScenarioId(scenarioId), m_MaxStep(maxStep)
{
    
    m_CurrentStep = STEP_1;
}

ScenarioStep& ScenarioController::GetStep(uint32 step)
{
    return m_Steps[step];
}

void ScenarioController::UpdateCurrentStepCriteria(uint32 criteriaId, uint32 count)
{
    if (m_Steps.find(m_CurrentStep) == m_Steps.end())
        return;

    ScenarioStep& step = m_Steps[m_CurrentStep];

    if (!step.UpdateCriteria(criteriaId, count))
        return;

    SendScenarioProgressToAll(criteriaId);

    if (step.IsCompleted())
    {
        if (m_CurrentStep < m_MaxStep)
        {
            m_CurrentStep++;

            SendScenarioStateToAll();
        }
    }
}

uint32 ScenarioController::GetCurrentStepCriteriaCount(uint32 criteriaId)
{
    if (m_Steps.find(m_CurrentStep) == m_Steps.end())
        return 0;

    ScenarioStep& step = m_Steps[m_CurrentStep];

    return step.GetCriteriaCount(criteriaId);
}

bool ScenarioController::IsCompleted()
{
    if (m_Steps.find(m_CurrentStep) == m_Steps.end())
        return false;

    return GetStep(m_CurrentStep).IsCompleted();
}

void ScenarioController::SendScenarioProgressToAll(uint32 criteriaId)
{
    ObjectGuid counter = GetCurrentStepCriteriaCount(criteriaId);

    for (Map::PlayerList::const_iterator itr = m_Map->GetPlayers().begin(); itr != m_Map->GetPlayers().end(); ++itr)
    {
        Player* player = itr->getSource();
        if (!player)
            continue;

        WorldPacket packet(SMSG_SCENARIO_PROGRESS_UPDATE);
        
        ObjectGuid playerGuid = player->GetGUID();

        uint32 bits4 = 0;

        uint32 uint32_1 = 0;
        uint32 uint32_2 = 0;

        packet.WriteBit(playerGuid[7]);
        packet.WriteBit(playerGuid[0]);
        packet.WriteBit(playerGuid[4]);
        packet.WriteBits(bits4, 4);
        packet.WriteBit(counter[3]);
        packet.WriteBit(counter[4]);
        packet.WriteBit(counter[0]);
        packet.WriteBit(playerGuid[6]);
        packet.WriteBit(counter[2]);
        packet.WriteBit(playerGuid[3]);
        packet.WriteBit(counter[7]);
        packet.WriteBit(playerGuid[5]);
        packet.WriteBit(counter[6]);
        packet.WriteBit(counter[5]);
        packet.WriteBit(counter[1]);
        packet.WriteBit(playerGuid[2]);
        packet.WriteBit(playerGuid[1]);

        packet.WriteByteSeq(playerGuid[5]);
        packet.WriteByteSeq(counter[2]);
        packet.WriteByteSeq(counter[6]);
        packet.WriteByteSeq(playerGuid[4]);
        packet.WriteByteSeq(counter[4]);
        packet.WriteByteSeq(playerGuid[6]);
        packet.WriteByteSeq(counter[3]);
        packet << getMSTime();
        packet.WriteByteSeq(playerGuid[7]);
        packet.WriteByteSeq(counter[5]);
        packet.WriteByteSeq(counter[7]);
        packet.WriteByteSeq(counter[0]);
        packet.WriteByteSeq(playerGuid[1]);
        packet.WriteByteSeq(counter[1]);
        packet.WriteByteSeq(playerGuid[2]);
        packet.WriteByteSeq(playerGuid[3]);

        packet << uint32_1;
        packet << uint32_2;
        packet << criteriaId;

        packet.WriteByteSeq(playerGuid[0]);

        player->GetSession()->SendPacket(&packet);
    }
}

void ScenarioController::SendScenarioProgressToAll(uint32 criteriaId, uint32 count)
{
    ObjectGuid counter = count;

    for (Map::PlayerList::const_iterator itr = m_Map->GetPlayers().begin(); itr != m_Map->GetPlayers().end(); ++itr)
    {
        Player* player = itr->getSource();
        if (!player)
            continue;

        WorldPacket packet(SMSG_SCENARIO_PROGRESS_UPDATE);
        
        ObjectGuid playerGuid = player->GetGUID();

        uint32 bits4 = 0;

        uint32 uint32_1 = 0;
        uint32 uint32_2 = 0;

        packet.WriteBit(playerGuid[7]);
        packet.WriteBit(playerGuid[0]);
        packet.WriteBit(playerGuid[4]);
        packet.WriteBits(bits4, 4);
        packet.WriteBit(counter[3]);
        packet.WriteBit(counter[4]);
        packet.WriteBit(counter[0]);
        packet.WriteBit(playerGuid[6]);
        packet.WriteBit(counter[2]);
        packet.WriteBit(playerGuid[3]);
        packet.WriteBit(counter[7]);
        packet.WriteBit(playerGuid[5]);
        packet.WriteBit(counter[6]);
        packet.WriteBit(counter[5]);
        packet.WriteBit(counter[1]);
        packet.WriteBit(playerGuid[2]);
        packet.WriteBit(playerGuid[1]);

        packet.WriteByteSeq(playerGuid[5]);
        packet.WriteByteSeq(counter[2]);
        packet.WriteByteSeq(counter[6]);
        packet.WriteByteSeq(playerGuid[4]);
        packet.WriteByteSeq(counter[4]);
        packet.WriteByteSeq(playerGuid[6]);
        packet.WriteByteSeq(counter[3]);
        packet << getMSTime();
        packet.WriteByteSeq(playerGuid[7]);
        packet.WriteByteSeq(counter[5]);
        packet.WriteByteSeq(counter[7]);
        packet.WriteByteSeq(counter[0]);
        packet.WriteByteSeq(playerGuid[1]);
        packet.WriteByteSeq(counter[1]);
        packet.WriteByteSeq(playerGuid[2]);
        packet.WriteByteSeq(playerGuid[3]);

        packet << uint32_1;
        packet << uint32_2;
        packet << criteriaId;

        packet.WriteByteSeq(playerGuid[0]);

        player->GetSession()->SendPacket(&packet);
    }
}

void ScenarioController::SendScenarioState(Player* player)
{
    WorldPacket packet(SMSG_SCENARIO_STATE);

    uint32 uint32_1 = 0;
    uint32 uint32_4 = 0;
    uint32 uint32_5 = 0;
    uint32 uint32_6 = 0;
    uint32 uint32_7 = 0; // bonus step

    bool bit1 = false;
    bool bit2 = false;

    uint32 bits19_1 = 0;

    packet << uint32_1;
    packet << GetCurrentStep(); 
    packet << m_ScenarioId; 
    packet << uint32_4;
    packet << uint32_5;
    packet << uint32_6;
    packet << uint32_7; // bonus step

    packet.WriteBit(bit1);

    packet.WriteBits(bits19_1, 19);
    
    /*guid1[i][2] = packet.ReadBit();
                bits18 = (int)packet.ReadBits(4);
                guid1[i][6] = packet.ReadBit();
                guid2[i][2] = packet.ReadBit();
                guid2[i][7] = packet.ReadBit();
                guid1[i][1] = packet.ReadBit();
                guid2[i][3] = packet.ReadBit();
                guid2[i][5] = packet.ReadBit();
                guid1[i][7] = packet.ReadBit();
                guid1[i][3] = packet.ReadBit();
                guid1[i][5] = packet.ReadBit();
                guid2[i][4] = packet.ReadBit();
                guid2[i][0] = packet.ReadBit();
                guid2[i][1] = packet.ReadBit();
                guid1[i][4] = packet.ReadBit();
                guid2[i][6] = packet.ReadBit();
                guid1[i][0] = packet.ReadBit();*/

    packet.WriteBit(bit2);

    /* for (var i = 0; i < bits10; ++i)
            {
                packet.ReadXORByte(guid1[i], 6);
                packet.ReadXORByte(guid2[i], 3);
                packet.ReadXORByte(guid1[i], 3);
                packet.ReadInt32("Int54");
                packet.ReadInt32("Int60");
                packet.ReadXORByte(guid1[i], 0);
                packet.ReadXORByte(guid2[i], 7);
                packet.ReadXORByte(guid1[i], 2);
                packet.ReadXORByte(guid2[i], 1);
                packet.ReadXORByte(guid2[i], 6);
                packet.ReadXORByte(guid2[i], 0);
                packet.ReadXORByte(guid1[i], 1);
                packet.ReadXORByte(guid2[i], 2);
                packet.ReadXORByte(guid1[i], 5);
                packet.ReadXORByte(guid2[i], 4);
                packet.ReadPackedTime("Time", i);
                packet.ReadXORByte(guid1[i], 4);
                packet.ReadXORByte(guid2[i], 5);
                packet.ReadInt32("Int14");
                packet.ReadXORByte(guid1[i], 7);
                packet.WriteGuid("Guid1", guid1[i], i);
                packet.WriteGuid("Guid2", guid2[i], i);
            }*/
    player->GetSession()->SendPacket(&packet);
}

void ScenarioController::SendScenarioStateToAll()
{
    WorldPacket packet(SMSG_SCENARIO_STATE);

    uint32 uint32_1 = 0;
    uint32 uint32_4 = 0;
    uint32 uint32_5 = 0;
    uint32 uint32_6 = 0;
    uint32 uint32_7 = 0; // bonus step

    bool bit1 = false;
    bool bit2 = false;

    uint32 bits19_1 = 0;

    packet << uint32_1;
    packet << GetCurrentStep(); 
    packet << m_ScenarioId; 
    packet << uint32_4;
    packet << uint32_5;
    packet << uint32_6;
    packet << uint32_7; // bonus step

    packet.WriteBit(bit1);

    packet.WriteBits(bits19_1, 19);
    
    /*guid1[i][2] = packet.ReadBit();
                bits18 = (int)packet.ReadBits(4);
                guid1[i][6] = packet.ReadBit();
                guid2[i][2] = packet.ReadBit();
                guid2[i][7] = packet.ReadBit();
                guid1[i][1] = packet.ReadBit();
                guid2[i][3] = packet.ReadBit();
                guid2[i][5] = packet.ReadBit();
                guid1[i][7] = packet.ReadBit();
                guid1[i][3] = packet.ReadBit();
                guid1[i][5] = packet.ReadBit();
                guid2[i][4] = packet.ReadBit();
                guid2[i][0] = packet.ReadBit();
                guid2[i][1] = packet.ReadBit();
                guid1[i][4] = packet.ReadBit();
                guid2[i][6] = packet.ReadBit();
                guid1[i][0] = packet.ReadBit();*/

    packet.WriteBit(bit2);

    /* for (var i = 0; i < bits10; ++i)
            {
                packet.ReadXORByte(guid1[i], 6);
                packet.ReadXORByte(guid2[i], 3);
                packet.ReadXORByte(guid1[i], 3);
                packet.ReadInt32("Int54");
                packet.ReadInt32("Int60");
                packet.ReadXORByte(guid1[i], 0);
                packet.ReadXORByte(guid2[i], 7);
                packet.ReadXORByte(guid1[i], 2);
                packet.ReadXORByte(guid2[i], 1);
                packet.ReadXORByte(guid2[i], 6);
                packet.ReadXORByte(guid2[i], 0);
                packet.ReadXORByte(guid1[i], 1);
                packet.ReadXORByte(guid2[i], 2);
                packet.ReadXORByte(guid1[i], 5);
                packet.ReadXORByte(guid2[i], 4);
                packet.ReadPackedTime("Time", i);
                packet.ReadXORByte(guid1[i], 4);
                packet.ReadXORByte(guid2[i], 5);
                packet.ReadInt32("Int14");
                packet.ReadXORByte(guid1[i], 7);
                packet.WriteGuid("Guid1", guid1[i], i);
                packet.WriteGuid("Guid2", guid2[i], i);
            }*/

    for (Map::PlayerList::const_iterator itr = m_Map->GetPlayers().begin(); itr != m_Map->GetPlayers().end(); ++itr)
    {
        itr->getSource()->GetSession()->SendPacket(&packet);
    }
}

void ScenarioController::SendScenarioPOI(uint32 criteriaTreeId, Player* player)
{

}

void ScenarioController::SendScenarioPOI(std::list<uint32>& criteriaTrees, Player* player)
{
    uint32 uint32_1 = 0;
    uint32 uint32_2 = 0;
    uint32 uint32_3 = 0;
    uint32 uint32_4 = 0;
    uint32 uint32_5 = 0;
    int32 uint32_6 = -3803;
    int32 uint32_7 = -4788;
    uint32 uint32_8 = 0;
    uint32 uint32_9 = 0;
    uint32 uint32_10 = 0;
    uint32 uint32_11 = 27242; // criteria tree id

    /*packet.ReadInt32("BlobID", i, j);
                    packet.ReadInt32("MapID", i, j);
                    packet.ReadInt32("WorldMapAreaID", i, j);
                    packet.ReadInt32("Floor", i, j);
                    packet.ReadInt32("Priority", i, j);
                    packet.ReadInt32("Flags", i, j);
                    packet.ReadInt32("WorldEffectID", i, j);
                    packet.ReadInt32("PlayerConditionID", i, j);*/

    for (std::list<uint32>::const_iterator itr = criteriaTrees.begin(); itr != criteriaTrees.end(); ++itr)
    {
        if ((*itr) == 27243)
        {
            WorldPacket packet(SMSG_SCENARIO_POI);
            
            packet.WriteBits(1, 21); // ScenarioPOIDataCount

            packet.WriteBits(1, 19); // ScenarioBlobDataCount

            packet.WriteBits(1, 21); // ScenarioPOIPointDataCount

            packet << uint32_1;
            packet << uint32_2;
            packet << uint32_3;
            packet << uint32_4;
            packet << uint32_5;

            packet << uint32_6;
            packet << uint32_7;

            packet << uint32_8;
            packet << uint32_9;
            packet << uint32_10;

            packet << uint32_11;

            player->GetSession()->SendPacket(&packet);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
/// Combat Resurrection - http://wow.gamepedia.com/Resurrect#Combat_resurrection
void InstanceScript::ResetCombatResurrection()
{
    if (!instance->IsRaid())
        return;

    m_InCombatResCount = 0;

    SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_RESET_COMBAT_RES_LIMIT);
}

void InstanceScript::StartCombatResurrection()
{
    if (!instance->IsRaid())
        return;

    /// Upon engaging a boss, all combat resurrection spells will have their cooldowns reset and begin with 1 (or 3) charge(s).
    for (uint8 l_I = 0; l_I < eInstanceSpells::MaxBattleResSpells; ++l_I)
        DoRemoveSpellCooldownOnPlayers(g_BattleResSpells[l_I]);

    if (instance->Is25ManRaid())
        m_InCombatResCount = 3;
    else
        m_InCombatResCount = 1;

    SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_SET_COMBAT_RES_LIMIT, nullptr, m_InCombatResCount);
}

bool InstanceScript::CanUseCombatResurrection() const
{
    if (!instance->IsRaid())
        return true;

    if (!IsEncounterInProgress())
        return true;

    if (m_InCombatResCount <= 0)
        return false;

    return true;
}

void InstanceScript::ConsumeCombatResurrectionCharge()
{
    /// Shouldn't happen
    if (m_InCombatResCount <= 0)
        return;

    --m_InCombatResCount;
    SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_SET_COMBAT_RES_LIMIT, nullptr, m_InCombatResCount);
}
//////////////////////////////////////////////////////////////////////////
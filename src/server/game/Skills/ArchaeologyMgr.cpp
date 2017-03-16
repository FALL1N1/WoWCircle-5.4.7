#include "ArchaeologyMgr.h"
#include "Common.h"
#include "Containers.h"
#include "ObjectMgr.h"

const static int q_patt[2][2] = { {0,1}, {3,2} };

namespace JadeCore
{
    bool IsPointInZone(const ResearchPOIPoint &test, const ResearchPOIPoints &polygon)
    {
        if (polygon.size() < 3)
            return false;

        ResearchPOIPoints::const_iterator end = polygon.end();
        ResearchPOIPoint pred_pt = polygon.back();
        pred_pt.x -= test.x;
        pred_pt.y -= test.y;

        int pred_q = q_patt[pred_pt.y < 0][pred_pt.x<  0];

        int w = 0;

        for (std::vector<ResearchPOIPoint>::const_iterator iter = polygon.begin(); iter != end; ++iter)
        {
            ResearchPOIPoint cur_pt = *iter;

            cur_pt.x -= test.x;
            cur_pt.y -= test.y;

            int q = q_patt[cur_pt.y < 0][cur_pt.x < 0];

            switch (q - pred_q)
            {
                case -3:
                    ++w;
                    break;
                case 3:
                    --w;
                    break;
                case -2:
                    if (pred_pt.x * cur_pt.y >= pred_pt.y * cur_pt.x)
                        ++w;
                    break;
                case 2:
                    if (!(pred_pt.x * cur_pt.y >= pred_pt.y * cur_pt.x))
                        --w;
                    break;
            }

            pred_pt = cur_pt;
            pred_q = q;
        }
        return w != 0;
    }
}

bool ArchaeologyMgr::GenerateDigitLoot(uint16 siteId, DigitSite &site)
{
    ResearchLootVector const* loots = sObjectMgr->GetResearchLootForSite(siteId);
    if (loots == nullptr || loots->empty())
        return false;

    ResearchLootVector::const_iterator itr = loots->begin();
    std::advance(itr, urand(0, loots->size() - 1));

    site.loot_x = itr->x;
    site.loot_y = itr->y;
    site.loot_z = itr->z;
    site.loot_id = itr->lootId;

    return true;
}

uint32 ArchaeologyMgr::GetSurveyBotEntry(float &orientation)
{
    uint16 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return 0;

    uint16 zoneid = GetResearchSiteID();
    if (!zoneid)
        return 0;

    uint32 at_pos = 0xFFFF;

    for(uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
    {
        uint32 site_now = _player->GetDynamicUInt32Value(PLAYER_DYNAMIC_RESEARCH_SITES, i);
        if (zoneid == site_now)
        {
            at_pos = i;
            break;
        }
    }

    if (at_pos == 0xFFFF)
        return 0;

    ASSERT(at_pos < MAX_RESEARCH_SITES);

    DigitSite &site = _digSites[at_pos];
    if (site.site_id != zoneid)
    {
        if (!GenerateDigitLoot(zoneid, site))
            return 0;

        site.site_id = zoneid;
    }

    orientation = _player->GetAngle(site.loot_x, site.loot_y);
    float dist_now = _player->GetDistance2d(site.loot_x, site.loot_y);

    if (dist_now >= ARCHAEOLOGY_DIG_SITE_FAR_DIST)
        return ARCHAEOLOGY_DIG_SITE_FAR_SURVEYBOT;
    if (dist_now >= ARCHAEOLOGY_DIG_SITE_MED_DIST)
        return ARCHAEOLOGY_DIG_SITE_MEDIUM_SURVEYBOT;
    if (dist_now >= ARCHAEOLOGY_DIG_SITE_CLOSE_DIST)
        return ARCHAEOLOGY_DIG_SITE_CLOSE_SURVEYBOT;

    if (skill_now < 50)
        _player->UpdateSkill(SKILL_ARCHAEOLOGY, 1);

    _player->SummonGameObject(site.loot_id, site.loot_x, site.loot_y, site.loot_z, 0, 0, 0, 0, 0, 30000);

    int32 lootCountAtSite = 3;
    if (_player->GetMapId() == 870) // Pandaria
        lootCountAtSite = 6;

    if (site.count < (lootCountAtSite-1))
    {
        site.count++;
        if (!GenerateDigitLoot(zoneid, site))
            return 0;
    }
    else
    {
        site.clear();
        UseResearchSite(zoneid);
    }

    _archaeologyChanged = true;

    return 0;
}

uint16 ArchaeologyMgr::GetResearchSiteID()
{
    uint32 zoneId = _player->GetZoneId();
    ResearchPOIPoint test(_player->GetPositionX(), _player->GetPositionY());

    ResearchZoneMap const& zoneMap = sObjectMgr->GetResearchZoneMap();
    for (auto const kvp : zoneMap)
    {
        ResearchZoneEntry const& entry = kvp.second;
        if (entry.map == _player->GetMapId() && entry.zone == zoneId)
        {
            if (JadeCore::IsPointInZone(test, entry.coords))
                return kvp.first;
        }
    }
    return 0;
}

void ArchaeologyMgr::UseResearchSite(uint32 id)
{
    uint8 index = GetIndexFromMap(_player->GetMapId());
    _researchSites[index].erase(id);
    GenerateResearchSiteInMap(_player->GetMapId(), index);
}

void ArchaeologyMgr::ShowResearchSites()
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    uint8 idx = 0;
    for (auto const& set : _researchSites)
    {
        for (uint32 const id : set)
        {
            _player->SetDynamicUInt32Value(PLAYER_DYNAMIC_RESEARCH_SITES, idx, CanResearchWithLevel(id) == RS_RESULT_HIDE ? 0 : id, true);

            ++idx;
        }
    }
}

void ArchaeologyMgr::ShowResearchProjects()
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    uint8 count = 0;
    for (uint32 const project : _researchProjects)
    {
        _player->SetUInt16Value(PLAYER_FIELD_RESEARCHING + (count / 2), count % 2, project, true);
        if (++count >= 16) // maximum supported count
            break;
    }

    for (uint8 i = count; i < 16; ++i)
    {
        _player->SetUInt16Value(PLAYER_FIELD_RESEARCHING + (i / 2), i % 2, 0);
    }
}

ResearchWithLevelResult ArchaeologyMgr::CanResearchWithLevel(uint32 siteId)
{
    uint32 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return RS_RESULT_FAIL;

    ResearchZoneMap const& zoneMap = sObjectMgr->GetResearchZoneMap();

    auto itr = zoneMap.find(siteId);
    if (itr == zoneMap.end())
        return RS_RESULT_FAIL;

    ResearchZoneEntry const& entry = itr->second;

    uint16 skill_cap = 0;
    uint8 level_cap = 0;
    uint8 cur_level = _player->getLevel();
    switch (entry.map)
    {
    case 0:
        if (entry.zone == 4922) // Twilight Hightlands
            skill_cap = 450;
        level_cap = entry.level;
        if (skill_now < skill_cap || (cur_level + 29 < level_cap))
            return RS_RESULT_FAIL;
        break;
    case 1:
        if (entry.zone == 616) // Hyjal
            skill_cap = 450;
        else if (entry.zone == 5034) // Uldum
            skill_cap = 450;
        level_cap = entry.level;
        if (skill_now < skill_cap || (cur_level + 29 < level_cap))
            return RS_RESULT_FAIL;
        break;
    case 530: // Outland
        skill_cap = 275;
        level_cap = 58;
        if (skill_now < skill_cap || (cur_level < level_cap))
            return RS_RESULT_HIDE;
        break;
    case 571: // Northrend
        skill_cap = 350;
        level_cap = 68;
        if (skill_now < skill_cap || (cur_level < level_cap))
            return RS_RESULT_HIDE;
        break;
    case 870: // Pandaria
        skill_cap = 500;
        level_cap = entry.level;
        if (skill_now < skill_cap || (cur_level < level_cap))
            return RS_RESULT_HIDE;
        break;
    default:
        return RS_RESULT_FAIL;
    }

    return RS_RESULT_OK;
}

void ArchaeologyMgr::GenerateResearchSiteInMap(uint32 mapId, uint32 map)
{
    if (sResearchSiteSet.empty())
        return;

    SiteSet tempSites;

    for (ResearchSiteEntry const* site : sResearchSiteSet)
        if (!HasResearchSite(site->ID, mapId) && site->mapId == mapId && CanResearchWithLevel(site->ID))
            tempSites.insert(site->ID);

    if (tempSites.empty())
        return;

    _researchSites[map].insert(JadeCore::Containers::SelectRandomContainerElement(tempSites));
    _archaeologyChanged = true;

    ShowResearchSites();
}

void ArchaeologyMgr::GenerateResearchSites()
{
    if (sResearchSiteSet.empty())
        return;

    for (auto& set : _researchSites)
        set.clear();

    for (ResearchSiteEntry const* site : sResearchSiteSet)
    {
        if (CanResearchWithLevel(site->ID))
        {
            uint8 index = GetIndexFromMap(site->mapId);
            _researchSites[index].insert(site->ID);
        }
    }

    for (auto& set : _researchSites)
        JadeCore::Containers::RandomResizeSet(set, RESEARCH_SITES_PER_MAP);

    _archaeologyChanged = true;

    ShowResearchSites();
}

void ArchaeologyMgr::GenerateResearchProjects()
{

    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    _researchProjects.clear();

    for (auto const& kvp : sResearchProjectMap)
    {
        if (kvp.second.empty())
            break;

        auto itr = JadeCore::Containers::SelectRandomWeightedContainerElement(kvp.second, [this](ResearchProjectEntry const* entry) -> double
        {
            if (entry->rare && IsCompletedProject(entry->ID))
                return 0.0;

            return (double)entry->GetWeight();
        });

        _researchProjects.insert((*itr)->ID);
    }

    _archaeologyChanged = true;

    ShowResearchProjects();
}

bool ArchaeologyMgr::SolveResearchProject(uint32 projectId)
{
    if (!projectId)
        return false;

    // Check for skill
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return false;

    // Check for correct cost data
    if (!ValidateCostData())
        return false;

    // Check for project id
    ResearchProjectEntry const* entry = sResearchProjectStore.LookupEntry(projectId);
    if (!entry)
        return false;

    // Check for existing project
    if (!HasResearchProject(projectId))
        return false;

    // All done
    for (ProjectCost const& cost : costData)
    {
        if (cost.currency)
            _player->ModifyCurrency(cost.itemId, -int32(cost.count));
        else
            _player->DestroyItemCount(cost.itemId, cost.count, true);
    }

    ClearProjectCost();

    _researchProjects.erase(projectId);

    _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS, projectId, 1);

    /// Already completed
    if (_completedProjects.find(projectId) != _completedProjects.end())
        ++_completedProjects[projectId].Count;
    else
    {
        CompletedProject l_Project;
        l_Project.Count     = 1;
        l_Project.FirstDate = time(nullptr);

        _completedProjects.insert(std::make_pair(projectId, l_Project));
    }

    // Add new project
    auto itr = JadeCore::Containers::SelectRandomWeightedContainerElement(sResearchProjectMap[entry->branchId], [this](ResearchProjectEntry const* entry) -> double
    {
        if (entry->rare && IsCompletedProject(entry->ID))
            return 0.0;

        return (double)entry->GetWeight();
    });

    _researchProjects.insert((*itr)->ID);

    _archaeologyChanged = true;

    WorldPacket data(SMSG_RESEARCH_COMPLETE, 12);
    data << uint32(time(nullptr));
    data << uint32(projectId);
    data << uint32(1);
    _player->SendDirectMessage(&data);

    ShowResearchProjects();
    return true;
}

void ArchaeologyMgr::SaveArchaeology(SQLTransaction& trans)
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    if (!_archaeologyChanged)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ARCHAEOLOGY);
    stmt->setUInt32(0, _player->GetGUIDLow());
    trans->Append(stmt);

    std::ostringstream ss;

    uint8 l_Index = 0;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_ARCHAEOLOGY);
    stmt->setUInt32(l_Index++, _player->GetGUIDLow());

    for (auto const& set : _researchSites)
    {
        for (uint32 const id : set)
            ss << id << " ";

        stmt->setString(l_Index++, ss.str().c_str());
        ss.str("");
    }

    for (auto const& set : _digSites)
        ss << uint32(set.count) << " ";

    stmt->setString(l_Index++, ss.str().c_str());
    ss.str("");

    for (uint32 project : _researchProjects)
        ss << project << " ";

    stmt->setString(l_Index++, ss.str().c_str());
    ss.str("");

    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ARCHAEOLOGY_PROJECTS);
    stmt->setUInt32(0, _player->GetGUIDLow());
    trans->Append(stmt);

    for (auto l_CompletedProject: _completedProjects)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_ARCHAEOLOGY_PROJECTS);
        stmt->setUInt32(0, _player->GetGUIDLow());
        stmt->setUInt32(1, l_CompletedProject.first);
        stmt->setUInt32(2, l_CompletedProject.second.Count);
        stmt->setUInt32(3, l_CompletedProject.second.FirstDate);
        trans->Append(stmt);
    }

    _archaeologyChanged = false;
}

void ArchaeologyMgr::LoadArchaeology(PreparedQueryResult result, PreparedQueryResult resultProjects)
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    for (auto& set : _digSites)
        set.clear();

    for (auto& set : _researchSites)
        set.clear();

    _researchProjects.clear();

    std::list<uint32> l_OldCompletedProjects;

    if (result)
    {
        Field* fields = result->Fetch();
        for (uint8 i = 0; i < ARCHAEOLOGY_CONTINENT_COUNT; ++i)
        {
            if (const char* str = fields[i].GetCString())
            {
                Tokenizer tokens(str, ' ', RESEARCH_SITES_PER_MAP);
                if (tokens.size() == RESEARCH_SITES_PER_MAP)
                    for (uint8 j = 0; j < tokens.size(); ++j)
                        _researchSites[i].insert(uint32(atoi(tokens[j])));
            }
        }

        // Loading current zone info
        if (const char* str = fields[5].GetCString())
        {
            Tokenizer tokens(str, ' ', MAX_RESEARCH_SITES);
            if (tokens.size() == MAX_RESEARCH_SITES)
            {
                for (uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
                    _digSites[i].count = uint32(atoi(tokens[i]));
            }
        }

        // Loading current projects
        if (const char* str = fields[6].GetCString())
        {
            Tokenizer tokens(str, ' ', MAX_RESEARCH_PROJECTS);
            if (tokens.size() == MAX_RESEARCH_PROJECTS)
            {
                for (uint8 i = 0; i < MAX_RESEARCH_PROJECTS; ++i)
                {
                    uint32 l_ProjectID = uint32(atoi(tokens[i]));

                    /// Check if project is available
                    bool l_IsOkay = false;
                    for (auto const& kvp : sResearchProjectMap)
                    {
                        for (ResearchProjectEntry const* l_Entry : kvp.second)
                        {
                            if (l_ProjectID == l_Entry->ID)
                            {
                                _researchProjects.insert(l_ProjectID);

                                l_IsOkay = true;
                                break;
                            }
                        }

                        if (l_IsOkay)
                            break;
                    }

                    /// If not, set to 0, it'll regenerate all projects
                    if (!l_IsOkay)
                        _researchProjects.insert(0);
                }
            }
        }

        // Loading completed projects
        if (const char* str = fields[7].GetCString())
        {
            Tokenizer tokens(str, ' ');
            if (tokens.size() > 0)
            {
                for (Tokenizer::const_iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
                    l_OldCompletedProjects.push_back(atoi((*itr)));
            }
        }
    }

    ValidateSites();
    ValidateProjects(_player->HasAtLoginFlag(AT_LOGIN_FIX_ARCHEAOLOGY));

    _completedProjects.clear();

    if (!resultProjects)
    {
        for (uint32 l_ProjectID : l_OldCompletedProjects)
        {
            CompletedProject l_Project;
            l_Project.Count     = 1;
            l_Project.FirstDate = time(nullptr);

            _completedProjects.insert(std::make_pair(l_ProjectID, l_Project));
        }

        return;
    }

    do
    {
        Field* l_Fields = resultProjects->Fetch();

        CompletedProject l_Project;
        uint32 l_ProjectID  = l_Fields[0].GetUInt32();
        l_Project.Count     = l_Fields[1].GetUInt32();
        l_Project.FirstDate = l_Fields[2].GetUInt32();

        _completedProjects.insert(std::make_pair(l_ProjectID, l_Project));
    }
    while (resultProjects->NextRow());

    for (uint32 l_ProjectID : l_OldCompletedProjects)
    {
        if (_completedProjects.find(l_ProjectID) != _completedProjects.end())
            continue;

        CompletedProject l_Project;
        l_Project.Count     = 1;
        l_Project.FirstDate = time(nullptr);

        _completedProjects.insert(std::make_pair(l_ProjectID, l_Project));
    }
}

void ArchaeologyMgr::ValidateSites()
{
    bool err = false;

    for (auto& set : _researchSites)
    {
        if (set.empty())
        {
            err = true;
            break;
        }
        for (uint32 const id : set)
        {
            if (!sResearchSiteStore.LookupEntry(id))
            {
                err = true;
                break;
            }
        }
    }

    if (err)
        GenerateResearchSites();
}

void ArchaeologyMgr::ValidateProjects(bool force /*= false*/)
{
    bool err = false;
    if (_researchProjects.empty() || force)
        err = true;

    for (uint32 project : _researchProjects)
    {
        if (!sResearchProjectStore.LookupEntry(project))
        {
            err = true;
            break;
        }
    }

    if (err)
        GenerateResearchProjects();
}

bool ArchaeologyMgr::ValidateCostData()
{
    if (costData.empty())
        return false;
    
    for (ProjectCost const& cost : costData)
    {
        if (cost.currency)
        {
            if (!_player->HasCurrency(cost.itemId, cost.count))
                return false;
        }
        else
        {
            if (!_player->HasItemCount(cost.itemId, cost.count))
                return false;
        }
    }
    return true;
}

uint16 ArchaeologyMgr::GetRandomActiveSiteInMap(uint32 mapId) const
{
    uint8 map = 0;
    switch (mapId)
    {
        case 0: map = 0; break;
        case 1: map = 1; break;
        case 530: map = 2; break;
        case 571: map = 3; break;
        case 870: map = 4; break;
        default: map = 0; break;
    }

    std::list<uint16> sitesId;

    for (auto itr : _researchSites[map])
        sitesId.push_back(itr);

    if (sitesId.empty())
        return 0;

    JadeCore::Containers::RandomResizeList(sitesId, 1);
    return sitesId.front();
}

bool ArchaeologyMgr::IsCompletedProject(uint32 projectId) const
{
    auto itr = _completedProjects.find(projectId);
    return itr != _completedProjects.end();
}
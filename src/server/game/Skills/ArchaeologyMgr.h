#ifndef __TRINITY_ARCHAEOLOGYMGR_H
#define __TRINITY_ARCHAEOLOGYMGR_H

#include <map>
#include <string>

#include "Common.h"
#include <ace/Singleton.h>
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "DBCStores.h"

class Player;

#define RESEARCH_SITES_PER_MAP                  4
#define ARCHAEOLOGY_CONTINENT_COUNT             5
#define MAX_RESEARCH_SITES                      RESEARCH_SITES_PER_MAP * ARCHAEOLOGY_CONTINENT_COUNT
#define MAX_RESEARCH_PROJECTS                   12
#define ARCHAEOLOGY_DIG_SITE_FAR_DIST           40
#define ARCHAEOLOGY_DIG_SITE_MED_DIST           20
#define ARCHAEOLOGY_DIG_SITE_CLOSE_DIST         8
#define ARCHAEOLOGY_DIG_SITE_FAR_SURVEYBOT      206590
#define ARCHAEOLOGY_DIG_SITE_MEDIUM_SURVEYBOT   206589
#define ARCHAEOLOGY_DIG_SITE_CLOSE_SURVEYBOT    204272

enum ResearchWithLevelResult
{
    RS_RESULT_FAIL  = 0,
    RS_RESULT_OK    = 1,
    RS_RESULT_HIDE  = 2,
};

struct DigitSite
{
    uint8 count;
    uint16 site_id;
    uint32 loot_id;
    float loot_x;
    float loot_y;
    float loot_z;

    void clear()
    {
        site_id = loot_id = 0;
        loot_x = loot_y = loot_z = 0.0f;
        count = 0;
    }

    bool empty() { return site_id == 0; }
};

struct ProjectCost
{
    ProjectCost(uint32 _itemId, uint32 _count, bool isCurrency)
    {
        itemId = _itemId;
        count = _count;
        currency = isCurrency;
    }

    uint32 itemId;
    uint32 count;
    bool currency;
};

struct CompletedProject
{
    uint32 Count;
    uint32 FirstDate;
};

typedef std::set<uint32> SiteSet;
typedef std::map<uint32, SiteSet> Sites;
typedef std::set<uint32> ProjectSet;
typedef std::map<uint32, ProjectSet> Projects;

typedef std::set<uint32> ResearchSiteSet;
typedef std::set<uint32> ResearchProjectSet;
typedef std::map<uint32, CompletedProject> CompletedProjectMap;

class ArchaeologyMgr
{
    public:
        ArchaeologyMgr(Player* player) : _player(player), _archaeologyChanged(false)
        { 
            for (auto& set :_digSites)
                set.clear();
        }
        ~ArchaeologyMgr() {}

        void LoadArchaeology(PreparedQueryResult result, PreparedQueryResult resultProjects);
        void SaveArchaeology(SQLTransaction& trans);
        
        void AddProjectCost(uint32 entry, uint32 count, bool isCurrency)
        {
            costData.push_back(ProjectCost(entry, count, isCurrency));
        }
        void ClearProjectCost() { costData.clear(); }

        bool ValidateCostData();
        bool SolveResearchProject(uint32 projectId);
        uint32 GetSurveyBotEntry(float &orientation);

        CompletedProjectMap& GetCompletedProjects() { return _completedProjects; }

        void GenerateResearchProjects();
        void GenerateResearchSites();

        void ShowResearchProjects();
        void ShowResearchSites();

        void UseResearchSite(uint32 id);

        uint16 GetRandomActiveSiteInMap(uint32 mapId) const;

    private:
        Player* _player;
        std::vector<ProjectCost> costData;
        DigitSite _digSites[MAX_RESEARCH_SITES];
        ResearchSiteSet _researchSites[ARCHAEOLOGY_CONTINENT_COUNT];
        ResearchProjectSet _researchProjects;
        CompletedProjectMap _completedProjects;
        bool _archaeologyChanged;

        bool HasResearchSite(uint32 id, uint32 mapId) const
        {
            uint8 index = GetIndexFromMap(mapId);
            if (index == 0xFF)
                return false;

            return _researchSites[index].find(id) != _researchSites[index].end();
        }

        uint8 GetIndexFromMap(uint32 mapId) const
        {
            switch (mapId)
            {
            case 0:
                return 0;
            case 1:
                return 1;
            case 530:
                return 2;
            case 571:
                return 3;
            case 870:
                return 4;
            default:
                return 0xFF;
            }
        }

        bool HasResearchProject(uint32 id) const
        {
            return _researchProjects.find(id) != _researchProjects.end();
        }

        bool GenerateDigitLoot(uint16 siteId, DigitSite &site);

        uint16 GetResearchSiteID();

        void GenerateResearchSiteInMap(uint32 mapId, uint32 map);
        ResearchWithLevelResult CanResearchWithLevel(uint32 siteId);

        void ValidateSites();
        void ValidateProjects(bool force = false);

        bool IsCompletedProject(uint32 projectId) const;
};

#endif

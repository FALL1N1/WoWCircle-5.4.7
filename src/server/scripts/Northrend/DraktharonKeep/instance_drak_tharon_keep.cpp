#include "ScriptPCH.h"
#include "drak_tharon_keep.h"

#define MAX_ENCOUNTER     4

/* Drak'Tharon Keep encounters:
0 - Trollgore
1 - Novos
2 - King Dred
3 - Tharon Ja
*/

class instance_drak_tharon : public InstanceMapScript
{
public:
    instance_drak_tharon() : InstanceMapScript("instance_drak_tharon", 600) { }

    struct instance_drak_tharon_InstanceScript : public InstanceScript
    {
        instance_drak_tharon_InstanceScript(Map* map) : InstanceScript(map) 
        {
            SetBossNumber(MAX_BOSSES);
        }

        uint8 uiDredAchievCounter;

        uint64 uiTrollgore;
        uint64 uiNovos;
        uint64 uiDred;
        uint64 uiTharonJa;

        uint64 uiNovosCrystal1;
        uint64 uiNovosCrystal2;
        uint64 uiNovosCrystal3;
        uint64 uiNovosCrystal4;

        std::string str_data;

        void Initialize()
        {
            uiTrollgore = 0;
            uiNovos = 0;
            uiDred = 0;
            uiTharonJa = 0;
            uiNovosCrystal1 = 0;
            uiNovosCrystal2 = 0;
            uiNovosCrystal3 = 0;
            uiNovosCrystal4 = 0;
            uiDredAchievCounter = 0;
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_NOVOS_CRYSTAL_1:
                    uiNovosCrystal1 = go->GetGUID();
                    break;
                case GO_NOVOS_CRYSTAL_2:
                    uiNovosCrystal2 = go->GetGUID();
                    break;
                case GO_NOVOS_CRYSTAL_3:
                    uiNovosCrystal3 = go->GetGUID();
                    break;
                case GO_NOVOS_CRYSTAL_4:
                    uiNovosCrystal4 = go->GetGUID();
                    break;
            }
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_TROLLGORE:
                    uiTrollgore = creature->GetGUID();
                    break;
                case NPC_NOVOS_THE_SUMMONER:
                    uiNovos = creature->GetGUID();
                    break;
                case NPC_KING_DRED:
                    uiDred = creature->GetGUID();
                    break;
                case NPC_PROPHET_THARON_JA:
                    uiTharonJa = creature->GetGUID();
                    break;
            }
        }

        uint64 GetData64(uint32 identifier)
        {
            switch (identifier)
            {
                case DATA_TROLLGORE:          return uiTrollgore;
                case DATA_NOVOS:              return uiNovos;
                case DATA_DRED:               return uiDred;
                case DATA_THARON_JA:          return uiTharonJa;
                case DATA_NOVOS_CRYSTAL_1:    return uiNovosCrystal1;
                case DATA_NOVOS_CRYSTAL_2:    return uiNovosCrystal2;
                case DATA_NOVOS_CRYSTAL_3:    return uiNovosCrystal3;
                case DATA_NOVOS_CRYSTAL_4:    return uiNovosCrystal4;
            }

            return 0;
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;
                
            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_KING_DRED_ACHIEV:
                    uiDredAchievCounter = data;
                    break;
            }

            if (data == DONE)
            {
                SaveToDB();
            }
        }

        uint32 GetData(uint32 type)
        {
            switch (type)
            {
                case DATA_KING_DRED_ACHIEV:   return uiDredAchievCounter;
            }
            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "D K " << GetBossSaveData();

            OUT_SAVE_INST_DATA_COMPLETE;
            return saveStream.str();
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2;

            if (dataHead1 == 'D' && dataHead2 == 'K')
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                        tmpState = NOT_STARTED;

                    SetBossState(i, EncounterState(tmpState));
                }
            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_drak_tharon_InstanceScript(map);
    }
};

void AddSC_instance_drak_tharon()
{
    new instance_drak_tharon;
}

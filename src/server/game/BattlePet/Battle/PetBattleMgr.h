#pragma once

#include <ace/Singleton.h>
#include "PetBattle.h"
#include <LockedMap.h>

namespace BattlePet
{
    class PetBattleMgr
    {
        friend class ACE_Singleton<PetBattleMgr, ACE_Thread_Mutex>;
        
    public:
        PetBattleMgr();
        ~PetBattleMgr();
        
        void AddBattle(Battle* battle, uint64 id);
        Battle* GetBattle(uint64 id);
        void RemoveBattle(uint64 id);
        void Clear();
        void Update(const int32 diff);
        
        typedef std::map<uint64, Battle*> BattleMap;

    private:
        BattleMap m_battleContainer;

        typedef ACE_RW_Thread_Mutex LockType;

        LockType i_lock;
    };
};

#define sPetBattleMgr ACE_Singleton<BattlePet::PetBattleMgr, ACE_Thread_Mutex>::instance()
#include "PetBattleMgr.h"

BattlePet::PetBattleMgr::PetBattleMgr()
{
}

BattlePet::PetBattleMgr::~PetBattleMgr()
{
    Clear();
}

void BattlePet::PetBattleMgr::AddBattle(Battle* battle, uint64 id)
{
    if (battle == nullptr)
        return;

    {
        if (GetBattle(id) != nullptr)
            RemoveBattle(id);
    }

    {
        TRINITY_WRITE_GUARD(LockType, i_lock);

        m_battleContainer[id] = battle;
    }
}

BattlePet::Battle* BattlePet::PetBattleMgr::GetBattle(uint64 id)
{
    TRINITY_READ_GUARD(LockType, i_lock);

    BattleMap::iterator itr = m_battleContainer.find(id);
    if (itr == m_battleContainer.end())
        return nullptr;

    return itr->second;
}

void BattlePet::PetBattleMgr::RemoveBattle(uint64 id)
{
    TRINITY_WRITE_GUARD(LockType, i_lock);

    BattleMap::iterator itr = m_battleContainer.find(id);
    if (itr == m_battleContainer.end())
        return;

    Battle* battle = itr->second;
    battle->Cleanup();

    //delete battle;

    m_battleContainer.erase(itr);
}

void BattlePet::PetBattleMgr::Clear()
{
    for (auto& pair : m_battleContainer)
    {
        Battle* battle = pair.second;
        battle->Cleanup();

        delete battle;
    }

    m_battleContainer.clear();
}

void BattlePet::PetBattleMgr::Update(const int32 diff)
{
    for (auto& pair : m_battleContainer)
    {
        Battle* battle = pair.second;
        if (!battle || !battle->IsPvP())
            continue;

        battle->Update(diff);
    }
}
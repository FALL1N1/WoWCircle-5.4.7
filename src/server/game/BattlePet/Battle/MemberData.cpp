#include "PetBattle.h"
#include "Player.h"

BattlePet::MemberData::MemberData(Battle* battle, PBOID side, Unit* owner)
    : m_side(side)
    , m_guid(owner->GetGUID())
    , m_battle(battle)
    , m_pet(nullptr)
    , m_initialized(false)
    , m_opponent(nullptr)
{
}

BattlePet::MemberData::~MemberData()
{
    for (PetPair const& pair : m_pets)
    {
        delete pair.second;
    }

    m_pets.clear();
}

bool BattlePet::MemberData::Initial(Unit* owner)
{
    ASSERT(m_pets.empty());
    ASSERT(owner != nullptr);

    if (Player* player = owner->ToPlayer())
    {
        uint8 index = 0;

        auto const journal = player->GetPetJournal();
        for (uint8 i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; ++i)
        {
            uint64 id = journal->GetLoadoutSlot(i);
            if (id == 0LL)
                continue;

            PetTemplate* proto = journal->GetBattlePet(id);
            if (proto == nullptr)
                continue;

            if (!proto->IsPetAvailableForBattle())
                continue;

            PBOID pboid = IsInitiator() ? PBOID(P0_PET_0 + index) : PBOID(P1_PET_0 + index);

            Pet* pet = new Pet(m_battle, shared_from_this(), proto, pboid);
            m_pets[pboid] = pet;

            ++index;
        }
    }
    else
    {
        // if member is creature
    }

    if (m_pets.empty())
        return false;

    m_pet = m_pets[IsInitiator() ? PBOID::P0_PET_0 : PBOID::P1_PET_0];
    return true;
}

void BattlePet::MemberData::SetActivePet(PBOID target)
{
    m_pet = m_pets[target];

    ASSERT(m_pet != nullptr && "Active pet can't be null");
}

TrapStatus BattlePet::MemberData::GetTrapStatus() const
{
    return TrapStatus::TRAP_STATUS_CANT_CATCH_OPPONENTS_PET;
}

BattlePetDisableFlags BattlePet::MemberData::GetFlags() const
{
    BattlePetDisableFlags flags = BattlePetDisableFlags::DisableNone;
    if (SwapReq())
        flags = BattlePetDisableFlags::Pass | BattlePetDisableFlags::Ability;
    if (m_battle->HasMoved(m_side) || !m_battle->CanMove(m_side))
        flags |= BattlePetDisableFlags::Pass | BattlePetDisableFlags::Ability | BattlePetDisableFlags::Swap;

    return flags;
}

void BattlePet::MemberData::Update()
{
    for (PetPair const& pair : m_pets)
    {
        Pet* pet = pair.second;
        pet->Update();
    }
}
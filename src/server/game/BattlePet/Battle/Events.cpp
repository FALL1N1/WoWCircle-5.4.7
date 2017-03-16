#include "PetBattle.h"
#include "PetBattleEvent.h"

void BattlePet::Battle::OnPetDied(Pet* pet)
{
    PBOID side = pet->GetSide();
    sMemberData& member = m_members[side];

    PBOID nextAvailablePet = PBOID::INVALID_TARGET;
    for (PetPair const& pair : member->m_pets)
    {
        Pet* temp = pair.second;
        if (temp->GetPBOID() == pet->GetPBOID())
            continue;

        if (temp->IsDead())
            continue;

        nextAvailablePet = temp->GetPBOID();
    }

    if (nextAvailablePet == PBOID::INVALID_TARGET)
    {
        SendEvents();
        EndBattle(side == PBOID::PAD_0 ? PBOID::PAD_1 : PBOID::PAD_0, PetBattleEndReason::Default);
        return;
    }
}

void BattlePet::Battle::OnHPChanged(Pet* pet, Pet* whoChange, int32 newHp, uint32 dueToAbilityEffect, uint16 flags)
{
    if (!m_event || m_state == PetBattleState::StateEnding)
        return;

    m_event->Make(
        Event::EventInfo(PetBattleEvents::SET_HEALTH, whoChange->GetPBOID(), dueToAbilityEffect == 1554 ? 0 : dueToAbilityEffect, flags),
        Event::EventValue(pet->GetPBOID(), newHp));
}

void BattlePet::Battle::OnAuraCreated(Pet* owner, Pet* creator, uint32 dueToAbilityEffect, uint16 auraId, uint32 amount, int32 duration, uint16 flags)
{
    if (!m_event || m_state == PetBattleState::StateEnding)
        return;

    m_event->Make(
        Event::EventInfo(PetBattleEvents::AURA_APPLY, creator->GetPBOID(), dueToAbilityEffect, flags),
        Event::EventValue(owner->GetPBOID(), auraId, amount, m_round, duration));
}

void BattlePet::Battle::OnAuraChanged(Pet* owner, uint16 auraId, uint32 newAmount, int32 newDuration) // new round also
{
    if (!m_event || m_state == PetBattleState::StateEnding)
        return;

    m_event->Make(
        Event::EventInfo(PetBattleEvents::AURA_CHANGE, owner->GetPBOID(), 0/*dueToAbilityEffect*/, 0/*flags*/),
        Event::EventValue(owner->GetPBOID(), auraId, newAmount, m_round, newDuration));
}

void BattlePet::Battle::OnAuraRemoved(Pet* owner, uint16 auraId, uint8 removeReason /*duration, dispell*/, uint16 flags)
{
    if (!m_event || m_state == PetBattleState::StateEnding)
        return;

    m_event->Make(
        Event::EventInfo(PetBattleEvents::AURA_CANCEL, owner->GetPBOID(), 0/*dueToAbilityEffect*/, 0/*flags*/),
        Event::EventValue(owner->GetPBOID(), auraId, 0, m_round, 0));
}

void BattlePet::Battle::OnStateChanged(Pet* pet, Pet* whoChange, uint32 dueToAbilityEffect, uint16 stateId, int32 newValue, uint16 flags)
{
    if (!m_event || m_state == PetBattleState::StateEnding)
        return;

    m_event->Make(
        Event::EventInfo(PetBattleEvents::SET_STATE, whoChange->GetPBOID(), dueToAbilityEffect, flags),
        Event::EventValue(pet->GetPBOID(), stateId, newValue));
}
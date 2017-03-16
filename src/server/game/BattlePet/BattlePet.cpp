#include "BattlePet.h"
#include "Ability/BattlePetAbility.h"
#include "Battle/PetBattle.h"
#include "Battle/PetBattleEvent.h"
#include "DBCStores.h"
#include "ObjectMgr.h"

BattlePet::Pet::Pet(Battle* battle, std::shared_ptr<MemberData> member, PetTemplate* proto, PBOID target)
    : m_proto(proto)
    , m_type(target)
    , m_battle(battle)
    , m_member(member)
    , m_hp(m_proto->GetCurrentHealth())
    , m_maxHp(m_proto->GetMaxHealth())
    , m_xp(m_proto->GetXp())
    , m_level(m_proto->GetLevel())
    , m_oldLevel(m_proto->GetLevel())
    , m_hasActiveMultiTurnAbility(false)
{
    Initial();
}

BattlePet::Pet::~Pet()
{
    for (auto const& itr : m_auras)
    {
        delete itr.second;
    }

    m_auras.clear();
}

void BattlePet::Pet::Initial()
{
    InitialAbilities();

    BattlePetBreedStatesMap::iterator itr = sBattlePetBreedStatesMap.find(m_proto->GetSpecies());
    if (itr != sBattlePetBreedStatesMap.end())
    {
        for (BattlePetBreedStateEntry const* state : itr->second)
        {
            ModifyState(this, (BattlePetStates)state->StateId, state->Value, true);
        }
    }

    BattlePetSpeciesStatesMap::iterator itr2 = sBattlePetSpeciesStatesMap.find(m_proto->GetSpecies());
    if (itr2 != sBattlePetSpeciesStatesMap.end())
    {
        for (BattlePetSpeciesStateEntry const* state : itr2->second)
        {
            ModifyState(this, (BattlePetStates)state->StateId, state->Modifier, true);
        }
    }

    ModifyState(this, m_proto->GetStateFromFamily(), 1, true);

    //AddAura(m_proto->GetPetPassive(), -1, m_member, nullptr, false);
}

void BattlePet::Pet::InitialAbilities()
{
    //TODO: fix and refactore
    BattlePetSpeciesXAbilityMap::iterator itr = sBattlePetSpeciesXAbilityMap.find(m_proto->GetSpecies());
    ASSERT(itr != sBattlePetSpeciesXAbilityMap.end());

    for (BattlePetSpeciesXAbilityEntry const* entry : itr->second)
    {
        if (entry->RequiredLevel > m_proto->GetLevel())
            continue;

        if (entry->RequiredLevel >= 10)
        {
            if (!m_proto->HasFlag(1 << (4 + entry->SlotId)))
                continue;
        }
        else
        {
            if (m_proto->HasFlag(1 << (4 + entry->SlotId)))
                continue;
        }

        m_abilities.push_back(entry->AbilityId);
    }
}

void BattlePet::Pet::Update()
{
    for (CooldownMap::iterator itr = m_cooldowns.begin(); itr != m_cooldowns.end();)
    {
        uint16& cooldown = itr->second;
        if (cooldown == 0)
        {
            itr = m_cooldowns.erase(itr);
            continue;
        }

        --cooldown;
        ++itr;
    }

    for (AbilityMap::iterator itr = m_auras.begin(); itr != m_auras.end(); ++itr)
    {
        Ability* ability = itr->second;
        if (!ability->IsExpired())
            ability->Update();
    }

    for (AbilityMap::iterator itr = m_auras.begin(); itr != m_auras.end();)
    {
        Ability* ability = itr->second;
        if (ability->IsExpired())
            RemoveAura(itr);
        else
            ++itr;
    }

    ClearDamage();
}

void BattlePet::Pet::CastAbility(uint32 abilityId)
{
    if (HasCooldown(abilityId))
        return;

    uint8 duration = 0;
    if (BattlePetAbilityData const* data = sObjectMgr->GetAbilityData(abilityId))
    {
        /* 0x1 - multiturn ability
        */
        if (data->m_flags & 0x1)
        {
            BattlePetAbilityEffectsMap::iterator itr = sBattlePetAbilityEffectsMap.find(abilityId);
            if (itr != sBattlePetAbilityEffectsMap.end())
                duration = uint8(itr->second.size());
        }

        if (duration > 0)
            duration -= 1;

        if (Ability* aura = m_member->GetOpponent()->GetActivePet()->AddAura(abilityId, duration, m_member))
        {
            aura->CastAura();
            AddCooldown(abilityId, aura->GetCooldown());
        }
        return;
    }

    Ability* ability = new Ability(abilityId, m_member, m_member->GetOpponent());
    ability->CastAbility();
    AddCooldown(abilityId, ability->GetCooldown());
    delete ability;
}

BattlePet::Ability* BattlePet::Pet::AddAura(uint32 abilityId, int32 duration, std::shared_ptr<MemberData>& caster, BattlePetAbilityEffectEntry const* createdBy, bool send)
{
    if (HasCooldown(abilityId))
        return nullptr;

    if (Ability* aura = GetAura(abilityId))
    {
        aura->RefreshDuration();
        return aura;
    }

    Ability* aura = new Ability(abilityId, caster, m_member, duration, createdBy);
    m_auras[abilityId] = aura;

    if (send)
        m_battle->OnAuraCreated(this, caster->GetActivePet(), createdBy ? createdBy->Id : 0, abilityId, 0, duration, 0);
    return aura;
}

void BattlePet::Pet::RemoveAura(uint32 abilityId)
{
    AbilityMap::iterator itr = m_auras.find(abilityId);
    if (itr == m_auras.end())
        return;

    RemoveAura(itr);
}

void BattlePet::Pet::RemoveAura(AbilityMap::iterator& itr)
{
    if (itr == m_auras.end())
        return;

    Ability* aura = itr->second;
    uint32 abilityId = aura->GetId();

    aura->Remove();

    delete aura;
    itr = m_auras.erase(itr);

    m_battle->OnAuraRemoved(this, abilityId, 0, 0);
}

void BattlePet::Pet::ModifyHealth(int32 dVal)
{
    SetHealth(GetHealth() + dVal);
}

void BattlePet::Pet::SetHealth(int32 value)
{
    if (m_hp == value)
        return;

    m_hp = value;

    if (m_hp > m_maxHp)
        m_hp = m_maxHp;

    GetProto()->SetHp(GetHealth());
}

void BattlePet::Pet::DealDamageByEffect(Pet* victim, uint32 points, uint32 accuracy, Effect* effect)
{
    if (victim->IsDead())
        return;

    uint32 flags = 0;

    int32 finalAccuracy = CalculateAccuracy(accuracy);
    uint32 amount = CalculateDamage(effect, victim, points);

    if (finalAccuracy > 0 && amount > 0 && roll_chance_i(finalAccuracy))
    {
        if (roll_chance_i(GetCritChance()))
        {
            amount *= 1.5;

            flags |= BattlePetEffectFlags::Crit;
        }

        if (!victim->HasState(BattlePetStates::BATTLE_PET_STATE_UNTARGETTABLE))
        {
            if (IsPetStrongAgainst(effect->GetBase()->GetFamily(), victim->GetFamily()))
                flags |= BattlePetEffectFlags::Strong;
            else if (IsPetWeakAgainst(effect->GetBase()->GetFamily(), victim->GetFamily()))
                flags |= BattlePetEffectFlags::Weak;

            amount = victim->CalculateDamageTaken(amount);
            if (amount > 0)
            {
                victim->ModifyHealth(-(int32)amount);

                SetDealtDamage(amount);
                victim->SetTakenDamage(amount);
            }
            else
                flags |= BattlePetEffectFlags::Miss;
        }
        else
            flags |= BattlePetEffectFlags::Immune;
    }
    else
        flags |= BattlePetEffectFlags::Miss;

    m_battle->OnHPChanged(victim, this, victim->GetHealth(), effect->GetEffId(), flags);

    if (victim->GetHealth() <= 0)
        victim->MakeDead();
}

uint32 BattlePet::Pet::CalculateHealTaken(uint32 heal)
{
    AddPct(heal, GetStateValue(BATTLE_PET_STATE_MOD_HEALINGTAKENPERCENT));
    return heal;
}

void BattlePet::Pet::HealByEffect(Pet* victim, uint32 points, uint32 accuracy, Effect* effect)
{
    if (IsDead() || victim->IsDead())
        return;

    uint32 flags = 0;

    int32 finalAccuracy = CalculateAccuracy(accuracy);
    uint32 amount = CalculateHeal(effect, victim, points);
    if (finalAccuracy > 0 && roll_chance_i(finalAccuracy))
    {
        if (roll_chance_i(GetCritChance()))
        {
            amount *= 1.5;

            flags |= BattlePetEffectFlags::Crit;
        }

        amount = victim->CalculateHealTaken(amount);

        victim->ModifyHealth((int32)amount);
    }
    else
        flags |= BattlePetEffectFlags::Miss;

    m_battle->OnHPChanged(victim, this, victim->GetHealth(), effect->GetEffId(), flags);
}

void BattlePet::Pet::SetState(Pet* whoChange, BattlePetStates state, int32 value)
{
    if (BattlePetStateEntry const* entry = sBattlePetStateStore.LookupEntry(state))
    {
        m_states[state] = value;

        if (entry->Flags & 0x8)
        {
            m_battle->OnStateChanged(this, whoChange, 0, state, value, 0);
        }
    }
}

void BattlePet::Pet::ModifyState(Pet* whoChange, BattlePetStates state, int32 value, bool apply)
{
    if (BattlePetStateEntry const* entry = sBattlePetStateStore.LookupEntry(state))
    {
        StateMap::iterator itr = m_states.find(state);
        if (itr == m_states.end())
        {
            if (apply)
            {
                m_states.insert(StateMap::value_type(state, value));
            }
        }
        else
        {
            itr->second += apply ? value : -value;
        }

        if (entry->Flags & 0x8)
        {
            m_battle->OnStateChanged(this, whoChange, 0, state, m_states[state], 0);
        }
    }
}

void BattlePet::Pet::MakeDead()
{
    for (auto const& itr : m_auras)
    {
        if (itr.second)
            itr.second->Remove();
    }

    ModifyState(this, BattlePetStates::BATTLE_PET_STATE_IS_DEAD, true);

    m_battle->OnPetDied(this);
}

void BattlePet::Pet::GixeXP(uint16 xp)
{
    if (GetLevel() >= BATTLE_PET_MAX_LEVEL)
        xp = 0;

    if (!xp)
        return;

    m_xp += xp;

    while (GetXP() >= sObjectMgr->GetBattlePetXPForLevel(GetLevel()) && GetLevel() < BATTLE_PET_MAX_LEVEL)
    {
        m_xp -= sObjectMgr->GetBattlePetXPForLevel(GetLevel());
        ++m_level;
    }

    GetProto()->SetXP(GetXP());
    GetProto()->SetLevel(GetLevel());
}

int32 BattlePet::Pet::CalculateAccuracy(int32 accuracy)
{
    accuracy += GetStateValue(BATTLE_PET_STATE_STAT_ACCURACY);
    return accuracy;
}

uint32 BattlePet::Pet::CalculateHeal(Effect* effect, Pet* opponent, uint32 points)
{
    //SharedPetBattleTemplates.lua
    float finalHeal = points * HealingBonus();
    if (effect->GetEffectType() == BattlePetEffectType::ABILITY_EFFECT_HEAL_PCT)
        finalHeal = float(points);

    if (int32 modifier = GetStateValue(BATTLE_PET_STATE_MOD_HEALINGDEALTPERCENT))
    {
        AddPct(finalHeal, modifier);
    }
    return uint32(floor(finalHeal));
}

uint32 BattlePet::Pet::CalculateDamage(Effect* effect, Pet* opponent, uint32 points)
{
    float finalDamage = points * AttackBonus();
    if (effect->GetEffectType() == BattlePetEffectType::ABILITY_EFFECT_DAMAGE_PCT_FROM_TAKEN || effect->GetEffectType() == BattlePetEffectType::ABILITY_EFFECT_DAMAGE_PCT_FROM_HP)
        finalDamage = float(points);

    finalDamage *= GetBattlePetDamageMod(effect->GetBase()->GetFamily(), opponent->GetFamily());
    if (int32 modifier = GetStateValue(BATTLE_PET_STATE_MOD_DAMAGEDEALTPERCENT))
    {
        AddPct(finalDamage, modifier);
    }

    finalDamage = floorf(finalDamage);

    return finalDamage > 0 ? uint32(finalDamage) : 0;
}

uint32 BattlePet::Pet::CalculateDamageTaken(uint32 damage)
{
    int finalDamage = int32(damage);

    AddPct(finalDamage, GetStateValue(BATTLE_PET_STATE_MOD_DAMAGETAKENPERCENT));
    finalDamage += GetStateValue(BATTLE_PET_STATE_ADD_FLATDAMAGETAKEN);
    return finalDamage > 0 ? uint32(finalDamage) : 0;
}
 
uint32 BattlePet::Pet::GetCritChance() const
{
    uint32 crit = 5;
    crit += GetStateValue(BATTLE_PET_STATE_STAT_CRITCHANCE);
    return crit;
}

uint32 BattlePet::Pet::GetSpeed() const
{
    uint32 speed = GetProto()->GetSpeed();
    AddPct(speed, GetStateValue(BATTLE_PET_STATE_MOD_SPEEDPERCENT));
    return speed;
}

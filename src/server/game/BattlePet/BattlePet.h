#pragma once

#include "PetJournal/BattlePetTemplate.h"

namespace BattlePet
{
    class Ability;
    class Effect;
    class Battle;
    class EventHolder;
    class MemberData;

    typedef std::unordered_map<BattlePetStates /*stateId*/, int32 /*stateVal*/, std::hash<int>> StateMap;
    typedef std::vector<uint32 /*abilityId*/> AbilityList;
    typedef std::map<uint32 /*abilityId*/, uint16 /*cooldown*/> CooldownMap;
    typedef std::map<uint32 /*abilityId*/, Ability*> AbilityMap;

class Pet
{
public:
    Pet(Battle* battle, std::shared_ptr<MemberData> member, PetTemplate* proto, PBOID target);
    ~Pet();
    
    void Update();

    void CastAbility(uint32 abilityId);

    bool IsPetAvailableForBattle() { return m_proto->IsPetAvailableForBattle(); }
    
    bool HasState(BattlePetStates state) const
    {
        StateMap::const_iterator itr = m_states.find(state);
        if (itr == m_states.end())
            return false;

        return itr->second != 0;
    }

    int32 GetStateValue(BattlePetStates state) const
    {
        StateMap::const_iterator itr = m_states.find(state);
        if (itr == m_states.end())
            return 0;

        return itr->second;
    }

    void SetState(Pet* whoChange, BattlePetStates state, int32 value);
    void ModifyState(Pet* whoChange, BattlePetStates state, int32 value, bool apply = true);

    void AddCooldown(uint32 abilityId, uint16 cooldown)
    {
        if (cooldown == 0)
            return;

        m_cooldowns[abilityId] = cooldown;
    }

    bool HasCooldown(uint32 abilityId)
    {
        CooldownMap::iterator itr = m_cooldowns.find(abilityId);
        if (itr != m_cooldowns.end())
            return itr->second > 0;

        return false;
    }

    PBOID GetPBOID() const { return m_type; }

    AbilityList const& GetAbilities() const { return m_abilities; }

    uint8 GetAbilityIndex(uint32 abilityId) const
    {
        uint8 index = 0;
        for (uint32 ability : m_abilities)
        {
            if (ability == abilityId)
                return index;

            ++index;
        }

        return 0;
    }

    StateMap const& GetStates() const { return m_states; }

    PetTemplate* GetProto() { return m_proto; }

    PetTemplate const* GetProto() const { return m_proto; }

    CooldownMap const& GetCooldowns() const { return m_cooldowns; }

    AbilityMap const& GetAuras() const { return m_auras; }

    Ability* AddAura(uint32 abilityId, int32 duration, std::shared_ptr<MemberData>& caster, BattlePetAbilityEffectEntry const* createdBy = nullptr, bool send = true);

    bool HasAura(uint32 abilityId) const
    {
        return m_auras.find(abilityId) != m_auras.end();
    }

    Ability* GetAura(uint32 abilityId)
    {
        AbilityMap::iterator itr = m_auras.find(abilityId);
        if (itr == m_auras.end())
            return nullptr;

        return itr->second;
    }

    void RemoveAura(uint32 abilityId);

    void RemoveAura(AbilityMap::iterator& itr);

    int32 GetHealth() const { return m_hp; }
    int32 GetMaxHealth() const { return m_maxHp; }

    void SetHealth(int32 value);
    void SetMaxHealth() { SetHealth(GetMaxHealth()); }

    void ModifyHealth(int32 dVal);

    void DealDamageByEffect(Pet* victim, uint32 points, uint32 accuracy, Effect* effect);
    void HealByEffect(Pet* victim, uint32 points, uint32 accuracy, Effect* effect);

    uint8 GetFamily() const { return m_proto->GetFamily(); }

    bool IsDead() const { return HasState(BattlePetStates::BATTLE_PET_STATE_IS_DEAD); }
    void MakeDead();

    Battle* GetBattle() { return m_battle; }

    PBOID GetSide()
    {
        return (m_type == PBOID::P0_PET_0 || m_type == PBOID::P0_PET_1 || m_type == PBOID::P0_PET_2) ? PBOID::PAD_0 : PBOID::PAD_1;
    }

    uint16 GetXP() const { return m_xp; }

    uint16 GetLevel() const { return m_level; }

    uint16 GetOldLevel() const { return m_oldLevel; }

    void GixeXP(uint16 xp);

    uint32 CalculateHeal(Effect* effect, Pet* opponent, uint32 points);
    uint32 CalculateHealTaken(uint32 heal);

    int32 CalculateAccuracy(int32 accuracy);

    uint32 CalculateDamage(Effect* effect, Pet* opponent, uint32 points);
    uint32 CalculateDamageTaken(uint32 damage);
    uint32 GetCritChance() const;
    uint32 GetSpeed() const;

    void SetTakenDamage(uint32 damage) { SetState(this, BattlePetStates::BATTLE_PET_STATE_LAST_HITTAKEN, damage); }
    void SetDealtDamage(uint32 damage) { SetState(this, BattlePetStates::BATTLE_PET_STATE_LAST_HITDEALT, damage); }

    uint32 GetTakenDamage() { return GetStateValue(BattlePetStates::BATTLE_PET_STATE_LAST_HITTAKEN); }

    uint32 GetDealtDamage() { return GetStateValue(BattlePetStates::BATTLE_PET_STATE_LAST_HITDEALT); }

    void ClearDamage()
    {
        SetState(this, BattlePetStates::BATTLE_PET_STATE_LAST_HITTAKEN, 0);
        SetState(this, BattlePetStates::BATTLE_PET_STATE_LAST_HITDEALT, 0);
    }

    float AttackBonus() const { return 1 + 0.05f * GetProto()->GetPower(); }
    float HealingBonus() const { return 1 + 0.05f * GetProto()->GetPower(); }

    bool HasActiveMultiturnAbility() const
    {
        return m_hasActiveMultiTurnAbility;
    }

    void SetHasActiveMultiTurnAbility(bool set)
    {
        m_hasActiveMultiTurnAbility = set;
    }

private:
    void Initial();
    void InitialAbilities();

    PetTemplate* m_proto;
    PBOID m_type;
    
    StateMap m_states;
    CooldownMap m_cooldowns;
    AbilityList m_abilities;
    AbilityMap m_auras;

    int32 m_hp;
    int32 m_maxHp;
    uint16 m_xp;
    uint16 m_level;
    uint16 m_oldLevel;

    Battle* m_battle;

    bool m_hasActiveMultiTurnAbility;

    std::shared_ptr<MemberData> m_member;
};
}

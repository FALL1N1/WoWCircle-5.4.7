#pragma once

#include "BattlePetAbilityDefines.h"
#include "DB2Stores.h"
#include "DB2Structure.h"
#include "TemporarySummon.h"

namespace BattlePet
{
    class Pet;
    class Effect;
    class EventHolder;
    class MemberData;

    typedef std::vector<Effect*> Effects;
    typedef std::map<uint8, Effects> Turns;

    typedef void(Effect::*pEffect)();

    struct EffectHandler
    {
        BattlePetEffectType m_effect;
        pEffect m_ptr;

        EffectHandler()
        {
            m_effect = BattlePetEffectType::ABILITY_EFFECT_NONE;
            m_ptr = nullptr;
        }

        EffectHandler(BattlePetEffectType effect, pEffect ptr)
            : m_effect(effect), m_ptr(ptr)
        {
        }
    };

    class Ability
    {
    public:
        Ability(uint32 abilityId, std::shared_ptr<MemberData>& caster, std::shared_ptr<MemberData>& target, int32 duration = 0, BattlePetAbilityEffectEntry const* createdBy = nullptr);
        ~Ability();

        void CastAbility();
        void CastAura();

        Pet* GetCaster();
        Pet const* GetCaster() const;

        Pet* GetTarget();
        Pet const* GetTarget() const;

        uint32 GetId() const { return m_proto->AbilityId; }
        uint32 GetCooldown() const { return m_proto->Cooldown; }
        int32 GetFamily() const { return m_proto->FamilyId; }

        Effects* GetEffects(uint8 turn)
        {
            if (m_turns.empty())
                return nullptr;

            Turns::iterator itr = m_turns.find(turn);
            if (itr == m_turns.end())
            {
                itr = m_turns.find(1);
                return itr == m_turns.end() ? nullptr : &itr->second;
            }

            return &itr->second;
        }

        void Apply();
        void Remove();

        void Update();
        void RefreshDuration();
        void SetDuration(int32 newDuration);

        int32 GetDuration() const { return m_duration; }

        int32 GetDurationMax() const { return m_maxDuration; }

        bool IsExpired() const { return !GetDuration(); }

        std::shared_ptr<MemberData>& GetCasterMember() { return m_caster; }
        std::shared_ptr<MemberData> const & GetCasterMember() const { return m_caster; }

        std::shared_ptr<MemberData>& GetTargetMember() { return m_target; }
        std::shared_ptr<MemberData> const& GetTargetMember() const { return m_target; }

        bool HasEffect(BattlePetEffectType effect) const;

    private:
        void InitialEffects();
        void ModifyStates(bool apply);

        std::shared_ptr<MemberData> m_caster;
        std::shared_ptr<MemberData> m_target;

        void HandleEffects();
        void HandleAllEffects();

        Turns m_turns;
        uint8 m_turn;
        int32 m_duration;
        int32 m_maxDuration;
        bool m_isDisabled;

        BattlePetAbilityEntry const* m_proto;
        BattlePetAbilityEffectEntry const* m_createdBy;
    };

    class Effect
    {
    public:
        Effect(BattlePetAbilityEffectEntry const* effect, Ability* base);
        ~Effect();

        void HandleEffect();

        uint8 GetEffIndex() const { return m_effect->EffectIndex; }

        uint32 GetEffId() const { return m_effect->Id; }

        BattlePetEffectType GetEffectType() const { return BattlePetEffectType(m_effect->Effect); }

        BattlePetAbilityEffectEntry const* GetProto() const { return m_effect; }

        Ability* GetBase() { return m_base; }

        Ability const* GetBase() const { return m_base; }

        void HandleNotImplemented();
        void HandleUnknown();
        void HandleDummy();
        void HandleDamage();
        void HandleHeal();
        void HandleAddAura();
        void HandleAddAuraPeriodicDamage();
        void HandleAuraUnapply();

        bool IsActiveEnemy() const
        {
            return m_prop->VisualId == AbilityTarget::TARGET_ENEMY_ACTIVE_PET;
        }

        bool IsSelf() const
        {
            return m_prop->VisualId == AbilityTarget::TARGET_SELF_ACTIVE_PET ||
                m_prop->VisualId == AbilityTarget::TARGET_SELF_ACTIVE_PET_2 ||
                m_prop->VisualId == AbilityTarget::TARGET_SELF_ACTIVE_PET_3 ||
                m_prop->VisualId == AbilityTarget::TARGET_SELF_ACTIVE_PET_4;
        }

        Pet* GetCaster() { return m_caster; }
        Pet* GetTarget();

    private:

        Ability* m_base;
        Pet* m_caster;
        Pet* m_target;

        BattlePetAbilityEffectEntry const* m_effect;
        BattlePetEffectPropertiesEntry const* m_prop;
    };
}

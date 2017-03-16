#include "BattlePetAbility.h"
#include "BattlePetAbilityEffect.h"
#include "BattlePet.h"
#include "Battle/PetBattleEvent.h"
#include "Battle/PetBattle.h"
#include "ObjectMgr.h"

/* Проблема с таргетами для абилок. Сейчас каждыйэ ффект берет текущего активного пета
Это сделано для тех случаев, когда аура накладывается на себя, но бьет по врагу.

До этого аура для эффектом брала, в качестве цели, ту же цель, что и при наложении ауры
Из-за этого некоторые ауры, котоыре накладывались на себя, позже дамажили как раз таки себя, а не врага

Из-за переделки, после которой целью береться активный пет, ауры переодического урона,
multi turn абилки которые по механике как ауры, наносят урон в активного пета, и при смене пета
урон будет идти также в активного пета, т.е таким образом можно прятать петов. Этого быть не должно

Как вариант: добавить в Effect указатели на Pet
в зависимости от того что это за эффект.
Т.к эффект не предоставляет инфу о целях, надо изощряться и определять цель по эффекту
на этапе инициализации обьекта

*/

BattlePet::EffectHandler _Effects[MAX_ABILITY_EFFECT] =
{
    { BattlePetEffectType::ABILITY_EFFECT_DUMMY, &BattlePet::Effect::HandleDummy },
    { BattlePetEffectType::ABILITY_EFFECT_HEAL, &BattlePet::Effect::HandleHeal },
    { BattlePetEffectType::ABILITY_EFFECT_HEAL_PCT, &BattlePet::Effect::HandleHeal },
    { BattlePetEffectType::ABILITY_EFFECT_DAMAGE, &BattlePet::Effect::HandleDamage },
    { BattlePetEffectType::ABILITY_EFFECT_DAMAGE2, &BattlePet::Effect::HandleDamage },
    { BattlePetEffectType::ABILITY_EFFECT_DAMAGE_PCT_FROM_HP, &BattlePet::Effect::HandleDamage },
    { BattlePetEffectType::ABILITY_EFFECT_DAMAGE_PCT_FROM_TAKEN, &BattlePet::Effect::HandleDamage },
    { BattlePetEffectType::ABILITY_EFFECT_AURA, &BattlePet::Effect::HandleAddAura },
    { BattlePetEffectType::ABILITY_EFFECT_IMMUNE_AURA, &BattlePet::Effect::HandleAddAura },
    { BattlePetEffectType::ABILITY_EFFECT_AURA_CONDITION, &BattlePet::Effect::HandleAddAura },
    { BattlePetEffectType::ABILITY_EFFECT_AURA_WEATHER, &BattlePet::Effect::HandleAddAura },
    { BattlePetEffectType::ABILITY_EFFECT_AURA_PERIODIC_HEAL_PCT, &BattlePet::Effect::HandleAddAura },
    { BattlePetEffectType::ABILITY_EFFECT_AURA_PERIODIC_DAMAGE, &BattlePet::Effect::HandleAddAuraPeriodicDamage },
    { BattlePetEffectType::ABILITY_EFFECT_AURA_UNAPPLY , &BattlePet::Effect::HandleAuraUnapply },

};

BattlePet::Ability::Ability(uint32 abilityId, std::shared_ptr<MemberData>& caster, std::shared_ptr<MemberData>& target, int32 duration, BattlePetAbilityEffectEntry const* createdBy)
    : m_caster(caster)
    , m_target(target)
    , m_createdBy(createdBy)
    , m_turn(1)
    , m_duration(duration)
    , m_maxDuration(duration)
    , m_isDisabled(true)
{
    ASSERT(abilityId);

    m_proto = sBattlePetAbilityStore.LookupEntry(abilityId);

    ASSERT(m_proto);

    InitialEffects();

    Apply();
}

BattlePet::Ability::~Ability()
{
    for (auto const& pair : m_turns)
    {
        for (auto const effect : pair.second)
        {
            delete effect;
        }
    }

    m_turns.clear();
}

void BattlePet::Ability::InitialEffects()
{
    BattlePetAbilityEffectsMap::iterator itr = sBattlePetAbilityEffectsMap.find(m_proto->AbilityId);
    if (itr == sBattlePetAbilityEffectsMap.end())
        return;

    for (auto const& turns : itr->second)
    {
        for (auto const effect : turns.second)
        {
            m_turns[turns.first].push_back(new Effect(effect, this));
        }
    }
}

void BattlePet::Ability::CastAbility()
{
    HandleAllEffects();
}

void BattlePet::Ability::CastAura()
{
    HandleEffects();
}

void BattlePet::Ability::HandleEffects()
{
    auto effects = GetEffects(m_turn);
    if (effects == nullptr)
        return;

    for (Effect* effect : *effects)
    {
        effect->HandleEffect();
    }
}

void BattlePet::Ability::HandleAllEffects()
{
    for (auto& turn : m_turns)
    {
        if (turn.first == 0)
            continue;

        for (auto& effect : turn.second)
        {
            if (effect)
                effect->HandleEffect();
        }
    }
}

void BattlePet::Ability::RefreshDuration()
{
    if (m_duration == m_maxDuration)
        return;

    SetDuration(m_maxDuration);
}

void BattlePet::Ability::SetDuration(int32 newDuration)
{
    m_duration = newDuration;

    GetTarget()->GetBattle()->OnAuraChanged(GetTarget(), GetId(), 0/*newAmount*/, m_duration);
}

/* Также аура триггерит эффекты не только при изменении хода,
у многих из них также есть прок система*/
void BattlePet::Ability::Update()
{
    if (!m_isDisabled)
    {
        if (m_duration > 0)
        {
            SetDuration(m_duration - 1);
        }

        ++m_turn;

        HandleEffects();
    }
    else
        m_isDisabled = false;
}

void BattlePet::Ability::Apply()
{
    if (BattlePetAbilityData const* data = sObjectMgr->GetAbilityData(GetId()))
    {
        if (data->m_flags & 0x1)
        {
            GetCaster()->SetHasActiveMultiTurnAbility(true);
        }
    }

    ModifyStates(true);
}

void BattlePet::Ability::Remove()
{
    if (BattlePetAbilityData const* data = sObjectMgr->GetAbilityData(GetId()))
    {
        if (data->m_flags & 0x1)
        {
            GetCaster()->SetHasActiveMultiTurnAbility(false);
        }
    }

    ModifyStates(false);
}

void BattlePet::Ability::ModifyStates(bool apply)
{
    BattlePetAbilityStatesMap::iterator itr = sBattlePetAbilityStatesMap.find(GetId());
    if (itr == sBattlePetAbilityStatesMap.end())
        return;

    for (BattlePetAbilityStateEntry const* state : itr->second)
    {
        int value = int32(floor(state->Value * GetTarget()->AttackBonus()));
        switch ((BattlePetStates)state->StateId)
        {
        case BattlePetStates::BATTLE_PET_STATE_MOD_DAMAGEDEALTPERCENT:
        case BattlePetStates::BATTLE_PET_STATE_MOD_DAMAGETAKENPERCENT:
        case BattlePetStates::BATTLE_PET_STATE_MOD_HEALINGDEALTPERCENT:
        case BattlePetStates::BATTLE_PET_STATE_MOD_HEALINGTAKENPERCENT:
        case BattlePetStates::BATTLE_PET_STATE_MOD_MAXHEALTHPERCENT:
        case BattlePetStates::BATTLE_PET_STATE_MOD_SPEEDPERCENT:
        case BattlePetStates::BATTLE_PET_STATE_MOD_PETTYPEDAMAGEDEALTPERCENT:
        case BattlePetStates::BATTLE_PET_STATE_MOD_PETTYPEDAMAGETAKENPERCENT:
        case BattlePetStates::BATTLE_PET_STATE_STAT_CRITCHANCE:
        case BattlePetStates::BATTLE_PET_STATE_STAT_ACCURACY:
        case BattlePetStates::BATTLE_PET_STATE_STAT_DODGE:
            value = state->Value;
            break;
        }
        GetTarget()->ModifyState(GetCaster(), (BattlePetStates)state->StateId, value, apply);
    }
}

BattlePet::Pet* BattlePet::Ability::GetCaster()
{
    return GetCasterMember()->GetActivePet();
}

BattlePet::Pet const* BattlePet::Ability::GetCaster() const
{
    return GetCasterMember()->GetActivePet();
}

BattlePet::Pet* BattlePet::Ability::GetTarget()
{
    return GetTargetMember()->GetActivePet();
}

BattlePet::Pet const* BattlePet::Ability::GetTarget() const
{
    return GetTargetMember()->GetActivePet();
}

bool BattlePet::Ability::HasEffect(BattlePetEffectType effect) const
{
    for (auto turns : m_turns)
    {
        for (auto effects : turns.second)
        {
            if (effects->GetEffectType() != effect)
                continue;

            return true;
        }
    }

    return false;
}

/* Effects */

BattlePet::Effect::Effect(BattlePetAbilityEffectEntry const* effect, Ability* base)
    : m_effect(effect)
    , m_base(base)
    , m_caster(nullptr)
    , m_target(nullptr)
{
    ASSERT(m_effect);
    ASSERT(m_base);

    m_prop = sBattlePetEffectPropertiesStore.LookupEntry(effect->Effect);

    m_caster = base->GetCasterMember()->GetActivePet();

    if (IsSelf())
        m_target = base->GetCasterMember()->GetActivePet();
    else
        m_target = base->GetCasterMember()->GetOpponent()->GetActivePet();
}

BattlePet::Effect::~Effect()
{

}

BattlePet::Pet* BattlePet::Effect::GetTarget()
{
    if (IsActiveEnemy())
        return GetBase()->GetCasterMember()->GetOpponent()->GetActivePet();

    return m_target;
}

void BattlePet::Effect::HandleEffect()
{
    for (EffectHandler& handler : _Effects)
    {
        if (handler.m_effect != GetEffectType())
            continue;

        (*this.*handler.m_ptr)();
    }
}

void BattlePet::Effect::HandleNotImplemented()
{
    sLog->outError(LOG_FILTER_SPELLS_AURAS, "Unhandled battle pet ability effect %u for ability %u", m_effect->Effect, m_base->GetId());
}

void BattlePet::Effect::HandleUnknown()
{
    sLog->outError(LOG_FILTER_SPELLS_AURAS, "Unknown battle pet ability effect %u for ability %u", m_effect->Effect, m_base->GetId());
}

void BattlePet::Effect::HandleDummy()
{
}

void BattlePet::Effect::HandleHeal()
{
    int points = m_effect->heal.m_points;
    int accuracy = m_effect->heal.m_accuracy;
    if (GetEffectType() == BattlePetEffectType::ABILITY_EFFECT_HEAL_PCT)
    {
        points = CalculatePct(GetCaster()->GetMaxHealth(), m_effect->healPct.m_pct);
        accuracy = m_effect->healPct.m_accuracy;
    }

    GetCaster()->HealByEffect(GetTarget(), points, accuracy, this);
}

/*Неоторые ауры с переодическим уроном, будут наносить урон только по активному пету противника, т.е когда тот сменится
урон будет идти в активного. Некоторые необорот сохраняют свою цель. Т.е замена пета ничего не изменит, урон будет идти по тому же пету*/

void BattlePet::Effect::HandleDamage()
{
    int32 accuracy = m_effect->damage.m_accuracy;
    int32 damage = m_effect->damage.m_points;
    if (GetEffectType() == BattlePetEffectType::ABILITY_EFFECT_DAMAGE2)
    {
        if (GetCaster()->HasState(BattlePetStates(m_effect->damage2.m_casterState)))
            accuracy = 100;
        if (GetTarget()->HasState(BattlePetStates(m_effect->damage2.m_targetState)))
            accuracy = 100;
    }
    else if (GetEffectType() == BattlePetEffectType::ABILITY_EFFECT_DAMAGE_PCT_FROM_TAKEN)
    {
        accuracy = m_effect->damageFromTakenLastRound.m_accuracy;
        damage = CalculatePct(GetCaster()->GetTakenDamage(), m_effect->damageFromTakenLastRound.m_power);
    }
    else if (GetEffectType() == BattlePetEffectType::ABILITY_EFFECT_DAMAGE_PCT_FROM_HP)
    {
        accuracy = m_effect->damagePctFromHp.m_accuracy;
        damage = CalculatePct(GetTarget()->GetMaxHealth(), m_effect->damagePctFromHp.m_points);
    }

    GetCaster()->DealDamageByEffect(GetTarget(), damage, accuracy, this);
}

void BattlePet::Effect::HandleAddAura()
{
    int32 duration = m_effect->triggerAura.m_duration;

    if (GetEffectType() == BattlePetEffectType::ABILITY_EFFECT_AURA_CONDITION)
    {
        if (BattlePetStates casterState = (BattlePetStates)m_effect->addAuraCondition.m_casterState)
        {
            if (!GetCaster()->HasState(casterState))
                return;
        }
        if (BattlePetStates targetState = (BattlePetStates)m_effect->addAuraCondition.m_targetState)
        {
            if (!GetTarget()->HasState(targetState))
                return;
        }

        duration = m_effect->addAuraCondition.m_duration;
    }

    if (duration == 0)
        duration = -1;

    if (Ability* aura = GetTarget()->AddAura(
        m_effect->TriggerId, duration, GetBase()->GetCasterMember(), m_effect))
    {
        aura->CastAura();
    }
}

void BattlePet::Effect::HandleAddAuraPeriodicDamage()
{
    if (Ability* aura = GetTarget()->AddAura(
        m_effect->TriggerId, m_effect->triggerAuraPeriodicDamage.m_duration, GetBase()->GetCasterMember(), m_effect))
    {
        aura->CastAura();
    }
}

void BattlePet::Effect::HandleAuraUnapply()
{
    GetTarget()->RemoveAura(m_effect->TriggerId);
}

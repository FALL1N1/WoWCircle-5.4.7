/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_PALADIN and SPELLFAMILY_GENERIC spells used by paladin players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pal_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum PaladinSpells
{
    PALADIN_SPELL_JUDGMENT                       = 20271,
    PALADIN_SPELL_JUDGMENTS_OF_THE_BOLD          = 111529,
    PALADIN_SPELL_JUDGMENTS_OF_THE_WISE          = 105424,
    PALADIN_SPELL_PHYSICAL_VULNERABILITY         = 81326,
    PALADIN_SPELL_LONG_ARM_OF_THE_LAW            = 87172,
    PALADIN_SPELL_LONG_ARM_OF_THE_LAW_RUN_SPEED  = 87173,
    PALADIN_SPELL_BURDEN_OF_GUILT                = 54931,
    PALADIN_SPELL_BURDEN_OF_GUILT_DECREASE_SPEED = 110300,
    PALADIN_SPELL_HOLY_SHOCK_R1                  = 20473,
    PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE           = 25912,
    PALADIN_SPELL_HOLY_SHOCK_R1_HEALING          = 25914,
    PALADIN_SPELL_HOLY_SHOCK_ENERGIZE            = 148976,
    SPELL_BLESSING_OF_LOWER_CITY_DRUID           = 37878,
    SPELL_BLESSING_OF_LOWER_CITY_PALADIN         = 37879,
    SPELL_BLESSING_OF_LOWER_CITY_PRIEST          = 37880,
    SPELL_BLESSING_OF_LOWER_CITY_SHAMAN          = 37881,
    SPELL_DIVINE_STORM                           = 53385,
    SPELL_DIVINE_STORM_DUMMY                     = 54171,
    SPELL_DIVINE_STORM_HEAL                      = 54172,
    SPELL_FORBEARANCE                            = 25771,
    PALADIN_SPELL_WORD_OF_GLORY                  = 85673,
    PALADIN_SPELL_WORD_OF_GLORY_HEAL             = 130551,
    PALADIN_SPELL_GLYPH_OF_WORD_OF_GLORY         = 54936,
    PALADIN_SPELL_GLYPH_OF_WORD_OF_GLORY_DAMAGE  = 115522,
    PALADIN_SPELL_CONSECRATION_AREA_DUMMY        = 81298,
    PALADIN_SPELL_CONSECRATION_DAMAGE            = 81297,
    PALADIN_SPELL_HOLY_PRISM_ALLIES              = 114871,
    PALADIN_SPELL_HOLY_PRISM_ENNEMIES            = 114852,
    PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL       = 114862,
    PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2     = 114870,
    PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL         = 121551,
    PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2       = 121552,
    PALADIN_SPELL_ARCING_LIGHT_HEAL              = 119952,
    PALADIN_SPELL_ARCING_LIGHT_DAMAGE            = 114919,
    PALADIN_SPELL_EXECUTION_SENTENCE             = 114916,
    PALADIN_SPELL_STAY_OF_EXECUTION              = 114917,
    PALADIN_SPELL_INQUISITION                    = 84963,
    PALADIN_SPELL_GLYPH_OF_BLINDING_LIGHT        = 54934,
    PALADIN_SPELL_BLINDING_LIGHT_CONFUSE         = 105421,
    PALADIN_SPELL_BLINDING_LIGHT_STUN            = 115752,
    PALADIN_SPELL_EXORCISM                       = 879,
    PALADIN_SPELL_MASS_EXORCISM                  = 122032,
    PALADIN_SPELL_SACRED_SHIELD                  = 65148,
    PALADIN_SPELL_ARDENT_DEFENDER_HEAL           = 66235,
    PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE     = 88852,
    PALADIN_SPELL_BEACON_OF_LIGHT                = 53563,
    PALADIN_SPELL_BEACON_OF_LIGHT_AURA           = 53651,
    PALADIN_SPELL_SELFLESS_HEALER                = 85804,
    PALADIN_SPELL_SELFLESS_HEALER_STACK          = 114250,
    PALADIN_SPELL_ETERNAL_FLAME                  = 114163,
    PALADIN_SPELL_SHIELD_OF_THE_RIGHTEOUS_PROC   = 132403,
    PALADIN_SPELL_BASTION_OF_GLORY               = 114637,
    PALADIN_SPELL_DIVINE_PURPOSE                 = 90174,
    PALADIN_SPELL_DIVINE_SHIELD                  = 642,
    PALADIN_SPELL_LAY_ON_HANDS                   = 633,
    PALADIN_SPELL_DIVINE_PROTECTION              = 498,
    PALADIN_SPELL_GLYPH_OF_AVENGING_WRATH        = 54927,
    PALADIN_SPELL_AVENGING_WRATH_REGEN_BY_GLYPH  = 115547,
    PALADIN_SPELL_SACRED_CLEANSING               = 53551,
    PALADIN_SPELL_DAYBREAK_AURA                  = 88821,
    PALADIN_SPELL_DAYBREAK_PROC                  = 88819,
    PALADIN_SPELL_DAYBREAK_HEAL                  = 121129,
    PALADIN_SPELL_TOWER_OF_RADIANCE              = 85512,
    PALADIN_SPELL_HOLY_AVENGER                   = 105809,
    PALADIN_ITEM_PVP_HOLY_4P_BONUS               = 131665,
    PALADIN_SPELL_GLYPH_OF_DIVINE_STORM          = 63220,
    PALADIN_SPELL_GLYPH_OF_DIVINE_STORM_HEAL     = 115515,
    PALADIN_SPELL_GLYPH_OF_DENOUNCE              = 56420,
    PALADIN_SPELL_GLYPH_OF_DENOUNCE_PROC         = 115654,
    PALADIN_SPELL_SANCTIFIED_WRATH_TALENT        = 53376,
    PALADIN_SPELL_SANCTIFIED_WRATH_BONUS         = 114232,
    PALADIN_SPELL_AVENGING_WRATH                 = 31884,
    PALADIN_SPELL_HAMMER_OF_WRATH                = 24275,
    PALADIN_SPELL_WEAKENED_BLOWS                 = 115798,
    PALADIN_SACRED_SHIELD_HOLY                   = 148039,
    PALADIN_T16_RETRIBUTION_2P                   = 144586,
    PALADIN_SPELL_WARRIOR_OF_THE_LIGHT           = 144587,
    PALADIN_SPELL_HOLY_INSIGHT                   = 112859,
    GLYPH_OF_PROTECTOR_OF_THE_INNOCENT           = 93466,
    GLYPH_OF_PROTECTOR_OF_THE_INNOCENT_HEAL      = 115536,
    GLYPH_OF_DEVINE_SHIELD                       = 146956,
    GLYPH_OF_DOUBLE_JEOPARDY                     = 54922,
    GLYPH_OF_DOUBLE_JEOPARDY_EFFECT              = 121027,
    PALADIN_SPELL_CONTEMPLATION                  = 121183,
    PALADIN_SPELL_SEAL_OF_INSIGHT                = 20165,
    PALADIN_SPELL_EXORCISM_ENERGIZE              = 147715
};

// Called by Avenging Wrath - 31884
// Sanctified Wrath - 53376
class spell_pal_sanctified_wrath : public SpellScriptLoader
{
    public:
        spell_pal_sanctified_wrath() : SpellScriptLoader("spell_pal_sanctified_wrath") { }

        class spell_pal_sanctified_wrath_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_sanctified_wrath_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(PALADIN_SPELL_SANCTIFIED_WRATH_TALENT))
                        _player->CastSpell(_player, PALADIN_SPELL_SANCTIFIED_WRATH_BONUS, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->RemoveAura(PALADIN_SPELL_SANCTIFIED_WRATH_BONUS);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_pal_sanctified_wrath_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_pal_sanctified_wrath_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_sanctified_wrath_AuraScript();
        }
};

// Called by Divine Storm - 53385
// Glyph of Divine Storm - 63220
class spell_pal_glyph_of_divine_storm : public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_divine_storm() : SpellScriptLoader("spell_pal_glyph_of_divine_storm") { }

        class spell_pal_glyph_of_divine_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_glyph_of_divine_storm_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(PALADIN_SPELL_GLYPH_OF_DIVINE_STORM))
                        _player->CastSpell(_player, PALADIN_SPELL_GLYPH_OF_DIVINE_STORM_HEAL, true);
                    // Item - Paladin T16 Retribution 4P Bonus
                    if (_player->HasAura(144595))
                        _player->RemoveAura(144595);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pal_glyph_of_divine_storm_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_glyph_of_divine_storm_SpellScript();
        }
};

// Holy Radiance - 82327
class spell_pal_holy_radiance : public SpellScriptLoader
{
    public:
        spell_pal_holy_radiance() : SpellScriptLoader("spell_pal_holy_radiance") { }

        class spell_pal_holy_radiance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_radiance_SpellScript);

            uint32 m_TargetCount;
            uint64 m_TargetId;

            enum Constants : int32
            {
                FullHealTargetCount = 6, ///< After this amount of targets the spell will lose BasePoints% efficiency for each target
                HealDecrease        = 5, ///< % of heal lost per target over FullHealTargetCount
                MinimalHeal         = 5  ///< Minimal heal % applied on each target when a lot of target are hit
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                m_TargetCount = p_Targets.size();

                if (!GetCaster())
                    return;
            }

            void HandleBeforeCast()
            {
                Unit* l_Target = GetExplTargetUnit();
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Target)
                    return;

                m_TargetId = l_Target->GetGUID();
            }

            void HandleOnHit()
            {
                Unit* l_Target = GetHitUnit();
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Target || !m_TargetId)  ///< m_TargetId can't be uninitialized if the caster and the target are valid
                    return;

                int32 l_HealValue = GetHitHeal();
                if (l_Target->GetGUID() != m_TargetId)
                {
                    int32 l_Pct = GetSpellInfo()->Effects[EFFECT_0].BasePoints;
                    if (m_TargetCount > Constants::FullHealTargetCount)
                        l_Pct = std::max<int32>(l_Pct - Constants::HealDecrease * (m_TargetCount - Constants::FullHealTargetCount), Constants::MinimalHeal);

                    ApplyPct(l_HealValue, l_Pct);
                }

                SetHitHeal(l_HealValue);
            }

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(PALADIN_SPELL_DAYBREAK_AURA))
                        l_Caster->CastSpell(l_Caster, PALADIN_SPELL_DAYBREAK_PROC, true);

                    l_Caster->CastSpell(l_Caster, PALADIN_SPELL_EXORCISM_ENERGIZE, true);
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_pal_holy_radiance_SpellScript::HandleAfterCast);
                BeforeCast += SpellCastFn(spell_pal_holy_radiance_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_pal_holy_radiance_SpellScript::HandleOnHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_holy_radiance_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_pal_holy_radiance_SpellScript();
        }
};

// Hand of Purity - 114039
class spell_pal_hand_of_purity : public SpellScriptLoader
{
    public:
        spell_pal_hand_of_purity() : SpellScriptLoader("spell_pal_hand_of_purity") { }

        class spell_pal_hand_of_purity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_hand_of_purity_AuraScript);

            uint32 healtPct;

            bool Load()
            {
                healtPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return GetUnitOwner()->ToPlayer();
            }

            bool CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
                return true;
            }

            void Absorb(AuraEffect* /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                SpellInfo const* procSpell = dmgInfo.GetSpellInfo();
                if (!procSpell)
                    return;

                if (!procSpell->HasAura(SPELL_AURA_PERIODIC_DAMAGE) && !procSpell->HasAura(SPELL_AURA_PERIODIC_DAMAGE_PERCENT))
                    return;

                absorbAmount = CalculatePct(dmgInfo.GetDamage(), healtPct);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_hand_of_purity_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_pal_hand_of_purity_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_hand_of_purity_AuraScript();
        }
};

// Called by Avenging Wrath - 31884
// Glyph of Avenging Wrath - 54927
class spell_pal_glyph_of_avenging_wrath : public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_avenging_wrath() : SpellScriptLoader("spell_pal_glyph_of_avenging_wrath") { }

        class spell_pal_glyph_of_avenging_wrath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_glyph_of_avenging_wrath_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(PALADIN_SPELL_GLYPH_OF_AVENGING_WRATH))
                        _player->CastSpell(_player, PALADIN_SPELL_AVENGING_WRATH_REGEN_BY_GLYPH, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_glyph_of_avenging_wrath_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_glyph_of_avenging_wrath_SpellScript();
        }
};

// Unbreakable Spirit - 114154
class spell_pal_unbreakable_spirit : public SpellScriptLoader
{
    public:
        spell_pal_unbreakable_spirit() : SpellScriptLoader("spell_pal_unbreakable_spirit") { }

        class spell_pal_unbreakable_spirit_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_unbreakable_spirit_AuraScript);

            uint32 holyPowerConsumed;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                holyPowerConsumed = 0;
            }

            void SetData(uint32 type, uint32 data)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    holyPowerConsumed = data;

                    if (_player->HasSpellCooldown(PALADIN_SPELL_DIVINE_SHIELD))
                    {
                        uint32 totalCooldown = sSpellMgr->GetSpellInfo(PALADIN_SPELL_DIVINE_SHIELD)->RecoveryTime;
                        int32 lessCooldown = CalculatePct(totalCooldown, holyPowerConsumed);
                        _player->ReduceSpellCooldown(PALADIN_SPELL_DIVINE_SHIELD, lessCooldown);
                    }
                    if (_player->HasSpellCooldown(PALADIN_SPELL_LAY_ON_HANDS))
                    {
                        uint32 totalCooldown = sSpellMgr->GetSpellInfo(PALADIN_SPELL_LAY_ON_HANDS)->CategoryRecoveryTime;
                        int32 lessCooldown = CalculatePct(totalCooldown, holyPowerConsumed);
                        _player->ReduceSpellCooldown(PALADIN_SPELL_LAY_ON_HANDS, lessCooldown);
                    }
                    if (_player->HasSpellCooldown(PALADIN_SPELL_DIVINE_PROTECTION))
                    {
                        uint32 totalCooldown = sSpellMgr->GetSpellInfo(PALADIN_SPELL_DIVINE_PROTECTION)->RecoveryTime;
                        int32 lessCooldown = CalculatePct(totalCooldown, holyPowerConsumed);
                        _player->ReduceSpellCooldown(PALADIN_SPELL_DIVINE_PROTECTION, lessCooldown);
                    }
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_pal_unbreakable_spirit_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_unbreakable_spirit_AuraScript();
        }
};

// Shield of the Righteous - 53600
class spell_pal_shield_of_the_righteous : public SpellScriptLoader
{
    public:
        spell_pal_shield_of_the_righteous() : SpellScriptLoader("spell_pal_shield_of_the_righteous") { }

        class spell_pal_shield_of_the_righteous_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_shield_of_the_righteous_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (_player->HasAura(PALADIN_SPELL_SHIELD_OF_THE_RIGHTEOUS_PROC))
                        {
                            if (Aura* shieldofTheRighteous = _player->GetAura(PALADIN_SPELL_SHIELD_OF_THE_RIGHTEOUS_PROC))
                            {
                                int32 currentDuration = shieldofTheRighteous->GetDuration();
                                int32 maxDuration = shieldofTheRighteous->GetMaxDuration();
                                shieldofTheRighteous->SetDuration(maxDuration + currentDuration);
                            }
                        }
                        else
                        {
                            // -30% damage taken for 3s
                            _player->CastSpell(_player, PALADIN_SPELL_SHIELD_OF_THE_RIGHTEOUS_PROC, true);
                        }

                        _player->CastSpell(_player, PALADIN_SPELL_BASTION_OF_GLORY, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_shield_of_the_righteous_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_shield_of_the_righteous_SpellScript();
        }
};

// Selfless healer - 85804
// Called by flash of light - 19750, Holy Light - 82326 - 135403, Holy Radiance - 82327
class spell_pal_selfless_healer : public SpellScriptLoader
{
    public:
        spell_pal_selfless_healer() : SpellScriptLoader("spell_pal_selfless_healer") { }

        class spell_pal_selfless_healer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_selfless_healer_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Target != l_Caster)
                        {
                            if (Aura* l_SelflessHealer = l_Caster->GetAura(PALADIN_SPELL_SELFLESS_HEALER_STACK))
                            {
                                int32 l_Heal = GetHitHeal();
                                AddPct(l_Heal, 20 * l_SelflessHealer->GetStackAmount());

                                SetHitHeal(l_Heal);
                            }
                        }

                        // Item - Paladin PvP Set Holy 4P Bonus - 131665
                        if (GetSpellInfo()->Id == 19750 && l_Caster->HasAura(PALADIN_ITEM_PVP_HOLY_4P_BONUS))
                            l_Caster->EnergizeBySpell(l_Caster, 19750, 1, POWER_HOLY_POWER);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_selfless_healer_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_selfless_healer_SpellScript();
        }
};

// Selfless Healer - 85804
// Aura must be removed after cast Divine Light (82326) or Holy Radiance (82327).
class spell_pal_selfless_healer_spell : public SpellScriptLoader
{
    public:
        spell_pal_selfless_healer_spell() : SpellScriptLoader("spell_pal_selfless_healer_spell") { }

        class spell_pal_selfless_healer_spell_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_selfless_healer_spell_SpellScript);

            void HandleOnCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        // Report just for Divine Light, so we add bonus only for it
                        if (_player->HasAura(PALADIN_SPELL_SELFLESS_HEALER_STACK) && GetSpellInfo()->Id == 82326)
                        {
                            int32 charges = _player->GetAura(PALADIN_SPELL_SELFLESS_HEALER_STACK)->GetStackAmount();

                            if (_player->IsValidAssistTarget(target) && target != _player)
                                SetHitHeal(int32(GetHitHeal() + ((GetHitHeal() * 0.2f) * charges)));
                        }
                    }
                }
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(PALADIN_SPELL_SELFLESS_HEALER_STACK))
                        _player->RemoveAura(PALADIN_SPELL_SELFLESS_HEALER_STACK);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_pal_selfless_healer_spell_SpellScript::HandleOnCast);
                AfterCast += SpellCastFn(spell_pal_selfless_healer_spell_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_selfless_healer_spell_SpellScript();
        }
};

// Called by Flash of Light - 19750 and Divine Light - 82326
// Tower of Radiance - 85512
class spell_pal_tower_of_radiance : public SpellScriptLoader
{
    public:
        spell_pal_tower_of_radiance() : SpellScriptLoader("spell_pal_tower_of_radiance") { }

        class spell_pal_tower_of_radiance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_tower_of_radiance_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                    {
                        int32 restorePower = 1;

                        // With Holy Avenger we restore 3 holy powers
                        if (_player->HasAura(PALADIN_SPELL_HOLY_AVENGER))
                            restorePower += 2;

                        if (_player->HasAura(PALADIN_SPELL_TOWER_OF_RADIANCE))
                            if (target->HasAura(PALADIN_SPELL_BEACON_OF_LIGHT, _player->GetGUID()))
                                _player->EnergizeBySpell(_player, PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE, restorePower, POWER_HOLY_POWER);
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_tower_of_radiance_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_tower_of_radiance_SpellScript();
        }
};

// Sacred shield - 20925 and Sacred Shield (Holy) - 148039
class spell_pal_sacred_shield : public SpellScriptLoader
{
    public:
        spell_pal_sacred_shield() : SpellScriptLoader("spell_pal_sacred_shield") { }

        class spell_pal_sacred_shield_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_sacred_shield_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetTarget())
                        _player->CastSpell(target, PALADIN_SPELL_SACRED_SHIELD, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_sacred_shield_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_sacred_shield_AuraScript();
        }
};

// Sacred shield absorb - 65148
class spell_pal_sacred_shield_absorb : public SpellScriptLoader
{
    public:
        spell_pal_sacred_shield_absorb() : SpellScriptLoader("spell_pal_sacred_shield_absorb") { }

        class spell_pal_sacred_shield_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_sacred_shield_absorb_AuraScript);

            bool CalculateAmount(AuraEffect const* , int32 & amount, bool & )
            {
                if (!GetCaster())
                    return false;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_PALADIN_PROTECTION)
                        amount = int32(30 + _player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY) * 0.819f);
                    else
                        amount = int32(30 + _player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY) * 1.17f);
                    
                    amount *= _player->GetPvpHealingBonus();
                }

                return true;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_sacred_shield_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_sacred_shield_absorb_AuraScript();
        }
};

// Sacred shield absorb holy spec - 148039
class spell_pal_sacred_shield_absorb_holy : public SpellScriptLoader
{
    public:
        spell_pal_sacred_shield_absorb_holy() : SpellScriptLoader("spell_pal_sacred_shield_absorb_holy") { }

        class spell_pal_sacred_shield_absorb_holy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_sacred_shield_absorb_holy_AuraScript);

            bool CalculateAmount(AuraEffect const* , int32 & amount, bool & )
            {
                if (!GetCaster())
                    return false;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_PALADIN_PROTECTION)
                        amount = int32(30 + _player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY) * 0.819f);
                    else
                        amount = int32(30 + _player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY) * 1.17f);
                }

                return true;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_sacred_shield_absorb_holy_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_sacred_shield_absorb_holy_AuraScript();
        }
};

// Emancipate - 121783
class spell_pal_emancipate : public SpellScriptLoader
{
    public:
        spell_pal_emancipate() : SpellScriptLoader("spell_pal_emancipate") { }

        class spell_pal_emancipate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_emancipate_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    std::list<Aura*> auraList;

                    for (auto itr : _player->GetAppliedAuras())
                    {
                        Aura* aura = itr.second->GetBase();
                        if (aura && aura->GetSpellInfo()->GetAllEffectsMechanicMask() & ((1<<MECHANIC_SNARE)|(1<<MECHANIC_ROOT)))
                            auraList.push_back(aura);
                    }

                    if (!auraList.empty())
                    {
                        JadeCore::Containers::RandomResizeList(auraList, 1);
                        _player->RemoveAura(*auraList.begin());
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_emancipate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_emancipate_SpellScript();
        }
};

// Art of War - 59578
class spell_pal_art_of_war : public SpellScriptLoader
{
    public:
        spell_pal_art_of_war() : SpellScriptLoader("spell_pal_art_of_war") { }

        class spell_pal_art_of_war_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_art_of_war_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasSpellCooldown(PALADIN_SPELL_EXORCISM))
                        _player->RemoveSpellCooldown(PALADIN_SPELL_EXORCISM, true);

                    // Glyph of Mass Exorcism
                    if (_player->HasSpellCooldown(PALADIN_SPELL_MASS_EXORCISM))
                        _player->RemoveSpellCooldown(PALADIN_SPELL_MASS_EXORCISM, true);

                    if (_player->HasAura(PALADIN_T16_RETRIBUTION_2P))
                        _player->CastSpell(_player,PALADIN_SPELL_WARRIOR_OF_THE_LIGHT, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_art_of_war_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_art_of_war_SpellScript();
        }
};

// Seal of Insight - 20167
class spell_pal_seal_of_insight : public SpellScriptLoader
{
    public:
        spell_pal_seal_of_insight() : SpellScriptLoader("spell_pal_seal_of_insight") { }

        class spell_pal_seal_of_insight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_seal_of_insight_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->EnergizeBySpell(_player, GetSpellInfo()->Id, int32(_player->GetMaxPower(POWER_MANA) * 0.04), POWER_MANA);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_seal_of_insight_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_seal_of_insight_SpellScript();
        }
};

// Blinding Light - 115750
class spell_pal_blinding_light : public SpellScriptLoader
{
    public:
        spell_pal_blinding_light() : SpellScriptLoader("spell_pal_blinding_light") { }

        class spell_pal_blinding_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_blinding_light_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(PALADIN_SPELL_GLYPH_OF_BLINDING_LIGHT))
                            _player->CastSpell(target, PALADIN_SPELL_BLINDING_LIGHT_STUN, true);
                        else
                            _player->CastSpell(target, PALADIN_SPELL_BLINDING_LIGHT_CONFUSE, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_blinding_light_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_blinding_light_SpellScript();
        }
};

// Hand of Protection - 1022
class spell_pal_hand_of_protection : public SpellScriptLoader
{
    public:
        spell_pal_hand_of_protection() : SpellScriptLoader("spell_pal_hand_of_protection") { }

        class spell_pal_hand_of_protection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_hand_of_protection_SpellScript);

            SpellCastResult CheckForbearance()
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetExplTargetUnit())
                    if (target->HasAura(SPELL_FORBEARANCE))
                        return SPELL_FAILED_TARGET_AURASTATE;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_FORBEARANCE, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_hand_of_protection_SpellScript::CheckForbearance);
                OnHit += SpellHitFn(spell_pal_hand_of_protection_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_hand_of_protection_SpellScript();
        }
};

// Divine Shield - 642 and Divine Shield - 110700
class spell_pal_divine_shield : public SpellScriptLoader
{
    public:
        spell_pal_divine_shield() : SpellScriptLoader("spell_pal_divine_shield") { }

        class spell_pal_divine_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_divine_shield_SpellScript);

            SpellCastResult CheckForbearance()
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(SPELL_FORBEARANCE))
                        return SPELL_FAILED_TARGET_AURASTATE;

                return SPELL_CAST_OK;
            }

            void HandleBeforeHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(GLYPH_OF_DEVINE_SHIELD))
                    {
                        uint32 aurasCount = 0;
                        uint32 school_mask = GetSpellInfo()->Effects[1].MiscValue;
                        Unit::AuraApplicationMap& Auras = _player->GetAppliedAuras();

                        for (Unit::AuraApplicationMap::iterator iter = Auras.begin(); iter != Auras.end(); ++iter)
                        {
                            SpellInfo const* spell = iter->second->GetBase()->GetSpellInfo();
                            if ((spell->GetSchoolMask() & school_mask)//Check for school mask
                                && GetSpellInfo()->CanDispelAura(spell)
                                && !iter->second->IsPositive()          //Don't remove positive spells
                                && spell->Id != GetSpellInfo()->Id)               //Don't remove self
                                aurasCount++;
                        }

                        if (aurasCount >= 5)
                            aurasCount = 5;

                        int32 heal = CalculatePct(_player->GetMaxHealth(), 10) * aurasCount;

                        heal = _player->SpellHealingBonusDone(_player, GetSpellInfo(), heal, HEAL);
                        heal = _player->SpellHealingBonusTaken(_player, GetSpellInfo(), (uint8)EFFECT_0, heal, HEAL);
                        
                        heal = std::min<int32>(heal, (int32)_player->CountPctFromMaxHealth(50));

                        _player->HealBySpell(_player, GetSpellInfo(), heal, false);
                    }
                }
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_FORBEARANCE, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_divine_shield_SpellScript::CheckForbearance);
                OnHit += SpellHitFn(spell_pal_divine_shield_SpellScript::HandleOnHit);
                BeforeHit += SpellHitFn(spell_pal_divine_shield_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_divine_shield_SpellScript();
        }
};

// Inquisition - 84963
class spell_pal_inquisition : public SpellScriptLoader
{
    public:
        spell_pal_inquisition() : SpellScriptLoader("spell_pal_inquisition") { }

        class spell_pal_inquisition_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_inquisition_SpellScript);

            SpellCastResult CheckMorePowerfulAura()
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    int32 holyPower = player->GetPower(POWER_HOLY_POWER);
                    int32 newDuration;
                    int32 oldDuration;

                    if (Aura* inquisition = player->GetAura(PALADIN_SPELL_INQUISITION, player->GetGUID()))
                    {
                        if (holyPower >= 1)
                        {
                            newDuration = (holyPower * 20) * 1000;
                            oldDuration = inquisition->GetDuration();

                            if (newDuration < oldDuration)
                                return SPELL_FAILED_AURA_BOUNCED;   
                        }
                    }
                }

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Aura* inquisition = _player->GetAura(PALADIN_SPELL_INQUISITION))
                    {
                        int32 holyPower = _player->GetPower(POWER_HOLY_POWER);

                        if (holyPower > 2)
                            holyPower = 2;

                        if (_player->HasAura(PALADIN_SPELL_DIVINE_PURPOSE))
                            holyPower = 2;

                        int32 maxDuration = inquisition->GetMaxDuration();
                        int32 newDuration = inquisition->GetDuration() + maxDuration * holyPower;
                        inquisition->SetDuration(newDuration);

                        if (newDuration > maxDuration)
                            inquisition->SetMaxDuration(newDuration);

                        if (!_player->HasAura(PALADIN_SPELL_DIVINE_PURPOSE))
                            _player->SetPower(POWER_HOLY_POWER, _player->GetPower(POWER_HOLY_POWER) - holyPower);
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_inquisition_SpellScript::CheckMorePowerfulAura);
                OnHit += SpellHitFn(spell_pal_inquisition_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_inquisition_SpellScript();
        }
};

// Execution Sentence - 114157
class spell_pal_execution_sentence : public SpellScriptLoader
{
    public:
        spell_pal_execution_sentence() : SpellScriptLoader("spell_pal_execution_sentence") { }

        class spell_pal_execution_sentence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_execution_sentence_SpellScript);

            void HandleBeforeCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        if (_player->IsValidAttackTarget(target))
                            _player->CastSpell(target, PALADIN_SPELL_EXECUTION_SENTENCE, true);
                        else if (_player->GetGUID() == target->GetGUID())
                            _player->CastSpell(_player, PALADIN_SPELL_STAY_OF_EXECUTION, true);
                        else
                            _player->CastSpell(target, PALADIN_SPELL_STAY_OF_EXECUTION, true);
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_pal_execution_sentence_SpellScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_execution_sentence_SpellScript();
        }
};

// Light's Hammer (periodic dummy for npc) - 114918
class spell_pal_lights_hammer : public SpellScriptLoader
{
    public:
        spell_pal_lights_hammer() : SpellScriptLoader("spell_pal_lights_hammer") { }

        class spell_pal_lights_hammer_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_lights_hammer_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (GetCaster())
                {
                    if (GetCaster()->GetOwner())
                    {
                        GetCaster()->CastSpell(GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), GetCaster()->GetPositionZ(), PALADIN_SPELL_ARCING_LIGHT_HEAL, true, 0, nullptr, GetCaster()->GetOwner()->GetGUID());
                        GetCaster()->CastSpell(GetCaster()->GetPositionX(), GetCaster()->GetPositionY(), GetCaster()->GetPositionZ(), PALADIN_SPELL_ARCING_LIGHT_DAMAGE, true, 0, nullptr, GetCaster()->GetOwner()->GetGUID());
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_lights_hammer_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_lights_hammer_AuraScript();
        }
};

// called by Holy Prism (damage) - 114852 or Holy Prism (heal) - 114871
// Holy Prism visual for other targets
class spell_pal_holy_prism_visual : public SpellScriptLoader
{
    public:
        spell_pal_holy_prism_visual() : SpellScriptLoader("spell_pal_holy_prism_visual") { }

        class spell_pal_holy_prism_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_visual_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->IsValidAttackTarget(target))
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2, true);
                        }
                        else
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2, true);

                            // Mastery: Illuminated Healing - 76669
                            if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_PALADIN_HOLY)
                            {
                                int32 Mastery = int32(_player->GetFloatValue(PLAYER_MASTERY) * 1.25f);
 
                                int32 basepoints0 = CalculatePct(GetHitHeal(), Mastery);
                                int32 triggered_spell_id = 86273;
 
                                if (AuraEffect* aurShield = target->GetAuraEffect(triggered_spell_id, EFFECT_0, _player->GetGUID()))
                                    basepoints0 += aurShield->GetAmount();
 
                                int32 maxHealth = _player->CountPctFromMaxHealth(33);
                                basepoints0 = std::min(basepoints0, maxHealth);
 
                                if (!target->HasAura(triggered_spell_id, _player->GetGUID()))
                                    _player->AddAura(triggered_spell_id, target);
 
                                if (AuraEffect* aurShield = target->GetAuraEffect(triggered_spell_id, EFFECT_0, _player->GetGUID()))
                                    aurShield->SetAmount(basepoints0);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_visual_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_visual_SpellScript();
        }
};

// called by Holy Prism (visual damage) - 114862 or Holy Prism (visual heal) - 121551
// Holy Prism (damage) - 114852 or Holy Prism (heal) - 114871
class spell_pal_holy_prism_effect : public SpellScriptLoader
{
    public:
        spell_pal_holy_prism_effect() : SpellScriptLoader("spell_pal_holy_prism_effect") { }

        class spell_pal_holy_prism_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_effect_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // damage
                        if (GetSpellInfo()->Id == 114862)
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_ENNEMIES, true);
                        // heal
                        else if (GetSpellInfo()->Id == 121551)
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_ALLIES, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_effect_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_effect_SpellScript();
        }
};

// Holy Prism - 114165
class spell_pal_holy_prism : public SpellScriptLoader
{
    public:
        spell_pal_holy_prism() : SpellScriptLoader("spell_pal_holy_prism") { }

        class spell_pal_holy_prism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->IsValidAttackTarget(target))
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2, true);
                        }
                        else
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_SpellScript();
        }
};

// Word of Glory - 85673
class spell_pal_word_of_glory : public SpellScriptLoader
{
    public:
        spell_pal_word_of_glory() : SpellScriptLoader("spell_pal_word_of_glory") { }

        class spell_pal_word_of_glory_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_word_of_glory_SpellScript);

            bool Validate()
            {
                if (!sSpellMgr->GetSpellInfo(PALADIN_SPELL_WORD_OF_GLORY))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (!unitTarget->IsFriendlyTo(_player))
                            unitTarget = _player;

                        int32 holyPower = _player->GetPower(POWER_HOLY_POWER);

                        if (holyPower > 2)
                            holyPower = 2;

                        _player->CastSpell(unitTarget, PALADIN_SPELL_WORD_OF_GLORY_HEAL, true);

                        if (_player->HasAura(PALADIN_SPELL_GLYPH_OF_WORD_OF_GLORY))
                        {
                            Aura* aura = _player->AddAura(PALADIN_SPELL_GLYPH_OF_WORD_OF_GLORY_DAMAGE, _player);

                            if (aura)
                            {
                                aura->GetEffect(0)->ChangeAmount(aura->GetEffect(0)->GetAmount() * (holyPower + 1));
                                aura->SetNeedClientUpdateForTargets();
                            }
                        }

                        if (!_player->HasAura(PALADIN_SPELL_DIVINE_PURPOSE))
                            _player->ModifyPower(POWER_HOLY_POWER, -holyPower);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_word_of_glory_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_word_of_glory_SpellScript();
        }
};

// Judgment - 20271
class spell_pal_judgment : public SpellScriptLoader
{
    public:
        spell_pal_judgment() : SpellScriptLoader("spell_pal_judgment") { }

        class spell_pal_judgment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_judgment_SpellScript);

            bool Validate()
            {
                if (!sSpellMgr->GetSpellInfo(PALADIN_SPELL_JUDGMENT))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (_player->HasAura(PALADIN_SPELL_JUDGMENTS_OF_THE_BOLD) || _player->HasAura(PALADIN_SPELL_JUDGMENTS_OF_THE_WISE))
                        {
                            int32 power = 1;
                            if (_player->HasAura(PALADIN_SPELL_HOLY_AVENGER))
                                power = 3;

                            _player->SetPower(POWER_HOLY_POWER, _player->GetPower(POWER_HOLY_POWER) + power);
                        }

                        if (_player->HasAura(PALADIN_SPELL_JUDGMENTS_OF_THE_BOLD))
                            _player->CastSpell(unitTarget, PALADIN_SPELL_PHYSICAL_VULNERABILITY, true);

                        if (_player->HasAura(PALADIN_SPELL_LONG_ARM_OF_THE_LAW))
                            _player->CastSpell(_player, PALADIN_SPELL_LONG_ARM_OF_THE_LAW_RUN_SPEED, true);

                        if (_player->HasAura(PALADIN_SPELL_BURDEN_OF_GUILT))
                            _player->CastSpell(unitTarget, PALADIN_SPELL_BURDEN_OF_GUILT_DECREASE_SPEED, true);

                        // If caster has Holy Insight and selfless healer, judgment generate a charge of Holy Power
                        if (_player->HasAura(PALADIN_SPELL_HOLY_INSIGHT) && _player->HasAura(PALADIN_SPELL_SELFLESS_HEALER))
                            _player->ModifyPower(POWER_HOLY_POWER, 1);

                        if (_player->HasAura(GLYPH_OF_DOUBLE_JEOPARDY))
                        {
                            if (Aura* doubleJeopardyBonus = _player->GetAura(121027, _player->GetGUID()))
                            {
                                int32 lastGuid = doubleJeopardyBonus->GetEffect(1)->GetAmount();
                                int32 newGuid = unitTarget->GetGUID();
                                int32 hitDamage = int32(GetHitDamage() * 1.2f);
                                if (lastGuid != newGuid)
                                    SetHitDamage(hitDamage);
                            }
                        }
                    }
                }
            }

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (_player->HasAura(GLYPH_OF_DOUBLE_JEOPARDY))
                        {
                            if (Aura* doubleJeopardyBonus = _player->GetAura(121027, _player->GetGUID()))
                            {
                                if (doubleJeopardyBonus->GetEffect(1)->GetAmount() == 1 || doubleJeopardyBonus->GetEffect(1)->GetAmount() == unitTarget->GetGUID())
                                    doubleJeopardyBonus->GetEffect(1)->SetAmount(unitTarget->GetGUID());
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_judgment_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_pal_judgment_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_judgment_SpellScript();
        }
};

// Ardent Defender - 31850
class spell_pal_ardent_defender : public SpellScriptLoader
{
    public:
        spell_pal_ardent_defender() : SpellScriptLoader("spell_pal_ardent_defender") { }

        class spell_pal_ardent_defender_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_ardent_defender_AuraScript);

            uint32 absorbPct, healPct;

            bool Load()
            {
                healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
                return GetUnitOwner()->GetTypeId() == TYPEID_PLAYER;
            }

            bool CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & canBeRecalculated)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
                return true;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                Unit* victim = GetTarget();
                int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
                // If damage kills us
                if (remainingHealth <= 0 && !victim->ToPlayer()->HasSpellCooldown(PALADIN_SPELL_ARDENT_DEFENDER_HEAL))
                {
                    // Cast healing spell, completely avoid damage
                    absorbAmount = dmgInfo.GetDamage();

                    int32 healAmount = int32(victim->CountPctFromMaxHealth(healPct));
                    victim->CastCustomSpell(victim, PALADIN_SPELL_ARDENT_DEFENDER_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff);
                    victim->ToPlayer()->AddSpellCooldown(PALADIN_SPELL_ARDENT_DEFENDER_HEAL, 0, 120 * IN_MILLISECONDS);
                }
                else
                    absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_ardent_defender_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_pal_ardent_defender_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_ardent_defender_AuraScript();
        }
};

class spell_pal_blessing_of_faith : public SpellScriptLoader
{
    public:
        spell_pal_blessing_of_faith() : SpellScriptLoader("spell_pal_blessing_of_faith") { }

        class spell_pal_blessing_of_faith_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_blessing_of_faith_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_BLESSING_OF_LOWER_CITY_DRUID) || !sSpellMgr->GetSpellInfo(SPELL_BLESSING_OF_LOWER_CITY_PALADIN) || !sSpellMgr->GetSpellInfo(SPELL_BLESSING_OF_LOWER_CITY_PRIEST) || !sSpellMgr->GetSpellInfo(SPELL_BLESSING_OF_LOWER_CITY_SHAMAN))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unitTarget = GetHitUnit())
                {
                    uint32 spell_id = 0;
                    switch (unitTarget->getClass())
                    {
                        case CLASS_DRUID:   spell_id = SPELL_BLESSING_OF_LOWER_CITY_DRUID; break;
                        case CLASS_PALADIN: spell_id = SPELL_BLESSING_OF_LOWER_CITY_PALADIN; break;
                        case CLASS_PRIEST:  spell_id = SPELL_BLESSING_OF_LOWER_CITY_PRIEST; break;
                        case CLASS_SHAMAN:  spell_id = SPELL_BLESSING_OF_LOWER_CITY_SHAMAN; break;
                        default: return;                    // ignore for non-healing classes
                    }
                    Unit* caster = GetCaster();
                    caster->CastSpell(caster, spell_id, true);
                }
            }

            void Register()
            {
                // add dummy effect spell handler to Blessing of Faith
                OnEffectHitTarget += SpellEffectFn(spell_pal_blessing_of_faith_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_blessing_of_faith_SpellScript();
        }
};

// Holy Shock (heal) - 25914, Holy Shock (damage) - 25912
class spell_pal_holy_shock_heal : public SpellScriptLoader
{
    public:
        spell_pal_holy_shock_heal() : SpellScriptLoader("spell_pal_holy_shock_heal") { }

        class spell_pal_holy_shock_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_shock_heal_SpellScript);

            void HandleOnHit()
            {
                if (Player* caster = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (Aura* dayBreak = caster->GetAura(PALADIN_SPELL_DAYBREAK_PROC))
                        {
                            int32 heal = dayBreak->GetStackAmount();

                            if (GetSpellInfo()->Id == 25914)
                                heal *= CalculatePct(GetHitHeal(), 75);
                            else if (GetSpellInfo()->Id == 25912)
                                heal *= CalculatePct(GetHitDamage(), 75);

                            // Handle critical hits
                            if (GetSpell()->IsCritForTarget(unitTarget))
                                heal *= 2;

                            caster->CastCustomSpell(unitTarget, PALADIN_SPELL_DAYBREAK_HEAL, NULL, &heal, NULL, true);
                            caster->RemoveAura(PALADIN_SPELL_DAYBREAK_PROC);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_shock_heal_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_shock_heal_SpellScript();
        }
};

// Holy Shock (damage) - 25912
class spell_pal_holy_shock_damage : public SpellScriptLoader
{
    public:
        spell_pal_holy_shock_damage() : SpellScriptLoader("spell_pal_holy_shock_damage") { }

        class spell_pal_holy_shock_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_shock_damage_SpellScript);

            void HandleOnHit()
            {
                if (Player* caster = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        /*int32 damage = GetHitDamage();

                        if (caster->getLevel() < 85)
                        {
                            damage = int32(GetHitDamage() * 0.15f);
                            SetHitDamage(damage);
                        }
                        else if (caster->getLevel() < 90)
                        {
                            damage = int32(GetHitDamage() * 0.61f);
                            SetHitDamage(damage);
                        }*/
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_shock_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_shock_damage_SpellScript();
        }
};

// Holy Shock - 20473
class spell_pal_holy_shock : public SpellScriptLoader
{
    public:
        spell_pal_holy_shock() : SpellScriptLoader("spell_pal_holy_shock") { }

        class spell_pal_holy_shock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_shock_SpellScript);

            bool Validate(SpellInfo const* spell)
            {
                if (!sSpellMgr->GetSpellInfo(PALADIN_SPELL_HOLY_SHOCK_R1))
                    return false;

                // can't use other spell than holy shock due to spell_ranks dependency
                if (sSpellMgr->GetFirstSpellInChain(PALADIN_SPELL_HOLY_SHOCK_R1) != sSpellMgr->GetFirstSpellInChain(spell->Id))
                    return false;

                uint8 rank = sSpellMgr->GetSpellRank(spell->Id);
                if (!sSpellMgr->GetSpellWithRank(PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE, rank, true) || !sSpellMgr->GetSpellWithRank(PALADIN_SPELL_HOLY_SHOCK_R1_HEALING, rank, true))
                    return false;

                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        caster->CastSpell(unitTarget, caster->IsFriendlyTo(unitTarget) ? PALADIN_SPELL_HOLY_SHOCK_R1_HEALING : PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE, true);

                        if (caster->HasAura(PALADIN_SPELL_GLYPH_OF_DENOUNCE))
                            if (roll_chance_i(50))
                                caster->CastSpell(caster, PALADIN_SPELL_GLYPH_OF_DENOUNCE_PROC, true);
                    }
                }
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (GetHitUnit())
                        caster->CastSpell(caster, PALADIN_SPELL_HOLY_SHOCK_ENERGIZE, true);
                }
            }

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetExplTargetUnit())
                {
                    if (!caster->IsFriendlyTo(target))
                    {
                        if (!caster->IsValidAttackTarget(target))
                            return SPELL_FAILED_BAD_TARGETS;

                        if (!caster->isInFront(target))
                            return SPELL_FAILED_UNIT_NOT_INFRONT;
                    }
                    else if (!caster->IsValidAssistTarget(target))
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else
                    return SPELL_FAILED_BAD_TARGETS;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                // add dummy effect spell handler to Holy Shock
                OnCheckCast += SpellCheckCastFn(spell_pal_holy_shock_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_pal_holy_shock_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnHit += SpellHitFn(spell_pal_holy_shock_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_shock_SpellScript();
        }
};

class spell_pal_divine_storm : public SpellScriptLoader
{
    public:
        spell_pal_divine_storm() : SpellScriptLoader("spell_pal_divine_storm") { }

        class spell_pal_divine_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_divine_storm_SpellScript);

            uint32 healPct;

            bool Validate(SpellInfo const* /* spell */)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DIVINE_STORM_DUMMY))
                    return false;
                return true;
            }

            bool Load()
            {
                healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
                return true;
            }

            void TriggerHeal()
            {
                Unit* caster = GetCaster();
                caster->CastCustomSpell(SPELL_DIVINE_STORM_DUMMY, SPELLVALUE_BASE_POINT0, (GetHitDamage() * healPct) / 100, caster, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_pal_divine_storm_SpellScript::TriggerHeal);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_divine_storm_SpellScript();
        }
};

// Lay on Hands - 633
class spell_pal_lay_on_hands : public SpellScriptLoader
{
    public:
        spell_pal_lay_on_hands() : SpellScriptLoader("spell_pal_lay_on_hands") { }

        class spell_pal_lay_on_hands_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_lay_on_hands_SpellScript);

            SpellCastResult CheckForbearance()
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetExplTargetUnit())
                    if (target->HasAura(SPELL_FORBEARANCE))
                        return SPELL_FAILED_TARGET_AURASTATE;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_FORBEARANCE, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_lay_on_hands_SpellScript::CheckForbearance);
                OnHit += SpellHitFn(spell_pal_lay_on_hands_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_lay_on_hands_SpellScript();
        }
};

class spell_pal_righteous_defense : public SpellScriptLoader
{
    public:
        spell_pal_righteous_defense() : SpellScriptLoader("spell_pal_righteous_defense") { }

        class spell_pal_righteous_defense_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_righteous_defense_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_DONT_REPORT;

                if (Unit* target = GetExplTargetUnit())
                {
                    if (!target->IsFriendlyTo(caster) || target->getAttackers().empty())
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else
                    return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_righteous_defense_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_righteous_defense_SpellScript();
        }
};

// Sword of Light - 53503
// Hammer of Wrath - 24275
class spell_pal_hammer_of_wrath : public SpellScriptLoader
{
    public:
        spell_pal_hammer_of_wrath() : SpellScriptLoader("spell_pal_hammer_of_wrath") { }

        class spell_pal_hammer_of_wrath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_hammer_of_wrath_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if ( _player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_PALADIN_RETRIBUTION)
                        {
                            int32 power = 1;
                            if (_player->HasAura(PALADIN_SPELL_HOLY_AVENGER))
                                power = 3;
                            if (_player->HasAura(53503))
                                _player->EnergizeBySpell(_player, PALADIN_SPELL_HAMMER_OF_WRATH, power, POWER_HOLY_POWER);
                        }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_hammer_of_wrath_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_hammer_of_wrath_SpellScript();
        }
};

// Exorcism	- 879
class spell_pal_exorcism : public SpellScriptLoader
{
    public:
        spell_pal_exorcism() : SpellScriptLoader("spell_pal_exorcism") { }

        class spell_pal_exorcism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_exorcism_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                    {
                        int32 power = 1;
                        if (_player->HasAura(PALADIN_SPELL_HOLY_AVENGER))
                            power = 3;
                        if (_player->HasAura(53503))
                            _player->EnergizeBySpell(_player, PALADIN_SPELL_EXORCISM, power, POWER_HOLY_POWER);
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_exorcism_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_exorcism_SpellScript();
        }
};

/// Exorcism (melee) - 122032
class spell_pal_exorcism_melee : public SpellScriptLoader
{
    public:
        spell_pal_exorcism_melee() : SpellScriptLoader("spell_pal_exorcism_melee") { }

        class spell_pal_exorcism_melee_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_exorcism_melee_SpellScript);

            void HandleHit()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (Unit* l_ExplicitTarget = GetExplTargetUnit())
                    {
                        if (l_ExplicitTarget->GetGUID() != l_Target->GetGUID())
                            SetHitDamage(CalculatePct(GetHitDamage(), 25));
                    }
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_pal_exorcism_melee_SpellScript::HandleHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_exorcism_melee_SpellScript();
        }
};

// Crusader Strike - 35395
class spell_pal_crusader_strike : public SpellScriptLoader
{
    public:
        spell_pal_crusader_strike() : SpellScriptLoader("spell_pal_crusader_strike") { }

        class spell_pal_crusader_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_crusader_strike_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetGUID() == _player->GetGUID())
                            return;

                        _player->CastSpell(target, PALADIN_SPELL_WEAKENED_BLOWS, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_crusader_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_crusader_strike_SpellScript();
        }
};

// Selfless Healer - 85804
// Selfless Healer Effect (trigger) - 114250
class spell_pal_selfless_healer_aura : public SpellScriptLoader
{
    public:
        spell_pal_selfless_healer_aura() : SpellScriptLoader("spell_pal_selfless_healer_aura") { }

        class spell_pal_selfless_healer_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_selfless_healer_aura_AuraScript);

            uint32 holyPowerConsumed;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Aura* selflessHealer = _player->GetAura(PALADIN_SPELL_SELFLESS_HEALER_STACK))
                    {
                        uint8 charges = selflessHealer->GetStackAmount();
                        if (_player && charges == 3)
                            _player->AddAura(128863, _player);
                    }
                }
            }
            
            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_pal_selfless_healer_aura_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_selfless_healer_aura_AuraScript();
        }
};

class spell_pal_glyph_of_devotian_aura : public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_devotian_aura() : SpellScriptLoader("spell_pal_glyph_of_devotian_aura") { }

        class spell_pal_glyph_of_devotian_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_glyph_of_devotian_aura_SpellScript);

            bool Load()
            {
                return true;
            }

            void CountTargets(std::list<WorldObject*>& targets)
            {
                if (GetCaster() && GetCaster()->HasAura(146955))
                {
                    targets.clear();
                    targets.push_back(GetCaster());
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotian_aura_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_glyph_of_devotian_aura_SpellScript();
        }
};

class spell_pal_glyph_of_devotian_trigger_aura : public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_devotian_trigger_aura() : SpellScriptLoader("spell_pal_glyph_of_devotian_trigger_aura") { }

        class spell_pal_glyph_of_devotian_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_glyph_of_devotian_aura_SpellScript);

            bool Load()
            {
                return true;
            }

            void CountTargets(std::list<WorldObject*>& targets)
            {
                if (GetCaster() && GetCaster()->HasAura(146955))
                {
                    targets.clear();
                    targets.push_back(GetCaster());
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotian_aura_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotian_aura_SpellScript::CountTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotian_aura_SpellScript::CountTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_glyph_of_devotian_aura_SpellScript();
        }
};

// Beacon of Light - 53563, Beacon of Light (aura) - 53651
class spell_pal_beacon_of_light : public SpellScriptLoader
{
    public:
        spell_pal_beacon_of_light() : SpellScriptLoader("spell_pal_beacon_of_light") { }

        class spell_pal_beacon_of_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_beacon_of_light_SpellScript)

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_PALADIN_HOLY)
                        {
                            std::list<Unit*> groupList;

                            _player->GetPartyMembers(groupList);

                            for (auto itr : groupList)
                            {
                                if (itr->HasAura(PALADIN_SPELL_BEACON_OF_LIGHT) && itr->GetGUID() == target->GetGUID())
                                    continue;

                                if (!_player->IsWithinLOSInMap(itr))
                                    continue;

                                if (itr->isDead())
                                    continue;

                                if (itr->HasAura(PALADIN_SPELL_BEACON_OF_LIGHT))
                                {
                                    // check if it was heal by paladin which casted this beacon of light
                                    if (itr->GetAura(PALADIN_SPELL_BEACON_OF_LIGHT, _player->GetGUID()))
                                    {
                                        int32 basepoints = GetHitHeal();
                                        float percent = 1.0;

                                        if (GetSpell()->IsCritForTarget(target))
                                            basepoints *= 2;

                                        switch (GetSpellInfo()->Id)
                                        {
                                            case 82327: // Holy Radiance
                                            case 119952:// Light's Hammer
                                            case 114871:// Holy Prism
                                            case 85222: // Light of Dawn
                                                percent = 0.15f; // 15% heal from these spells
                                                break;
                                            case 635:   // Holy Light
                                                percent = 1.0f; // 100% heal from Holy Light
                                                break;
                                            default:
                                                percent = 0.5f; // 50% heal from all other heals
                                                break;
                                        }
                                        basepoints = int32(basepoints * percent);

                                        _player->CastCustomSpell(itr, 53652, &basepoints, NULL, NULL, true);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                if (auto caster = GetCaster())
                {
                    uint8 maxSize = caster->HasAura(54940) ? 4 : 6; // Glyph of Light of Dawn

                    targets.push_back(caster);
                    if (targets.size() > maxSize)
                    {
                        targets.sort(JadeCore::HealthPctOrderPred());
                        targets.resize(maxSize);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_beacon_of_light_SpellScript::HandleOnHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_beacon_of_light_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_beacon_of_light_SpellScript();
        }
};

// Avenger's Shield - 31935
class spell_pal_avengers_shield : public SpellScriptLoader
{
    public:
        spell_pal_avengers_shield() : SpellScriptLoader("spell_pal_avengers_shield") { }

        class spell_pal_avengers_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_avengers_shield_SpellScript);

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                if (Player* _player = caster->ToPlayer())
                {
                    if (_player->GetAuraBeforeInstantCast())
                        _player->EnergizeBySpell(_player, GetSpellInfo()->Id, (caster->HasAura(PALADIN_SPELL_HOLY_AVENGER)) ? 3 : 1, POWER_HOLY_POWER);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pal_avengers_shield_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_avengers_shield_SpellScript();
        }
};

// Word of Glory (heal spell) - 130551, Eternal Flame - 114163
class spell_pal_word_of_glory_heal : public SpellScriptLoader
{
    public:
        spell_pal_word_of_glory_heal() : SpellScriptLoader("spell_pal_word_of_glory_heal") { }

        class spell_pal_word_of_glory_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_word_of_glory_heal_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        // Glyph of Protector of the Innocent- 93466
                        if (unitTarget->GetGUID() != _player->GetGUID() && _player->HasAura(GLYPH_OF_PROTECTOR_OF_THE_INNOCENT))
                        {
                            int32 healAmount = CalculatePct(GetHitHeal(), 20);

                            if (GetSpell()->IsCritForTarget(unitTarget))
                                healAmount *= 2;

                            _player->CastCustomSpell(_player, GLYPH_OF_PROTECTOR_OF_THE_INNOCENT_HEAL, &healAmount, NULL, NULL, true);
                        }
                    }
                }
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_pal_word_of_glory_heal_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_pal_word_of_glory_heal_SpellScript();
        }
};

// Word of Glory (damage spell) - 130552
class spell_pal_word_of_glory_damage : public SpellScriptLoader
{
    public:
        spell_pal_word_of_glory_damage() : SpellScriptLoader("spell_pal_word_of_glory_damage") { }

        class spell_pal_word_of_glory_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_word_of_glory_damage_SpellScript);

            int32 holyPower;
            void HandleBeforeHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    holyPower = _player->GetPower(POWER_HOLY_POWER);
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        int32 hitDamage = GetHitDamage();
                        hitDamage = int32(hitDamage * holyPower);
                        SetHitDamage(hitDamage);

                        _player->ModifyPower(POWER_HOLY_POWER, -holyPower);
                    }
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_pal_word_of_glory_damage_SpellScript::HandleBeforeHit);
                OnHit += SpellHitFn(spell_pal_word_of_glory_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_word_of_glory_damage_SpellScript();
        }
};

// Glyph of Contemplation - 125043
class spell_pal_glyph_of_contemplation : public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_contemplation() : SpellScriptLoader("spell_pal_glyph_of_contemplation") { }

        class spell_pal_glyph_of_contemplation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_glyph_of_contemplation_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(PALADIN_SPELL_CONTEMPLATION, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(PALADIN_SPELL_CONTEMPLATION))
                        _player->removeSpell(PALADIN_SPELL_CONTEMPLATION, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_pal_glyph_of_contemplation_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_pal_glyph_of_contemplation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_glyph_of_contemplation_AuraScript();
        }
};

class spell_pal_seal_of_insight_heal : public SpellScriptLoader
{
public:
    spell_pal_seal_of_insight_heal() : SpellScriptLoader("spell_pal_seal_of_insight_heal") { }

    class spell_pal_seal_of_insight_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_seal_of_insight_heal_SpellScript);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            if (targets.empty() || !GetCaster())
                return;

            if (GetCaster()->HasAura(119477))
            {
                std::list<Unit*> unitList;

                for (auto itr : targets)
                    if (itr->ToPlayer())
                        unitList.push_back(itr->ToUnit());

                targets.clear();


                unitList.sort(JadeCore::HealthPctOrderPred());
                unitList.resize(1);

                for (auto itr : unitList)
                    targets.push_back(itr);
            }
            else
            {
                targets.clear();
                targets.push_back(GetCaster());
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_seal_of_insight_heal_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_seal_of_insight_heal_SpellScript();
    }
};

// Daybreak Heal - 121129
class spell_pal_daybreak_heal : public SpellScriptLoader
{
    public:
        spell_pal_daybreak_heal() : SpellScriptLoader("spell_pal_daybreak_heal") { }

        class spell_pal_daybreak_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_daybreak_heal_SpellScript);

            uint32 m_TargetCount;

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                p_Targets.remove(GetExplTargetUnit());
                m_TargetCount = p_Targets.size();
            }

            void HandleOnHit()
            {
                SetHitHeal(GetHitHeal() / m_TargetCount);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_pal_daybreak_heal_SpellScript::HandleOnHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_daybreak_heal_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_pal_daybreak_heal_SpellScript();
        }
};

void AddSC_paladin_spell_scripts()
{
    new spell_pal_seal_of_insight_heal();
    new spell_pal_glyph_of_devotian_aura();
    new spell_pal_glyph_of_devotian_trigger_aura();
    new spell_pal_sanctified_wrath();
    new spell_pal_glyph_of_divine_storm();
    new spell_pal_holy_radiance();
    new spell_pal_hand_of_purity();
    new spell_pal_glyph_of_avenging_wrath();
    new spell_pal_unbreakable_spirit();
    new spell_pal_shield_of_the_righteous();
    new spell_pal_selfless_healer();
    new spell_pal_tower_of_radiance();
    new spell_pal_sacred_shield();
    new spell_pal_sacred_shield_absorb();
    new spell_pal_sacred_shield_absorb_holy();
    new spell_pal_emancipate();
    new spell_pal_art_of_war();
    new spell_pal_seal_of_insight();
    new spell_pal_blinding_light();
    new spell_pal_hand_of_protection();
    new spell_pal_divine_shield();
    new spell_pal_inquisition();
    new spell_pal_execution_sentence();
    new spell_pal_lights_hammer();
    new spell_pal_holy_prism_visual();
    new spell_pal_holy_prism_effect();
    new spell_pal_holy_prism();
    new spell_pal_word_of_glory();
    new spell_pal_judgment();
    new spell_pal_ardent_defender();
    new spell_pal_blessing_of_faith();
    new spell_pal_holy_shock_heal();
    new spell_pal_holy_shock_damage();
    new spell_pal_holy_shock();
    new spell_pal_divine_storm();
    new spell_pal_lay_on_hands();
    new spell_pal_righteous_defense();
    new spell_pal_hammer_of_wrath();
    new spell_pal_exorcism();
    new spell_pal_exorcism_melee();
    new spell_pal_crusader_strike();
    new spell_pal_selfless_healer_aura();
    new spell_pal_selfless_healer_spell();
    new spell_pal_beacon_of_light();
    new spell_pal_avengers_shield();
    new spell_pal_word_of_glory_heal();
    new spell_pal_word_of_glory_damage();
    new spell_pal_glyph_of_contemplation();
    new spell_pal_daybreak_heal();
}

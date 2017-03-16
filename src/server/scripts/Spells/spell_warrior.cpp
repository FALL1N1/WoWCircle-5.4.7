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
 * Scripts for spells with SPELLFAMILY_WARRIOR and SPELLFAMILY_GENERIC spells used by warrior players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warr_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum WarriorSpells
{
    WARRIOR_SPELL_LAST_STAND_TRIGGERED          = 12976,
    WARRIOR_SPELL_VICTORY_RUSH_DAMAGE           = 34428,
    WARRIOR_SPELL_VICTORY_RUSH_HEAL             = 118779,
    WARRIOR_SPELL_VICTORIOUS_STATE              = 32216,
    WARRIOR_SPELL_BLOODTHIRST                   = 23881,
    WARRIOR_SPELL_BLOODTHIRST_HEAL              = 117313,
    WARRIOR_SPELL_DEEP_WOUNDS                   = 115767,
    WARRIOR_SPELL_THUNDER_CLAP                  = 6343,
    WARRIOR_SPELL_WEAKENED_BLOWS                = 115798,
    WARRIOR_SPELL_BLOOD_AND_THUNDER             = 84615,
    WARRIOR_SPELL_SHOCKWAVE_STUN                = 132168,
    WARRIOR_SPELL_HEROIC_LEAP_DAMAGE            = 52174,
    WARRIOR_SPELL_RALLYING_CRY		      = 97463,
    WARRIOR_SPELL_GLYPH_OF_MORTAL_STRIKE        = 58368,
    WARRIOR_SPELL_SWORD_AND_BOARD               = 50227,
    WARRIOR_SPELL_SHIELD_SLAM                   = 23922,
    WARRIOR_SPELL_ALLOW_RAGING_BLOW             = 131116,
    WARRIOR_SPELL_MOCKING_BANNER_TAUNT          = 114198,
    WARRIOR_NPC_MOCKING_BANNER                  = 59390,
    WARRIOR_SPELL_BERZERKER_RAGE_EFFECT         = 23691,
    WARRIOR_SPELL_ENRAGE                        = 12880,
    WARRIOR_SPELL_COLOSSUS_SMASH                = 86346,
    WARRIOR_SPELL_MORTAL_STRIKE_AURA            = 12294,
    WARRIOR_SPELL_TASTE_FOR_BLOOD               = 56636, // it was 56638
    WARRIOR_SPELL_ALLOW_OVERPOWER               = 60503, // it was 119962
    WARRIOR_SPELL_TASTE_FOR_BLOOD_DAMAGE_DONE   = 125831,
    WARRIOR_SPELL_SECOND_WIND_REGEN             = 16491,
    WARRIOR_SPELL_DRAGON_ROAR_KNOCK_BACK        = 118895,
    WARRIOR_SPELL_MEAT_CLEAVER_PROC             = 85739,
    WARRIOR_SPELL_PHYSICAL_VULNERABILITY        = 81326,
    WARRIOR_SPELL_STORM_BOLT_STUN               = 132169,
    WARRIOR_SPELL_SHIELD_BLOCK_TRIGGERED        = 132404,
    WARRIOR_SPELL_GLYPH_OF_HINDERING_STRIKES    = 58366,
    WARRIOR_SPELL_SLUGGISH                      = 129923,
    WARRIOR_SPELL_IMPENDING_VICTORY             = 103840,
    WARRIOR_SPELL_ITEM_PVP_SET_4P_BONUS         = 133277,
    WARRIOR_SPELL_HEROIC_LEAP_SPEED             = 133278,
    WARRIOR_SPELL_GLYPH_OF_BULL_RUSH            = 94372,
    WARRIOR_SPELL_DIE_BY_THE_SWORD              = 118038,
    WARRIOR_SPELL_GLYPH_OF_DIE_BY_THE_SWORD     = 58386,
    WARRIOR_SPELL_BLOODSURGE                    = 46915,
    WARRIOR_SPELL_BLOODSURGE_EFFECT             = 46916,
    WARRIOR_SPELL_ULTIMATUM                     = 122509,
    WARRIOR_SPELL_ULTIMATUM_EFFECT              = 122510,
    WARRIOR_SPELL_SUDDEN_EXECUTE                = 139958,
    WARRIOR_SPELL_SUDDEN_DEATH                  = 29725,
    WARRIOR_SPELL_SHIELD_OF_WALL_NOSHIELD       = 146128,
    WARRIOR_SPELL_SHIELD_OF_WALL_HORDE          = 146127,
    WARRIOR_SPELL_SHIELD_OF_WALL_ALLIANCE       = 147925,
    WARRIOR_SPELL_SPELL_REFLECTION_NOSHIELD     = 146120,
    WARRIOR_SPELL_SPELL_REFLECTION_HORDE        = 146122,
    WARRIOR_SPELL_SPELL_REFLECTION_ALLIANCE     = 147923,
    WARRIOR_SPELL_INTIMIDATING_SHOUT_ON_PLACE   = 65931,
    WARRIOR_SPELL_RIPOSTE                       = 145674,
    WARRIOR_SPELL_SUNDER_ARMOR_EFFECT           = 113746,
    WARRIOR_GLYPH_OF_COLOSSUS_SMASH             = 89003,
    WARRIOR_GLYPH_OF_GAG_ORDER                  = 58357,
    WARRIOR_SPELL_GAG_ORDER_SILENCE             = 18498,
    WARRIOR_SPELL_INTERVENE_TRIGGERED           = 34784,
    WARRIOR_SPELL_T16_DPS_4P_BONUS              = 144441,
    WARRIOR_SPELL_T16_DPS_4P_BONUS_PROC         = 144442,
    WARRIOR_GLYPH_OF_BLAZING_TRAIL              = 123779,
    WARRIOR_SPELL_BLAZING_TRAIL                 = 123780,
    WARRIOR_COLOSSUS_SMASH_AURA                 = 108126,
    WARRIOR_SPELL_GLYPH_OF_MYSTIC_SHOUT         = 58095,
    WARRIOR_SPELL_MYSTIC_SHOUT_HOVER            = 121186,
    WARRIOR_SPELL_GLYPH_OF_WEAPONMASTER         = 146974,
    WARRIOR_SPELL_WEAPONMASTER                  = 147367,
    WARRIOR_SPELL_GLYPH_OF_BLOODCURDLING_SHOUT  = 58096,
    WARRIOR_SPELL_BLOODCURDLING_SHOUT           = 23690,
    WARRIOR_SPELL_T15_DPS_4P                    = 138126,
    WARRIOR_SPELL_T15_DPS_4P_EFFECT             = 138127
};

// Victorious State - 32216
class spell_warr_victorious_state : public SpellScriptLoader
{
    public:
        spell_warr_victorious_state() : SpellScriptLoader("spell_warr_victorious_state") { }

        class spell_warr_victorious_state_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_victorious_state_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasSpellCooldown(WARRIOR_SPELL_IMPENDING_VICTORY))
                        _player->RemoveSpellCooldown(WARRIOR_SPELL_IMPENDING_VICTORY, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_victorious_state_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_victorious_state_SpellScript();
        }
};

// Called by Heroic Strike - 78 and Cleave - 845
// Glyph of Hindering Strikes - 58366
class spell_warr_glyph_of_hindering_strikes : public SpellScriptLoader
{
    public:
        spell_warr_glyph_of_hindering_strikes() : SpellScriptLoader("spell_warr_glyph_of_hindering_strikes") { }

        class spell_warr_glyph_of_hindering_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_of_hindering_strikes_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(WARRIOR_SPELL_GLYPH_OF_HINDERING_STRIKES))
                            _player->CastSpell(target, WARRIOR_SPELL_SLUGGISH, true);
                        if (_player->HasAura(WARRIOR_SPELL_ULTIMATUM))
                            _player->RemoveAura(WARRIOR_SPELL_ULTIMATUM_EFFECT);
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_of_hindering_strikes_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_of_hindering_strikes_SpellScript();
        }
};

// Stampeding Shout - 122294
class spell_warr_stampeding_shout : public SpellScriptLoader
{
    public:
        spell_warr_stampeding_shout() : SpellScriptLoader("spell_warr_stampeding_shout") { }

        class spell_warr_stampeding_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_stampeding_shout_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        target->RemoveMovementImpairingAuras();
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_stampeding_shout_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_stampeding_shout_SpellScript();
        }
};

// Shield Barrier - 112048
class spell_warr_shield_barrier : public SpellScriptLoader
{
    public:
        spell_warr_shield_barrier() : SpellScriptLoader("spell_warr_shield_barrier") { }

        class spell_warr_shield_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_shield_barrier_AuraScript);

            bool CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (GetCaster())
                {
                    int32 usedrage = int32(GetCaster()->GetPower(POWER_RAGE) / 10);
                    if (usedrage > 40)
                        usedrage = 40;

                    int totalrage = usedrage + 20; // + cost

                    int32 AP = int32(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));
                    int32 Strength = int32(GetCaster()->GetStat(STAT_STRENGTH));
                    int32 Stamina = int32(GetCaster()->GetStat(STAT_STAMINA));

                    int32 add = std::max(int32(2 * (AP - 2 * (Strength - 10))), int32(Stamina * 2.5f));
                    add = (add * totalrage) / 60;

                    amount += add;

                    GetCaster()->SetPower(POWER_RAGE, GetCaster()->GetPower(POWER_RAGE) - usedrage * 10);
                    return true;
                }

                return false;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_shield_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_shield_barrier_AuraScript();
        }
};

// Shield Block - 2565
class spell_warr_shield_block : public SpellScriptLoader
{
    public:
        spell_warr_shield_block() : SpellScriptLoader("spell_warr_shield_block") { }

        class spell_warr_shield_block_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shield_block_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(WARRIOR_SPELL_SHIELD_BLOCK_TRIGGERED))
                    {
                        if (Aura* shieldBlock = _player->GetAura(WARRIOR_SPELL_SHIELD_BLOCK_TRIGGERED))
                        {
                            int32 currentDuration = shieldBlock->GetDuration();
                            int32 maxDuration = shieldBlock->GetMaxDuration();
                            shieldBlock->SetDuration(maxDuration + currentDuration);
                        }
                    }
                    else
                        _player->CastSpell(_player, WARRIOR_SPELL_SHIELD_BLOCK_TRIGGERED, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_shield_block_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shield_block_SpellScript();
        }
};

// Storm Bolt - 107570 and Storm Bolt (off hand) - 145585
class spell_warr_storm_bolt : public SpellScriptLoader
{
    public:
        spell_warr_storm_bolt() : SpellScriptLoader("spell_warr_storm_bolt") { }

        class spell_warr_storm_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_storm_bolt_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (unitTarget->GetTypeId() == TYPEID_UNIT && unitTarget->IsImmunedToSpellEffect(sSpellMgr->GetSpellInfo(WARRIOR_SPELL_STORM_BOLT_STUN), 0, true))
                            SetHitDamage(GetHitDamage() * 4);

                        if (GetSpellInfo()->Id == 107570 && !unitTarget->HasAura(19263)) // Deterrence
                            _player->CastSpell(unitTarget, WARRIOR_SPELL_STORM_BOLT_STUN, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_storm_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_storm_bolt_SpellScript();
        }
};

// Colossus Smash - 86346
class spell_warr_colossus_smash : public SpellScriptLoader
{
    public:
        spell_warr_colossus_smash() : SpellScriptLoader("spell_warr_colossus_smash") { }

        class spell_warr_colossus_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_colossus_smash_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->HasAura(86346))
                            target->RemoveAura(86346);

                        for (auto const& info : GetSpell()->GetTargetInfos())
                            if (info.missCondition != SPELL_MISS_NONE)
                                return;

                        _player->AddAura(WARRIOR_COLOSSUS_SMASH_AURA, target);
                        _player->CastSpell(target, WARRIOR_SPELL_PHYSICAL_VULNERABILITY, true);

                        // Remove Sudden Death after hit, to prevent a bug when colossus smash can't be used after proc.
                        if (_player->HasAura(52437))
                            _player->RemoveAura(52437);

                        // Fix Glyph of Colossus Smash - 89003
                        if (_player->HasAura(WARRIOR_GLYPH_OF_COLOSSUS_SMASH))
                            _player->CastSpell(target, WARRIOR_SPELL_SUNDER_ARMOR_EFFECT, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_colossus_smash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_colossus_smash_SpellScript();
        }
};

// Called by Raging Blow - 85288
// Meat Cleaver - 85739
class spell_warr_meat_cleaver : public SpellScriptLoader
{
    public:
        spell_warr_meat_cleaver() : SpellScriptLoader("spell_warr_meat_cleaver") { }

        class spell_warr_meat_cleaver_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_meat_cleaver_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(WARRIOR_SPELL_MEAT_CLEAVER_PROC))
                        _player->RemoveAura(WARRIOR_SPELL_MEAT_CLEAVER_PROC);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_warr_meat_cleaver_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_meat_cleaver_SpellScript();
        }
};

// Dragon Roar - 118000
class spell_warr_dragon_roar : public SpellScriptLoader
{
    public:
        spell_warr_dragon_roar() : SpellScriptLoader("spell_warr_dragon_roar") { }

        class spell_warr_dragon_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_dragon_roar_SpellScript);

            uint32 m_TargetCount;

            void CountTargets(std::list<WorldObject*>& targets)
            {
                m_TargetCount = targets.size();
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        switch (m_TargetCount)
                        {
                            case 1:
                                break;
                            case 2:
                                SetHitDamage(CalculatePct(GetHitDamage(), 75));
                                break;
                            case 3:
                                SetHitDamage(CalculatePct(GetHitDamage(), 65));
                                break;
                            case 4:
                                SetHitDamage(CalculatePct(GetHitDamage(), 55));
                                break;
                            default:
                                SetHitDamage(CalculatePct(GetHitDamage(), 50));
                                break;
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_dragon_roar_SpellScript::HandleOnHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_dragon_roar_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_dragon_roar_SpellScript();
        }
};

// Staggering Shout - 107566
class spell_warr_staggering_shout : public SpellScriptLoader
{
    public:
        spell_warr_staggering_shout() : SpellScriptLoader("spell_warr_staggering_shout") { }

        class spell_warr_staggering_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_staggering_shout_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraTypeCheck(false, SPELL_AURA_MOD_DECREASE_SPEED));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_staggering_shout_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_staggering_shout_SpellScript();
        }
};

// Frenzied Regeneration - 55694
class spell_warr_frenzied_regeneration : public SpellScriptLoader
{
    public:
        spell_warr_frenzied_regeneration() : SpellScriptLoader("spell_warr_frenzied_regeneration") { }

        class spell_warr_frenzied_regeneration_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_frenzied_regeneration_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAuraState(AURA_STATE_ENRAGE))
                        _player->EnergizeBySpell(_player, GetSpellInfo()->Id, 600, POWER_RAGE);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_frenzied_regeneration_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_frenzied_regeneration_SpellScript();
        }
};

// Second Wind - 29838
class spell_warr_second_wind : public SpellScriptLoader
{
    public:
        spell_warr_second_wind() : SpellScriptLoader("spell_warr_second_wind") { }

        class spell_warr_second_wind_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_second_wind_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, WARRIOR_SPELL_SECOND_WIND_REGEN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_second_wind_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_second_wind_SpellScript();
        }

        class spell_warr_second_wind_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_second_wind_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(WARRIOR_SPELL_SECOND_WIND_REGEN))
                        caster->RemoveAura(WARRIOR_SPELL_SECOND_WIND_REGEN);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_warr_second_wind_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_second_wind_AuraScript();
        }
};

// Called by Heroic Strike - 78 and Cleave - 845
// Taste for Blood (damage done) - 125831
class spell_warr_taste_for_blood_aura : public SpellScriptLoader
{
    public:
        spell_warr_taste_for_blood_aura() : SpellScriptLoader("spell_warr_taste_for_blood_aura") { }

        class spell_warr_taste_for_blood_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_taste_for_blood_aura_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Hotfix (2012-12-11): "Taste for Blood now only stacks 1 time versus other players (was 3 times)."
                        // Patch 5.1.0 (2012-11-27): Will now stack up to 3 times in PvP. It will continue to stack to 5 in other situations.

                        int32 stacks = 0;

                        if (Aura* tasteForBlood = _player->GetAura(WARRIOR_SPELL_TASTE_FOR_BLOOD_DAMAGE_DONE))
                            stacks = tasteForBlood->GetStackAmount();

                        stacks++;

                        if (_player->HasAura(WARRIOR_SPELL_TASTE_FOR_BLOOD_DAMAGE_DONE))
                            _player->RemoveAura(WARRIOR_SPELL_TASTE_FOR_BLOOD_DAMAGE_DONE);

                        if (target->GetTypeId() == TYPEID_PLAYER)
                            if (stacks > 2)
                                SetHitDamage(int32(GetHitDamage() / stacks) * 2);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_taste_for_blood_aura_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_taste_for_blood_aura_SpellScript();
        }
};

// Called by Overpower - 7384
// Taste for Blood - 56638
class spell_warr_taste_for_blood : public SpellScriptLoader
{
    public:
        spell_warr_taste_for_blood() : SpellScriptLoader("spell_warr_taste_for_blood") { }

        class spell_warr_taste_for_blood_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_taste_for_blood_SpellScript);

            bool rolled;

            void HandleOnHit()
            {
                rolled = false;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(WARRIOR_SPELL_TASTE_FOR_BLOOD))
                        {
                            if (Aura* overpower = _player->GetAura(WARRIOR_SPELL_ALLOW_OVERPOWER))
                            {
                                int32 stacks = overpower->GetStackAmount();
                                if (stacks <= 1)
                                    _player->RemoveAura(WARRIOR_SPELL_ALLOW_OVERPOWER);
                                else
                                    overpower->SetStackAmount(stacks - 1);
                            }
                            if (_player->HasAura(WARRIOR_SPELL_GLYPH_OF_DIE_BY_THE_SWORD) && (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_WARRIOR_FURY || _player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_WARRIOR_ARMS))
                                if (Aura* dieByTheSword = _player->GetAura(WARRIOR_SPELL_DIE_BY_THE_SWORD))
                                {
                                    dieByTheSword->SetDuration(dieByTheSword->GetDuration() + 1000);
                                    dieByTheSword->SetNeedClientUpdateForTargets();
                                }
                            if (_player->HasSpellCooldown(WARRIOR_SPELL_MORTAL_STRIKE_AURA))
                                _player->ReduceSpellCooldown(WARRIOR_SPELL_MORTAL_STRIKE_AURA, 500);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_taste_for_blood_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_taste_for_blood_SpellScript();
        }
};

// Sudden Death - 52437
class spell_warr_sudden_death : public SpellScriptLoader
{
    public:
        spell_warr_sudden_death() : SpellScriptLoader("spell_warr_sudden_death") { }

        class spell_warr_sudden_death_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_sudden_death_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    _player->ToPlayer()->ReduceSpellCooldown(WARRIOR_SPELL_COLOSSUS_SMASH, 4000);
                    _player->RemoveSpellCooldown(WARRIOR_SPELL_COLOSSUS_SMASH, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_sudden_death_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_sudden_death_SpellScript();
        }
};

// Berzerker Rage - 18499
class spell_warr_berzerker_rage : public SpellScriptLoader
{
    public:
        spell_warr_berzerker_rage() : SpellScriptLoader("spell_warr_berzerker_rage") { }

        class spell_warr_berzerker_rage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_berzerker_rage_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    _player->CastSpell(_player, WARRIOR_SPELL_ENRAGE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_berzerker_rage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_berzerker_rage_SpellScript();
        }
};

// Mocking Banner - 114192
class spell_warr_mocking_banner : public SpellScriptLoader
{
    public:
        spell_warr_mocking_banner() : SpellScriptLoader("spell_warr_mocking_banner") { }

        class spell_warr_mocking_banner_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_mocking_banner_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (Player* player = GetTarget()->ToPlayer())
                {
                    std::list<Creature*> bannerList;
                    std::list<Creature*> tempList;

                    GetTarget()->GetCreatureListWithEntryInGrid(tempList, WARRIOR_NPC_MOCKING_BANNER, 30.0f);

                    bannerList = tempList;

                    // Remove other players banners
                    for (auto itr : tempList)
                    {
                        Unit* owner = itr->GetOwner();
                        if (owner && owner == player && itr->isSummon())
                            continue;

                        bannerList.remove(itr);
                    }

                    for (auto itr : bannerList)
                        player->CastSpell(itr, WARRIOR_SPELL_MOCKING_BANNER_TAUNT, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_mocking_banner_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_mocking_banner_AuraScript();
        }
};

// Called by the proc of Enrage - 12880
// Raging Blow (allow to use it) - 131116
class spell_warr_raging_blow_proc : public SpellScriptLoader
{
    public:
        spell_warr_raging_blow_proc() : SpellScriptLoader("spell_warr_raging_blow_proc") { }

        class spell_warr_raging_blow_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_raging_blow_proc_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_WARRIOR_FURY && _player->getLevel() >= 30)
                        _player->AddAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW, _player);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_raging_blow_proc_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_raging_blow_proc_SpellScript();
        }
};

// Raging Blow - 85288
class spell_warr_raging_blow : public SpellScriptLoader
{
    public:
        spell_warr_raging_blow() : SpellScriptLoader("spell_warr_raging_blow") { }

        class spell_warr_raging_blow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_raging_blow_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW))
                    {
                        if (Aura* ragingBlow = _player->GetAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW))
                        {
                            int32 stacks = ragingBlow->GetStackAmount();
                            if (stacks <= 1)
                                _player->RemoveAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW);
                            else
                                ragingBlow->SetStackAmount(stacks - 1);
                         }
                    }

                    if (Unit* target = GetHitUnit())
                    {
                        if (!_player->HasSpellCooldown(12950))
                        {
                            _player->CastSpell(target, 96103, false);
                            _player->CastSpell(target, 85384, false);
                            _player->AddSpellCooldown(12950, 0, 1 * IN_MILLISECONDS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_raging_blow_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_raging_blow_SpellScript();
        }
};

// Called by Devastate - 20243
// Sword and Board - 46953
class spell_warr_sword_and_board : public SpellScriptLoader
{
    public:
        spell_warr_sword_and_board() : SpellScriptLoader("spell_warr_sword_and_board") { }

        class spell_warr_sword_and_board_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_sword_and_board_SpellScript);

            void HandleOnHit()
            {
                // Fix Sword and Board
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (roll_chance_i(30))
                        {
                            _player->CastSpell(_player, WARRIOR_SPELL_SWORD_AND_BOARD, true);
                            _player->RemoveSpellCooldown(WARRIOR_SPELL_SHIELD_SLAM, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_sword_and_board_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_sword_and_board_SpellScript();
        }
};

// Mortal strike - 12294
class spell_warr_mortal_strike : public SpellScriptLoader
{
    public:
        spell_warr_mortal_strike() : SpellScriptLoader("spell_warr_mortal_strike") { }

        class spell_warr_mortal_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_mortal_strike_SpellScript);

            void HandleOnHit()
            {
                // Fix Apply Mortal strike buff on player only if he has the correct glyph
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(WARRIOR_SPELL_MORTAL_STRIKE_AURA))
                            if (!_player->HasAura(WARRIOR_SPELL_GLYPH_OF_MORTAL_STRIKE))
                                _player->RemoveAura(WARRIOR_SPELL_MORTAL_STRIKE_AURA);

                        if (_player->HasAura(WARRIOR_SPELL_TASTE_FOR_BLOOD) && _player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_WARRIOR_ARMS)
                        {
                            _player->AddComboPoints(target, 1);
                            _player->StartReactiveTimer(REACTIVE_OVERPOWER);
                            _player->CastSpell(_player,WARRIOR_SPELL_ALLOW_OVERPOWER, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_mortal_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_mortal_strike_SpellScript();
        }
};

// Rallying cry - 97462
class spell_warr_rallying_cry : public SpellScriptLoader
{
    public:
        spell_warr_rallying_cry() : SpellScriptLoader("spell_warr_rallying_cry") { }

        class spell_warr_rallying_cry_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_rallying_cry_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    std::list<Unit*> memberList;
                    _player->GetPartyMembers(memberList);

                    for (auto itr : memberList)
                        if (itr == _player || itr->IsWithinDistInMap(_player, 30.0f))
                        {
                            int32 MaxHealth = CalculatePct(itr->GetMaxHealth(), 20);
                            _player->CastCustomSpell(itr, WARRIOR_SPELL_RALLYING_CRY, &MaxHealth, nullptr, nullptr, true);
                        }
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_warr_rallying_cry_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_rallying_cry_SpellScript();
        }
};

// Heroic leap - 6544
class spell_warr_heroic_leap : public SpellScriptLoader
{
    public:
        spell_warr_heroic_leap() : SpellScriptLoader("spell_warr_heroic_leap") { }

        class spell_warr_heroic_leap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_leap_SpellScript);

            std::list<Unit*> targetList;

            SpellCastResult CheckElevation()
            {
                Unit* caster = GetCaster();
                if (!caster || !caster->ToPlayer())
                    return SPELL_FAILED_DONT_REPORT;

                Player* player = caster->ToPlayer();

                WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
                if (!dest)
                    return SPELL_FAILED_DONT_REPORT;

                if (dest->GetPositionZ() > player->GetPositionZ() + 3.0f)
                    return SPELL_FAILED_NOPATH;
                else if (player->HasAuraType(SPELL_AURA_MOD_ROOT))
                    return SPELL_FAILED_ROOTED;
                else if (player->GetMap()->IsBattlegroundOrArena())
                {
                    if (Battleground* bg = player->GetBattleground())
                    {
                        if (bg->GetStatus() != STATUS_IN_PROGRESS)
                            return SPELL_FAILED_NOT_READY;
                    }
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warr_heroic_leap_SpellScript::CheckElevation);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_leap_SpellScript();
        }

        class spell_warr_heroic_leap_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_heroic_leap_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, WARRIOR_SPELL_HEROIC_LEAP_DAMAGE, true);

                    // Item - Warrior PvP Set 4P Bonus
                    if (caster->HasAura(WARRIOR_SPELL_ITEM_PVP_SET_4P_BONUS))
                        caster->CastSpell(caster, WARRIOR_SPELL_HEROIC_LEAP_SPEED, true);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_warr_heroic_leap_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_heroic_leap_AuraScript();
        }
};

// Heroic Leap (damage) - 52174
class spell_warr_heroic_leap_damage : public SpellScriptLoader
{
    public:
        spell_warr_heroic_leap_damage() : SpellScriptLoader("spell_warr_heroic_leap_damage") { }

        class spell_warr_heroic_leap_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_leap_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    SetHitDamage(int32(caster->GetTotalAttackPowerValue(BASE_ATTACK) * 0.5f));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_heroic_leap_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_leap_damage_SpellScript();
        }
};

// Shockwave - 46968
class spell_warr_shockwave : public SpellScriptLoader
{
    public:
        spell_warr_shockwave() : SpellScriptLoader("spell_warr_shockwave") { }

        class spell_warr_shockwave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shockwave_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetGUID() == _player->GetGUID())
                            return;

                        _player->CastSpell(target, WARRIOR_SPELL_SHOCKWAVE_STUN, true);
                    }
                }
            }

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    // if we must - reduce shockwave cooldown
                    if (_player->GetResetCooldownFromShockWave())
                    {
                        _player->ToPlayer()->ReduceSpellCooldown(46968, 20000);
                        _player->NeedResetCooldownFromShockWave(false);
                    }
                }

            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_shockwave_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_warr_shockwave_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shockwave_SpellScript();
        }
};

// Bloodthirst - 23881
class spell_warr_bloodthirst : public SpellScriptLoader
{
    public:
        spell_warr_bloodthirst() : SpellScriptLoader("spell_warr_bloodthirst") { }

        class spell_warr_bloodthirst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_bloodthirst_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_BLOODTHIRST))
                    return false;
                return true;
            }
            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (GetHitDamage())
                        {
                            _player->CastSpell(_player, WARRIOR_SPELL_BLOODTHIRST_HEAL, true);
                            if (_player->HasAura(WARRIOR_SPELL_BLOODSURGE))
                                if(roll_chance_i(20))
                                    _player->CastSpell(_player,WARRIOR_SPELL_BLOODSURGE_EFFECT, true);
                        }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_bloodthirst_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_bloodthirst_SpellScript();
        }
};

// Victory Rush - 34428
class spell_warr_victory_rush : public SpellScriptLoader
{
    public:
        spell_warr_victory_rush() : SpellScriptLoader("spell_warr_victory_rush") { }

        class spell_warr_victory_rush_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_victory_rush_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_VICTORY_RUSH_DAMAGE))
                    return false;
                return true;
            }
            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(_player, WARRIOR_SPELL_VICTORY_RUSH_HEAL, true);
                        if (_player->HasAura(WARRIOR_SPELL_VICTORIOUS_STATE))
                            _player->RemoveAura(WARRIOR_SPELL_VICTORIOUS_STATE);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_victory_rush_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_victory_rush_SpellScript();
        }
};

class spell_warr_last_stand : public SpellScriptLoader
{
    public:
        spell_warr_last_stand() : SpellScriptLoader("spell_warr_last_stand") { }

        class spell_warr_last_stand_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_last_stand_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_LAST_STAND_TRIGGERED))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(30));
                    caster->CastCustomSpell(caster, WARRIOR_SPELL_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
                }
            }

            void Register()
            {
                // add dummy effect spell handler to Last Stand
                OnEffectHit += SpellEffectFn(spell_warr_last_stand_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_last_stand_SpellScript();
        }
};

// Thunder Clap - 6343
class spell_warr_thunder_clap : public SpellScriptLoader
{
    public:
        spell_warr_thunder_clap() : SpellScriptLoader("spell_warr_thunder_clap") { }

        class spell_warr_thunder_clap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_thunder_clap_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, WARRIOR_SPELL_WEAKENED_BLOWS, true);
                        // Glyph of Resonating Power - 58356, increases damage the damage and cooldown by 50%
                        if (_player->HasAura(58356))
                            _player->ToPlayer()->AddSpellCooldown(6343, 0, 12 * IN_MILLISECONDS);
                    }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_thunder_clap_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_thunder_clap_SpellScript();
        }
};

// Called By Thunder Clap - 6343, Mortal Strike - 12294, Bloodthirst - 23881 and Devastate - 20243
// Deep Wounds - 115767
class spell_warr_deep_wounds : public SpellScriptLoader
{
    public:
        spell_warr_deep_wounds() : SpellScriptLoader("spell_warr_deep_wounds") { }

        class spell_warr_deep_wounds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_deep_wounds_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetGUID() == _player->GetGUID())
                            return;

                        if (_player->getLevel() >= 32)
                        {
                            if (GetSpellInfo()->Id != WARRIOR_SPELL_THUNDER_CLAP || _player->HasAura(WARRIOR_SPELL_BLOOD_AND_THUNDER))
                                _player->CastSpell(target, WARRIOR_SPELL_DEEP_WOUNDS, true);
                        }

                        // Item - Warrior T16 DPS 4P Bonus - 144441
                        if (GetSpellInfo()->Id == WARRIOR_SPELL_MORTAL_STRIKE_AURA || GetSpellInfo()->Id == WARRIOR_SPELL_BLOODTHIRST)
                        {
                            if (_player->HasAura(WARRIOR_SPELL_T16_DPS_4P_BONUS))
                            {
                                if (roll_chance_i(10))
                                    _player->CastSpell(_player, WARRIOR_SPELL_T16_DPS_4P_BONUS_PROC, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_deep_wounds_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_deep_wounds_SpellScript();
        }
};

// Charge - 100
class spell_warr_charge : public SpellScriptLoader
{
    class script_impl : public SpellScript
    {
        PrepareSpellScript(script_impl)

        enum
        {
            CHARGE_STUN        = 7922,
            DOUBLE_TIME        = 103827,
            WARBRINGER         = 103828,
            WARBRINGER_STUN    = 105771,
            DOUBLE_TIME_MARKER = 124184,
            FIRE_VISUAL        = 96840,
            BLAZING_TRAIL      = 123779
        };

        bool canGenerateCharge;

        bool Load()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return false;

            canGenerateCharge = !caster->HasAura(DOUBLE_TIME) || !caster->HasAura(DOUBLE_TIME_MARKER);
            return true;
        }

        void HandleCharge()
        {
            Unit* target = GetHitUnit();
            Unit* caster = GetCaster();
            if (!target || !caster || caster == target)
                return;

            uint32 stunSpellId = caster->HasAura(WARBRINGER) ? WARBRINGER_STUN : CHARGE_STUN;
            caster->CastSpell(target, stunSpellId, true);

            // Glyph of Blazing Trail
            if (caster->HasAura(BLAZING_TRAIL))
                caster->CastSpell(caster, FIRE_VISUAL, true);
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            Unit* caster = GetCaster();
            if (canGenerateCharge && caster)
                caster->EnergizeBySpell(caster, GetSpellInfo()->Id, GetEffectValue(), POWER_RAGE);
        }

        void Register()
        {
            BeforeHit += SpellHitFn(script_impl::HandleCharge);
            OnEffectHitTarget += SpellEffectFn(script_impl::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    public:
        spell_warr_charge() : SpellScriptLoader("spell_warr_charge") { }

    SpellScript* GetSpellScript() const
    {
        return new script_impl;
    }
};

// Execute - 5308
class spell_warr_execute : public SpellScriptLoader
{
    public:
        spell_warr_execute() : SpellScriptLoader("spell_warr_execute") { }

        class spell_warr_execute_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_execute_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(WARRIOR_SPELL_SUDDEN_DEATH))
                        _player->CastSpell(_player, WARRIOR_SPELL_SUDDEN_EXECUTE, true);

                    if (Unit* target = GetHitUnit())
                    {
                        if (SpellInfo const* l_Execute = sSpellMgr->GetSpellInfo(5308))
                        {
                            for (SpellPowerEntry const* l_Power : l_Execute->SpellPowers)
                            {
                                if (l_Power->powerType == Powers::POWER_RAGE)
                                {
                                    int32 l_Cost = int32(l_Power->Cost * -1.0f);
                                    if (_player->HasAura(WARRIOR_SPELL_T16_DPS_4P_BONUS_PROC) && target->GetHealthPct() > 20.0f)
                                    {
                                        _player->EnergizeBySpell(_player, GetSpellInfo()->Id, l_Cost, POWER_RAGE);
                                        _player->RemoveAura(WARRIOR_SPELL_T16_DPS_4P_BONUS_PROC);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_execute_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_execute_SpellScript();
        }
};

// Shield Wall - 871
class spell_warr_shield_wall : public SpellScriptLoader
{
    public:
        spell_warr_shield_wall() : SpellScriptLoader("spell_warr_shield_wall") { }

        class spell_warr_shield_wall_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_shield_wall_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetShield())
                        _player->AddAura(WARRIOR_SPELL_SHIELD_OF_WALL_NOSHIELD, _player);
                    else
                    {
                        if (_player->GetTeam() == HORDE)
                            _player->AddAura(WARRIOR_SPELL_SHIELD_OF_WALL_HORDE, _player);
                        else
                            _player->AddAura(WARRIOR_SPELL_SHIELD_OF_WALL_ALLIANCE, _player);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(WARRIOR_SPELL_SHIELD_OF_WALL_NOSHIELD))
                        _player->RemoveAura(WARRIOR_SPELL_SHIELD_OF_WALL_NOSHIELD);
                    else if (_player->HasAura(WARRIOR_SPELL_SHIELD_OF_WALL_HORDE))
                        _player->RemoveAura(WARRIOR_SPELL_SHIELD_OF_WALL_HORDE);
                    else
                        _player->RemoveAura(WARRIOR_SPELL_SHIELD_OF_WALL_ALLIANCE);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warr_shield_wall_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_warr_shield_wall_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_shield_wall_AuraScript();
        }
};

// Spell Reflection - 23920
class spell_warr_spell_reflection : public SpellScriptLoader
{
    public:
        spell_warr_spell_reflection() : SpellScriptLoader("spell_warr_spell_reflection") { }

        class spell_warr_spell_reflection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_spell_reflection_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetShield())
                        _player->AddAura(WARRIOR_SPELL_SPELL_REFLECTION_NOSHIELD, _player);
                    else
                    {
                        if (_player->GetTeam() == HORDE)
                            _player->AddAura(WARRIOR_SPELL_SPELL_REFLECTION_HORDE, _player);
                        else
                            _player->AddAura(WARRIOR_SPELL_SPELL_REFLECTION_ALLIANCE, _player);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(WARRIOR_SPELL_SPELL_REFLECTION_NOSHIELD))
                        _player->RemoveAura(WARRIOR_SPELL_SPELL_REFLECTION_NOSHIELD);
                    else if (_player->HasAura(WARRIOR_SPELL_SPELL_REFLECTION_HORDE))
                        _player->RemoveAura(WARRIOR_SPELL_SPELL_REFLECTION_HORDE);
                    else
                        _player->RemoveAura(WARRIOR_SPELL_SPELL_REFLECTION_ALLIANCE);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warr_spell_reflection_AuraScript::OnApply, EFFECT_0, SPELL_AURA_REFLECT_SPELLS, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_warr_spell_reflection_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_REFLECT_SPELLS, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_spell_reflection_AuraScript();
        }
};

// Avatar - 107574
class spell_warr_avatar : public SpellScriptLoader
{
    public:
        spell_warr_avatar() : SpellScriptLoader("spell_warr_avatar") { }

        class spell_warr_avatar_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_avatar_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
					_player->RemoveMovementImpairingAuras();
                }
            }
			
            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warr_avatar_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_avatar_AuraScript();
        }
};

// Riposte - 145674
class spell_warr_riposte : public SpellScriptLoader
{
    public:
        spell_warr_riposte() : SpellScriptLoader("spell_warr_riposte") { }

        class spell_warr_riposte_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_riposte_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Aura* riposte = _player->GetAura(WARRIOR_SPELL_RIPOSTE))
                    {
                        float valueParry = _player->GetRatingBonusValue(CR_PARRY) * 885;
                        float valueDodge = _player->GetRatingBonusValue(CR_DODGE) * 885;
                        int32 effectAmount = int32((valueParry + valueDodge) * 0.75f);
                        riposte->GetEffect(0)->ChangeAmount(effectAmount);
                    }
                }
            }

            void Register()
            {
		  OnHit += SpellHitFn(spell_warr_riposte_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_riposte_SpellScript();
        }
};

// Called by Slam - 1464
// Increased damage of Slam if target has Colossus Smash aura
class spell_warr_slam : public SpellScriptLoader
{
    public:
        spell_warr_slam() : SpellScriptLoader("spell_warr_slam") { }

        class spell_warr_slam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_slam_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->HasAura(WARRIOR_COLOSSUS_SMASH_AURA))
                            SetHitDamage(int32(GetHitDamage() * 1.1f));
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_slam_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_slam_SpellScript();
        }
};

// Called by Intervene - 3411, Safeguard - 114029 and Charge - 100
class spell_warr_checkway : public SpellScriptLoader
{
    public:
        spell_warr_checkway() : SpellScriptLoader("spell_warr_checkway") { }

        class spell_warr_checkway_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_checkway_SpellScript);

            SpellCastResult CheckWay()
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return SPELL_FAILED_DONT_REPORT;

                if (!GetExplTargetUnit())
                    return SPELL_FAILED_NO_VALID_TARGETS;

                Position pos;
                if (Unit* target = GetExplTargetUnit())
                {
                    target->GetPosition(&pos);
                    if (!caster->IsWithinLOS(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ()))
                        return SPELL_FAILED_NOPATH;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warr_checkway_SpellScript::CheckWay);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_checkway_SpellScript();
        }
};

// Called by Pummel - 6552 or Heroic Throw - 57755
class spell_warr_glyph_of_gag_order : public SpellScriptLoader
{
    public:
        spell_warr_glyph_of_gag_order() : SpellScriptLoader("spell_warr_glyph_of_gag_order") { }

        class spell_warr_glyph_of_gag_order_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_of_gag_order_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER && caster->HasAura(WARRIOR_GLYPH_OF_GAG_ORDER))
                        {
                            if (caster->ToPlayer() && !caster->ToPlayer()->HasSpellCooldown(WARRIOR_SPELL_IMPENDING_VICTORY))
                            {
                                caster->ToPlayer()->AddSpellCooldown(WARRIOR_SPELL_IMPENDING_VICTORY, 0, 30 * IN_MILLISECONDS);
                                caster->CastSpell(target, WARRIOR_SPELL_GAG_ORDER_SILENCE, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_of_gag_order_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_of_gag_order_SpellScript();
        }
};

// Intervene - 3411
class spell_warr_intervene : public SpellScriptLoader
{
    public:
        spell_warr_intervene() : SpellScriptLoader("spell_warr_intervene") { }

        class spell_warr_intervene_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_intervene_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, WARRIOR_SPELL_INTERVENE_TRIGGERED, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_intervene_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_intervene_SpellScript();
        }
};


// Glyph of Impaling Throws (Heroic Throw - 57755) and (periodic 147838)
class spell_warr_glyph_of_impaling_throws : public SpellScriptLoader
{
    public:
        spell_warr_glyph_of_impaling_throws() : SpellScriptLoader("spell_warr_glyph_of_impaling_throws") { }

        class spell_warr_glyph_of_impaling_throws_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_of_impaling_throws_SpellScript);

            void HandleOnHit()
            {
                if (GetSpellInfo()->Id == 57755)
                    if (Unit* caster = GetCaster())
                        if (Unit* target = GetHitUnit())
                            if (caster->HasAura(146970) && caster->GetDistance(target) > 10.0f)
                                caster->CastSpell(target,147838,true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_of_impaling_throws_SpellScript::HandleOnHit);
            }
        };

        class spell_warr_glyph_of_impaling_throws_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_glyph_of_impaling_throws_AuraScript);

            void OnPeriodic(AuraEffect const* aurEff)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetTarget())
                        if (caster->GetDistance(target) < 5.0f)
                        {
                            if (Player *player = caster->ToPlayer())
                                player->RemoveSpellCooldown(57755,true);
                            target->RemoveAurasDueToSpell(147838);
                        }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_glyph_of_impaling_throws_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_of_impaling_throws_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_glyph_of_impaling_throws_AuraScript();
        }
};

// Glyph of Sweeping Strikes - 58384
class spell_warr_glyph_sweeping_strikes : public SpellScriptLoader
{
    public:
        spell_warr_glyph_sweeping_strikes() : SpellScriptLoader("spell_warr_glyph_sweeping_strikes") { }

        class spell_warr_glyph_sweeping_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_sweeping_strikes_SpellScript);

            void HandleOnHit()
            {
                if (!GetCaster())
                    return;

                if (GetCaster()->HasAura(58384))
                    GetCaster()->CastSpell(GetCaster(),124333,true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_sweeping_strikes_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_sweeping_strikes_SpellScript();
        }
};

// Glyph of Hamstring - 58385
class spell_warr_glyph_of_hamstring : public SpellScriptLoader
{
    public:
        spell_warr_glyph_of_hamstring() : SpellScriptLoader("spell_warr_glyph_of_hamstring") { }

        class spell_warr_glyph_of_hamstring_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_of_hamstring_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(58385) && !caster->GetAuraBeforeInstantCast())
                    {
                        caster->SetAuraBeforeInstantCast(false);
                        caster->CastSpell(caster, 115945, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_of_hamstring_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_of_hamstring_SpellScript();
        }
};

// Glyph of Incite
class spell_warr_glyph_of_incite : public SpellScriptLoader
{
    public:
        spell_warr_glyph_of_incite() : SpellScriptLoader("spell_warr_glyph_of_incite") { }

        class spell_warr_glyph_of_incite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_of_incite_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(122013))
                       if (GetSpellInfo()->Id == 1160) // Demoralzing shout
                        {
                            caster->CastSpell(caster, 122016, true);
                            if (caster->HasAura(122016))
                                caster->GetAura(122016)->SetStackAmount(3);
                        }
            }

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(122016) && GetSpellInfo()->Id != 1160)
                        caster->GetAura(122016)->ModStackAmount(-1);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_of_incite_SpellScript::HandleOnHit);
                AfterCast += SpellCastFn(spell_warr_glyph_of_incite_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_of_incite_SpellScript();
        }
};

// Called by Commanding Shout - 469, Battle Shout - 6673
class spell_warr_shouts : public SpellScriptLoader
{
    public:
        spell_warr_shouts() : SpellScriptLoader("spell_warr_shouts") { }

        class spell_warr_shouts_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shouts_SpellScript);

            void HandleOnCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(WARRIOR_SPELL_GLYPH_OF_BLOODCURDLING_SHOUT))
                        l_Caster->CastSpell(l_Caster, WARRIOR_SPELL_BLOODCURDLING_SHOUT, true);

                    if (l_Caster->HasAura(WARRIOR_SPELL_GLYPH_OF_MYSTIC_SHOUT))
                        l_Caster->CastSpell(l_Caster, WARRIOR_SPELL_MYSTIC_SHOUT_HOVER, true);

                    if (l_Caster->HasAura(WARRIOR_SPELL_GLYPH_OF_WEAPONMASTER))
                    {
                        if (l_Caster->HasAura(WARRIOR_SPELL_WEAPONMASTER))
                            l_Caster->RemoveAura(WARRIOR_SPELL_WEAPONMASTER);

                        l_Caster->CastSpell(l_Caster, WARRIOR_SPELL_WEAPONMASTER, true);
                    }

                }
            }

            void Register() override
            {
                OnCast += SpellCastFn(spell_warr_shouts_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_warr_shouts_SpellScript();
        }
};

/// Glyph of the Weaponmaster - 146974
/// Weaponmaster - 147367
class spell_warr_weaponmaster : public SpellScriptLoader
{
    public:
        spell_warr_weaponmaster() : SpellScriptLoader("spell_warr_weaponmaster") { }

        class spell_warr_weaponmaster_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_weaponmaster_AuraScript);

            enum eSpells
            {
                SingleMindedFury = 81099
            };

            uint64 m_ItemGUID;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                m_ItemGUID = 0;

                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    uint16 l_EndItems = l_Player->HasAura(eSpells::SingleMindedFury) ? EQUIPMENT_SLOT_OFFHAND : EQUIPMENT_SLOT_MAINHAND;

                    for (uint16 l_I = EQUIPMENT_SLOT_MAINHAND; l_I <= l_EndItems; l_I++)
                    {
                        if (Item* l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, l_I))
                        {
                            l_Item->RandomWeaponTransmogrificationFromPrimaryBag(l_Player, l_Item, true);
                            m_ItemGUID = l_Item->GetGUID();
                            break;
                        }
                    }
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Item* l_Item = l_Player->GetItemByGuid(m_ItemGUID))
                        l_Item->RandomWeaponTransmogrificationFromPrimaryBag(l_Player, l_Item, false);
                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_warr_weaponmaster_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_warr_weaponmaster_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_warr_weaponmaster_AuraScript();
        }
};

class spell_warr_enrage : public SpellScriptLoader
{
    public:
        spell_warr_enrage() : SpellScriptLoader("spell_warr_enrage") { }

        class spell_warr_enrage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_enrage_AuraScript);

            bool CalculateAmount(AuraEffect const* /*aurEffect*/, int32& amount, bool& canBeRecalculated)
            {
                amount = 10;
                if (GetCaster()->ToPlayer() && GetCaster()->ToPlayer()->GetSpecializationId() == SPEC_WARRIOR_FURY && GetCaster()->HasAura(76856))
                    amount += int32(GetCaster()->GetFloatValue(PLAYER_MASTERY) * 1.4f);
                canBeRecalculated = false;
                return true;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_enrage_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_enrage_AuraScript();
        }
};
// Called by Skull Banner - 114207
class spell_warr_skull_banner : public SpellScriptLoader
{
public:
    spell_warr_skull_banner() : SpellScriptLoader("spell_warr_skull_banner") { }

    class spell_warr_skull_banner_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_skull_banner_SpellScript);

        void HandleOnCast()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(WARRIOR_SPELL_T15_DPS_4P))
                    l_Caster->CastSpell(l_Caster, WARRIOR_SPELL_T15_DPS_4P_EFFECT, true);
            }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_warr_skull_banner_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_warr_skull_banner_SpellScript();
    }
};
void AddSC_warrior_spell_scripts()
{
    new spell_warr_skull_banner();
    new spell_warr_victorious_state();
    new spell_warr_glyph_of_hindering_strikes();
    new spell_warr_stampeding_shout();
    new spell_warr_shield_barrier();
    new spell_warr_shield_block();
    new spell_warr_storm_bolt();
    new spell_warr_colossus_smash();
    new spell_warr_meat_cleaver();
    new spell_warr_dragon_roar();
    new spell_warr_staggering_shout();
    new spell_warr_frenzied_regeneration();
    new spell_warr_second_wind();
    new spell_warr_taste_for_blood_aura();
    new spell_warr_taste_for_blood();
    new spell_warr_sudden_death();
    new spell_warr_berzerker_rage();
    new spell_warr_mocking_banner();
    new spell_warr_raging_blow_proc();
    new spell_warr_raging_blow();
    new spell_warr_sword_and_board();
    new spell_warr_mortal_strike();
    new spell_warr_rallying_cry();
    new spell_warr_heroic_leap_damage();
    new spell_warr_heroic_leap();
    new spell_warr_shockwave();
    new spell_warr_bloodthirst();
    new spell_warr_victory_rush();
    new spell_warr_last_stand();
    new spell_warr_thunder_clap();
    new spell_warr_deep_wounds();
    new spell_warr_charge();
    new spell_warr_execute();
    new spell_warr_shield_wall();
    new spell_warr_spell_reflection();
    new spell_warr_avatar();
    new spell_warr_riposte();
    new spell_warr_slam();
    new spell_warr_checkway();
    new spell_warr_glyph_of_gag_order();
    new spell_warr_intervene();
    new spell_warr_glyph_of_impaling_throws();
    new spell_warr_glyph_sweeping_strikes();
    new spell_warr_glyph_of_hamstring();
    new spell_warr_glyph_of_incite();
    new spell_warr_shouts();
    new spell_warr_weaponmaster();
    new spell_warr_enrage();
}

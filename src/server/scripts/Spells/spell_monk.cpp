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
 * Scripts for spells with SPELLFAMILY_MONK and SPELLFAMILY_GENERIC spells used by monk players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_monk_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "ScriptedCreature.h"

enum MonkSpells
{
    SPELL_MONK_LEGACY_OF_THE_EMPEROR            = 117667,
    SPELL_MONK_FORTIFYING_BREW                  = 120954,
    SPELL_MONK_PROVOKE                          = 116189,
    SPELL_MONK_PROVOKE_AREA                     = 118635,
    SPELL_MONK_BLACKOUT_KICK_DOT                = 128531,
    SPELL_MONK_BLACKOUT_KICK_HEAL               = 128591,
    SPELL_MONK_SHUFFLE                          = 115307,
    SPELL_MONK_ZEN_PILGRIMAGE                   = 126892,
    SPELL_MONK_ZEN_PILGRIMAGE_RETURN            = 126895,
    SPELL_MONK_DISABLE_ROOT                     = 116706,
    SPELL_MONK_DISABLE                          = 116095,
    SPELL_MONK_SOOTHING_MIST_VISUAL             = 125955,
    SPELL_MONK_SOOTHING_MIST_ENERGIZE           = 116335,
    SPELL_MONK_BREATH_OF_FIRE_DOT               = 123725,
    SPELL_MONK_BREATH_OF_FIRE_CONFUSED          = 123393,
    SPELL_MONK_ELUSIVE_BREW_STACKS              = 128939,
    SPELL_MONK_ELUSIVE_BREW                     = 115308,
    SPELL_MONK_KEG_SMASH_VISUAL                 = 123662,
    SPELL_MONK_KEG_SMASH_ENERGIZE               = 127796,
    SPELL_MONK_WEAKENED_BLOWS                   = 115798,
    SPELL_MONK_DIZZYING_HAZE                    = 116330,
    SPELL_MONK_CLASH_CHARGE                     = 126452,
    SPELL_MONK_LIGHT_STAGGER                    = 124275,
    SPELL_MONK_MODERATE_STAGGER                 = 124274,
    SPELL_MONK_HEAVY_STAGGER                    = 124273,
    SPELL_MONK_DAMAGE_STAGGER                   = 124255,
    SPELL_MONK_ROLL                             = 109132,
    SPELL_MONK_ROLL_TRIGGER                     = 107427,
    SPELL_MONK_CHI_TORPEDO_HEAL                 = 124040,
    SPELL_MONK_CHI_TORPEDO_DAMAGE               = 117993,
    SPELL_MONK_FLYING_SERPENT_KICK              = 101545,
    SPELL_MONK_FLYING_SERPENT_KICK_NEW          = 115057,
    SPELL_MONK_FLYING_SERPENT_KICK_AOE          = 123586,
    SPELL_MONK_TIGEREYE_BREW                    = 116740,
    SPELL_MONK_TIGEREYE_BREW_STACKS             = 125195,
    SPELL_MONK_SPEAR_HAND_STRIKE_SILENCE        = 116709,
    SPELL_MONK_CHI_BURST_DAMAGE                 = 130651,
    SPELL_MONK_CHI_BURST_HEAL                   = 130654,
    SPELL_MONK_ZEN_SPHERE_DAMAGE                = 124098,
    SPELL_MONK_ZEN_SPHERE_HEAL                  = 124081,
    SPELL_MONK_ZEN_SPHERE_DETONATE_HEAL         = 124101,
    SPELL_MONK_ZEN_SPHERE_DETONATE_DAMAGE       = 125033,
    SPELL_MONK_HEALING_ELIXIRS_PASSIVE          = 134563,
    SPELL_MONK_HEALING_ELIXIRS_AURA             = 122280,
    SPELL_MONK_HEALING_ELIXIRS_RESTORE_HEALTH   = 122281,
    SPELL_MONK_RENEWING_MIST_HOT                = 119611,
    SPELL_MONK_RENEWING_MIST_JUMP_AOE           = 119607,
    SPELL_MONK_RENEWING_MIST_JUMP_VISUAL        = 119647,
    SPELL_MONK_RENEWING_MIST_JUMP_AURA = 119607,
    SPELL_MONK_GLYPH_OF_RENEWING_MIST           = 123334,
    SPELL_MONK_SURGING_MIST_HEAL                = 116995,
    SPELL_MONK_ENVELOPING_MIST_HEAL             = 132120,
    SPELL_MONK_PLUS_ONE_MANA_TEA                = 123760,
    SPELL_MONK_MANA_TEA_STACKS                  = 115867,
    SPELL_MONK_MANA_TEA_REGEN                   = 115294,
    SPELL_MONK_SPINNING_CRANE_KICK_HEAL         = 117640,
    MONK_NPC_JADE_SERPENT_STATUE                = 60849,
    SPELL_MONK_UPLIFT_ALLOWING_CAST             = 123757,
    SPELL_MONK_THUNDER_FOCUS_TEA                = 116680,
    SPELL_MONK_PATH_OF_BLOSSOM_AREATRIGGER      = 122035,
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_DAMAGE     = 123408,
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_MISSILE    = 118852,
    SPELL_MONK_SPINNING_FIRE_BLOSSOM_ROOT       = 123407,
    SPELL_MONK_TOUCH_OF_KARMA                   = 122470,
    SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE   = 124280,
    SPELL_MONK_JADE_LIGHTNING_ENERGIZE          = 123333,
    SPELL_MONK_CRACKLING_JADE_SHOCK_BUMP        = 117962,
    SPELL_MONK_POWER_STRIKES_TALENT             = 121817,
    SPELL_MONK_POWER_STRIKES_TALENT_EFFECT      = 129914,
    SPELL_MONK_CREATE_CHI_SPHERE                = 121286,
    SPELL_MONK_GLYPH_OF_ZEN_FLIGHT              = 125893,
    SPELL_MONK_ZEN_FLIGHT                       = 125883,
    SPELL_MONK_BEAR_HUG                         = 127361,
    ITEM_MONK_T14_TANK_4P                       = 123159,
    MONK_NPC_BLACK_OX_STATUE                    = 61146,
    SPELL_MONK_GUARD                            = 115295,
    SPELL_MONK_ITEM_2_S12_MISTWEAVER            = 131561,
    SPELL_MONK_ITEM_4_S12_MISTWEAVER            = 124487,
    SPELL_MONK_ZEN_FOCUS                        = 124488,
    SPELL_MONK_EMINENCE_HEAL                    = 126890,
    SPELL_MONK_GRAPPLE_WEAPON_DPS_UPGRADE       = 123231,
    SPELL_MONK_GRAPPLE_WEAPON_TANK_UPGRADE      = 123232,
    SPELL_MONK_GRAPPLE_WEAPON_HEAL_UPGRADE      = 123234,
    SPELL_MONK_GLYPH_OF_BLACKOUT_KICK           = 132005,
    SPELL_MONK_CHI_WAVE_HEAL                    = 132463,
    SPELL_MONK_CHI_WAVE_DAMAGE                  = 132467,
    SPELL_MONK_CHI_WAVE_HEALING_BOLT            = 132464,
    SPELL_MONK_CHI_WAVE_TALENT_AURA             = 115098,
    SPELL_MONK_ITEM_PVP_GLOVES_BONUS            = 124489,
    SPELL_MONK_MUSCLE_MEMORY                    = 139598,
    SPELL_MONK_MUSCLE_MEMORY_EFFECT             = 139597,
    SPELL_MONK_CHI_BREW                         = 115399,
    SPELL_MONK_GLYPH_OF_MANA_TEA                = 123763,
    SPELL_MONK_GLYPH_OF_MANA_TEA_EFFECT         = 123761,
    SPELL_MONK_COMBO_BREAKER_1                  = 118864,
    SPELL_MONK_COMBO_BREAKER_2                  = 116768,
    SPELL_MONK_LEER_OF_THE_OX                   = 115543,
    SPELL_MONK_LEER_OF_THE_OX_EFFECT            = 118585,
    SPELL_MONK_LEER_OF_THE_OX_GLYPH             = 125967,
    SPELL_MONK_GLYPH_OF_LIFE_COCOON             = 124989,
    SPELL_MONK_MASTERY_BOTTLED_FURY             = 115636,
    SPELL_MONK_STORM_EARTH_AND_FIRE             = 137639,
    SPELL_MONK_RING_OF_PEACE_AURA               = 140023,
    SPELL_MONK_RING_OF_PEACE_DISARM             = 137461,
    SPELL_MONK_RING_OF_PEACE_SILENCE            = 137460,
    MONK_GLYPH_OF_PARALYSIS                     = 125755,
    SPELL_MONK_EMINENCE_AURA                    = 130324,
    SPELL_MONK_GLYPH_OF_GUARD                   = 123401
};

// Ring of Peace (dummy) - 140023
class spell_monk_ring_of_peace_dummy : public SpellScriptLoader
{
    public:
        spell_monk_ring_of_peace_dummy() : SpellScriptLoader("spell_monk_ring_of_peace_dummy") {}

        class spell_monk_ring_of_peace_dummy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_ring_of_peace_dummy_AuraScript);

            void HandleDummyProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        caster->CastSpell(target, SPELL_MONK_RING_OF_PEACE_SILENCE, true);
                        caster->CastSpell(target, SPELL_MONK_RING_OF_PEACE_DISARM, true);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_monk_ring_of_peace_dummy_AuraScript::HandleDummyProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_ring_of_peace_dummy_AuraScript();
        }
};

// Ring of Peace - 116844
class spell_monk_ring_of_peace : public SpellScriptLoader
{
    public:
        spell_monk_ring_of_peace() : SpellScriptLoader("spell_monk_ring_of_peace") { }

        class spell_monk_ring_of_peace_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_ring_of_peace_AuraScript);

            void OnUpdate(uint32 diff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetUnitOwner())
                    {
                        std::list<Unit*> targetList;
                        float radius = 8.0f;

                        JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(target, caster, radius);
                        JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(target, targetList, u_check);
                        target->VisitNearbyObject(radius, searcher);

                        for (auto itr : targetList)
                            caster->CastSpell(itr, SPELL_MONK_RING_OF_PEACE_AURA, true);
                    }
                }
            }

            void Register()
            {
                OnAuraUpdate += AuraUpdateFn(spell_monk_ring_of_peace_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_ring_of_peace_AuraScript();
        }
};

const uint32 spiritEntry[3] = { 69680,  69792,  69791  }; // Storm, Earth and Fire
const uint32 summonsMonk[3] = { 138122, 138121, 138123 }; // Storm, Earth and Fire
const uint32 visualMorph[3] = { 138080, 138083, 138081 }; // Storm, Earth and Fire

// Storm, Earth and Fire - 138130
class spell_monk_storm_earth_and_fire_stats : public SpellScriptLoader
{
    public:
        spell_monk_storm_earth_and_fire_stats() : SpellScriptLoader("spell_monk_storm_earth_and_fire_stats") { }

        class spell_monk_storm_earth_and_fire_stats_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_storm_earth_and_fire_stats_AuraScript);

            void OnUpdate(uint32 /*diff*/, AuraEffect* aurEff)
            {
                if (!GetCaster())
                    return;

                if (Unit* caster = GetCaster()->GetOwner())
                {
                    if (AuraEffect* stormAura = caster->GetAuraEffect(SPELL_MONK_STORM_EARTH_AND_FIRE, EFFECT_1))
                    {
                        if (aurEff->GetAmount() != stormAura->GetAmount())
                            aurEff->ChangeAmount(stormAura->GetAmount());
                    }
                }
            }

            bool CalculateReducedDamage(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (!GetCaster() || !GetCaster()->GetOwner())
                    return false;

                if (Unit* owner = GetCaster()->GetOwner())
                    if (AuraEffect* stormAura = owner->GetAuraEffect(SPELL_MONK_STORM_EARTH_AND_FIRE, EFFECT_1))
                        amount = stormAura->GetAmount();
                return true;
            }

            bool CalculateHealing(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (!GetCaster() || !GetCaster()->GetOwner())
                    return false;

                if (Unit* owner = GetCaster()->GetOwner())
                    amount = owner->GetTotalAuraModifier(SPELL_AURA_MOD_HEALING_DONE_PERCENT);

                return true;
            }

            bool CalculateAttackPower(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (!GetCaster() || !GetCaster()->GetOwner())
                    return false;

                if (Unit* owner = GetCaster()->GetOwner())
                    amount = owner->GetTotalAttackPowerValue(BASE_ATTACK);

                return true;
            }

            bool CalculateHaste(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (!GetCaster() || !GetCaster()->GetOwner())
                    return false;

                if (Player* owner = GetCaster()->GetOwner()->ToPlayer())
                {
                    if (Unit* caster = GetCaster())
                    {
                        // Convert Owner's haste into the Spirit haste
                        float ownerHaste = 1.0f  +  owner->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CR_HASTE_MELEE) *
                                                    owner->GetRatingMultiplier(CR_HASTE_MELEE) / 100.0f;
                        caster->ApplyPercentModFloatValue(UNIT_MOD_CAST_SPEED, ownerHaste, false);
                        caster->ApplyPercentModFloatValue(UNIT_MOD_CAST_HASTE, ownerHaste, false);
                        caster->ApplyPercentModFloatValue(UNIT_MOD_HASTE, ownerHaste, false);
                        return true;
                    }
                }

                return false;
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_monk_storm_earth_and_fire_stats_AuraScript::OnUpdate, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_storm_earth_and_fire_stats_AuraScript::CalculateReducedDamage, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_storm_earth_and_fire_stats_AuraScript::CalculateHealing, EFFECT_3, SPELL_AURA_MOD_HEALING_DONE_PERCENT);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_storm_earth_and_fire_stats_AuraScript::CalculateAttackPower, EFFECT_4, SPELL_AURA_MOD_ATTACK_POWER);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_storm_earth_and_fire_stats_AuraScript::CalculateHaste, EFFECT_5, SPELL_AURA_MOD_ATTACKSPEED_2);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_storm_earth_and_fire_stats_AuraScript();
        }
};

// Storm, Earth and Fire - 137639
class spell_monk_storm_earth_and_fire : public SpellScriptLoader
{
    public:
        spell_monk_storm_earth_and_fire() : SpellScriptLoader("spell_monk_storm_earth_and_fire") { }

        class spell_monk_storm_earth_and_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_storm_earth_and_fire_SpellScript);

            void HandleTarget(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        Creature* spiritSummoned[3]; // Storm, Earth, Fire
                        for (Creature*& spiritPointer : spiritSummoned)
                            spiritPointer = nullptr;

                        uint32 spiritCount = 0;

                        for (Unit* controlled : caster->m_Controlled)
                        {
                            bool isSpirit = false;
                            switch (controlled->GetEntry())
                            {
                                case 69680: // Storm
                                    isSpirit = true;
                                    spiritSummoned[0] = controlled->ToCreature();
                                    break;

                                case 69792: // Earth
                                    isSpirit = true;
                                    spiritSummoned[1] = controlled->ToCreature();
                                    break;

                                case 69791: // Fire
                                    isSpirit = true;
                                    spiritSummoned[2] = controlled->ToCreature();
                                    break;

                                default:
                                    break;
                            }

                            if (isSpirit)
                            {
                                if (Creature* spirit = controlled->ToCreature())
                                {
                                    if (spirit->AI() && spirit->AI()->GetGUID() == target->GetGUID())
                                    {
                                        spirit->AI()->DoAction(0); // Disappear
                                        return;
                                    }
                                }

                                spiritCount++;
                            }
                        }

                        if (spiritCount == 0)
                            caster->CastSpell(caster, visualMorph[urand(0, 2)], true);
                        else if (spiritCount >= 2)
                        {
                            // When using this spell against a new target while having two spirits, this will make them both disappear (as tested on retail)
                            for (Creature*& spiritPointer : spiritSummoned)
                            {
                                if (spiritPointer && spiritPointer->AI())
                                    spiritPointer->AI()->DoAction(0); // Disappear

                            }
                            return;
                        }

                        for (uint8 i = 0; i < 3; ++i)
                        {
                            if (caster->HasAura(visualMorph[i]) || spiritSummoned[i])
                                continue;

                            caster->CastSpell(caster, summonsMonk[i], true);

                            // Find summoned spirit
                            for (Unit* controlled : caster->m_Controlled)
                            {
                                if (controlled->GetEntry() == spiritEntry[i])
                                {
                                    if (controlled->GetAI())
                                        controlled->GetAI()->SetGUID(target->GetGUID());
                                    break;
                                }
                            }

                            break;
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_storm_earth_and_fire_SpellScript::HandleTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_storm_earth_and_fire_SpellScript();
        }

        class spell_monk_storm_earth_and_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_storm_earth_and_fire_AuraScript);

            bool CalculateAmount(AuraEffect const* aurEff, int32& amount, bool & /*canBeRecalculated*/)
            {
                amount = (aurEff->GetBase()->GetStackAmount() > 1) ? -45 : -30;

                return true;
            }

            void OnUpdate(uint32 /*diff*/, AuraEffect* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    uint8 count = 0;
                    for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                        if ((*itr)->GetEntry() == 69680 || (*itr)->GetEntry() == 69792 || (*itr)->GetEntry() == 69791)
                            ++count;

                    if (!count)
                    {
                        caster->RemoveAura(GetSpellInfo()->Id);
                        return;
                    }

                    if (Aura* stormAura = caster->GetAura(GetSpellInfo()->Id))
                    {
                        if (count != stormAura->GetStackAmount())
                            stormAura->SetStackAmount(count);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    for (uint8 i = 0; i < 3; ++i)
                        caster->RemoveAura(visualMorph[i]);

                    for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                    {
                        if ((*itr)->GetEntry() == 69680 || (*itr)->GetEntry() == 69792 || (*itr)->GetEntry() == 69791)
                        {
                            if (Creature* spirit = (*itr)->ToCreature())
                            {
                                spirit->GetMotionMaster()->Clear();
                                spirit->GetMotionMaster()->MoveJump(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), 15.0f, 10.0f, spirit->GetOrientation(), 1);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_storm_earth_and_fire_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_storm_earth_and_fire_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_DUMMY);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_storm_earth_and_fire_AuraScript::CalculateAmount, EFFECT_3, SPELL_AURA_ADD_PCT_MODIFIER);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_storm_earth_and_fire_AuraScript::CalculateAmount, EFFECT_4, SPELL_AURA_ADD_PCT_MODIFIER);
                OnEffectUpdate += AuraEffectUpdateFn(spell_monk_storm_earth_and_fire_AuraScript::OnUpdate, EFFECT_2, SPELL_AURA_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_monk_storm_earth_and_fire_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_storm_earth_and_fire_AuraScript();
        }
};

// Fists of Fury (stun effect) - 120086
class spell_monk_fists_of_fury_stun : public SpellScriptLoader
{
    public:
        spell_monk_fists_of_fury_stun() : SpellScriptLoader("spell_monk_fists_of_fury_stun") { }

        class spell_monk_fists_of_fury_stun_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_fists_of_fury_stun_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraCheck(true, GetSpellInfo()->Id));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_fists_of_fury_stun_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_fists_of_fury_stun_SpellScript();
        }
};

// Expel Harm - 115072
class spell_monk_expel_harm : public SpellScriptLoader
{
    public:
        spell_monk_expel_harm() : SpellScriptLoader("spell_monk_expel_harm") { }

        class spell_monk_expel_harm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_expel_harm_SpellScript);

            void HandleOnHit()
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    std::list<Unit*> targetList;
                    float radius = 10.0f;

                    JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(_player, _player, radius);
                    JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(_player, targetList, u_check);
                    _player->VisitNearbyObject(radius, searcher);

                    for (auto itr : targetList)
                    {
                        if (_player->IsValidAttackTarget(itr))
                        {
                            int32 bp = CalculatePct((-GetHitDamage()), 50);
                            _player->CastCustomSpell(itr, 115129, &bp, NULL, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_expel_harm_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_expel_harm_SpellScript();
        }
};

// Chi Wave (healing bolt) - 132464
class spell_monk_chi_wave_healing_bolt : public SpellScriptLoader
{
    public:
        spell_monk_chi_wave_healing_bolt() : SpellScriptLoader("spell_monk_chi_wave_healing_bolt") { }

        class spell_monk_chi_wave_healing_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_wave_healing_bolt_SpellScript);

            void HandleOnHit()
            {
                if (!GetOriginalCaster())
                    return;

                if (Player* _player = GetOriginalCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_MONK_CHI_WAVE_HEAL, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_chi_wave_healing_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_wave_healing_bolt_SpellScript();
        }
};

// Chi Wave (damage) - 132467 and Chi Wave (heal) - 132463
class spell_monk_chi_wave_bolt : public SpellScriptLoader
{
    public:
        spell_monk_chi_wave_bolt() : SpellScriptLoader("spell_monk_chi_wave_bolt") { }

        class spell_monk_chi_wave_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_wave_bolt_SpellScript);

            void HandleOnHit()
            {
                if (!GetOriginalCaster() || !GetHitUnit())
                    return;

                Unit* originalcaster = GetOriginalCaster();
                Unit* target = GetHitUnit();

                if (AuraEffect* chiWave = originalcaster->GetAuraEffect(SPELL_MONK_CHI_WAVE_TALENT_AURA, EFFECT_1))
                {
                    int32 count = chiWave->GetAmount();

                    if (count >= 7)
                    {
                        originalcaster->RemoveAura(SPELL_MONK_CHI_WAVE_TALENT_AURA);
                        return;
                    }

                    count++;
                    chiWave->SetAmount(count);

                    // current spell is heal (1) or damage (2)
                    int32 bp1 = (GetSpellInfo()->Id == 132463 ? 1 : 2);

                    target->CastCustomSpell(target, 132466, 0, &bp1, 0, true, 0, 0, originalcaster->GetGUID());
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_chi_wave_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_wave_bolt_SpellScript();
        }
};

// Chi Wave (damage) - 132467 and Chi Wave (heal) - 132463
class spell_monk_chi_wave_aoe : public SpellScriptLoader
{
public:
    spell_monk_chi_wave_aoe() : SpellScriptLoader("spell_monk_chi_wave_aoe") { }

    class spell_monk_chi_wave_aoe_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_chi_wave_aoe_SpellScript);

        void FilterTargets(std::list<WorldObject*>& objectList)
        {
            if (!GetOriginalCaster() || !GetCaster())
                return;

            Unit* caster = GetCaster();
            Unit* originalCaster = GetOriginalCaster();

            if (Spell* spell = GetSpell())
            {
                bool isHeal = (spell->GetSpellValue(SPELLVALUE_BASE_POINT1) == 1);

                std::list<Unit*> unitList;
                for (WorldObject* object : objectList)
                    unitList.push_back(object->ToUnit());

                if (isHeal)
                {
                    // next spell is damage
                    unitList.remove_if(UnfriendlyUnitCheck(originalCaster));

                    if (unitList.size() > 1)
                    {
                        unitList.sort(JadeCore::DistanceOrderPred(caster));
                        unitList.resize(1);
                    }

                    if (!unitList.empty())
                        caster->CastCustomSpell(unitList.front(), 132467, 0, 0, 0, true, 0, 0, originalCaster->GetGUID());
                }
                else
                {
                    // next spell is heal
                    unitList.remove_if(FriendlyUnitCheck(originalCaster));

                    if (unitList.size() > 1)
                    {
                        unitList.sort(JadeCore::HealthPctOrderPred()); // "When bouncing to allies, Chi Wave will prefer those injured over full health"
                        unitList.resize(1);
                    }

                    if (!unitList.empty())
                        caster->CastCustomSpell(unitList.front(), 132464, 0, 0, 0, true, 0, 0, originalCaster->GetGUID());
                }

                objectList.clear();
                if (!unitList.empty())
                    objectList.push_back(unitList.front());
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_chi_wave_aoe_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
        }

        class FriendlyUnitCheck
        {
            public:
                FriendlyUnitCheck(Unit* caster) : _caster(caster) {}

                bool operator()(WorldObject* object) const
                {
                    if (object->GetTypeId() != TYPEID_UNIT &&
                        object->GetTypeId() != TYPEID_PLAYER)
                        return true;

                    return !_caster->IsValidAssistTarget(object->ToUnit());
                }

            private:
                Unit* _caster;
        };

        class UnfriendlyUnitCheck
        {
            public:
                UnfriendlyUnitCheck(Unit* caster) : _caster(caster) {}

                bool operator()(WorldObject* object) const
                {
                    if (object->GetTypeId() != TYPEID_UNIT &&
                        object->GetTypeId() != TYPEID_PLAYER)
                        return true;

                    return !_caster->IsValidAttackTarget(object->ToUnit());
                }

            private:
                Unit* _caster;
        };
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_chi_wave_aoe_SpellScript();
    }
};

// Chi Wave (talent) - 115098
class spell_monk_chi_wave : public SpellScriptLoader
{
    public:
        spell_monk_chi_wave() : SpellScriptLoader("spell_monk_chi_wave") { }

        class spell_monk_chi_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_wave_SpellScript);

            uint64 targetGUID;
            bool done;

            bool Load()
            {
                targetGUID = 0;
                done = false;
                return true;
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                if (Unit* target = GetHitUnit())
                    targetGUID = target->GetGUID();
            }

            void HandleApplyAura()
            {
                if (!targetGUID || done)
                    return;

                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = sObjectAccessor->GetUnit(*caster, targetGUID))
                    {
                        if (caster->IsValidAttackTarget(target))
                            caster->CastSpell(target, SPELL_MONK_CHI_WAVE_DAMAGE, true);
                        else if (caster->IsValidAssistTarget(target))
                            caster->CastSpell(target, SPELL_MONK_CHI_WAVE_HEALING_BOLT, true);

                        done = true;
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_wave_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                AfterHit += SpellHitFn(spell_monk_chi_wave_SpellScript::HandleApplyAura);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_wave_SpellScript();
        }
};

// Grapple Weapon - 117368
class spell_monk_grapple_weapon : public SpellScriptLoader
{
    public:
        spell_monk_grapple_weapon() : SpellScriptLoader("spell_monk_grapple_weapon") { }

        class spell_monk_grapple_weapon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_grapple_weapon_SpellScript)

            void HandleBeforeHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->ToPlayer())
                        {
                            Item* mainItem = _player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                            Item* targetMainItem = target->ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                            if (mainItem && targetMainItem)
                            {
                                if (targetMainItem->GetTemplate()->ItemLevel > mainItem->GetTemplate()->ItemLevel)
                                {
                                    switch (_player->GetSpecializationId(_player->GetActiveSpec()))
                                    {
                                        case SPEC_MONK_BREWMASTER:
                                            _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_TANK_UPGRADE, true);
                                            break;
                                        case SPEC_MONK_MISTWEAVER:
                                            _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_HEAL_UPGRADE, true);
                                            break;
                                        case SPEC_MONK_WINDWALKER:
                                            _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_DPS_UPGRADE, true);
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            }
                        }
                        else if (target->GetTypeId() == TYPEID_UNIT)
                        {
                            if (target->getLevel() > _player->getLevel())
                            {
                                switch (_player->GetSpecializationId(_player->GetActiveSpec()))
                                {
                                    case SPEC_MONK_BREWMASTER:
                                        _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_TANK_UPGRADE, true);
                                        break;
                                    case SPEC_MONK_MISTWEAVER:
                                        _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_HEAL_UPGRADE, true);
                                        break;
                                    case SPEC_MONK_WINDWALKER:
                                        _player->CastSpell(_player, SPELL_MONK_GRAPPLE_WEAPON_DPS_UPGRADE, true);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_monk_grapple_weapon_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_grapple_weapon_SpellScript();
        }
};

// Transcendence : Transfer - 119996
class spell_monk_transcendence_transfer : public SpellScriptLoader
{
public:
    spell_monk_transcendence_transfer() : SpellScriptLoader("spell_monk_transcendence_transfer") { }

    class spell_monk_transcendence_transfer_SpellScript : public SpellScript
    {
    public:

        Creature* m_copy;

        spell_monk_transcendence_transfer_SpellScript()
            : m_copy(nullptr)
        {
        }

        PrepareSpellScript(spell_monk_transcendence_transfer_SpellScript);

        SpellCastResult CheckRange()
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->ToPlayer())
                return SPELL_FAILED_DONT_REPORT;

            float distance = 40.0f;
            if (caster->HasAura(123023))
                distance = 50.0f;

            std::list<Creature*> npcs;
            GetCreatureListWithEntryInGrid(npcs, caster, 54569, distance);
            for (Creature* creature : npcs)
            {
                if (creature->ToTempSummon())
                {
                    if (creature->ToTempSummon()->GetOwner() == caster)
                    {
                        m_copy = creature;
                        break;
                    }
                }
            }

            if (m_copy == nullptr)
            {
                GetCaster()->ToPlayer()->GetSession()->SendNotification("Your clone is too far away!");
                return SPELL_FAILED_DONT_REPORT;
            }

            return SPELL_CAST_OK;
        }

        void HandleAfterCast()
        {
            if (!m_copy)
                return;

            if (Player* player = GetCaster()->ToPlayer())
            {
                Position tpos;
                m_copy->GetPosition(&tpos);

                Position ppos;
                player->GetPosition(&ppos);

                m_copy->NearTeleportTo(ppos.GetPositionX(), ppos.GetPositionY(), ppos.GetPositionZ(), ppos.GetOrientation());

                player->TeleportTo(m_copy->GetMapId(), tpos.GetPositionX(), tpos.GetPositionY(), tpos.GetPositionZ(), tpos.GetOrientation(), TELE_TO_NOT_LEAVE_COMBAT);
            }
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_monk_transcendence_transfer_SpellScript::CheckRange);
            AfterCast += SpellCastFn(spell_monk_transcendence_transfer_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_transcendence_transfer_SpellScript();
    }
};

// Dampen Harm - 122278
class spell_monk_dampen_harm : public SpellScriptLoader
{
    public:
        spell_monk_dampen_harm() : SpellScriptLoader("spell_monk_dampen_harm") { }

        class spell_monk_dampen_harm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_dampen_harm_AuraScript);

            uint32 healthPct;

            bool Load()
            {
                healthPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return GetUnitOwner()->ToPlayer();
            }

            bool CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
                return true;
            }

            void Absorb(AuraEffect* auraEffect, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                uint32 health = target->CountPctFromMaxHealth(healthPct);

                if (dmgInfo.GetDamage() < health)
                    return;

                // The next 3 attacks within 45 sec that deal damage equal to 10% or more of your total health are reduced by half
                absorbAmount = dmgInfo.GetDamage() / 2;
                auraEffect->GetBase()->DropCharge();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_dampen_harm_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_dampen_harm_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_dampen_harm_AuraScript();
        }
};

// Called by Thunder Focus Tea - 116680
// Item S12 4P - Mistweaver - 124487
class spell_monk_item_s12_4p_mistweaver : public SpellScriptLoader
{
    public:
        spell_monk_item_s12_4p_mistweaver() : SpellScriptLoader("spell_monk_item_s12_4p_mistweaver") { }

        class spell_monk_item_s12_4p_mistweaver_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_item_s12_4p_mistweaver_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(SPELL_MONK_ITEM_4_S12_MISTWEAVER))
                        _player->CastSpell(_player, SPELL_MONK_ZEN_FOCUS, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_item_s12_4p_mistweaver_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_item_s12_4p_mistweaver_SpellScript();
        }
};

struct auraData
{
    auraData(uint32 id, uint64 casterGUID) : m_id(id), m_casterGuid(casterGUID) {}
    uint32 m_id;
    uint64 m_casterGuid;
};

// Diffuse Magic - 122783
class spell_monk_diffuse_magic : public SpellScriptLoader
{
    public:
        spell_monk_diffuse_magic() : SpellScriptLoader("spell_monk_diffuse_magic") { }

        class spell_monk_diffuse_magic_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_diffuse_magic_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    std::set<auraData*> auraListToRemove;
                    Unit::AuraApplicationMap AuraList = _player->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::iterator iter = AuraList.begin(); iter != AuraList.end(); ++iter)
                    {
                        Aura* aura = iter->second->GetBase();
                        if (!aura)
                            continue;

                        Unit* caster = aura->GetCaster();
                        if (!caster || caster->GetGUID() == _player->GetGUID())
                            continue;

                        if (caster->IsVehicle())
                            continue;

                        if (!caster->IsWithinDist(_player, 40.0f))
                            continue;

                        if (aura->GetSpellInfo()->IsPositive())
                            continue;

                        if (!(aura->GetSpellInfo()->GetSchoolMask() & SPELL_SCHOOL_MASK_MAGIC))
                            continue;

                        _player->AddAura(aura->GetSpellInfo()->Id, caster);

                        if (Aura* targetAura = caster->GetAura(aura->GetSpellInfo()->Id, _player->GetGUID()))
                        {
                            for (int i = 0; i < MAX_SPELL_EFFECTS; ++i)
                            {
                                if (targetAura->GetEffect(i) && aura->GetEffect(i))
                                {
                                    AuraEffect* auraEffect = _player->GetAuraEffect(aura->GetSpellInfo()->Id, i);
                                    if (!auraEffect)
                                        continue;

                                    int32 damage = auraEffect->GetAmount();

                                    // need to check auraEffect pointer here !!
                                    if (auraEffect->GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE ||
                                        auraEffect->GetAuraType() == SPELL_AURA_PERIODIC_DAMAGE_PERCENT)
                                        damage = caster->SpellDamageBonusDone(_player, aura->GetSpellInfo(), damage, DOT, auraEffect->GetBase()->GetStackAmount());

                                    targetAura->GetEffect(i)->SetAmount(damage);
                                }
                            }
                        }

                        auraListToRemove.insert(new auraData(aura->GetSpellInfo()->Id, caster->GetGUID()));
                    }

                    for (std::set<auraData*>::iterator itr = auraListToRemove.begin(); itr != auraListToRemove.end(); ++itr)
                    {
                        _player->RemoveAura((*itr)->m_id, (*itr)->m_casterGuid);
                        delete (*itr);
                    }

                    auraListToRemove.clear();
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_diffuse_magic_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_diffuse_magic_SpellScript();
        }
};

// Sanctuary of the Ox - 126119
class spell_monk_black_ox_statue : public SpellScriptLoader
{
    public:
        spell_monk_black_ox_statue() : SpellScriptLoader("spell_monk_black_ox_statue") { }

        class spell_monk_black_ox_statue_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_black_ox_statue_AuraScript);

            uint32 m_DamageDealt;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                m_DamageDealt = 0;
            }

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                if (Unit* l_Player = GetUnitOwner())
                {
                    if (eventInfo.GetDamageInfo()->GetVictim() == l_Player)
                        return; // Don't proc on self-inflicted damage

                    m_DamageDealt += eventInfo.GetDamageInfo()->GetDamage();

                    uint32 l_AP = l_Player->GetTotalAttackPowerValue(BASE_ATTACK);
                    uint32 l_Threshold = l_AP * 16;
                    if (m_DamageDealt >= l_Threshold)
                    {
                        if (Unit* l_Statue = GetCaster())
                        {
                            if (l_Statue->GetOwner() && l_Statue->GetOwner()->GetGUID() == l_Player->GetGUID())
                            {
                                std::list<Unit*> l_Targets;
                                l_Player->GetPartyMembers(l_Targets, true);

                                l_Targets.remove_if([=] (Unit* l_Target)
                                {
                                    if (l_Target == l_Player || l_Target->IsFullHealth() || l_Target->GetEntry() == MONK_NPC_BLACK_OX_STATUE)
                                        return true;

                                    return false;
                                });

                                if (!l_Targets.empty())
                                {
                                    m_DamageDealt %= l_Threshold; // To prevent players from accumulating more than one guard charge

                                    Unit* l_Target = JadeCore::Containers::SelectRandomContainerElement(l_Targets);
                                    l_Target->CastSpell(l_Target, SPELL_MONK_GUARD, true, nullptr, nullptr, l_Statue->GetGUID());
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_monk_black_ox_statue_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
                AfterEffectApply += AuraEffectApplyFn(spell_monk_black_ox_statue_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_black_ox_statue_AuraScript();
        }
};

// Guard - 115295, Guard (Glyphed) - 123402
class spell_monk_guard : public SpellScriptLoader
{
    public:
        spell_monk_guard() : SpellScriptLoader("spell_monk_guard") { }

        class spell_monk_guard_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_guard_AuraScript);

            bool CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & /*canBeRecalculated*/)
            {
                Unit* l_StatUnit = GetCaster();
                if (l_StatUnit && !l_StatUnit->IsPlayer())
                    l_StatUnit = l_StatUnit->GetOwner();

                if (l_StatUnit)
                {
                    amount += l_StatUnit->GetTotalAttackPowerValue(BASE_ATTACK) * 1.971f;
                    if (l_StatUnit->HasAura(ITEM_MONK_T14_TANK_4P))
                        AddPct(amount, 20);
                    if (l_StatUnit->HasAura(SPELL_MONK_GLYPH_OF_GUARD))
                        AddPct(amount, 10);
                }

                return true;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_guard_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_guard_AuraScript();
        }
};

// Bear Hug - 127361
class spell_monk_bear_hug : public SpellScriptLoader
{
    public:
        spell_monk_bear_hug() : SpellScriptLoader("spell_monk_bear_hug") { }

        class spell_monk_bear_hug_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_bear_hug_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (Aura* bearHug = target->GetAura(SPELL_MONK_BEAR_HUG, _player->GetGUID()))
                            if (bearHug->GetEffect(1))
                                bearHug->GetEffect(1)->SetAmount(_player->CountPctFromMaxHealth(2));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_bear_hug_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_bear_hug_SpellScript();
        }
};

// Zen Flight - 125883
class spell_monk_zen_flight_check : public SpellScriptLoader
{
    public:
        spell_monk_zen_flight_check() : SpellScriptLoader("spell_monk_zen_flight_check") { }

        class spell_monk_zen_flight_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_zen_flight_check_SpellScript);

            SpellCastResult CheckTarget()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetMap()->IsRaidOrHeroicDungeon())
                        return SPELL_FAILED_NOT_IN_RAID_INSTANCE;

                    if (_player->GetMap()->IsBattlegroundOrArena())
                        return SPELL_FAILED_NOT_IN_BATTLEGROUND;

                    // In Kalimdor or Eastern Kingdom with Flight Master's License
                    if (!_player->HasSpell(90267) && (_player->GetMapId() == 1 || _player->GetMapId() == 0))
                        return SPELL_FAILED_NOT_HERE;

                    // In Pandaria with Wisdom of the Four Winds
                    if (!_player->HasSpell(115913) && (_player->GetMapId() == 870))
                        return SPELL_FAILED_NOT_HERE;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_zen_flight_check_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_zen_flight_check_SpellScript();
        }

        class spell_monk_zen_flight_check_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_zen_flight_check_AuraScript);

            bool Load()
            {
                return GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (!GetCaster())
                    return false;

                if (Player* caster = GetCaster()->ToPlayer())
                    if (caster->GetSkillValue(SKILL_RIDING) >= 375)
                        amount = 310;

                return true;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_zen_flight_check_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_zen_flight_check_AuraScript();
        }
};

// Glyph of Zen Flight - 125893
class spell_monk_glyph_of_zen_flight : public SpellScriptLoader
{
    public:
        spell_monk_glyph_of_zen_flight() : SpellScriptLoader("spell_monk_glyph_of_zen_flight") { }

        class spell_monk_glyph_of_zen_flight_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_glyph_of_zen_flight_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(SPELL_MONK_ZEN_FLIGHT, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(SPELL_MONK_ZEN_FLIGHT))
                        _player->removeSpell(SPELL_MONK_ZEN_FLIGHT, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_monk_glyph_of_zen_flight_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_monk_glyph_of_zen_flight_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_glyph_of_zen_flight_AuraScript();
        }
};

// Called by Jab - 100780
// Power Strikes - 121817
class spell_monk_power_strikes : public SpellScriptLoader
{
    public:
        spell_monk_power_strikes() : SpellScriptLoader("spell_monk_power_strikes") { }

        class spell_monk_power_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_power_strikes_SpellScript)

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetGUID() != _player->GetGUID())
                        {
                            if (_player->HasAura(SPELL_MONK_POWER_STRIKES_TALENT))
                            {
                                if (!_player->HasSpellCooldown(SPELL_MONK_POWER_STRIKES_TALENT))
                                {
                                    if (_player->GetPower(POWER_CHI) < _player->GetMaxPower(POWER_CHI))
                                    {
                                        _player->EnergizeBySpell(_player, GetSpellInfo()->Id, 1, POWER_CHI);
                                        _player->AddSpellCooldown(SPELL_MONK_POWER_STRIKES_TALENT, 0, 20 * IN_MILLISECONDS);
                                        _player->RemoveAura(SPELL_MONK_POWER_STRIKES_TALENT_EFFECT);
                                        _player->AddSpellCooldown(SPELL_MONK_POWER_STRIKES_TALENT_EFFECT, 0, 20 * IN_MILLISECONDS);
                                    }
                                    else
                                        _player->CastSpell(_player, SPELL_MONK_CREATE_CHI_SPHERE, true);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_power_strikes_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_power_strikes_SpellScript();
        }
};

// Crackling Jade Lightning - 117952
class spell_monk_crackling_jade_lightning : public SpellScriptLoader
{
    public:
        spell_monk_crackling_jade_lightning() : SpellScriptLoader("spell_monk_crackling_jade_lightning") { }

        class spell_monk_crackling_jade_lightning_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_crackling_jade_lightning_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (roll_chance_i(30))
                        caster->CastSpell(caster, SPELL_MONK_JADE_LIGHTNING_ENERGIZE, true);
                    /*if (caster->HasAura(103985) || caster->HasAura(115069))
                        if (caster->GetPower(POWER_ENERGY) > 20)
                            caster->EnergizeBySpell(caster, GetSpellInfo()->Id, -20, POWER_ENERGY);
                        else
                            caster->CastStop();*/
                }
            }

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (eventInfo.GetActor()->GetGUID() != GetTarget()->GetGUID())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (GetTarget()->HasAura(aurEff->GetSpellInfo()->Id, _player->GetGUID()))
                    {
                        if (!_player->HasSpellCooldown(SPELL_MONK_CRACKLING_JADE_SHOCK_BUMP))
                        {
                            _player->CastSpell(GetTarget(), SPELL_MONK_CRACKLING_JADE_SHOCK_BUMP, true);
                            _player->AddSpellCooldown(SPELL_MONK_CRACKLING_JADE_SHOCK_BUMP, 0, 8 * IN_MILLISECONDS);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_crackling_jade_lightning_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectProc += AuraEffectProcFn(spell_monk_crackling_jade_lightning_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_crackling_jade_lightning_AuraScript();
        }
};

// Touch of Karma - 122470
class spell_monk_touch_of_karma : public SpellScriptLoader
{
    public:
        spell_monk_touch_of_karma() : SpellScriptLoader("spell_monk_touch_of_karma") { }

        class spell_monk_touch_of_karma_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_touch_of_karma_AuraScript);

            uint64 targetGuid;

            bool Load()
            {
                targetGuid = 0;
                return true;
            }

            void SetGuid(uint32 type, uint64 data)
            {
                targetGuid = data;
            }

            bool CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (GetCaster())
                {
                    amount = GetCaster()->GetMaxHealth();
                    return true;
                }
                return false;
            }

            void OnAbsorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if (!GetCaster())
                    return;

                Unit* caster = GetCaster();

                if (Unit* target = sObjectAccessor->GetUnit(*caster, targetGuid))
                {
                    if (!target->HasAura(SPELL_MONK_TOUCH_OF_KARMA))
                    {
                        absorbAmount = 0;
                        Remove();
                        return;
                    }

                    int32 bp = dmgInfo.GetDamage();
                    bp /= 6;
                    bp += target->GetRemainingPeriodicAmount(caster->GetGUID(), SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE, SPELL_AURA_PERIODIC_DAMAGE);
                    caster->CastCustomSpell(target, SPELL_MONK_TOUCH_OF_KARMA_REDIRECT_DAMAGE, &bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_touch_of_karma_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_touch_of_karma_AuraScript::OnAbsorb, EFFECT_1, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_touch_of_karma_AuraScript();
        }
};

// Spinning Fire Blossom - 123408
class spell_monk_spinning_fire_blossom_damage : public SpellScriptLoader
{
    public:
        spell_monk_spinning_fire_blossom_damage() : SpellScriptLoader("spell_monk_spinning_fire_blossom_damage") { }

        class spell_monk_spinning_fire_blossom_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_spinning_fire_blossom_damage_SpellScript);

            SpellCastResult CheckTarget()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetExplTargetUnit())
                        if (_player->IsFriendlyTo(target))
                            return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetDistance(_player) > 10.0f)
                        {
                            SetHitDamage(int32(GetHitDamage() * 1.5f));
                            _player->CastSpell(target, SPELL_MONK_SPINNING_FIRE_BLOSSOM_ROOT, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_spinning_fire_blossom_damage_SpellScript::CheckTarget);
                OnHit += SpellHitFn(spell_monk_spinning_fire_blossom_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_spinning_fire_blossom_damage_SpellScript();
        }
};

// Spinning Fire Blossom - 115073
class spell_monk_spinning_fire_blossom : public SpellScriptLoader
{
    public:
        spell_monk_spinning_fire_blossom() : SpellScriptLoader("spell_monk_spinning_fire_blossom") { }

        class spell_monk_spinning_fire_blossom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_spinning_fire_blossom_SpellScript)

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    std::list<Unit*> tempList;
                    std::list<Unit*> targetList;

                    _player->GetAttackableUnitListInRange(tempList, 50.0f);

                    for (auto itr : tempList)
                    {
                        if (!_player->IsValidAttackTarget(itr))
                            continue;

                        if (!_player->isInFront(itr))
                            continue;

                        if (!itr->IsWithinLOSInMap(_player))
                            continue;

                        if (itr->GetGUID() == _player->GetGUID())
                            continue;

                        targetList.push_back(itr);
                    }

                    if (!targetList.empty())
                    {
                        JadeCore::Containers::RandomResizeList(targetList, 1);

                        for (auto itr : targetList)
                            _player->CastSpell(itr, SPELL_MONK_SPINNING_FIRE_BLOSSOM_DAMAGE, true);
                    }
                    else
                        _player->CastSpell(_player, SPELL_MONK_SPINNING_FIRE_BLOSSOM_MISSILE, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_spinning_fire_blossom_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_spinning_fire_blossom_SpellScript();
        }
};

// Path of Blossom - 124336
class spell_monk_path_of_blossom : public SpellScriptLoader
{
    public:
        spell_monk_path_of_blossom() : SpellScriptLoader("spell_monk_path_of_blossom") { }

        class spell_monk_path_of_blossom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_path_of_blossom_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (GetCaster())
                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_PATH_OF_BLOSSOM_AREATRIGGER, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_path_of_blossom_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_path_of_blossom_AuraScript();
        }
};

// Called by Uplift - 116670 and Uplift - 130316
// Thunder Focus Tea - 116680
class spell_monk_thunder_focus_tea : public SpellScriptLoader
{
    public:
        spell_monk_thunder_focus_tea() : SpellScriptLoader("spell_monk_thunder_focus_tea") { }

        class spell_monk_thunder_focus_tea_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_thunder_focus_tea_SpellScript)

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                hasTea = false;
                if (Aura* aur = GetCaster()->GetAura(SPELL_MONK_THUNDER_FOCUS_TEA))
                {
                    hasTea = true;
                    aur->Remove();
                }
                unitList.remove_if(JadeCore::UnitAuraCheck(false, SPELL_MONK_RENEWING_MIST_HOT, GetCaster()->GetGUID()));
            }

            void HandleOnHit(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                Unit* target = GetHitUnit();
                
                if (hasTea)
                    if (Aura* aur = target->GetAura(SPELL_MONK_RENEWING_MIST_HOT, GetCaster()->GetGUID()))
                        aur->RefreshDuration();
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_thunder_focus_tea_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_monk_thunder_focus_tea_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }

        private:

            bool hasTea;

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_thunder_focus_tea_SpellScript();
        }
};

// Called by Spinning Crane Kick - 107270 and Rushing Jade Wind - 148187
// Teachings of the Monastery - 116645
class spell_monk_teachings_of_the_monastery : public SpellScriptLoader
{
    public:
        spell_monk_teachings_of_the_monastery() : SpellScriptLoader("spell_monk_teachings_of_the_monastery") { }

        class spell_monk_teachings_of_the_monastery_SpellScript : public SpellScript
        {
    public:
            spell_monk_teachings_of_the_monastery_SpellScript()
                : m_targetCount(0)
            {
            }

            PrepareSpellScript(spell_monk_teachings_of_the_monastery_SpellScript);

            uint32 m_targetCount;

            void HandleAfterCast()
            {
                if (auto caster = GetCaster())
                {
                    if (caster->HasAura(118672))
                        caster->CastSpell(caster, SPELL_MONK_SPINNING_CRANE_KICK_HEAL, true);

                    if (auto player = caster->ToPlayer())
                        if (player->GetSpecializationId() == SPEC_MONK_MISTWEAVER && m_targetCount >= 3)
                            if (!player->HasAura(SPELL_MONK_MUSCLE_MEMORY_EFFECT))
                                player->AddAura(SPELL_MONK_MUSCLE_MEMORY_EFFECT, player);
                }
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                m_targetCount = targets.size();
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_teachings_of_the_monastery_SpellScript::HandleAfterCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_teachings_of_the_monastery_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_teachings_of_the_monastery_SpellScript();
        }
};

// Mana Tea - 115294
class spell_monk_mana_tea : public SpellScriptLoader
{
    public:
        spell_monk_mana_tea() : SpellScriptLoader("spell_monk_mana_tea") { }

        class spell_monk_mana_tea_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_mana_tea_SpellScript);

            SpellModifier* spellMod;

            void HandleBeforeCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    int32 stacks = 0;

                    if (Aura* manaTeaStacks = _player->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                        stacks = manaTeaStacks->GetStackAmount();

                    int32 newDuration = (stacks * IN_MILLISECONDS) / 2;

                    spellMod = new SpellModifier();
                    spellMod->op = SPELLMOD_DURATION;
                    spellMod->type = SPELLMOD_FLAT;
                    spellMod->spellId = SPELL_MONK_MANA_TEA_REGEN;
                    spellMod->value = newDuration;
                    spellMod->mask[1] = 0x200000;
                    spellMod->mask[2] = 0x1;

                    _player->AddSpellMod(spellMod, true);
                }
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    _player->AddSpellMod(spellMod, false);
                    if (AuraApplication* aura = _player->GetAuraApplication(SPELL_MONK_MANA_TEA_REGEN))
                    {
                        Aura* manaTea = aura->GetBase();
                        int32 newDuration = manaTea->GetDuration() - 1000;
                        manaTea->SetDuration(newDuration);
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_monk_mana_tea_SpellScript::HandleBeforeCast);
                AfterCast += SpellCastFn(spell_monk_mana_tea_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_mana_tea_SpellScript();
        }

        class spell_monk_mana_tea_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_mana_tea_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (GetCaster())
                {
                    // remove one charge per tick instead of remove aura on cast
                    // "Cancelling the channel will not waste stacks"
                    if (Aura* manaTea = GetCaster()->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                    {
                        if (manaTea->GetStackAmount() > 1)
                            manaTea->SetStackAmount(manaTea->GetStackAmount() - 1);
                        else
                            GetCaster()->RemoveAura(SPELL_MONK_MANA_TEA_STACKS);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_mana_tea_AuraScript::OnTick, EFFECT_0, SPELL_AURA_OBS_MOD_POWER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_mana_tea_AuraScript();
        }
};

// Brewing : Mana Tea - 123766
class spell_monk_mana_tea_stacks : public SpellScriptLoader
{
    public:
        spell_monk_mana_tea_stacks() : SpellScriptLoader("spell_monk_mana_tea_stacks") { }

        class spell_monk_mana_tea_stacks_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_mana_tea_stacks_AuraScript);

            uint32 chiConsumed;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                chiConsumed = 0;
            }

            void SetData(uint32 type, uint32 data)
            {
                while ((chiConsumed += data) >= 4)
                {
                    chiConsumed = 0;
                    data = data > 4 ? data - 4: 0;

                    if (GetCaster())
                    {
                        GetCaster()->CastSpell(GetCaster(), SPELL_MONK_MANA_TEA_STACKS, true);
                        GetCaster()->CastSpell(GetCaster(), SPELL_MONK_PLUS_ONE_MANA_TEA, true);
                    }
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_monk_mana_tea_stacks_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_mana_tea_stacks_AuraScript();
        }
};

// Enveloping Mist - 124682
class spell_monk_enveloping_mist : public SpellScriptLoader
{
    public:
        spell_monk_enveloping_mist() : SpellScriptLoader("spell_monk_enveloping_mist") { }

        class spell_monk_enveloping_mist_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_enveloping_mist_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetExplTargetUnit())
                        _player->CastSpell(target, SPELL_MONK_ENVELOPING_MIST_HEAL, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_enveloping_mist_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_enveloping_mist_SpellScript();
        }
};

// Surging Mist - 116694
class spell_monk_surging_mist : public SpellScriptLoader
{
    public:
        spell_monk_surging_mist() : SpellScriptLoader("spell_monk_surging_mist") { }

        class spell_monk_surging_mist_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_surging_mist_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetExplTargetUnit())
                        _player->CastSpell(target, SPELL_MONK_SURGING_MIST_HEAL, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_surging_mist_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_surging_mist_SpellScript();
        }
};

class spell_monk_renewing_mist : public SpellScriptLoader
{
public:
    spell_monk_renewing_mist() : SpellScriptLoader("spell_monk_renewing_mist") { }

    class spell_monk_renewing_mist_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_renewing_mist_SpellScript);

        void HandleOnHitTarget(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            int32 bp = 2; // to mark first aura

            GetCaster()->CastCustomSpell(GetHitUnit(), SPELL_MONK_RENEWING_MIST_HOT, NULL, &bp, NULL, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_monk_renewing_mist_SpellScript::HandleOnHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_renewing_mist_SpellScript();
    }
};

class spell_monk_renewing_mist_aoe : public SpellScriptLoader
{
public:
    spell_monk_renewing_mist_aoe() : SpellScriptLoader("spell_monk_renewing_mist_aoe") { }

    class spell_monk_renewing_mist_aoe_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_renewing_mist_aoe_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            if (!GetCaster() || !GetOriginalCaster())
                return;

            if (unitList.size() > 0)
                unitList.remove(GetCaster());

            if (unitList.size() > 0)
                unitList.remove_if(TargetCheck(GetOriginalCaster()->GetGUID()));

            if (unitList.size() > 1)
                JadeCore::Containers::RandomResizeList(unitList, 1);
        }

        void HandleOnHitTarget(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetOriginalCaster() || !GetHitUnit())
                return;

            int32 amount = GetSpell()->GetSpellValue(SPELLVALUE_BASE_POINT1);
            if (amount > 0)
            {
                amount--;
                GetOriginalCaster()->CastCustomSpell(GetHitUnit(), SPELL_MONK_RENEWING_MIST_HOT, NULL, &amount, NULL, true);
                GetCaster()->CastSpell(GetHitUnit(), SPELL_MONK_RENEWING_MIST_JUMP_VISUAL, true);
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_renewing_mist_aoe_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            OnEffectHitTarget += SpellEffectFn(spell_monk_renewing_mist_aoe_SpellScript::HandleOnHitTarget, EFFECT_1, SPELL_EFFECT_DUMMY);
        }

    private:

        class TargetCheck
        {
            public:
                TargetCheck(uint64 casterGUID) : _casterGUID(casterGUID) {}

                bool operator()(WorldObject* object) const
                {
                    if (object->GetTypeId() != TYPEID_PLAYER)
                        return true;

                    if (object->ToUnit()->HasAura(SPELL_MONK_RENEWING_MIST_HOT, _casterGUID))
                        return true;

                    return false;
                }

            private:
                uint64 _casterGUID;
        };
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_renewing_mist_aoe_SpellScript();
    }
};

// Renewing Mist - 119611
class spell_monk_renewing_mist_hot : public SpellScriptLoader
{
    public:
        spell_monk_renewing_mist_hot() : SpellScriptLoader("spell_monk_renewing_mist_hot") { }

        class spell_monk_renewing_mist_hot_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_renewing_mist_hot_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                update = 0;

                if (!sSpellMgr->GetSpellInfo(119611))
                    return false;
                return true;
            }

            bool Load()
            {
                spreadCount = 0;

                // first aura with amount=2
                // marked by trigger spell 115151

                return true;
            }

            void OnUpdate(uint32 diff, AuraEffect* aurEff)
            {
                update += diff;

                if (update >= 2500)
                {
                    if (Unit* caster = GetCaster())
                        caster->CastSpell(caster, SPELL_MONK_UPLIFT_ALLOWING_CAST, true);

                    update = 0;
                }
            }

            void OnTick(AuraEffect const* aurEff)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                if (Aura* aur = GetAura())
                    if (AuraEffect* aurEff = aur->GetEffect(EFFECT_1))
                    {
                        int32 amount = aurEff->GetAmount();
                        if (amount > 0 && amount <= 2)
                        {
                            aurEff->SetAmount(0);
                            GetUnitOwner()->CastCustomSpell(GetUnitOwner(), SPELL_MONK_RENEWING_MIST_JUMP_AOE, NULL, &amount, NULL, true, NULL, NULL, GetCaster()->GetGUID());                   
                        }
                    }
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Aura* uplift = caster->GetAura(SPELL_MONK_UPLIFT_ALLOWING_CAST, caster->GetGUID()))
                        uplift->Remove();

                    if (caster->HasAura(SPELL_MONK_ITEM_2_S12_MISTWEAVER))
                    {
                        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                        if (removeMode == AURA_REMOVE_BY_ENEMY_SPELL)
                        {
                            caster->CastSpell(caster, SPELL_MONK_MANA_TEA_STACKS, true);
                            caster->CastSpell(caster, SPELL_MONK_PLUS_ONE_MANA_TEA, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_renewing_mist_hot_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                OnEffectUpdate += AuraEffectUpdateFn(spell_monk_renewing_mist_hot_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                AfterEffectRemove += AuraEffectApplyFn(spell_monk_renewing_mist_hot_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }

        private:
            uint32 update;
            uint8  spreadCount;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_renewing_mist_hot_AuraScript();
        }
};

// Called by : Fortifying Brew - 115203, Chi Brew - 115399, Elusive Brew - 115308, Tigereye Brew - 116740
// Purifying Brew - 119582, Mana Tea - 115294, Thunder Focus Tea - 116680, Energizing Brew - 115288 and Nimble Brew - 137562
// Healing Elixirs - 122280
class spell_monk_healing_elixirs : public SpellScriptLoader
{
    public:
        spell_monk_healing_elixirs() : SpellScriptLoader("spell_monk_healing_elixirs") { }

        class spell_monk_healing_elixirs_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_healing_elixirs_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Aura* healingElixirs = _player->GetAura(SPELL_MONK_HEALING_ELIXIRS_AURA))
                    {
                        int32 bp = 15;

                        if (!_player->IsFullHealth() && _player->HasAura(134563))
                        {
                            _player->CastCustomSpell(_player, SPELL_MONK_HEALING_ELIXIRS_RESTORE_HEALTH, &bp, NULL, NULL, true);
                            _player->RemoveAura(134563);
                            // This effect cannot occur more than once per 18s
                            healingElixirs->GetEffect(0)->ResetPeriodic(true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_healing_elixirs_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_healing_elixirs_SpellScript();
        }
};

// Zen Sphere - 124081
class spell_monk_zen_sphere_hot : public SpellScriptLoader
{
    public:
        spell_monk_zen_sphere_hot() : SpellScriptLoader("spell_monk_zen_sphere_hot") { }

        class spell_monk_zen_sphere_hot_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_zen_sphere_hot_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr)
                    return;

                l_Caster->CastSpell(l_Target, SPELL_MONK_ZEN_SPHERE_DAMAGE, true);
                if (l_Target->GetHealthPct() < 35.0f)
                    l_Target->RemoveAura(GetSpellInfo()->Id, l_Caster->GetGUID());
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes p_Mode)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr)
                    return;

                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();

                if (l_RemoveMode != AURA_REMOVE_BY_CANCEL)
                {
                    l_Caster->CastSpell(l_Target, SPELL_MONK_ZEN_SPHERE_DETONATE_HEAL, true);
                    l_Caster->CastSpell(l_Target, SPELL_MONK_ZEN_SPHERE_DETONATE_DAMAGE, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_zen_sphere_hot_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_monk_zen_sphere_hot_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_zen_sphere_hot_AuraScript();
        }
};

class spell_monk_zen_sphere_damage : public SpellScriptLoader
{
public:
    spell_monk_zen_sphere_damage() : SpellScriptLoader("spell_monk_zen_sphere_damage") { }

    class spell_monk_zen_sphere_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_monk_zen_sphere_damage_SpellScript);

        void FilterTargets(std::list<WorldObject*>& p_Targets)
        {
            Unit* l_Caster = GetCaster();

            p_Targets.remove_if([l_Caster](WorldObject* p_Target) -> bool
            {
                if (p_Target->ToUnit() == nullptr)
                    return true;

                if (!l_Caster->IsValidAttackTarget(p_Target->ToUnit()))
                    return true;

                return false;
            });
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_zen_sphere_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_monk_zen_sphere_damage_SpellScript();
    }
};

// Chi Burst - 123986
class spell_monk_chi_burst : public SpellScriptLoader
{
    public:
        spell_monk_chi_burst() : SpellScriptLoader("spell_monk_chi_burst") { }

        class spell_monk_chi_burst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_burst_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> tempUnitMap;
                        _player->GetAttackableUnitListInRange(tempUnitMap, _player->GetDistance(target));

                        // Chi Burst will always heal the Monk, but not heal twice if Monk targets himself
                        if (target->GetGUID() != _player->GetGUID())
                            _player->CastSpell(_player, SPELL_MONK_CHI_BURST_HEAL, true);

                        if (_player->IsValidAttackTarget(target))
                            _player->CastSpell(target, SPELL_MONK_CHI_BURST_DAMAGE, true);
                        else
                            _player->CastSpell(target, SPELL_MONK_CHI_BURST_HEAL, true);

                        for (auto itr : tempUnitMap)
                        {
                            if (itr->GetGUID() == _player->GetGUID())
                                continue;

                            if (!itr->IsInBetween(_player, target, 3.0f))
                                continue;

                            uint32 spell = _player->IsValidAttackTarget(itr) ? SPELL_MONK_CHI_BURST_DAMAGE : SPELL_MONK_CHI_BURST_HEAL;
                            _player->CastSpell(itr, spell, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_chi_burst_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_burst_SpellScript();
        }
};

// Energizing Brew - 115288
class spell_monk_energizing_brew : public SpellScriptLoader
{
    public:
        spell_monk_energizing_brew() : SpellScriptLoader("spell_monk_energizing_brew") { }

        class spell_monk_energizing_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_energizing_brew_SpellScript);

            SpellCastResult CheckFight()
            {
                if (!GetCaster()->isInCombat())
                    return SPELL_FAILED_CASTER_AURASTATE;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_energizing_brew_SpellScript::CheckFight);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_energizing_brew_SpellScript();
        }
};

// Spear Hand Strike - 116705
class spell_monk_spear_hand_strike : public SpellScriptLoader
{
    public:
        spell_monk_spear_hand_strike() : SpellScriptLoader("spell_monk_spear_hand_strike") { }

        class spell_monk_spear_hand_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_spear_hand_strike_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->isInFront(_player))
                        {
                            _player->CastSpell(target, SPELL_MONK_SPEAR_HAND_STRIKE_SILENCE, true);
                            _player->AddSpellCooldown(116705, 0, 15 * IN_MILLISECONDS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_spear_hand_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_spear_hand_strike_SpellScript();
        }
};

// Tigereye Brew - 116740
class spell_monk_tigereye_brew : public SpellScriptLoader
{
    public:
        spell_monk_tigereye_brew() : SpellScriptLoader("spell_monk_tigereye_brew") { }

        class spell_monk_tigereye_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_tigereye_brew_SpellScript);

            bool Validate()
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_TIGEREYE_BREW))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 stacks = 0;
                        //if (AuraApplication* aura = _player->GetAuraApplication(SPELL_MONK_TIGEREYE_BREW_STACKS, _player->GetGUID()))
                        if (Aura* tigereyeBrewStacks = _player->GetAura(SPELL_MONK_TIGEREYE_BREW_STACKS))
                        {
                            int32 effectAmount = tigereyeBrewStacks->GetStackAmount() * 6;
                            stacks = tigereyeBrewStacks->GetStackAmount();

                            if (stacks >= 10)
                                effectAmount = 60;

                            AuraApplication* tigereyeBrew = _player->GetAuraApplication(SPELL_MONK_TIGEREYE_BREW, _player->GetGUID());
                            if (tigereyeBrew)
                            {
                                tigereyeBrew->GetBase()->GetEffect(0)->ChangeAmount(effectAmount);
                                tigereyeBrew->GetBase()->GetEffect(1)->ChangeAmount(effectAmount);
                            }

                            if (stacks >= 10)
                                tigereyeBrewStacks->SetStackAmount(stacks - 10);
                            else
                                _player->RemoveAura(SPELL_MONK_TIGEREYE_BREW_STACKS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_tigereye_brew_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_tigereye_brew_SpellScript();
        }
};

// Tiger's Lust - 116841
class spell_monk_tigers_lust : public SpellScriptLoader
{
    public:
        spell_monk_tigers_lust() : SpellScriptLoader("spell_monk_tigers_lust") { }

        class spell_monk_tigers_lust_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_tigers_lust_SpellScript);

            bool Validate()
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_FLYING_SERPENT_KICK_NEW))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        target->RemoveMovementImpairingAuras();
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_tigers_lust_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_tigers_lust_SpellScript();
        }
};

// Flying Serpent Kick - 115057
class spell_monk_flying_serpent_kick : public SpellScriptLoader
{
    public:
        spell_monk_flying_serpent_kick() : SpellScriptLoader("spell_monk_flying_serpent_kick") { }

        class spell_monk_flying_serpent_kick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_flying_serpent_kick_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_FLYING_SERPENT_KICK_NEW))
                    return false;
                return true;
            }

            void HandleOnCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (_player->HasAura(SPELL_MONK_FLYING_SERPENT_KICK))
                            _player->RemoveAura(SPELL_MONK_FLYING_SERPENT_KICK);

                        if (caster->HasAura(SPELL_MONK_ITEM_PVP_GLOVES_BONUS))
                            caster->RemoveAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);

                        _player->CastSpell(_player, SPELL_MONK_FLYING_SERPENT_KICK_AOE, true);
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_monk_flying_serpent_kick_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_flying_serpent_kick_SpellScript();
        }
};

// Chi Torpedo - 115008 or Chi Torpedo (3 charges) - 121828
class spell_monk_chi_torpedo : public SpellScriptLoader
{
    public:
        spell_monk_chi_torpedo() : SpellScriptLoader("spell_monk_chi_torpedo") { }

        class spell_monk_chi_torpedo_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_torpedo_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        std::list<Unit*> tempUnitMap;
                        _player->GetAttackableUnitListInRange(tempUnitMap, 20.0f);

                        for (auto itr : tempUnitMap)
                        {
                            if (!itr->isInFront(_player, M_PI / 3) && itr->GetGUID() != _player->GetGUID())
                                continue;

                            uint32 spell = _player->IsValidAttackTarget(itr) ? SPELL_MONK_CHI_TORPEDO_DAMAGE : SPELL_MONK_CHI_TORPEDO_HEAL;
                            _player->CastSpell(itr, spell, true);
                        }

                        if (caster->HasAura(SPELL_MONK_ITEM_PVP_GLOVES_BONUS))
                            caster->RemoveAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_monk_chi_torpedo_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_torpedo_SpellScript();
        }
};

// Purifying Brew - 119582
class spell_monk_purifying_brew : public SpellScriptLoader
{
    public:
        spell_monk_purifying_brew() : SpellScriptLoader("spell_monk_purifying_brew") { }

        class spell_monk_purifying_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_purifying_brew_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        const static uint32 staggerEffects[4] = { SPELL_MONK_LIGHT_STAGGER, SPELL_MONK_MODERATE_STAGGER, SPELL_MONK_HEAVY_STAGGER, SPELL_MONK_DAMAGE_STAGGER };

                        for (uint32 spellId : staggerEffects)
                        {
                            if (_player->HasAura(spellId))
                                _player->RemoveAura(spellId);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_purifying_brew_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_purifying_brew_SpellScript();
        }
};

// Clash - 122057 and Clash - 126449
class spell_monk_clash : public SpellScriptLoader
{
    public:
        spell_monk_clash() : SpellScriptLoader("spell_monk_clash") { }

        class spell_monk_clash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_clash_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (Unit* target = GetHitUnit())
                        {
                            int32 basePoint = 2;
                            _player->CastCustomSpell(target, SPELL_MONK_CLASH_CHARGE, &basePoint, NULL, NULL, true);
                            target->CastSpell(_player, SPELL_MONK_CLASH_CHARGE, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_clash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_clash_SpellScript();
        }
};

// Keg Smash - 121253
class spell_monk_keg_smash : public SpellScriptLoader
{
    public:
        spell_monk_keg_smash() : SpellScriptLoader("spell_monk_keg_smash") { }

        class spell_monk_keg_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_keg_smash_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (Unit* target = GetHitUnit())
                        {
                            _player->CastSpell(target, SPELL_MONK_KEG_SMASH_VISUAL, true);
                            _player->CastSpell(target, SPELL_MONK_WEAKENED_BLOWS, true);
                            _player->CastSpell(_player, SPELL_MONK_KEG_SMASH_ENERGIZE, true);
                            // Prevent to receive 2 CHI more than once time per cast
                            _player->AddSpellCooldown(SPELL_MONK_KEG_SMASH_ENERGIZE, 0, 1 * IN_MILLISECONDS);
                            _player->CastSpell(target, SPELL_MONK_DIZZYING_HAZE, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_keg_smash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_keg_smash_SpellScript();
        }
};

// Elusive Brew - 115308
class spell_monk_elusive_brew : public SpellScriptLoader
{
    public:
        spell_monk_elusive_brew() : SpellScriptLoader("spell_monk_elusive_brew") { }

        class spell_monk_elusive_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_elusive_brew_SpellScript);

            void HandleOnHit()
            {
                int32 stackAmount = 0;

                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (AuraApplication* brewStacks = _player->GetAuraApplication(SPELL_MONK_ELUSIVE_BREW_STACKS))
                            stackAmount = brewStacks->GetBase()->GetStackAmount();

                        _player->AddAura(SPELL_MONK_ELUSIVE_BREW, _player);

                        if (AuraApplication* aura = _player->GetAuraApplication(SPELL_MONK_ELUSIVE_BREW))
                        {
                            Aura* elusiveBrew = aura->GetBase();
                            int32 maxDuration = elusiveBrew->GetMaxDuration();
                            int32 newDuration = stackAmount * 1000;
                            elusiveBrew->SetDuration(newDuration);

                            if (newDuration > maxDuration)
                                elusiveBrew->SetMaxDuration(newDuration);

                            _player->RemoveAura(SPELL_MONK_ELUSIVE_BREW_STACKS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_elusive_brew_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_elusive_brew_SpellScript();
        }
};

// Breath of Fire - 115181
class spell_monk_breath_of_fire : public SpellScriptLoader
{
    public:
        spell_monk_breath_of_fire() : SpellScriptLoader("spell_monk_breath_of_fire") { }

        class spell_monk_breath_of_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_breath_of_fire_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (Unit* target = GetHitUnit())
                        {
                            // if Dizzying Haze is on the target, they will burn for an additionnal damage over 8s
                            if (target->HasAura(SPELL_MONK_DIZZYING_HAZE))
                            {
                                _player->CastSpell(target, SPELL_MONK_BREATH_OF_FIRE_DOT, true);

                                // Glyph of Breath of Fire
                                if (_player->HasAura(123394))
                                    _player->CastSpell(target, SPELL_MONK_BREATH_OF_FIRE_CONFUSED, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_monk_breath_of_fire_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_breath_of_fire_SpellScript();
        }
};

// Soothing Mist - 115175
class spell_monk_soothing_mist : public SpellScriptLoader
{
    public:
        spell_monk_soothing_mist() : SpellScriptLoader("spell_monk_soothing_mist") { }

        class spell_monk_soothing_mist_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_soothing_mist_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        l_Target->CastSpell(l_Target, SPELL_MONK_SOOTHING_MIST_VISUAL, true);

                        Creature* l_SerpentStatue = nullptr;
                        for (Unit* l_Controlled : l_Caster->m_Controlled)
                        {
                            if (l_Controlled->GetEntry() == MONK_NPC_JADE_SERPENT_STATUE && l_Controlled->GetOwnerGUID() == l_Caster->GetGUID())
                            {
                                l_SerpentStatue = l_Controlled->ToCreature();
                                break;
                            }
                        }

                        if (l_SerpentStatue)
                        {
                            std::list<Unit*> l_PartyMembers;
                            l_Caster->GetPartyMembers(l_PartyMembers);

                            if (!l_PartyMembers.empty())
                            {
                                l_PartyMembers.sort(JadeCore::HealthPctOrderPred());

                                l_SerpentStatue->CastSpell(l_PartyMembers.front(), GetSpellInfo()->Id, true);
                            }
                        }
                    }
                }
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (!caster->IsPlayer())
                        return;

                    if (Unit* target = GetTarget())
                    {
                        // 30% to give 1 chi per tick
                        if (roll_chance_i(30))
                            caster->CastSpell(caster, SPELL_MONK_SOOTHING_MIST_ENERGIZE, true);
                    }
                }
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        l_Target->RemoveAura(SPELL_MONK_SOOTHING_MIST_VISUAL);

                        Creature* l_SerpentStatue = nullptr;
                        for (Unit* l_Controlled : l_Caster->m_Controlled)
                        {
                            if (l_Controlled->GetEntry() == MONK_NPC_JADE_SERPENT_STATUE && l_Controlled->GetOwnerGUID() == l_Caster->GetGUID())
                            {
                                l_SerpentStatue = l_Controlled->ToCreature();
                                break;
                            }
                        }

                        if (l_SerpentStatue)
                            l_SerpentStatue->CastStop();
                    }
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_monk_soothing_mist_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_soothing_mist_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_monk_soothing_mist_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_soothing_mist_AuraScript();
        }
};

// Disable - 116095
class spell_monk_disable : public SpellScriptLoader
{
    public:
        spell_monk_disable() : SpellScriptLoader("spell_monk_disable") { }

        class spell_monk_disable_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_disable_SpellScript);

            bool snaredOnHit;

            SpellCastResult CheckCast()
            {
                snaredOnHit = false;

                if (GetCaster())
                    if (Unit* target = GetCaster()->getVictim())
                        if (target->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED))
                            snaredOnHit = true;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Player* _player = caster->ToPlayer())
                        if (Unit* target = GetHitUnit())
                            if (snaredOnHit)
                                _player->CastSpell(target, SPELL_MONK_DISABLE_ROOT, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_disable_SpellScript::CheckCast);
                OnHit += SpellHitFn(spell_monk_disable_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_disable_SpellScript();
        }

        class spell_monk_disable_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_disable_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (!GetCaster())
                    return;

                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        if (target->GetDistance(caster) < 10.0f)
                        {
                            if (Aura* disable = target->GetAura(SPELL_MONK_DISABLE, caster->GetGUID()))
                            {
                                if (disable->GetDuration() <= 500)
                                    aurEff->GetBase()->RefreshDuration();
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_monk_disable_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_disable_AuraScript();
        }
};

// Zen Pilgrimage - 126892 and Zen Pilgrimage : Return - 126895
class spell_monk_zen_pilgrimage : public SpellScriptLoader
{
    public:
        spell_monk_zen_pilgrimage() : SpellScriptLoader("spell_monk_zen_pilgrimage") { }

        class spell_monk_zen_pilgrimage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_zen_pilgrimage_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ZEN_PILGRIMAGE) || !sSpellMgr->GetSpellInfo(SPELL_MONK_ZEN_PILGRIMAGE_RETURN))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Player* _player = caster->ToPlayer())
                    {
                        if (GetSpellInfo()->Id == SPELL_MONK_ZEN_PILGRIMAGE)
                        {
                            _player->SaveRecallPosition();
                            _player->TeleportTo(870, 3818.55f, 1793.18f, 950.35f, _player->GetOrientation());
                        }
                        else if (GetSpellInfo()->Id == SPELL_MONK_ZEN_PILGRIMAGE_RETURN)
                        {
                            _player->TeleportTo(_player->m_recallMap, _player->m_recallX, _player->m_recallY, _player->m_recallZ, _player->m_recallO);
                            _player->RemoveAura(126896);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_zen_pilgrimage_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_zen_pilgrimage_SpellScript();
        }
};

// Blackout Kick - 100784
class spell_monk_blackout_kick : public SpellScriptLoader
{
    public:
        spell_monk_blackout_kick() : SpellScriptLoader("spell_monk_blackout_kick") { }

        class spell_monk_blackout_kick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_blackout_kick_SpellScript);

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Combo Breaker: Blackout Kick
                        if (caster->HasAura(SPELL_MONK_COMBO_BREAKER_2))
                            caster->RemoveAura(SPELL_MONK_COMBO_BREAKER_2);

                        // Second effect by spec : Instant heal or DoT
                        if (caster->GetTypeId() == TYPEID_PLAYER && caster->ToPlayer()->GetSpecializationId(caster->ToPlayer()->GetActiveSpec()) == SPEC_MONK_WINDWALKER
                            && caster->ToPlayer()->HasAura(128595))
                        {
                            // Your Blackout Kick always deals 20% additional damage over 4 sec regardless of positioning but you're unable to trigger the healing effect.
                            if (caster->HasAura(SPELL_MONK_GLYPH_OF_BLACKOUT_KICK))
                            {
                                int32 bp = int32(GetHitDamage() * 0.2f) / 4;
                                caster->CastCustomSpell(target, SPELL_MONK_BLACKOUT_KICK_DOT, &bp, NULL, NULL, true);
                            }
                            else
                            {
                                // If behind : 20% damage on DoT
                                if (target->isInBack(caster))
                                {
                                    int32 bp = int32(GetHitDamage() * 0.2f) / 4;
                                    caster->CastCustomSpell(target, SPELL_MONK_BLACKOUT_KICK_DOT, &bp, NULL, NULL, true);
                                }
                                // else : 20% damage on instant heal
                                else
                                {
                                    int32 bp = int32(GetHitDamage() * 0.2f);
                                    caster->CastCustomSpell(caster, SPELL_MONK_BLACKOUT_KICK_HEAL, &bp, NULL, NULL, true);
                                }
                            }
                        }
                        // Brewmaster : Training - you gain Shuffle, increasing parry chance and stagger amount by 20%
                        else if (caster->GetTypeId() == TYPEID_PLAYER && caster->ToPlayer()->GetSpecializationId(caster->ToPlayer()->GetActiveSpec()) == SPEC_MONK_BREWMASTER)
                        {
                            if (Aura* aura = caster->GetAura(SPELL_MONK_SHUFFLE))
                            {
                                aura->SetMaxDuration(aura->GetMaxDuration() + 6 * IN_MILLISECONDS);
                                aura->SetDuration(aura->GetDuration() + 6 * IN_MILLISECONDS);
                            }
                            else
                                caster->CastSpell(caster, SPELL_MONK_SHUFFLE, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_blackout_kick_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_blackout_kick_SpellScript();
        }
};

// Provoke - 115546
class spell_monk_provoke : public SpellScriptLoader
{
    public:
        spell_monk_provoke() : SpellScriptLoader("spell_monk_provoke") { }

        class spell_monk_provoke_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_provoke_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster)
                {
                    Unit* target = GetExplTargetUnit();
                    if (!target)
                        return SPELL_FAILED_NO_VALID_TARGETS;
                    else if (target->GetTypeId() == TYPEID_PLAYER)
                        return SPELL_FAILED_BAD_TARGETS;
                    else if (!target->IsWithinLOSInMap(GetCaster()))
                        return SPELL_FAILED_LINE_OF_SIGHT;
                    else if (target->GetEntry() != MONK_NPC_BLACK_OX_STATUE && !caster->IsValidAttackTarget(target))
                        return SPELL_FAILED_TARGET_FRIENDLY;
                }
                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetEntry() == MONK_NPC_BLACK_OX_STATUE && target->GetOwnerGUID() == caster->GetGUID())
                            target->CastSpell(target, SPELL_MONK_PROVOKE_AREA, true);
                        else
                            caster->CastSpell(target, SPELL_MONK_PROVOKE, true);
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_provoke_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_provoke_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_provoke_SpellScript();
        }
};

// Paralysis - 115078
class spell_monk_paralysis : public SpellScriptLoader
{
    public:
        spell_monk_paralysis() : SpellScriptLoader("spell_monk_paralysis") { }

        class spell_monk_paralysis_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_paralysis_SpellScript);

            void HandleBeforeHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        if (caster->HasAura(MONK_GLYPH_OF_PARALYSIS))
                        {
                            target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
                            target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
                            target->RemoveAurasByType(SPELL_AURA_PERIODIC_LEECH);
                        }
                    }
                }
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->isInBack(caster))
                        {
                            if (AuraApplication* aura = target->GetAuraApplication(115078))
                            {
                                Aura* Paralysis = aura->GetBase();
                                int32 maxDuration = Paralysis->GetMaxDuration();
                                int32 newDuration = (int32)(maxDuration * 1.5f);
                                Paralysis->SetDuration(newDuration);

                                if (newDuration > maxDuration)
                                    Paralysis->SetMaxDuration(newDuration);
                            }
                        }
                        
                        if (target->ToPlayer())
                        {
                            if (AuraApplication* aura = target->GetAuraApplication(115078))
                            {
                                Aura* Paralysis = aura->GetBase();
                                int32 maxDuration = Paralysis->GetMaxDuration();
                                int32 newDuration = maxDuration / 2;
                                Paralysis->SetDuration(newDuration);
                                Paralysis->SetMaxDuration(newDuration);
                            }                            
                        }
                    }
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_monk_paralysis_SpellScript::HandleBeforeHit);
                OnHit += SpellHitFn(spell_monk_paralysis_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_paralysis_SpellScript();
        }
};

// Touch of Death - 115080
class spell_monk_touch_of_death : public SpellScriptLoader
{
    public:
        spell_monk_touch_of_death() : SpellScriptLoader("spell_monk_touch_of_death") { }

        class spell_monk_touch_of_death_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_touch_of_death_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetExplTargetUnit())
                    {
                        if (l_Target->GetTypeId() == TYPEID_UNIT)
                        {
                            if (l_Target->ToCreature()->IsDungeonBoss() || l_Target->GetHealth() > l_Caster->GetHealth())
                                return SPELL_FAILED_BAD_TARGETS;
                        }
                        else
                        {
                            if (l_Target->GetOwner() && l_Target->GetOwner()->IsPlayer() || l_Target->IsPlayer())
                            {
                                if (l_Target->GetHealthPct() > 10.0f || !l_Caster->HasAura(124490))
                                    return SPELL_FAILED_BAD_TARGETS;
                            }
                        }

                        return SPELL_CAST_OK;
                    }
                }

                return SPELL_FAILED_NO_VALID_TARGETS;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_touch_of_death_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_touch_of_death_SpellScript();
        }
};

// Fortifying brew - 115203
class spell_monk_fortifying_brew : public SpellScriptLoader
{
    public:
        spell_monk_fortifying_brew() : SpellScriptLoader("spell_monk_fortifying_brew") { }

        class spell_monk_fortifying_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_fortifying_brew_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (caster && caster->GetTypeId() == TYPEID_PLAYER)
                    caster->CastSpell(caster, SPELL_MONK_FORTIFYING_BREW, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_fortifying_brew_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_fortifying_brew_SpellScript();
        }
};

// Legacy of the Emperor - 115921
class spell_monk_legacy_of_the_emperor : public SpellScriptLoader
{
    public:
        spell_monk_legacy_of_the_emperor() : SpellScriptLoader("spell_monk_legacy_of_the_emperor") { }

        class spell_monk_legacy_of_the_emperor_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_legacy_of_the_emperor_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* plr = GetCaster()->ToPlayer())
                {
                    std::list<Unit*> groupList;

                    plr->GetPartyMembers(groupList, true);

                    if (!groupList.empty())
                    {
                        for (auto itr : groupList)
                        {
                            if (!plr)
                                continue;

                            if (!itr)
                                continue;

                            plr->CastSpell(itr, SPELL_MONK_LEGACY_OF_THE_EMPEROR, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_legacy_of_the_emperor_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_legacy_of_the_emperor_SpellScript();
        }
};

// Roll - 109132 or Roll (3 charges) - 121827
class spell_monk_roll : public SpellScriptLoader
{
    public:
        spell_monk_roll() : SpellScriptLoader("spell_monk_roll") { }

        class spell_monk_roll_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_roll_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ROLL))
                    return false;
                return true;
            }

            void HandleBeforeCast()
            {
                Aura* aur = GetCaster()->AddAura(SPELL_MONK_ROLL_TRIGGER, GetCaster());
                if (!aur)
                    return;

                AuraApplication* app =  aur->GetApplicationOfTarget(GetCaster()->GetGUID());
                if (!app)
                    return;

                app->ClientUpdate();
            }

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                caster->CastSpell(caster, SPELL_MONK_ROLL_TRIGGER, true);

                if (caster->HasAura(SPELL_MONK_ITEM_PVP_GLOVES_BONUS))
                    caster->RemoveAurasByType(SPELL_AURA_MOD_DECREASE_SPEED);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_monk_roll_SpellScript::HandleBeforeCast);
                AfterCast += SpellCastFn(spell_monk_roll_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_roll_SpellScript();
        }
};

// Brewing : Tigereye Brew - 123980
class spell_monk_tigereye_brew_stacks : public SpellScriptLoader
{
    public:
        spell_monk_tigereye_brew_stacks() : SpellScriptLoader("spell_monk_tigereye_brew_stacks") { }

        class spell_monk_tigereye_brew_stacks_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_tigereye_brew_stacks_AuraScript);

            uint32 chiConsumed;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                chiConsumed = 0;
            }

            void SetData(uint32 type, uint32 data)
            {
                while ((chiConsumed += data) >= 4)
                {
                    chiConsumed = 0;
                    data = data > 4 ? data - 4: 0;

                    if (GetCaster())
                        GetCaster()->CastSpell(GetCaster(), SPELL_MONK_TIGEREYE_BREW_STACKS, true);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_monk_tigereye_brew_stacks_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_tigereye_brew_stacks_AuraScript();
        }
};

// Called by 100780 / 108557 / 115698 / 115687 / 115693 / 115695 - Jab (and overrides)
// Muscle Memory - 139598
class spell_monk_muscle_memory : public SpellScriptLoader
{
    public:
        spell_monk_muscle_memory() : SpellScriptLoader("spell_monk_muscle_memory") { }

        class spell_monk_muscle_memory_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_muscle_memory_SpellScript)

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_MONK_MISTWEAVER)
                            _player->AddAura(SPELL_MONK_MUSCLE_MEMORY_EFFECT,_player);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_muscle_memory_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_muscle_memory_SpellScript();
        }
};

// Chi Brew - 115399
class spell_monk_chi_brew : public SpellScriptLoader
{
    public:
        spell_monk_chi_brew() : SpellScriptLoader("spell_monk_chi_brew") { }

        class spell_monk_chi_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_brew_SpellScript);

            bool Validate()
            {
                if (!sSpellMgr->GetSpellInfo(115399))
                    return false;

                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 stacks = 0;
                        switch (_player->GetSpecializationId(_player->GetActiveSpec()))
                        {
                            case SPEC_MONK_BREWMASTER:
                                if (Aura* elusiveBrewStacks = _player->GetAura(SPELL_MONK_ELUSIVE_BREW_STACKS))
                                {
                                    int32 maxDuration = elusiveBrewStacks->GetMaxDuration();
                                    stacks = elusiveBrewStacks->GetStackAmount();
                                    elusiveBrewStacks->SetStackAmount(stacks + 5);
                                    if (_player->HasAura(115636))
                                    {
                                        for (int i = 1; i <= 5; i++)
                                        {
                                            // Mastery: Bottled Fury
                                            if (roll_chance_i(20))
                                                _player->CastSpell(_player, SPELL_MONK_TIGEREYE_BREW_STACKS, true); 
                                        }
                                    }
                                    elusiveBrewStacks->SetDuration(maxDuration);
                                    // to prevent overstack
                                    if (elusiveBrewStacks->GetStackAmount() > 15)
                                        elusiveBrewStacks->SetStackAmount(15);
                                }
                                else
                                {
                                    _player->CastSpell(_player, SPELL_MONK_ELUSIVE_BREW_STACKS, true);
                                    _player->CastSpell(_player, SPELL_MONK_ELUSIVE_BREW_STACKS, true);
                                }
                                break;
                            case SPEC_MONK_MISTWEAVER:
                                if (_player->HasAura(115070)) // allow to add mana tea stacks only in Stance of the Wise Serpent
                                {
                                    if (Aura* manaTeaStacks = _player->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                                    {
                                        int32 maxDuration = manaTeaStacks->GetMaxDuration();
                                        stacks = manaTeaStacks->GetStackAmount();
                                        manaTeaStacks->SetStackAmount(stacks + 2);
                                        for (int i = 1; i <= 2; i++)
                                        {
                                            float crit_chance = 0.0f;
                                            crit_chance += GetCaster()->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + SPELL_SCHOOL_MASK_NORMAL);
                                            if (roll_chance_i(crit_chance))
                                            {
                                                if (GetCaster())
                                                {
                                                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_MANA_TEA_STACKS, true);
                                                    GetCaster()->CastSpell(GetCaster(), SPELL_MONK_PLUS_ONE_MANA_TEA, true);
                                                }
                                            }
                                        }
                                        manaTeaStacks->SetDuration(maxDuration);
                                    // to prevent overstack
                                    if (manaTeaStacks->GetStackAmount() > 20)
                                        manaTeaStacks->SetStackAmount(20);
                                    }
                                    else
                                    {
                                        _player->CastSpell(_player, SPELL_MONK_MANA_TEA_STACKS, true);
                                        _player->CastSpell(_player, SPELL_MONK_MANA_TEA_STACKS, true);
                                    }
                                }
                                break;
                            case SPEC_MONK_WINDWALKER:
                                if (Aura* tigereyeBrewStacks = _player->GetAura(SPELL_MONK_TIGEREYE_BREW_STACKS))
                                {
                                    int32 maxDuration = tigereyeBrewStacks->GetMaxDuration();
                                    stacks = tigereyeBrewStacks->GetStackAmount();
                                    tigereyeBrewStacks->SetStackAmount(stacks + 2);
                                    tigereyeBrewStacks->SetDuration(maxDuration);
                                    // Mastery: Bottled Fury
                                    if (_player->HasAura(115636))
                                    {
                                        float Mastery = _player->GetFloatValue(PLAYER_MASTERY) * 2.5f;
                                        for (int i = 1; i <= 2; i++)
                                        {
                                            if (roll_chance_f(Mastery))
                                                _player->CastSpell(_player, SPELL_MONK_TIGEREYE_BREW_STACKS, true);
                                        }
                                    }
                                    // to prevent overstack
                                    if (tigereyeBrewStacks->GetStackAmount() > 20)
                                        tigereyeBrewStacks->SetStackAmount(20);
                                }
                                else
                                {
                                    _player->CastSpell(_player, SPELL_MONK_TIGEREYE_BREW_STACKS, true);
                                    _player->CastSpell(_player, SPELL_MONK_TIGEREYE_BREW_STACKS, true);
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_chi_brew_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_ENERGIZE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_chi_brew_SpellScript();
        }
};

// Called by Brewing: Tigereye Brew - 125195
// Mastery: Bottled Fury - 115636
class spell_monk_mastery_bottled_fury : public SpellScriptLoader
{
    public:
        spell_monk_mastery_bottled_fury() : SpellScriptLoader("spell_monk_mastery_bottled_fury") { }

        class spell_monk_mastery_bottled_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_mastery_bottled_fury_SpellScript)

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* plr = GetCaster()->ToPlayer())
                {
                    // Mastery: Bottled Fury
                    if (plr->HasAura(115636) && roll_chance_i(20))
                        plr->CastSpell(plr, SPELL_MONK_TIGEREYE_BREW_STACKS, true); 
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_mastery_bottled_fury_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_mastery_bottled_fury_SpellScript();
        }
};

// Brewing: Mana Tea passiv bonus - 115867
class spell_monk_brewing_mana_tea_passiv : public SpellScriptLoader
{
    public:
        spell_monk_brewing_mana_tea_passiv() : SpellScriptLoader("spell_monk_brewing_mana_tea_passiv") { }

        class spell_monk_brewing_mana_tea_passiv_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_brewing_mana_tea_passiv_SpellScript)

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                if (Player* plr = GetCaster()->ToPlayer())
                {
                    // Brewing: Mana Tea passiv bonus
                    float crit_chance = 0.0f;
                    crit_chance += GetCaster()->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + SPELL_SCHOOL_MASK_NORMAL);

                    // To prevent a crash
                    if (Aura* manaTeaStacks = plr->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                        if (manaTeaStacks->GetStackAmount() >= 20)
                            return;

                    if (roll_chance_i(crit_chance))
                    {
                        if (plr)
                        {
                            plr->CastSpell(plr, SPELL_MONK_MANA_TEA_STACKS, true);
                            plr->CastSpell(plr, SPELL_MONK_PLUS_ONE_MANA_TEA, true);
                        }
                    }  
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_brewing_mana_tea_passiv_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_brewing_mana_tea_passiv_SpellScript();
        }
};

// Called by Mana Tea (instant) - 123761
// Glyph of Mana Tea - 123763
class spell_monk_glyph_of_mana_tea : public SpellScriptLoader
{
    public:
        spell_monk_glyph_of_mana_tea() : SpellScriptLoader("spell_monk_glyph_of_mana_tea") { }

        class spell_monk_glyph_of_mana_tea_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_glyph_of_mana_tea_SpellScript)

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                        if (_player->HasAura(SPELL_MONK_GLYPH_OF_MANA_TEA))
                        {
                            if (Aura* manaTeaStacks = _player->GetAura(SPELL_MONK_MANA_TEA_STACKS))
                                {
                                    int32 maxDuration = manaTeaStacks->GetMaxDuration();
                                    int32 stacks = manaTeaStacks->GetStackAmount();
                                    if (stacks > 2)
                                    {
                                        manaTeaStacks->SetStackAmount(stacks - 2);
                                        manaTeaStacks->SetDuration(maxDuration);
                                    }
                                    else
                                        _player->RemoveAura(manaTeaStacks);
                                }
                        }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_monk_glyph_of_mana_tea_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_glyph_of_mana_tea_SpellScript();
        }
};

// Called by 100780 / 108557 / 115698 / 115687 / 115693 / 115695 - Jab (and overrides)
// 137384 - Combo Breaker
class spell_combo_breaker : public SpellScriptLoader
{
    public:
        spell_combo_breaker() : SpellScriptLoader("spell_combo_breaker") { }

        class spell_combo_breaker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_combo_breaker_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (caster->GetTypeId() == TYPEID_PLAYER && caster->HasAura(137384))
                        {
                            if (roll_chance_i(12))
                            {
                                if (roll_chance_i(50))
                                    caster->CastSpell(caster, SPELL_MONK_COMBO_BREAKER_1, true);
                                else
                                    caster->CastSpell(caster, SPELL_MONK_COMBO_BREAKER_2, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_combo_breaker_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_combo_breaker_SpellScript();
        }
};

// Glyph: Leer of the Ox
class spell_monk_leer_of_the_ox : public SpellScriptLoader
{
    public:
        spell_monk_leer_of_the_ox() : SpellScriptLoader("spell_monk_leer_of_the_ox") { }

        class spell_monk_leer_of_the_ox_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_leer_of_the_ox_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(SPELL_MONK_LEER_OF_THE_OX, false);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(SPELL_MONK_LEER_OF_THE_OX))
                        _player->removeSpell(SPELL_MONK_LEER_OF_THE_OX, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_monk_leer_of_the_ox_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_monk_leer_of_the_ox_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_monk_leer_of_the_ox_AuraScript();
        }
};

// Life Cocoon - 116849
// Glyph of Life Cocoon - 124989
class spell_monk_life_cocoon : public SpellScriptLoader
{
    public:
        spell_monk_life_cocoon() : SpellScriptLoader("spell_monk_life_cocoon") { }

        class spell_monk_life_cocoon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_life_cocoon_SpellScript);

            SpellCastResult CheckTarget()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAuraType(SPELL_AURA_MOD_STUN) && !_player->HasAura(SPELL_MONK_GLYPH_OF_LIFE_COCOON))
                        return SPELL_FAILED_STUNNED;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_life_cocoon_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_life_cocoon_SpellScript();
        }
};

// Stance of the Wise Serpent - 115070
class spell_monk_stance_of_the_wise_serpent : public SpellScriptLoader
{
    public:
        spell_monk_stance_of_the_wise_serpent() : SpellScriptLoader("spell_monk_stance_of_the_wise_serpent") { }

        class spell_monk_stance_of_the_wise_serpent_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_stance_of_the_wise_serpent_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetTarget())
                    l_Caster->CastSpell(l_Caster, SPELL_MONK_EMINENCE_AURA, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetTarget())
                    l_Caster->RemoveAura(SPELL_MONK_EMINENCE_AURA);
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_monk_stance_of_the_wise_serpent_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_monk_stance_of_the_wise_serpent_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_monk_stance_of_the_wise_serpent_AuraScript();
        }
};

// Avert Harm - 115213
class spell_monk_avert_harm : public SpellScriptLoader
{
    public:
        spell_monk_avert_harm() : SpellScriptLoader("spell_monk_avert_harm") { }

        class spell_monk_avert_harm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_avert_harm_AuraScript);

            void PreventEffect(AuraEffect* /*aurEff*/, DamageInfo& /*dmgInfo*/, uint32& /*absorbAmount*/)
            {
                PreventDefaultAction();
            }

            void Register() override
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_avert_harm_AuraScript::PreventEffect, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_monk_avert_harm_AuraScript();
        }
};

// Leer of the Lox - 115543
class spell_monk_leer_of_the_lox : public SpellScriptLoader
{
    public:
        spell_monk_leer_of_the_lox() : SpellScriptLoader("spell_monk_leer_of_the_lox") { }

        class spell_monk_leer_of_the_lox_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_leer_of_the_lox_SpellScript);

            /// To prevent searching in the grid multiple times
            uint64 m_OxStatueGUID;

            bool Load() override
            {
                m_OxStatueGUID = 0;
                return true;
            }

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetExplTargetUnit())
                    {
                        if (!l_Caster->IsValidAttackTarget(l_Target))
                            return SPELL_FAILED_TARGET_FRIENDLY;

                        Creature* l_OxStatue = nullptr;
                        for (Unit* l_Controlled : l_Caster->m_Controlled)
                        {
                            if (l_Controlled->GetEntry() == MONK_NPC_BLACK_OX_STATUE && l_Controlled->GetOwnerGUID() == l_Caster->GetGUID())
                            {
                                l_OxStatue = l_Controlled->ToCreature();
                                break;
                            }
                        }

                        if (!l_OxStatue)
                            return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW; ///< Assume the caster has no Ox Statue

                        if (!l_Caster->HasAura(SPELL_MONK_LEER_OF_THE_OX_GLYPH))
                            return SPELL_FAILED_INVALID_GLYPH;

                        if (!l_OxStatue->IsWithinLOSInMap(l_Target))
                            return SPELL_FAILED_LINE_OF_SIGHT;

                        m_OxStatueGUID = l_OxStatue->GetGUID();
                    }
                    else
                        return SPELL_FAILED_NO_VALID_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void HandleEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (Creature* l_OxStatue = l_Target->GetMap()->GetCreature(m_OxStatueGUID))
                        l_OxStatue->CastSpell(l_Target, SPELL_MONK_LEER_OF_THE_OX_EFFECT, true);
                }
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_leer_of_the_lox_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_monk_leer_of_the_lox_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_leer_of_the_lox_SpellScript();
        }
};

// Gift of the serpent - 135920
// Gift of the serpent - 124041
class spell_monk_gift_of_the_serpent_explode : public SpellScriptLoader
{
    public:
        spell_monk_gift_of_the_serpent_explode() : SpellScriptLoader("spell_monk_gift_of_the_serpent_explode") { }

        class spell_monk_gift_of_the_serpent_explode_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_gift_of_the_serpent_explode_SpellScript);

            enum eSpells
            {
                T16Mistweaver2PBonus        = 145439,
                T16Mistweaver2PBonusShield  = 145441
            };

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster      = GetCaster();
                Unit* l_Target      = GetHitUnit();
                int32 l_Absorb     = 0;
                int32 l_Heal       = GetHitHeal();

                if (l_Target == nullptr || l_Heal <= 0)
                    return;

                if (!l_Caster->HasAura(eSpells::T16Mistweaver2PBonus))
                    return;

                l_Absorb = l_Heal;
                if (l_Heal + l_Target->GetHealth() > l_Target->GetMaxHealth())
                    l_Absorb = l_Heal - (l_Heal + l_Target->GetHealth() - l_Target->GetMaxHealth());

                if (l_Absorb > 0)
                {
                    if (AuraEffect* l_AuraEffect = l_Caster->GetAuraEffect(eSpells::T16Mistweaver2PBonus, EFFECT_0))
                        l_Absorb = CalculatePct(l_Absorb, l_AuraEffect->GetAmount());
                    if (AuraEffect* l_PreviousShield = l_Caster->GetAuraEffect(eSpells::T16Mistweaver2PBonusShield, EFFECT_0))
                        l_Absorb += l_PreviousShield->GetAmount();
                    l_Caster->CastCustomSpell(l_Target, eSpells::T16Mistweaver2PBonusShield, &l_Absorb, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_monk_gift_of_the_serpent_explode_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_monk_gift_of_the_serpent_explode_SpellScript();
        }
};

/// Nimble Brew - 137562
class spell_monk_nimble_brew : public SpellScriptLoader
{
    public:
        spell_monk_nimble_brew() : SpellScriptLoader("spell_monk_nimble_brew") { }

        class spell_monk_nimble_brew_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_nimble_brew_SpellScript);

            int m_DoesDispell; // Ternary

            void CheckAura()
            {
                if (Unit* caster = GetCaster())
                    m_DoesDispell = (caster->HasAura(146952)) ? 0 : -1;
                else
                    m_DoesDispell = -1;
            }

            void CheckDispell(SpellEffIndex p_EffIndex)
            {
                if (m_DoesDispell != 0)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo const* l_SpellInfo = GetSpellInfo();

                    Mechanics l_DispelMechanics = Mechanics(l_SpellInfo->Effects[p_EffIndex].MiscValue);
                    uint32 l_DispelMask = 1 << l_DispelMechanics;

                    Unit::AuraMap const& auras = l_Caster->GetOwnedAuras();
                    for (auto l_It = auras.cbegin(); l_It != auras.cend(); ++l_It)
                    {
                        const Aura* aura = l_It->second;
                        // don't try to remove passive auras
                        if (aura->IsPassive())
                            continue;

                        AuraApplication const* aurApp = aura->GetApplicationOfTarget(l_Caster->GetGUID());
                        if (!aurApp)
                            continue;

                        if (aura->GetSpellInfo()->GetAllEffectsMechanicMask() & l_DispelMask)
                        {
                            // do not remove positive auras if friendly target
                            //               negative auras if non-friendly target
                            if (aurApp->IsPositive())
                                continue;

                            // The charges / stack amounts don't count towards the total number of auras that can be dispelled.
                            // Ie: A dispel on a target with 5 stacks of Winters Chill and a Polymorph has 1 / (1 + 1) -> 50% chance to dispell
                            // Polymorph instead of 1 / (5 + 1) -> 16%.
                            bool dispel_charges = aura->GetSpellInfo()->AttributesEx7 & SPELL_ATTR7_DISPEL_CHARGES;
                            uint8 charges = dispel_charges ? aura->GetCharges() : aura->GetStackAmount();
                            if (charges > 0)
                                m_DoesDispell = 1;
                        }
                    }
                }
            }

            void HandleHeal(SpellEffIndex p_EffIndex)
            {
                if (m_DoesDispell != 1)
                    PreventHitEffect(p_EffIndex);
            }

            void Register() override
            {
                OnCast += SpellCastFn(spell_monk_nimble_brew_SpellScript::CheckAura);
                OnEffectLaunchTarget += SpellEffectFn(spell_monk_nimble_brew_SpellScript::CheckDispell, EFFECT_0, SPELL_EFFECT_DISPEL_MECHANIC);
                OnEffectLaunchTarget += SpellEffectFn(spell_monk_nimble_brew_SpellScript::CheckDispell, EFFECT_1, SPELL_EFFECT_DISPEL_MECHANIC);
                OnEffectLaunchTarget += SpellEffectFn(spell_monk_nimble_brew_SpellScript::CheckDispell, EFFECT_2, SPELL_EFFECT_DISPEL_MECHANIC);
                OnEffectLaunchTarget += SpellEffectFn(spell_monk_nimble_brew_SpellScript::CheckDispell, EFFECT_6, SPELL_EFFECT_DISPEL_MECHANIC);
                OnEffectLaunchTarget += SpellEffectFn(spell_monk_nimble_brew_SpellScript::CheckDispell, EFFECT_9, SPELL_EFFECT_DISPEL_MECHANIC);
                OnEffectHitTarget    += SpellEffectFn(spell_monk_nimble_brew_SpellScript::HandleHeal,   EFFECT_8, SPELL_EFFECT_HEAL_PCT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_nimble_brew_SpellScript();
        }
};

class spell_area_monk_healing_sphere : public SpellAreaTriggerScript
{
public:
    spell_area_monk_healing_sphere() : SpellAreaTriggerScript("spell_area_monk_healing_sphere") {}

    bool OnAddTarget(AreaTrigger* trigger, Unit* target) override
    {
        auto caster = trigger->GetCaster();
        if (!caster || !target || !caster->ToPlayer())
            return false;

        // Skip if target is creature, but not a pet
        if (target->ToCreature() && !target->ToPet())
            return false;

        // skip not valid assist targets
        if (!caster->_IsValidAssistTarget(target, sSpellMgr->GetSpellInfo(115464)))
            return false;

        if (trigger->GetDuration() > 0)
        {
            caster->CastSpell(target, 115464, true);
            trigger->Remove(0);
        }

        return false;
    }

    void OnRemove(AreaTrigger* trigger, bool expired) override
    {
        if (!expired)
            return;

        if (auto caster = trigger->GetCaster())
        {
            Position pos;
            trigger->GetPosition(&pos);

            caster->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 135920, true);
        }
    }
};

class spell_area_monk_healing_sphere_afterlife : public SpellAreaTriggerScript
{
public:
    spell_area_monk_healing_sphere_afterlife() : SpellAreaTriggerScript("spell_area_monk_healing_sphere_afterlife") {}

    bool OnAddTarget(AreaTrigger* trigger, Unit* target)
    {
        if (!trigger->GetCaster())
            return false;

        if (trigger->GetCasterGUID() != target->GetGUID())
            return false;

        if (trigger->GetDuration() > 0)
        {
            trigger->GetCaster()->CastSpell(target, 125355, true);
            trigger->SetDuration(0);
        }
        return false;
    }
};

class spell_area_monk_gift_of_the_serpent : public SpellAreaTriggerScript
{
public:
    spell_area_monk_gift_of_the_serpent() : SpellAreaTriggerScript("spell_area_monk_gift_of_the_serpent") {}

    bool OnAddTarget(AreaTrigger* trigger, Unit* target)
    {
        Unit* caster = trigger->GetCaster();
        if (!trigger->GetCaster())
            return false;

        // skip not valid assist targets
        if (!caster->_IsValidAssistTarget(target, sSpellMgr->GetSpellInfo(124041)))
            return false;

        if (trigger->GetDuration() > 0)
        {
            trigger->GetCaster()->CastSpell(target, 124041, true);
            trigger->Remove(false);
        }
        return false;
    }
};

class spell_area_monk_chi_sphere_afterlife : public SpellAreaTriggerScript
{
public:
    spell_area_monk_chi_sphere_afterlife() : SpellAreaTriggerScript("spell_area_monk_chi_sphere_afterlife") {}

    bool OnAddTarget(AreaTrigger* trigger, Unit* target)
    {
        if (!trigger->GetCaster())
            return false;

        if (trigger->GetCasterGUID() != target->GetGUID())
            return false;

        if (trigger->GetDuration() > 0)
        {
            trigger->GetCaster()->CastSpell(target, 121283, true);
            trigger->SetDuration(0);
        }
        return false;
    }
};

class spell_area_monk_path_of_blossom : public SpellAreaTriggerScript
{
public:
    spell_area_monk_path_of_blossom() : SpellAreaTriggerScript("spell_area_monk_path_of_blossom") {}

    bool OnAddTarget(AreaTrigger* trigger, Unit* target)
    {
        if (!trigger->GetCaster())
            return false;

        if (!target->isTargetableForAttack())
            return false;

        if (trigger->GetDuration() > 0)
        {
            trigger->GetCaster()->CastSpell(target, 122036, true);
            trigger->SetDuration(0);
        }
        return false;
    }
};

class spell_area_monk_gift_of_the_ox_1 : public SpellAreaTriggerScript
{
public:
    spell_area_monk_gift_of_the_ox_1() : SpellAreaTriggerScript("spell_area_monk_gift_of_the_ox_1") {}

    bool OnAddTarget(AreaTrigger* trigger, Unit* target)
    {
        if (!trigger->GetCaster())
            return false;

        if (trigger->GetCasterGUID() != target->GetGUID())
            return false;

        if (trigger->GetDuration() > 0)
        {
            trigger->GetCaster()->CastSpell(target, 124507, true);
            trigger->SetDuration(0);
        }
        return false;
    }
};

class spell_area_monk_gift_of_the_ox_2 : public SpellAreaTriggerScript
{
public:
    spell_area_monk_gift_of_the_ox_2() : SpellAreaTriggerScript("spell_area_monk_gift_of_the_ox_2") {}

    bool OnAddTarget(AreaTrigger* trigger, Unit* target)
    {
        if (!trigger->GetCaster())
            return false;

        if (trigger->GetCasterGUID() != target->GetGUID())
            return false;

        if (trigger->GetDuration() > 0)
        {
            trigger->GetCaster()->CastSpell(target, 124507, true);
            trigger->SetDuration(0);
        }
        return false;
    }
};

void AddSC_monk_spell_scripts()
{
    new spell_monk_ring_of_peace_dummy();
    new spell_monk_ring_of_peace();
    new spell_monk_storm_earth_and_fire_stats();
    new spell_monk_storm_earth_and_fire();
    new spell_monk_fists_of_fury_stun();
    new spell_monk_expel_harm();
    new spell_monk_chi_wave_healing_bolt();
    new spell_monk_chi_wave_bolt();
    new spell_monk_chi_wave();
    new spell_monk_chi_wave_aoe();
    new spell_monk_grapple_weapon();
    new spell_monk_transcendence_transfer();
    new spell_monk_dampen_harm();
    new spell_monk_item_s12_4p_mistweaver();
    new spell_monk_diffuse_magic();
    new spell_monk_black_ox_statue();
    new spell_monk_guard();
    new spell_monk_bear_hug();
    new spell_monk_zen_flight_check();
    new spell_monk_glyph_of_zen_flight();
    new spell_monk_power_strikes();
    new spell_monk_crackling_jade_lightning();
    new spell_monk_touch_of_karma();
    new spell_monk_spinning_fire_blossom_damage();
    new spell_monk_spinning_fire_blossom();
    new spell_monk_path_of_blossom();
    new spell_monk_thunder_focus_tea();
    new spell_monk_teachings_of_the_monastery();
    new spell_monk_mana_tea();
    new spell_monk_mana_tea_stacks();
    new spell_monk_enveloping_mist();
    new spell_monk_surging_mist();
    new spell_monk_renewing_mist();
    new spell_monk_renewing_mist_aoe();
    new spell_monk_renewing_mist_hot();
    new spell_monk_healing_elixirs();
    new spell_monk_zen_sphere_hot();
    new spell_monk_chi_burst();
    new spell_monk_energizing_brew();
    new spell_monk_spear_hand_strike();
    new spell_monk_tigereye_brew();
    new spell_monk_tigers_lust();
    new spell_monk_flying_serpent_kick();
    new spell_monk_chi_torpedo();
    new spell_monk_purifying_brew();
    new spell_monk_clash();
    new spell_monk_keg_smash();
    new spell_monk_elusive_brew();
    new spell_monk_breath_of_fire();
    new spell_monk_soothing_mist();
    new spell_monk_disable();
    new spell_monk_zen_pilgrimage();
    new spell_monk_blackout_kick();
    new spell_monk_legacy_of_the_emperor();
    new spell_monk_fortifying_brew();
    new spell_monk_touch_of_death();
    new spell_monk_paralysis();
    new spell_monk_provoke();
    new spell_monk_roll();
    new spell_monk_tigereye_brew_stacks();
    new spell_monk_muscle_memory();
    new spell_monk_chi_brew();
    new spell_monk_mastery_bottled_fury();
    new spell_monk_brewing_mana_tea_passiv();
    new spell_monk_glyph_of_mana_tea();
    new spell_combo_breaker();
    new spell_monk_leer_of_the_ox();
    new spell_monk_life_cocoon();
    new spell_monk_stance_of_the_wise_serpent();
    new spell_monk_avert_harm();
    new spell_monk_leer_of_the_lox();
    new spell_monk_gift_of_the_serpent_explode();
    new spell_monk_nimble_brew();

    new spell_area_monk_healing_sphere();
    new spell_area_monk_healing_sphere_afterlife();
    new spell_area_monk_gift_of_the_serpent();
    new spell_area_monk_chi_sphere_afterlife();
    new spell_area_monk_path_of_blossom();
    new spell_area_monk_gift_of_the_ox_1();
    new spell_area_monk_gift_of_the_ox_2();
    new spell_monk_zen_sphere_damage();
}

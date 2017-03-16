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
 * Scripts for spells with SPELLFAMILY_SHAMAN and SPELLFAMILY_GENERIC spells used by shaman players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_sha_".
 */

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "GridNotifiers.h"
#include "Unit.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum ShamanSpells
{
    SHAMAN_SPELL_SATED                      = 57724,
    SHAMAN_SPELL_EXHAUSTION                 = 57723,
    HUNTER_SPELL_INSANITY                   = 95809,
    MAGE_SPELL_TEMPORAL_DISPLACEMENT        = 80354,
    SPELL_SHA_LIGHTNING_SHIELD_AURA         = 324,
    SPELL_SHA_ASCENDANCE_ELEMENTAL	         = 114050,
    SPELL_SHA_ASCENDANCE_RESTORATION        = 114052,
    SPELL_SHA_ASCENDANCE_ENHANCED	         = 114051,
    SPELL_SHA_ASCENDANCE			  = 114049,
    SPELL_SHA_HEALING_RAIN                  = 142923,
    SPELL_SHA_HEALING_RAIN_TICK             = 73921,
    SPELL_SHA_HEALING_RAIN_AURA             = 73920,
    SPELL_SHA_EARTHQUAKE                    = 61882,
    SPELL_SHA_EARTHQUAKE_TICK               = 77478,
    SPELL_SHA_EARTHQUAKE_KNOCKING_DOWN      = 77505,
    SPELL_SHA_ELEMENTAL_BLAST               = 117014,
    SPELL_SHA_ELEMENTAL_BLAST_RATING_BONUS  = 118522,
    SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL = 118517,
    SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL  = 118515,
    SPELL_SHA_LAVA_LASH                     = 60103,
    SPELL_SHA_FLAME_SHOCK                   = 8050,
    SPELL_SHA_STORMSTRIKE                   = 17364,
    SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE   = 26364,
    SPELL_SHA_HEALING_STREAM                = 52042,
    SPELL_SHA_GLYPH_OF_HEALING_STREAM       = 119523,
    SPELL_SHA_FULMINATION                   = 88766,
    SPELL_SHA_FULMINATION_TRIGGERED         = 88767,
    SPELL_SHA_FULMINATION_INFO              = 95774,
    SPELL_SHA_ROLLING_THUNDER_AURA          = 88764,
    SPELL_SHA_ROLLING_THUNDER_ENERGIZE      = 88765,
    SPELL_SHA_UNLEASH_ELEMENTS              = 73680,
    SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE    = 77661,
    SPELL_SHA_FIRE_NOVA                     = 1535,
    SPELL_SHA_FIRE_NOVA_TRIGGERED           = 131786,
    SPELL_SHA_TIDAL_WAVES                   = 51564,
    SPELL_SHA_TIDAL_WAVES_PROC              = 53390,
    SPELL_SHA_MANA_TIDE                     = 16191,
    SPELL_SHA_FROST_SHOCK_FREEZE            = 63685,
    SPELL_SHA_FROZEN_POWER                  = 63374,
    SPELL_SHA_MAIL_SPECIALIZATION_AGI       = 86099,
    SPELL_SHA_MAIL_SPECIALISATION_INT       = 86100,
    SPELL_SHA_UNLEASHED_FURY_TALENT         = 117012,
    SPELL_SHA_UNLEASHED_FURY_FLAMETONGUE    = 118470,
    SPELL_SHA_UNLEASHED_FURY_WINDFURY       = 118472,
    SPELL_SHA_UNLEASHED_FURY_EARTHLIVING    = 118473,
    SPELL_SHA_UNLEASHED_FURY_FROSTBRAND     = 118474,
    SPELL_SHA_UNLEASHED_FURY_ROCKBITER      = 118475,
    SPELL_SHA_STONE_BULWARK_ABSORB          = 114893,
    SPELL_SHA_EARTHGRAB_IMMUNITY            = 116946,
    SPELL_SHA_EARTHBIND_FOR_EARTHGRAB_TOTEM = 116947,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS          = 108283,
    SPELL_SHA_ANCESTRAL_GUIDANCE            = 114911,
    SPELL_SHA_CONDUCTIVITY_TALENT           = 108282,
    SPELL_SHA_CONDUCTIVITY_HEAL             = 118800,
    SPELL_SHA_GLYPH_OF_LAKESTRIDER          = 55448,
    SPELL_SHA_WATER_WALKING                 = 546,
    SPELL_SHA_GLYPH_OF_SHAMANISTIC_RAGE     = 63280,
    SPELL_SHA_SOLAR_BEAM                    = 113286,
    SPELL_SHA_SOLAR_BEAM_SILENCE            = 113288,
    SPELL_SHA_GHOST_WOLF                    = 2645,
    SPELL_SHA_ITEM_T14_4P                   = 123124,
    SPELL_SHA_GLYPH_OF_HEALING_STREAM_TOTEM = 55456,
    SPELL_SHA_LAVA_BURST                    = 51505,
    SPELL_SHA_ITEM_S12_4P_ENHANCEMENT_BONUS = 131554,
    SPELL_SHA_RESTORATIVE_MISTS             = 114083,
    SPELL_SHA_GLYPH_OF_TELLURIC_CURRENTS    = 55453,
    SPELL_SHA_LAVA_SURGE                    = 77762,
    SHAMAN_ITEM_T16_ELEMENTAL_2P_BONUS      = 144998,
    SPELL_SHA_ELEMENTAL_DISCHARGE           = 144999,
    GLYPH_SHA_FAR_SIGHT                     = 58059
};

// Totemic Projection - 108287
class spell_sha_totemic_projection : public SpellScriptLoader
{
    public:
        spell_sha_totemic_projection() : SpellScriptLoader("spell_sha_totemic_projection") { }

        class spell_sha_totemic_projection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_totemic_projection_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Creature*> leapList;
                    Unit* leapTarget = NULL;

                    caster->GetCreatureListWithEntryInGrid(leapList, 47319, 45.0f);

                    for (auto itr : leapList)
                    {
                        if (!itr->ToUnit() || !itr->ToTempSummon())
                            continue;

                        if (!itr->ToTempSummon()->GetSummoner())
                            continue;

                        if (itr->ToTempSummon()->GetSummoner()->GetGUID() != caster->GetGUID())
                            continue;

                        leapTarget = itr->ToUnit();
                        break;
                    }

                    if (!leapTarget)
                        return;

                    for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                    {
                        if ((*itr)->isTotem())
                        {
                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo((*itr)->GetUInt32Value(UNIT_CREATED_BY_SPELL));
                            if (!spellInfo)
                                continue;

                            float objSize = caster->GetObjectSize();
                            float angle = spellInfo->Effects[0].TargetA.CalcDirectionAngle();
                            float dist = spellInfo->Effects[0].CalcRadius(caster);

                            if (dist < objSize)
                                dist = objSize;

                            Position pos;
                            leapTarget->GetPosition(&pos);

                            angle += (*itr)->GetOrientation();
                            float destx, desty, destz, ground, floor;
                            destx = pos.m_positionX + dist * std::cos(angle);
                            desty = pos.m_positionY + dist * std::sin(angle);
                            ground = (*itr)->GetMap()->GetHeight((*itr)->GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
                            floor = (*itr)->GetMap()->GetHeight((*itr)->GetPhaseMask(), destx, desty, pos.m_positionZ, true);
                            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

                            (*itr)->NearTeleportTo(destx, desty, destz, (*itr)->GetOrientation());
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_totemic_projection_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_totemic_projection_SpellScript();
        }
};

// Hex - 51514
class spell_sha_hex : public SpellScriptLoader
{
    public:
        spell_sha_hex() : SpellScriptLoader("spell_sha_hex") { }

        class spell_sha_hex_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_hex_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (target->IsMounted())
                    {
                        target->Dismount();
                        target->RemoveAurasByType(SPELL_AURA_MOUNTED);
                    }

                    if (target->HasUnitState(UNIT_STATE_CASTING))
                    {
                        target->InterruptSpell(CURRENT_GENERIC_SPELL);
                        target->InterruptSpell(CURRENT_CHANNELED_SPELL);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sha_hex_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_hex_AuraScript();
        }
};

// Water Ascendant - 114052
class spell_sha_water_ascendant : public SpellScriptLoader
{
    public:
        spell_sha_water_ascendant() : SpellScriptLoader("spell_sha_water_ascendant") { }

        class spell_sha_water_ascendant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_water_ascendant_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return;

                if (_player->HasSpellCooldown(SPELL_SHA_RESTORATIVE_MISTS))
                    return;

                if (eventInfo.GetActor()->GetGUID() != _player->GetGUID())
                    return;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_RESTORATIVE_MISTS)
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_ANCESTRAL_GUIDANCE)
                    return;

                if (!(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetSpellInfo()->IsPositive()))
                    return;

                if (Unit* target = eventInfo.GetActionTarget())
                {
                    std::list<Unit*> tempList;
                    std::list<Unit*> alliesList;
                    target->GetAttackableUnitListInRange(tempList, 15.0f);

                    for (auto itr : tempList)
                    {
                        if (!_player->IsWithinLOSInMap(itr))
                            continue;

                        if (itr->IsHostileTo(_player))
                            continue;

                        alliesList.push_back(itr);
                    }

                    if (!alliesList.empty())
                    {
                        // Heal amount distribued for all allies, caster included
                        int32 bp = eventInfo.GetHealInfo()->GetHeal() / alliesList.size();

                        if (bp > 0)
                            _player->CastCustomSpell((*alliesList.begin()), SPELL_SHA_RESTORATIVE_MISTS, &bp, NULL, NULL, true);   // Restorative Mists

                        _player->AddSpellCooldown(SPELL_SHA_RESTORATIVE_MISTS, 0, 1 * IN_MILLISECONDS);               // This prevent from multiple procs
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_water_ascendant_AuraScript::OnProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_water_ascendant_AuraScript();
        }
};

// Prowl - 113289
class spell_sha_prowl : public SpellScriptLoader
{
    public:
        spell_sha_prowl() : SpellScriptLoader("spell_sha_prowl") { }

        class spell_sha_prowl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_prowl_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, SPELL_SHA_GHOST_WOLF, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_prowl_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_prowl_SpellScript();
        }
};

// Solar beam - 113286
class spell_sha_solar_beam : public SpellScriptLoader
{
    public:
        spell_sha_solar_beam() : SpellScriptLoader("spell_sha_solar_beam") { }

        class spell_sha_solar_beam_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_solar_beam_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_SHA_SOLAR_BEAM))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHA_SOLAR_BEAM_SILENCE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_solar_beam_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_solar_beam_AuraScript();
        }
};

// Called by Shamanistic Rage - 30823
// Glyph of Shamanistic Rage - 63280
class spell_sha_glyph_of_shamanistic_rage : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_shamanistic_rage() : SpellScriptLoader("spell_sha_glyph_of_shamanistic_rage") { }

        class spell_sha_glyph_of_shamanistic_rage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_shamanistic_rage_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_SHA_GLYPH_OF_SHAMANISTIC_RAGE))
                    {
                        DispelChargesList dispelList;
                        _player->GetDispellableAuraList(_player, DISPEL_ALL_MASK, dispelList);
                        if (!dispelList.empty())
                        {
                            for (auto itr : dispelList)
                            {
                                // Hack for Unstable Affliction
                                if (itr.first->GetId() == 30108)
                                {
                                    if (Unit* originalCaster = itr.first->GetCaster())
                                    {
                                        if (AuraEffect const* aurEff = _player->GetAuraEffect(30108, EFFECT_0, originalCaster->GetGUID()))
                                        {
                                            int32 damage = aurEff->GetAmount() * 8;
                                            // backfire damage and silence
                                            originalCaster->CastCustomSpell(_player, 31117, &damage, NULL, NULL, true, NULL, aurEff);
                                            if (!_player->HasAura(31117))
                                                originalCaster->AddAura(31117, _player);
                                        }
                                    }
                                }

                                if (_player->HasAura(itr.first->GetId()))
                                    _player->RemoveAura(itr.first);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_shamanistic_rage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_shamanistic_rage_SpellScript();
        }
};

// Called by Ghost Wolf - 2645
// Glyph of Lakestrider - 55448
class spell_sha_glyph_of_lakestrider : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_lakestrider() : SpellScriptLoader("spell_sha_glyph_of_lakestrider") { }

        class spell_sha_glyph_of_lakestrider_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_lakestrider_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(SPELL_SHA_GLYPH_OF_LAKESTRIDER))
                        _player->CastSpell(_player, SPELL_SHA_WATER_WALKING, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_lakestrider_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_lakestrider_SpellScript();
        }
};

// Call of the Elements - 108285
class spell_sha_call_of_the_elements : public SpellScriptLoader
{
    public:
        spell_sha_call_of_the_elements() : SpellScriptLoader("spell_sha_call_of_the_elements") { }

        class spell_sha_call_of_the_elements_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_call_of_the_elements_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    // immediately finishes the cooldown on totems with less than 3min cooldown
                    const SpellCooldowns& cm = _player->GetSpellCooldownMap();
                    for (SpellCooldowns::const_iterator itr = cm.begin(); itr != cm.end();)
                    {
                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);
                        if (!spellInfo)
                            continue;

                        if ((spellInfo->Id == 51485 || 
                            spellInfo->Id == 108273 || 
                            spellInfo->Id == 108270 ||
                            spellInfo->Id == 108269 || 
                            spellInfo->Id == 8143 || 
                            spellInfo->Id == 8177 ||
                            spellInfo->Id == 5394 || 
                            spellInfo->Id == 2484 || 
                            spellInfo->Id == 108273) &&
                             spellInfo->GetRecoveryTime() > 0)
                            _player->RemoveSpellCooldown((itr++)->first, true);
                        else
                            ++itr;
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_call_of_the_elements_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_call_of_the_elements_SpellScript();
        }
};

// Called by Healing Wave - 331, Greater Healing Wave - 77472 and Healing Surge - 8004
// Called by Lightning Bolt - 403, Chain Lightning - 421, Earth Shock - 8042 and Stormstrike - 17364
// Called by Lightning Bolt - 45284, Chain Lightning - 45297, Chain Heal - 1064
/// Conductivity - 108282
class spell_sha_conductivity : public SpellScriptLoader
{
    public:
        spell_sha_conductivity() : SpellScriptLoader("spell_sha_conductivity") { }

        class spell_sha_conductivity_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_conductivity_SpellScript);

            bool m_Effective;
            
            bool Load() override
            {
                m_Effective = true;
                return true;
            }
            
            void HandleAfterHit()
            {
                if (!m_Effective)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (AuraEffect* l_ConductivityEffect = l_Caster->GetAuraEffect(ShamanSpells::SPELL_SHA_CONDUCTIVITY_TALENT, SpellEffIndex::EFFECT_2))
                        {
                            if (DynamicObject* l_RainObj = l_Caster->GetDynObject(ShamanSpells::SPELL_SHA_HEALING_RAIN))
                            {
                                uint32 l_RemainingDuration = l_ConductivityEffect->GetAmount();
                                uint32 l_AddDuration = std::min(l_RemainingDuration, uint32(4000));

                                l_RainObj->SetDuration(l_RainObj->GetDuration() + l_AddDuration);
                                l_ConductivityEffect->SetAmount(l_RemainingDuration - l_AddDuration);

                                if (Aura* l_HealingRain = l_Caster->GetAura(ShamanSpells::SPELL_SHA_HEALING_RAIN_AURA))
                                {
                                    l_HealingRain->SetDuration(l_HealingRain->GetDuration() + l_AddDuration);
                                    l_HealingRain->SetMaxDuration(l_HealingRain->GetMaxDuration() + l_AddDuration);
                                }

                                m_Effective = false;
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_conductivity_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_conductivity_SpellScript();
        }
};

// Ancestral Guidance - 108281
class spell_sha_ancestral_guidance : public SpellScriptLoader
{
    public:
        spell_sha_ancestral_guidance() : SpellScriptLoader("spell_sha_ancestral_guidance") { }

        class spell_sha_ancestral_guidance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_ancestral_guidance_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Unit* _player = GetCaster();
                if (!_player)
                    return;

                if (eventInfo.GetActor()->GetGUID() != _player->GetGUID())
                    return;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_ANCESTRAL_GUIDANCE)
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_RESTORATIVE_MISTS)
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamage()) && !(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE) && !(eventInfo.GetDamageInfo()->GetDamageType() == HEAL))
                    return;

                if (Unit* target = eventInfo.GetActionTarget())
                {
                    int32 damage = eventInfo.GetDamageInfo()->GetDamage();
                    int32 heal = eventInfo.GetHealInfo()->GetHeal();
                    if (!damage && !heal)
                        return;

                    int32 bp;
                    if (damage > heal)
                        bp = CalculatePct(damage, 40);
                    else
                        bp = CalculatePct(heal, 60);

                    _player->CastCustomSpell(target, SPELL_SHA_ANCESTRAL_GUIDANCE, &bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_ancestral_guidance_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_ancestral_guidance_AuraScript();
        }
};

// Ancestral Guidance - 114911 - effect
class spell_sha_ancestral_guidance_heal : public SpellScriptLoader
{
    public:
        spell_sha_ancestral_guidance_heal() : SpellScriptLoader("spell_sha_ancestral_guidance_heal") { }

        class spell_sha_ancestral_guidance_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ancestral_guidance_heal_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ANCESTRAL_GUIDANCE))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                struct 
                {
                    Unit* caster;

                    bool operator() (const WorldObject* target)
                    {
                        if (!caster || !target->ToUnit())
                            return true;

                        Unit const* uTarget = target->ToUnit();
                        if (uTarget->isTotem() || uTarget->isDead())
                            return true;

                        if (!uTarget->IsInPartyWith(caster) && !uTarget->IsInRaidWith(caster))
                            return true;

                        return false;
                     }
                } is_not_valid_heal_target;

                is_not_valid_heal_target.caster = GetCaster();
                targets.remove_if(is_not_valid_heal_target);

                if (targets.size() > 3)
                    targets.resize(3);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_ancestral_guidance_heal_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ancestral_guidance_heal_SpellScript();
        }
};

// Echo of the Elements - 108283
class spell_sha_echo_of_the_elements : public SpellScriptLoader
{
    public:
        spell_sha_echo_of_the_elements() : SpellScriptLoader("spell_sha_echo_of_the_elements") { }

        class spell_sha_echo_of_the_elements_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_echo_of_the_elements_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* _player = GetCaster()->ToPlayer();
                Unit* target = eventInfo.GetActionTarget();
                if (!_player || !target)
                    return;

                if (_player->HasSpellCooldown(SPELL_SHA_ECHO_OF_THE_ELEMENTS))
                    return;

                if (eventInfo.GetActor()->GetGUID() != _player->GetGUID())
                    return;

                Spell const* procSpell = eventInfo.GetSpell();
                if (!procSpell)
                    return;

                SpellInfo const* procSpellInfo = procSpell->GetSpellInfo();
                if (!procSpell)
                    return;

                bool singleTarget = false;
                for (uint8 i = 0; i < procSpellInfo->EffectCount; ++i)
                {
                    if ((procSpellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY ||
                        procSpellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_TARGET_ENEMY) &&
                        procSpellInfo->Effects[i].TargetB.GetTarget() == 0)
                    {    
                        singleTarget = true;
                        break;
                    }
                }

                if (!singleTarget)
                    return;

                int32 chance = 6;

                // devs told that proc chance is 6% for Elemental and Restoration specs and 30% for Enhancement
                if (_player->GetSpecializationId() == SPEC_SHAMAN_ENHANCEMENT && procSpellInfo->Id != SPELL_SHA_ELEMENTAL_BLAST)
                    chance = 30;

                if (!(eventInfo.GetDamageInfo()->GetDamage()) && !(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetDamageType() == SPELL_DIRECT_DAMAGE) && !(eventInfo.GetDamageInfo()->GetDamageType() == HEAL))
                    return;

                if (!roll_chance_i(chance))
                    return;

                _player->AddSpellCooldown(SPELL_SHA_ECHO_OF_THE_ELEMENTS, 0, 6 * IN_MILLISECONDS); // This prevent from multiple procs

                // Duplicate custom values
                CustomSpellValues values;
                for (unsigned int i = 0; i < 6; ++i)
                    values.AddSpellMod(SpellValueMod(SPELLVALUE_BASE_POINT0 + i), procSpell->GetSpellValue(SpellValueMod(SPELLVALUE_BASE_POINT0 + i)));

                values.AddSpellMod(SPELLVALUE_ALWAYS_CRIT, procSpell->IsCritForTarget(target));

                _player->CastCustomSpell(procSpellInfo->Id, values, target, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_echo_of_the_elements_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_echo_of_the_elements_AuraScript();
        }
};

// Earthgrab - 64695
class spell_sha_earthgrab : public SpellScriptLoader
{
    public:
        spell_sha_earthgrab() : SpellScriptLoader("spell_sha_earthgrab") { }

        class spell_sha_earthgrab_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthgrab_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->HasAura(SPELL_SHA_EARTHGRAB_IMMUNITY, caster->GetGUID()))
                            caster->CastSpell(target, SPELL_SHA_EARTHBIND_FOR_EARTHGRAB_TOTEM, true);
                        else
                            caster->CastSpell(target, SPELL_SHA_EARTHGRAB_IMMUNITY, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_earthgrab_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthgrab_SpellScript();
        }
};

// Stone Bulwark - 114889
class spell_sha_stone_bulwark_passive : public SpellScriptLoader
{
public:
    spell_sha_stone_bulwark_passive() : SpellScriptLoader("spell_sha_stone_bulwark_passive") { }

    class spell_sha_stone_bulwark_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_stone_bulwark_AuraScript);

        void OnTick(AuraEffect const* aurEff)
        {

            Unit* caster = aurEff->GetCaster();
            if (!caster || !caster->GetOwner() || !caster->GetOwner()->ToPlayer())
                return;

            bool prevent = aurEff->GetTickNumber() > 1;

            Player* player = caster->GetOwner()->ToPlayer();
            if (auto effect_0 = player->GetAuraEffect(SPELL_SHA_STONE_BULWARK_ABSORB, EFFECT_0))
            {
                int32 ap = (int32)player->GetTotalAttackPowerValue(BASE_ATTACK);
                int32 sp = player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL);

                int32 amount = ((ap > sp) ? int32(0.547f * ap * 1) : int32(0.875f * sp * 1));

                effect_0->SetAmount(effect_0->GetAmount() + amount - 1);
                effect_0->GetBase()->RefreshDuration();
            }
            else
                prevent = false;

            if (prevent) // do not prevent default action if it is first tick, or SPELL_SHA_STONE_BULWARK_ABSORB is removed
                PreventDefaultAction(); 
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_stone_bulwark_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_sha_stone_bulwark_AuraScript();
    }
};

// Stone Bulwark - 114893
class spell_sha_stone_bulwark : public SpellScriptLoader
{
    public:
        spell_sha_stone_bulwark() : SpellScriptLoader("spell_sha_stone_bulwark") { }

        class spell_sha_stone_bulwark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_stone_bulwark_AuraScript);

            bool CalculateAmount(AuraEffect const* aurEff , int32 & amount, bool & )
            {
                Unit* caster = GetCaster();
                if (!caster || !caster->GetOwner() || !caster->GetOwner()->ToPlayer())
                    return false;

                Player* player = caster->GetOwner()->ToPlayer();
                
                int32 ap = (int32)player->GetTotalAttackPowerValue(BASE_ATTACK);
                int32 sp = player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL);

                amount = (ap > sp) ? int32(0.547f * ap * 4) : int32(0.875f * sp * 4);

                return true;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_stone_bulwark_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_stone_bulwark_AuraScript();
        }
};

// Mail Specialization - 86529
class spell_sha_mail_specialization : public SpellScriptLoader
{
    public:
        spell_sha_mail_specialization() : SpellScriptLoader("spell_sha_mail_specialization") { }

        class spell_sha_mail_specialization_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_mail_specialization_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_SHAMAN_ELEMENTAL
                            || _player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_SHAMAN_RESTORATION)
                        _player->CastSpell(_player, SPELL_SHA_MAIL_SPECIALISATION_INT, true);
                    else if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_SHAMAN_ENHANCEMENT)
                        _player->CastSpell(_player, SPELL_SHA_MAIL_SPECIALIZATION_AGI, true);
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_SHA_MAIL_SPECIALISATION_INT))
                        _player->RemoveAura(SPELL_SHA_MAIL_SPECIALISATION_INT);
                    else if (_player->HasAura(SPELL_SHA_MAIL_SPECIALIZATION_AGI))
                        _player->RemoveAura(SPELL_SHA_MAIL_SPECIALIZATION_AGI);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_sha_mail_specialization_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_sha_mail_specialization_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_mail_specialization_AuraScript();
        }
};

// Frost Shock - 8056
class spell_sha_frozen_power : public SpellScriptLoader
{
    public:
        spell_sha_frozen_power() : SpellScriptLoader("spell_sha_frozen_power") { }

        class spell_sha_frozen_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_frozen_power_SpellScript);

            bool Validate(SpellEntry const * spellEntry)
            {
                if (!sSpellMgr->GetSpellInfo(8056))
                    return false;
                return true;
            }

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(SPELL_SHA_FROZEN_POWER))
                            _player->CastSpell(target, SPELL_SHA_FROST_SHOCK_FREEZE, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_frozen_power_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_frozen_power_SpellScript();
        }
};

// Spirit Link - 98020 : triggered by 98017
// Spirit Link Totem
class spell_sha_spirit_link : public SpellScriptLoader
{
    public:
        spell_sha_spirit_link() : SpellScriptLoader("spell_sha_spirit_link") { }

        class spell_sha_spirit_link_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_spirit_link_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->GetEntry() == 53006)
                    {
                        if (caster->GetOwner())
                        {
                            if (Player* _player = caster->GetOwner()->ToPlayer())
                            {
                                std::list<Unit*> memberList;
                                _player->GetPartyMembers(memberList);

                                float totalRaidHealthPct = 0;

                                uint32 amountOfAffected = 0;

                                for (auto itr : memberList)
                                    if (itr->IsWithinDistInMap(_player, 10.0f))
                                    {
                                        ++amountOfAffected;
                                        totalRaidHealthPct += itr->GetHealthPct();
                                    }

                                totalRaidHealthPct /= amountOfAffected * 100.0f;

                                for (auto itr : memberList)
                                    if (itr->IsWithinDistInMap(_player, 10.0f))
                                        itr->SetHealth(uint32(totalRaidHealthPct * itr->GetMaxHealth()));
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_spirit_link_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_spirit_link_SpellScript();
        }
};

// Mana Tide - 16191
class spell_sha_mana_tide : public SpellScriptLoader
{
    public:
        spell_sha_mana_tide() : SpellScriptLoader("spell_sha_mana_tide") { }

        class spell_sha_mana_tide_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_mana_tide_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_BLAST))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Player* _player = GetCaster()->GetOwner()->ToPlayer())
                    {
                        AuraApplication* aura = target->GetAuraApplication(SPELL_SHA_MANA_TIDE, GetCaster()->GetGUID());

                        aura->GetBase()->GetEffect(0)->ChangeAmount(0);

                        int32 spirit = _player->GetStat(STAT_SPIRIT) * 2;

                        aura->GetBase()->GetEffect(0)->ChangeAmount(spirit);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_mana_tide_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_mana_tide_SpellScript();
        }
};

// Called by Chain Heal - 1064 or Riptide - 61295
// Tidal Waves - 51564
class spell_sha_tidal_waves : public SpellScriptLoader
{
    public:
        spell_sha_tidal_waves() : SpellScriptLoader("spell_sha_tidal_waves") { }

        class spell_sha_tidal_waves_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_tidal_waves_SpellScript)

            bool Validate(SpellEntry const * /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(1064) || !sSpellMgr->GetSpellInfo(61295))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(SPELL_SHA_TIDAL_WAVES))
                    {
                        if (Unit* target = GetHitUnit())
                        {
                            int32 bp0 = -(sSpellMgr->GetSpellInfo(SPELL_SHA_TIDAL_WAVES)->Effects[0].BasePoints);
                            int32 bp1 = sSpellMgr->GetSpellInfo(SPELL_SHA_TIDAL_WAVES)->Effects[1].BasePoints;
                            _player->CastCustomSpell(_player, SPELL_SHA_TIDAL_WAVES_PROC, &bp0, &bp1, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_tidal_waves_SpellScript::HandleOnHit);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_sha_tidal_waves_SpellScript();
        }
};

// Fire Nova - 1535
class spell_sha_fire_nova : public SpellScriptLoader
{
    public:
        spell_sha_fire_nova() : SpellScriptLoader("spell_sha_fire_nova") { }

        class spell_sha_fire_nova_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_fire_nova_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    if (target->HasAura(SPELL_SHA_FLAME_SHOCK))
                    {
                        caster->CastSpell(target, SPELL_SHA_FIRE_NOVA_TRIGGERED, true);
                    }
                }
            }
            SpellCastResult HandleCheckCast()
            {
                UnitList targets;
                JadeCore::AnyUnitHavingBuffInObjectRangeCheck u_check(GetCaster(), GetCaster(), 100, SPELL_SHA_FLAME_SHOCK, false);
                JadeCore::UnitListSearcher<JadeCore::AnyUnitHavingBuffInObjectRangeCheck> searcher(GetCaster(), targets, u_check);
                GetCaster()->VisitNearbyObject(100, searcher);
                
                return targets.size() == 0 ? SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW : SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_fire_nova_SpellScript::HandleCheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_sha_fire_nova_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_fire_nova_SpellScript();
        }
};

// Unleash Elements - 73680
class spell_sha_unleash_elements : public SpellScriptLoader
{
    public:
        spell_sha_unleash_elements() : SpellScriptLoader("spell_sha_unleash_elements") { }

        class spell_sha_unleash_elements_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_unleash_elements_SpellScript);

            SpellCastResult HandleCheckCast()
            {
                 Unit* caster = GetCaster();
                 if (!caster)
                     return SPELL_FAILED_DONT_REPORT;

                 Player* plr = caster->ToPlayer();
                 if (!plr)
                     return SPELL_FAILED_DONT_REPORT;

                 if (!GetExplTargetUnit())
                     return SPELL_FAILED_BAD_TARGETS;

                 Item* weapons[2];
                 weapons[0] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                 weapons[1] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                 bool found_enchantment = false;

                 for (int i = 0; i < 2; i++)
                 {
                     if (!weapons[i])
                          continue;

                     int32 enchant = weapons[i]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT);
 
                     if (enchant == 0)
                         continue;

                     found_enchantment = true;

                     switch (enchant)
                     {
                         case 5: // Flametongue Weapon
                         case 2: // Frostbrand Weapon
                         case 3021: // Rockbiter Weapon
                         case 283: // Windfury Weapon
                         {
                             if (!plr->IsValidAttackTarget(GetExplTargetUnit()))
                                 return SPELL_FAILED_BAD_TARGETS;
                             break;
                         }
                     }
                 }
                 return found_enchantment == false ? SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW : SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                       Unit* caster = GetCaster();
                       if (!caster)
                           return;
                       Player* plr = caster->ToPlayer();
                       if (!plr)
                           return;

                       if (!GetExplTargetUnit())
                           return;

                        Item* weapons[2];
                        weapons[0] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                        weapons[1] = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        for (int i = 0; i < 2; i++)
                        {
                            if (!weapons[i])
                                continue;

                            uint32 unleashSpell = 0;
                            uint32 furySpell = 0;
                            Unit *target = GetExplTargetUnit();
                            bool hasFuryTalent = plr->HasAura(SPELL_SHA_UNLEASHED_FURY_TALENT);
                            bool hostileTarget = plr->IsValidAttackTarget(target);
                            bool hostileSpell = true;
                            switch (weapons[i]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
                            {
                                case 3345: // Earthliving Weapon
                                    unleashSpell = 73685; // Unleash Life
                                    hostileSpell = false;

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_EARTHLIVING;
                                    break;
                                case 5: // Flametongue Weapon
                                    unleashSpell = 73683; // Unleash Flame

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_FLAMETONGUE;
                                    break;
                                case 2: // Frostbrand Weapon
                                    unleashSpell = 73682; // Unleash Frost

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_FROSTBRAND;
                                    break;
                                case 3021: // Rockbiter Weapon
                                    unleashSpell = 73684; // Unleash Earth

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_ROCKBITER;
                                    break;
                                case 283: // Windfury Weapon
                                    unleashSpell = 73681; // Unleash Wind

                                    if (hasFuryTalent)
                                        furySpell = SPELL_SHA_UNLEASHED_FURY_WINDFURY;
                                    break;
                            }

                            if (hostileSpell && !hostileTarget)
                                return; // don't allow to attack non-hostile targets. TODO: check this before cast

                            if (!hostileSpell && hostileTarget)
                                target = plr;   // heal ourselves instead of the enemy

                            if (unleashSpell)
                                plr->CastSpell(target, unleashSpell, true);

                            if (furySpell)
                                plr->CastSpell(target, furySpell, true);

                            target = GetHitUnit();

                            // If weapons are enchanted by same enchantment, only one should be unleashed
                            if (i == 0 && weapons[0] && weapons[1])
                                if (weapons[0]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == weapons[1]->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
                                    return;
                        }
                        
                        // Item - Shaman T16 Enhancement 2P Bonus
                        if (caster->HasAura(144962))
                            caster->AddAura(144964, caster);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_unleash_elements_SpellScript::HandleCheckCast);
                OnHit += SpellHitFn(spell_sha_unleash_elements_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_unleash_elements_SpellScript();
        }
};

// Called by Lightning Bolt - 403 and Chain Lightning - 421
// Lightning Bolt (Mastery) - 45284, Chain Lightning - 45297 and Lava Beam (Fire Ascendant) - 114074
// Rolling Thunder - 88764
class spell_sha_rolling_thunder : public SpellScriptLoader
{
    public:
        spell_sha_rolling_thunder() : SpellScriptLoader("spell_sha_rolling_thunder") { }

        class spell_sha_rolling_thunder_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_rolling_thunder_SpellScript)

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Glyph of Telluric Currents - 10% for Enhancement
                        if (_player->HasAura(55453) && GetSpellInfo()->Id == 403 && !_player->HasAura(123099) && !_player->HasAura(112858))
                            _player->EnergizeBySpell(_player, 55453, _player->CountPctFromMaxMana(10), POWER_MANA);
                        // 2% for Elemental and Restoration
                        else if (_player->HasAura(55453) && GetSpellInfo()->Id == 403 && (_player->HasAura(123099) || _player->HasAura(112858)))
                            _player->EnergizeBySpell(_player, 55453, _player->CountPctFromMaxMana(2), POWER_MANA);

                        if (roll_chance_i(60) && _player->HasAura(SPELL_SHA_ROLLING_THUNDER_AURA))
                        {
                            if (Aura* lightningShield = _player->GetAura(SPELL_SHA_LIGHTNING_SHIELD_AURA))
                            {
                                _player->CastSpell(_player, SPELL_SHA_ROLLING_THUNDER_ENERGIZE, true);

                                uint8 lsCharges = lightningShield->GetCharges();

                                if (lsCharges < 6)
                                {
                                    uint8 chargesBonus = _player->HasAura(SPELL_SHA_ITEM_T14_4P) ? 2 : 1;
                                    lightningShield->SetCharges(lsCharges + chargesBonus);
                                    lightningShield->RefreshDuration();
                                }
                                else if (lsCharges < 7)
                                {
                                    lightningShield->SetCharges(lsCharges + 1);
                                    lightningShield->RefreshDuration();
                                }

                                // refresh to handle Fulmination visual
                                lsCharges = lightningShield->GetCharges();

                                if (lsCharges >= 7 && _player->HasAura(SPELL_SHA_FULMINATION))
                                    _player->CastSpell(_player, SPELL_SHA_FULMINATION_INFO, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_rolling_thunder_SpellScript::HandleOnHit);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_sha_rolling_thunder_SpellScript();
        }
};

// 88766 Fulmination handled in 8042 Earth Shock
class spell_sha_fulmination : public SpellScriptLoader
{
    public:
        spell_sha_fulmination() : SpellScriptLoader("spell_sha_fulmination") { }

        class spell_sha_fulmination_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_fulmination_SpellScript)

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (!GetHitDamage())
                            return;

                        AuraEffect* fulminationAura = _player->GetDummyAuraEffect(SPELLFAMILY_SHAMAN, 2010, 0);
                        if (!fulminationAura)
                            return;

                        Aura* lightningShield = _player->GetAura(SPELL_SHA_LIGHTNING_SHIELD_AURA);
                        if (!lightningShield)
                            return;

                        uint8 lsCharges = lightningShield->GetCharges();
                        if (lsCharges <= 1)
                            return;

                        uint8 usedCharges = lsCharges - 1;

                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE);
                        if (!spellInfo)
                            return;
                        int32 basePoints = _player->CalculateSpellDamage(target, spellInfo, 0);
                        uint32 damage = usedCharges * _player->SpellDamageBonusDone(target, spellInfo, basePoints, SPELL_DIRECT_DAMAGE);

                        _player->CastCustomSpell(SPELL_SHA_FULMINATION_TRIGGERED, SPELLVALUE_BASE_POINT0, damage, target, true, NULL, fulminationAura);
                        lightningShield->SetCharges(lsCharges - usedCharges);

                        // Item - Shaman T16 Elemental 2P Bonus
                        if (_player->HasAura(SHAMAN_ITEM_T16_ELEMENTAL_2P_BONUS))
                        {
                            _player->AddAura(SPELL_SHA_ELEMENTAL_DISCHARGE, target);

                            if (Aura* elementalDischarge = target->GetAura(SPELL_SHA_ELEMENTAL_DISCHARGE, _player->GetGUID()))
                            {
                                elementalDischarge->SetMaxDuration(2000 * usedCharges);
                                elementalDischarge->SetDuration(2000 * usedCharges);
                            }
                        }

                        _player->RemoveAura(SPELL_SHA_FULMINATION_INFO);
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_fulmination_SpellScript::HandleAfterHit);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_sha_fulmination_SpellScript();
        }
};

// Healing Stream - 52042
class spell_sha_healing_stream : public SpellScriptLoader
{
    public:
        spell_sha_healing_stream() : SpellScriptLoader("spell_sha_healing_stream") { }

        class spell_sha_healing_stream_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_stream_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_HEALING_STREAM))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (!GetCaster()->GetOwner())
                    return;

                if (Player* _player = GetCaster()->GetOwner()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        // Glyph of Healing Stream Totem
                        if (_player->HasAura(SPELL_SHA_GLYPH_OF_HEALING_STREAM_TOTEM))
                            _player->CastSpell(target, SPELL_SHA_GLYPH_OF_HEALING_STREAM, true);
            }
            
            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty() || !GetCaster() || !GetCaster()->GetOwner())
                    return;

                targets.clear();

                if (Player* _player = GetCaster()->GetOwner()->ToPlayer())
                {
                    std::list<Unit*> unitList;
                    _player->GetPartyMembers(unitList, true, false);

                    unitList.sort(JadeCore::HealthPctOrderPred());
                    if (unitList.size() >= 2)
                        unitList.resize(2);

                    for (auto itr : unitList)
                        targets.push_back(itr);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_healing_stream_SpellScript::HandleOnHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_healing_stream_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_stream_SpellScript();
        }
};

// Called by Stormstrike - 17364 and Lava Lash - 60103
// Static Shock - 51527
class spell_sha_static_shock : public SpellScriptLoader
{
    public:
        spell_sha_static_shock() : SpellScriptLoader("spell_sha_static_shock") { }

        class spell_sha_static_shock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_static_shock_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // While have Lightning Shield active
                        if (target && _player->HasAura(SPELL_SHA_LIGHTNING_SHIELD_AURA) && _player->HasAura(51527))
                        {
                            // Item - Shaman T9 Enhancement 2P Bonus (Static Shock)
                            if (_player->HasAura(67220))
                            {
                                if (roll_chance_i(48))
                                {
                                    _player->CastSpell(target, SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE, true);
                                }
                            }
                            else
                            {
                                if (roll_chance_i(45))
                                {
                                    _player->CastSpell(target, SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE, true);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_static_shock_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_static_shock_SpellScript();
        }
};

// Elemental Blast - 117014
class spell_sha_elemental_blast : public SpellScriptLoader
{
    public:
        spell_sha_elemental_blast() : SpellScriptLoader("spell_sha_elemental_blast") { }

        class spell_sha_elemental_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_elemental_blast_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_BLAST))
                    return false;
                return true;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        _player->CastSpell(target, SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL, true);
                        _player->CastSpell(target, SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL, true);
                    }
                }
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (auto aura = _player->AddAura(SPELL_SHA_ELEMENTAL_BLAST_RATING_BONUS, _player))
                    {
                        int32 limit = 4;
                        if (_player->GetSpecializationId() != SPEC_SHAMAN_ENHANCEMENT)
                            limit = 3;

                        uint32 _random = urand(0, limit-1);
                        for (uint8 i = 0; i < 4; ++i) // only 4 effects available
                        {
                            if (i == _random)
                                continue;

                            if (auto _effect = aura->GetEffect(i))
                                _effect->ChangeAmount(0);
                        }
                    }

                    // Unleash Flame increase Elemental Blast damage for 30%
                    if (_player->HasAura(73683))
                        SetHitDamage(int32(GetHitDamage() * 1.3f));
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_elemental_blast_SpellScript::HandleAfterCast);
                OnHit += SpellHitFn(spell_sha_elemental_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_elemental_blast_SpellScript();
        }
};

// Earthquake : Ticks - 77478
class spell_sha_earthquake_tick : public SpellScriptLoader
{
    public:
        spell_sha_earthquake_tick() : SpellScriptLoader("spell_sha_earthquake_tick") { }

        class spell_sha_earthquake_tick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthquake_tick_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_EARTHQUAKE_TICK))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                // With a 10% chance of knocking down affected targets
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (roll_chance_i(10))
                            _player->CastSpell(target, SPELL_SHA_EARTHQUAKE_KNOCKING_DOWN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_earthquake_tick_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthquake_tick_SpellScript();
        }
};

// Earthquake - 61882
class spell_sha_earthquake : public SpellScriptLoader
{
    public:
        spell_sha_earthquake() : SpellScriptLoader("spell_sha_earthquake") { }

        class spell_sha_earthquake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_earthquake_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (!GetCaster())
                    return;

                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_SHA_EARTHQUAKE))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHA_EARTHQUAKE_TICK, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthquake_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_earthquake_AuraScript();
        }
};

// Healing Rain - 73920
class spell_sha_healing_rain : public SpellScriptLoader
{
    public:
        spell_sha_healing_rain() : SpellScriptLoader("spell_sha_healing_rain") { }

        class spell_sha_healing_rain_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_rain_SpellScript);

            void HandleOnHit()
            {
                if (WorldLocation const* loc = GetExplTargetDest())
                {
                    if (Unit* caster = GetCaster())
                    {
                        // Casting a second healing rain after prolonging the previous one using conductivity
                        // will replace the old healing rain with base amount of duration (in other words, you will not have 2 healing rains).
                        if (DynamicObject* dynObj = caster->GetDynObject(SPELL_SHA_HEALING_RAIN))
                            caster->RemoveDynObject(SPELL_SHA_HEALING_RAIN);

                        caster->CastSpell(loc->GetPositionX(), loc->GetPositionY(), loc->GetPositionZ(), SPELL_SHA_HEALING_RAIN, true);

                        // Reset amount of Conductivity
                        if (Aura* conductivity = caster->GetAura(SPELL_SHA_CONDUCTIVITY_TALENT))
                            conductivity->GetEffect(2)->SetAmount(40000);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_healing_rain_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_rain_SpellScript();
        }

        class spell_sha_healing_rain_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_healing_rain_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (!GetCaster())
                    return;

                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_SHA_HEALING_RAIN))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHA_HEALING_RAIN_TICK, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_healing_rain_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_healing_rain_AuraScript();
        }
};

// healing rain heal effect 73921
class spell_sha_healing_rain_heal : public SpellScriptLoader
{
    public:
        spell_sha_healing_rain_heal() : SpellScriptLoader("spell_sha_healing_rain_heal") { }

        class spell_sha_healing_rain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_rain_heal_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                // remove non-player target (pet, creatures and etc.)
                targets.remove_if([this](WorldObject* target) -> bool
                {
                    if (!target || !target->ToPlayer())
                        return true;
                    return false;
                });

                targets.sort(JadeCore::HealthPctOrderPred());
                if (targets.size() > 6)
                    targets.resize(6);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_healing_rain_heal_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_rain_heal_SpellScript();
        }
};

// Ascendance - 114049
class spell_sha_ascendance : public SpellScriptLoader
{
    public:
        spell_sha_ascendance() : SpellScriptLoader("spell_sha_ascendance") { }

        class spell_sha_ascendance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ascendance_SpellScript);

            bool Validate(SpellInfo const* spellEntry)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ASCENDANCE))
                    return false;
                return true;
            }

            SpellCastResult CheckCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_NONE)
                    {
                        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_SELECT_TALENT_SPECIAL);
                        return SPELL_FAILED_CUSTOM_ERROR;
                    }

                    return SPELL_CAST_OK;
                }

                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    switch(_player->GetSpecializationId(_player->GetActiveSpec()))
                    {
                        case SPEC_SHAMAN_ELEMENTAL:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_ELEMENTAL, true);

                            // If we has a cooldown on Lava Burst, we should remove it.
                            if (_player->HasSpellCooldown(SPELL_SHA_LAVA_BURST))
                                _player->RemoveSpellCooldown(SPELL_SHA_LAVA_BURST, true);
                            break;
                        case SPEC_SHAMAN_ENHANCEMENT:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_ENHANCED, true);

                            if (_player->HasSpellCooldown(SPELL_SHA_STORMSTRIKE))
                                _player->RemoveSpellCooldown(SPELL_SHA_STORMSTRIKE, true);
                            break;
                        case SPEC_SHAMAN_RESTORATION:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_RESTORATION, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_ascendance_SpellScript::CheckCast);
                AfterCast += SpellCastFn(spell_sha_ascendance_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ascendance_SpellScript();
        }
};

class EarthenPowerTargetSelector
{
    public:
        EarthenPowerTargetSelector() { }

        bool operator() (WorldObject* target)
        {
            if (!target->ToUnit())
                return true;

            if (!target->ToUnit()->HasAuraWithMechanic(1 << MECHANIC_SNARE))
                return true;

            return false;
        }
};

class spell_sha_bloodlust : public SpellScriptLoader
{
    public:
        spell_sha_bloodlust() : SpellScriptLoader("spell_sha_bloodlust") { }

        class spell_sha_bloodlust_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_bloodlust_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SHAMAN_SPELL_SATED))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_SATED));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HUNTER_SPELL_INSANITY));
                targets.remove_if(JadeCore::UnitAuraCheck(true, MAGE_SPELL_TEMPORAL_DISPLACEMENT));
                targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_EXHAUSTION));
            }

            void ApplyDebuff()
            {
                if (Unit* target = GetHitUnit())
                {
                    /// We must add a check here too, players can use a bug allowing them to receive twice a buff like this one
                    /// If they cast the spells exactly at the same time
                    if (target->HasAura(SHAMAN_SPELL_EXHAUSTION) || target->HasAura(HUNTER_SPELL_INSANITY) ||
                        target->HasAura(MAGE_SPELL_TEMPORAL_DISPLACEMENT) || target->HasAura(SHAMAN_SPELL_SATED))
                    {
                        target->RemoveAura(GetSpellInfo()->Id);
                        return;
                    }

                    target->CastSpell(target, SHAMAN_SPELL_SATED, true);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_sha_bloodlust_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_bloodlust_SpellScript();
        }
};

class spell_sha_heroism : public SpellScriptLoader
{
    public:
        spell_sha_heroism() : SpellScriptLoader("spell_sha_heroism") { }

        class spell_sha_heroism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_heroism_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SHAMAN_SPELL_EXHAUSTION))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_EXHAUSTION));
                targets.remove_if(JadeCore::UnitAuraCheck(true, HUNTER_SPELL_INSANITY));
                targets.remove_if(JadeCore::UnitAuraCheck(true, MAGE_SPELL_TEMPORAL_DISPLACEMENT));
                targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_SATED));
            }

            void ApplyDebuff()
            {
                if (Unit* target = GetHitUnit())
                {
                    /// We must add a check here too, players can use a bug allowing them to receive twice a buff like this one
                    /// If they cast the spells exactly at the same time
                    if (target->HasAura(SHAMAN_SPELL_EXHAUSTION) || target->HasAura(HUNTER_SPELL_INSANITY) ||
                        target->HasAura(MAGE_SPELL_TEMPORAL_DISPLACEMENT) || target->HasAura(SHAMAN_SPELL_SATED))
                    {
                        target->RemoveAura(GetSpellInfo()->Id);
                        return;
                    }

                    target->CastSpell(target, SHAMAN_SPELL_EXHAUSTION, true);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_sha_heroism_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_heroism_SpellScript();
        }
};

enum AncestralAwakeningProc
{
    SPELL_ANCESTRAL_AWAKENING_PROC   = 52752,
};

class spell_sha_ancestral_awakening_proc : public SpellScriptLoader
{
    public:
        spell_sha_ancestral_awakening_proc() : SpellScriptLoader("spell_sha_ancestral_awakening_proc") { }

        class spell_sha_ancestral_awakening_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ancestral_awakening_proc_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ANCESTRAL_AWAKENING_PROC))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                int32 damage = GetEffectValue();
                if (Unit* l_Caster = GetCaster())
                {
                    /// According to Wowpedia, Purification is applied twice:
                    /// http://wow.gamepedia.com/Ancestral_Awakening
                    if (AuraEffect* l_PurificationEffect = l_Caster->GetAuraEffect(16213, EFFECT_0))
                        AddPct(damage, l_PurificationEffect->GetAmount());

                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastCustomSpell(l_Target, SPELL_ANCESTRAL_AWAKENING_PROC, &damage, nullptr, nullptr, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_ancestral_awakening_proc_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ancestral_awakening_proc_SpellScript();
        }
};

// Lava Lash - 60103
class spell_sha_lava_lash : public SpellScriptLoader
{
    public:
        spell_sha_lava_lash() : SpellScriptLoader("spell_sha_lava_lash") { }

        class spell_sha_lava_lash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_lash_SpellScript)

            int32 searingFlameAmount;

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 hitDamage = GetHitDamage();
                        if (Item* weapon = _player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                        {
                            // Damage increased by 40% if off-hand weapon enchanted by Frostbrand weapon.
                            if (_player->HasAura(SPELL_SHA_ITEM_S12_4P_ENHANCEMENT_BONUS) &&
                                weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 2)
                                AddPct(hitDamage, 40);

                            // Damage is increased by 23%(custom)/40% if your off-hand weapon is enchanted with Flametongue.
                            if (_player->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_SHAMAN, 0x200000, 0, 0) && _player->HasAura(77661))
                                AddPct(hitDamage, 21);
                            else if (_player->GetAuraEffect(SPELL_AURA_DUMMY, SPELLFAMILY_SHAMAN, 0x200000, 0, 0) && !_player->HasAura(77661))
                                AddPct(hitDamage, 40);

                            // Unleash Flame increase Lava Lash damage for 30%
                            if (_player->HasAura(73683))
                                AddPct(hitDamage, 30);

                            // Your Lava Lash ability will consume Searing Flame effect, dealing 20% increased damage for each application
                            if (AuraApplication* searingFlame = _player->GetAuraApplication(SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE))
                            {
                                searingFlameAmount = searingFlame->GetBase()->GetStackAmount();
                                searingFlameAmount *= 20;
                                AddPct(hitDamage, searingFlameAmount);

                                _player->RemoveAura(SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE);
                            }

                            SetHitDamage(hitDamage);

                            // Spreading your Flame shock from the target to up to four ennemies within 12 yards
                            // Effect desactivated if has Glyph of Lava Lash
                            if (!_player->HasAura(55444))
                            {
                                if (Aura* flameShock = target->GetAura(SPELL_SHA_FLAME_SHOCK, _player->GetGUID()))
                                {
                                    std::list<Unit*> targetList;

                                    _player->GetAttackableUnitListInRange(targetList, 12.0f);

                                    int32 flameShockDuration = flameShock->GetDuration();

                                    int32 hitTargets = 0;

                                    for (auto itr : targetList)
                                    {
                                        if (!_player->IsValidAttackTarget(itr))
                                            continue;

                                        if (itr->GetGUID() == target->GetGUID())
                                            continue;

                                        if (itr->GetGUID() == _player->GetGUID())
                                            continue;

                                        if (hitTargets >= 4)
                                            continue;

                                        // We should add flame shock with the same duration, as it on the original target
                                        _player->AddAura(SPELL_SHA_FLAME_SHOCK, itr);
                                        if (Aura* flameShockNew = itr->GetAura(SPELL_SHA_FLAME_SHOCK, _player->GetGUID()))
                                            flameShockNew->SetDuration(flameShockDuration);

                                        hitTargets++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_lava_lash_SpellScript::HandleOnHit);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_lash_SpellScript();
        }
};

// Chain Heal - 1064
class spell_sha_chain_heal : public SpellScriptLoader
{
    public:
        spell_sha_chain_heal() : SpellScriptLoader("spell_sha_chain_heal") { }

        class spell_sha_chain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_chain_heal_SpellScript);

            bool Load()
            {
                firstHeal = true;
                riptide = false;
                return true;
            }

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (firstHeal)
                {
                    // Check if the target has Riptide
                    if (AuraEffect* aurEff = GetHitUnit()->GetAuraEffect(SPELL_AURA_PERIODIC_HEAL, SPELLFAMILY_SHAMAN, 0, 0, 0x10, GetCaster()->GetGUID()))
                    {
                        riptide = true;

                        // Consume it
                        // if caster has no Item - Shaman T12 Restoration 4P Bonus
                        if (!GetCaster()->HasAura(99195))
                            GetHitUnit()->RemoveAura(aurEff->GetBase());
                    }
                    firstHeal = false;
                }
                // Riptide increases the Chain Heal effect by 25%
                if (riptide)
                    SetHitHeal(GetHitHeal() * 1.25f);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_chain_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }

            bool firstHeal;
            bool riptide;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_chain_heal_SpellScript();
        }
};

// Glyph of Lightning Shield - 101052
class spell_sha_glyph_of_lightning_shield : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_lightning_shield() : SpellScriptLoader("spell_sha_glyph_of_lightning_shield") { }

        class spell_sha_glyph_of_lightning_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_lightning_shield_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (GetSpellInfo()->Id == 26364 && _player->HasAura(101052))
                        _player->CastSpell(_player, 142912, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_lightning_shield_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_lightning_shield_SpellScript();
        }
};

// Glyph of Telluric Currents - 55453
class spell_sha_glyph_of_telluric_currents : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_telluric_currents() : SpellScriptLoader("spell_sha_glyph_of_telluric_currents") { }

        class spell_sha_glyph_of_telluric_currents_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_glyph_of_telluric_currents_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster() || !eventInfo.GetDamageInfo()->GetVictim())
                    return;

                if ((eventInfo.GetDamageInfo()->GetSpellInfo() && eventInfo.GetDamageInfo()->GetSpellInfo()->Id != 403) || 
                       !GetCaster()->HasAura(SPELL_SHA_GLYPH_OF_TELLURIC_CURRENTS))
                    return;

                uint32 pct = 10;
                // Elemental and Restoration shamans have 300k mana, so 0.02% for them
                if (GetCaster()->HasAura(123099) || GetCaster()->HasAura(112858))
                    pct = 2;

                int32 restoreMana = GetCaster()->CountPctFromMaxMana(pct);

                GetCaster()->EnergizeBySpell(GetCaster(), 82987, restoreMana, POWER_MANA);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_telluric_currents_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_glyph_of_telluric_currents_AuraScript();
        }
};

// Called by Lava Burst - 51505
// Lava Surge - 77756
class spell_sha_lava_surge : public SpellScriptLoader
{
    public:
        spell_sha_lava_surge() : SpellScriptLoader("spell_sha_lava_surge") { }

        class spell_sha_lava_surge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_surge_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetAuraBeforeInstantCast())
                    {
                        _player->RemoveAura(SPELL_SHA_LAVA_SURGE);
                        _player->SetAuraBeforeInstantCast(false);
                    }
                    else if (_player->HasAura(SPELL_SHA_LAVA_SURGE))
                       _player->RemoveSpellCooldown(SPELL_SHA_LAVA_BURST, true);

                    if (_player->HasAura(138144)) // Item - Shaman T15 Elemental 4P Bonus
                    {
                        if (_player->HasSpellCooldown(114050))
                            _player->ReduceSpellCooldown(114050, 1 * IN_MILLISECONDS);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_lava_surge_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_surge_SpellScript();
        }
};

// Flame Shock - 8050
class spell_sha_flame_shock : public SpellScriptLoader
{
    public:
        spell_sha_flame_shock() : SpellScriptLoader("spell_sha_flame_shock") { }

        class spell_sha_flame_shock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_flame_shock_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (GetCaster() && GetCaster()->ToPlayer())
                {
                    // Item - Shaman T16 Enhancement 4P Bonus
                    if (roll_chance_i(5) && GetCaster()->HasAura(144966))
                    {
                        for (int i = 0; i < 5; i++)
                            GetCaster()->AddAura(SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE, GetCaster());  // Searing Flame buff - 5 stacks

                        if (GetCaster()->ToPlayer()->HasSpellCooldown(SPELL_SHA_LAVA_LASH))
                            GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_SHA_LAVA_LASH, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_flame_shock_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_flame_shock_AuraScript();
        }
};

// Called by Far Sight - 6196
// Glyph of Far Sight - 58059
class spell_sha_far_sight : public SpellScriptLoader
{
    public:
        spell_sha_far_sight() : SpellScriptLoader("spell_sha_far_sight") { }

        class spell_sha_far_sight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_far_sight_SpellScript);

            SpellCastResult CheckTarget()
            {
                // Any effect on Mechanical, Elemental units or player corpses
                if (Unit* caster = GetCaster())
                {
                    if (!caster->GetMap()->IsOutdoors(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ()) && !caster->HasAura(GLYPH_SHA_FAR_SIGHT))
                        return SPELL_FAILED_ONLY_OUTDOORS;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_far_sight_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_far_sight_SpellScript();
        }
};

// Called by Greater Healing Wave - 77472
class spell_sha_greater_healing_wave : public SpellScriptLoader
{
    public:
        spell_sha_greater_healing_wave() : SpellScriptLoader("spell_sha_greater_healing_wave") { }

        class spell_sha_greater_healing_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_greater_healing_wave_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (GetSpell()->IsCritForTarget(target))
                        {
                            // 30% of heal
                            // x2 because it's a crit
                            int32 heal = CalculatePct(GetHitHeal() * 2, 30);
                            _player->CastCustomSpell(target, 52759, &heal, NULL, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_greater_healing_wave_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_greater_healing_wave_SpellScript();
        }
};

void AddSC_shaman_spell_scripts()
{
    new spell_sha_totemic_projection();
    new spell_sha_hex();
    new spell_sha_water_ascendant();
    new spell_sha_prowl();
    new spell_sha_solar_beam();
    new spell_sha_glyph_of_shamanistic_rage();
    new spell_sha_glyph_of_lakestrider();
    new spell_sha_call_of_the_elements();
    new spell_sha_conductivity();
    new spell_sha_ancestral_guidance();
    new spell_sha_ancestral_guidance_heal();
    new spell_sha_echo_of_the_elements();
    new spell_sha_earthgrab();
    new spell_sha_stone_bulwark();
    new spell_sha_stone_bulwark_passive();
    new spell_sha_mail_specialization();
    new spell_sha_frozen_power();
    new spell_sha_spirit_link();
    new spell_sha_mana_tide();
    new spell_sha_tidal_waves();
    new spell_sha_fire_nova();
    new spell_sha_unleash_elements();
    new spell_sha_rolling_thunder();
    new spell_sha_fulmination();
    new spell_sha_healing_stream();
    new spell_sha_static_shock();
    new spell_sha_elemental_blast();
    new spell_sha_earthquake_tick();
    new spell_sha_earthquake();
    new spell_sha_healing_rain();
    new spell_sha_ascendance();
    new spell_sha_bloodlust();
    new spell_sha_heroism();
    new spell_sha_ancestral_awakening_proc();
    new spell_sha_lava_lash();
    new spell_sha_chain_heal();
    new spell_sha_glyph_of_lightning_shield();
    new spell_sha_glyph_of_telluric_currents();
    new spell_sha_lava_surge();
    new spell_sha_flame_shock();
    new spell_sha_far_sight();
    new spell_sha_greater_healing_wave();
    new spell_sha_healing_rain_heal();
}

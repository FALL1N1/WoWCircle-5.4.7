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
 * Spells used in holidays/game events that do not fit any other category.
 * Scriptnames in this file should be prefixed with "spell_#holidayname_".
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "CellImpl.h"
#include "Vehicle.h"

// 24750 Trick
enum TrickSpells
{
    SPELL_PIRATE_COSTUME_MALE           = 24708,
    SPELL_PIRATE_COSTUME_FEMALE         = 24709,
    SPELL_NINJA_COSTUME_MALE            = 24710,
    SPELL_NINJA_COSTUME_FEMALE          = 24711,
    SPELL_LEPER_GNOME_COSTUME_MALE      = 24712,
    SPELL_LEPER_GNOME_COSTUME_FEMALE    = 24713,
    SPELL_SKELETON_COSTUME              = 24723,
    SPELL_GHOST_COSTUME_MALE            = 24735,
    SPELL_GHOST_COSTUME_FEMALE          = 24736,
    SPELL_TRICK_BUFF                    = 24753
};

class spell_trick : public SpellScriptLoader
{
    public:
        spell_trick() : SpellScriptLoader("spell_trick") {}

        class spell_trick_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_trick_SpellScript);
            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PIRATE_COSTUME_MALE) || !sSpellMgr->GetSpellInfo(SPELL_PIRATE_COSTUME_FEMALE) || !sSpellMgr->GetSpellInfo(SPELL_NINJA_COSTUME_MALE)
                    || !sSpellMgr->GetSpellInfo(SPELL_NINJA_COSTUME_FEMALE) || !sSpellMgr->GetSpellInfo(SPELL_LEPER_GNOME_COSTUME_MALE) || !sSpellMgr->GetSpellInfo(SPELL_LEPER_GNOME_COSTUME_FEMALE)
                    || !sSpellMgr->GetSpellInfo(SPELL_SKELETON_COSTUME) || !sSpellMgr->GetSpellInfo(SPELL_GHOST_COSTUME_MALE) || !sSpellMgr->GetSpellInfo(SPELL_GHOST_COSTUME_FEMALE) || !sSpellMgr->GetSpellInfo(SPELL_TRICK_BUFF))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Player* target = GetHitPlayer())
                {
                    uint8 gender = target->getGender();
                    uint32 spellId = SPELL_TRICK_BUFF;
                    switch (urand(0, 5))
                    {
                        case 1:
                            spellId = gender ? SPELL_LEPER_GNOME_COSTUME_FEMALE : SPELL_LEPER_GNOME_COSTUME_MALE;
                            break;
                        case 2:
                            spellId = gender ? SPELL_PIRATE_COSTUME_FEMALE : SPELL_PIRATE_COSTUME_MALE;
                            break;
                        case 3:
                            spellId = gender ? SPELL_GHOST_COSTUME_FEMALE : SPELL_GHOST_COSTUME_MALE;
                            break;
                        case 4:
                            spellId = gender ? SPELL_NINJA_COSTUME_FEMALE : SPELL_NINJA_COSTUME_MALE;
                            break;
                        case 5:
                            spellId = SPELL_SKELETON_COSTUME;
                            break;
                        default:
                            break;
                    }

                    caster->CastSpell(target, spellId, true, NULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_trick_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_trick_SpellScript();
        }
};

// 24751 Trick or Treat
enum TrickOrTreatSpells
{
    SPELL_TRICK                 = 24714,
    SPELL_TREAT                 = 24715,
    SPELL_TRICKED_OR_TREATED    = 24755,
    SPELL_TRICKY_TREAT_SPEED    = 42919,
    SPELL_TRICKY_TREAT_TRIGGER  = 42965,
    SPELL_UPSET_TUMMY           = 42966
};

class spell_trick_or_treat : public SpellScriptLoader
{
    public:
        spell_trick_or_treat() : SpellScriptLoader("spell_trick_or_treat") {}

        class spell_trick_or_treat_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_trick_or_treat_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TRICK) || !sSpellMgr->GetSpellInfo(SPELL_TREAT) || !sSpellMgr->GetSpellInfo(SPELL_TRICKED_OR_TREATED))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Player* target = GetHitPlayer())
                {
                    caster->CastSpell(target, roll_chance_i(50) ? SPELL_TRICK : SPELL_TREAT, true, NULL);
                    caster->CastSpell(target, SPELL_TRICKED_OR_TREATED, true, NULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_trick_or_treat_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_trick_or_treat_SpellScript();
        }
};

class spell_tricky_treat : public SpellScriptLoader
{
    public:
        spell_tricky_treat() : SpellScriptLoader("spell_tricky_treat") {}

        class spell_tricky_treat_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tricky_treat_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TRICKY_TREAT_SPEED))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_TRICKY_TREAT_TRIGGER))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_UPSET_TUMMY))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (caster->HasAura(SPELL_TRICKY_TREAT_TRIGGER) && caster->GetAuraCount(SPELL_TRICKY_TREAT_SPEED) > 3 && roll_chance_i(33))
                    caster->CastSpell(caster, SPELL_UPSET_TUMMY, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_tricky_treat_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_tricky_treat_SpellScript();
        }
};

// 45102 Romantic Picnic
enum SpellsPicnic
{
    SPELL_BASKET_CHECK              = 45119, // Holiday - Valentine - Romantic Picnic Near Basket Check
    SPELL_MEAL_PERIODIC             = 45103, // Holiday - Valentine - Romantic Picnic Meal Periodic - effect dummy
    SPELL_MEAL_EAT_VISUAL           = 45120, // Holiday - Valentine - Romantic Picnic Meal Eat Visual
    //SPELL_MEAL_PARTICLE             = 45114, // Holiday - Valentine - Romantic Picnic Meal Particle - unused
    SPELL_DRINK_VISUAL              = 45121, // Holiday - Valentine - Romantic Picnic Drink Visual
    SPELL_ROMANTIC_PICNIC_ACHIEV    = 45123, // Romantic Picnic periodic = 5000
};

class spell_love_is_in_the_air_romantic_picnic : public SpellScriptLoader
{
    public:
        spell_love_is_in_the_air_romantic_picnic() : SpellScriptLoader("spell_love_is_in_the_air_romantic_picnic") { }

        class spell_love_is_in_the_air_romantic_picnic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_love_is_in_the_air_romantic_picnic_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->SetStandState(UNIT_STAND_STATE_SIT);
                target->CastSpell(target, SPELL_MEAL_PERIODIC, false);
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                // Every 5 seconds
                Unit* target = GetTarget();
                Unit* caster = GetCaster();

                // If our player is no longer sit, remove all auras
                if (target->getStandState() != UNIT_STAND_STATE_SIT)
                {
                    target->RemoveAura(SPELL_ROMANTIC_PICNIC_ACHIEV);
                    target->RemoveAura(GetAura());
                    return;
                }

                target->CastSpell(target, SPELL_BASKET_CHECK, false); // unknown use, it targets Romantic Basket
                target->CastSpell(target, RAND(SPELL_MEAL_EAT_VISUAL, SPELL_DRINK_VISUAL), false);

                bool foundSomeone = false;
                // For nearby players, check if they have the same aura. If so, cast Romantic Picnic (45123)
                // required by achievement and "hearts" visual
                std::list<Player*> playerList;
                JadeCore::AnyPlayerInObjectRangeCheck checker(target, INTERACTION_DISTANCE*2);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(target, playerList, checker);
                target->VisitNearbyWorldObject(INTERACTION_DISTANCE*2, searcher);
                for (std::list<Player*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                {
                    if ((*itr) != target && (*itr)->HasAura(GetId())) // && (*itr)->getStandState() == UNIT_STAND_STATE_SIT)
                    {
                        if (caster)
                        {
                            caster->CastSpell(*itr, SPELL_ROMANTIC_PICNIC_ACHIEV, true);
                            caster->CastSpell(target, SPELL_ROMANTIC_PICNIC_ACHIEV, true);
                        }
                        foundSomeone = true;
                        // break;
                    }
                }

                if (!foundSomeone && target->HasAura(SPELL_ROMANTIC_PICNIC_ACHIEV))
                    target->RemoveAura(SPELL_ROMANTIC_PICNIC_ACHIEV);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_love_is_in_the_air_romantic_picnic_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_love_is_in_the_air_romantic_picnic_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_love_is_in_the_air_romantic_picnic_AuraScript();
        }
};

// Piligrim's Bounty Holyday

enum PiligrimsBountySpells
{
    // Special spell, used by player when he seats on a chair
    SPELL_RIDE_VEHICLE              = 65403, 

    // Special auras
    // Player must have this auras he can use chair's abilities
    SPELL_CAN_EAT_CRANBERRIES       = 61798,
    SPELL_CAN_EAT_PIE               = 61799,
    SPELL_CAN_EAT_STUFFING          = 61800,
    SPELL_CAN_EAT_TURKEY            = 61801,
    SPELL_CAN_EAT_SWEET_POTATOES    = 61802,

    // Chair's abilities
    SPELL_FEAST_ON_CRANBERRIES      = 61785,
    SPELL_FEAST_ON_PIE              = 61787,
    SPELL_FEAST_ON_STUFFING         = 61788,
    SPELL_FEAST_ON_SWEET_POTATOES   = 61786,
    SPELL_FEAST_ON_TURKEY           = 61784,
    SPELL_PASS_THE_TURKEY           = 66250,
    SPELL_PASS_THE_PIE              = 66260,
    SPELL_PASS_THE_SWEET_POTATOES   = 66262,
    SPELL_PASS_THE_STUFFING         = 66259,
    SPELL_PASS_THE_CRANBERRIES      = 66261,

    // Player gets theese auras after using chair's abilities
    // 5 stacks max
    SPELL_PIE_HELPINGS              = 61845,
    SPELL_SWEET_POTATO_HELPINGS     = 61844,
    SPELL_STUFFING_HELPINGS         = 61843,
    SPELL_TURKEY_HELPINGS           = 61842,
    SPELL_CRANBERRY_HELPINGS        = 61841,

    SPELL_THE_SPIRIT_OF_SHARING     = 61849,

    SPELL_KILL_COUNTER_VISUAL       = 62015,
    SPELL_KILL_COUNTER_VISUAL_MAX   = 62021,
    SPELL_TURKEY_TRACKER            = 62014,
    SPELL_TURKEY_MARKER             = 25281,
    SPELL_TURKEY_VENGEANCE          = 25285,

    SPELL_PASS_THE_SWEET_POTATOES_ACHIEVEMENT   = 66376,
    SPELL_PASS_THE_CRANBERRIES_ACHIEVEMENT      = 66372,
    SPELL_PASS_THE_PIE_ACHIEVEMENT              = 66374,
    SPELL_PASS_THE_TURKEY_ACHIEVEMENT           = 66373,
    SPELL_PASS_THE_STUFFING_ACHIEVEMENT         = 66375,
};

class spell_piligrims_bounty_pass_food : public SpellScriptLoader
{
    public:
        spell_piligrims_bounty_pass_food(const char* name, uint32 spellId) : SpellScriptLoader(name), m_spellId(spellId) {}

        class spell_piligrims_bounty_pass_food_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_piligrims_bounty_pass_food_SpellScript);
        public:
            spell_piligrims_bounty_pass_food_SpellScript(uint32 spellId) : SpellScript(), m_spellId(spellId) {}

            SpellCastResult HandleCheck()
            {
                if (!GetCaster() || !GetExplTargetUnit())
                    return SPELL_FAILED_DONT_REPORT;

                if (GetCaster()->GetGUID() != GetExplTargetUnit()->GetGUID())
                    if (GetExplTargetUnit()->HasAura(65403))
                        return SPELL_CAST_OK;

                return SPELL_FAILED_DONT_REPORT;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                Unit* caster = GetCaster();
                if (caster->GetTypeId() != TYPEID_PLAYER)
                    if (Vehicle* vehicle = GetCaster()->GetVehicleKit())
                        caster = vehicle->GetPassenger(0);

                if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                switch (m_spellId)
                {
                    case SPELL_PASS_THE_TURKEY:
                        caster->CastSpell(GetHitUnit(), 61928, true);
                        caster->CastSpell(caster, SPELL_PASS_THE_TURKEY_ACHIEVEMENT, true);
                        break;
                    case SPELL_PASS_THE_STUFFING:
                        caster->CastSpell(GetHitUnit(), 61927, true);
                        caster->CastSpell(caster, SPELL_PASS_THE_STUFFING_ACHIEVEMENT, true);
                        break;
                    case SPELL_PASS_THE_PIE:
                        caster->CastSpell(GetHitUnit(), 61926, true);
                        caster->CastSpell(caster, SPELL_PASS_THE_PIE_ACHIEVEMENT, true);
                        break;
                    case SPELL_PASS_THE_CRANBERRIES: 
                        caster->CastSpell(GetHitUnit(), 61925, true);
                        caster->CastSpell(caster, SPELL_PASS_THE_CRANBERRIES_ACHIEVEMENT, true);
                        break;
                    case SPELL_PASS_THE_SWEET_POTATOES:
                        caster->CastSpell(GetHitUnit(), 61929, true);
                        caster->CastSpell(caster, SPELL_PASS_THE_SWEET_POTATOES_ACHIEVEMENT, true);
                        break;
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_piligrims_bounty_pass_food_SpellScript::HandleCheck);
                OnEffectHitTarget += SpellEffectFn(spell_piligrims_bounty_pass_food_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        private:
            uint32 m_spellId;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_piligrims_bounty_pass_food_SpellScript(m_spellId);
        }

    private:
        uint32 m_spellId;
};

class spell_piligrims_bounty_feast_on_food : public SpellScriptLoader
{
    public:
        spell_piligrims_bounty_feast_on_food(const char* name, uint32 spellId) : SpellScriptLoader(name), m_spellId(spellId) {}

        class spell_piligrims_bounty_feast_on_food_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_piligrims_bounty_feast_on_food_SpellScript);
        public:
            spell_piligrims_bounty_feast_on_food_SpellScript(uint32 spellId) : SpellScript(), m_spellId(spellId) {}

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                Unit* caster = GetCaster();
                if (caster->GetTypeId() != TYPEID_PLAYER)
                    if (Vehicle* vehicle = GetCaster()->GetVehicleKit())
                        caster = vehicle->GetPassenger(0);

                if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                // TODO: make well food spells

                UpdateHelpings(caster, m_spellId);

                if (CheckForSpiritOfSharing(caster))
                {
                    caster->RemoveAura(SPELL_PIE_HELPINGS);
                    caster->RemoveAura(SPELL_STUFFING_HELPINGS);
                    caster->RemoveAura(SPELL_SWEET_POTATO_HELPINGS);
                    caster->RemoveAura(SPELL_CRANBERRY_HELPINGS);
                    caster->RemoveAura(SPELL_TURKEY_HELPINGS);
                    caster->CastSpell(caster, SPELL_THE_SPIRIT_OF_SHARING, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_piligrims_bounty_feast_on_food_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        private:
            uint32 m_spellId;

            bool CheckForSpiritOfSharing(Unit* caster)
            {
                for (uint8 i = 0; i < 5; ++i)
                {
                    const uint32 auraList[5] = 
                    {
                        SPELL_PIE_HELPINGS,
                        SPELL_SWEET_POTATO_HELPINGS,
                        SPELL_STUFFING_HELPINGS,
                        SPELL_TURKEY_HELPINGS,
                        SPELL_CRANBERRY_HELPINGS
                    };

                    Aura const* aur = caster->GetAura(auraList[i]);
                    if (!aur)
                        return false;

                    if (aur->GetStackAmount() < 5)
                        return false;
                }

                return true;
            }

            void UpdateHelpings(Unit* caster, uint32 spellId)
            {
                switch (spellId)
                {
                    case SPELL_FEAST_ON_CRANBERRIES:
                        caster->CastSpell(caster, SPELL_CRANBERRY_HELPINGS, true);
                        break;
                    case SPELL_FEAST_ON_PIE:
                        caster->CastSpell(caster, SPELL_PIE_HELPINGS, true);
                        break;
                    case SPELL_FEAST_ON_SWEET_POTATOES:
                        caster->CastSpell(caster, SPELL_SWEET_POTATO_HELPINGS, true);
                        break;
                    case SPELL_FEAST_ON_STUFFING:
                        caster->CastSpell(caster, SPELL_STUFFING_HELPINGS, true);
                        break;
                    case SPELL_FEAST_ON_TURKEY:
                        caster->CastSpell(caster, SPELL_TURKEY_HELPINGS, true);
                        break;
                }
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_piligrims_bounty_feast_on_food_SpellScript(m_spellId);
        }

    private:
        uint32 m_spellId;
};

class spell_piligrims_bounty_turkey_tracker : public SpellScriptLoader
{
    public:
        spell_piligrims_bounty_turkey_tracker() : SpellScriptLoader("spell_piligrims_bounty_turkey_tracker") { }

        class spell_piligrims_bounty_turkey_tracker_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_piligrims_bounty_turkey_tracker_AuraScript);

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetUnitOwner())
                    return;

                Aura const* aur = GetAura();
                if (!aur)
                    return;

                if (aur->GetStackAmount() == 1)
                {
                    GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_TURKEY_MARKER, true);
                }
                else if (aur->GetStackAmount() == 10)
                {
                    GetUnitOwner()->MonsterTextEmote(LANG_THE_THUKINATOR_10, GetOwner()->GetGUID(), true);
                    GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_KILL_COUNTER_VISUAL, true);
                }
                else if (aur->GetStackAmount() == 15)
                {
                    GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_TURKEY_VENGEANCE, true);
                }
                else if (aur->GetStackAmount() == 20)
                {
                    GetUnitOwner()->MonsterTextEmote(LANG_THE_THUKINATOR_20, GetOwner()->GetGUID(), true);
                    GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_KILL_COUNTER_VISUAL, true);
                }
                else if (aur->GetStackAmount() == 30)
                {
                    GetUnitOwner()->MonsterTextEmote(LANG_THE_THUKINATOR_30, GetOwner()->GetGUID(), true);
                    GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_KILL_COUNTER_VISUAL, true);
                }
                else if (aur->GetStackAmount() == 40)
                {
                    GetUnitOwner()->MonsterTextEmote(LANG_THE_THUKINATOR_40, GetOwner()->GetGUID(), true);
                    GetUnitOwner()->RemoveAura(SPELL_TURKEY_MARKER);
                    GetUnitOwner()->RemoveAura(SPELL_TURKEY_TRACKER);
                    GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_KILL_COUNTER_VISUAL_MAX, true);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_piligrims_bounty_turkey_tracker_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_piligrims_bounty_turkey_tracker_AuraScript();
        }
};


void AddSC_holiday_spell_scripts()
{
    // Love is in the Air
    new spell_love_is_in_the_air_romantic_picnic();

    //Hallow's End
    new spell_trick();
    new spell_trick_or_treat();
    new spell_tricky_treat();

    new spell_piligrims_bounty_pass_food("spell_piligrims_bounty_pass_turkey", SPELL_PASS_THE_TURKEY);
    new spell_piligrims_bounty_pass_food("spell_piligrims_bounty_pass_stuffing", SPELL_PASS_THE_STUFFING);
    new spell_piligrims_bounty_pass_food("spell_piligrims_bounty_pass_pie", SPELL_PASS_THE_PIE);
    new spell_piligrims_bounty_pass_food("spell_piligrims_bounty_pass_cranberries", SPELL_PASS_THE_CRANBERRIES);
    new spell_piligrims_bounty_pass_food("spell_piligrims_bounty_pass_sweet_potatoes", SPELL_PASS_THE_SWEET_POTATOES);
    new spell_piligrims_bounty_feast_on_food("spell_piligrims_bounty_feast_on_pie", SPELL_FEAST_ON_PIE);
    new spell_piligrims_bounty_feast_on_food("spell_piligrims_bounty_feast_on_turkey", SPELL_FEAST_ON_TURKEY);
    new spell_piligrims_bounty_feast_on_food("spell_piligrims_bounty_feast_on_stuffing", SPELL_FEAST_ON_STUFFING);
    new spell_piligrims_bounty_feast_on_food("spell_piligrims_bounty_feast_on_cranberries", SPELL_FEAST_ON_CRANBERRIES);
    new spell_piligrims_bounty_feast_on_food("spell_piligrims_bounty_feast_on_sweet_potatoes", SPELL_FEAST_ON_SWEET_POTATOES);
    new spell_piligrims_bounty_turkey_tracker();
}

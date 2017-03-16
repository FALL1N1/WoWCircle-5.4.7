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
 * Scripts for spells with SPELLFAMILY_GENERIC spells used by items.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_item_".
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SkillDiscovery.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include <array>

enum DrumsOfRageEnum
{
    SPELL_MAGE_TEMPORAL_DISPLACEMENT = 80354,
    HUNTER_SPELL_INSANITY = 95809,
    SPELL_SHAMAN_SATED = 57724,
    SPELL_SHAMAN_EXHAUSTED = 57723,
};

// Generic script for handling item dummy effects which trigger another spell.
class spell_item_trigger_spell : public SpellScriptLoader
{
    private:
        uint32 _triggeredSpellId;

    public:
        spell_item_trigger_spell(const char* name, uint32 triggeredSpellId) : SpellScriptLoader(name), _triggeredSpellId(triggeredSpellId) { }

        class spell_item_trigger_spell_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_trigger_spell_SpellScript);
        private:
            uint32 _triggeredSpellId;

        public:
            spell_item_trigger_spell_SpellScript(uint32 triggeredSpellId) : SpellScript(), _triggeredSpellId(triggeredSpellId) { }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(_triggeredSpellId))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Item* item = GetCastItem())
                    caster->CastSpell(caster, _triggeredSpellId, true, item);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_trigger_spell_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_trigger_spell_SpellScript(_triggeredSpellId);
        }
};

// http://www.wowhead.com/item=6522 Deviate Fish
// 8063 Deviate Fish
enum DeviateFishSpells
{
    SPELL_SLEEPY            = 8064,
    SPELL_INVIGORATE        = 8065,
    SPELL_SHRINK            = 8066,
    SPELL_PARTY_TIME        = 8067,
    SPELL_HEALTHY_SPIRIT    = 8068,
};

class spell_item_deviate_fish : public SpellScriptLoader
{
    public:
        spell_item_deviate_fish() : SpellScriptLoader("spell_item_deviate_fish") { }

        class spell_item_deviate_fish_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_deviate_fish_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                for (uint32 spellId = SPELL_SLEEPY; spellId <= SPELL_HEALTHY_SPIRIT; ++spellId)
                    if (!sSpellMgr->GetSpellInfo(spellId))
                        return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = urand(SPELL_SLEEPY, SPELL_HEALTHY_SPIRIT);
                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_deviate_fish_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_deviate_fish_SpellScript();
        }
};

// http://www.wowhead.com/item=47499 Flask of the North
// 67019 Flask of the North
enum FlaskOfTheNorthSpells
{
    SPELL_FLASK_OF_THE_NORTH_SP = 67016,
    SPELL_FLASK_OF_THE_NORTH_AP = 67017,
    SPELL_FLASK_OF_THE_NORTH_STR = 67018,
};

class spell_item_flask_of_the_north : public SpellScriptLoader
{
    public:
        spell_item_flask_of_the_north() : SpellScriptLoader("spell_item_flask_of_the_north") { }

        class spell_item_flask_of_the_north_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_flask_of_the_north_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_SP) || !sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_AP) || !sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_STR))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                std::vector<uint32> possibleSpells;
                switch (caster->getClass())
                {
                    case CLASS_WARLOCK:
                    case CLASS_MAGE:
                    case CLASS_PRIEST:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                        break;
                    case CLASS_DEATH_KNIGHT:
                    case CLASS_WARRIOR:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_STR);
                        break;
                    case CLASS_ROGUE:
                    case CLASS_HUNTER:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_AP);
                        break;
                    case CLASS_DRUID:
                    case CLASS_PALADIN:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_STR);
                        break;
                    case CLASS_SHAMAN:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_AP);
                        break;
                }

                caster->CastSpell(caster, possibleSpells[irand(0, (possibleSpells.size() - 1))], true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_flask_of_the_north_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_flask_of_the_north_SpellScript();
        }
};

// http://www.wowhead.com/item=10645 Gnomish Death Ray
// 13280 Gnomish Death Ray
enum GnomishDeathRay
{
    SPELL_GNOMISH_DEATH_RAY_SELF = 13493,
    SPELL_GNOMISH_DEATH_RAY_TARGET = 13279,
};

class spell_item_gnomish_death_ray : public SpellScriptLoader
{
    public:
        spell_item_gnomish_death_ray() : SpellScriptLoader("spell_item_gnomish_death_ray") { }

        class spell_item_gnomish_death_ray_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_gnomish_death_ray_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_GNOMISH_DEATH_RAY_SELF) || !sSpellMgr->GetSpellInfo(SPELL_GNOMISH_DEATH_RAY_TARGET))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    if (urand(0, 99) < 15)
                        caster->CastSpell(caster, SPELL_GNOMISH_DEATH_RAY_SELF, true, NULL);    // failure
                    else
                        caster->CastSpell(target, SPELL_GNOMISH_DEATH_RAY_TARGET, true, NULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_gnomish_death_ray_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_gnomish_death_ray_SpellScript();
        }
};

// http://www.wowhead.com/item=27388 Mr. Pinchy
// 33060 Make a Wish
enum MakeAWish
{
    SPELL_MR_PINCHYS_BLESSING       = 33053,
    SPELL_SUMMON_MIGHTY_MR_PINCHY   = 33057,
    SPELL_SUMMON_FURIOUS_MR_PINCHY  = 33059,
    SPELL_TINY_MAGICAL_CRAWDAD      = 33062,
    SPELL_MR_PINCHYS_GIFT           = 33064,
};

class spell_item_make_a_wish : public SpellScriptLoader
{
    public:
        spell_item_make_a_wish() : SpellScriptLoader("spell_item_make_a_wish") { }

        class spell_item_make_a_wish_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_make_a_wish_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MR_PINCHYS_BLESSING) || !sSpellMgr->GetSpellInfo(SPELL_SUMMON_MIGHTY_MR_PINCHY) || !sSpellMgr->GetSpellInfo(SPELL_SUMMON_FURIOUS_MR_PINCHY) || !sSpellMgr->GetSpellInfo(SPELL_TINY_MAGICAL_CRAWDAD) || !sSpellMgr->GetSpellInfo(SPELL_MR_PINCHYS_GIFT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = SPELL_MR_PINCHYS_GIFT;
                switch (urand(1, 5))
                {
                    case 1: spellId = SPELL_MR_PINCHYS_BLESSING; break;
                    case 2: spellId = SPELL_SUMMON_MIGHTY_MR_PINCHY; break;
                    case 3: spellId = SPELL_SUMMON_FURIOUS_MR_PINCHY; break;
                    case 4: spellId = SPELL_TINY_MAGICAL_CRAWDAD; break;
                }
                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_make_a_wish_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_make_a_wish_SpellScript();
        }
};

// http://www.wowhead.com/item=32686 Mingo's Fortune Giblets
// 40802 Mingo's Fortune Generator
class spell_item_mingos_fortune_generator : public SpellScriptLoader
{
    public:
        spell_item_mingos_fortune_generator() : SpellScriptLoader("spell_item_mingos_fortune_generator") { }

        class spell_item_mingos_fortune_generator_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_mingos_fortune_generator_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                // Selecting one from Bloodstained Fortune item
                uint32 newitemid;
                switch (urand(1, 20))
                {
                    case 1:  newitemid = 32688; break;
                    case 2:  newitemid = 32689; break;
                    case 3:  newitemid = 32690; break;
                    case 4:  newitemid = 32691; break;
                    case 5:  newitemid = 32692; break;
                    case 6:  newitemid = 32693; break;
                    case 7:  newitemid = 32700; break;
                    case 8:  newitemid = 32701; break;
                    case 9:  newitemid = 32702; break;
                    case 10: newitemid = 32703; break;
                    case 11: newitemid = 32704; break;
                    case 12: newitemid = 32705; break;
                    case 13: newitemid = 32706; break;
                    case 14: newitemid = 32707; break;
                    case 15: newitemid = 32708; break;
                    case 16: newitemid = 32709; break;
                    case 17: newitemid = 32710; break;
                    case 18: newitemid = 32711; break;
                    case 19: newitemid = 32712; break;
                    case 20: newitemid = 32713; break;
                    default:
                        return;
                }

                CreateItem(effIndex, newitemid);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_mingos_fortune_generator_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_mingos_fortune_generator_SpellScript();
        }
};

// http://www.wowhead.com/item=10720 Gnomish Net-o-Matic Projector
// 13120 Net-o-Matic
enum NetOMaticSpells
{
    SPELL_NET_O_MATIC_TRIGGERED1 = 16566,
    SPELL_NET_O_MATIC_TRIGGERED2 = 13119,
    SPELL_NET_O_MATIC_TRIGGERED3 = 13099,
};

class spell_item_net_o_matic : public SpellScriptLoader
{
    public:
        spell_item_net_o_matic() : SpellScriptLoader("spell_item_net_o_matic") { }

        class spell_item_net_o_matic_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_net_o_matic_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED1) || !sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED2) || !sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED3))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    uint32 spellId = SPELL_NET_O_MATIC_TRIGGERED3;
                    uint32 roll = urand(0, 99);
                    if (roll < 2)                            // 2% for 30 sec self root (off-like chance unknown)
                        spellId = SPELL_NET_O_MATIC_TRIGGERED1;
                    else if (roll < 4)                       // 2% for 20 sec root, charge to target (off-like chance unknown)
                        spellId = SPELL_NET_O_MATIC_TRIGGERED2;

                    GetCaster()->CastSpell(target, spellId, true, NULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_net_o_matic_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_net_o_matic_SpellScript();
        }
};

// http://www.wowhead.com/item=8529 Noggenfogger Elixir
// 16589 Noggenfogger Elixir
enum NoggenfoggerElixirSpells
{
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1 = 16595,
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2 = 16593,
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3 = 16591,
};

class spell_item_noggenfogger_elixir : public SpellScriptLoader
{
    public:
        spell_item_noggenfogger_elixir() : SpellScriptLoader("spell_item_noggenfogger_elixir") { }

        class spell_item_noggenfogger_elixir_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_noggenfogger_elixir_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1) || !sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2) || !sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3;
                switch (urand(1, 3))
                {
                    case 1: spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1; break;
                    case 2: spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2; break;
                }

                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_noggenfogger_elixir_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_noggenfogger_elixir_SpellScript();
        }
};

// http://www.wowhead.com/item=6657 Savory Deviate Delight
// 8213 Savory Deviate Delight
enum SavoryDeviateDelight
{
    SPELL_FLIP_OUT_MALE     = 8219,
    SPELL_FLIP_OUT_FEMALE   = 8220,
    SPELL_YAAARRRR_MALE     = 8221,
    SPELL_YAAARRRR_FEMALE   = 8222,
};

class spell_item_savory_deviate_delight : public SpellScriptLoader
{
    public:
        spell_item_savory_deviate_delight() : SpellScriptLoader("spell_item_savory_deviate_delight") { }

        class spell_item_savory_deviate_delight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_savory_deviate_delight_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                for (uint32 spellId = SPELL_FLIP_OUT_MALE; spellId <= SPELL_YAAARRRR_FEMALE; ++spellId)
                    if (!sSpellMgr->GetSpellInfo(spellId))
                        return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = 0;
                switch (urand(1, 2))
                {
                    // Flip Out - ninja
                    case 1: spellId = (caster->getGender() == GENDER_MALE ? SPELL_FLIP_OUT_MALE : SPELL_FLIP_OUT_FEMALE); break;
                    // Yaaarrrr - pirate
                    case 2: spellId = (caster->getGender() == GENDER_MALE ? SPELL_YAAARRRR_MALE : SPELL_YAAARRRR_FEMALE); break;
                }
                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_savory_deviate_delight_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_savory_deviate_delight_SpellScript();
        }
};

// http://www.wowhead.com/item=7734 Six Demon Bag
// 14537 Six Demon Bag
enum SixDemonBagSpells
{
    SPELL_FROSTBOLT                 = 11538,
    SPELL_POLYMORPH                 = 14621,
    SPELL_SUMMON_FELHOUND_MINION    = 14642,
    SPELL_FIREBALL                  = 15662,
    SPELL_CHAIN_LIGHTNING           = 21179,
    SPELL_ENVELOPING_WINDS          = 25189,
};

class spell_item_six_demon_bag : public SpellScriptLoader
{
    public:
        spell_item_six_demon_bag() : SpellScriptLoader("spell_item_six_demon_bag") { }

        class spell_item_six_demon_bag_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_six_demon_bag_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FROSTBOLT) || !sSpellMgr->GetSpellInfo(SPELL_POLYMORPH) || !sSpellMgr->GetSpellInfo(SPELL_SUMMON_FELHOUND_MINION) || !sSpellMgr->GetSpellInfo(SPELL_FIREBALL) || !sSpellMgr->GetSpellInfo(SPELL_CHAIN_LIGHTNING) || !sSpellMgr->GetSpellInfo(SPELL_ENVELOPING_WINDS))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    uint32 spellId = 0;
                    uint32 rand = urand(0, 99);
                    if (rand < 25)                      // Fireball (25% chance)
                        spellId = SPELL_FIREBALL;
                    else if (rand < 50)                 // Frostball (25% chance)
                        spellId = SPELL_FROSTBOLT;
                    else if (rand < 70)                 // Chain Lighting (20% chance)
                        spellId = SPELL_CHAIN_LIGHTNING;
                    else if (rand < 80)                 // Polymorph (10% chance)
                    {
                        spellId = SPELL_POLYMORPH;
                        if (urand(0, 100) <= 30)        // 30% chance to self-cast
                            target = caster;
                    }
                    else if (rand < 95)                 // Enveloping Winds (15% chance)
                        spellId = SPELL_ENVELOPING_WINDS;
                    else                                // Summon Felhund minion (5% chance)
                    {
                        spellId = SPELL_SUMMON_FELHOUND_MINION;
                        target = caster;
                    }

                    caster->CastSpell(target, spellId, true, GetCastItem());
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_six_demon_bag_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_six_demon_bag_SpellScript();
        }
};

// http://www.wowhead.com/item=44012 Underbelly Elixir
// 59640 Underbelly Elixir
enum UnderbellyElixirSpells
{
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED1 = 59645,
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED2 = 59831,
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED3 = 59843,
};

class spell_item_underbelly_elixir : public SpellScriptLoader
{
    public:
        spell_item_underbelly_elixir() : SpellScriptLoader("spell_item_underbelly_elixir") { }

        class spell_item_underbelly_elixir_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_underbelly_elixir_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }
            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED1) || !sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED2) || !sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED3))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED3;
                switch (urand(1, 3))
                {
                    case 1: spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED1; break;
                    case 2: spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED2; break;
                }
                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_underbelly_elixir_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_underbelly_elixir_SpellScript();
        }
};

enum eShadowmourneVisuals
{
    SPELL_SHADOWMOURNE_VISUAL_LOW       = 72521,
    SPELL_SHADOWMOURNE_VISUAL_HIGH      = 72523,
    SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF  = 73422,
};

class spell_item_shadowmourne : public SpellScriptLoader
{
public:
    spell_item_shadowmourne() : SpellScriptLoader("spell_item_shadowmourne") { }

    class spell_item_shadowmourne_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_item_shadowmourne_AuraScript);

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_VISUAL_LOW) || !sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_VISUAL_HIGH) || !sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF))
                return false;
            return true;
        }

        void OnStackChange(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            switch (GetStackAmount())
            {
                case 1:
                    target->CastSpell(target, SPELL_SHADOWMOURNE_VISUAL_LOW, true);
                    break;
                case 6:
                    target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_LOW);
                    target->CastSpell(target, SPELL_SHADOWMOURNE_VISUAL_HIGH, true);
                    break;
                case 10:
                    target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_HIGH);
                    target->CastSpell(target, SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF, true);
                    break;
                default:
                    break;
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_LOW);
            target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_HIGH);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_item_shadowmourne_AuraScript::OnStackChange, EFFECT_0, SPELL_AURA_MOD_STAT, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY));
            AfterEffectRemove += AuraEffectRemoveFn(spell_item_shadowmourne_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_item_shadowmourne_AuraScript();
    }
};

enum AirRifleSpells
{
    SPELL_AIR_RIFLE_HOLD_VISUAL = 65582,
    SPELL_AIR_RIFLE_SHOOT       = 67532,
    SPELL_AIR_RIFLE_SHOOT_SELF  = 65577,
};

class spell_item_red_rider_air_rifle : public SpellScriptLoader
{
    public:
        spell_item_red_rider_air_rifle() : SpellScriptLoader("spell_item_red_rider_air_rifle") { }

        class spell_item_red_rider_air_rifle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_red_rider_air_rifle_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_HOLD_VISUAL) || !sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_SHOOT) || !sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_SHOOT_SELF))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    caster->CastSpell(caster, SPELL_AIR_RIFLE_HOLD_VISUAL, true);
                    // needed because this spell shares GCD with its triggered spells (which must not be cast with triggered flag)
                    if (Player* player = caster->ToPlayer())
                        player->GetGlobalCooldownMgr().CancelGlobalCooldown(GetSpellInfo());
                    if (urand(0, 4))
                        caster->CastSpell(target, SPELL_AIR_RIFLE_SHOOT, false);
                    else
                        caster->CastSpell(caster, SPELL_AIR_RIFLE_SHOOT_SELF, false);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_red_rider_air_rifle_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_red_rider_air_rifle_SpellScript();
        }
};

enum GenericData
{
    SPELL_ARCANITE_DRAGONLING           = 19804,
    SPELL_BATTLE_CHICKEN                = 13166,
    SPELL_MECHANICAL_DRAGONLING         = 4073,
    SPELL_MITHRIL_MECHANICAL_DRAGONLING = 12749,
};

enum CreateHeartCandy
{
    ITEM_HEART_CANDY_1 = 21818,
    ITEM_HEART_CANDY_2 = 21817,
    ITEM_HEART_CANDY_3 = 21821,
    ITEM_HEART_CANDY_4 = 21819,
    ITEM_HEART_CANDY_5 = 21816,
    ITEM_HEART_CANDY_6 = 21823,
    ITEM_HEART_CANDY_7 = 21822,
    ITEM_HEART_CANDY_8 = 21820,
};

class spell_item_create_heart_candy : public SpellScriptLoader
{
    public:
        spell_item_create_heart_candy() : SpellScriptLoader("spell_item_create_heart_candy") { }

        class spell_item_create_heart_candy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_create_heart_candy_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Player* target = GetHitPlayer())
                {
                    static const uint32 items[] = {ITEM_HEART_CANDY_1, ITEM_HEART_CANDY_2, ITEM_HEART_CANDY_3, ITEM_HEART_CANDY_4, ITEM_HEART_CANDY_5, ITEM_HEART_CANDY_6, ITEM_HEART_CANDY_7, ITEM_HEART_CANDY_8};
                    target->AddItem(items[urand(0, 7)], 1);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_create_heart_candy_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_create_heart_candy_SpellScript();
        }
};

class spell_item_book_of_glyph_mastery : public SpellScriptLoader
{
    public:
        spell_item_book_of_glyph_mastery() : SpellScriptLoader("spell_item_book_of_glyph_mastery") {}

        class spell_item_book_of_glyph_mastery_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_book_of_glyph_mastery_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            SpellCastResult CheckRequirement()
            {
                if (HasDiscoveredAllSpells(GetSpellInfo()->Id, GetCaster()->ToPlayer()))
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_LEARNED_EVERYTHING);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_book_of_glyph_mastery_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_book_of_glyph_mastery_SpellScript();
        }
};

enum GiftOfTheHarvester
{
    NPC_GHOUL   = 28845,
    MAX_GHOULS  = 5,
};

class spell_item_gift_of_the_harvester : public SpellScriptLoader
{
    public:
        spell_item_gift_of_the_harvester() : SpellScriptLoader("spell_item_gift_of_the_harvester") {}

        class spell_item_gift_of_the_harvester_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_gift_of_the_harvester_SpellScript);

            SpellCastResult CheckRequirement()
            {
                std::list<Creature*> ghouls;
                GetCaster()->GetAllMinionsByEntry(ghouls, NPC_GHOUL);
                if (ghouls.size() >= MAX_GHOULS)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_TOO_MANY_GHOULS);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_gift_of_the_harvester_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_gift_of_the_harvester_SpellScript();
        }
};

enum Sinkholes
{
    NPC_SOUTH_SINKHOLE      = 25664,
    NPC_NORTHEAST_SINKHOLE  = 25665,
    NPC_NORTHWEST_SINKHOLE  = 25666,
};

class spell_item_map_of_the_geyser_fields : public SpellScriptLoader
{
    public:
        spell_item_map_of_the_geyser_fields() : SpellScriptLoader("spell_item_map_of_the_geyser_fields") {}

        class spell_item_map_of_the_geyser_fields_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_map_of_the_geyser_fields_SpellScript);

            SpellCastResult CheckSinkholes()
            {
                Unit* caster = GetCaster();
                if (caster->FindNearestCreature(NPC_SOUTH_SINKHOLE, 30.0f, true) ||
                    caster->FindNearestCreature(NPC_NORTHEAST_SINKHOLE, 30.0f, true) ||
                    caster->FindNearestCreature(NPC_NORTHWEST_SINKHOLE, 30.0f, true))
                    return SPELL_CAST_OK;

                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_CLOSE_TO_SINKHOLE);
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_map_of_the_geyser_fields_SpellScript::CheckSinkholes);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_map_of_the_geyser_fields_SpellScript();
        }
};

enum VanquishedClutchesSpells
{
    SPELL_CRUSHER       = 64982,
    SPELL_CONSTRICTOR   = 64983,
    SPELL_CORRUPTOR     = 64984,
};

class spell_item_vanquished_clutches : public SpellScriptLoader
{
    public:
        spell_item_vanquished_clutches() : SpellScriptLoader("spell_item_vanquished_clutches") { }

        class spell_item_vanquished_clutches_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_vanquished_clutches_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CRUSHER) || !sSpellMgr->GetSpellInfo(SPELL_CONSTRICTOR) || !sSpellMgr->GetSpellInfo(SPELL_CORRUPTOR))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint32 spellId = RAND(SPELL_CRUSHER, SPELL_CONSTRICTOR, SPELL_CORRUPTOR);
                Unit* caster = GetCaster();
                caster->CastSpell(caster, spellId, true);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_vanquished_clutches_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_vanquished_clutches_SpellScript();
        }
};

enum MagicEater
{
    SPELL_WILD_MAGIC                             = 58891,
    SPELL_WELL_FED_1                             = 57288,
    SPELL_WELL_FED_2                             = 57139,
    SPELL_WELL_FED_3                             = 57111,
    SPELL_WELL_FED_4                             = 57286,
    SPELL_WELL_FED_5                             = 57291,
};

class spell_magic_eater_food : public SpellScriptLoader
{
    public:
        spell_magic_eater_food() : SpellScriptLoader("spell_magic_eater_food") {}

        class spell_magic_eater_food_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_magic_eater_food_AuraScript);

            void HandleTriggerSpell(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                Unit* target = GetTarget();
                switch (urand(0, 5))
                {
                    case 0:
                        target->CastSpell(target, SPELL_WILD_MAGIC, true);
                        break;
                    case 1:
                        target->CastSpell(target, SPELL_WELL_FED_1, true);
                        break;
                    case 2:
                        target->CastSpell(target, SPELL_WELL_FED_2, true);
                        break;
                    case 3:
                        target->CastSpell(target, SPELL_WELL_FED_3, true);
                        break;
                    case 4:
                        target->CastSpell(target, SPELL_WELL_FED_4, true);
                        break;
                    case 5:
                        target->CastSpell(target, SPELL_WELL_FED_5, true);
                        break;
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_magic_eater_food_AuraScript::HandleTriggerSpell, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_magic_eater_food_AuraScript();
        }
};

class spell_item_shimmering_vessel : public SpellScriptLoader
{
    public:
        spell_item_shimmering_vessel() : SpellScriptLoader("spell_item_shimmering_vessel") { }

        class spell_item_shimmering_vessel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_shimmering_vessel_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (Creature* target = GetHitCreature())
                    target->setDeathState(JUST_RESPAWNED);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_shimmering_vessel_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_shimmering_vessel_SpellScript();
        }
};

enum PurifyHelboarMeat
{
    SPELL_SUMMON_PURIFIED_HELBOAR_MEAT      = 29277,
    SPELL_SUMMON_TOXIC_HELBOAR_MEAT         = 29278,
    ITEM_PURIFIED_HELBOAR_MEAT              = 23248,
};

class spell_item_purify_helboar_meat : public SpellScriptLoader
{
    public:
        spell_item_purify_helboar_meat() : SpellScriptLoader("spell_item_purify_helboar_meat") { }

        class spell_item_purify_helboar_meat_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_purify_helboar_meat_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SUMMON_PURIFIED_HELBOAR_MEAT) ||  !sSpellMgr->GetSpellInfo(SPELL_SUMMON_TOXIC_HELBOAR_MEAT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (roll_chance_i(50))
                    caster->CastSpell(caster, SPELL_SUMMON_TOXIC_HELBOAR_MEAT, true);
                else
                    if (caster->GetTypeId() == TYPEID_PLAYER)
                        caster->ToPlayer()->AddItem(ITEM_PURIFIED_HELBOAR_MEAT, 1);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_purify_helboar_meat_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_purify_helboar_meat_SpellScript();
        }
};

enum CrystalPrison
{
    OBJECT_IMPRISONED_DOOMGUARD     = 179644,
};

class spell_item_crystal_prison_dummy_dnd : public SpellScriptLoader
{
    public:
        spell_item_crystal_prison_dummy_dnd() : SpellScriptLoader("spell_item_crystal_prison_dummy_dnd") { }

        class spell_item_crystal_prison_dummy_dnd_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_crystal_prison_dummy_dnd_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sObjectMgr->GetGameObjectTemplate(OBJECT_IMPRISONED_DOOMGUARD))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (Creature* target = GetHitCreature())
                    if (target->isDead() && !target->isPet())
                    {
                        GetCaster()->SummonGameObject(OBJECT_IMPRISONED_DOOMGUARD, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 0, 0, 0, 0, uint32(target->GetRespawnTime()-time(NULL)));
                        target->DespawnOrUnsummon();
                    }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_crystal_prison_dummy_dnd_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_crystal_prison_dummy_dnd_SpellScript();
        }
};

enum ReindeerTransformation
{
    SPELL_FLYING_REINDEER_310                   = 44827,
    SPELL_FLYING_REINDEER_280                   = 44825,
    SPELL_FLYING_REINDEER_60                    = 44824,
    SPELL_REINDEER_100                          = 25859,
    SPELL_REINDEER_60                           = 25858,
};

class spell_item_reindeer_transformation : public SpellScriptLoader
{
    public:
        spell_item_reindeer_transformation() : SpellScriptLoader("spell_item_reindeer_transformation") { }

        class spell_item_reindeer_transformation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_reindeer_transformation_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FLYING_REINDEER_310) || !sSpellMgr->GetSpellInfo(SPELL_FLYING_REINDEER_280)
                    || !sSpellMgr->GetSpellInfo(SPELL_FLYING_REINDEER_60) || !sSpellMgr->GetSpellInfo(SPELL_REINDEER_100)
                    || !sSpellMgr->GetSpellInfo(SPELL_REINDEER_60))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (caster->HasAuraType(SPELL_AURA_MOUNTED))
                {
                    float flyspeed = caster->GetSpeedRate(MOVE_FLIGHT);
                    float speed = caster->GetSpeedRate(MOVE_RUN);

                    caster->RemoveAurasByType(SPELL_AURA_MOUNTED);
                    //5 different spells used depending on mounted speed and if mount can fly or not

                    if (flyspeed >= 4.1f)
                        // Flying Reindeer
                        caster->CastSpell(caster, SPELL_FLYING_REINDEER_310, true); //310% flying Reindeer
                    else if (flyspeed >= 3.8f)
                        // Flying Reindeer
                        caster->CastSpell(caster, SPELL_FLYING_REINDEER_280, true); //280% flying Reindeer
                    else if (flyspeed >= 1.6f)
                        // Flying Reindeer
                        caster->CastSpell(caster, SPELL_FLYING_REINDEER_60, true); //60% flying Reindeer
                    else if (speed >= 2.0f)
                        // Reindeer
                        caster->CastSpell(caster, SPELL_REINDEER_100, true); //100% ground Reindeer
                    else
                        // Reindeer
                        caster->CastSpell(caster, SPELL_REINDEER_60, true); //60% ground Reindeer
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_reindeer_transformation_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_reindeer_transformation_SpellScript();
    }
};

enum NighInvulnerability
{
    SPELL_NIGH_INVULNERABILITY                  = 30456,
    SPELL_COMPLETE_VULNERABILITY                = 30457,
};

class spell_item_nigh_invulnerability : public SpellScriptLoader
{
    public:
        spell_item_nigh_invulnerability() : SpellScriptLoader("spell_item_nigh_invulnerability") { }

        class spell_item_nigh_invulnerability_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_nigh_invulnerability_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_NIGH_INVULNERABILITY) || !sSpellMgr->GetSpellInfo(SPELL_COMPLETE_VULNERABILITY))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (Item* castItem = GetCastItem())
                {
                    if (roll_chance_i(86))                  // Nigh-Invulnerability   - success
                        caster->CastSpell(caster, SPELL_NIGH_INVULNERABILITY, true, castItem);
                    else                                    // Complete Vulnerability - backfire in 14% casts
                        caster->CastSpell(caster, SPELL_COMPLETE_VULNERABILITY, true, castItem);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_nigh_invulnerability_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_nigh_invulnerability_SpellScript();
        }
};

enum Poultryzer
{
    SPELL_POULTRYIZER_SUCCESS    = 30501,
    SPELL_POULTRYIZER_BACKFIRE   = 30504,
};

class spell_item_poultryizer : public SpellScriptLoader
{
    public:
        spell_item_poultryizer() : SpellScriptLoader("spell_item_poultryizer") { }

        class spell_item_poultryizer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_poultryizer_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_POULTRYIZER_SUCCESS) || !sSpellMgr->GetSpellInfo(SPELL_POULTRYIZER_BACKFIRE))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (GetCastItem() && GetHitUnit())
                    GetCaster()->CastSpell(GetHitUnit(), roll_chance_i(80) ? SPELL_POULTRYIZER_SUCCESS : SPELL_POULTRYIZER_BACKFIRE , true, GetCastItem());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_poultryizer_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_poultryizer_SpellScript();
        }
};

enum SocretharsStone
{
    SPELL_SOCRETHAR_TO_SEAT     = 35743,
    SPELL_SOCRETHAR_FROM_SEAT   = 35744,
};

class spell_item_socrethars_stone : public SpellScriptLoader
{
    public:
        spell_item_socrethars_stone() : SpellScriptLoader("spell_item_socrethars_stone") { }

        class spell_item_socrethars_stone_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_socrethars_stone_SpellScript);

            bool Load()
            {
                return (GetCaster()->GetAreaId() == 3900 || GetCaster()->GetAreaId() == 3742);
            }
            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SOCRETHAR_TO_SEAT) || !sSpellMgr->GetSpellInfo(SPELL_SOCRETHAR_FROM_SEAT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                switch (caster->GetAreaId())
                {
                    case 3900:
                        caster->CastSpell(caster, SPELL_SOCRETHAR_TO_SEAT, true);
                        break;
                    case 3742:
                        caster->CastSpell(caster, SPELL_SOCRETHAR_FROM_SEAT, true);
                        break;
                    default:
                        return;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_socrethars_stone_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_socrethars_stone_SpellScript();
        }
};

enum DemonBroiledSurprise
{
    QUEST_SUPER_HOT_STEW                    = 11379,
    SPELL_CREATE_DEMON_BROILED_SURPRISE     = 43753,
    NPC_ABYSSAL_FLAMEBRINGER                = 19973,
};

class spell_item_demon_broiled_surprise : public SpellScriptLoader
{
    public:
        spell_item_demon_broiled_surprise() : SpellScriptLoader("spell_item_demon_broiled_surprise") { }

        class spell_item_demon_broiled_surprise_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_demon_broiled_surprise_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CREATE_DEMON_BROILED_SURPRISE) || !sObjectMgr->GetCreatureTemplate(NPC_ABYSSAL_FLAMEBRINGER) || !sObjectMgr->GetQuestTemplate(QUEST_SUPER_HOT_STEW))
                    return false;
                return true;
            }

            bool Load()
            {
               return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* player = GetCaster();
                player->CastSpell(player, SPELL_CREATE_DEMON_BROILED_SURPRISE, false);
            }

            SpellCastResult CheckRequirement()
            {
                Player* player = GetCaster()->ToPlayer();
                if (player->GetQuestStatus(QUEST_SUPER_HOT_STEW) != QUEST_STATUS_INCOMPLETE)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                if (Creature* creature = player->FindNearestCreature(NPC_ABYSSAL_FLAMEBRINGER, 10, false))
                    if (creature->isDead())
                        return SPELL_CAST_OK;
                return SPELL_FAILED_NOT_HERE;
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_demon_broiled_surprise_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_item_demon_broiled_surprise_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_demon_broiled_surprise_SpellScript();
        }
};

enum CompleteRaptorCapture
{
    SPELL_RAPTOR_CAPTURE_CREDIT     = 42337,
};

class spell_item_complete_raptor_capture : public SpellScriptLoader
{
    public:
        spell_item_complete_raptor_capture() : SpellScriptLoader("spell_item_complete_raptor_capture") { }

        class spell_item_complete_raptor_capture_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_complete_raptor_capture_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_RAPTOR_CAPTURE_CREDIT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (GetHitCreature())
                {
                    GetHitCreature()->DespawnOrUnsummon();

                    //cast spell Raptor Capture Credit
                    caster->CastSpell(caster, SPELL_RAPTOR_CAPTURE_CREDIT, true, NULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_complete_raptor_capture_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_complete_raptor_capture_SpellScript();
        }
};

enum ImpaleLeviroth
{
    NPC_LEVIROTH                = 26452,
    SPELL_LEVIROTH_SELF_IMPALE  = 49882,
};

class spell_item_impale_leviroth : public SpellScriptLoader
{
    public:
        spell_item_impale_leviroth() : SpellScriptLoader("spell_item_impale_leviroth") { }

        class spell_item_impale_leviroth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_impale_leviroth_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sObjectMgr->GetCreatureTemplate(NPC_LEVIROTH))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (Unit* target = GetHitCreature())
                    if (target->GetEntry() == NPC_LEVIROTH && !target->HealthBelowPct(95))
                        target->CastSpell(target, SPELL_LEVIROTH_SELF_IMPALE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_impale_leviroth_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_impale_leviroth_SpellScript();
        }
};

enum BrewfestMountTransformation
{
    SPELL_MOUNT_RAM_100                         = 43900,
    SPELL_MOUNT_RAM_60                          = 43899,
    SPELL_MOUNT_KODO_100                        = 49379,
    SPELL_MOUNT_KODO_60                         = 49378,
    SPELL_BREWFEST_MOUNT_TRANSFORM              = 49357,
    SPELL_BREWFEST_MOUNT_TRANSFORM_REVERSE      = 52845,
};

class spell_item_brewfest_mount_transformation : public SpellScriptLoader
{
    public:
        spell_item_brewfest_mount_transformation() : SpellScriptLoader("spell_item_brewfest_mount_transformation") { }

        class spell_item_brewfest_mount_transformation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_brewfest_mount_transformation_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MOUNT_RAM_100) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_RAM_60) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_KODO_100) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_KODO_60))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                if (caster->HasAuraType(SPELL_AURA_MOUNTED))
                {
                    caster->RemoveAurasByType(SPELL_AURA_MOUNTED);
                    uint32 spell_id;

                    switch (GetSpellInfo()->Id)
                    {
                        case SPELL_BREWFEST_MOUNT_TRANSFORM:
                            if (caster->GetSpeedRate(MOVE_RUN) >= 2.0f)
                                spell_id = caster->GetTeam() == ALLIANCE ? SPELL_MOUNT_RAM_100 : SPELL_MOUNT_KODO_100;
                            else
                                spell_id = caster->GetTeam() == ALLIANCE ? SPELL_MOUNT_RAM_60 : SPELL_MOUNT_KODO_60;
                            break;
                        case SPELL_BREWFEST_MOUNT_TRANSFORM_REVERSE:
                            if (caster->GetSpeedRate(MOVE_RUN) >= 2.0f)
                                spell_id = caster->GetTeam() == HORDE ? SPELL_MOUNT_RAM_100 : SPELL_MOUNT_KODO_100;
                            else
                                spell_id = caster->GetTeam() == HORDE ? SPELL_MOUNT_RAM_60 : SPELL_MOUNT_KODO_60;
                            break;
                        default:
                            return;
                    }
                    caster->CastSpell(caster, spell_id, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_brewfest_mount_transformation_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_brewfest_mount_transformation_SpellScript();
        }
};

enum NitroBoots
{
    SPELL_NITRO_BOOTS_SUCCESS       = 54861,
    SPELL_NITRO_BOOTS_BACKFIRE      = 46014,
};

class spell_item_nitro_boots : public SpellScriptLoader
{
    public:
        spell_item_nitro_boots() : SpellScriptLoader("spell_item_nitro_boots") { }

        class spell_item_nitro_boots_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_nitro_boots_SpellScript);

            bool Load()
            {
                if (!GetCastItem())
                    return false;
                return true;
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_NITRO_BOOTS_SUCCESS) || !sSpellMgr->GetSpellInfo(SPELL_NITRO_BOOTS_BACKFIRE))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                caster->CastSpell(caster, roll_chance_i(95) ? SPELL_NITRO_BOOTS_SUCCESS : SPELL_NITRO_BOOTS_BACKFIRE, true, GetCastItem());
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_nitro_boots_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_nitro_boots_SpellScript();
        }
};

enum TeachLanguage
{
    SPELL_LEARN_GNOMISH_BINARY      = 50242,
    SPELL_LEARN_GOBLIN_BINARY       = 50246,
};

class spell_item_teach_language : public SpellScriptLoader
{
    public:
        spell_item_teach_language() : SpellScriptLoader("spell_item_teach_language") { }

        class spell_item_teach_language_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_teach_language_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_LEARN_GNOMISH_BINARY) || !sSpellMgr->GetSpellInfo(SPELL_LEARN_GOBLIN_BINARY))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();

                if (roll_chance_i(34))
                    caster->CastSpell(caster,caster->GetTeam() == ALLIANCE ? SPELL_LEARN_GNOMISH_BINARY : SPELL_LEARN_GOBLIN_BINARY, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_teach_language_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_teach_language_SpellScript();
        }
};

enum RocketBoots
{
    SPELL_ROCKET_BOOTS_PROC      = 30452,
};

class spell_item_rocket_boots : public SpellScriptLoader
{
    public:
        spell_item_rocket_boots() : SpellScriptLoader("spell_item_rocket_boots") { }

        class spell_item_rocket_boots_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_rocket_boots_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ROCKET_BOOTS_PROC))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (!GetCaster() || !GetCaster()->ToPlayer())
                    return;

                Player* caster = GetCaster()->ToPlayer();
                if (Battleground* bg = caster->GetBattleground())
                    bg->EventPlayerDroppedFlag(caster);

                caster->RemoveSpellCooldown(SPELL_ROCKET_BOOTS_PROC);
                caster->CastSpell(caster, SPELL_ROCKET_BOOTS_PROC, true, NULL);
            }

            SpellCastResult CheckCast()
            {
                if (GetCaster()->IsInWater())
                    return SPELL_FAILED_ONLY_ABOVEWATER;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_rocket_boots_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_item_rocket_boots_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_rocket_boots_SpellScript();
        }
};

enum PygmyOil
{
    SPELL_PYGMY_OIL_PYGMY_AURA      = 53806,
    SPELL_PYGMY_OIL_SMALLER_AURA    = 53805,
};

class spell_item_pygmy_oil : public SpellScriptLoader
{
    public:
        spell_item_pygmy_oil() : SpellScriptLoader("spell_item_pygmy_oil") { }

        class spell_item_pygmy_oil_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_pygmy_oil_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PYGMY_OIL_PYGMY_AURA) || !sSpellMgr->GetSpellInfo(SPELL_PYGMY_OIL_SMALLER_AURA))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (Aura* aura = caster->GetAura(SPELL_PYGMY_OIL_PYGMY_AURA))
                    aura->RefreshDuration();
                else
                {
                    aura = caster->GetAura(SPELL_PYGMY_OIL_SMALLER_AURA);
                    if (!aura || aura->GetStackAmount() < 5 || !roll_chance_i(50))
                         caster->CastSpell(caster, SPELL_PYGMY_OIL_SMALLER_AURA, true);
                    else
                    {
                        aura->Remove();
                        caster->CastSpell(caster, SPELL_PYGMY_OIL_PYGMY_AURA, true);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_pygmy_oil_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_pygmy_oil_SpellScript();
        }
};

class spell_item_unusual_compass : public SpellScriptLoader
{
    public:
        spell_item_unusual_compass() : SpellScriptLoader("spell_item_unusual_compass") { }

        class spell_item_unusual_compass_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_unusual_compass_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                caster->SetOrientation(frand(0.0f, 62832.0f) / 10000.0f);
                caster->SendMovementFlagUpdate();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_unusual_compass_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_unusual_compass_SpellScript();
        }
};

enum ChickenCover
{
    SPELL_CHICKEN_NET               = 51959,
    SPELL_CAPTURE_CHICKEN_ESCAPE    = 51037,
    QUEST_CHICKEN_PARTY             = 12702,
    QUEST_FLOWN_THE_COOP            = 12532,
};

class spell_item_chicken_cover : public SpellScriptLoader
{
    public:
        spell_item_chicken_cover() : SpellScriptLoader("spell_item_chicken_cover") { }

        class spell_item_chicken_cover_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_chicken_cover_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CHICKEN_NET) || !sSpellMgr->GetSpellInfo(SPELL_CAPTURE_CHICKEN_ESCAPE) || !sObjectMgr->GetQuestTemplate(QUEST_CHICKEN_PARTY) || !sObjectMgr->GetQuestTemplate(QUEST_FLOWN_THE_COOP))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                if (Unit* target = GetHitUnit())
                {
                    if (!target->HasAura(SPELL_CHICKEN_NET) && (caster->GetQuestStatus(QUEST_CHICKEN_PARTY) == QUEST_STATUS_INCOMPLETE || caster->GetQuestStatus(QUEST_FLOWN_THE_COOP) == QUEST_STATUS_INCOMPLETE))
                    {
                        caster->CastSpell(caster, SPELL_CAPTURE_CHICKEN_ESCAPE, true);
                        target->Kill(target);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_chicken_cover_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_chicken_cover_SpellScript();
        }
};

enum Refocus
{
    SPELL_AIMED_SHOT    = 19434,
    SPELL_MULTISHOT     = 2643,
    SPELL_VOLLEY        = 42243,
};

class spell_item_refocus : public SpellScriptLoader
{
    public:
        spell_item_refocus() : SpellScriptLoader("spell_item_refocus") { }

        class spell_item_refocus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_refocus_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();

                if (!caster || caster->getClass() != CLASS_HUNTER)
                    return;

                if (caster->HasSpellCooldown(SPELL_AIMED_SHOT))
                    caster->RemoveSpellCooldown(SPELL_AIMED_SHOT, true);

                if (caster->HasSpellCooldown(SPELL_MULTISHOT))
                    caster->RemoveSpellCooldown(SPELL_MULTISHOT, true);

                if (caster->HasSpellCooldown(SPELL_VOLLEY))
                    caster->RemoveSpellCooldown(SPELL_VOLLEY, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_refocus_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_refocus_SpellScript();
        }
};

class spell_item_muisek_vessel : public SpellScriptLoader
{
    public:
        spell_item_muisek_vessel() : SpellScriptLoader("spell_item_muisek_vessel") { }

        class spell_item_muisek_vessel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_muisek_vessel_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Creature* target = GetHitCreature())
                    if (target->isDead())
                        target->DespawnOrUnsummon();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_muisek_vessel_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_muisek_vessel_SpellScript();
        }
};

enum GreatmothersSoulcather
{
    SPELL_FORCE_CAST_SUMMON_GNOME_SOUL = 46486,
};

class spell_item_greatmothers_soulcatcher : public SpellScriptLoader
{
public:
    spell_item_greatmothers_soulcatcher() : SpellScriptLoader("spell_item_greatmothers_soulcatcher") { }

    class spell_item_greatmothers_soulcatcher_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_greatmothers_soulcatcher_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (GetHitUnit())
                GetCaster()->CastSpell(GetCaster(),SPELL_FORCE_CAST_SUMMON_GNOME_SOUL);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_greatmothers_soulcatcher_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_greatmothers_soulcatcher_SpellScript();
    }
};

// Enohar Explosive Arrows - 78838
class spell_item_enohar_explosive_arrows : public SpellScriptLoader
{
    public:
        spell_item_enohar_explosive_arrows() : SpellScriptLoader("spell_item_enohar_explosive_arrows") { }

        class spell_item_enohar_explosive_arrows_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_enohar_explosive_arrows_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (caster && target)
                    caster->DealDamage(target, target->GetHealth());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_enohar_explosive_arrows_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_enohar_explosive_arrows_SpellScript();
        }
};

enum HolyThurible
{
    NPC_WITHDRAWN_SOUL           = 45166,
};

class spell_item_holy_thurible : public SpellScriptLoader
{
    public:
        spell_item_holy_thurible() : SpellScriptLoader("spell_item_holy_thurible") { }

        class spell_item_holy_thurible_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_holy_thurible_SpellScript);

          
            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                // GetHitCreature don't work
                Creature* target  = caster->FindNearestCreature(NPC_WITHDRAWN_SOUL, 2.0f, true);
                if (target && caster)
                {

                    if (roll_chance_i(50))
                    {
                        caster->KilledMonsterCredit(NPC_WITHDRAWN_SOUL, target->GetGUID());
                        target->DespawnOrUnsummon(0);
                    }
                    else
                        target->setFaction(14);
                }                
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_holy_thurible_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_holy_thurible_SpellScript();
        }
};

enum bandage
{
    NPC_WOUNDED_DEFENDER      = 38805,
};

class spell_item_bandage_q24944 : public SpellScriptLoader
{
    public:
        spell_item_bandage_q24944() : SpellScriptLoader("spell_item_bandage_q24944") { }

        class spell_item_bandage_q24944_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_bandage_q24944_SpellScript);

          
            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                Creature* target  = GetHitCreature();
                if (target && caster && target->GetEntry() == NPC_WOUNDED_DEFENDER)
                {
                    target->SetFullHealth();
                    target->MonsterSay("Thank you my friend", LANG_UNIVERSAL, 0);
                    target->ForcedDespawn();
                    caster->KilledMonsterCredit(NPC_WOUNDED_DEFENDER, 0);
                }
                                
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_bandage_q24944_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_bandage_q24944_SpellScript();
        }
};

class spell_item_gen_alchemy_mop : public SpellScriptLoader
{
    public:
        spell_item_gen_alchemy_mop() : SpellScriptLoader("spell_item_gen_alchemy_mop") { }

        class spell_item_gen_alchemy_mop_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_gen_alchemy_mop_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleOnHit()
            {
                uint8 chance = urand(1,5); // not official, todo: find the rate
                Player* caster = GetCaster()->ToPlayer();
                if (caster && GetCaster()->GetTypeId() == TYPEID_PLAYER && !HasDiscoveredAllSpells(114751, GetCaster()->ToPlayer()) && chance == 1)
                {
                    if (uint32 discoveredSpellId = GetExplicitDiscoverySpell(114751, caster->ToPlayer()))
                        caster->learnSpell(discoveredSpellId, false);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_gen_alchemy_mop_SpellScript::HandleOnHit);
            }

        };
        SpellScript* GetSpellScript() const
        {
            return new spell_item_gen_alchemy_mop_SpellScript();
        }                 
};

class spell_alchemist_rejuvenation : public SpellScriptLoader
{
    public:
        spell_alchemist_rejuvenation() : SpellScriptLoader("spell_alchemist_rejuvenation") { }

        class spell_alchemist_rejuvenation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_alchemist_rejuvenation_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->getPowerType() == POWER_MANA)
                    {
                        switch (caster->getLevel())
                        {
                             case 85:
                                caster->EnergizeBySpell(caster, 105704, urand(8484, 9376), POWER_MANA);                                
                                break;
                            case 86:
                                caster->EnergizeBySpell(caster, 105704, urand(13651, 15087), POWER_MANA);  
                                break;
                            case 87:
                                caster->EnergizeBySpell(caster, 105704, urand(16451, 18181), POWER_MANA);  
                                break;
                            case 88:
                                caster->EnergizeBySpell(caster, 105704, urand(19818, 21902), POWER_MANA);  
                                break;
                            case 89:
                                caster->EnergizeBySpell(caster, 105704, urand(23884, 26398), POWER_MANA);  
                                break;
                            case 90:
                                caster->EnergizeBySpell(caster, 105704, urand(28500, 31500), POWER_MANA);  
                                break;
                            default:
                                break;
                        }
                    }
                    switch (caster->getLevel())
                    {
                        case 85:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(33935, 37505), false);                                
                            break;
                        case 86:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(54601, 60347), false);  
                            break;
                        case 87:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(65801, 72727), false);  
                            break;
                        case 88:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(79268, 87610), false);  
                            break;
                        case 89:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(95534, 105590), false);  
                            break;
                        case 90:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(114001, 126001), false);  
                            break;
                        default:
                            break;
                    }
                }                    
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_alchemist_rejuvenation_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_alchemist_rejuvenation_SpellScript();
        }
};

enum AmberPrison
{
    SPELL_AMBER_PRISON     = 127266,
};


class spell_item_amber_prison : public SpellScriptLoader
{
    public:
        spell_item_amber_prison() : SpellScriptLoader("spell_item_amber_prison") { }

        class spell_item_amber_prison_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_amber_prison_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AMBER_PRISON))
                    return false;
                return true;
            }

            SpellCastResult CheckCast()
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if (target->GetTypeId() == TYPEID_PLAYER)
                        return SPELL_FAILED_TARGET_IS_PLAYER;
                    if (target->GetTypeId() == TYPEID_UNIT && target->getLevel() > 94)
                        return SPELL_FAILED_HIGHLEVEL;

                }
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_amber_prison_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_amber_prison_SpellScript();
        }
};

class spell_item_cauldron_of_battle : public SpellScriptLoader
{
    public:
        spell_item_cauldron_of_battle() : SpellScriptLoader("spell_item_cauldron_of_battle") { }

        class spell_item_cauldron_of_battle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_cauldron_of_battle_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster())
                    GetCaster()->CastSpell(GetCaster(), 92635, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_cauldron_of_battle_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_cauldron_of_battle_SpellScript();
        }
};

class spell_item_big_cauldron_of_battle : public SpellScriptLoader
{
    public:
        spell_item_big_cauldron_of_battle() : SpellScriptLoader("spell_item_big_cauldron_of_battle") { }

        class spell_item_big_cauldron_of_battle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_big_cauldron_of_battle_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster())
                    GetCaster()->CastSpell(GetCaster(), 92652, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_big_cauldron_of_battle_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_big_cauldron_of_battle_SpellScript();
        }
};

class spell_item_stay_of_execution : public SpellScriptLoader
{
    public:
        spell_item_stay_of_execution() : SpellScriptLoader("spell_item_stay_of_execution") { }

        class spell_item_stay_of_execution_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_stay_of_execution_AuraScript);

            uint32 absorbPct, healPct;

            bool Load()
            {
                withdrawn = 0;
                return GetUnitOwner()->GetTypeId() == TYPEID_PLAYER;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                absorbAmount = CalculatePct(dmgInfo.GetDamage(), 20);
                withdrawn += absorbAmount;
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            { 
                if (!GetCaster())
                    return;

                int32 bp0 = int32(0.08f * withdrawn);
                GetCaster()->CastCustomSpell(GetCaster(), 96993, &bp0, 0, 0, true);
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_item_stay_of_execution_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectRemove += AuraEffectRemoveFn(spell_item_stay_of_execution_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }

        private:
            int32 withdrawn;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_stay_of_execution_AuraScript();
        }
};

class spell_item_flameseers_staff_flamebreaker : public SpellScriptLoader
{
    public:
        spell_item_flameseers_staff_flamebreaker() : SpellScriptLoader("spell_item_flameseers_staff_flamebreaker") { }

        class spell_item_flameseers_staff_flamebreaker_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_flameseers_staff_flamebreaker_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            { 
                if (!GetCaster() || !GetTarget())
                    return;

                if (GetTarget()->GetEntry() == 38896)
                {
                    GetCaster()->CastSpell(GetTarget(), 74723, true);
                    GetTarget()->Kill(GetTarget());
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_item_flameseers_staff_flamebreaker_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_flameseers_staff_flamebreaker_AuraScript();
        }
};

class spell_drums_of_rage : public SpellScriptLoader
{
public:
    spell_drums_of_rage() : SpellScriptLoader("spell_drums_of_rage") { }

    class spell_drums_of_rage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_drums_of_rage_SpellScript);

        void RemoveInvalidTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(JadeCore::UnitAuraCheck(true, HUNTER_SPELL_INSANITY));
            targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_SHAMAN_EXHAUSTED));
            targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_SHAMAN_SATED));
            targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_MAGE_TEMPORAL_DISPLACEMENT));
        }

        void ApplyDebuff()
        {
            if (Unit* target = GetHitUnit())
            {
                /// We must add a check here too, players can use a bug allowing them to receive twice a buff like this one
                /// If they cast the spells exactly at the same time
                if (target->HasAura(SPELL_SHAMAN_EXHAUSTED) || target->HasAura(HUNTER_SPELL_INSANITY) ||
                    target->HasAura(SPELL_MAGE_TEMPORAL_DISPLACEMENT) || target->HasAura(SPELL_SHAMAN_SATED))
                {
                    target->RemoveAura(GetSpellInfo()->Id);
                    return;
                }

                target->CastSpell(target, SPELL_SHAMAN_EXHAUSTED, true);
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_drums_of_rage_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_drums_of_rage_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_drums_of_rage_SpellScript::RemoveInvalidTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
            AfterHit += SpellHitFn(spell_drums_of_rage_SpellScript::ApplyDebuff);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_drums_of_rage_SpellScript();
    }
};

enum JardsSpells
{
    SPELL_JARDS_PECULIAR_ENERGY_SOURCE      = 143743, // script

    SPELL_JARDS_PECULIAR_ENERGY_SOURCE_2    = 139176, // create item
    SPELL_SKY_GOLEM                         = 139192,
    SPELL_ADVANCED_REFRIGERATION_UNIT       = 139197,
    SPELL_PIERRE                            = 139196,
    SPELL_RASCAL_BOT                        = 143714,

};

const uint32 allJardSpells[5] = 
{
    SPELL_JARDS_PECULIAR_ENERGY_SOURCE_2,
    SPELL_SKY_GOLEM,
    SPELL_ADVANCED_REFRIGERATION_UNIT,
    SPELL_PIERRE,
    SPELL_RASCAL_BOT
};

class spell_item_jards_peculiar_energy_source : public SpellScriptLoader
{
    public:
        spell_item_jards_peculiar_energy_source() : SpellScriptLoader("spell_item_jards_peculiar_energy_source") { }

        class spell_item_jards_peculiar_energy_source_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_jards_peculiar_energy_source_SpellScript);

            SpellCastResult HandleCheck()
            {
                if (!GetCaster())
                    return SPELL_CAST_OK;

                if (Player* player = GetCaster()->ToPlayer())
                {
                    for (uint8 i = 0; i < 5; ++i)
                        if (!player->HasSpell(allJardSpells[i]))
                            return SPELL_CAST_OK;
                }

                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_NOTHING_TO_DISCOVER);
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            void HandleHit(SpellEffIndex effIndex)
            {
                if (!GetCaster())
                    return;

                if (Player* player = GetCaster()->ToPlayer())
                {
                    for (uint8 i = 0; i < 5; ++i)
                    {
                        if (!player->HasSpell(allJardSpells[i]))
                            player->learnSpell(allJardSpells[i], false);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_jards_peculiar_energy_source_SpellScript::HandleHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_jards_peculiar_energy_source_SpellScript();
        }
};

enum
{
    SPELL_ENDURANCE_OF_NIUZAO_COOLDOWN = 148010,
    DK_SPELL_SHROUD_OF_PURGATORY = 116888,
};

// 146193 - Endurance of Niuzao
class spell_endurance_of_niuzao_absorb : public SpellScriptLoader
{
    public:
        spell_endurance_of_niuzao_absorb() : SpellScriptLoader("spell_endurance_of_niuzao_absorb") { }

        class spell_endurance_of_niuzao_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_endurance_of_niuzao_absorb_AuraScript);

            bool CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
                return true;
            }

            void Absorb(AuraEffect* /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();
                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;
                
                Player* player = target->ToPlayer();
                if (!player || player->GetRoleForGroup(player->GetSpecializationId(player->GetActiveSpec())) != ROLES_TANK)
                    return;

                // Disable this script if player is under Shroud of Purgatory or on CD
                if (target->HasAura(SPELL_ENDURANCE_OF_NIUZAO_COOLDOWN) || target->HasAura(DK_SPELL_SHROUD_OF_PURGATORY))
                    return;

                target->CastSpell(target, SPELL_ENDURANCE_OF_NIUZAO_COOLDOWN, true);
                target->SetHealth(1);
                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_endurance_of_niuzao_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_endurance_of_niuzao_absorb_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_endurance_of_niuzao_absorb_AuraScript();
        }
};

/// Drink -   430,   431,   432,   1133,   1135,   1137,  10250,  22734,  27089,  34291,
///         43182, 43183, 46755,  57073,  61830,  72623,  80166,  80167,  87958,  87959,
///         92736, 92797, 92800,  92803, 104262, 104270, 105230, 105232, 118358, 130336, 
///         149000
/// Drink Coffee - 49472
/// Drink mecanism in arenas
class spell_item_drinks : public SpellScriptLoader
{
    public:
        spell_item_drinks() : SpellScriptLoader("spell_item_drinks") { }

        class spell_item_drinks_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_drinks_AuraScript);

            void CorrectAmounts(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Aura* l_Aura = GetAura();
                AuraEffect* l_AuraEffect = l_Aura->GetEffect(SpellEffIndex::EFFECT_0);
                AuraEffect* l_DummyEffect = l_Aura->GetEffect(SpellEffIndex::EFFECT_1);
                if (!l_AuraEffect || !l_DummyEffect)
                    return;

                if (l_Caster->GetTypeId() == TypeID::TYPEID_PLAYER && l_Caster->ToPlayer()->InArena())
                {
                    ///< In arena, this does nothing for 2s and starts working after the first tick (2s later)
                    l_AuraEffect->ChangeAmount(0);
                }
                else
                {
                    l_AuraEffect->ChangeAmount(l_DummyEffect->GetAmount() * 5);
                    l_DummyEffect->SetPeriodic(false);
                }
            }

            void OnTick(AuraEffect const* aurEff)
            {
                Aura* l_Aura = GetAura();
                AuraEffect* l_AuraEffect = l_Aura->GetEffect(SpellEffIndex::EFFECT_0);
                AuraEffect* l_DummyEffect = l_Aura->GetEffect(SpellEffIndex::EFFECT_1);
                if (!l_AuraEffect || !l_DummyEffect)
                    return;

                ///< Compensate for the lost 2s
                switch (l_DummyEffect->GetTickNumber())
                {
                    case 1:
                        l_AuraEffect->ChangeAmount(l_DummyEffect->GetAmount() * 5 * 5 / 3); ///< 166%
                        break;

                    case 2:
                        l_AuraEffect->ChangeAmount(l_DummyEffect->GetAmount() * 5 * 4 / 3); ///< 133%
                        break;

                    case 3:
                        l_AuraEffect->ChangeAmount(l_DummyEffect->GetAmount()); ///< 100%
                        l_DummyEffect->SetPeriodic(false); ///< No longer needs update
                        break;

                    default:
                        break;
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_item_drinks_AuraScript::CorrectAmounts, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_MOD_POWER_REGEN, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_drinks_AuraScript::OnTick, SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_drinks_AuraScript();
        }
};

/// Blades of Renataki - 138756
class spell_item_blades_of_renataki: public SpellScriptLoader
{
    public:
        spell_item_blades_of_renataki() : SpellScriptLoader("spell_item_blades_of_renataki") { }

        enum BladesOfRenatakiSpells
        {
            BLADES = 138737
        };

        class spell_item_blades_of_renataki_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_blades_of_renataki_AuraScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if (!sSpellMgr->GetSpellInfo(BladesOfRenatakiSpells::BLADES))
                    return false;

                return true;
            }

            void OnPeriodic(AuraEffect const* p_AurEff)
            {
                if (!p_AurEff || !p_AurEff->GetBase())
                    return;

                if (Unit* l_Target = GetUnitOwner())
                {
                    if (Player* l_Player = l_Target->ToPlayer())
                        l_Target->CastSpell(l_Target, BladesOfRenatakiSpells::BLADES, true, l_Player->GetItemByGuid(p_AurEff->GetBase()->GetCastItemGUID()), nullptr, l_Target->GetGUID());
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_blades_of_renataki_AuraScript::OnPeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_blades_of_renataki_AuraScript();
        }
};

/// Spark of Zandalar - 138958
class spell_item_spark_of_zandalar: public SpellScriptLoader
{
    public:
        spell_item_spark_of_zandalar() : SpellScriptLoader("spell_item_spark_of_zandalar") { }

        enum SparkOfZandalar
        {
            ZANDALARI_WARRIOR = 138960
        };

        class spell_item_spark_of_zandalar_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_spark_of_zandalar_AuraScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SparkOfZandalar::ZANDALARI_WARRIOR))
                    return false;

                return true;
            }

            void CheckStack(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Owner = GetUnitOwner())
                {
                    Aura* l_Aura = p_AurEff->GetBase();
                    if (l_Aura->GetStackAmount() >= 10)
                    {
                        if (Aura* l_ZandalariWarrior = l_Owner->GetAura(SparkOfZandalar::ZANDALARI_WARRIOR))
                            l_ZandalariWarrior->RefreshDuration(); ///< Do not recast if the aura is already there
                        else
                            l_Owner->CastSpell(l_Owner, SparkOfZandalar::ZANDALARI_WARRIOR, true);

                        l_Owner->RemoveAura(l_Aura, AuraRemoveMode::AURA_REMOVE_BY_DEFAULT);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_item_spark_of_zandalar_AuraScript::CheckStack, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_spark_of_zandalar_AuraScript();
        }
};

/// Rune of Re-Origination - 139116 - last update: 5.4.2 17688
enum ReoriginationSpells
{
    REORIGINATION_CRITICAL = 139117,
    REORIGINATION_MASTERY  = 139120,
    REORIGINATION_HASTE    = 139121
};

class spell_item_rune_of_reorigination : public SpellScriptLoader
{
    public:
        spell_item_rune_of_reorigination() : SpellScriptLoader("spell_item_rune_of_reorigination") { }

        class spell_item_rune_of_reorigination_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_rune_of_reorigination_AuraScript);

            void HandleProc(AuraEffect const* p_AurEff, ProcEventInfo& p_ProcInfo)
            {
                if (!GetUnitOwner()) ///< Can that even be null?
                    return;

                if (Player* l_Owner = GetUnitOwner()->ToPlayer())
                {
                    uint32 l_CritScore = l_Owner->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CombatRating::CR_CRIT_MELEE);
                    uint32 l_HasteScore = l_Owner->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CombatRating::CR_HASTE_MELEE);
                    uint32 l_MasteryScore = l_Owner->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CombatRating::CR_MASTERY);

                    ReoriginationSpells l_SpellId;
                    if (l_CritScore > l_HasteScore)
                    {
                        if (l_CritScore > l_MasteryScore)
                            l_SpellId = ReoriginationSpells::REORIGINATION_CRITICAL;
                        else
                            l_SpellId = ReoriginationSpells::REORIGINATION_MASTERY;
                    }
                    else
                    {
                        if (l_HasteScore > l_MasteryScore)
                            l_SpellId = ReoriginationSpells::REORIGINATION_HASTE;
                        else
                            l_SpellId = ReoriginationSpells::REORIGINATION_MASTERY;
                    }

                    l_Owner->CastSpell(l_Owner, l_SpellId, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_item_rune_of_reorigination_AuraScript::HandleProc, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_rune_of_reorigination_AuraScript();
        }
};

/// Called by Re-Origination - 139117 - 139120 - 139121 - last updated: 5.4.2 17688
class spell_aura_reorigination : public SpellScriptLoader
{
    public:
        spell_aura_reorigination() : SpellScriptLoader("spell_aura_reorigination") { }

        class spell_aura_reorigination_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_aura_reorigination_AuraScript);

            template<CombatRating Rating>
            bool DoubleRating(AuraEffect const* p_AurEff, int32& p_Amount, bool & /*p_CanBeRecalculated*/)
            {
                if (!GetUnitOwner()) ///< Can that even be null?
                    return false;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    p_Amount = l_Player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + Rating);
                    return true;
                }

                return false;
            }

            template<CombatRating Rating>
            bool ZeroRating(AuraEffect const* p_AurEff, int32& p_Amount, bool & /*p_CanBeRecalculated*/)
            {
                if (!GetUnitOwner()) ///< Can that even be null?
                    return false;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    p_Amount = -static_cast<int32>(l_Player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + Rating));
                    return true;
                }

                return false;
            }

            void Register() override
            {
                switch (m_scriptSpellId)
                {
                    case ReoriginationSpells::REORIGINATION_CRITICAL:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::DoubleRating<CombatRating::CR_CRIT_MELEE>, EFFECT_0, SPELL_AURA_MOD_RATING);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::ZeroRating<CombatRating::CR_MASTERY>,      EFFECT_1, SPELL_AURA_MOD_RATING);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::ZeroRating<CombatRating::CR_HASTE_MELEE>,  EFFECT_2, SPELL_AURA_MOD_RATING);
                        break;

                    case ReoriginationSpells::REORIGINATION_MASTERY:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::DoubleRating<CombatRating::CR_MASTERY>,   EFFECT_0, SPELL_AURA_MOD_RATING);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::ZeroRating<CombatRating::CR_CRIT_MELEE>,  EFFECT_1, SPELL_AURA_MOD_RATING);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::ZeroRating<CombatRating::CR_HASTE_MELEE>, EFFECT_2, SPELL_AURA_MOD_RATING);
                        break;

                    case ReoriginationSpells::REORIGINATION_HASTE:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::DoubleRating<CombatRating::CR_HASTE_MELEE>, EFFECT_0, SPELL_AURA_MOD_RATING);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::ZeroRating<CombatRating::CR_MASTERY>,       EFFECT_1, SPELL_AURA_MOD_RATING);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_aura_reorigination_AuraScript::ZeroRating<CombatRating::CR_CRIT_MELEE>,    EFFECT_2, SPELL_AURA_MOD_RATING);
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_aura_reorigination_AuraScript();
        }
};

/// Zandalari Warding - 138925 - last update: 5.4.2 17688
class spell_item_zandalari_warding : public SpellScriptLoader
{
    public:
        spell_item_zandalari_warding() : SpellScriptLoader("spell_item_zandalari_warding") { }

        enum eSpells
        {
            BlessingOfZuldazar = 138967
        };

        class spell_item_zandalari_warding_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_zandalari_warding_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(eSpells::BlessingOfZuldazar))
                        return SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                }
                else
                    return SpellCastResult::SPELL_FAILED_DONT_REPORT;

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_item_zandalari_warding_SpellScript::CheckCast);
            }
        };

        class spell_item_zandalari_warding_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_zandalari_warding_AuraScript);

            bool CalculateAmount(AuraEffect const* /*p_AuraEffect*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    uint32 l_BlessingCounts = 0;
                    if (Aura* l_ZandalariBlessing = l_Caster->GetAura(eSpells::BlessingOfZuldazar))
                    {
                        l_BlessingCounts += l_ZandalariBlessing->GetStackAmount();
                        l_ZandalariBlessing->Remove();
                    }

                    p_Amount *= l_BlessingCounts;
                }

                return false;
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_item_zandalari_warding_AuraScript::CalculateAmount, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_zandalari_warding_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_item_zandalari_warding_SpellScript();
        }
};

class spell_item_ancient_knowledge : public SpellScriptLoader
{
    public:
        spell_item_ancient_knowledge() : SpellScriptLoader("spell_item_ancient_knowledge") { }

        class spell_item_ancient_knowledge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_ancient_knowledge_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->getLevel() > 84)
                        return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_item_ancient_knowledge_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_ancient_knowledge_SpellScript();
        }
};

class spell_item_bilgewater_molotov : public SpellScriptLoader
{
    public:
        spell_item_bilgewater_molotov() : SpellScriptLoader("spell_item_bilgewater_molotov") { }

        class spell_item_bilgewater_molotov_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_bilgewater_molotov_SpellScript);

            SpellCastResult CheckCast()
            {
                if (!GetExplTargetUnit() || GetExplTargetUnit()->GetEntry() != 67359)
                    return SpellCastResult::SPELL_FAILED_BAD_TARGET_FILTER;

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_item_bilgewater_molotov_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_bilgewater_molotov_SpellScript();
        }
};

class spell_item_universal_remote : public SpellScriptLoader
{
public:
    spell_item_universal_remote() : SpellScriptLoader("spell_item_universal_remote") { }

    class spell_item_universal_remote_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_universal_remote_SpellScript);

        SpellCastResult CheckCast()
        {
            if (!GetExplTargetUnit() || (GetExplTargetUnit()->GetEntry() != 67967 && GetExplTargetUnit()->GetEntry() != 68011))
                return SpellCastResult::SPELL_FAILED_BAD_TARGET_FILTER;

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_item_universal_remote_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_item_universal_remote_SpellScript();
    }
};

enum e_item_assurance_of_consequence
{
    FERAL_DRUID     = 145961,
    BM_HUNTER       = 145964,
    MM_HUNTER       = 145965,
    SURV_HUNTER     = 145966,
    WINDWALKER_MONK = 145969,
    ENCH_SHAMAN     = 145986,
    ASSASIN_ROGUE   = 145983,
    COMBAT_ROGUE    = 145984,
    SUB_ROGUE       = 145985,
};

const static std::array<std::array<uint32, 2>, 9> s_AssuranceOfConsequenceSpells =
{
    {
        { SPEC_DRUID_CAT, FERAL_DRUID },
        { SPEC_HUNTER_BEASTMASTER, BM_HUNTER },
        { SPEC_HUNTER_MARKSMAN, MM_HUNTER },
        { SPEC_HUNTER_SURVIVAL, SURV_HUNTER },
        { SPEC_MONK_WINDWALKER, WINDWALKER_MONK },
        { SPEC_SHAMAN_ENHANCEMENT, ENCH_SHAMAN },
        { SPEC_ROGUE_ASSASSINATION, ASSASIN_ROGUE },
        { SPEC_ROGUE_COMBAT, COMBAT_ROGUE },
        { SPEC_ROGUE_SUBTLETY, SUB_ROGUE }
    }
};

enum e_item_evil_eye_of_galakras
{
    FROST_DK     = 145959,
    UNHOLY_DK    = 145960,
    RET_PALADIN  = 145978,
    ARMS_WARRIOR = 145990,
    FURY_WARRION = 145990,
};

const static std::array<std::array<uint32, 2>, 5> s_EvilEyeOfGalakrasSpells =
{
    {
        { SPEC_DK_FROST, FROST_DK },
        { SPEC_DK_UNHOLY, UNHOLY_DK },
        { SPEC_PALADIN_RETRIBUTION, RET_PALADIN },
        { SPEC_WARRIOR_ARMS, ARMS_WARRIOR },
        { SPEC_WARRIOR_FURY, FURY_WARRION }
    }
};

enum e_item_vial_of_living_corruption
{
    PALADIN_PROTECTION = 145976,
    WARRIOR_PROTECTION = 145992,
    DROOD_GUARDIAN     = 145962,
    DK_BLOOD           = 145958,
    MONK_BREWMASTER    = 145969
};

const static std::array<std::array<uint32, 2>, 5> s_VialOfLivingCorruption =
{
    {
        { SPEC_PALADIN_PROTECTION, PALADIN_PROTECTION },
        { SPEC_WARRIOR_PROTECTION, WARRIOR_PROTECTION },
        { SPEC_DRUID_CAT,          DROOD_GUARDIAN     },
        { SPEC_DK_BLOOD,           DK_BLOOD           },
        { SPEC_MONK_BREWMASTER,    MONK_BREWMASTER    }
    }
};

template<size_t Dimension, size_t Size>
class spell_item_readiness_generic : public SpellScriptLoader
{
    typedef std::array<std::array<uint32, Dimension>, Size> Spells;

public:
    spell_item_readiness_generic(const char* scriptName, Spells const& spells) : SpellScriptLoader(scriptName), _spells(spells) { }

    Spells _spells;

    class spell_item_readiness_generic_AuraScript : public AuraScript
    {
    public:
        spell_item_readiness_generic_AuraScript(Spells const& spells)
            : m_castedSpell(0), m_timer(0), m_spells(spells) { }

        PrepareAuraScript(spell_item_readiness_generic_AuraScript);

        uint32 m_castedSpell;
        uint32 m_timer;
        Spells m_spells;

        uint32 GetSpellIdForSpec(uint32 spec)
        {
            for (auto ptr : m_spells)
            {
                if (ptr[0] == spec)
                    return ptr[1];
            }
            return 0;
        }

        void Check(AuraEffect* aurEff)
        {
            if (!GetCaster() || !GetCaster()->ToPlayer())
                return;

            uint32 newSpellTocast = GetSpellIdForSpec(GetCaster()->ToPlayer()->GetSpecializationId());
            if (newSpellTocast != m_castedSpell)
            {
                OnRemove(aurEff, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
                OnApply(aurEff, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            }
        }

        void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            if (!GetCaster() || !GetCaster()->ToPlayer())
                return;

            Player* player = GetCaster()->ToPlayer();
            
            Item* castItem = nullptr;
            if (aurEff->GetBase()->GetCastItemGUID())
                castItem = player->GetItemByGuid(aurEff->GetBase()->GetCastItemGUID());

            m_castedSpell = GetSpellIdForSpec(player->GetSpecializationId());
            if (m_castedSpell)
            {
                int32 value = -aurEff->GetAmount();
                player->CastCustomSpell(player, m_castedSpell, &value, nullptr, nullptr, true, castItem, aurEff, aurEff->GetCasterGUID());
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (!GetCaster())
                return;

            if (m_castedSpell)
                GetCaster()->RemoveAura(m_castedSpell);
        }

        void OnUpdate(uint32 diff, AuraEffect* aurEff)
        {
            if (m_timer >= 500)
            {
                Check(aurEff);
                m_timer = 0;
            }
            else
                m_timer += diff;
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_item_readiness_generic_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_item_readiness_generic_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectUpdate += AuraEffectUpdateFn(spell_item_readiness_generic_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_item_readiness_generic_AuraScript(_spells);
    }
};

enum eWormholeTeleportSpell
{
    SPELL_WORMHOLE_GENERATOR_PANDARIA_TELEPORT = 126775,
};

class spell_item_wormhole_generator_pandaria : public SpellScriptLoader
{
    public:
        spell_item_wormhole_generator_pandaria() : SpellScriptLoader("spell_item_wormhole_generator_pandaria") { }

        class spell_item_wormhole_generator_pandaria_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_wormhole_generator_pandaria_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WORMHOLE_GENERATOR_PANDARIA_TELEPORT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                const static Position s_Positions[9] =
                {
                    { 2510.13f, -2126.75f, 390.61f, 3.88f},
                    { 124.64f, 1025.29f, 194.01f, 4.62f },
                    { -1504.79f, -433.90f, 288.27f, 0.75f },
                    { 2090.18f, 2159.22f, 455.91f, 4.33f },
                    { 5131.41f, 213.31f, 5.39f, 0.38f },
                    {  3072.98f, 6120.40f, 54.60f, 0.85f },
                    { -1508.58f, 4540.12f, 148.69f, 2.07f },
                    { 657.33f, 411.59f, 207.81f, 5.60f },
                    { 1208.63f, 1376.82f, 363.66f, 4.51f },
                };
                
                Position const& pos = s_Positions[urand(0, 9)];
                GetCaster()->ToPlayer()->TeleportTo(870, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation());
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_wormhole_generator_pandaria_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_wormhole_generator_pandaria_SpellScript();
        }
};

class spell_item_steadfast_talisman : public SpellScriptLoader
{
public:
    spell_item_steadfast_talisman() : SpellScriptLoader("spell_item_steadfast_talisman") { }

    class spell_item_steadfast_talisman_AuraScript : public AuraScript
    {
    public:
        spell_item_steadfast_talisman_AuraScript()
            : m_baseAmount(0)
        {
        }

        PrepareAuraScript(spell_item_steadfast_talisman_AuraScript);

        int m_baseAmount;

        void OnUpdate(AuraEffect const* aurEff)
        {
            if (!aurEff)
                return;

            auto base = aurEff->GetBase();
            if (!base)
                return;

            auto eff0 = base->GetEffect(EFFECT_0);
            eff0->ChangeAmount(eff0->GetAmount() - m_baseAmount);
        }

        bool CalculateAmount(AuraEffect const* /*auraEffect*/, int32& amount, bool& canBeRecalculated)
        {
            m_baseAmount = amount;
            amount *= 10;
            return false;
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_steadfast_talisman_AuraScript::OnUpdate, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_item_steadfast_talisman_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_item_steadfast_talisman_AuraScript();
    }
};

class spell_item_eye_of_the_black_prince : public SpellScriptLoader
{
public:
    spell_item_eye_of_the_black_prince() : SpellScriptLoader("spell_item_eye_of_the_black_prince") { }

    class spell_item_eye_of_the_black_prince_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_eye_of_the_black_prince_SpellScript);

        SpellCastResult CheckCast()
        {
            auto item = GetExplTargetItem();
            if (item == nullptr || item->GetTemplate() == nullptr)
                return SpellCastResult::SPELL_FAILED_ITEM_NOT_FOUND;

            ItemTemplate const* proto = item->GetTemplate();
            if (proto->ItemLevel < 528)
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

            switch (proto->InventoryType)
            {
                case INVTYPE_WEAPON:
                case INVTYPE_RANGED:
                case INVTYPE_2HWEAPON:
                case INVTYPE_WEAPONMAINHAND:
                case INVTYPE_WEAPONOFFHAND:
                case INVTYPE_RANGEDRIGHT:
                    break;
                default:
                    return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_item_eye_of_the_black_prince_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_item_eye_of_the_black_prince_SpellScript();
    }
};

/// Taunt Flag Targeting - 51640 - last update: 5.4.8 18414
class spell_taunt_flag_targeting : public SpellScriptLoader
{
    public:
        spell_taunt_flag_targeting() : SpellScriptLoader("spell_taunt_flag_targeting") {}

        class spell_taunt_flag_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_taunt_flag_targeting_SpellScript);

            Position m_BestTargetPos;
            char const* m_BestTargetName = nullptr;
            std::size_t m_BestTargetNameLength = 0;

            SpellCastResult CheckCast()
            {
                /// Used to improve error message
                bool l_FoundCorpse = false;
                bool l_InFrontOfMe = false;
                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    float l_SearchDist = GetSpellInfo()->Effects[SpellEffIndex::EFFECT_0].CalcRadius(l_Caster);

                    std::list<WorldObject*> l_Targets;
                    JadeCore::AllWorldObjectsInRange l_Check(l_Caster, l_SearchDist);
                    JadeCore::WorldObjectListSearcher<JadeCore::AllWorldObjectsInRange> l_Searcher(l_Caster, l_Targets, l_Check);
                    l_Caster->VisitNearbyObject(l_SearchDist, l_Searcher);

                    Position l_CasterPos;
                    l_Caster->GetPosition(&l_CasterPos);

                    for (auto l_It = l_Targets.begin(); l_It != l_Targets.end(); ++l_It)
                    {
                        /// Either we have a corpse, either we have a player
                        if (Corpse* l_Corpse = (*l_It)->ToCorpse())
                        {
                            if (Player* l_Owner = ObjectAccessor::FindPlayer(l_Corpse->GetOwnerGUID()))
                            {
                                if (l_Owner->GetGUID() == l_Caster->GetGUID())
                                    continue;

                                l_FoundCorpse = true;

                                if (!l_Caster->isInFront(l_Corpse)) ///< Only corpses in front of us
                                    continue;

                                l_InFrontOfMe = true;

                                if (m_BestTargetName && m_BestTargetPos.GetExactDistSq(&l_CasterPos) < l_Corpse->GetExactDistSq(&l_CasterPos))
                                    continue;

                                if (l_Caster->GetReactionTo(l_Owner) > REP_NEUTRAL || l_Owner->GetReactionTo(l_Caster) > REP_NEUTRAL) ///< Only enemies corpses
                                    continue;

                                l_Corpse->GetPosition(&m_BestTargetPos);

                                m_BestTargetName = l_Owner->GetName();
                                m_BestTargetNameLength = l_Owner->GetNameLength();
                            }
                        }
                        else if (Player* l_Player = (*l_It)->ToPlayer())
                        {
                            if (l_Player->GetGUID() == l_Caster->GetGUID())
                                continue;

                            if (l_Player->isAlive()) ///< Only corpses
                                continue;

                            /// Skips ghosts
                            if (l_Player->HasAuraType(SPELL_AURA_GHOST))
                                continue;

                            l_FoundCorpse = true;

                            if (!l_Caster->isInFront(l_Player))
                                continue;

                            l_InFrontOfMe = true;

                            /// Is this corpse closer?
                            if (m_BestTargetName && m_BestTargetPos.GetExactDistSq(&l_CasterPos) < l_Player->GetExactDistSq(&l_CasterPos))
                                continue;

                            if (l_Caster->GetReactionTo(l_Player) > REP_NEUTRAL || l_Player->GetReactionTo(l_Caster) > REP_NEUTRAL) ///< Only enemies corpses
                                continue;

                            l_Player->GetPosition(&m_BestTargetPos);

                            m_BestTargetName = l_Player->GetName();
                            m_BestTargetNameLength = l_Player->GetNameLength();
                        }
                    }
                }

                if (!m_BestTargetName)
                {
                    if (l_FoundCorpse)
                        return (l_InFrontOfMe) ? SpellCastResult::SPELL_FAILED_BAD_TARGETS : SpellCastResult::SPELL_FAILED_NOT_INFRONT;
                    else
                    {
                        SetCustomCastResultMessage(SpellCustomErrors::SPELL_CUSTOM_ERROR_NO_NEARBY_CORPSES);
                        return SpellCastResult::SPELL_FAILED_CUSTOM_ERROR;
                    }
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void HandleCast(SpellEffIndex p_Index)
            {
                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    if (m_BestTargetName)
                    {
                        l_Caster->CastSpell(m_BestTargetPos, 51657, true);
                        l_Caster->AddSpellCooldown(GetSpellInfo()->Id, 0, 60 * IN_MILLISECONDS);
                        l_Caster->SendSpellCooldown(GetSpellInfo()->Id, 60 * IN_MILLISECONDS);

                        if (WorldSession* l_Session = l_Caster->GetSession())
                        {
                            char const* l_TauntText = l_Session->GetTrinityString(LangTauntFlag);
                            std::size_t l_TauntTextLength = std::strlen(l_TauntText);

                            std::string l_Text;
                            l_Text.reserve(l_TauntTextLength + m_BestTargetNameLength);
                            l_Text.append(l_TauntText, l_TauntTextLength);
                            l_Text.append(m_BestTargetName, m_BestTargetNameLength);

                            l_Caster->TextEmote(l_Text);
                        }
                    }
                }
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_taunt_flag_targeting_SpellScript::CheckCast);
                OnEffectLaunch += SpellEffectFn(spell_taunt_flag_targeting_SpellScript::HandleCast, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_taunt_flag_targeting_SpellScript;
        }
};

enum eCleaveSpell
{
    SPELL_CLEAVE_SPREAD = 146137
};

// 146136 - Cleave
class spell_item_fusion_fire_core : public SpellScriptLoader
{
public:
    spell_item_fusion_fire_core() : SpellScriptLoader("spell_item_fusion_fire_core") { }

    class spell_item_fusion_fire_core_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_item_fusion_fire_core_AuraScript);

        void HandleProc(AuraEffect const* p_AurEff, ProcEventInfo& p_ProcInfo)
        {
            Unit* l_Caster = GetCaster();
            if (!l_Caster)
                return;

            if (DamageInfo* l_DamageInfos = p_ProcInfo.GetDamageInfo())
            {
                int32 l_Damages = l_DamageInfos->GetDamage();
                l_Caster->CastCustomSpell(l_Caster, SPELL_CLEAVE_SPREAD, &l_Damages, NULL, NULL, true);
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(spell_item_fusion_fire_core_AuraScript::HandleProc, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_item_fusion_fire_core_AuraScript();
    }
};

enum eLifeStealSpell
{
    SPELL_LIFE_STEAL = 146347
};

// 146346 - Life Steal
class spell_item_juggernault_focusing_crystal : public SpellScriptLoader
{
    public:
        spell_item_juggernault_focusing_crystal() : SpellScriptLoader("spell_item_juggernault_focusing_crystal") { }

        class spell_item_juggernault_focusing_crystal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_juggernault_focusing_crystal_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (DamageInfo* l_DamageInfo = procInfo.GetDamageInfo())
                {
                    int32 l_Healing = l_DamageInfo->GetDamage() * 0.0235f;
                    l_Caster->CastCustomSpell(l_Caster, SPELL_LIFE_STEAL, &l_Healing, NULL, NULL, true);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_item_juggernault_focusing_crystal_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_juggernault_focusing_crystal_AuraScript();
        }
};

enum eMultiStrikeSpell
{
    SPELL_MULTI_STRIKE      = 146061,
    SPELL_MULTI_STRIKE_HEAL = 146177
};

// 146059 - Multistrike
class spell_item_multistrike : public SpellScriptLoader
{
    public:
        spell_item_multistrike() : SpellScriptLoader("spell_item_multistrike") { }

        class spell_item_multistrike_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_multistrike_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (DamageInfo* l_DmgInfo = procInfo.GetDamageInfo())
                {
                    int32 l_Damages = l_DmgInfo->GetDamage() / 3;
                    l_Caster->CastCustomSpell(l_Caster, SPELL_MULTI_STRIKE, &l_Damages, NULL, NULL, true);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_item_multistrike_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_multistrike_AuraScript();
        }
};

// 146176 - Multistrike (healing version)
class spell_item_multistrike_heal : public SpellScriptLoader
{
    public:
        spell_item_multistrike_heal() : SpellScriptLoader("spell_item_multistrike_heal") { }

        class spell_item_multistrike_heal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_multistrike_heal_AuraScript);

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (HealInfo* l_HealInfos = procInfo.GetHealInfo())
                {
                    int32 l_Heal = l_HealInfos->GetHeal() / 3;
                    l_Caster->CastCustomSpell(l_Caster, SPELL_MULTI_STRIKE_HEAL, &l_Heal, NULL, NULL, true);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_item_multistrike_heal_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_multistrike_heal_AuraScript();
        }
};

void AddSC_item_spell_scripts()
{
    new spell_item_eye_of_the_black_prince();
    new spell_item_readiness_generic<2, 9>("spell_item_assurance_of_consequence", s_AssuranceOfConsequenceSpells);
    new spell_item_readiness_generic<2, 5>("spell_item_evil_eye_of_galakras", s_EvilEyeOfGalakrasSpells);
    new spell_item_readiness_generic<2, 5>("spell_item_vial_of_living_corruption", s_VialOfLivingCorruption);
    new spell_drums_of_rage();
    // 23074 Arcanite Dragonling
    new spell_item_trigger_spell("spell_item_arcanite_dragonling", SPELL_ARCANITE_DRAGONLING);
    // 23133 Gnomish Battle Chicken
    new spell_item_trigger_spell("spell_item_gnomish_battle_chicken", SPELL_BATTLE_CHICKEN);
    // 23076 Mechanical Dragonling
    new spell_item_trigger_spell("spell_item_mechanical_dragonling", SPELL_MECHANICAL_DRAGONLING);
    // 23075 Mithril Mechanical Dragonling
    new spell_item_trigger_spell("spell_item_mithril_mechanical_dragonling", SPELL_MITHRIL_MECHANICAL_DRAGONLING);
    new spell_item_deviate_fish();
    new spell_item_flask_of_the_north();
    new spell_item_gnomish_death_ray();
    new spell_item_make_a_wish();
    new spell_item_mingos_fortune_generator();
    new spell_item_net_o_matic();
    new spell_item_noggenfogger_elixir();
    new spell_item_savory_deviate_delight();
    new spell_item_six_demon_bag();
    new spell_item_underbelly_elixir();
    new spell_item_shadowmourne();
    new spell_item_red_rider_air_rifle();
    new spell_item_create_heart_candy();
    new spell_item_book_of_glyph_mastery();
    new spell_item_gift_of_the_harvester();
    new spell_item_map_of_the_geyser_fields();
    new spell_item_vanquished_clutches();
    new spell_magic_eater_food();
    new spell_item_refocus();
    new spell_item_shimmering_vessel();
    new spell_item_purify_helboar_meat();
    new spell_item_crystal_prison_dummy_dnd();
    new spell_item_reindeer_transformation();
    new spell_item_nigh_invulnerability();
    new spell_item_poultryizer();
    new spell_item_socrethars_stone();
    new spell_item_demon_broiled_surprise();
    new spell_item_complete_raptor_capture();
    new spell_item_impale_leviroth();
    new spell_item_brewfest_mount_transformation();
    new spell_item_nitro_boots();
    new spell_item_teach_language();
    new spell_item_rocket_boots();
    new spell_item_pygmy_oil();
    new spell_item_unusual_compass();
    new spell_item_chicken_cover();
    new spell_item_muisek_vessel();
    new spell_item_greatmothers_soulcatcher();
    new spell_item_enohar_explosive_arrows();
    new spell_item_holy_thurible();
    new spell_item_bandage_q24944();
    new spell_item_gen_alchemy_mop();
    new spell_alchemist_rejuvenation();
    new spell_item_amber_prison();
    new spell_item_cauldron_of_battle();
    new spell_item_big_cauldron_of_battle();
    new spell_item_stay_of_execution();
    new spell_item_flameseers_staff_flamebreaker();
    new spell_item_jards_peculiar_energy_source();
    new spell_endurance_of_niuzao_absorb();
    new spell_item_drinks();
    new spell_item_blades_of_renataki();
    new spell_item_spark_of_zandalar();
    new spell_item_rune_of_reorigination();
    new spell_aura_reorigination();
    new spell_item_zandalari_warding();
    new spell_item_ancient_knowledge();
    new spell_item_wormhole_generator_pandaria();
    new spell_item_steadfast_talisman();
    new spell_item_bilgewater_molotov();
    new spell_item_universal_remote();
    new spell_taunt_flag_targeting();
    new spell_item_fusion_fire_core();
    new spell_item_juggernault_focusing_crystal();
    new spell_item_multistrike();
    new spell_item_multistrike_heal();
}

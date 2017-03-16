/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#define SPECTATOR_ADDON_SPELL_INTERUPTED 99999 // specific addons
#define SPECTATOR_ADDON_SPELL_CANCELED 99998   // numbers =\

#include "WorldPacket.h"

enum SpectatorPrefix {
    SPECTATOR_PREFIX_PLAYER,
    SPECTATOR_PREFIX_NAME,
    SPECTATOR_PREFIX_STATUS,
    SPECTATOR_PREFIX_MAXHP,
    SPECTATOR_PREFIX_CURHP,
    SPECTATOR_PREFIX_MAXPOWER,
    SPECTATOR_PREFIX_CURPOWER,
    SPECTATOR_PREFIX_POWERTYPE,
    SPECTATOR_PREFIX_TARGET,
    SPECTATOR_PREFIX_CLASS,
    SPECTATOR_PREFIX_TEAM,
    SPECTATOR_PREFIX_SPELL,
    SPECTATOR_PREFIX_AURA,
    SPECTATOR_PREFIX_ARENA_TIMER,
    SPECTATOR_PREFIX_PET,
    SPECTATOR_PREFIX_PET_HP,
    SPECTATOR_PREFIX_COOLDOWN,
    SPECTATOR_PREFIX_COUNT      // must be at the end of list
};

class SpectatorAddonMsg {
    public:
        SpectatorAddonMsg();

        void SetPlayer(uint64 _player)          { player    = _player;  EnableFlag(SPECTATOR_PREFIX_PLAYER); }
        void SetName(std::string _name)         { name      = _name;    EnableFlag(SPECTATOR_PREFIX_NAME); }
        void SetStatus(bool _isAlive)           { isAlive   = _isAlive; EnableFlag(SPECTATOR_PREFIX_STATUS); }
        void SetClass(uint8 _class)             { pClass    = _class;   EnableFlag(SPECTATOR_PREFIX_CLASS); }
        void SetTarget(uint64 _target)          { target    = _target;  EnableFlag(SPECTATOR_PREFIX_TARGET); }
        void SetTeam(uint32 _team)              { team      = _team;    EnableFlag(SPECTATOR_PREFIX_TEAM); }

        void SetMaxHP(uint32 hp)                { maxHP     = hp; EnableFlag(SPECTATOR_PREFIX_MAXHP); }
        void SetCurrentHP(uint32 hp)            { currHP    = hp; EnableFlag(SPECTATOR_PREFIX_CURHP);  }
        void SetMaxPower(int32 power)          { maxPower  = power; EnableFlag(SPECTATOR_PREFIX_MAXPOWER); }
        void SetCurrentPower(int32 power)      { currPower = power; EnableFlag(SPECTATOR_PREFIX_CURPOWER); }
        void SetPowerType(Powers power)         { powerType = power; EnableFlag(SPECTATOR_PREFIX_POWERTYPE); }

        void SetPet(uint8 _family)              { family = _family; EnableFlag(SPECTATOR_PREFIX_PET); }
        void SetPetHP(uint32 hp)                { petHP = hp; EnableFlag(SPECTATOR_PREFIX_PET_HP); }

        void SetEndTime(uint32 _time)           { time = _time; EnableFlag(SPECTATOR_PREFIX_ARENA_TIMER); }

        void CastSpell(uint32 _spellId, uint32 _castTime) { spellId = _spellId; castTime = _castTime; EnableFlag(SPECTATOR_PREFIX_SPELL); }
        void CreateAura(uint32 _caster,  uint32 _spellId, bool _isDebuff, uint8 _type, int32 _duration, int32 _expire, uint16 _stack, bool _isRemove);
        void AddCooldown(uint32 _spellId, uint32 _cooldown) { spellId = _spellId; cooldown = _cooldown; EnableFlag(SPECTATOR_PREFIX_COOLDOWN); }
 
        static bool SendPacket(SpectatorAddonMsg& msg, uint64 receiver);
        bool SendPacket(uint64 receiver);

        bool isFilledIn(uint8 prefix) { return prefixFlags[prefix]; }

        static bool CanSandAura(uint32 auraID);
    private:

        WorldPacket&  CachedMessage();

        void EnableFlag(uint8 prefix) { prefixFlags[prefix] = true; }
        uint64 player;
        std::string name;
        bool isAlive;
        uint64 target;
        uint8 pClass;
        uint32 time;

        uint32 maxHP;
        int32 maxPower;
        uint32 currHP;
        int32 currPower;
        Powers powerType;

        uint8 family;
        uint32 petHP;

        uint32 spellId;
        uint32 castTime;
        uint32 cooldown;

        uint32 team;

        // aura data
        uint32 aCaster;
        uint32 aSpellId;
        bool aIsDebuff;
        uint8 aType;
        int32 aDuration;
        int32 aExpire;
        uint16 aStack;
        bool aRemove;

        bool prefixFlags[SPECTATOR_PREFIX_COUNT];
        WorldPacket cachedMessage;
};

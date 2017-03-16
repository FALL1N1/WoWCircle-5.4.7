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

#ifndef TRINITY_OBJECTDEFINES_H
#define TRINITY_OBJECTDEFINES_H

#include "Define.h"
#include "World.h"

// used for creating values for respawn for example
#define MAKE_PAIR64(l, h)  uint64(uint32(l) | (uint64(h) << 32))
#define PAIR64_HIPART(x)   (uint32)((uint64(x) >> 32) & UI64LIT(0x00000000FFFFFFFF))
#define PAIR64_LOPART(x)   (uint32)(uint64(x)         & UI64LIT(0x00000000FFFFFFFF))

#define MAKE_PAIR16(l, h)  uint16(uint8(l) | (uint16(h) << 8))
#define MAKE_PAIR32(l, h)  uint32(uint16(l) | (uint32(h) << 16))
#define PAIR32_HIPART(x)   (uint16)((uint32(x) >> 16) & 0x0000FFFF)
#define PAIR32_LOPART(x)   (uint16)(uint32(x)         & 0x0000FFFF)

enum class HighGuid
{
    Item               = 0x400,                        // blizz 4000
    Container          = 0x400,                        // blizz 4000
    Player             = 0x018,                        // blizz 0018
    GameObject         = 0xF11,                        // blizz F110
    Transport          = 0xF12,                        // blizz F120 (for GAMEOBJECT_TYPE_TRANSPORT)
    Unit               = 0xF13,                        // blizz F130
    Pet                = 0xF14,                        // blizz F140
    Vehicle            = 0xF15,                        // blizz F550
    DynamicObject      = 0xF10,                        // blizz F100
    Corpse             = 0xF101,                       // blizz F100
    AreaTrigger        = 0xF1B,                        // blizz F100
    TypeBattleGround   = 0x1F1,                        // new 4.x
    Mo_Transport       = 0x1FC,                        // blizz 1FC0 (for GAMEOBJECT_TYPE_MO_TRANSPORT)
    Group              = 0x1F5,
    Guild              = 0x1FF,                        // new 4.x
    Instance           = 0x104,                        // new 5.x
    Loot               = 0xF19,                        // new 5.4.x
    SceneObject        = 0x18F
};

template<HighGuid high>
struct ObjectGuidTraits
{
    static bool const Global = false;
    static bool const MapSpecific = false;
};

#define GUID_TRAIT_GLOBAL(highguid) \
    template<> struct ObjectGuidTraits<highguid> \
    { \
        static bool const Global = true; \
        static bool const MapSpecific = false; \
    };

#define GUID_TRAIT_MAP_SPECIFIC(highguid) \
    template<> struct ObjectGuidTraits<highguid> \
    { \
        static bool const Global = false; \
        static bool const MapSpecific = true; \
    };

GUID_TRAIT_GLOBAL(HighGuid::Player)
GUID_TRAIT_GLOBAL(HighGuid::Item)
GUID_TRAIT_GLOBAL(HighGuid::Mo_Transport)
GUID_TRAIT_GLOBAL(HighGuid::Group)
GUID_TRAIT_GLOBAL(HighGuid::Guild)
GUID_TRAIT_GLOBAL(HighGuid::Instance)
GUID_TRAIT_GLOBAL(HighGuid::TypeBattleGround)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Transport)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Unit)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::AreaTrigger)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Vehicle)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Pet)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::GameObject)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::DynamicObject)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Corpse)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::SceneObject)

#define IS_EMPTY_GUID(Guid)          (Guid == 0)

#define IS_CREATURE_GUID(Guid)       (GUID_HIPART(Guid) == HighGuid::Unit)
#define IS_PET_GUID(Guid)            (GUID_HIPART(Guid) == HighGuid::Pet)
#define IS_VEHICLE_GUID(Guid)        (GUID_HIPART(Guid) == HighGuid::Vehicle)
#define IS_CRE_OR_VEH_GUID(Guid)     (IS_CREATURE_GUID(Guid) || IS_VEHICLE_GUID(Guid))
#define IS_CRE_OR_VEH_OR_PET_GUID(Guid)(IS_CRE_OR_VEH_GUID(Guid) || IS_PET_GUID(Guid))
#define IS_PLAYER_GUID(Guid)         (GUID_HIPART(Guid) == HighGuid::Player && Guid != 0)
#define IS_GUILD_GUID(Guid)          (GUID_HIPART(Guid) == HighGuid::Guild && Guid != 0)
#define IS_UNIT_GUID(Guid)           (IS_CRE_OR_VEH_OR_PET_GUID(Guid) || IS_PLAYER_GUID(Guid))
                                                            // special case for empty guid need check
#define IS_ITEM_GUID(Guid)           (GUID_HIPART(Guid) == HighGuid::Item)
#define IS_GAMEOBJECT_GUID(Guid)     (GUID_HIPART(Guid) == HighGuid::GameObject)
#define IS_DYNAMICOBJECT_GUID(Guid)  (GUID_HIPART(Guid) == HighGuid::DynamicObject)
#define IS_CORPSE_GUID(Guid)         (GUID_HIPART(Guid) == HighGuid::Corpse)
#define IS_TRANSPORT(Guid)           (GUID_HIPART(Guid) == HighGuid::Transport)
#define IS_MO_TRANSPORT(Guid)        (GUID_HIPART(Guid) == HighGuid::Mo_Transport)
#define IS_GROUP(Guid)               (GUID_HIPART(Guid) == HighGuid::Group)
#define IS_GUILD(Guid)               (GUID_HIPART(Guid) == HighGuid::Guild)
#define IS_AREATRIGGER(Guid)         (GUID_HIPART(Guid) == HighGuid::AreaTrigger);

// l - OBJECT_FIELD_GUID
// e - OBJECT_FIELD_ENTRY for GO (except GAMEOBJECT_TYPE_MO_TRANSPORT) and creatures or UNIT_FIELD_PETNUMBER for pets
// h - OBJECT_FIELD_GUID + 1
inline uint64 MAKE_NEW_GUID(uint64 l, uint64 e, HighGuid h)
{
    if (!l)
        return 0;

    return uint64(uint64(l) | (uint64(e) << 32) | (uint64(h) << (((HighGuid)h == HighGuid::Corpse || (HighGuid)h == HighGuid::AreaTrigger) ? 48 : 52)));
}
//#define MAKE_NEW_GUID(l, e, h)   uint64(uint64(l) | (uint64(e) << 32) | (uint64(h) << ((h == HighGuid::Guild || h == HighGuid::Corpse) ? 48 : 52)))

//#define GUID_HIPART(x)   (uint32)((uint64(x) >> 52)) & 0x0000FFFF)
inline HighGuid GUID_HIPART(uint64 guid)
{
    HighGuid t = (HighGuid)((uint64(guid) >> 48) & 0x0000FFFF);
    return (HighGuid)((t == HighGuid::Corpse || t == HighGuid::AreaTrigger) ? t : (HighGuid)((uint32(t) >> 4) & 0x00000FFF));
}

// We have different low and middle part size for different guid types
#define _GUID_ENPART_2(x) 0
#define _GUID_ENPART_3(x) (uint32)((uint64(x) >> 32) & UI64LIT(0x00000000000FFFFF))
#define _GUID_LOPART_2(x) (uint32)(uint64(x)         & UI64LIT(0x00000000FFFFFFFF))
#define _GUID_LOPART_3(x) (uint32)(uint64(x)         & UI64LIT(0x00000000FFFFFFFF))

#define GET_MAX_COUNTER(x) (IsGuidHaveEnPart(x) ? uint32(0x00FFFFFF) : uint32(0xFFFFFFFF))

inline bool IsGuidHaveEnPart(HighGuid high)
{
    switch (high)
    {
        case HighGuid::Item:
        case HighGuid::Player:
        case HighGuid::DynamicObject:
        case HighGuid::Corpse:
        case HighGuid::Group:
        case HighGuid::Guild:
            return false;
        case HighGuid::GameObject:
        case HighGuid::Transport:
        case HighGuid::Unit:
        case HighGuid::Pet:
        case HighGuid::Vehicle:
        case HighGuid::Mo_Transport:
        case HighGuid::AreaTrigger:
        default:
            return true;
    }
}

#define GUID_ENPART(x) (IsGuidHaveEnPart(GUID_HIPART(x)) ? _GUID_ENPART_3(x) : _GUID_ENPART_2(x))
#define GUID_LOPART(x) (IsGuidHaveEnPart(GUID_HIPART(x)) ? _GUID_LOPART_3(x) : _GUID_LOPART_2(x))

char const* GetLogNameForHigh(HighGuid high);

inline char const* GetLogNameForGuid(uint64 guid)
{
    return GetLogNameForHigh(GUID_HIPART(guid));
}

inline char const* GetLogNameForHigh(HighGuid high)
{
    switch (high)
    {
        case HighGuid::Item:         return "item";
        case HighGuid::Player:       return  "player" ;
        case HighGuid::GameObject:   return "gameobject";
        case HighGuid::Transport:    return "transport";
        case HighGuid::Unit:         return "creature";
        case HighGuid::Pet:          return "pet";
        case HighGuid::Vehicle:      return "vehicle";
        case HighGuid::DynamicObject:return "dynobject";
        case HighGuid::Corpse:       return "corpse";
        case HighGuid::Mo_Transport: return "mo_transport";
        case HighGuid::Group:        return "group";
        case HighGuid::Guild:        return "guild";
        case HighGuid::AreaTrigger:  return "areatrigger";
        default:
            return "<unknown>";
    }
}

class ObjectGuidGeneratorBase
{
public:
    ObjectGuidGeneratorBase(uint32 start = 1) : _nextGuid(start) { }

    virtual void Set(uint32 val) { _nextGuid = val; }
    virtual uint32 Generate() = 0;
    uint32 GetNextAfterMaxUsed() const { return _nextGuid; }

protected:
    static void HandleCounterOverflow(HighGuid high)
    {
        sLog->outError(LOG_FILTER_GENERAL, "%s guid overflow!! Can't continue, shutting down server. ", GetLogNameForHigh(high));
        World::StopNow(ERROR_EXIT_CODE);
    }

    uint32 _nextGuid;
};

template<HighGuid high>
class ObjectGuidGenerator : public ObjectGuidGeneratorBase
{
public:
    explicit ObjectGuidGenerator(uint32 start = 1) : ObjectGuidGeneratorBase(start) { }

    uint32 Generate() override
    {
        if (_nextGuid >= GET_MAX_COUNTER(high) - 1)
            HandleCounterOverflow(high);
        return _nextGuid++;
    }
};

#endif


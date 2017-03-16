# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
    Custom/event_april.cpp
    Custom/duel_reset.cpp
    Custom/duel_zone.cpp
    Custom/ArenaFastStart.cpp
    Custom/teleguy.cpp
    Custom/petguy.cpp
    Custom/arena_spectator.cpp
    Custom/npc_transmogrify.cpp
)

message("  -> Prepared: Custom")

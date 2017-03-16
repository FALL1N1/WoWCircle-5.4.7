# Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources_GateSettingSun Pandaria/GateSettingSun/*.hpp Pandaria/GateSettingSun/*.cpp Pandaria/GateSettingSun/*.hxx)
file(GLOB_RECURSE sources_HeartOfFear Pandaria/HeartOfFear/*.hpp Pandaria/HeartOfFear/*.cpp Pandaria/HeartOfFear/*.hxx)
file(GLOB_RECURSE sources_MogushanPalace Pandaria/MogushanPalace/*.hpp Pandaria/MogushanPalace/*.cpp Pandaria/MogushanPalace/*.hxx)
file(GLOB_RECURSE sources_MogushanVault Pandaria/MogushanVault/*.hpp Pandaria/MogushanVault/*.cpp Pandaria/MogushanVault/*.hxx)
file(GLOB_RECURSE sources_ShadopanMonastery Pandaria/ShadopanMonastery/*.hpp Pandaria/ShadopanMonastery/*.cpp Pandaria/ShadopanMonastery/*.hxx)
file(GLOB_RECURSE sources_SiegeNiuzaoTemple Pandaria/SiegeNiuzaoTemple/*.hpp Pandaria/SiegeNiuzaoTemple/*.cpp Pandaria/SiegeNiuzaoTemple/*.hxx)
file(GLOB_RECURSE sources_StormstoutBrewery Pandaria/StormstoutBrewery/*.hpp Pandaria/StormstoutBrewery/*.cpp Pandaria/StormstoutBrewery/*.hxx)
file(GLOB_RECURSE sources_TempleJadeSerpent Pandaria/TempleJadeSerpent/*.hpp Pandaria/TempleJadeSerpent/*.cpp Pandaria/TempleJadeSerpent/*.hxx)
file(GLOB_RECURSE sources_TerraceOfEndlessSpring Pandaria/TerraceOfEndlessSpring/*.hpp Pandaria/TerraceOfEndlessSpring/*.cpp Pandaria/TerraceOfEndlessSpring/*.hxx)
file(GLOB_RECURSE sources_ThroneOfThunder Pandaria/ThroneOfThunder/*.hpp Pandaria/ThroneOfThunder/*.cpp Pandaria/ThroneOfThunder/*.hxx)
file(GLOB_RECURSE sources_SiegeOfOrgrimmar Pandaria/SiegeOfOrgrimmar/*.hpp Pandaria/SiegeOfOrgrimmar/*.cpp Pandaria/SiegeOfOrgrimmar/*.hxx)
file(GLOB_RECURSE sources_WanderingIsland Pandaria/WanderingIsland/*.hpp Pandaria/WanderingIsland/*.cpp Pandaria/WanderingIsland/*.hxx)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_GateSettingSun}
  ${sources_HeartOfFear}
  ${sources_MogushanPalace}
  ${sources_MogushanVault}
  ${sources_ShadopanMonastery}
  ${sources_SiegeNiuzaoTemple}
  ${sources_StormstoutBrewery}
  ${sources_TempleJadeSerpent}
  ${sources_TerraceOfEndlessSpring}
  ${sources_ThroneOfThunder}
  ${sources_SiegeOfOrgrimmar}
  ${sources_WanderingIsland}

  Pandaria/boss_sha_of_anger.cpp
  Pandaria/boss_galion.cpp
  Pandaria/boss_nalak.cpp
  Pandaria/boss_chi_ji.cpp
  Pandaria/boss_ordos.cpp
  Pandaria/boss_oondasta.cpp
  
  Pandaria/celestial_challenge.cpp

  Pandaria/the_veiled_stair.cpp
  Pandaria/the_veiled_stair.h
  Pandaria/townlong_steppes.cpp
  Pandaria/townlong_steppes.h
  Pandaria/dread_wastes.cpp
  Pandaria/dread_wastes.h
  Pandaria/valley_of_the_four_winds.cpp
  Pandaria/valley_of_the_four_winds.h
  Pandaria/krasarang_wilds.cpp
  Pandaria/krasarang_wilds.h
  Pandaria/kun_lai_summit.cpp
  Pandaria/kun_lai_summit.h
  Pandaria/jade_forest.cpp
  Pandaria/jade_forest.h
  Pandaria/vale_of_eternal_blossoms.cpp
  Pandaria/vale_of_eternal_blossoms.h
  Pandaria/isle_of_thunder.h
  Pandaria/isle_of_thunder.cpp
  Pandaria/isle_of_giants.cpp
  Pandaria/timeless_isle.cpp
  Pandaria/timeless_isle.h
)

message("  -> Prepared: Pandaria")

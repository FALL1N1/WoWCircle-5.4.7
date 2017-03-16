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

  Scenarios/TheramoresFall_A/instance_theramores_fall_a.cpp
  Scenarios/TheramoresFall_A/theramores_fall_a.cpp
  Scenarios/TheramoresFall_A/theramores_fall_a.h
  
  Scenarios/TheramoresFall_H/instance_theramores_fall_h.cpp
  Scenarios/TheramoresFall_H/theramores_fall_h.cpp
  Scenarios/TheramoresFall_H/theramores_fall_h.h
  Scenarios/TheramoresFall_H/theramores_fall_a.h
  
  Scenarios/BloodOnTheSnow/blood_on_the_snow.h
  Scenarios/BloodOnTheSnow/blood_on_the_snow.cpp
  Scenarios/BloodOnTheSnow/instance_blood_on_the_snow.cpp
  
  Scenarios/PursuingTheBlackHarvest/pursuing_the_black_harvest.h
  Scenarios/PursuingTheBlackHarvest/pursuing_the_black_harvest.cpp
  Scenarios/PursuingTheBlackHarvest/instance_pursuing_the_black_harvest.cpp
  
  Scenarios/AssaultOnZanvess/assault_on_zanvess.h
  Scenarios/AssaultOnZanvess/assault_on_zanvess.cpp
  Scenarios/AssaultOnZanvess/instance_assault_on_zanvess.cpp
)

message("  -> Prepared: Scenarios")

//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SIEGE_NIUZOA_TEMPLE_HPP
# define SIEGE_NIUZOA_TEMPLE_HPP

# include "SpellScript.h"
# include "Map.h"
# include "Creature.h"
# include "CreatureAIImpl.h"
# include "ScriptPCH.h"
# include "ScriptMgr.h"
# include "InstanceScript.h"
# include "AchievementMgr.h"
# include "ObjectMgr.h"
# include "Player.h"
# include "SpellAuraEffects.h"
# include "ScriptedGossip.h"
# include "PassiveAI.h"

enum DataTypes
{
    DATA_JINBAK,
    DATA_VOJAK,
    DATA_PAVALAK,
    DATA_NERONOK
};

enum CreaturesIds
{
    NPC_JINBAK = 61567,
    NPC_VOJAK = 61634,
    NPC_PAVALAK = 61485,
    NPC_NERONOK = 62205,

    NPC_SAP_PUDDLE = 61613
};

enum GameobjectIds
{
    GO_HARDENED_RESIN       = 213174,
    GO_DOOR                 = 212921, 
    GO_INVISIBLE_FIRE_WALL  = 210097,

    ChallengeModeDoor       = 211989
};

enum eScenarioDatas
{
    ScenarioID  = 49,
    /// Bosses datas
    Jinbak      = 19249,
    Vojak       = 19250,
    Pavalak     = 19251,
    Neronok     = 19252,
    Ennemies    = 20023,
    KillCount   = 65
};

#endif ///< SIEGE_NIUZOA_TEMPLE_HPP
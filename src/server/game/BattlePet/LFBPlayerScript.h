#pragma once

#include "Common.h"
#include "SharedDefines.h"
#include "ScriptMgr.h"

class Player;

class LFBPlayerScript : public PlayerScript
{
    public:
        LFBPlayerScript();

        void OnLogout(Player* player);
        void OnLogin(Player* player);
};
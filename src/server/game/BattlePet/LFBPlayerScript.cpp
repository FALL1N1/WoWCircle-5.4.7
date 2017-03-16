
#include "Common.h"
#include "SharedDefines.h"
#include "Player.h"
#include "LFBMgr.h"
#include "LFBPlayerScript.h"
#include "ScriptMgr.h"
#include "ObjectAccessor.h"

LFBPlayerScript::LFBPlayerScript() : PlayerScript("LFBPlayerScript")
{
}

void LFBPlayerScript::OnLogout(Player* player)
{
    sLFBMgr->SetOnline(player, false);
}

void LFBPlayerScript::OnLogin(Player* player)
{
    //sLFBMgr->SetOnline(player, true);
}

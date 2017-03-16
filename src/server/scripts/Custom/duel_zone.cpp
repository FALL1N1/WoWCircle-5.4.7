#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Object.h"

enum DuelSpectateMode
{
    SpectateDisabled = 0,
    SpectateEnabled = 1,
};

class duel_zone : public PlayerScript
{
    public:
        duel_zone() : PlayerScript("duel_zone") {}

        const static uint32 AllowedZone = 268;

        void OnDuelEnd(Player* winner, Player* looser, DuelCompleteType type)
        {
            if (winner->GetAreaId() != AllowedZone)
                return;

            if (type == DUEL_WON)
            {
                winner->RemoveArenaSpellCooldowns();
                looser->RemoveArenaSpellCooldowns();

                winner->SetHealth(winner->GetMaxHealth());
                looser->SetHealth(looser->GetMaxHealth());

                if (winner->getPowerType() == POWER_MANA)
                    winner->SetPower(POWER_MANA, winner->GetMaxPower(POWER_MANA));

                if (looser->getPowerType() == POWER_MANA)
                    looser->SetPower(POWER_MANA, looser->GetMaxPower(POWER_MANA));
            }

            SwitchSpectateMode(winner, SpectateEnabled);
            SwitchSpectateMode(looser, SpectateEnabled);
        }

        void OnDuelStart(Player* player1, Player* player2)
        {
            if (player1->GetAreaId() != AllowedZone)
                return;

            uint32 uniqValue = getMSTime();
            SwitchSpectateMode(player1, uniqValue);
            SwitchSpectateMode(player2, uniqValue);
        }

        void OnUpdateZone(Player* p_Player, uint32 p_NewZone, uint32 p_OldZone, uint32 p_NewArea, uint32 p_OldArea)
        {
            switch (p_NewArea)
            {
                case AllowedZone:
                    SwitchSpectateMode(p_Player, SpectateEnabled);
                    break;
                default:
                    SwitchSpectateMode(p_Player, SpectateDisabled);
                    break;
            }
        }

        void SwitchSpectateMode(Player* player, uint32 value)
        {
            player->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_DUEL, value);
            player->UpdateObjectVisibility();
        }
};

void AddSC_DuelZone()
{
    new duel_zone();
}
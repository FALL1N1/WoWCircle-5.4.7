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

#include "Player.h"
#include "ObjectMgr.h"

SpectatorAddonMsg::SpectatorAddonMsg()
{
    for (uint8 i = 0; i < SPECTATOR_PREFIX_COUNT; ++i)
        prefixFlags[i] = false;

    player = NULL;
    name = "";
    target = NULL;
    isAlive = false;
    pClass = CLASS_WARRIOR;
    maxHP = 0;
    maxPower = 0;
    currHP = 0;
    currPower = 0;
    powerType = POWER_MANA;
    spellId = 0;
    castTime = 0;
    team = ALLIANCE;
    time = 47*MINUTE*IN_MILLISECONDS;
    family = 0;
    petHP = 0;
    cooldown = 0;
    aCaster = 0;
    aSpellId = 0;
    aIsDebuff = false;
    aType = 0;
    aDuration = 0;
    aExpire = 0;
    aStack = 0;
    aRemove = false;
}

bool SpectatorAddonMsg::CanSandAura(uint32 auraID)
{
    const SpellInfo *spell = sSpellMgr->GetSpellInfo(auraID);
    if (!spell)
        return false;

    if (spell->SpellIconID == 1)
        return false;

    if ((spell->Attributes & SPELL_ATTR0_HIDDEN_CLIENTSIDE) || (spell->Attributes & SPELL_ATTR0_HIDE_IN_COMBAT_LOG))
        return false;

    return true;
}

void SpectatorAddonMsg::CreateAura(uint32 _caster, uint32 _spellId, bool _isDebuff, uint8 _type, int32 _duration, int32 _expire, uint16 _stack, bool _isRemove)
{
    if (!CanSandAura(_spellId))
        return;

    aCaster = _caster;
    aSpellId = _spellId;
    aIsDebuff = _isDebuff;
    aType = _type;
    aDuration = _duration;
    aExpire = _expire;
    aStack = _stack;
    aRemove = _isRemove;
    EnableFlag(SPECTATOR_PREFIX_AURA);
}

WorldPacket& SpectatorAddonMsg::CachedMessage()
{
    if (!cachedMessage.empty())
        return cachedMessage;

    std::string msg;

    if (!isFilledIn(SPECTATOR_PREFIX_PLAYER))
    {
        //sLog->outString("SPECTATOR ADDON: player is not filled in.");
    }
    else
    {
        for (uint8 i = 0; i < SPECTATOR_PREFIX_COUNT; ++i)
            if (isFilledIn(i))
            {
                switch (i)
                {
                    case SPECTATOR_PREFIX_PLAYER:
                        char buffer[64];
                        sprintf(buffer, "%i;", player);
                        msg += buffer;
                        break;
                    case SPECTATOR_PREFIX_NAME:
                        msg += "NME=" + name + ";";
                        break;
                    case SPECTATOR_PREFIX_TARGET:
                    {
                        char buffer[20];
                        sprintf(buffer, "TRG=%i;", target);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_TEAM:
                    {
                        char buffer[20];
                        sprintf(buffer, "TEM=%i;", (uint16)team);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_STATUS:
                    {
                        char buffer[20];
                        sprintf(buffer, "STA=%d;", isAlive);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_CLASS:
                    {
                        char buffer[20];
                        sprintf(buffer, "CLA=%i;", (int)pClass);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_MAXHP:
                    {
                        char buffer[30];
                        sprintf(buffer, "MHP=%i;", maxHP);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_CURHP:
                    {
                        char buffer[30];
                        sprintf(buffer, "CHP=%i;", currHP);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_MAXPOWER:
                    {
                        char buffer[30];
                        sprintf(buffer, "MPW=%i;", maxPower);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_CURPOWER:
                    {
                        char buffer[30];
                        sprintf(buffer, "CPW=%i;", currPower);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_POWERTYPE:
                    {
                        char buffer[20];
                        sprintf(buffer, "PWT=%i;", (uint8)powerType);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_SPELL:
                    {
                        char buffer[80];
                        sprintf(buffer, "SPE=%i,%i;", spellId, castTime);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_ARENA_TIMER:
                    {
                        char buffer[20];
                        sprintf(buffer, "TIM=%i;", time);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_PET:
                    {
                        char buffer[10];
                        sprintf(buffer, "PET=%i;", family);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_PET_HP:
                    {
                        char buffer[30];
                        sprintf(buffer, "PHP=%i;", petHP);
                        msg += buffer;
                        break;
                    }
                    case SPECTATOR_PREFIX_AURA:
                    {
                        char buffer[300];
                        sprintf(buffer, "AUR=%i,%i,%i,%i,%i,%i,%i,0x%X;", aRemove, aStack,
                            aExpire, aDuration,
                            aSpellId, aType,
                            aIsDebuff, aCaster);
                        msg += buffer;
                        break;
                    }
                        case SPECTATOR_PREFIX_COOLDOWN:
                        {
                            char buffer[80];
                            sprintf(buffer, "CD=%i,%i;", spellId, cooldown);
                            msg += buffer;
                            break;
                        }
                }
            }

        //if (!msg.empty())
        //    msg.insert(0, "ARENASPEC\t");
    }

    /*cachedMessage.Initialize(SMSG_MESSAGE_CHAT, 200);
    cachedMessage << uint8(CHAT_MSG_WHISPER);
    cachedMessage << uint32(LANG_ADDON);
    cachedMessage << uint64(player);
    cachedMessage << uint32(0);
    cachedMessage << uint64(player);
    cachedMessage << std::string("ARENASPEC");
    cachedMessage << uint32(msg.length() + 1);
    cachedMessage << msg;
    cachedMessage << uint8(CHAT_TAG_NONE);*/

    ObjectGuid playerGUID = player;
    ObjectGuid groupGuid = 0;
    ObjectGuid guildGuid = 0;
    uint32 speakerNameLength = name.length();
    uint32 targetLength = name.length();
    std::string addonPrefix = "ARENASPEC";
    uint32 prefixeLength = addonPrefix.length();
    uint32 channelLength = 0;
    bool hasChatTag = false;
    uint32 language = LANG_ADDON;
    uint32 achievementId = 0;
    uint32 messageLength = msg.length();
    bool sendRealmId = false;
    bool bit5264 = false;
    uint32 msgtype = CHAT_MSG_WHISPER;

    cachedMessage.Initialize(SMSG_MESSAGE_CHAT, 100);     // guess size
    cachedMessage.WriteBit(false);                     // has guild GUID - FAKE GUID
    cachedMessage.WriteBit(false);               // has sender GUID - FAKE GUID
    uint8 bitsOrder4[8] = { 4, 5, 1, 0, 2, 6, 7, 3 };
    cachedMessage.WriteBitInOrder(0, bitsOrder4);
    cachedMessage.WriteBit(true);                 // (inversed) has chat tag
    cachedMessage.WriteBit(false);              // has lang
    uint8 bitsOrder[8] = { 2, 7, 0, 3, 4, 6, 1, 5 };
    cachedMessage.WriteBitInOrder(playerGUID, bitsOrder);
    cachedMessage.WriteBit(false);            // Unk bit 5268
    cachedMessage.WriteBit(true);            // Has achievement
    cachedMessage.WriteBit(true);         // has receiver
    cachedMessage.WriteBit(true);     // has sender
    cachedMessage.WriteBit(false);     // hasText
    cachedMessage.WriteBit(false);         // has receiver GUID - FAKE GUID
    uint8 bitsOrder3[8] = { 5, 7, 6, 4, 3, 2, 1, 0 };
    cachedMessage.WriteBitInOrder(playerGUID, bitsOrder3);
    cachedMessage.WriteBit(true);             // sendRealmId
    //if (playerGUID)
    //    cachedMessage.WriteBits(targetLength, 11);
    //if (this != NULL)
    //    cachedMessage.WriteBits(speakerNameLength, 11);
    cachedMessage.WriteBit(false);                          // has group GUID - FAKE GUID
    uint8 bitsOrder2[8] = { 5, 2, 6, 1, 7, 3, 0, 4 };
    cachedMessage.WriteBitInOrder(groupGuid, bitsOrder2);
    cachedMessage.WriteBit(true);                                   // (inversed) unk bit 5264
    // Must be inversed
    //if (hasChatTag)
        //cachedMessage.WriteBits(CHAT_TAG_NONE, 9);
    //if (messageLength)
        cachedMessage.WriteBits(messageLength, 12);
    cachedMessage.WriteBit(false);                     // Unk bit
    cachedMessage.WriteBit(false);                     // has prefix
    cachedMessage.WriteBit(true);                                // has channel
    //if (prefixeLength)
        cachedMessage.WriteBits(prefixeLength, 5);
    //if (hasChannel)
    //    cachedMessage.WriteBits(channelLength, 7);
    cachedMessage.WriteBit(true);                      // unk uint block
    uint8 byteOrder3[8] = { 7, 2, 1, 4, 6, 5, 3, 0 };
    cachedMessage.WriteBytesSeq(guildGuid, byteOrder3);
    uint8 byteOrder[8] = { 5, 3, 2, 4, 1, 0, 7, 6 };
    cachedMessage.WriteBytesSeq(groupGuid, byteOrder);
    cachedMessage << uint8(msgtype);
    //if (sendRealmId)
    //    cachedMessage << uint32(realmID);                               // realmd id / flags
    //if (prefixeLength)
        cachedMessage.append(addonPrefix.c_str(), prefixeLength);
    uint8 byteOrder1[8] = { 4, 2, 3, 0, 6, 7, 5, 1 };
    cachedMessage.WriteBytesSeq(playerGUID, byteOrder1);
    uint8 byteOrder2[8] = { 6, 1, 0, 2, 4, 5, 7, 3 };
    cachedMessage.WriteBytesSeq(playerGUID, byteOrder2);
    //if (achievementId)
    //    cachedMessage << uint32(achievementId);
    //if (targetLength)
    //    cachedMessage.append(name.c_str(), targetLength);
    //if (messageLength)
        cachedMessage.append(msg.c_str(), messageLength);
    //if (speakerNameLength)
    //    cachedMessage.append(name.c_str(), speakerNameLength);
    cachedMessage << uint8(LANG_ADDON);
    //if (channelLength && hasChannel)
        //data->append(channel.c_str(), channelLength);

    return cachedMessage;
}

bool SpectatorAddonMsg::SendPacket(uint64 receiver)
{
    Player* rPlayer = ObjectAccessor::FindPlayer(receiver);
    if (!rPlayer)
        return false;

    rPlayer->GetSession()->SendPacket(&CachedMessage());
    return true;
}

bool SpectatorAddonMsg::SendPacket(SpectatorAddonMsg& msg, uint64 receiver)
{
    Player* rPlayer = ObjectAccessor::FindPlayer(receiver);
    if (!rPlayer)
        return false;

    rPlayer->GetSession()->SendPacket(&msg.CachedMessage());
    return true;
}

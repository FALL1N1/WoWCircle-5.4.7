#include "InterRealmOpcodes.h"
#include "InterRealmSession.h"
#include "World.h"
#include "Config.h"
#include "AccountMgr.h"
#include "Chat.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "BattlegroundMgr.h"
#include "AnticheatMgr.h"
#include "GuildMgr.h"

#ifdef NOT_WINDOWS
#include <netinet/tcp.h>
#endif

#define SLEEP_TIME 30*IN_MILLISECONDS

class IRReactorRunnable : protected ACE_Task_Base
{
    public:

        IRReactorRunnable() :
            m_Reactor(0),
            m_ThreadId(-1)
        {
            ACE_Reactor_Impl* imp = 0;

            #if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)

            imp = new ACE_Dev_Poll_Reactor();

            imp->max_notify_iterations (128);
            imp->restart (1);

            #else

            imp = new ACE_TP_Reactor();
            imp->max_notify_iterations (128);

            #endif

            m_Reactor = new ACE_Reactor (imp, 1);

            m_IRSocket = NULL;
        }

        virtual ~IRReactorRunnable()
        {
            Stop();
            Wait();

            delete m_Reactor;
        }

        void Stop()
        {
            m_Reactor->end_reactor_event_loop();
        }

        int Start()
        {
            if (m_ThreadId != -1)
                return -1;

            return (m_ThreadId = activate());
        }

        void Wait() { ACE_Task_Base::wait(); }

        int SetSocket (IRSocket* sock)
        {
            sock->AddReference();
            sock->reactor(m_Reactor);
            m_IRSocket = sock;

            return 0;
        }

        ACE_Reactor* GetReactor()
        {
            return m_Reactor;
        }

    protected:

        virtual int svc()
        {
            ACE_ASSERT (m_Reactor);

            while (!m_Reactor->reactor_event_loop_done())
            {
                // dont be too smart to move this outside the loop
                // the run_reactor_event_loop will modify interval
                ACE_Time_Value interval (0, 10000);

                if (m_Reactor->run_reactor_event_loop (interval) == -1)
                    break;

                if (m_IRSocket)
                {
                    if (m_IRSocket->Update() == -1)
                    {
                        m_IRSocket->CloseSocket();
                        m_IRSocket->RemoveReference();
                        m_IRSocket = NULL;
                    }
                }
            }

            return 0;
        }

    private:
        typedef std::atomic<long> AtomicInt;

        ACE_Reactor* m_Reactor;
        int m_ThreadId;

        IRSocket* m_IRSocket;
};

InterRealmSession::InterRealmSession() : m_rand(0), m_tunnel_open(false),
m_RecvWPct(0), m_RecvPct()
{
    m_IsConnected = false;
    m_force_stop = false;
    m_port = 12345;
    m_ir_id = 0;
    m_IRSocket = NULL;
    m_SockOutKBuff = -1;
    m_SockOutUBuff = 65536;
    m_UseNoDelay = true;
    m_needProcessDisconnect = false;

    m_ProtocolVersion = ConfigMgr::GetIntDefault("InterRealm.Protocol", PROTOCOL_WITH_KILLS);
}

InterRealmSession::~InterRealmSession()
{
}

int InterRealmSession::OnSocketOpen(IRSocket* socket)
{
    m_SockOutKBuff = ConfigMgr::GetIntDefault ("Network.OutKBuff", -1);
    m_SockOutUBuff = ConfigMgr::GetIntDefault ("Network.OutUBuff", 65536);

    // set some options here
    if (m_SockOutKBuff >= 0)
    {
        if (socket->peer().set_option (SOL_SOCKET,
            SO_SNDBUF,
            (void*) & m_SockOutKBuff,
            sizeof (int)) == -1 && errno != ENOTSUP)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "InterRealmSession::OnSocketOpen set_option SO_SNDBUF");
            return -1;
        }
    }

    static const int ndoption = 1;

    // Set TCP_NODELAY.
    if (m_UseNoDelay)
    {
        if (socket->peer().set_option (ACE_IPPROTO_TCP,
            TCP_NODELAY,
            (void*)&ndoption,
            sizeof (int)) == -1)
        {
            sLog->outError(LOG_FILTER_INTERREALM, "InterRealmSession::OnSocketOpen peer().set_option TCP_NODELAY errno = %s", ACE_OS::strerror (errno));
            return -1;
        }
    }

    socket->m_OutBufferSize = static_cast<size_t> (m_SockOutUBuff);


    return m_Reactor->SetSocket(socket);
}

void InterRealmSession::ProcessPlayersAfterDisconnect()
{
    m_needProcessDisconnect = false;

    const SessionMap& _sessionMap = sWorld->GetAllSessions();
    if (!_sessionMap.empty())
    {
        for (SessionMap::const_iterator itr = _sessionMap.begin(); itr != _sessionMap.end(); ++itr)
        {
            if (itr->second->GetInterRealmBG())
            {
                itr->second->SetInterRealmBG(0);

                if (Player* pPlayer = itr->second->GetPlayer())
                {
                    pPlayer->SetForceCleanupChannels(true);
                    pPlayer->GetSession()->LoadAfterInterRealm();

                    pPlayer->ScheduleDelayedOperation(DELAYED_BG_MOUNT_RESTORE);
                    pPlayer->ScheduleDelayedOperation(DELAYED_BG_TAXI_RESTORE);
                    pPlayer->ScheduleDelayedOperation(DELAYED_BG_GROUP_RESTORE);
                    pPlayer->ScheduleDelayedOperation(DELAYED_BG_UPDATE_ZONE);
                    pPlayer->ScheduleDelayedOperation(DELAYED_BG_NAME_RESTORE);

                    pPlayer->TeleportToBGEntryPoint(true);  

                    pPlayer->SetInterRealmZoneId(0);
                    pPlayer->SetInterRealmAreaId(0);
                    pPlayer->SetInterRealmMapId(0);

                    // remove group frame
                    /*WorldPacket data(SMSG_GROUP_LIST, 1+1+1+1+8+4+4+8);
                    data << uint8(0x10) << uint8(0) << uint8(0) << uint8(0);
                    data << uint64(0) << uint32(0) << uint32(0) << uint64(0);
                    pPlayer->GetSession()->SendPacket(&data);*/
                }
            }
        }
    }
}

void InterRealmSession::ClearSocket()
{
    m_tunnel_open = false;

    if (m_IRSocket)
    {
        m_IRSocket = NULL;
    }

    if (m_Reactor)
    {
        m_Reactor->Stop();
        delete m_Reactor;
    }

    if (m_Connector)
    {
        m_Connector->close();
        delete m_Connector;
    }
}

void InterRealmSession::run()
{
    sLog->outError(LOG_FILTER_INTERREALM, "Connecting to InterRealm...");

    if (!sWorld->getBoolConfig(CONFIG_INTERREALM_ENABLE))
    {
        sLog->outError(LOG_FILTER_INTERREALM, "InterRealm is disabled.");
        return;
    }

    m_IP = ConfigMgr::GetStringDefault("InterRealm.IP", "0.0.0.0");
    m_port = uint16(ConfigMgr::GetIntDefault("InterRealm.Port", 12345));
    m_ir_id = ConfigMgr::GetIntDefault("InterRealm.Id", 1);

    sLog->outError(LOG_FILTER_INTERREALM, "Loaded InterRealm configuration, %s:%u, id %u", m_IP.c_str(), m_port, m_ir_id);    

    m_Connector = NULL;

    ACE_INET_Addr connect_addr (m_port, m_IP.c_str());

    while (!World::IsStopped())
    {
        if (!m_IRSocket || m_IRSocket->IsClosed())
        {
            //int i_ret = m_Connector->open(m_Reactor->GetReactor(), ACE_NONBLOCK);
            sLog->outError(LOG_FILTER_INTERREALM, "Trying to connect to interrealm.");

            m_Reactor = new IRReactorRunnable();
            m_Connector = new IRSocketConnector();

            int ret = m_Connector->connect(m_IRSocket, connect_addr);
            if (ret != 0)
            {
                ClearSocket();
                sLog->outError(LOG_FILTER_INTERREALM, "Cannot connect interrealm");    
                ACE_Based::Thread::Sleep(30000);
                continue;
            }

            m_Reactor->Start();

            m_force_stop = false;

            sLog->outError(LOG_FILTER_INTERREALM, "Sending 'hello' message to InterRealm.");
            
            m_rand = urand(0, 255);
            WorldPacket hello_packet(IR_CMSG_HELLO, 10 + 1 + 1 + 1 + 1 + 1);
            hello_packet << std::string("HELO");
            hello_packet << uint8(m_rand);
            SendPacket(&hello_packet);
        }

        ACE_Based::Thread::Sleep(30000);
    }
}

void InterRealmSession::SendPing(uint64 guid, uint32 ping, uint32 latency)
{
    WorldPacket packet(IR_CMSG_PING, 16);
    packet << guid;
    packet << latency;
    packet << ping;
    SendPacket(&packet);
}

void InterRealmSession::SendTunneledPacket(uint64 playerGuid, WorldPacket const* packet)
{
    if (playerGuid == 0)
    {
        delete packet;
        return;
    }

    if (!m_tunnel_open)
    {
        delete packet;
        return;
    }
    
    WorldPacket tmpPacket(IR_CMSG_TUNNEL_PACKET, 8 + 2 + packet->size());
    tmpPacket << (uint64)playerGuid;
    tmpPacket << (uint16)packet->GetOpcode();

    if (packet->size() > 0)
        tmpPacket.append(packet->contents(), packet->size());

    delete packet;
    
    SendPacket(&tmpPacket);
}

void InterRealmSession::SendTunneledPacketToClient(uint64 guid, WorldPacket const *packet)
{
    if (guid == 0)
        return;

    if (packet == NULL)
        return;

    if (!m_IRSocket)
        return;

    WorldPacket tmpPacket(IR_TUNNELED_PACKET_TO_CLIENT, 8 + 2 + packet->size());
    tmpPacket << (uint64)guid;
    tmpPacket << (uint16)packet->GetOpcode();

    for (unsigned int i = 0; i < packet->size(); ++i)
        tmpPacket << (uint8)packet->contents()[i];

    SendPacket(&tmpPacket);
}

void InterRealmSession::SendPacket(WorldPacket const* packet)
{
    if (packet == NULL)
        return;

    if (packet->GetOpcode() != IR_CMSG_WHO_AM_I && 
        packet->GetOpcode() != IR_CMSG_HELLO && !IsConnected())
        return;

    if (!m_IRSocket || m_IRSocket->IsClosed())
        return;

    if (m_IRSocket->SendPacket(packet) == -1)
    {
        //sLog->outInterRealm("[INTERREALM] Cannot send packet %u, closing socket.", packet->GetOpcode());
        //m_IRSocket->CloseSocket();
    }
}

void InterRealmSession::SendPSysMessage(Player *player, char const *format, ...)
{
    va_list ap;
    char str[2048];
    va_start(ap, format);
    vsnprintf(str, 2048, format, ap);
    va_end(ap);

    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = strdup(str);
    char* pos = buf;

    while (char* line = ChatHandler::LineFromMessage(pos))
    {
        ChatHandler(player).FillSystemMessageData(&data, line);
        SendTunneledPacketToClient(player->GetGUID(), &data);
    }

    free(buf);
}

void InterRealmSession::SendServerAnnounce(uint64 guid, std::string const &text)
{
    WorldPacket data(IR_SMSG_SERVER_ANNOUNCE, 8 + text.size());
    data << guid;
    data << text;
    SendPacket(&data);
}

void InterRealmSession::Handle_Unhandled(WorldPacket& recvPacket)
{
    //sLog->outInterRealm("[INTERREALM] Unhandled Packet with IROpcode %u received !",recvPacket.GetOpcode());
}

void InterRealmSession::Handle_Null(WorldPacket& recvPacket)
{
    //sLog->outInterRealm("[INTERREALM] Packet with Invalid IROpcode %u received !",recvPacket.GetOpcode());
}

void InterRealmSession::Handle_Hello(WorldPacket& packet)
{
    //sLog->outInterRealm("[INTERREALM] Received packet IR_SMSG_HELLO");

    std::string _hello;
    uint8 _rand, _resp;

    packet >> _hello;
    packet >> _rand;
    packet >> _resp;

    if (strcmp(_hello.c_str(), "HELO") != 0)
    {
       sLog->outError(LOG_FILTER_INTERREALM, "closing socket !");
        m_force_stop = true;
    }

    if (_rand != m_rand)
    {
        sLog->outError(LOG_FILTER_INTERREALM, "Random hello check is incorrect, closing socket");
        m_force_stop = true;
    }

    if (_resp == IR_HELO_RESP_PROTOCOL_MISMATCH)
        sLog->outError(LOG_FILTER_INTERREALM, "InterRealm Protocol Mismatch, closing doors to me !");

    if(_resp == IR_HELO_RESP_POLITE)
        sLog->outError(LOG_FILTER_INTERREALM, "Server like to be polite, closing doors to me !");

    if  (!m_force_stop && _resp == IR_HELO_RESP_OK)
    {
        //sLog->outInterRealm("[INTERREALM] Hello was succeed. Sending id...");

        WorldPacket packet(IR_CMSG_WHO_AM_I, 4);
        packet << uint32(m_ir_id); // Realm Id
        packet << float(sWorld->getRate(RATE_HONOR));
        packet << float(sWorld->getRate(RATE_HONOR_PREMIUM));
        //packet << uint32(sWorld->getIntConfig(CONFIG_MAX_HONOR_POINTS));
        //packet << uint32(sWorld->getIntConfig(CONFIG_MAX_ARENA_POINTS));
        packet << float(sWorld->getRate(RATE_REPUTATION_GAIN)); // TODO: send session RateType::ReputationGain there
        packet << float(sWorld->getRate(RATE_REPUTATION_GAIN_PREMIUM));

        packet << m_ProtocolVersion;

        SendPacket(&packet);
    }
}

void InterRealmSession::Handle_TunneledPacket(WorldPacket& recvPacket)
{
    uint64 playerGuid;
    uint16 opcodeId;
    recvPacket >> playerGuid;
    recvPacket >> opcodeId;

    recvPacket.eraseFirst(10); // remove playerGuid and opcodeId data
    recvPacket.SetOpcode(opcodeId);
    recvPacket.rfinish();

    // Now we have standart packet

    if (Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        pPlayer->GetSession()->SendPacket(&recvPacket, true);
    }
}

void InterRealmSession::Handle_WhoAmI(WorldPacket& packet)
{
    uint8 _valid;
    packet >> _valid;

    if (_valid == 0)
    {
        m_tunnel_open = true;

        SetConnected(true);

        sLog->outError(LOG_FILTER_INTERREALM, "Tunnel is opened.");

        SendBattlegroundHolidaysQuery();
    }
    else
    {
        m_force_stop = true;
        sLog->outError(LOG_FILTER_INTERREALM, "Registration was failed.");
    }
}

void InterRealmSession::Handle_CheckPlayers(WorldPacket& packet)
{
    //sLog->outInterRealm(LOG_FILTER_GENERAL, "[INTERREALM] Received a packet IR_SMSG_CHECK_PLAYERS");
    
    uint32 num_players;
    std::vector<uint64> playerGuids;

    packet >> num_players;

    for (uint32 i = 0; i < num_players; ++i)
    {
        uint64 playerGuid;
        uint8 inIRBG;
        packet >> playerGuid;
        packet >> inIRBG;
        if (inIRBG == 0)
        {
            if (!sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
                playerGuids.push_back(playerGuid);
        }
        else
        {
            if (Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
            {
                if (!pPlayer->GetSession()->GetInterRealmBG())
                    playerGuids.push_back(playerGuid);
            }
            else
                playerGuids.push_back(playerGuid);
        }
    }

    if (!playerGuids.empty())
    {
        WorldPacket pckt(IR_CMSG_CHECK_PLAYERS, 1 + 4 + (8 * playerGuids.size()));
        pckt << uint8(1); // has offline players
        pckt << uint32(playerGuids.size());

        for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
            pckt << uint64((*itr));

        SendPacket(&pckt);
    }
    else
    {
        WorldPacket pckt(IR_CMSG_CHECK_PLAYERS, 1);
        pckt << uint8(0);
        SendPacket(&pckt);
    }
}

void InterRealmSession::Handle_DebugBGResp(WorldPacket& packet)
{
    uint8 val;

    packet >> val;

    sWorld->SendWorldText((val != 0) ? LANG_DEBUG_BG_ON : LANG_DEBUG_BG_OFF);
}

void InterRealmSession::Handle_DebugArenaResp(WorldPacket& packet)
{
    uint8 val;

    packet >> val;

    sWorld->SendWorldText((val != 0) ? LANG_DEBUG_ARENA_ON : LANG_DEBUG_ARENA_OFF);
}

void InterRealmSession::Handle_RegisterPlayerResp(WorldPacket& recvPacket)
{
    //sLog->outInterRealm("[INTERREALM] Received a packet IR_SMSG_REGISTER_PLAYER_RESP");
    uint64 playerGuid;
    uint8 _valid;

    recvPacket >> _valid;
    recvPacket >> playerGuid;

    if (_valid != 0)
    {
        //sLog->outInterRealm("[INTERREALM] Player registration was failed (%u)(loguid %u).", _valid, GUID_LOPART(playerGuid));
        if (Player* pPlayer = ObjectAccessor::FindPlayer(playerGuid))
        {
            switch (_valid)
            {
                case 1:
                    pPlayer->GetSession()->SendNotification(LANG_INTERREALM_PLAYER_FAILED);
                    break;
                case 2:
                    pPlayer->GetSession()->SendNotification(LANG_INTERREALM_PLAYER_LOAD_FAILED);
                    break;
                case 3:
                    if (Group* grp = pPlayer->GetGroup())
                    {
                        for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
                            if (Player* pMember = itr->getSource())
                                pMember->GetSession()->SendNotification(LANG_INTERREALM_PLAYER_GROUP_FAILED, pPlayer->GetName());
                    }
                    else
                        pPlayer->GetSession()->SendNotification(LANG_INTERREALM_PLAYER_GROUP_FAILED, pPlayer->GetName());
                    break;
                default:
                    break;
            }
        }
    }
}

void InterRealmSession::Handle_RegisterRatedResp(WorldPacket& recvPacket)
{

}

void InterRealmSession::Handle_RegisterSpectatorResp(WorldPacket& packet)
{
    uint64 playerGuid, targetGuid;
    uint32 bgZoneId;
    uint8 answer;

    packet >> playerGuid;
    packet >> targetGuid;
    packet >> answer;
    packet >> bgZoneId;

    // ASSERT(bgZoneId);

    Player* pPlayer = sObjectAccessor->FindPlayer(playerGuid);
    if (!pPlayer)
        return;

    if (answer == IR_REG_SPEC_CANNOT_FIND)
    {
        ChatHandler(pPlayer).PSendSysMessage("Target was not found in arena.");
        return;
    }
    else if (answer != IR_REG_OK)
    {
        ChatHandler(pPlayer).PSendSysMessage("Cannot register a spectator.");
        return;
    }

    if (!bgZoneId)
    {
        ChatHandler(pPlayer).PSendSysMessage("Cannot register a spectator because of wrong battleground zone.");
        return;
    }

    pPlayer->SetBattlegroundEntryPoint();

    pPlayer->TradeCancel(false);
    pPlayer->DuelComplete(DUEL_INTERRUPTED);

    pPlayer->SaveToDB();

    if (pPlayer->GetMap())
        pPlayer->GetMap()->RemovePlayerFromMap(pPlayer, false);     

    pPlayer->GetSession()->SetInterRealmBG(bgZoneId);

    WorldPacket pckt(IR_CMSG_SPECTATOR_PORT, 8 + 8);
    pckt << uint64(playerGuid);
    pckt << uint64(targetGuid);
    SendPacket(&pckt);
}

void InterRealmSession::Handle_UnRegisterPlayerResp(WorldPacket& recvPacket)
{
    //sLog->outInterRealm("[INTERREALM] Received a packet IR_SMSG_UNREGISTER_PLAYER_RESP");
    
    uint8 reason;
    uint64 playerGuid;

    recvPacket >> reason; // = 2 if cannot load player
    recvPacket >> playerGuid;

    if (Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        pPlayer->GetSession()->SetInterRealmBG(0);
        pPlayer->SetForceCleanupChannels(true);
    }
}

void InterRealmSession::Handle_BattlefieldPortResp(WorldPacket& packet)
{
    //sLog->outInterRealm("[INTERREALM] Received a packet IR_SMSG_BATTLEFIELD_PORT_RESP");

    uint8 _valid;
    uint64 _playerGuid;
    uint32 _instanceId, bgZoneId;
    uint16 _bgTypeId;

    packet >> _valid;
    packet >> _playerGuid;
    packet >> _instanceId;
    packet >> _bgTypeId;
    packet >> bgZoneId;

    ASSERT(bgZoneId);

    if (Player* pPlayer = sObjectAccessor->FindPlayer(_playerGuid))
    {
        pPlayer->TradeCancel(false);
        pPlayer->DuelComplete(DUEL_INTERRUPTED);

        if (pPlayer->GetMap())
            pPlayer->GetMap()->RemovePlayerFromMap(pPlayer, false);     

        pPlayer->GetSession()->SetInterRealmBG(bgZoneId);

        WorldPacket pckt(IR_CMSG_BATTLEFIELD_ENTER, 8 + 4 + 4 + 2);
        pckt << uint64(_playerGuid);
        pckt << uint32(pPlayer->GetGUIDLow());
        pckt << uint32(_instanceId);
        pckt << uint16(_bgTypeId);
        SendPacket(&pckt);
    }
}

void InterRealmSession::SendPlayerTeleport(Player *player, uint32 zoneId, Player *target, bool isSpectator /* = false */)
{
    player->SetBattlegroundEntryPoint();

    WorldPacket data(IR_CMSG_TELEPORT_PLAYER, 8 + 8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 + NUM_ACCOUNT_DATA_TYPES * 1
                     + 10 + strlen(player->GetName()) + 1 + 1 + 1 + 4 + 4 + 1 + 4 + 1 + 8);

    data << uint64(player->GetGUID());
    data << uint64(target->GetGUID());
    data << uint32(player->GetSession()->GetAccountId());
    data << uint8(player->GetSession()->GetSecurity());
    data << uint8(player->GetSession()->IsPremium());
    data << uint8(player->GetSession()->Expansion());
    data << uint64(player->GetSession()->m_muteTime);
    data << uint8(player->GetSession()->GetSessionDbLocaleIndex());
    data << uint8(player->GetSession()->GetRecruiterId());
    data << uint8(isSpectator ? 1 : 0);

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        AccountData* pData = player->GetSession()->GetAccountData(AccountDataType(i));

        data << uint32(pData->Data.length());
        data.WriteString(pData->Data);
        data << uint64(pData->Time);
    }

    data << std::string(player->GetName());

    data << uint8(player->getLevel()); // Level
    data << uint8(player->getRace()); // Race
    data << uint8(player->getClass()); // Class
    data << uint32(player->GetUInt32Value(PLAYER_FIELD_BYTES)); // Skin, Face, Hairstyle, Haircolor
    data << uint32(player->GetUInt32Value(PLAYER_BYTES_2)); // FacialHair
    data << uint8(player->GetByteValue(PLAYER_BYTES_3, 0)); // Gender
    data << uint32(player->GetGuildId());
    data << uint8(player->GetRandomWinner() ? 1 : 0); // Has random winner

    player->TradeCancel(false);
    player->DuelComplete(DUEL_INTERRUPTED);

    if (player->GetMap())
        player->GetMap()->RemovePlayerFromMap(player, false);

    player->GetSession()->SetInterRealmBG(zoneId);
    SendPacket(&data);
}

void InterRealmSession::Handle_BattlefieldLeave(WorldPacket& packet)
{
    //sLog->outInterRealm(LOG_FILTER_GENERAL, "[INTERREALM] Received a packet IR_SMSG_BATTLEFIELD_LEAVE");

    uint64 playerGuid;
    uint8 desertir;

    packet >> playerGuid;
    packet >> desertir;

    if (Player* pPlayer = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        pPlayer->SetForceCleanupChannels(true);
        pPlayer->GetSession()->SetInterRealmBG(0);
        pPlayer->GetSession()->LoadAfterInterRealm();

        pPlayer->ScheduleDelayedOperation(DELAYED_BG_MOUNT_RESTORE);
        pPlayer->ScheduleDelayedOperation(DELAYED_BG_TAXI_RESTORE);
        pPlayer->ScheduleDelayedOperation(DELAYED_BG_GROUP_RESTORE);
        pPlayer->ScheduleDelayedOperation(DELAYED_BG_UPDATE_ZONE);
        pPlayer->ScheduleDelayedOperation(DELAYED_BG_NAME_RESTORE);
        if (desertir)
            pPlayer->ScheduleDelayedOperation(DELAYED_SPELL_CAST_DESERTER);
        if (pPlayer->isSpectator())
            pPlayer->ScheduleDelayedOperation(DELAYED_SPECTATOR_REMOVE);

        pPlayer->TeleportToBGEntryPoint(true);

        pPlayer->SetInterRealmZoneId(0);
        pPlayer->SetInterRealmAreaId(0);
        pPlayer->SetInterRealmMapId(0);
    }
}

void InterRealmSession::Handle_BGQueueAnnouncer(WorldPacket& packet)
{
    uint8 toPlayer;
    std::string bgName;
    uint32 q_min_level, q_max_level;
    uint32 qAlliance, qAllianceDiff, qHorde, qHordeDiff;

    packet >> bgName;
    packet >> q_min_level >> q_max_level;
    packet >> qAlliance >> qAllianceDiff;
    packet >> qHorde >> qHordeDiff;
    packet >> toPlayer;
    
    
    if (toPlayer == 1)
    {
        uint64 playerGuid;
        packet >> playerGuid;

        if (Player* pPlayer = sObjectAccessor->FindPlayer(playerGuid))
            ChatHandler(pPlayer).PSendSysMessage(LANG_BG_QUEUE_ANNOUNCE_SELF, bgName.c_str(), q_min_level, q_max_level,
                qAlliance, qAllianceDiff, qHorde, qHordeDiff);      
    }
    else
        sWorld->SendWorldText(LANG_BG_QUEUE_ANNOUNCE_WORLD, bgName.c_str(), q_min_level, q_max_level,
            qAlliance, qAllianceDiff, qHorde, qHordeDiff);
                
}

void InterRealmSession::Handle_ArenaQueueAnnouncer(WorldPacket& packet)
{
    uint32 arenaType, teamRating;

    packet >> arenaType >> teamRating;

    sWorld->SendWorldText(LANG_ARENA_QUEUE_ANNOUNCE_WORLD_JOIN, arenaType, arenaType, teamRating);
}

void InterRealmSession::Handle_BattlegroundStartAnnoucer(WorldPacket& packet)
{
    std::string name;
    uint32 minLevel, maxLevel;

    packet >> name >> minLevel >> maxLevel;

    sWorld->SendWorldText(LANG_BG_STARTED_ANNOUNCE_WORLD, name.c_str(), minLevel, maxLevel);
}

void InterRealmSession::Handle_Whisper(WorldPacket& packet)
{
    uint64 senderGUID;
    std::string message, targetName;
    uint32 language;

    packet >> senderGUID >> language >> targetName >> message;

    bool sent = false;
    if (Player *receiver = ObjectAccessor::FindPlayerByNameInOrOutOfWorld(targetName.c_str()))
    {
        if (Player *sender = ObjectAccessor::FindPlayerInOrOutOfWorld(senderGUID))
        {
            if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT))
            {
                if (receiver->GetTeam() != sender->GetTeam())
                {
                    WorldPacket data(SMSG_CHAT_WRONG_FACTION, 0);
                    SendTunneledPacketToClient(senderGUID, &data);
                    return;
                }
            }

            sent = true;
            sender->Whisper(message, language, receiver);
        }
    }

    if (!sent)
    {
        WorldPacket data(SMSG_CHAT_PLAYER_NOT_FOUND, targetName.size() + 1);
        data << targetName;
        SendTunneledPacketToClient(senderGUID, &data);
    }
}

void InterRealmSession::Handle_GuildChat(WorldPacket& packet)
{
    uint64 playerGuid;
    uint32 lang;
    std::string msg;

    packet >> playerGuid;
    packet >> lang;
    packet >> msg;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        if (player->GetGuildId())
        {
            if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
            {
                guild->BroadcastToGuild(player->GetSession(), false, msg, lang == LANG_ADDON ? LANG_ADDON : LANG_UNIVERSAL);
            }
        }
    }
            
}

void InterRealmSession::Handle_PlayerZoneID(WorldPacket& recvPacket)
{
    uint64 playerGUID;
    uint32 zoneId, areaId, mapId;

    recvPacket >> playerGUID;
    recvPacket >> zoneId;
    recvPacket >> areaId;
    recvPacket >> mapId;

    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(playerGUID))
    {
        player->SetInterRealmZoneId(zoneId);
        player->SetInterRealmAreaId(areaId);
        player->SetInterRealmMapId(mapId);
    }
}

void InterRealmSession::Handle_PlayerPInfo(WorldPacket& recvPacket)
{
    std::string playerName;
    uint64 guid;
    recvPacket >> guid >> playerName;

    Player *receiver = ObjectAccessor::FindPlayerInOrOutOfWorld(guid);
    if (!receiver)
        return;

    Player *pinfoPlayer = ObjectAccessor::FindPlayerByNameInOrOutOfWorld(playerName.c_str());
    if (!pinfoPlayer)
    {
        SendPSysMessage(receiver, "Player not found");
        return;
    }

    /*if (!Player::HandlePinfoCommand(receiver, pinfoPlayer, pinfoPlayer->GetGUID(), playerName))
    {
        SendPSysMessage(receiver, "Player not found");
        return;
    }*/
}

//void InterRealmSession::Handle_Ban(WorldPacket &recvPacket)
//{
//    // @todo need better way to implement this shit
//    uint64 guid;
//    uint8 banMode;
//    std::string nameOrIP, duration, reason, gmName;
//
//    recvPacket >> guid >> banMode >> nameOrIP >> duration >> reason >> gmName;
//
//    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(guid))
//    {
//        ChatHandler handler(player);
//        //AccountMgr::BanHelper(banMode, nameOrIP, duration, reason, gmName, &handler, player->GetSession());
//    }
//}

//void InterRealmSession::Handle_Unban(WorldPacket& recvPacket)
//{
//    uint64 guid;
//    uint8 banMode;
//    std::string nameOrIP, gmName;
//
//    recvPacket >> guid >> banMode >> nameOrIP >> gmName;
//
//    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(guid))
//    {
//        ChatHandler handler(player);
//        //AccountMgr::UnbanHelper(banMode, nameOrIP, &handler, player->GetSession());
//    }
//}
//
//void InterRealmSession::Handle_UnbanCharacter(WorldPacket& recvPacket)
//{
//    uint64 guid;
//    std::string name, gmName;
//
//    recvPacket >> guid >> name >> gmName;
//
//    if (Player *player = ObjectAccessor::FindPlayerInOrOutOfWorld(guid))
//    {
//        ChatHandler handler(player);
//        //AccountMgr::UnbanCharacter(name, &handler, player->GetSession());
//    }   
//}

void InterRealmSession::Handle_KickPlayer(WorldPacket& recvPacket)
{
    uint64 gmGUID;
    std::string name;
    recvPacket >> gmGUID >> name;

    // @todo need better way to implement this shit
    Player *target = ObjectAccessor::FindPlayerByNameInOrOutOfWorld(name.c_str());
    Player *gm = ObjectAccessor::FindPlayerInOrOutOfWorld(gmGUID);

    if (!target || !gm)
    {
        SendPSysMessage(gm, "Player or gm not found. If you are gm u're probably very weird person >_>");
        return;
    }

    ChatHandler handler = ChatHandler(gm);

    if (target == gm)
    {
        SendPSysMessage(gm, handler.GetTrinityString(LANG_COMMAND_KICKSELF));
        return;
    }

    // check online security
    if (handler.HasLowerSecurity(target, 0))
        return;

    if (sWorld->getBoolConfig(CONFIG_SHOW_KICK_IN_WORLD))
    {
        SendServerAnnounce(gm->GetGUID(), handler.GetTrinityString(LANG_COMMAND_KICKMESSAGE));
        sWorld->SendWorldText(LANG_COMMAND_KICKMESSAGE, target->GetName());
    }
    else
    {
        SendPSysMessage(gm, handler.GetTrinityString(LANG_COMMAND_KICKMESSAGE), target->GetName());
    }

    target->GetSession()->KickPlayer();
}

void InterRealmSession::Handle_BattlegroundHolidayResp(WorldPacket& recvPacket)
{
    uint8 num, state;
    uint32 bgTypeId;

    recvPacket >> num;

    if (num > 0)
    {
        BattlegroundHolidayContainer& holidays = sBattlegroundMgr->GetBattlegroundHolidays();

        holidays.clear();

        for (uint8 i = 0; i < num; ++i)
        {
            recvPacket >> bgTypeId;
            recvPacket >> state;

            holidays[(BattlegroundTypeId)bgTypeId] = (bool)state;
        }
    }
}

void InterRealmSession::Handle_SummonPlayer(WorldPacket& recvPacket)
{
    uint64 senderGUID;
    std::string playerName;

    recvPacket >> senderGUID >> playerName;

    if (auto teleportTarget = ObjectAccessor::FindPlayerInOrOutOfWorld(senderGUID))
    {
        if (auto player = ObjectAccessor::FindPlayerByNameInOrOutOfWorld(playerName.c_str()))
            SendPlayerTeleport(player, teleportTarget->GetInterRealmZoneId(), teleportTarget);
        else
            SendPSysMessage(teleportTarget, "Player not found");
    }
}

void InterRealmSession::Handle_SpectatorData(WorldPacket& recvPacket)
{
    uint16 teamsCount;
    uint64 portplayerGUID;
    std::string specData;

    recvPacket >> teamsCount;

    sBattlegroundMgr->ClearSpectatorData();

    if (teamsCount > 0)
    {
        for (uint16 i = 0; i < teamsCount; ++i)
        {
            recvPacket >> portplayerGUID >> specData;
            sBattlegroundMgr->AddSpectatorData(portplayerGUID, specData);
        }
    }
}

void InterRealmSession::Handle_DistributeArenaPointsResp(WorldPacket& recvPacket)
{
    /*sLog->outError(LOG_FILTER_INTERREALM, "Received IR_SMSG_DISTRIBUTE_ARENA_POINTS_RESP.");

    uint32 count;
    
    recvPacket >> count;

    if (count > 0)
    {
        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_START);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_ONLINE_START);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        uint32 realCount = 0;

        for (uint32 i = 0; i < count; ++i)
        {
            uint64 guid;
            uint32 arenaPoints;

            recvPacket >> guid >> arenaPoints;

            if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(guid))
            {
                // skip
                if (player->GetSession()->GetInterRealmBG())
                {
                    continue;
                }

                player->ModifyArenaPoints(arenaPoints, &trans);

                realCount++;
            }
            else
            {
                trans->PAppend("UPDATE characters SET arenaPoints=arenaPoints+%u WHERE guid=%u", arenaPoints, GUID_LOPART(guid));
            
                realCount++;
            }
        }

        CharacterDatabase.CommitTransaction(trans);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_ONLINE_END);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_TEAM_START);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_TEAM_END);

        sWorld->SendWorldText(LANG_DIST_ARENA_POINTS_END);
    }*/
}

void InterRealmSession::Handle_AchievementReward(WorldPacket& recvPacket)
{
    uint64 playerGuid;
    uint32 achievement;

    recvPacket >> playerGuid;
    recvPacket >> achievement;

    Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid);
    if (!player)
        return;

    AchievementEntry const* achievementEntry = sAchievementStore.LookupEntry(achievement);
    if (!achievementEntry)
        return;

    AchievementReward const* reward = sAchievementMgr->GetAchievementReward(achievementEntry);
    if (!reward)
        return;

    if (reward->sender)
    {
        Item* item = reward->itemId ? Item::CreateItem(reward->itemId, 1, player) : NULL;

        int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

        // subject and text
        std::string subject = reward->subject;
        std::string text = reward->text;
        if (loc_idx >= 0)
        {
            if (AchievementRewardLocale const* loc = sAchievementMgr->GetAchievementRewardLocale(achievementEntry))
            {
                ObjectMgr::GetLocaleString(loc->subject, loc_idx, subject);
                ObjectMgr::GetLocaleString(loc->text, loc_idx, text);
            }
        }

        MailDraft draft(subject, text);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        if (item)
        {
            // save new item before send
            item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted

            // item
            draft.AddItem(item);
        }

        draft.SendMailTo(trans, player, MailSender(MAIL_CREATURE, reward->sender));
        CharacterDatabase.CommitTransaction(trans);
    }
}

void InterRealmSession::Handle_AnticheatReport(WorldPacket& recvPacket)
{
    uint32 guidLow;
    uint8 reportType;

    recvPacket >> guidLow >> reportType;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(MAKE_NEW_GUID(guidLow, 0, HighGuid::Player)))
    {
        sAnticheatMgr->BuildReport(player, reportType);
    }
}

void InterRealmSession::Handle_AnticheatReportNotification(WorldPacket& recvPacket)
{
    uint64 playerGuid = 0; // do not use it to get player by guid
    uint32 realmNumber = 0;
    std::string message;

    recvPacket >> playerGuid;
    recvPacket >> realmNumber;
    recvPacket >> message;

    WorldPacket data(SMSG_NOTIFICATION, (message.size()+1));
    data << message;
    sWorld->SendGlobalGMMessage(&data);

    sWorld->SendGMText(message.c_str());
}

void InterRealmSession::Handle_AppearRequestResp(WorldPacket& recvPacket)
{
    uint64 playerGuid, targetGuid;
    uint32 bgZoneId;
    uint8 answer;

    recvPacket >> playerGuid;
    recvPacket >> targetGuid;
    recvPacket >> answer;
    recvPacket >> bgZoneId;

    ASSERT(bgZoneId);

    Player* pPlayer = sObjectAccessor->FindPlayer(playerGuid);
    if (!pPlayer)
        return;

    pPlayer->SetBattlegroundEntryPoint();

    pPlayer->TradeCancel(false);
    pPlayer->DuelComplete(DUEL_INTERRUPTED);

    pPlayer->SaveToDB();

    if (pPlayer->GetMap())
        pPlayer->GetMap()->RemovePlayerFromMap(pPlayer, false);     

    pPlayer->GetSession()->SetInterRealmBG(bgZoneId);

    WorldPacket pckt(IR_CMSG_APPEAR, 8 + 8);
    pckt << uint64(playerGuid);
    pckt << uint64(targetGuid);
    SendPacket(&pckt);
}

void InterRealmSession::Handle_AdditionalInfo(WorldPacket& recvPacket)
{
    uint64 playerGuid;
    recvPacket >> playerGuid;

    Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid);
    if (!player)
        return;
    

    WorldPacket packet(IR_CMSG_ADDITIONAL_INFO, 20);
    packet << playerGuid;
    packet << uint8(1); // count ?
    packet << std::string(player->GetSession()->GetRemoteAddress());
    SendPacket(&packet);
}

void InterRealmSession::Handle_GuildQuery(WorldPacket& recvPacket)
{
    sLog->outInfo(LOG_FILTER_INTERREALM, "Received guild query for new guilds.");

    uint64 guildGuid = 0;

    recvPacket >> guildGuid;

    SendGuild(guildGuid);
}

void InterRealmSession::Handle_SyncPlayerCurrency(WorldPacket& packet)
{
    uint64 playerGuid = 0;

    packet >> playerGuid;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {

        uint32 count = 0;
        packet >> count;
        if (count > 0)
        {
            for (uint32 i = 0; i < count; ++i)
            {
                uint32 entry;
                uint32 weekCount;
                uint32 totalCount;
                uint32 seasonTotal;
                uint8 flags;
                uint32 weekCap;
                uint8 needResetCap;

                packet >> entry;
                packet >> weekCount >> totalCount >> seasonTotal;
                packet >> flags >> weekCap >> needResetCap;

                PlayerCurrenciesMap& storage = player->GetCurrencyStorage();
                bool isNew = (storage.find(entry) == storage.end());
                PlayerCurrency& currency = storage[entry];
                currency.weekCount = weekCount;
                currency.totalCount = totalCount;
                currency.seasonTotal = seasonTotal;
                currency.flags = flags;
                currency.weekCap = weekCap;
                currency.needResetCap = (needResetCap != 0);
                currency.state = isNew ? PLAYERCURRENCY_NEW : PLAYERCURRENCY_CHANGED;
            }

            player->SaveCurrency();
        }
    }
}

void InterRealmSession::Handle_SyncPlayerAchievements(WorldPacket& packet)
{
    uint64 playerGuid = 0;

    packet >> playerGuid;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        ACE_Thread_Mutex& playerAchievementsLock = player->GetAchievementMgr().GetCompletedAchievementLock();
        CompletedAchievementMap& playerAchievements = player->GetAchievementMgr().GetCompletedAchievements();
        CriteriaProgressMap& playerCriterias = player->GetAchievementMgr().GetCriteriaProgress();

        uint32 achievementsCount = 0;
        uint32 criteriasCount = 0;

        packet >> achievementsCount;
        packet >> criteriasCount;

        if (achievementsCount > 0)
        {
            playerAchievementsLock.acquire();

            for (uint32 i = 0; i < achievementsCount; ++i)
            {
                uint32 achievementId = 0;
                uint64 date = 0;
                uint64 firstGuid = 0;
                uint8 completedByThisCharacter = 0;
                uint32 guidsCount = 0;

                packet >> achievementId >> date >> firstGuid >> completedByThisCharacter;
                packet >> guidsCount;

                CompletedAchievementData& ca = playerAchievements[achievementId];
                ca.date = date;
                ca.first_guid = firstGuid;
                ca.completedByThisCharacter = completedByThisCharacter ? true : false;

                if (guidsCount > 0)
                {
                    for (uint32 i = 0; i < guidsCount; ++i)
                    {
                        uint64 guid = 0;
                        packet >> guid;

                        ca.guids.insert(guid);
                    }
                }

                ca.changed = true;
            }

            playerAchievementsLock.release();
        }

        if (criteriasCount > 0)
        {
            for (uint32 i = 0; i < criteriasCount; ++i)
            {
                uint32 criteriaId = 0;
                uint32 criteraCounter = 0;
                uint64 date = 0;
                uint64 completedGuid = 0;

                packet >> criteriaId >> criteraCounter >> date >> completedGuid;

                CriteriaProgress& progress = playerCriterias[criteriaId];
                progress.counter = criteraCounter;
                progress.date    = date;
                progress.CompletedGUID = completedGuid;
                progress.changed = true;
            }
        }

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        player->GetAchievementMgr().SaveToDB(trans);
        CharacterDatabase.CommitTransaction(trans);
    }
}

void InterRealmSession::Handle_SyncPlayerReputation(WorldPacket& packet)
{
    uint64 playerGuid = 0;

    packet >> playerGuid;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        FactionStateList& factions = player->GetReputationMgr().GetStateList();

        uint32 reputationsCount = 0;

        packet >> reputationsCount;

        if (reputationsCount > 0)
        {
            for (uint32 i = 0; i < reputationsCount; ++i)
            {
                uint32 reputationId = 0;
                uint32 reputationListId = 0;
                int32 reputationStanding = 0;
                uint8 reputationFlags = 0;

                packet >> reputationListId >> reputationId >> reputationStanding >> reputationFlags;

                FactionState& faction = factions[reputationListId];
                faction.ID = reputationId;
                faction.ReputationListID = reputationListId;
                faction.Standing = reputationStanding;
                faction.Flags = reputationFlags;
                faction.needSave = true;
            }
        }

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        player->GetReputationMgr().SaveToDB(trans);
        CharacterDatabase.CommitTransaction(trans);
    }
}

void InterRealmSession::Handle_SyncPlayerKills(WorldPacket& packet)
{
    uint64 playerGuid = 0;

    packet >> playerGuid;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        uint32 kills = player->GetUInt32Value(PLAYER_FIELD_KILLS);
        uint32 lifetimeKills = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);

        packet >> kills;
        packet >> lifetimeKills;
        
        player->SetUInt32Value(PLAYER_FIELD_KILLS, kills);
        player->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, lifetimeKills);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        player->SaveKillsToDB(trans);
        CharacterDatabase.CommitTransaction(trans);
    }
}

void InterRealmSession::Handle_SyncPlayerQuests(WorldPacket& packet)
{
    uint64 playerGuid = 0;

    packet >> playerGuid;

    if (Player* player = sObjectAccessor->FindPlayerInOrOutOfWorld(playerGuid))
    {
        uint32 statusCount = 0;
        uint32 rewardedCount = 0;

        packet >> statusCount >> rewardedCount;

        QuestStatusSaveMap& questStatusSaves = player->getQuestStatusSaveMap();
        QuestStatusSaveMap& rewardedQuestSaves = player->getRewardedQuestsSave();

        if (statusCount > 0)
        {
            for (uint32 i = 0; i < statusCount; ++i)
            {
                uint32 questId;

                packet >> questId;

                Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
                if (!quest)
                    continue;

                QuestStatusMap& questStatusMap = player->getQuestStatusMap();

                // TODO: enable new quests from interrealm
                if (questStatusMap.find(questId) == questStatusMap.end())
                    continue;

                uint16 log_slot = player->FindQuestSlot(questId);
                if (log_slot >= MAX_QUEST_LOG_SIZE)
                    continue;

                QuestStatusData& questStatusData = questStatusMap[questId];

                uint32 questTimer;
                uint8 questStatus, isExplored;

                packet >> questStatus >> isExplored;
                packet >> questTimer;

                if (questStatus < MAX_QUEST_STATUS)
                    questStatusData.Status = QuestStatus(questStatus);
                else
                    questStatusData.Status = QUEST_STATUS_INCOMPLETE;

                questStatusData.Explored = (isExplored > 0);

                time_t quest_time = time_t(questTimer);

                if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED) && !player->GetQuestRewardStatus(questId))
                {
                    player->AddTimedQuest(questId);

                    if (quest_time <= sWorld->GetGameTime())
                        questStatusData.Timer = 1;
                    else
                        questStatusData.Timer = uint32((quest_time - sWorld->GetGameTime()) * IN_MILLISECONDS);
                }
                else
                    quest_time = 0;

                for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; i++)
                {
                    uint16 count = 0;
                    packet >> count;
                    questStatusData.CreatureOrGOCount[i] = count;

                    if (count > 0)
                        player->SetQuestSlotCounter(log_slot, i, count);
                }

                for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; i++)
                {
                    uint16 count = 0;
                    packet >> count;
                    questStatusData.ItemCount[i] = count;
                }

                uint16 playerCount = 0;
                packet >> playerCount;
                questStatusData.PlayerCount = playerCount;    

                if (playerCount > 0)
                    player->SetQuestSlotCounter(log_slot, QUEST_PVP_KILL_SLOT, playerCount);


                questStatusSaves[questId] = true;
            }
        }

        if (rewardedCount > 0)
        {
            /*for (uint32 i = 0; i < rewardedCount; ++i)
            {
                uint32 questId;

                packet >> questId;

                Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
                if (!quest)
                    continue;

                player->getRewardedQuests().insert(questId);

                rewardedQuestSaves[questId] = true;
            }*/
        }

        if (statusCount > 0 || rewardedCount > 0)
        {
            SQLTransaction trans = CharacterDatabase.BeginTransaction();
            player->SaveKillsToDB(trans);
            CharacterDatabase.CommitTransaction(trans);
        }
    }
}


void InterRealmSession::SendWhisper(uint64 sender, uint64 receiver, const std::string& text, uint32 language)
{
    /*Player* pSender = ObjectAccessor::FindPlayer(sender);
    Player* pReceiver = ObjectAccessor::FindPlayerInOrOutOfWorld(receiver);

    bool isAddonMessage = language == LANG_ADDON;

    if (!isAddonMessage) // if not addon data
        language = LANG_UNIVERSAL; // whispers should always be readable

    std::string _text(text);

    if (!pReceiver->isAcceptWhispers() && !pSender->isGameMaster())
    {
        ChatHandler(pSender->GetSession()).SendSysMessage(LANG_COMMAND_WHISPEROFF);
        return;
    }

    WorldPacket pckt(IR_MSG_WHISPER);
    pckt << uint64(sender);
    pckt << uint64(receiver);
    pckt << uint32(language);
    pckt << uint8(pSender->GetChatTag());
    pckt << text;
    SendPacket(&pckt); 

    // rest stuff shouldn't happen in case of addon message
    if (isAddonMessage)
        return;

    pckt.Initialize(SMSG_MESSAGECHAT, 200);
    //pReceiver->BuildPlayerChat(&pckt, CHAT_MSG_WHISPER_INFORM, _text, language);

	pckt << uint8(CHAT_MSG_WHISPER_INFORM);
    pckt << uint32(language);
    pckt << uint64(pReceiver->GetGUID());
    pckt << uint32(0);                                      // constant unknown time
    pckt << uint64(pReceiver->GetGUID());
    pckt << uint32(_text.length() + 1);
    pckt << _text;
    pckt << uint8(pReceiver->GetChatTag());

    pSender->GetSession()->SendPacket(&pckt);

    if (!pSender->isAcceptWhispers() && !pSender->isGameMaster() && !pReceiver->isGameMaster())
    {
        pSender->SetAcceptWhispers(true);
        ChatHandler(pSender->GetSession()).SendSysMessage(LANG_COMMAND_WHISPERON);
    }*/
}

void InterRealmSession::SendBattlegroundHolidaysQuery()
{
    WorldPacket packet(IR_CMSG_BATTLEGROUND_HOLIDAY, 0);
    SendPacket(&packet);
}

void InterRealmSession::BuildPlayerArenaInfoBlock(Player* player, ByteBuffer& packet)
{
    for (uint8 i = 0; i < MAX_PVP_SLOT; ++i)
    {
        packet << player->GetArenaPersonalRating(i);
        packet << player->GetBestRatingOfWeek(i);
        packet << player->GetBestRatingOfSeason(i);
        packet << player->GetArenaMatchMakerRating(i);
        packet << player->GetWeekGames(i);
        packet << player->GetWeekWins(i);
        packet << player->GetPrevWeekWins(i);
        packet << player->GetSeasonGames(i);
        packet << player->GetSeasonWins(i);
    }
}

void InterRealmSession::SendRegisterPlayer(Player* player, uint32 bgInstanceId, uint64 bgGuid, uint32 bgTypeId)
{
    WorldPacket pckt(IR_CMSG_REGISTER_PLAYER, 8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 + NUM_ACCOUNT_DATA_TYPES * 1
                         + 10 + strlen(player->GetName()) + 1 + 1 + 1 + 4 + 4 + 1 + 1 + 4 + 8);

    pckt << uint64(player->GetGUID());
    pckt << uint32(player->GetSession()->GetAccountId());
    pckt << uint8(player->GetSession()->GetSecurity());
    pckt << uint8(player->GetSession()->IsPremium());
    pckt << uint8(player->GetSession()->Expansion());
    pckt << uint64(player->GetSession()->m_muteTime);
    pckt << uint8(player->GetSession()->GetSessionDbLocaleIndex());
    pckt << uint8(player->GetSession()->GetRecruiterId());

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        AccountData* pData = player->GetSession()->GetAccountData(AccountDataType(i));

        pckt << uint32(pData->Data.length());
        pckt.WriteString(pData->Data);
        pckt << uint64(pData->Time);
    }

    pckt << std::string(player->GetName());

    pckt << uint8(player->getLevel()); // Level
    pckt << uint8(player->getRace()); // Race
    pckt << uint8(player->getClass()); // Class
    pckt << uint32(player->GetUInt32Value(PLAYER_FIELD_BYTES)); // Skin, Face, Hairstyle, Haircolor
    pckt << uint32(player->GetUInt32Value(PLAYER_BYTES_2)); // FacialHair
    pckt << uint8(player->GetByteValue(PLAYER_BYTES_3, 0)); // Gender
    pckt << uint32(player->GetGuildId());
    pckt << uint8(player->GetRank());
    pckt << uint32(player->GetGuildLevel());
    
    pckt << uint8(player->GetRandomWinner() ? 1 : 0); // Has random winner

    pckt << uint32(bgInstanceId); // bg instance id
    pckt << uint64(bgGuid); // bg guid
    pckt << uint32(bgTypeId);

    BuildPlayerArenaInfoBlock(player, pckt);

    SendPacket(&pckt);
}

void InterRealmSession::SendRegisterGroup(Group* group, uint32 bgInstanceId, uint64 bgGuid, uint32 bgTypeId)
{
    WorldPacket pckt(IR_CMSG_REGISTER_GROUP, (8 + 4 + 8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 +
            10 + 1 + 1 + 1 + 4 + 4 + 1 + 1));

    ByteBuffer data;

    size_t num_pos = pckt.wpos();
    pckt << uint8(0);
    pckt << uint32(bgInstanceId); // bg instance id
    pckt << uint64(bgGuid); // bg guid
    pckt << uint32(bgTypeId);

    uint8 count = 0;
    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member || !member->IsInWorld())
            continue;

        pckt << uint64(member->GetGUID());

        count++;

        uint64 _mute;
        uint32 _accountId;
        uint8 _security, _premium, _exp, _locale, _recruiter;

        _accountId = member->GetSession()->GetAccountId();
        _security = member->GetSession()->GetSecurity();
        _premium = member->GetSession()->IsPremium();
        _exp = member->GetSession()->Expansion();
        _mute = member->GetSession()->m_muteTime;
        _locale = member->GetSession()->GetSessionDbLocaleIndex();
        _recruiter = member->GetSession()->GetRecruiterId();

        data << uint32(_accountId);
        data << uint8(_security);
        data << uint8(_premium);
        data << uint8(_exp);
        data << uint64(_mute);
        data << uint8(_locale);
        data << uint8(_recruiter);

        data << std::string(member->GetName());

        data << uint8(member->getLevel()); // Level
        data << uint8(member->getRace()); // Race
        data << uint8(member->getClass()); // Class
        data << uint32(member->GetUInt32Value(PLAYER_FIELD_BYTES)); // Skin, Face, Hairstyle, Haircolor
        data << uint32(member->GetUInt32Value(PLAYER_BYTES_2)); // FacialHair
        data << uint8(member->GetByteValue(PLAYER_BYTES_3, 0)); // Gender
        data << uint32(member->GetGuildId());
        data << uint8(member->GetRank());
        data << uint32(member->GetGuildLevel());

        data << uint8(member->GetRandomWinner() ? 1 : 0); // Has random winner

        BuildPlayerArenaInfoBlock(member, data);
    }

    pckt.put<uint8>(num_pos, count);
    pckt.append(data);

    SendPacket(&pckt);
}

void InterRealmSession::SendRegisterArena(Group* group, uint32 arenaSlot)
{
    WorldPacket pckt(IR_CMSG_REGISTER_ARENA, 1 + 4 + (8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 +
        10 + 1 + 1 + 1 + 4 + 4 + 1 + 1));
    
    ByteBuffer data;

    size_t num_pos = data.wpos();
    pckt << uint8(0);
    pckt << uint8(arenaSlot);

    uint8 count = 0;
    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member || !member->IsInWorld())
            continue;

        count++;

        pckt << uint64(member->GetGUID());

        uint64 _mute;
        uint32 _accountId;
        uint8 _security, _premium, _exp, _locale, _recruiter;

        _accountId = member->GetSession()->GetAccountId();
        _security = member->GetSession()->GetSecurity();
        _premium = member->GetSession()->IsPremium();
        _exp = member->GetSession()->Expansion();
        _mute = member->GetSession()->m_muteTime;
        _locale = member->GetSession()->GetSessionDbLocaleIndex();
        _recruiter = member->GetSession()->GetRecruiterId();

        //pckt << uint64(member->GetGUID());
        data << uint32(_accountId);
        data << uint8(_security);
        data << uint8(_premium);
        data << uint8(_exp);
        data << uint64(_mute);
        data << uint8(_locale);
        data << uint8(_recruiter);

        data << std::string(member->GetName());

        data << uint8(member->getLevel()); // Level
        data << uint8(member->getRace()); // Race
        data << uint8(member->getClass()); // Class
        data << uint32(member->GetUInt32Value(PLAYER_FIELD_BYTES)); // Skin, Face, Hairstyle, Haircolor
        data << uint32(member->GetUInt32Value(PLAYER_BYTES_2)); // FacialHair
        data << uint8(member->GetByteValue(PLAYER_BYTES_3, 0)); // Gender
        data << uint32(member->GetGuildId());
        data << uint8(member->GetRank());
        data << uint32(member->GetGuildLevel());

        data << uint8(member->GetRandomWinner() ? 1 : 0); // Has random winner

        BuildPlayerArenaInfoBlock(member, data);
    }

    pckt.put<uint8>(num_pos, count);
    pckt.append(data);

    SendPacket(&pckt);
}

void InterRealmSession::SendRegisterRated(Group* group, uint32 personalRating, uint32 matchmakerRating)
{
    WorldPacket pckt(IR_CMSG_REGISTER_RATED, 1 + 4 * (8 + 4 + 1 + 1 + 1 + 8 + 1 + 1 +
        10 + 1 + 1 + 1 + 4 + 4 + 1 + 1));

    ByteBuffer data;

    size_t num_pos = pckt.wpos();
    pckt << uint8(0);
    pckt << uint32(personalRating);
    pckt << uint32(matchmakerRating);

    uint8 count = 0;
    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member || !member->IsInWorld())
            continue;

        pckt << uint64(member->GetGUID());

        count++;

        uint64 _mute;
        uint32 _accountId;
        uint8 _security, _premium, _exp, _locale, _recruiter;

        _accountId = member->GetSession()->GetAccountId();
        _security = member->GetSession()->GetSecurity();
        _premium = member->GetSession()->IsPremium();
        _exp = member->GetSession()->Expansion();
        _mute = member->GetSession()->m_muteTime;
        _locale = member->GetSession()->GetSessionDbLocaleIndex();
        _recruiter = member->GetSession()->GetRecruiterId();

        data << uint32(_accountId);
        data << uint8(_security);
        data << uint8(_premium);
        data << uint8(_exp);
        data << uint64(_mute);
        data << uint8(_locale);
        data << uint8(_recruiter);

        data << std::string(member->GetName());

        data << uint8(member->getLevel()); // Level
        data << uint8(member->getRace()); // Race
        data << uint8(member->getClass()); // Class
        data << uint32(member->GetUInt32Value(PLAYER_FIELD_BYTES)); // Skin, Face, Hairstyle, Haircolor
        data << uint32(member->GetUInt32Value(PLAYER_BYTES_2)); // FacialHair
        data << uint8(member->GetByteValue(PLAYER_BYTES_3, 0)); // Gender
        data << uint32(member->GetGuildId());
        data << uint8(member->GetRank());
        data << uint32(member->GetGuildLevel());

        data << uint8(member->GetRandomWinner() ? 1 : 0); // Has random winner

        BuildPlayerArenaInfoBlock(member, data);
    }

    pckt.put<uint8>(num_pos, count);
    pckt.append(data);

    SendPacket(&pckt);
}

void InterRealmSession::SendRegisterSpectator(Player* player, uint64 targetGuid)
{
    WorldPacket packet(IR_CMSG_REGISTER_SPECTATOR);

    packet << uint64(player->GetGUID());
    packet << uint64(targetGuid);
    packet << uint32(player->GetSession()->GetAccountId());
    packet << uint8(player->GetSession()->GetSecurity());
    packet << uint8(player->GetSession()->IsPremium());
    packet << uint8(player->GetSession()->Expansion());
    packet << uint64(player->GetSession()->m_muteTime);
    packet << uint8(player->GetSession()->GetSessionDbLocaleIndex());
    packet << uint8(player->GetSession()->GetRecruiterId());

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        AccountData* pData = player->GetSession()->GetAccountData(AccountDataType(i));

        packet << uint32(pData->Data.length());
        packet.WriteString(pData->Data);
        packet << uint64(pData->Time);
    }

    packet << std::string(player->GetName());

    packet << uint8(player->getLevel()); // Level
    packet << uint8(player->getRace()); // Race
    packet << uint8(player->getClass()); // Class
    packet << uint32(player->GetUInt32Value(PLAYER_FIELD_BYTES)); // Skin, Face, Hairstyle, Haircolor
    packet << uint32(player->GetUInt32Value(PLAYER_BYTES_2)); // FacialHair
    packet << uint8(player->GetByteValue(PLAYER_BYTES_3, 0)); // Gender
    packet << uint32(player->GetGuildId());
    packet << uint8(player->GetRank());
    packet << uint32(player->GetGuildLevel());
    packet << uint8(player->GetRandomWinner() ? 1 : 0); // Has random winner

    SendPacket(&packet);
}

void InterRealmSession::SendCurrencyResetForPlayer(Player* player)
{
    PlayerCurrenciesMap const& storage = player->GetCurrencyStorage();

    WorldPacket packet(IR_CMSG_CURRENCY_CAP_RESET, 8 + 4 + storage.size() * (4 + 4));

    packet << player->GetGUID();
    packet << uint32(storage.size());

    for (PlayerCurrenciesMap::const_iterator itr = storage.begin(); itr != storage.end(); ++itr)
    {
        packet << uint32(itr->first);
        packet << uint32(itr->second.weekCap);
    }

    SendPacket(&packet);
}

void InterRealmSession::SendSummonPlayer(uint64 playerGuid)
{
    WorldPacket data(IR_CMSG_SUMMON_PLAYER, 8);
    data << playerGuid;
    SendPacket(&data);
}

void InterRealmSession::SendAppearRequest(Player* sender, uint64 targetGuid)
{
    WorldPacket data(IR_CMSG_APPEAR_REQUEST);
    data << sender->GetGUID();
    data << targetGuid;

    data << uint32(sender->GetSession()->GetAccountId());
    data << uint8(sender->GetSession()->GetSecurity());
    data << uint8(sender->GetSession()->IsPremium());
    data << uint8(sender->GetSession()->Expansion());
    data << uint64(sender->GetSession()->m_muteTime);
    data << uint8(sender->GetSession()->GetSessionDbLocaleIndex());
    data << uint8(sender->GetSession()->GetRecruiterId());

    for (uint8 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
    {
        AccountData* pData = sender->GetSession()->GetAccountData(AccountDataType(i));

        data << uint32(pData->Data.length());
        data.WriteString(pData->Data);
        data << uint64(pData->Time);
    }

    data << std::string(sender->GetName());

    data << uint8(sender->getLevel()); // Level
    data << uint8(sender->getRace()); // Race
    data << uint8(sender->getClass()); // Class
    data << uint32(sender->GetUInt32Value(PLAYER_FIELD_BYTES)); // Skin, Face, Hairstyle, Haircolor
    data << uint32(sender->GetUInt32Value(PLAYER_BYTES_2)); // FacialHair
    data << uint8(sender->GetByteValue(PLAYER_BYTES_3, 0)); // Gender
    data << uint32(sender->GetGuildId());
    data << uint8(sender->GetRank());
    data << uint8(sender->GetRandomWinner() ? 1 : 0); // Has random winner

    SendPacket(&data);
}

void InterRealmSession::SendAccountSpells(Player* player)
{
    if (!player)
        return;

    PlayerSpellMap const& spellMap = player->GetSpellMap();

    WorldPacket packet(IR_CMSG_ACCOUNT_SPELLS, 8 + spellMap.size() * 4);

    packet << player->GetGUID();

    uint32 count = 0;
    size_t count_pos = packet.wpos();
    packet << uint32(0);

    for (PlayerSpellMap::const_iterator itr = spellMap.begin(); itr != spellMap.end(); ++itr)
    {
        if (!itr->second)
            continue;

        if (const SpellInfo* spell = sSpellMgr->GetSpellInfo(itr->first))
        {
            packet << uint32(spell->Id);
            packet << uint8(itr->second->active);
            packet << uint8(itr->second->dependent);
            packet << uint8(itr->second->disabled);
            count++;
        }
    }

    packet.put(count_pos, count);

    SendPacket(&packet);
}

void InterRealmSession::SendCharacterSkills(Player* player)
{
    if (!player)
        return;

    SkillStatusMap const& skillMap = player->GetSkillsMap();

    WorldPacket packet(IR_CMSG_CHARACTER_SKILLS, 8 + skillMap.size() * 4);

    packet << player->GetGUID();

    uint32 count = 0;
    size_t count_pos = packet.wpos();
    packet << uint32(0);

    for (SkillStatusMap::const_iterator itr = skillMap.begin(); itr != skillMap.end(); ++itr)
    {
        if (itr->second.uState ==  SKILL_DELETED)
            continue;

        uint32 value = player->GetPureSkillValue(itr->first);
        uint32 maxValue = player->GetPureMaxSkillValue(itr->first);

        packet << uint32(itr->first);
        packet << uint32(value);
        packet << uint32(maxValue);

        count++;
    }

    packet.put(count_pos, count);

    SendPacket(&packet);
}

void InterRealmSession::SendGuild(uint64 guildGuid)
{
    WorldPacket packet(IR_CMSG_GUILD);

    Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid);
    if (!guild)
        return;

    packet << guild->GetGUID();
    packet << guild->GetName();
    packet << guild->GetMembersCount();
    packet << guild->GetLevel();
    packet << guild->GetExperience();
    packet << guild->GetEmblemInfo().GetStyle();
    packet << guild->GetEmblemInfo().GetColor();
    packet << guild->GetEmblemInfo().GetBorderStyle();
    packet << guild->GetEmblemInfo().GetBorderColor();
    packet << guild->GetEmblemInfo().GetBackgroundColor();

    Guild::Ranks const* ranks = guild->GetRanks();

    packet << uint32(ranks->size());

    for (Guild::Ranks::const_iterator itr = ranks->begin(); itr != ranks->end(); ++itr)
    {
        packet << itr->GetId();
        packet << itr->GetName();
    }

    SendPacket(&packet);
}

void InterRealmSession::SendCharacterQuests(Player* player)
{
    if (!player)
        return;

    WorldPacket packet(IR_CMSG_CHARACTER_QUESTS);

    QuestStatusMap& questStatuses = player->getQuestStatusMap();
    const RewardedQuestSet& rewardedQuests = player->getRewardedQuests();

    packet << player->GetGUID();

    packet << uint32(questStatuses.size());
    packet << uint32(rewardedQuests.size());

    for (auto questStatus : questStatuses)
    {
        packet << uint32(questStatus.first);

        packet << uint8(questStatus.second.Status);
        packet << uint8(questStatus.second.Explored);
        packet << uint32(questStatus.second.Timer / IN_MILLISECONDS + sWorld->GetGameTime());

        for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; i++)
            packet << uint16(questStatus.second.CreatureOrGOCount[i]);

        for (uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; i++)
            packet << uint16(questStatus.second.ItemCount[i]);

        packet << uint16(questStatus.second.PlayerCount);
    }

    for (auto rewardedQuest : rewardedQuests)
    {
        packet << uint32(rewardedQuest);
    }

    SendPacket(&packet);
}

void InterRealmSession::SendBanInterRealmCharacter(const std::string& name, const std::string& realmName, const std::string& gmName, const std::string& reason, const std::string& duration)
{
    WorldPacket packet(IR_CMSG_BAN_INTERREALM_PLAYER);

    packet << name;
    packet << realmName;
    packet << gmName;
    packet << reason;
    packet << duration;

    SendPacket(&packet);
}

void InterRealmSession::Handle_BanInterRealmCharacterResponse(WorldPacket& recvPacket)
{
    uint32 responseCode = 0;
    std::string playerName;
    std::string realmName;
    std::string reason;
    std::string gmName;
    std::string duration;

    recvPacket >> responseCode;
    recvPacket >> playerName;
    recvPacket >> realmName;
    recvPacket >> reason;
    recvPacket >> gmName;
    recvPacket >> duration;

    if (Player* gmPlayer = sObjectAccessor->FindPlayerByName(gmName.c_str()))
    {
        switch (responseCode)
        {
            case BanInterRealmPlayerResponses::BAN_IR_PLAYER_OK:
                ChatHandler(gmPlayer).PSendSysMessage(LANG_INTERREALM_BAN_PLAYER_RESPONSE_OK, playerName.c_str(), realmName.c_str());
                break;
            case BanInterRealmPlayerResponses::BAN_IR_PLAYER_REALM_NOT_FOUND:
                ChatHandler(gmPlayer).PSendSysMessage(LANG_INTERREALM_BAN_PLAYER_RESPONSE_REALM_NOT_FOUND, realmName.c_str());
                break;
            case BanInterRealmPlayerResponses::BAN_IR_PLAYER_WRONG_NAME:
                ChatHandler(gmPlayer).PSendSysMessage(LANG_INTERREALM_BAN_PLAYER_RESPONSE_WRONG_NAME, playerName.c_str());
                break;
            case BanInterRealmPlayerResponses::BAN_IR_PLAYER_WRONG_REASON:
                ChatHandler(gmPlayer).PSendSysMessage(LANG_INTERREALM_BAN_PLAYER_RESPONSE_WRONG_REASON, reason.c_str());
                break;
            case BanInterRealmPlayerResponses::BAN_IR_PLAYER_WRONG_GMNAME:
                ChatHandler(gmPlayer).PSendSysMessage(LANG_INTERREALM_BAN_PLAYER_RESPONSE_WRONG_GMNAME, gmName.c_str());
                break;
            case BanInterRealmPlayerResponses::BAN_IR_PLAYER_WRONG_DURATION:
                ChatHandler(gmPlayer).PSendSysMessage(LANG_INTERREALM_BAN_PLAYER_RESPONSE_WRONG_DURATION, duration.c_str());
                break;
        }
    }
}

void InterRealmSession::Handle_BanCharacter(WorldPacket& recvPacket)
{
    std::string name, realmName, duration, reason, gmName;
    uint8 fromOtherRealm;

    recvPacket >> name >> duration >> reason >> gmName >> fromOtherRealm;
    recvPacket >> realmName;

    if (!normalizePlayerName(name))
    {
        SendBanCharacterResponse(BanInterRealmPlayerResponses::BAN_IR_PLAYER_WRONG_NAME, name, duration, reason, gmName, fromOtherRealm ? true : false, realmName);
        return;
    }

    BanReturn result = sWorld->BanCharacter(name, duration, reason, gmName);
    switch (result)
    {
        case BanReturn::BAN_SUCCESS:
            SendBanCharacterResponse(BanInterRealmPlayerResponses::BAN_IR_PLAYER_OK, name, gmName, reason, duration, fromOtherRealm ? true : false, realmName);
            break;
        case BanReturn::BAN_SYNTAX_ERROR:
            SendBanCharacterResponse(BanInterRealmPlayerResponses::BAN_IR_PLAYER_WRONG_DURATION, name, gmName, reason, duration, fromOtherRealm ? true : false, realmName);
            break;
        default:
            SendBanCharacterResponse(BanInterRealmPlayerResponses::BAN_IR_PLAYER_WRONG_NAME, name, gmName, reason, duration, fromOtherRealm ? true : false, realmName);
            break;
    }
}

void InterRealmSession::SendBanCharacterResponse(BanInterRealmPlayerResponses resposeCode, const std::string& name, const std::string& gmName, const std::string& reason, const std::string& duration, bool fromOtherRealm, const std::string realmName/* = ""*/)
{
    WorldPacket packet(IR_CMSG_BAN_CHARACTER, 4 + name.size() + gmName.size() + reason.size() + duration.size() + 1 + (fromOtherRealm ? realmName.size() : 0));

    packet << uint32(resposeCode);
    packet << name;
    packet << gmName;
    packet << reason;
    packet << duration;
    packet << uint8(fromOtherRealm ? 1 : 0);
    packet << realmName;

    SendPacket(&packet);
}

void InterRealmSession::AddPacket(WorldPacket* new_packet)
{
    _queue.add(new_packet);
}

void InterRealmSession::Update(const uint32 diff)
{
    if (m_needProcessDisconnect)
        ProcessPlayersAfterDisconnect();

    // Process packets
    WorldPacket* packet = NULL;

    if (sBattlegroundMgr->HaveSpectatorData() && (!m_IRSocket || m_IRSocket && m_IRSocket->IsClosed()))
        sBattlegroundMgr->ClearSpectatorData();

    while (m_IRSocket && !m_IRSocket->IsClosed() && !_queue.empty() && _queue.next(packet))
    {
        // Handle Packet
        if (packet->GetOpcode() < IR_NUM_MSG_TYPES)
        {
            try
            {
                IROpcodeHandler* IRopHandle = IRopcodeTable[packet->GetOpcode()];
                if (!IRopHandle)
                {
                    sLog->outError(LOG_FILTER_INTERREALM, "Cannot find handle for the opcode (%u). Skipped packet.",
                    packet->GetOpcode());
                    continue;
                }
                (this->*(IRopHandle->IRhandler))(*packet);
            }
            catch(ByteBufferException &)
            {
                sLog->outError(LOG_FILTER_INTERREALM, "InterRealmSession ByteBufferException occured while parsing a packet (opcode: %u). Skipped packet.",
                    packet->GetOpcode());
            }
            if (packet->rpos() < packet->wpos())
            {
                sLog->outError(LOG_FILTER_INTERREALM, "Unprocessed tail data (read stop at %u from %u) in opcode %s", packet->rpos(), packet->wpos(), IRopcodeTable[packet->GetOpcode()]->name);
            }
        }
        else
            this->Handle_Unhandled(*packet);

        // Delete Packet from memory
        if (packet != NULL)
        {
            //sLog->outInterRealm(LOG_FILTER_GENERAL, "[INTERREALM] Deleting packet");
            delete packet;
        }
    }
}

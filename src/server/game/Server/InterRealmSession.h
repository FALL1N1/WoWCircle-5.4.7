#ifndef INTERREALM_TUNNEL_H
#define INTERREALM_TUNNEL_H

#ifndef INVALID_SOCKET
#define NOT_WINDOWS 1

#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#endif

//typedef struct sockaddr_in SOCKADDR_IN;
//typedef struct sockaddr SOCKADDR;

#include <ace/Reactor.h>
#include <ace/Reactor_Impl.h>
#include <ace/TP_Reactor.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Connector.h>
#include <ace/SOCK_Connector.h>

#include "IRSocketConnector.h"

#define IR_PROTOCOL_VERSION     1
#define IR_PROTOCOL_SUBVERSION  0
#define IR_PROTOCOL_COMPRESS    0
#define IR_PROTOCOL_ENCRYPT     0

enum irHeloResp
{
    IR_HELO_RESP_OK	                = 0,
    IR_HELO_RESP_POLITE	            = 1,
    IR_HELO_RESP_PROTOCOL_MISMATCH  = 2,
};

enum irRegisterResp
{
    IR_REG_OK                   = 0,
    IR_REG_ALREADY_REGISTRED    = 1,
    IR_REG_GROUP_NO_GUIDS       = 2,
    IR_REG_CANNOT_CREATE        = 3,
    IR_REG_CANNOT_LOAD          = 4,
    IR_REG_SPEC_CANNOT_FIND     = 5,
};

enum BanInterRealmPlayerResponses : int
{
    BAN_IR_PLAYER_OK                = 0,
    BAN_IR_PLAYER_REALM_NOT_FOUND   = 1,
    BAN_IR_PLAYER_WRONG_NAME        = 2,
    BAN_IR_PLAYER_WRONG_GMNAME      = 3,
    BAN_IR_PLAYER_WRONG_DURATION    = 4,
    BAN_IR_PLAYER_WRONG_REASON      = 5,
};

struct IRInPktHeader
{
    uint32 size;
    uint32 cmd;
};

struct IROutPktHeader
{
    /**
     * size is the length of the payload _plus_ the length of the opcode
     */
    IROutPktHeader(uint32 size, uint32 cmd) : size(size)
    {
        uint8 headerIndex = 0;
        header[headerIndex++] = 0xFF & size;
        header[headerIndex++] = 0xFF &(size >> 8);
        header[headerIndex++] = 0xFF &(size >> 16);
        header[headerIndex++] = 0xFF &(size >> 24);
        
        header[headerIndex++] = 0xFF & cmd;
        header[headerIndex++] = 0xFF & (cmd >> 8);
        header[headerIndex++] = 0xFF &(cmd >> 16);
        header[headerIndex++] = 0xFF &(cmd >> 24);
    }

    uint8 getHeaderLength()
    {
        // cmd = 4 bytes, size= 4 bytes
        return 4 + 4;
    }

    const uint32 size;
    uint8 header[8];
};

enum ProtocolVersions
{
    PROTOCOL_BASIC              = 0, // without any packet syncronizations
    PROTOCOL_WITH_CURRENCY      = 1, // with currency syncronization
    PROTOCOL_WITH_ACHIEVEMENT   = 2, // with both currency and achievement syncronization
    PROTOCOL_WITH_REPUTATION    = 3, // with both currency/achievement and reputation syncronization
    PROTOCOL_WITH_KILLS         = 4, // with both honorable kills and currency/achievement/reputation syncronization
};

class IRReactorRunnable;
class ACE_Event_Handler;
class Player;
class Group;
class ByteBuffer;

class InterRealmSession: public ACE_Based::Runnable
{
    public:
        InterRealmSession();
        ~InterRealmSession();

        void run();

        uint32 GetProtocolVersion() const { return m_ProtocolVersion; }

        bool IsConnected() const { return m_IsConnected; }
        void SetConnected(bool value) { m_IsConnected = value; }

        bool IsTunnelOpened() const { return m_tunnel_open; }
        int OnSocketOpen(IRSocket* socket);

        void SendPing(uint64 guid, uint32 ping, uint32 latency);

        void SendTunneledPacket(uint64 guid, WorldPacket const* packet);
        void SendTunneledPacketToClient(uint64 guid, WorldPacket const *packet);
        void SendPacket(WorldPacket const* packet);
        void SendPSysMessage(Player *player, char const *format, ...);
        void SendServerAnnounce(uint64 guid, std::string const &text);
        void SendPlayerTeleport(Player *player, uint32 zoneId, Player *target, bool isSpectator = false);

        void SendRegisterPlayer(Player* player, uint32 bgInstanceId, uint64 bgGuid, uint32 bgTypeId);
        void SendRegisterGroup(Group* group, uint32 bgInstanceId, uint64 bgGuid, uint32 bgTypeId);
        void SendRegisterArena(Group* group, uint32 arenaSlot);
        void SendRegisterRated(Group* group, uint32 personalRating, uint32 matchmakerRating);
        void SendRegisterSpectator(Player* player, uint64 targetGuid);

        void Handle_Unhandled(WorldPacket& recvPacket);
        void Handle_Null(WorldPacket& recvPacket);
        void Handle_ClientSide(WorldPacket& recvPacket) { }

        void Handle_Hello(WorldPacket& recvPacket);
        void Handle_WhoAmI(WorldPacket& recvPacket);
        void Handle_CheckPlayers(WorldPacket& recvPacket);

        void Handle_DebugBGResp(WorldPacket& recvPacket);
        void Handle_DebugArenaResp(WorldPacket& recvPacket);

        void Handle_TunneledPacket(WorldPacket& recvPacket);

        void Handle_RegisterPlayerResp(WorldPacket& recvPacket);
        void Handle_UnRegisterPlayerResp(WorldPacket& recvPacket);
        void Handle_RegisterRatedResp(WorldPacket& recvPacket);
        void Handle_RegisterSpectatorResp(WorldPacket& packet);
        void Handle_BattlefieldPortResp(WorldPacket& recvPacket);
        void Handle_BattlefieldLeave(WorldPacket& recvPacket);

        void Handle_BGQueueAnnouncer(WorldPacket& recvPacket);
        void Handle_ArenaQueueAnnouncer(WorldPacket& recvPacket);
        void Handle_BattlegroundStartAnnoucer(WorldPacket& recvPacket);

        void Handle_Whisper(WorldPacket& recvPacket);
        void Handle_GuildChat(WorldPacket& recvPacket);

        void Handle_PlayerZoneID(WorldPacket& recvPacket);

        void Handle_PlayerPInfo(WorldPacket& recvPacket);

        void Handle_BanCharacter(WorldPacket& recvPacket);
        void SendBanInterRealmCharacter(const std::string& name, const std::string& realmName, const std::string& gmName, const std::string& reason, const std::string& duration);
        void Handle_BanInterRealmCharacterResponse(WorldPacket& recvPacket);
        void SendBanCharacterResponse(BanInterRealmPlayerResponses resposeCode, const std::string& name, const std::string& gmName, const std::string& reason, const std::string& duration, bool fromRealm, const std::string realmName = "");
        //void Handle_Ban(WorldPacket& recvPacket);
        //void Handle_UnbanCharacter(WorldPacket& recvPacket);
        //void Handle_Unban(WorldPacket& recvPacket);
        void Handle_KickPlayer(WorldPacket& recvPacket);
        void Handle_SummonPlayer(WorldPacket& recvPacket);
        void Handle_SpectatorData(WorldPacket& recvPacket);

        void Handle_BattlegroundHolidayResp(WorldPacket& recvPacket);
        
        void Handle_DistributeArenaPointsResp(WorldPacket& recvPacket);

        void Handle_AchievementReward(WorldPacket& recvPacket);

        void Handle_AnticheatReport(WorldPacket& recvPacket);
        void Handle_AnticheatReportNotification(WorldPacket& recvPacket);

        void Handle_AppearRequestResp(WorldPacket& recvPacket);

        void Handle_AdditionalInfo(WorldPacket& recvPacket);

        void Handle_GuildQuery(WorldPacket& recvPacket);

        void Handle_SyncPlayerCurrency(WorldPacket& recvPacket);    
        void Handle_SyncPlayerAchievements(WorldPacket& recvPacket);
        void Handle_SyncPlayerReputation(WorldPacket& recvPacket);
        void Handle_SyncPlayerKills(WorldPacket& recvPacket);
        void Handle_SyncPlayerQuests(WorldPacket& recvPacket);

        void SendWhisper(uint64 sender, uint64 receiver, const std::string& text, const uint32 language);
        void SendBattlegroundHolidaysQuery();

        void SendCurrencyResetForPlayer(Player* player);

        void SendSummonPlayer(uint64 playerGuid);

        void SendAppearRequest(Player* sender, uint64 targetGuid);

        void SendAccountSpells(Player* player);

        void SendCharacterSkills(Player* player);

        void SendGuild(uint64 guildGuid);

        void SendCharacterQuests(Player* player);

        void AddPacket(WorldPacket* new_packet);

        void Update(const uint32 diff);

        void SetProcessDisconnect(bool value)
        {
            m_needProcessDisconnect = value;
        }

        void ProcessPlayersAfterDisconnect();

        void ClearSocket();

    private:

        void BuildPlayerArenaInfoBlock(Player* player, ByteBuffer& packet);

    private:

        bool m_needProcessDisconnect;

        IRReactorRunnable* m_Reactor;
        IRSocketConnector* m_Connector;
        IRSocket* m_IRSocket;
        int m_SockOutKBuff;
        int m_SockOutUBuff;
        bool m_UseNoDelay;

        bool m_force_stop;
        bool m_tunnel_open;
        bool m_IsConnected;

        std::string m_IP;
        uint16 m_port;
        uint32 m_ir_id;
        uint32 m_ProtocolVersion;

        uint8 m_rand;

        ACE_Based::LockedQueue<WorldPacket*, ACE_Thread_Mutex> _queue; 

        /// here are stored the fragments of the received data
        WorldPacket* m_RecvWPct;

        /// This block actually refers to m_RecvWPct contents,
        /// which allows easy and safe writing to it.
        /// It wont free memory when its deleted. m_RecvWPct takes care of freeing.
        ACE_Message_Block m_RecvPct;

        /// Fragment of the received header.
        ACE_Message_Block m_Header;

        int handle_input_header (void);
        int handle_input_payload (void);
        int handle_input_missing_data (void);
};

#define sIRTunnel ACE_Singleton<InterRealmSession, ACE_Null_Mutex>::instance()

#endif

#pragma once

#include "Unit.h"
#include "BattlePet.h"

#define PET_BATTLE_XP_REWARD 100

enum PetBattleState : uint32
{
    StateNone = 0,
    StateBeginning = 2,
    StateInProgress = 3,
    StateWaitingForPetSwitch = 4,
    StateEnding = 7,
};

namespace BattlePet
{
    struct ActionInfo;

    typedef bool(Battle::*pAction)(ActionInfo const* action, std::shared_ptr<MemberData> member);

    typedef std::map<PBOID, Pet*> Pets;
    typedef std::pair<PBOID, Pet*> PetPair;

    class EventHolder;

    class MemberData : public std::enable_shared_from_this<MemberData>
    {
    public:
        MemberData(Battle* battle, PBOID side, Unit* owner);
        ~MemberData();

        bool Initial(Unit* owner);
        void Update();

        void SetActivePet(PBOID target);

        TrapStatus GetTrapStatus() const;
        BattlePetDisableFlags GetFlags() const;

        bool IsInitiator() const { return m_side == PBOID::PAD_0; }

        bool SwapReq() const { return m_pet && m_pet->IsDead(); }

        Pet* GetActivePet() { return m_pet; }
        Pet const* GetActivePet() const { return m_pet; }

        uint64 GetOwner() { return m_guid; }
        uint64 const GetOwner() const { return m_guid; }

        std::shared_ptr<MemberData>& GetOpponent() { return m_opponent; }
        std::shared_ptr<MemberData> const& GetOpponent() const { return m_opponent; }

        Pet* m_pet;
        Battle* m_battle;

        Pets m_pets;
        uint64 m_guid;
        PBOID m_side;
        bool m_initialized;

        std::shared_ptr<MemberData> m_opponent;
    };

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif
    struct ActionInfo
    {
        union
        {
            uint32 m_abilityId;
            uint32 m_petIndex;
        };

        ActionInfo()
            : m_index(0)
            , m_action(BattlePetActionType::BATTLE_PET_ACTION_FORFEIT)
            , m_unkBit(false)
        {
            m_abilityId = 0;
        }

        uint32 m_index;
        uint8 m_action;
        bool m_unkBit;
    };

    struct PositionData
    {
        PositionData()
            : m_orientation(0.0f)
        {
        }

        G3D::Vector3 m_positions[PET_BATTLE_OPPONENT_COUNT];
        G3D::Vector3 m_center;
        float m_orientation;

        void Calculate();
    };

    struct RewardInfo
    {
        struct RewardData
        {
            RewardData(Pet* pet, uint16 xp)
                : m_pboid(pet->GetPBOID())
                , m_level(pet->GetLevel())
                , m_oldLevel(pet->GetOldLevel())
                , m_hp(pet->GetHealth())
                , m_maxHp(pet->GetMaxHealth())
                , m_xp(xp)
            {
            }
            PBOID m_pboid : 16;
            uint16 m_xp;
            uint16 m_level;
            uint16 m_oldLevel;
            int32 m_hp;
            int32 m_maxHp;
        };

        typedef std::vector<RewardData> Rewards;

        PBOID m_winner : 16;
        PetBattleEndReason m_reason : 16;
        Rewards m_rewards;

        RewardInfo(PBOID winner, PetBattleEndReason reason)
            : m_winner(winner)
            , m_reason(reason)
        {

        }
    };
#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

    typedef std::shared_ptr<MemberData> sMemberData;

    class Battle
    {
    public:
        Battle(bool isPvp, bool isLfb);

        ~Battle();

        bool AddMember(Unit* owner, PBOID pboid);
        void EndBattle(PBOID winner, PetBattleEndReason reason);
        void EndBattle(uint64 guid);

        void GiveRewards(PBOID winner, PetBattleEndReason reason);
        void SendRewards(RewardInfo const& info);

        void Cleanup();

        void SendPacket(WorldPacket* data);

        void InitialAllMembers();
        void SendInitialPetPos(Player* player);
        void SendInitialBattle(Player* player);
        void SendInitialPet(Player* player);

        void SendNewRound();
        void SendEvents();

        void UpdateFlags();

        void HandleAction(Unit* unit, ActionInfo const* action);

        void SendSwapReq(PBOID diedPet, PBOID side);

        static void CreateBattle(Unit* initiator, Unit* opponent, bool start = true, bool isLfb = false);

        void InitialMember(Player* player);
        void SendInitialToPlayer(Player* player);
        void InitialBattle();

        void Update(uint32 const diff);

        void StartRound();
        void EndRound();

        void OnPetDied(Pet* pet);
        void OnHPChanged(Pet* pet, Pet* whoChange, int32 newHp, uint32 dueToAbilityEffect, uint16 flags);
        void OnAuraCreated(Pet* owner, Pet* creator, uint32 dueToAbilityEffect, uint16 auraId, uint32 amount, int32 duration, uint16 flags);
        void OnAuraChanged(Pet* owner, uint16 auraId, uint32 newAmount, int32 newDuration); // new round also
        void OnAuraRemoved(Pet* owner, uint16 auraId, uint8 removeReason /*duration, dispell*/, uint16 flags);
        void OnStateChanged(Pet* pet, Pet* whoChange, uint32 dueToAbilityEffect, uint16 stateId, int32 newValue, uint16 flags);

        void OnActionPerformed(PBOID side, BattlePetActionType action);
        void TryEndRound();

        bool IsPvP() { return m_isPvp; }

        bool IsPvP() const { return m_isPvp; }

        bool IsStarted() { return m_state == PetBattleState::StateBeginning; }

        bool IsStarted() const { return m_state == PetBattleState::StateBeginning; }

        bool IsEnded() { return m_state == PetBattleState::StateEnding; }

        bool IsEnded() const { return m_state == PetBattleState::StateEnding; }

        void SetPosition(G3D::Vector3 position, PBOID side);

        uint64 GetId() { return m_battleId; }

        uint64 GetId() const { return m_battleId; }

        bool HasMoved(PBOID side) { return m_movedMask & (1 << side); }

        bool HasMoved(PBOID side) const { return m_movedMask & (1 << side); }

        bool CanMove(PBOID side) { return m_canMoveMask & (1 << side); }

        bool CanMove(PBOID side) const { return m_canMoveMask & (1 << side); }

        void SetCanMove(PBOID side, bool set)
        {
            if (!set)
                m_canMoveMask &= ~(1 << side);
            else
                m_canMoveMask |= 1 << side;
        }

        void SetMoved(PBOID side, bool set)
        {
            if (!set)
                m_movedMask &= ~(1 << side);
            else
                m_movedMask |= 1 << side;
        }

        bool ActionForfeit(ActionInfo const* action, sMemberData member);
        bool ActionUseAbility(ActionInfo const* action, sMemberData member);
        bool ActionSwapPet(ActionInfo const* action, sMemberData member);
        bool ActionUseTrap(ActionInfo const* action, sMemberData member);
        bool ActionEndBattle(ActionInfo const* action, sMemberData member);

    private:
        sMemberData GetMember(uint64 const guid)
        {
            for (MemberPair const& pair : m_members)
            {
                if (sMemberData member = pair.second)
                    if (member->GetOwner() == guid)
                        return member;
            }

            return nullptr;
        }

        void SetActivePet(sMemberData& member, PBOID pboid);

        PBOID GetInitialSide() const;

        typedef std::map<PBOID, sMemberData> Members;
        typedef std::pair<PBOID, sMemberData> MemberPair;

        bool m_isPvp;
        bool m_isLfb;

        PetBattleState m_state;

        uint32 m_movedMask;
        uint32 m_canMoveMask;
        uint32 m_round;
        uint64 m_battleId;
        int32 m_roundTime;

        EventHolder* m_event;
        PositionData* m_pos;

        Members m_members;
    };
}
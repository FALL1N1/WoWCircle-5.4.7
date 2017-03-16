#include "PetBattle.h"
#include "PetBattleEvent.h"

#include "Player.h"
#include "ObjectMgr.h"
#include "../PetJournal/PetJounral.h"
#include "WorldPacket.h"
#include "Ability/BattlePetAbility.h"
#include "PetBattleMgr.h"

/*Создали битву, добавили двух участников через AddMember, проинициализировали петов
Вызываем SendInitialBattle
Вызываем SendInitialPetPos (для каждого игрока свой)
и вызываем InitialBattle()

Далее клиент отправляет опкод, в ответ на который мы должны отправить SendInitialPet
В котором содержится информация о  том кто будет ходить первый, статус ловушки и т.п
В нем m_round = 0, так что скорее всего битва еще даже не стартанула.
Это похоже на то, что мы информируем сервер о своей готовности учавствовать в битве.
И когда оба игрока проинформировали - стартуем

Так и делаем
Игрок отправляет этот опкод, ставим текущему member-у статус m_readyToStart = true
Вызываем CheckStartBattle()
В которой проверяем чтобы у обоих был m_readyToStart = true
До тех пор ничего не делаем. Как только у обоих m_readyToStart = true

вызываем UpdateFlags()
Внутри нее мы указываем кто может ходить основываяс на скорости петов

TODO: надо иметь 2 отдельный переменные - m_moved и m_canMove
Одной определяем походил ли игрок, второй - может ли он ходить.
Для удобства сделаем 2 uint переменные в PetBattle, будем хранить это все ввиде маски
Чтобы не получать каждый раз доступ к memberData

Вызвали UpdateFlags. Оба мембера имеют m_moved = false
Тот, у кого меньше скорость имеет m_canMove = true, второй - m_canMove = false

Выставили флаги, отправили каждому SendInitialPet
В этой функции отправляем не m_roundTime/1000, а PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC

выставили m_round = 1, m_state = Running

Теперь 1 из игроков ходит, вызывается HandleAction, которая производить действие
в зависимости от входных данных

в конце HandleAction маркируем игрока, который походит как походивший (m_moved = true)
и запрещаем ему ходить (m_canMove = false)
и противнику выставляем поля наоборот (m_canMove = true)

проверяем походити ли оба игрока (m_moved = true для обоих)
и если да то вызываем EndRound, в конце обрабатываем события конца раунда
и стартуем новый раунд.

Очень важно чтобы в HandleAction был вызван SendEvents
Также он должен быть вызван в EndRound и в StartRound

Сделаем функцию OnTurnMaked(PBOID side)
в которой записываем кто походит, обновляем флаги  вызовом UpdateFlags
Вызываем SendEvents()

В StartRound также вызываем Update для каждого MemberData
Внутри MemberData::Update вызываем Update у каждого пета



В функции StartRound есть InitialRound
Которая проверяет кто первый ходит, обновляет флаги и отправляет клиенту.
После в StartRound идет вызов MemberData::Update
Далее Pet::Update Далее там идет вызов эффектов абилки.
Может произойти ситуация, когда ходить должна сторона X
Но сторона Y умерла в течении Update

Получится так, что сторона X сможет бить по уже мертвому пету

Для избежания этого необходимо перед Update отправить инфу о том, что стартовал новый раунд
Чтобы урон был нанесен уже в новом, а не в конце старого
А после пересчитать флаги и отправить еще 1 SendEvents

По идеи, в первом SendEvents ( перед Update) мы должна обоим отправить 7 флаг



SendInitialPetPos - расставляет петов (вызываем сами)
SendInitialBattle - инициализирует петов (вызываем сами)
SendInitialPet - стартуем битву




==============================================================================
По идеи ауры должны тикать в конце каждого раунда, а не в начале. 

Сейчас тикает в начале каждого т.к если он будет тикать в конце, то за 1 ход протикает 2 раза
1 раз при наложении, и второй раз в конце.


Решение: добавляем переменную m_round в которой сохраним round в котором абилка была наложена
В апдейте проверяем: если текущий раунд равен раунду из абилки, то апдейт не вызываем

==============================================================================
Стартует раунд, вызываем Update всем петам и мемберам
Это нужно для того чтобы вызвать Update у аур и абилок и обработать Multi-turn абилки

Выставляем флаги по которым определяем кто может ходить\походил
Может ходить если: мертвы или скорость больше чем у оппонента. Первыми должны ходить мемберы, имеющие мертвого активного пета
Походил: есть активная multi-turn абилка

Отправили их в SendEvents клиенту

Далее в OnTurnMaked смотрим

Раунд меняется когда оба участника сделали ход, вернее отмечены как походившие



Отправляем их клиенту
*/

BattlePet::pAction Actions[BATTLE_PET_ACTION_MAX] =
{
    &BattlePet::Battle::ActionForfeit,
    &BattlePet::Battle::ActionUseAbility,
    &BattlePet::Battle::ActionSwapPet,
    &BattlePet::Battle::ActionUseTrap,
    &BattlePet::Battle::ActionEndBattle,
};

BattlePet::Battle::Battle(bool isPvp, bool isLfb)
    : m_isPvp(isPvp)
    , m_isLfb(isLfb)
    , m_canMoveMask(0)
    , m_movedMask(0)
    , m_round(0)
    , m_battleId(0LL)
    , m_roundTime(PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC * IN_MILLISECONDS)
    , m_state(PetBattleState::StateNone)
    , m_pos(nullptr)
    , m_event(nullptr)
{
    m_event = new EventHolder();
    m_pos = new PositionData();
}

BattlePet::Battle::~Battle()
{
    Cleanup();

    m_members.clear();

    delete m_event;
    m_event = nullptr;

    delete m_pos;
    m_pos = nullptr;
}

bool BattlePet::Battle::AddMember(Unit* owner, PBOID pboid)
{
    ASSERT(m_members.size() < PET_BATTLE_OPPONENT_COUNT);

    if (pboid == PBOID::PAD_0)
    {
        for (MemberPair const& pair : m_members)
        {
            sMemberData const& member = pair.second;

            ASSERT(member);

            if (member->IsInitiator())
            {
                // TODO: error - there is another initiator
                return false;
            }
        }

        m_battleId = owner->GetGUID();
    }

    SetPosition(G3D::Vector3(owner->GetPositionX(),
        owner->GetPositionY(), owner->GetPositionZ()), pboid);

    m_members[pboid] = std::make_shared<MemberData>(this, pboid, owner);
    if (!m_members[pboid]->Initial(owner))
        return false;

    owner->SetPetBattleId(m_battleId); // TODO: ugly hack

    if (pboid == PBOID::PAD_1)
    {
        m_members[PBOID::PAD_0]->m_opponent = m_members[PBOID::PAD_1];
        m_members[PBOID::PAD_1]->m_opponent = m_members[PBOID::PAD_0];
    }

    return true;
}

void BattlePet::Battle::EndBattle(uint64 guid)
{
    if (guid == 0LL || m_state == PetBattleState::StateEnding)
        return;

    if (GetMember(guid) == nullptr)
        return;

    PBOID side = GetMember(guid)->m_side;
    PBOID winner = side == PBOID::PAD_0 ? PBOID::PAD_1 : PBOID::PAD_0;

    EndBattle(winner, PetBattleEndReason::Forfeit);
}

void BattlePet::Battle::EndBattle(PBOID winner, PetBattleEndReason reason)
{
    if (m_state == PetBattleState::StateEnding)
        return;

    m_state = PetBattleState::StateEnding;

    GiveRewards(winner, reason);

    for (MemberPair const& pair : m_members)
    {
        if (sMemberData const& member = pair.second)
        {
            if (Player* owner = sObjectAccessor->FindPlayer(member->GetOwner()))
            {
                owner->GetPetJournal()->SendPetsUpdateInLoadoutSlots(false);
            }
        }
    }

    sPetBattleMgr->RemoveBattle(m_battleId);
}

void BattlePet::Battle::GiveRewards(PBOID winner, PetBattleEndReason reason)
{
    RewardInfo info(winner, reason);

    if (sMemberData const& team = m_members[winner]) // winner team
    {
        for (PetPair const& pair : team->m_pets)
        {
            Pet* pet = pair.second;
            if (!pet)
                continue;

            uint16 xp = PET_BATTLE_XP_REWARD; // TODO - calculate proper value based on pet activity and pet status
            if (pet->IsDead())
                xp /= 1.5f;

            pet->GixeXP(xp);

            info.m_rewards.push_back(RewardInfo::RewardData(pet, xp));
        }
    }

    SendRewards(info);
}

void BattlePet::Battle::SendRewards(RewardInfo const& info)
{
    if (m_state != PetBattleState::StateEnding)
        return;

    WorldPacket data(SMSG_PET_BATTLE_REWARD);

    data.WriteBits(info.m_rewards.size(), 20);
    for (MemberPair const& pair : m_members)
    {
        data.WriteBit(pair.first == info.m_winner);
    }

    for (RewardInfo::RewardData const& reward : info.m_rewards)
    {
        /*показывает 40% от того числа, что мы отправили - wtf ?*/
        ////Captured | Caged | SeenAction | AwardedXP
        data.WriteBit(false); // unk
        data.WriteBit(false); // pet loose
        data.WriteBit(true); // SeenAction
        data.WriteBit(false); // Captured
        data.WriteBit(reward.m_level == 0);
        data.WriteBit(reward.m_xp == 0);
        data.WriteBit(reward.m_oldLevel == 0);
    }

    for (MemberPair const& pair : m_members)
    {
        if (pair.first != info.m_winner)
            data.WriteBit(info.m_reason == PetBattleEndReason::Forfeit);
        else
            data.WriteBit(false);
    }

    for (uint8 i = 0; i < PET_BATTLE_OPPONENT_COUNT; ++i)
    {
        data << uint32(0); // npcId
    }

    for (RewardInfo::RewardData const& reward : info.m_rewards)
    {
        data << uint8(reward.m_pboid);
        if (reward.m_level != 0)
            data << uint16(reward.m_level);
        if (reward.m_xp != 0)
            data << uint16(reward.m_xp);

        data << int32(reward.m_hp);
        data << int32(reward.m_maxHp);

        if (reward.m_oldLevel != 0)
            data << uint16(reward.m_oldLevel);
    }

    SendPacket(&data);
}

void BattlePet::Battle::Cleanup()
{
    for (MemberPair const& pair : m_members)
    {
        sMemberData const& member = pair.second;
        if (Player* player = sObjectAccessor->FindPlayer(member->GetOwner()))
            player->SetPetBattleId(0);
    }
}

PBOID BattlePet::Battle::GetInitialSide() const
{
    uint32 pad0 = 0;
    uint32 pad1 = 0;

    for (MemberPair const& pair : m_members)
    {
        if (sMemberData const& member = pair.second)
        {
            if (Pet const* pet = member->GetActivePet())
            {
                if (pet->IsDead())
                    return member->m_side;

                if (pet->HasActiveMultiturnAbility())
                    return member->m_side == PBOID::PAD_0 ? PBOID::PAD_1 : PBOID::PAD_0;

                if (member->m_side == PBOID::PAD_0)
                    pad0 = pet->GetSpeed();
                else
                    pad1 = pet->GetSpeed();
            }
        }

    }

    if (pad0 > pad1)
        return PBOID::PAD_0;

    return PBOID::PAD_1;
}

void BattlePet::Battle::SendInitialPetPos(Player* player)
{
    m_pos->Calculate();

    sMemberData member = GetMember(player->GetGUID());
    if (!member)
        return;

    WorldPacket data(SMSG_PET_BATTLE_POSITION);
    for (int i = 0; i < 2; ++i)
    {
        G3D::Vector3 const& pos = m_pos->m_positions[member->m_side == PBOID::PAD_0 ? 0 : 1];
        data << pos.z << pos.y << pos.x;
    }

    data
        << m_pos->m_center.z
        << m_pos->m_center.x
        << m_pos->m_center.y
        ;

    data.WriteBit(G3D::fuzzyEq(m_pos->m_orientation, 0.0f));
    data.WriteBit(!true);

    if (!G3D::fuzzyEq(m_pos->m_orientation, 0.0f))
        data << m_pos->m_orientation;
    if (true) // status
        data << uint32(21);

    player->SendDirectMessage(&data);
}

void BattlePet::Battle::SetPosition(G3D::Vector3 position, PBOID side)
{
    uint8 index = side == PBOID::PAD_0 ? 0 : 1;

    m_pos->m_positions[index] = position;
}

void BattlePet::Battle::SendInitialBattle(Player* player)
{
    bool hasFrontPet = true;
    bool hasRoundTimeSecs = false;

    WorldPacket data(SMSG_PET_BATTLE_INITIAL);
    for (MemberPair const& pair : m_members)
    {
        sMemberData const& member = pair.second;

        ObjectGuid guid = IS_PLAYER_GUID(member->GetOwner()) ? member->GetOwner() : 0LL;

        data.WriteBit(guid[0]);
        data.WriteBit(guid[4]);
        data.WriteBit(TrapStatus::TRAP_STATUS_CANT_CATCH_OPPONENTS_PET == TrapStatus::TRAP_STATUS_OK);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[7]);
        data.WriteBit(guid[6]);
        data.WriteBit(guid[2]);
        data.WriteBit(!hasFrontPet);
        data.WriteBit(!hasRoundTimeSecs);
        data.WriteBit(guid[5]);
        data.WriteBits(member->m_pets.size(), 2);
        
        for (PetPair const& ppair : member->m_pets)
        {
            Pet const* pet = ppair.second;
            if (!pet)
                continue;

            ObjectGuid pguid = pet->GetProto()->GetId();

            data.WriteBit(pguid[4]);
            data.WriteBit(pguid[2]);
            data.WriteBits(pet->GetAuras().size(), 21);

            for (auto& apair : pet->GetAuras())
            {
                Ability const* aura = apair.second;

                data.WriteBit(aura->IsExpired());
                data.WriteBit(m_round == 0);
                data.WriteBit(!true); // aura->GetCaster()->GetPBOID() - should always present
            }

            data.WriteBit(pguid[6]);
            data.WriteBit(pguid[3]);
            data.WriteBit(!false);
            data.WriteBits(pet->GetStates().size(), 21);
            data.WriteBit(pguid[0]);
            data.WriteBits(pet->GetAbilities().size(), 20);
            data.WriteBit(pguid[1]);
            data.WriteBits(pet->GetProto()->GetNickname().size(), 7);

            for (uint32 abilityId : pet->GetAbilities())
            {
                data.WriteBit(!true); // pet->GetPBOID() - should always present
            }

            data.WriteBit(pguid[7]);
            data.WriteBit(pguid[5]);
            data.WriteBit(!true); // hasSlot
        }

        data.WriteBit(guid[3]);
    }

    for (int i = 0; i < BATTLE_PET_MAX_LOADOUT_SLOTS; ++i)
    {
        data.WriteBits(0, 21);
        data.WriteBits(0, 21);
    }

    sMemberData const& opponent = m_members[PBOID::PAD_1];

    data.WriteBit(!false);
    data.WriteBit(PET_BATTLE_FORFEIT_PENALTY == 0);
    data.WriteBit(!false);
    data.WriteBit(true); // CanRewardXP
    data.WriteBit(!PET_BATTLE_PVP_MAX_ROUND_SEC);
    data.WriteBit(!PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC);
    data.WriteBit(m_isPvp);
    data.WriteBit(false); // fake

    ObjectGuid wildPet = ObjectGuid(IS_PLAYER_GUID(opponent->GetOwner()) ? 0LL : opponent->GetOwner());

    data.WriteBit(wildPet[1]);
    data.WriteBit(wildPet[0]);
    data.WriteBit(wildPet[3]);
    data.WriteBit(wildPet[7]);
    data.WriteBit(wildPet[2]);
    data.WriteBit(wildPet[6]);
    data.WriteBit(wildPet[5]);
    data.WriteBit(wildPet[4]);

    data.WriteBit(!true); // Unk

    data.WriteByteSeq(wildPet[7]);
    data.WriteByteSeq(wildPet[2]);
    data.WriteByteSeq(wildPet[6]);
    data.WriteByteSeq(wildPet[0]);
    data.WriteByteSeq(wildPet[1]);
    data.WriteByteSeq(wildPet[5]);
    data.WriteByteSeq(wildPet[4]);
    data.WriteByteSeq(wildPet[3]);

    for (MemberPair const& pair : m_members)
    {
        sMemberData const& member = pair.second;

        if (hasFrontPet)
            data << uint8(0);

        uint8 slot = 0;
        for (PetPair const& ppair : member->m_pets)
        {
            Pet const* pet = ppair.second;
            if (!pet)
                continue;

            uint8 index = 0;
            for (uint32 abilityId : pet->GetAbilities())
            {
                data << uint16(0);
                data << uint8(index++);

                if (true)
                    data << uint8(pet->GetPBOID());

                data << uint32(abilityId);
                data << uint16(0);
            }

            ObjectGuid pguid = pet->GetProto()->GetId();

            data.WriteByteSeq(pguid[7]);

            for (auto& apair : pet->GetAuras())
            {
                Ability const* aura = apair.second;

                data << uint32(aura->GetId());
                if (!aura->IsExpired())
                    data << int32(aura->GetDuration());
                if (m_round)
                    data << uint32(m_round);
                if (true)
                    data << uint8(aura->GetCaster()->GetPBOID());
                data << uint32(1);
            }

            data << uint16(pet->GetLevel());
            data.WriteByteSeq(pguid[0]);
            data << uint32(pet->GetProto()->GetSpeed());
            data.WriteByteSeq(pguid[5]);
            data << uint32(pet->GetMaxHealth());
            data.WriteByteSeq(pguid[4]);
            data << uint16(pet->GetXP());
            data << uint8(slot++);
            data.WriteByteSeq(pguid[6]);
            data.WriteByteSeq(pguid[1]);
            data << uint32(pet->GetProto()->GetSpecies());
            data.WriteByteSeq(pguid[2]);

            for (auto const kvp : pet->GetStates())
            {
                data << uint32(kvp.second); // Value
                data << uint32(kvp.first); // StateId
            }

            data << uint16(pet->GetProto()->GetQuality());
            data << uint32(pet->GetHealth());
            data.WriteByteSeq(pguid[3]);
            data << uint32(0);
            data << uint32(pet->GetProto()->GetPower());
            data << uint32(sObjectMgr->GetCreatureTemplate(pet->GetProto()->GetNPCId())->Modelid1/*pet->GetProto()->GetModel()*/);
            data.WriteString(pet->GetProto()->GetNickname());
            data << uint32(0);
        }

        ObjectGuid guid = IS_PLAYER_GUID(member->GetOwner()) ? member->GetOwner() : 0LL;

        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guid[4]);
        if (hasRoundTimeSecs)
            data << uint16(0);

        data.WriteByteSeq(guid[1]);
        data << uint8(6);
        data.WriteByteSeq(guid[6]);

        data << uint32(TRAP_STATUS_CANT_CATCH_OPPONENTS_PET);

        data.WriteByteSeq(guid[2]);
        data << uint32(PET_BATTLE_TRAP_ABILITY_ID); // TODO:
        data.WriteByteSeq(guid[0]);
        data.WriteByteSeq(guid[7]);
        data.WriteByteSeq(guid[3]);
    }

    data << uint8(PET_BATTLE_FORFEIT_PENALTY);
    data << uint16(PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC);

    data << uint32(m_round);

    data << uint8(1); // CurPetBattleState
    data << uint16(PET_BATTLE_PVP_MAX_ROUND_SEC);

    player->SendDirectMessage(&data);
}

void BattlePet::Battle::SendInitialPet(Player* player)
{
    ASSERT(player);

    WorldPacket data(SMSG_PET_BATTLE_EVENT_AT_START);
    data.WriteBits(PET_BATTLE_OPPONENT_COUNT, 22);
    for (uint8 i = 0; i < PET_BATTLE_OPPONENT_COUNT; ++i)
    {
        data.WriteBit(!false);
        data.WriteBit(!PetBattleEvents::PET_SWAP); // m_eventId
        data.WriteBit(!false);
        data.WriteBit(!true);                      // m_eventTargetType
        data.WriteBit(!false);
        data.WriteBit(!false);
        data.WriteBit(!false);

        data.WriteBits(1, 25);
        {
            data.WriteBit(!true); // m_hasEventValue
            data.WriteBits(6, 3); // 6 - with 0 arguments
        }
    }

    data.WriteBits(0, 3);
    data.WriteBit(!true);
    data.WriteBits(0, 20); // m_abilityCount

    for (uint8 i = 0; i < PET_BATTLE_OPPONENT_COUNT; ++i)
    {
        PBOID target = i == 0 ? PBOID::P0_PET_0 : PBOID::P1_PET_0;
        {
            data << uint8(target); // m_eventValue
        }

        data << uint8(target);
        data << uint8(PetBattleEvents::PET_SWAP);
    }

    data << uint8(2);

    for (MemberPair const& pair : m_members)
    {
        sMemberData const& member = pair.second;

        data << uint16(m_isPvp ? PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC : 0);
        data << uint8(member->GetFlags());
        data << uint8(member->GetTrapStatus());
    }

    data << uint32(m_round);

    player->SendDirectMessage(&data);
}

void BattlePet::Battle::SendPacket(WorldPacket* data)
{
    for (MemberPair const& pair : m_members)
    {
        sMemberData const& member = pair.second;
        if (Player* player = sObjectAccessor->FindPlayer(member->GetOwner()))
        {
            if (player->IsInWorld())
                player->SendDirectMessage(data);
        }
    }
}

void BattlePet::Battle::SetActivePet(sMemberData& member, PBOID pboid)
{
    if (pboid == member->GetActivePet()->GetPBOID())
        return;

    member->SetActivePet(pboid);

    if (m_event)
        m_event->Make(Event::EventInfo(PetBattleEvents::PET_SWAP, pboid, 0, 0), Event::EventValue(pboid));
}

void BattlePet::Battle::CreateBattle(Unit* initiator, Unit* opponent, bool start, bool isLfb)
{
    if (initiator == nullptr || opponent == nullptr)
        return;

    BattlePet::Battle* battle = new BattlePet::Battle(true, isLfb);
    if (!battle->AddMember(initiator, PBOID::PAD_0))
    {
        delete battle;
        return;
    }

    if (!battle->AddMember(opponent, PBOID::PAD_1))
    {
        delete battle;
        return;
    }

    if (start)
        battle->InitialAllMembers();

    sPetBattleMgr->AddBattle(battle, battle->GetId());
}

void BattlePet::Battle::InitialAllMembers()
{
    for (MemberPair const& pair : m_members)
    {
        sMemberData const& member = pair.second;
        if (Player* player = sObjectAccessor->FindPlayer(member->GetOwner()))
            SendInitialToPlayer(player);
    }
}

void BattlePet::Battle::SendNewRound()
{
    WorldPacket data(SMSG_PET_BATTLE_EVENT_2);

    for (MemberPair const& pair : m_members)
    {
        if (sMemberData const& member = pair.second)
        {
            data << uint8(member->GetFlags());
            data << uint8(member->GetTrapStatus());
            data << uint16(m_isPvp ? (m_roundTime / IN_MILLISECONDS) : 0);
        }
    }

    data << uint32(m_round);

    data.WriteBits(0, 20);
    data.WriteBits(m_state == PetBattleState::StateEnding, 3);

    data.WriteBit(!m_state);
    data.WriteBits(0, 22);

    if (m_state)
        data << uint8(m_state);
    if (m_state == PetBattleState::StateEnding)
        data << uint8(3);

    SendPacket(&data);
}

void BattlePet::Battle::SendEvents()
{
    if (!m_event)
        return;

    uint32 cooldowns = 0;
    for (MemberPair const& pair : m_members)
    {
        if (sMemberData const& member = pair.second)
        {
            for (PetPair const& ppair : member->m_pets)
            {
                if (Pet const* pet = ppair.second)
                    cooldowns += pet->GetCooldowns().size();
            }
        }
    }

    WorldPacket data(SMSG_PET_BATTLE_EVENT_2);

    for (MemberPair const& pair : m_members)
    {
        if (sMemberData const& member = pair.second)
        {
            data << uint8(member->GetFlags());
            data << uint8(member->GetTrapStatus());
            data << uint16(m_isPvp ? (m_roundTime / IN_MILLISECONDS) : 0);
        }
    }

    data << uint32(m_round);

    data.WriteBits(cooldowns, 20);
    data.WriteBits(0, 3); // died pets count

    for (uint32 i = 0; i < cooldowns; ++i)
    {
        data.WriteBit(!true); // PBOID of the cooldown owner - present always in data
    }

    data.WriteBit(!m_state);
    data.WriteBits(m_event->GetEvents().size(), 22);
    for (Event const* _event : m_event->GetEvents())
    {
        Event::EventInfo const& info = _event->m_info;

        data.WriteBit(!true); // _event->m_info.m_provider - should always present
        data.WriteBit(info.m_flag == 0);
        data.WriteBit(!true); // 6
        data.WriteBit(!true); // 7
        data.WriteBit(info.m_event == PetBattleEvents::SET_HEALTH);
        data.WriteBit(!false); // 5
        data.WriteBit(info.m_abilityEffectId == 0); // effect Id to play visual
        data.WriteBits(_event->m_values.size(), 25); // targets count

        for (Event::EventValue const& value : _event->m_values)
        {
            data.WriteBit(!true); // value.m_provider - should always present
            data.WriteBits(info.GetType(), 3);

            if (info.m_event == PetBattleEvents::SET_STATE) // 3
            {
                data.WriteBit(value.state.m_stateId == 0);
                data.WriteBit(value.state.m_value == 0);
            }
            else if (info.m_event == PetBattleEvents::AURA_APPLY
                || info.m_event == PetBattleEvents::AURA_CHANGE
                || info.m_event == PetBattleEvents::AURA_CANCEL) // 1
            {
                data.WriteBit(value.aura.m_amount == 0);
                data.WriteBit(value.aura.m_currentRound == 0);
                data.WriteBit(value.aura.m_abilityId == 0);
                data.WriteBit(value.aura.m_remaingCooldown == 0);
            }
            else if (info.m_event == PetBattleEvents::STATUS_CHANGE) // 4
            {
                data.WriteBit(value.status.m_status == 0);
            }
            else if (info.m_event == PetBattleEvents::SET_SPEED) // 4
            {
                data.WriteBit(value.speed.m_speed == 0);
            }
            else if (info.m_event == PetBattleEvents::SET_MAX_HEALTH) // 4
            {
                data.WriteBit(value.maxHealth.m_health == 0);
            }
            else if (info.m_event == PetBattleEvents::SET_HEALTH)
            {
                data.WriteBit(value.health.m_health == 0); // 5
            }
            else if (info.m_event == PetBattleEvents::TRIGGER_ABILTY)
            {
                data.WriteBit(value.trigger.m_abilityId == 0); // 0
            }
        }
    }

    /*m_event->GetEvents() - > GetEffects()*/
    for (Event const* _event : m_event->GetEvents())
    {
        Event::EventInfo const& info = _event->m_info; // effects

        for (Event::EventValue const& value : _event->m_values) // _event->m_values -> m_targets
        {
            if (info.GetType() != 4)
            {
                if (true)
                    data << uint8(value.m_target);
            }

            if (info.m_event == PetBattleEvents::SET_STATE) // 3
            {
                if (value.state.m_value)
                    data << int32(value.state.m_value);
                if (value.state.m_stateId)
                    data << int32(value.state.m_stateId);
            }
            else if (info.m_event == PetBattleEvents::AURA_APPLY
                || info.m_event == PetBattleEvents::AURA_CHANGE
                || info.m_event == PetBattleEvents::AURA_CANCEL) // 1
            {
                if (value.aura.m_currentRound)
                    data << int32(value.aura.m_currentRound);
                if (value.aura.m_amount)
                    data << int32(value.aura.m_amount);
                if (value.aura.m_remaingCooldown)
                    data << int32(value.aura.m_remaingCooldown);
                if (value.aura.m_abilityId)
                    data << int32(value.aura.m_abilityId);
            }
            else if (info.m_event == PetBattleEvents::STATUS_CHANGE) // 4
            {
                if (value.status.m_status)
                    data << int32(value.status.m_status);
            }
            else if (info.m_event == PetBattleEvents::SET_SPEED) // 4
            {
                if (value.speed.m_speed)
                    data << int32(value.speed.m_speed);
            }
            else if (info.m_event == PetBattleEvents::SET_HEALTH) // 4
            {
                if (value.health.m_health)
                    data << int32(value.health.m_health);
            }
            else if (info.m_event == PetBattleEvents::SET_MAX_HEALTH) // 4
            {
                if (value.maxHealth.m_health)
                    data << int32(value.maxHealth.m_health);
            }
            else if (info.m_event == PetBattleEvents::TRIGGER_ABILTY) // 0
            {
                if (value.trigger.m_abilityId)
                    data << int32(value.trigger.m_abilityId);
            }
            //PetBattleEvents -> PetBattleEffectType

            if (info.GetType() == 4)
            {
                if (true)
                    data << uint8(value.m_target);
            }
        }
        if (info.m_flag != 0) 
            data << uint16(info.m_flag);
        if (true)
            data << uint8(info.m_provider);
        if (info.m_event)
            data << uint8(info.m_event);
        if (info.m_abilityEffectId != 0)
            data << uint32(info.m_abilityEffectId);
        if (true) // 7
            data << uint16(1);
        if (true) // 6
            data << uint8(1);
    }

    for (MemberPair const& pair : m_members)
    {
        if (sMemberData const& member = pair.second)
        {
            for (PetPair const& ppair : member->m_pets)
            {
                if (Pet const* pet = ppair.second)
                {
                    for (auto const& kvp : pet->GetCooldowns())
                    {
                        data << uint16(0); // lockdown remaing
                        data << uint16(kvp.second); // cooldown remaing
                        data << uint32(kvp.first); // ability
                        if (true)
                            data << uint8(pet->GetPBOID());

                        data << uint8(pet->GetAbilityIndex(kvp.first));
                    }
                }
            }
        }
    }

    if (m_state)
        data << uint8(m_state);
    if (false /*died pets count*/)
        data << uint8(3);

    SendPacket(&data);

    m_event->Clear();
}

void BattlePet::Battle::SendSwapReq(PBOID diedPet, PBOID side)
{
    /*WorldPacket data(SMSG_PET_BATTLE_SWAP_REQ);
    data.WriteBits(1, 22);
    //for (uint8 i = 0; i < 1; ++i)
    {
        data.WriteBit(!false); // bit6
        data.WriteBit(!false); // effectId
        data.WriteBit(!false); // bit7
        data.WriteBit(!false); // bit5
        data.WriteBit(!PetBattleEvents::PET_SWAP); // m_eventId
        data.WriteBit(!false); // bit3
        data.WriteBit(!true);                      // m_eventTargetType

        data.WriteBits(1, 25);
        {
            data.WriteBits(6, 3); // 6 - with 0 arguments
            data.WriteBit(!true); // m_hasEventValue
        }
    }

    data.WriteBits(0, 3);
    data.WriteBits(0, 20); // m_abilityCount
    data.WriteBit(!true);

    data << uint8(m_state);

    //for (uint8 i = 0; i < 1; ++i)
    {
        //PBOID target = i == 0 ? PBOID::P0_PET_0 : PBOID::P1_PET_0;
        {
            data << uint8(diedPet); // m_eventValue
        }

        data << uint8(diedPet);
        data << uint8(PetBattleEvents::PET_SWAP);
    }

    for (MemberPair const& pair : m_members)
    {
        MemberData const * member = pair.second;

        uint8 flags = 0;
        if (member->SwapReq())
            flags = BattlePetDisableFlags::Pass | BattlePetDisableFlags::Ability;
        if (member->HasMoved() || !member->CanMove())
            flags |= BattlePetDisableFlags::Pass | BattlePetDisableFlags::Ability | BattlePetDisableFlags::Swap;

        data << uint8(0);
        data << uint16(m_isPvp ? PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC : 0);
        data << uint8(member->GetTrapStatus());
    }

    data << uint32(m_round);

    if (Player* player = sObjectAccessor->FindPlayer(m_members[side]->GetOwner()))
        player->SendDirectMessage(&data);*/
}

void BattlePet::Battle::HandleAction(Unit* unit, ActionInfo const* action)
{
    if (!unit || !action)
        return;

    if (m_state == PetBattleState::StateEnding)
        return;

    if (action->m_action >= BATTLE_PET_ACTION_MAX)
        return;

    sMemberData member = GetMember(unit->GetGUID());
    if (!(*this.*Actions[action->m_action])(action, member))
        return;

    OnActionPerformed(member->m_side, (BattlePetActionType)action->m_action);
}

void BattlePet::Battle::InitialBattle()
{
    if (IsStarted())
    {
        sLog->OutTrinity("BattlePet::Battle:StartBattle error! Trying to start battle, but battle already started");
        return;
    }

    UpdateFlags();

    for (MemberPair const& pair : m_members)
    {
        if (sMemberData const& member = pair.second)
        {
            if (Player* player = sObjectAccessor->FindPlayer(member->GetOwner()))
                SendInitialPet(player);
        }
    }

    m_round = 1;
    m_state = PetBattleState::StateBeginning;
    m_roundTime = PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC * IN_MILLISECONDS;
}

void BattlePet::Battle::InitialMember(Player* player)
{
    if (IsStarted() || IsEnded())
        return;

    if (!player || !player->GetSession())
        return; 

    if (player->GetSession()->PlayerLogout())
        return;

    sMemberData member = GetMember(player->GetGUID());
    if (member->m_initialized)
        return; // already initialized

    member->m_initialized = true;
    sMemberData const& opponent = member->GetOpponent();
    if (opponent->m_initialized)
        InitialBattle();
}

void BattlePet::Battle::SendInitialToPlayer(Player* player)
{
    if (IsStarted() || IsEnded())
        return;

    if (!player || !player->GetSession())
        return;

    if (player->GetSession()->PlayerLogout())
        return;

    SendInitialPetPos(player);
    SendInitialBattle(player);
}

void BattlePet::Battle::Update(uint32 const diff)
{
    if (!IsStarted())
        return;

    if (m_state == PetBattleState::StateEnding)
        return;

    m_roundTime -= diff;
    if (m_roundTime <= 0)
    {
        EndRound();
    }
}

void BattlePet::Battle::StartRound()
{
    if (!IsStarted())
        return;

    SendEvents(); // отправляем инфу о смене раунда

    UpdateFlags(); // определяем кто ходит первый
    SendNewRound();

    TryEndRound(); // это надо на случай если 2 пета имеют multi-turn абилки.
}

void BattlePet::Battle::EndRound()
{
    for (auto& pair : m_members)
    {
        if (sMemberData& member = pair.second)
            member->Update();
    }

    for (auto& pair : m_members)
    {
        sMemberData& data = pair.second;
        if (!data || !data->SwapReq())
            continue;

        for (PetPair const& ppair : data->m_pets)
        {
            Pet* temp = ppair.second;
            if (temp->IsDead())
                continue;

            SetActivePet(data, temp->GetPBOID());
            SendEvents();
            break;
        }
    }

    m_round += 1;
    m_roundTime = PET_BATTLE_WAIT_FOR_FRONT_PET_MAX_SEC * IN_MILLISECONDS;

    StartRound();
}

void BattlePet::PositionData::Calculate()
{
    /*TODO: distance between 2 players must equal 10*/

    G3D::Vector3 minimum = m_positions[0].min(m_positions[1]);
    G3D::Vector3 maximum = m_positions[0].max(m_positions[1]);

    m_center = m_positions[0].min(m_positions[1]);
    m_center += (maximum - minimum) / 2.0f;

    G3D::Vector3 const& one = m_positions[0];
    G3D::Vector3 const& second = m_positions[1];

    float dx = second.x - one.x;
    float dy = second.y - one.y;

    float angle = atan2(dy, dx);
    m_orientation = (angle >= 0) ? angle : 2 * M_PI + angle;
}

void BattlePet::Battle::UpdateFlags()
{
    PBOID side = GetInitialSide();

    m_movedMask = 0;
    m_canMoveMask = 1 << side;

    for (auto& pair : m_members)
    {
        if (sMemberData const& member = pair.second)
        {
            /*Все работает, но одно но - т.е multi turn абилки храняться не у кастера, а у цели, то
            нам надо проверять наличие такой абилки и еще смотреть кто ее владелец.
            
            Можно в конструкторе ставить кастеру m_hasActiveMultiTurnAbility = true
            а в деструкторе false*/
            if (member->GetActivePet()->HasActiveMultiturnAbility())
            {
                m_movedMask |= 1 << member->m_side;
            }
        }
    }
}

void BattlePet::Battle::OnActionPerformed(PBOID side, BattlePetActionType action)
{
    SetMoved(side, true);
    SetCanMove(side, false);

    PBOID opponent = side == PBOID::PAD_0 ? PBOID::PAD_1 : PBOID::PAD_0;
    if (!HasMoved(opponent))
        SetCanMove(opponent, true);

    SendEvents();

    TryEndRound();
}

void BattlePet::Battle::TryEndRound()
{
    uint8 count = 0;
    for (uint8 pad = PBOID::PAD_0; pad <= PBOID::PAD_1; ++pad)
    {
        if (HasMoved(PBOID(pad)))
            ++count;
    }

    if (count != PET_BATTLE_OPPONENT_COUNT)
        return;

    EndRound();
}

bool BattlePet::Battle::ActionForfeit(ActionInfo const* action, BattlePet::sMemberData member)
{
    EndBattle((member && member->IsInitiator()) ? PBOID::PAD_1 : PBOID::PAD_0, PetBattleEndReason::Forfeit);
    return false;
}

bool BattlePet::Battle::ActionUseAbility(ActionInfo const* action, BattlePet::sMemberData member)
{
    if (!member || !member->GetOpponent())
        return false;

    if (!CanMove(member->m_side) || HasMoved(member->m_side))
        return false; // TODO: cheater ?

    if (action->m_abilityId == 0)
        return false;

    member->GetActivePet()->CastAbility(action->m_abilityId);
    return true;
}

bool BattlePet::Battle::ActionSwapPet(ActionInfo const* action, BattlePet::sMemberData member)
{
    if (!CanMove(member->m_side) || HasMoved(member->m_side))
        return false; // TODO: cheater ?

    uint32 index = action->m_petIndex;

    ASSERT(index < member->m_pets.size());

    PBOID pboid = member->IsInitiator() ? PBOID(P0_PET_0 + index) : PBOID(P1_PET_0 + index);
    if (pboid != member->GetActivePet()->GetPBOID())
        SetActivePet(member, pboid);
    //else
        // Pass round

    return true;
}

bool BattlePet::Battle::ActionUseTrap(ActionInfo const* action, BattlePet::sMemberData member)
{
    //TODO: not implemented
    return true;
}

bool BattlePet::Battle::ActionEndBattle(ActionInfo const* action, BattlePet::sMemberData member)
{
    return false;
}

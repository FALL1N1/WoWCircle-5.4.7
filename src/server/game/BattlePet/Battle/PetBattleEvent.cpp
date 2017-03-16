#include "PetBattleEvent.h"
#include "BattlePet.h"

BattlePet::EventHolder::EventHolder()
{

}

BattlePet::EventHolder::~EventHolder()
{
    Clear();
}

BattlePet::Event::EventValue BattlePet::Event::Create(PBOID target, int32 raw0, int32 raw1, int32 raw2, int32 raw3)
{
    EventValue value(target);
    value.raw.m_raw0 = raw0;
    value.raw.m_raw1 = raw1;
    value.raw.m_raw2 = raw2;
    value.raw.m_raw3 = raw3;

    return value;
}

void BattlePet::Event::Add(BattlePet::Event::EventValue value)
{
    m_values.push_back(value);
}

void BattlePet::EventHolder::AddEvent(BattlePet::Event const* _event)
{
    ASSERT(_event != nullptr);

    m_events.push_back(_event);
}

void BattlePet::EventHolder::Clear()
{
    for (Event const* _event : m_events)
    {
        delete _event;
    }

    m_events.clear();
}

BattlePet::Event* BattlePet::EventHolder::Make(BattlePet::Event::EventInfo info)
{
    Event* _event = new Event(info);
    AddEvent(_event);
    return _event;
}

BattlePet::Event* BattlePet::EventHolder::Make(BattlePet::Event::EventInfo info, BattlePet::Event::EventValue value)
{
    Event* _event = new Event(info, value);
    AddEvent(_event);
    return _event;
}

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "the_veiled_stair.h"
#include "GameObjectAI.h"

// 64616 - Wrathion
class npc_wrathion : public CreatureScript
{
    public:
        npc_wrathion() : CreatureScript("npc_wrathion")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* /*p_Creature*/, Quest const* p_Quest)
        {
            switch (p_Quest->GetQuestId())
            {
                case VeiledStairQuests::QuestALegendInTheMaking:
                {
                    switch (p_Player->GetTeamId())
                    {
                        case TEAM_ALLIANCE:
                            p_Player->PlayScene(208, p_Player);
                            p_Player->KilledMonsterCredit(VeiledStairCreatures::CreditALegendInTheMaking);
                            break;
                        case TEAM_HORDE:
                            p_Player->PlayScene(234, p_Player);
                            p_Player->KilledMonsterCredit(VeiledStairCreatures::CreditALegendInTheMaking);
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case VeiledStairQuests::QuestFearItself:
                    /// Talk(10);
                    break;
                case VeiledStairQuests::QuestIncoming: ///< @TODO : set autocomp
                    /// Talk(11); ///< "We are just getting started, friend ! Our greatest challenges lie ahead."
                    /// Talk(12); ///< "But when war comes to these shores, you and I will be ready for it !"
                    break;
                case VeiledStairQuests::QuestMeasureOfALeaderAlly:
                    p_Player->PlayScene(315, p_Player);
                    p_Player->KilledMonsterCredit(68316);
                    break;
                case VeiledStairQuests::QuestMeasureOfALeaderHorde:
                    p_Player->PlayScene(319, p_Player);
                    p_Player->KilledMonsterCredit(68316);
                    break;
                case VeiledStairQuests::QuestCallOfThePackmaster:
                    p_Player->PlayScene(320, p_Player);
                    p_Player->KilledMonsterCredit(69053);
                    break;
                case VeiledStairQuests::QuestTheSoulOfTheHorde:
                    p_Player->PlayScene(321, p_Player);
                    p_Player->KilledMonsterCredit(69053);
                    break;
                default:
                    break;
            }
            return true;
        }
};

// 69782 - Wrathion
class npc_wrathion_tavern_upstairs : public CreatureScript
{
    public:
        npc_wrathion_tavern_upstairs() : CreatureScript("npc_wrathion_tavern_upstairs")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* /*p_Creature*/, Quest const* p_Quest)
        {
            switch (p_Quest->GetQuestId())
            {
                case VeiledStairQuests::QuestTheCrownOfHeaven:
                    p_Player->PlayScene(416, p_Player);
                    break;
                default:
                    break;
            }
            return true;
        }
};

// 64822 - Wrathion
class npc_wrathion_mason : public CreatureScript
{
    public:
        npc_wrathion_mason() : CreatureScript("npc_wrathion_mason")
        {
        }

        bool OnQuestReward(Player* p_Player, Creature* p_Creature, Quest const* p_Quest, uint32 p_Option)
        {
            if (p_Quest->GetQuestId() == 31482)
            {
                if (p_Creature->isQuestGiver() && p_Creature->AI())
                {
                    p_Creature->DespawnOrUnsummon();

                    if (AchievementEntry const* l_Achievement = sAchievementStore.LookupEntry(7533)) ///< Chapter I: Trial of the Black Prince
                        p_Player->CompletedAchievement(l_Achievement);
                }
            }
            return true;
        }

        struct npc_wrathion_masonAI : public npc_escortAI
        {
            npc_wrathion_masonAI(Creature* creature) : npc_escortAI(creature)
            {
                m_PlayerGuid = 0;
            }

            EventMap m_Events;
            uint64 m_PlayerGuid;

            void Reset()
            {
                m_Events.Reset();
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                if (Player* p_Player = p_Summoner->ToPlayer())
                {
                    if (p_Player->GetQuestStatus(31482) == QUEST_STATUS_INCOMPLETE)
                    {
                        SetDespawnAtFar(false);
                        SetDespawnAtEnd(false);
                        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                        m_Events.ScheduleEvent(VeiledStairEvents::EventFirstMove, 2000);
                        m_PlayerGuid = p_Player->GetGUID();
                    }
                }
            }

            void WaypointReached(uint32 p_WaypointId)
            {
                switch (p_WaypointId)
                {
                    case 1:
                        Talk(1);
                        break;
                    case 3:
                        SetEscortPaused(true);
                        me->SetFacingTo(4.719869f);
                        me->SetOrientation(4.719869f);
                        Talk(2);
                        m_Events.ScheduleEvent(VeiledStairEvents::EventThirdTalk, 5000);
                        m_Events.ScheduleEvent(VeiledStairEvents::EventSecondMove, 10000);
                        break;
                    case 4:
                        me->SetFacingTo(5.442434f);
                        me->SetOrientation(5.442434f);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                npc_escortAI::UpdateAI(p_Diff);
                m_Events.Update(p_Diff);

                if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                {
                    switch (m_Events.ExecuteEvent())
                    {
                        case VeiledStairEvents::EventFirstMove:
                            Talk(0);
                            Start(false, false, l_Player->GetGUID());
                            break;
                        case VeiledStairEvents::EventThirdTalk:
                            Talk(3);
                            m_Events.ScheduleEvent(VeiledStairEvents::EventPlayScene, 4000);
                            m_Events.ScheduleEvent(VeiledStairEvents::EventFourthTalk, 7000);
                            break;
                        case VeiledStairEvents::EventSecondMove:
                            SetEscortPaused(false);
                            break;
                        case VeiledStairEvents::EventPlayScene:
                            l_Player->PlayScene(245, l_Player);
                            break;
                        case VeiledStairEvents::EventFourthTalk:
                            Talk(4);
                            m_Events.ScheduleEvent(VeiledStairEvents::EventFifthTalk, 6000);
                            break;
                        case VeiledStairEvents::EventFifthTalk:
                            Talk(5);
                            m_Events.ScheduleEvent(VeiledStairEvents::EventSixthTalk, 5000);
                            break;
                        case VeiledStairEvents::EventSixthTalk:
                            Talk(6);
                            m_Events.ScheduleEvent(VeiledStairEvents::EventSeventhTalk, 10000);
                            break;
                        case VeiledStairEvents::EventSeventhTalk:
                            Talk(7);
                            m_Events.ScheduleEvent(VeiledStairEvents::EventEigthTalk, 8000);
                            break;
                        case VeiledStairEvents::EventEigthTalk:
                            Talk(8);
                            m_Events.ScheduleEvent(VeiledStairEvents::EventFinal, 10000);
                            break;
                        case VeiledStairEvents::EventFinal:
                            l_Player->KilledMonsterCredit(64664);
                            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                            break;
                        default:
                            break;
                    }
                }
            }
        };
        
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wrathion_masonAI(creature);
        }
};

/// Wrathion / 32595
class npc_wrathion_masons_foly : public CreatureScript
{
    public:
        npc_wrathion_masons_foly() : CreatureScript("npc_wrathion_masons_foly")
        {
        }

        bool OnQuestReward(Player* p_Player, Creature* p_Creature, Quest const* p_Quest, uint32 p_Option)
        {
            if (p_Quest->GetQuestId() == 32595)
                p_Creature->DespawnOrUnsummon();

            return true;
        }
};

/// Chest - 216837
/// Signal Fire - 216274
class gob_wrathion_chest : public GameObjectScript
{
    public:
        gob_wrathion_chest() : GameObjectScript("gob_wrathion_chest") { }

        bool OnQuestReward(Player* p_Player, GameObject* p_Gob, Quest const* p_Quest, uint32 p_Option)
        {
            uint32 l_QuestId = p_Quest->GetQuestId();

            switch (l_QuestId)
            {
                case VeiledStairQuests::QuestCallOfThePackmaster:
                    if (AchievementEntry const* l_Achievement = sAchievementStore.LookupEntry(7534)) ///< Chapter II: Wrathion's War
                        p_Player->CompletedAchievement(l_Achievement);
                    break;
                case VeiledStairQuests::QuestTheSoulOfTheHorde:
                    if (AchievementEntry const* l_Achievement = sAchievementStore.LookupEntry(8008)) ///< Chapter II: Wrathion's War
                        p_Player->CompletedAchievement(l_Achievement);
                    break;
                default:
                    break;
            }

            return false;
        }
};

void AddSC_the_veiled_stair()
{
    new npc_wrathion();
    new npc_wrathion_tavern_upstairs();
    new npc_wrathion_mason();
    new npc_wrathion_masons_foly();
    new gob_wrathion_chest();
}
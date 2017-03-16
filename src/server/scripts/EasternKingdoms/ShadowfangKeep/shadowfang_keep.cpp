#include "ScriptPCH.h"
#include "LFGMgr.h"
#include "Group.h"
#include "shadowfang_keep.h"
#include "LFGMgr.h"

enum ScriptedTexts
{
    SAY_AGGRO_1                      = 0, // Did they bother to tell you who I am and why Iam doing this?
    SAY_AGGRO_2                      = 1, // ...or are they just using you like they do everybody else?
    SAY_AGGRO_3                      = 2, // But what does it matter. It is time for this to end.
    SAY_CALL_BAXTER                  = 3, // Baxter! Get in here and help! NOW!
    SAY_CALL_FRYE                    = 4, // It is time, Frye! Attack!
    SAY_DEATH_HUMMEL                 = 6, // ... please don't think less of me.
    SAY_DEATH_BAXTER                 = 0, // It has been the greatest honor of my life to server with you, Hummel.
    SAY_DEATH_FRYE                   = 0, // Great. We're nto gutless, we're incompetent.
};

enum Spells
{
    // hummel
    SPELL_ALLURING_PERFUME           = 68589, // aura
    SPELL_ALLURING_PERFUME_DMG       = 68641,
    SPELL_ALLURING_PERFUME_SPRAY     = 68607,

    // baxter
    SPELL_IRRESISTIBLE_COLOGNE       = 68946, // aura
    SPELL_IRRESISTIBLE_COLOGNE_DMG   = 68947,
    SPELL_IRRESISTIBLE_COLOGNE_SPRAY = 68948,

    // both
    SPELL_CHAIN_REACTION             = 68821,

    SPELL_COLOGNE_IMMUNE             = 68530,
    SPELL_PARFUME_IMMUNE             = 68529,

    SPELL_TABLE_APPEAR               = 69216,
    SPELL_SUMMON_TABLE               = 69218,
    
    SPELL_UNSTABLE_REACTION          = 68957,

    // frye
    SPELL_THROW_PERFUME              = 68799,
    SPELL_THROW_COLOGNE              = 68841,
    SPELL_ALLURING_PERFUME_SPILL     = 68798,
    SPELL_IRRESISTIBLE_COLOGNE_SPILL = 68614,
};

enum Events
{
    EVENT_SPRAY = 1,
    EVENT_CHAIN_REACTION,
    EVENT_THROW,

    EVENT_INTRO_1,
    EVENT_INTRO_2,
    EVENT_INTRO_3,
    EVENT_START_FIGHT,
    EVENT_START_BAXTER,
    EVENT_START_FRYE,
};

enum Action
{
    ACTION_START_INTRO  = 1,
    ACTION_APOTHECARY_DIED,
};

enum Phase
{
    PHASE_NORMAL,
    PHASE_INTRO
};

const Position Loc[]=
{
    // spawn points
    {-215.776443f, 2242.365479f, 79.769257f, 0.0f},
    {-169.500702f, 2219.286377f, 80.613045f, 0.0f},
    {-200.056641f, 2152.635010f, 79.763107f, 0.0f},
    {-238.448242f, 2165.165283f, 89.582985f, 0.0f},
    // moveto points
    {-210.784164f, 2219.004150f, 79.761803f, 0.0f},
    {-198.453400f, 2208.410889f, 79.762474f, 0.0f},
    {-208.469910f, 2167.971924f, 79.764969f, 0.0f},
    {-228.251511f, 2187.282471f, 79.762840f, 0.0f}
};

class npc_apothecary_hummel : public CreatureScript
{
    public:
        npc_apothecary_hummel() : CreatureScript("npc_apothecary_hummel") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_apothecary_hummelAI (pCreature);
        }

        bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
        {
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            pPlayer->PlayerTalkClass->ClearMenus();
            pPlayer->CLOSE_GOSSIP_MENU();

            pCreature->AI()->DoAction(ACTION_START_INTRO);
            return false;
        }

        struct npc_apothecary_hummelAI : public ScriptedAI
        {
            npc_apothecary_hummelAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = me->GetInstanceScript();
                isFirst = true;
            }

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();

                me->RestoreFaction();
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                RespawnBosses();

                isIntro = false;
                isEnd = false;
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_START_INTRO)
                {
                    isIntro = true;
                    events.ScheduleEvent(EVENT_INTRO_1, 1000);
                }
                else if (action == ACTION_APOTHECARY_DIED)
                {
                    if (IsAllBossesDied())
                    {
                        EndEncounter();
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                me->AddAura(SPELL_ALLURING_PERFUME, me);

                events.ScheduleEvent(EVENT_SPRAY, urand(8000, 15000));
                events.ScheduleEvent(EVENT_CHAIN_REACTION, urand(15000, 25000));
                events.ScheduleEvent(EVENT_START_BAXTER, 8000);
                events.ScheduleEvent(EVENT_START_FRYE, 16000);
            }

            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH_HUMMEL);

                events.Reset();
                summons.DespawnAll();

                DoAction(ACTION_APOTHECARY_DIED);

                if (!isFirst)
                    me->SetLootRecipient(NULL);

                isFirst = false;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() && !isIntro)
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_1:
                            Talk(SAY_AGGRO_1);
                            events.ScheduleEvent(EVENT_INTRO_2, 4000);
                            break;
                        case EVENT_INTRO_2:
                            Talk(SAY_AGGRO_2);
                            events.ScheduleEvent(EVENT_INTRO_3, 5500);
                            break;
                        case EVENT_INTRO_3:
                            Talk(SAY_AGGRO_3);
                            events.ScheduleEvent(EVENT_START_FIGHT, 2000);
                            break;
                        case EVENT_START_FIGHT:
                            isIntro = false;
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->setFaction(16);
                            break;
                        case EVENT_START_BAXTER:
                            Talk(SAY_CALL_BAXTER);
                            if (Creature* pBaxter = me->GetCreature(*me, pInstance->GetData64(DATA_BAXTER)))
                            {
                                pBaxter->setFaction(16);
                                DoZoneInCombat(pBaxter);
                            }
                            break;
                        case EVENT_START_FRYE:
                            Talk(SAY_CALL_FRYE);
                            if (Creature* pFrye = me->GetCreature(*me, pInstance->GetData64(DATA_FRYE)))
                            {
                                pFrye->setFaction(16);
                                DoZoneInCombat(pFrye);
                            }
                            break;
                        case EVENT_SPRAY:
                            DoCastVictim(SPELL_ALLURING_PERFUME_SPRAY);
                            events.ScheduleEvent(EVENT_SPRAY, urand(15000, 25000));
                            break;
                        case EVENT_CHAIN_REACTION:
                            DoCastAOE(SPELL_CHAIN_REACTION);
                            events.ScheduleEvent(EVENT_CHAIN_REACTION, urand(15000, 25000));
                            break;  
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;
            bool isIntro;
            bool isEnd;
            bool isFirst;

        private:

            void RespawnBosses()
            {
                if (!pInstance)
                    return;

                if (Creature* pBaxter = me->GetCreature(*me, pInstance->GetData64(DATA_BAXTER)))
                {
                    if (pBaxter->isDead())
                        pBaxter->Respawn();
                }

                if (Creature* pFrye = me->GetCreature(*me, pInstance->GetData64(DATA_FRYE)))
                {
                    if (pFrye->isDead())
                        pFrye->Respawn();
                }
            }

            bool IsAllBossesDied()
            {
                if (!me->isDead())
                    return false;

                if (Creature* pBaxter = me->GetCreature(*me, pInstance->GetData64(DATA_BAXTER)))
                {
                    if (!pBaxter->isDead())
                        return false;
                }

                if (Creature* pFrye = me->GetCreature(*me, pInstance->GetData64(DATA_FRYE)))
                {
                    if (!pFrye->isDead())
                        return false;
                }

                return true;
            }

            void EndEncounter()
            {
                if (isEnd)
                    return;

                if (pInstance)
                {
                    Map::PlayerList const& players = pInstance->instance->GetPlayers();
                    if (!players.isEmpty())
                        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                            if (Player* player = i->getSource())
                                if (player->IsAtGroupRewardDistance(me))
                                    sLFGMgr->RewardDungeonDoneFor(288, player);
                }

                isEnd = true;
            }

        };
};

class npc_apothecary_baxter : public CreatureScript
{
    public:
        npc_apothecary_baxter() : CreatureScript("npc_apothecary_baxter") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_apothecary_baxterAI (pCreature);
        }

        struct npc_apothecary_baxterAI : public ScriptedAI
        {
            npc_apothecary_baxterAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();

                me->RestoreFaction();

                RespawnBosses();
            }

            void EnterCombat(Unit* who)
            {
                me->AddAura(SPELL_IRRESISTIBLE_COLOGNE, me);

                events.ScheduleEvent(EVENT_SPRAY, urand(8000, 15000));
                events.ScheduleEvent(EVENT_CHAIN_REACTION, urand(15000, 25000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();
                summons.DespawnAll();

                Talk(SAY_DEATH_BAXTER);

                if (Creature* pHummel = me->GetCreature(*me, pInstance->GetData64(DATA_HUMMEL)))
                    pHummel->AI()->DoAction(ACTION_APOTHECARY_DIED);
            }

        private:

            InstanceScript* pInstance;

        private:

            void RespawnBosses()
            {
                if (!pInstance)
                    return;

                if (Creature* pHummel = me->GetCreature(*me, pInstance->GetData64(DATA_HUMMEL)))
                {
                    if (pHummel->isDead())
                        pHummel->Respawn();
                }

                if (Creature* pFrye = me->GetCreature(*me, pInstance->GetData64(DATA_FRYE)))
                {
                    if (pFrye->isDead())
                        pFrye->Respawn();
                }
            }
        };
};

class npc_apothecary_frye : public CreatureScript
{
    public:
        npc_apothecary_frye() : CreatureScript("npc_apothecary_frye") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_apothecary_fryeAI (pCreature);
        }

        struct npc_apothecary_fryeAI : public ScriptedAI
        {
            npc_apothecary_fryeAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();

                me->RestoreFaction();

                RespawnBosses();
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();
                summons.DespawnAll();

                Talk(SAY_DEATH_FRYE);

                if (Creature* pHummel = me->GetCreature(*me, pInstance->GetData64(DATA_HUMMEL)))
                    pHummel->AI()->DoAction(ACTION_APOTHECARY_DIED);
            }

        private:

            InstanceScript* pInstance;

        private:

            void RespawnBosses()
            {
                if (!pInstance)
                    return;

                if (Creature* pHummel = me->GetCreature(*me, pInstance->GetData64(DATA_HUMMEL)))
                {
                    if (pHummel->isDead())
                        pHummel->Respawn();
                }

                if (Creature* pBaxter = me->GetCreature(*me, pInstance->GetData64(DATA_BAXTER)))
                {
                    if (pBaxter->isDead())
                        pBaxter->Respawn();
                }
            }

        };
};

void AddSC_shadowfang_keep()
{
    new npc_apothecary_hummel();
    new npc_apothecary_baxter();
    new npc_apothecary_frye();
}

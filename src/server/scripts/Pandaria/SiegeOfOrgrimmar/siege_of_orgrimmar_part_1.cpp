#include "siege_of_orgrimmar.hpp"
#include "GameObjectAI.h"
// 143948
// 149189
// 140708

enum ScriptedTexts
{
    SAY_CHO_INTRO_1 = 0,
    SAY_CHO_INTRO_2 = 1,
    SAY_CHO_INTRO_3 = 2,
    SAY_CHO_INTRO_4 = 3,
    SAY_CHO_INTRO_5 = 4,
    SAY_CHO_INTRO_6 = 5,
};

enum Spells
{
    SPELL_COSMETIC_CHANNEL_1        = 147449, // on target, blue beam
    SPELL_COSMETIC_CHANNEL_2        = 147392, // on caster, big blue pool

    SPELL_SPAWN                     = 146920,
    SPELL_OOZE_SPAWN_MISSILE        = 146944,
    SPELL_OOZE_SPAWN_DUMMY          = 147166,
    SPELL_OOZE_SPAWN_1              = 146949, // summons sha puddle
    SPELL_OOZE_SPAWN_2              = 147141, // summons contaminated puddle

    SPELL_LESSER_SHA_RESIDUE        = 147083,
    SPELL_LESSER_PURIFIED_RESIDUE   = 147155,

    SPELL_BOOBLE_SHIELD_1           = 147333,

    SPELL_BOOBLE_SHIELD_2           = 147450,
    SPELL_CORRUPTED_WATER           = 147351,
    SPELL_WATER_BOLT                = 147398,

    SPELL_RUSHING_WATERS            = 147185,
    SPELL_RUSHING_WATERS_DMG_1      = 149164, // from creature
    SPELL_RUSHING_WATERS_DMG_2      = 147213, // periodic from areatriggers
    SPELL_SWIRL_ZONE                = 147184,
    SPELL_SWIRL_ZONE_NE             = 147181,
    SPELL_SWIRL_ZONE_N              = 147178,
    SPELL_SWIRL_ZONE_NW             = 147182,
    SPELL_SWIRL_ZONE_SW             = 147191,
    SPELL_SWIRL_ZONE_SE             = 147189,

    // Fragment of Pride
    SPELL_GROWING_PRIDE             = 145818,

    // Amalgamated Hubris
    SPELL_SHA_NOVA                  = 145989,

    // Vanity
    SPELL_SELF_ABSORBED             = 145861,

    // Arrogance
    SPELL_OVERCONFIDANCE            = 145893,
    SPELL_OVERCONFIDANCE_2          = 145894, // removes 145940, forces 145893
    SPELL_OVERCONFIDANCE_FORCE      = 145895, // triggers 146009
    SPELL_OVERCONFIDANCE_REMOVE     = 145897, // from all in 50000 yards
    SPELL_GROWING_OVERCONFIDANCE    = 145940,
    SPELL_IMPENDING_OVERCONFIDANCE  = 146009, // triggered by 145895
    SPELL_OVERCONFIDANCE_DMG        = 145891,

    // Zeal
    SPELL_ARROGANCE                 = 145941,
    SPELL_VANITY                    = 145942,
    SPELL_SHA_SPLASH                = 145944,
};

enum Events
{
    EVENT_BOOBLE_SHIELD = 1,
    EVENT_CORRUPTED_WATER,
    EVENT_WATER_BOLT,
    EVENT_RUSHING_WATERS,

    EVENT_CHO_TALK_INTRO_1,
    EVENT_CHO_TALK_INTRO_2,
    EVENT_CHO_TALK_INTRO_3,
    EVENT_CHO_TALK_INTRO_4,
    EVENT_CHO_TALK_INTRO_5,
    EVENT_CHO_TALK_INTRO_6,

    // Fragment of Pride
    EVENT_GROWING_PRIDE,

    // Amalgamated Hubris
    EVENT_SHA_NOVA,

    // Vanity
    EVENT_SELF_ABSORBED,

    // Zeal
    EVENT_SHA_SPLASH,

};

enum Adds
{
    NPC_OOZE_CONTROLLER                 = 73222,

    NPC_LESSER_SHA_PUDDLE_1             = 73226, // summoned by spell 146949
    NPC_LESSER_SHA_PUDDLE_2             = 73197,
    NPC_LESSER_CONTAMINATED_PUDDLE_1    = 73283, // summoned by spell 147141
    NPC_LESSER_CONTAMINATED_PUDDLE_2    = 73260,

    NPC_TORMENTED_INITIATE              = 73349,
    NPC_FALLEN_POOL_TENDER              = 73342,
    NPC_AQUEUOS_DEFENDER                = 73191,

    NPC_FRAGMENT_OF_PRIDE               = 72655,
    NPC_AMALGAMATED_HUBRIS              = 72658,
    NPC_SHA_INFUSED_DEFENDER            = 72788,
};

enum Actions
{
    ACTION_CHO_TALK_INTRO   = 1,
    ACTION_CHO_TALK_INTRO_IMMERSEUS,
    ACTION_CHO_TALK_OUTRO_IMMERSEUS,
};

class npc_siege_of_orgrimmar_ooze_controller : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_ooze_controller() : CreatureScript("npc_siege_of_orgrimmar_ooze_controller") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_ooze_controllerAI(creature);
        }

        struct npc_siege_of_orgrimmar_ooze_controllerAI : public Scripted_NoMovementAI
        {
            npc_siege_of_orgrimmar_ooze_controllerAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            
                me->setActive(true);
            }

            void Reset()
            {
                std::list<Creature*> creatures;
                me->GetCreatureListWithEntryInGrid(creatures, NPC_TORMENTED_INITIATE, 100.0f);

                for (std::list<Creature*>::const_iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* creature = (*itr);

                    if (creature->GetPositionZ() > 250.0f)
                    {
                        creature->CastSpell(me, SPELL_COSMETIC_CHANNEL_1);
                    }
                }

                DoCast(me, SPELL_COSMETIC_CHANNEL_2);
            }
        };
};

class npc_siege_of_orgrimmar_tormented_initiate : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_tormented_initiate() : CreatureScript("npc_siege_of_orgrimmar_tormented_initiate") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_tormented_initiateAI(creature);
        }

        struct npc_siege_of_orgrimmar_tormented_initiateAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_tormented_initiateAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);

                JustReachedHome();
            }

            void Reset()
            {
                events.Reset();
            }

            void JustReachedHome()
            {
                if (me->GetPositionZ() > 250.0f)
                {
                    if (Creature* pController = me->FindNearestCreature(NPC_OOZE_CONTROLLER, 80.0f))
                    {
                        DoCast(pController, SPELL_COSMETIC_CHANNEL_1);
                    }
                }
                else
                {
                    if (Creature* pFallenPoolTender = me->FindNearestCreature(NPC_FALLEN_POOL_TENDER, 25.0f))
                        DoCast(pFallenPoolTender, SPELL_COSMETIC_CHANNEL_1);
                }
            }

            void EnterCombat(Unit* who)
            {
                me->CastStop();

                events.ScheduleEvent(EVENT_BOOBLE_SHIELD, urand(5000, 30000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOOBLE_SHIELD:
                        {
                            std::list<Creature*> creatures;
                            CreaturesCheck checker(me->GetGUID());
                            JadeCore::CreatureListSearcher<CreaturesCheck> searcher(me, creatures, checker);
                            me->VisitNearbyWorldObject(40.0f, searcher);

                            if (!creatures.empty())
                            {
                                Creature* target = JadeCore::Containers::SelectRandomContainerElement(creatures);

                                DoCast(target, SPELL_BOOBLE_SHIELD_1);
                            }

                            break;
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            class CreaturesCheck
            {
                public:
                    CreaturesCheck(uint64 casterGuid) : _casterGuid(casterGuid) { }
                    bool operator()(Creature* u)
                    {
                        if (!u->isAlive())
                            return false;

                        if (_casterGuid == u->GetGUID())
                            return false;

                        if (!u->isInCombat())
                            return false;


                        if (u->GetEntry() != NPC_TORMENTED_INITIATE && 
                            u->GetEntry() != NPC_FALLEN_POOL_TENDER &&
                            u->GetEntry() != NPC_AQUEUOS_DEFENDER)
                            return false;

                        return true;
                    }

                private:

                    uint64 _casterGuid;
            };
        };
};

class npc_siege_of_orgrimmar_fallen_pool_tender : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_fallen_pool_tender() : CreatureScript("npc_siege_of_orgrimmar_fallen_pool_tender") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_fallen_pool_tenderAI(creature);
        }

        struct npc_siege_of_orgrimmar_fallen_pool_tenderAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_fallen_pool_tenderAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);

                std::list<Creature*> creatures;
                me->GetCreatureListWithEntryInGrid(creatures, NPC_TORMENTED_INITIATE, 25.0f);

                for (std::list<Creature*>::const_iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* creature = (*itr);

                    creature->CastSpell(me, SPELL_COSMETIC_CHANNEL_1);
                }

                DoCast(me, SPELL_BOOBLE_SHIELD_2);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                me->RemoveAura(SPELL_BOOBLE_SHIELD_2);

                events.ScheduleEvent(EVENT_CORRUPTED_WATER, urand(8000, 15000));
                events.ScheduleEvent(EVENT_WATER_BOLT, 1);

                TriggerLorewalkerChoIntro();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WATER_BOLT:
                            DoCastVictim(SPELL_WATER_BOLT);
                            events.ScheduleEvent(EVENT_WATER_BOLT, 2000);
                            break;
                        case EVENT_CORRUPTED_WATER:
                            DoCastAOE(SPELL_CORRUPTED_WATER);
                            events.ScheduleEvent(EVENT_CORRUPTED_WATER, urand(40000, 60000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            void TriggerLorewalkerChoIntro()
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (pInstance->GetData(DATA_LOREWALKER_CHO_INTRO_IMMERSEUS) != DONE)
                    {
                        pInstance->SetData(DATA_LOREWALKER_CHO_INTRO_IMMERSEUS, DONE);

                        if (Creature* pCho = pInstance->instance->GetCreature(pInstance->GetData64(DATA_LOREWALKER_CHO_1)))
                            pCho->AI()->DoAction(ACTION_CHO_TALK_INTRO_IMMERSEUS);
                    }
                }
            }
        };
};

class npc_siege_of_orgrimmar_aqueous_defender : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_aqueous_defender() : CreatureScript("npc_siege_of_orgrimmar_aqueous_defender") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_aqueous_defenderAI(creature);
        }

        struct npc_siege_of_orgrimmar_aqueous_defenderAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_aqueous_defenderAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_RUSHING_WATERS, urand(8000, 15000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RUSHING_WATERS:
                            DoCast(me, SPELL_RUSHING_WATERS);
                            events.ScheduleEvent(EVENT_RUSHING_WATERS, urand(20000, 30000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_siege_of_orgrimmar_lesser_sha_puddle : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_lesser_sha_puddle() : CreatureScript("npc_siege_of_orgrimmar_lesser_sha_puddle") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_lesser_sha_puddleAI(creature);
        }

        struct npc_siege_of_orgrimmar_lesser_sha_puddleAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_lesser_sha_puddleAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
            }

            void JustDied(Unit* who)
            {
                DoCastAOE(SPELL_LESSER_SHA_RESIDUE, true);
            }
        };
};

class npc_siege_of_orgrimmar_lesser_contaminated_puddle : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_lesser_contaminated_puddle() : CreatureScript("npc_siege_of_orgrimmar_lesser_contaminated_puddle") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_lesser_contaminated_puddleAI(creature);
        }

        struct npc_siege_of_orgrimmar_lesser_contaminated_puddleAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_lesser_contaminated_puddleAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);

                checkPuddleTimer = 1000;
                isHealed = false;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!isHealed)
                {
                    if (checkPuddleTimer <= diff)
                    {
                        checkPuddleTimer = 1000;

                        if (me->IsFullHealth())
                        {
                            isHealed = true;

                            DoCastAOE(SPELL_LESSER_PURIFIED_RESIDUE, true);

                            me->DespawnOrUnsummon(1000);
                        }
                    }
                    else
                    {
                        checkPuddleTimer -= diff;
                    }
                }
            }

        private:

            uint32 checkPuddleTimer;
            bool isHealed;

        };
};

class npc_siege_of_orgrimmar_lorewalker_cho_1 : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_lorewalker_cho_1() : CreatureScript("npc_siege_of_orgrimmar_lorewalker_cho_1") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_lorewalker_cho_1AI(creature);
        }

        struct npc_siege_of_orgrimmar_lorewalker_cho_1AI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_lorewalker_cho_1AI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);

                CanTalkIntro = false;
                CanTalkIntroImmerseus = false;
                CanTalkOutroImmerseus = false;
            }

            void DoAction(const int32 action)
            {
                if (CanTalkIntro || CanTalkIntroImmerseus || CanTalkOutroImmerseus)
                    return;

                if (action == ACTION_CHO_TALK_INTRO)
                {
                    CanTalkIntro = true;
                    events.ScheduleEvent(EVENT_CHO_TALK_INTRO_1, 1000);
                }
                else if (action == ACTION_CHO_TALK_INTRO_IMMERSEUS)
                {
                    CanTalkIntroImmerseus = true;
                    events.ScheduleEvent(EVENT_CHO_TALK_INTRO_6, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHO_TALK_INTRO_1:
                            Talk(SAY_CHO_INTRO_1);
                            events.ScheduleEvent(EVENT_CHO_TALK_INTRO_2, 12000);
                            break;
                        case EVENT_CHO_TALK_INTRO_2:
                            Talk(SAY_CHO_INTRO_2);
                            events.ScheduleEvent(EVENT_CHO_TALK_INTRO_3, 12000);
                            break;
                        case EVENT_CHO_TALK_INTRO_3:
                            Talk(SAY_CHO_INTRO_3);
                            events.ScheduleEvent(EVENT_CHO_TALK_INTRO_4, 9000);
                            break;
                        case EVENT_CHO_TALK_INTRO_4:
                            Talk(SAY_CHO_INTRO_4);
                            events.ScheduleEvent(EVENT_CHO_TALK_INTRO_5, 12000);
                            break;
                        case EVENT_CHO_TALK_INTRO_5:
                            Talk(SAY_CHO_INTRO_5);
                            CanTalkIntro = false;
                            break;
                        case EVENT_CHO_TALK_INTRO_6:
                            Talk(SAY_CHO_INTRO_6);
                            CanTalkIntroImmerseus = false;
                            break;
                    }
                }
            }

        private:

            bool CanTalkIntro;
            bool CanTalkIntroImmerseus;
            bool CanTalkOutroImmerseus;

        };
};

class npc_siege_of_orgrimmar_fragment_of_pride : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_fragment_of_pride() : CreatureScript("npc_siege_of_orgrimmar_fragment_of_pride") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_fragment_of_prideAI(creature);
        }

        struct npc_siege_of_orgrimmar_fragment_of_prideAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_fragment_of_prideAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_GROWING_PRIDE, urand(3000, 10000));
            }

            void JustDied(Unit* /*who*/)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GROWING_PRIDE:
                            DoCastAOE(SPELL_GROWING_PRIDE);
                            events.ScheduleEvent(EVENT_GROWING_PRIDE, urand(5000, 15000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_siege_of_orgrimmar_amalgamated_hubris : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_amalgamated_hubris() : CreatureScript("npc_siege_of_orgrimmar_amalgamated_hubris") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_amalgamated_hubrisAI(creature);
        }

        struct npc_siege_of_orgrimmar_amalgamated_hubrisAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_amalgamated_hubrisAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SHA_NOVA, urand(5000, 10000));
            }

            void JustDied(Unit* /*who*/)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHA_NOVA:
                            DoCastAOE(SPELL_SHA_NOVA);
                            events.ScheduleEvent(EVENT_SHA_NOVA, urand(10000, 20000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_siege_of_orgrimmar_vanity : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_vanity() : CreatureScript("npc_siege_of_orgrimmar_vanity") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_vanityAI(creature);
        }

        struct npc_siege_of_orgrimmar_vanityAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_vanityAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 8.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 8.0f);
            }

            void InitializeAI()
            {
                if (me->isDead())
                {
                    SendVanityDied();
                }
                else
                {
                    Reset();
                }
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SELF_ABSORBED, urand(3000, 7000));
            }

            void JustDied(Unit* /*who*/)
            {
                events.Reset();

                SendVanityDied();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SELF_ABSORBED:
                            DoCastAOE(SPELL_SELF_ABSORBED);
                            events.ScheduleEvent(EVENT_SELF_ABSORBED, urand(10000, 12000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            void SendVanityDied()
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (Creature* pZeal = pInstance->instance->GetCreature(pInstance->GetData64(DATA_ZEAL)))
                    {
                        pZeal->AI()->DoAction(ACTION_VANITY_DIED);
                    }
                }
            }
        };
};

class npc_siege_of_orgrimmar_arrogance : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_arrogance() : CreatureScript("npc_siege_of_orgrimmar_arrogance") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_arroganceAI(creature);
        }

        struct npc_siege_of_orgrimmar_arroganceAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_arroganceAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 8.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 8.0f);
            }

            void InitializeAI()
            {
                if (me->isDead())
                {
                    SendArroganceDied();
                }
                else
                {
                    Reset();
                }
            }

            void Reset()
            {
                events.Reset();
            }

            void JustDied(Unit* /*who*/)
            {
                events.Reset();

                SendArroganceDied();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:

            void SendArroganceDied()
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (Creature* pZeal = pInstance->instance->GetCreature(pInstance->GetData64(DATA_ZEAL)))
                    {
                        pZeal->AI()->DoAction(ACTION_ARROGANCE_DIED);
                    }
                }
            }
        };
};

class npc_siege_of_orgrimmar_zeal : public CreatureScript
{
    public:
        npc_siege_of_orgrimmar_zeal() : CreatureScript("npc_siege_of_orgrimmar_zeal") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_siege_of_orgrimmar_zealAI(creature);
        }

        struct npc_siege_of_orgrimmar_zealAI : public ScriptedAI
        {
            npc_siege_of_orgrimmar_zealAI(Creature* creature) : ScriptedAI(creature)
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 8.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 8.0f);

                isVanityActive = false;
                isArroganceActive = false;
            }

            void Reset()
            {
                events.Reset();

                CheckVanityAlive();

                CheckArroganceAlive();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_SHA_SPLASH, urand(5000, 10000));

                if (isVanityActive)
                {
                    events.ScheduleEvent(EVENT_SELF_ABSORBED, urand(5000, 10000));
                }
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_VANITY_DIED)
                {
                    me->RemoveAura(SPELL_VANITY);
                    events.CancelEvent(EVENT_SELF_ABSORBED);
                }
                else if (action == ACTION_ARROGANCE_DIED)
                {
                    me->RemoveAura(SPELL_ARROGANCE);
                }
            }

            void JustDied(Unit* /*who*/)
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SELF_ABSORBED:
                            DoCastAOE(SPELL_SELF_ABSORBED);
                            events.ScheduleEvent(EVENT_SELF_ABSORBED, urand(10000, 12000));
                            break;
                        case EVENT_SHA_SPLASH:
                            DoCastVictim(SPELL_SHA_SPLASH);
                            events.ScheduleEvent(EVENT_SHA_SPLASH, urand(15000, 20000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            bool isVanityActive;
            bool isArroganceActive;

        private:

            void CheckVanityAlive()
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (Creature* pVanity = pInstance->instance->GetCreature(pInstance->GetData64(DATA_VANITY)))
                    {
                        if (pVanity->isAlive())
                        {
                            me->AddAura(SPELL_VANITY, me);
                            isVanityActive = true;
                        }
                    }
                }
            }

            void CheckArroganceAlive()
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (Creature* pArrogance = pInstance->instance->GetCreature(pInstance->GetData64(DATA_ARROGANCE)))
                    {
                        if (pArrogance->isAlive())
                        {
                            me->AddAura(SPELL_ARROGANCE, me);
                            isArroganceActive = true;
                        }
                    }
                }
            }

        };
};

class at_soo_lorewalker_cho_intro : public AreaTriggerScript
{
    public:
        at_soo_lorewalker_cho_intro() : AreaTriggerScript("at_soo_lorewalker_cho_intro") { }

        bool OnTrigger(Player* pPlayer, const AreaTriggerEntry* /*pAt*/, bool p_Enter)
        {
            if (InstanceScript* pInstance = pPlayer->GetInstanceScript())
            {
                if (pInstance->GetData(DATA_LOREWALKER_CHO_INTRO) != DONE)
                {
                    pInstance->SetData(DATA_LOREWALKER_CHO_INTRO, DONE);

                    if (Creature* pCho = pInstance->instance->GetCreature(pInstance->GetData64(DATA_LOREWALKER_CHO_1)))
                        pCho->AI()->DoAction(ACTION_CHO_TALK_INTRO);
                }
            }
            return true;
        }
};

void AddSC_siege_of_orgrimmar_part_1()
{
    new npc_siege_of_orgrimmar_ooze_controller();       // 73222
    new npc_siege_of_orgrimmar_tormented_initiate();    // 73349
    new npc_siege_of_orgrimmar_fallen_pool_tender();    // 73342
    new npc_siege_of_orgrimmar_aqueous_defender();      // 73191
    new npc_siege_of_orgrimmar_lesser_sha_puddle();     // 73226 73197
    new npc_siege_of_orgrimmar_lesser_contaminated_puddle();    // 73283 73260
    new npc_siege_of_orgrimmar_lorewalker_cho_1();      // 73330
    new npc_siege_of_orgrimmar_fragment_of_pride();     // 72655
    new npc_siege_of_orgrimmar_amalgamated_hubris();    // 72658
    new npc_siege_of_orgrimmar_vanity();                // 72662
    new npc_siege_of_orgrimmar_arrogance();             // 72663
    new npc_siege_of_orgrimmar_zeal();                  // 72661

    new at_soo_lorewalker_cho_intro();                  // 9355
}
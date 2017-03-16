#include "ScriptPCH.h"
#include "blood_on_the_snow.h"

enum ScriptedTextsMoira
{
    SAY_MOIRA_START_1   = 2,
    SAY_MOIRA_START_2   = 3,
    SAY_MOIRA_START_3   = 4,
    SAY_MOIRA_START_4   = 5,
    SAY_MOIRA_START_5   = 6,
    SAY_MOIRA_START_6   = 7,
    SAY_MOIRA_START_7   = 8,
    SAY_MOIRA_BATTLE    = 9,
    SAY_MOIRA_END_1     = 10,
    SAY_MOIRA_END_2     = 11,
};

enum ScriptedTextsVarian
{
    SAY_VARIAN_START_1  = 0,
    SAY_VARIAN_START_2  = 1,
    SAY_VARIAN_START_3  = 2,
    SAY_VARIAN_START_4  = 3,
    SAY_VARIAN_START_5  = 4,
    SAY_VARIAN_START_6  = 5,
    SAY_VARIAN_END_1    = 6,
    SAY_VARIAN_END_2    = 7,
    SAY_VARIAN_END_3    = 8,
};

enum ScriptedTextsGrimbolt
{
    SAY_GRIMBOLT_1  = 0,
    SAY_GRIMBOLT_2  = 1,
};

enum ScriptedTextsStonebeard
{
    SAY_STONEBEARD_1 = 0,
    SAY_STONEBEARD_2 = 1,
};

enum ScriptedTextsBoldbrew
{
    SAY_BOLDBREW_1 = 0,
    SAY_BOLDBREW_2 = 1,
};

enum ScriptedTextsForgefellow
{
    SAY_FORGEFELLOW_1 = 0,
    SAY_FORGEFELLOW_2 = 1,
};

enum Spells
{
    SPELL_FREE_SCOUT_STONEBEARD     = 141316,

    // Moira Thaurissan
    SPELL_MOIRAS_MAJESTY            = 140602,
    SPELL_MOIRAS_MENDING            = 140614, // triggered by 140602

    // Zandalari Rage Banner
    SPELL_ZANDALARI_RAGE            = 141315,
    SPELL_ZANDALARI_BANNER          = 142669,

    // Frostmane Witch Doctor
    SPELL_FROST_SHOCK               = 46180,
    SPELL_HEKIMAS_WISDOM            = 141423,

    // Frostmane Prowler
    SPELL_POUNCE                    = 75008,

    // Frostmane Mauler
    SPELL_ENRAGE                    = 32714,

    // Frostmane Flesh Eater
    SPELL_DRAIN_BLOOD               = 112896,

    // Frostmane Berserker
    SPELL_DEVASTATING_LEAP          = 126903,
    SPELL_DEVASTATING_LEAP_DMG      = 126902,

    // Frostmane Bonechiller
    SPELL_BLIZZARD                  = 141379,
    SPELL_FROST_NOVA                = 11831,
    SPELL_FROSTBOLT                 = 141619,
    SPELL_SUMMON_WATER_ELEMENTAL    = 141380,

    // Bonechiller Banafu
    SPELL_FROSTBOLT_VOLLEY          = 141620,
    SPELL_BONECHILLING_BLIZZARD     = 141428,

    // Skullcrusher Gundu
    SPELL_CLEAVE                    = 40504,
    SPELL_SHOCKWAVE                 = 131570,

    // Farastu
    SPELL_ICE_SPIKES                = 132980,
    SPELL_ICE_SPIKES_SUMMON         = 141416,
    SPELL_ICE_SPIKE_DMG             = 141413,

    // Hekima the Wise
    SPELL_ZANDALARI_RAGE_BANNER     = 141329,
    SPELL_HEKIMAS_BRAND             = 139555,
    SPELL_HEKIMAS_SCORN             = 142623,
    SPELL_HEKIMAS_SCORN_DMG         = 133837,
};

enum Events
{
    EVENT_MOIRA_TALK_1  = 1,
    EVENT_MOIRA_TALK_2,
    EVENT_MOIRA_TALK_3,
    EVENT_MOIRA_TALK_4,
    EVENT_MOIRA_TALK_5,
    EVENT_MOIRA_TALK_6,
    EVENT_MOIRA_TALK_7,
    EVENT_MOIRA_BATTLE,
    EVENT_MOIRA_TALK_8,
    EVENT_MOIRA_TALK_9,

    EVENT_VARIAN_TALK_1,
    EVENT_VARIAN_TALK_2,
    EVENT_VARIAN_TALK_3,
    EVENT_VARIAN_TALK_4,
    EVENT_VARIAN_TALK_5,
    EVENT_VARIAN_TALK_6,
    EVENT_VARIAN_TALK_7,
    EVENT_VARIAN_TALK_8,
    EVENT_VARIAN_TALK_9,

    EVENT_GRIMBOLT_TALK_1,
    EVENT_GRIMBOLT_TALK_2,

    EVENT_STONEBEARD_TALK_1,
    EVENT_STONEBEARD_TALK_2,

    EVENT_BOLDBREW_TALK_1,
    EVENT_BOLDBREW_TALK_2,

    EVENT_FORGEFELLOW_TALK_1,
    EVENT_FORGEFELLOW_TALK_2,

    // Frostmane Witch Doctor
    EVENT_FROST_SHOCK,
    EVENT_HEKIMAS_WISDOM,

    // Frostmane Prowler
    EVENT_POUNCE,

    // Frostmane Mauler
    EVENT_ENRAGE,

    // Frostmane Flesh Eater
    EVENT_DRAIN_BLOOD,

    // Frostmane Berserker
    EVENT_DEVASTATING_LEAP,

    // Frostmane Bonechiller
    EVENT_BLIZZARD,
    EVENT_FROST_NOVA,
    EVENT_FROSTBOLT,
    EVENT_SUMMON_WATER_ELEMENTAL,

    // Bonechiller Banafu
    EVENT_BONECHILLING_BLIZZARD,
    EVENT_FROSTBOLT_VOLLEY,

    // Skullcrusher Gundu
    EVENT_CLEAVE,
    EVENT_SHOCKWAVE,

    // Farastu
    EVENT_ICE_SPIKES,

    // Hekima the Wise
    EVENT_ZANDALARI_BANNER,
    EVENT_HEKIMAS_BRAND,
    EVENT_HEKIMAS_SCORN,
};

enum Actions
{
    ACTION_RELEASE_FROM_SPIT    = 1,
    ACTION_TALK_GRIMBOLT,
    ACTION_RELEASE_BOLDBREW,
    ACTION_RELEASE_FORGEFELLOW,
};

enum Equipments
{
    ITEM_FROSTMANE_AXE          = 2260,
    ITEM_FROSTMANE_STAFF        = 2257,
    ITEM_FROSTMANE_SCEPTER      = 3223,
    ITEM_FROSTMANE_SHORTSWORD   = 2258,
};

class npc_blood_on_the_snow_zandalari_rage_banner : public CreatureScript
{
    public:

        npc_blood_on_the_snow_zandalari_rage_banner() : CreatureScript("npc_blood_on_the_snow_zandalari_rage_banner") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_zandalari_rage_bannerAI(pCreature);
        }

        struct npc_blood_on_the_snow_zandalari_rage_bannerAI : public Scripted_NoMovementAI
        {
            npc_blood_on_the_snow_zandalari_rage_bannerAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                me->AddAura(SPELL_ZANDALARI_RAGE_BANNER, me);
            }

            void JustDied(Unit* /*killer*/)
            {
                me->RemoveAura(SPELL_ZANDALARI_RAGE_BANNER);

                me->DespawnOrUnsummon(2000);
            }
        };
};

class npc_blood_on_the_snow_roasting_spit : public CreatureScript
{
    public:

        npc_blood_on_the_snow_roasting_spit() : CreatureScript("npc_blood_on_the_snow_roasting_spit") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_roasting_spitAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {        
                if (pInstance->GetData(DATA_CURRENT_STEP) != STEP_2)
                    return true;

                Map::PlayerList const& players = pInstance->instance->GetPlayers();
                if (!players.isEmpty())
                    for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                        if (Player* player = i->getSource())
                            if (player->isInCombat())
                                return true;



                if (Creature* pStonebeard = creature->FindNearestCreature(NPC_SCOUT_STONEBEARD, 10.0f))
                {
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    
                    if (pStonebeard->AI())
                        pStonebeard->AI()->DoAction(ACTION_RELEASE_FROM_SPIT);
                }
            }

            return true;

        }

        struct npc_blood_on_the_snow_roasting_spitAI : public Scripted_NoMovementAI
        {
            npc_blood_on_the_snow_roasting_spitAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void Reset()
            {
               
            }
        };
};

class npc_blood_on_the_snow_scout_stonebeard : public CreatureScript
{
    public:

        npc_blood_on_the_snow_scout_stonebeard() : CreatureScript("npc_blood_on_the_snow_scout_stonebeard") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_scout_stonebeardAI(pCreature);
        }

        struct npc_blood_on_the_snow_scout_stonebeardAI : public Scripted_NoMovementAI
        {
            npc_blood_on_the_snow_scout_stonebeardAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                pInstance = me->GetInstanceScript();

                m_IsOnSpit = false;
            }

            void Reset()
            {
                // TODO: switch on when we find vehicleid for the spit
                //PutOnSpit();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_RELEASE_FROM_SPIT)
                {
                    if (!pInstance || pInstance->GetData(DATA_CURRENT_STEP) != STEP_2)
                        return;

                    // TODO: switch on when we find vehicleid for the spit
                    //ReleaseFromSpit();

                    pInstance->SetData(DATA_STONEBEARD, DONE);

                    events.ScheduleEvent(EVENT_STONEBEARD_TALK_1, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);
                
                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STONEBEARD_TALK_1:
                            Talk(SAY_STONEBEARD_1);
                            events.ScheduleEvent(EVENT_STONEBEARD_TALK_2, 5000);
                            break;
                        case EVENT_STONEBEARD_TALK_2:
                            Talk(SAY_STONEBEARD_2);
                            break;
                    }
                }
            }

        private:

            bool m_IsOnSpit;
            InstanceScript* pInstance;

        private:

            void PutOnSpit()
            {
                if (m_IsOnSpit)
                    return;

                if (Creature* pSpit = me->FindNearestCreature(NPC_ROASTING_SPIT, 10.0f))
                {
                    me->EnterVehicle(pSpit);
                    m_IsOnSpit = true;
                }
            }

            void ReleaseFromSpit()
            {
                if (!m_IsOnSpit)
                    return;

                me->ExitVehicle();
                m_IsOnSpit = false;
            }
        };
};

class npc_blood_on_the_snow_moira_thaurissan : public CreatureScript
{
    public:

        npc_blood_on_the_snow_moira_thaurissan() : CreatureScript("npc_blood_on_the_snow_moira_thaurissan") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_moira_thaurissanAI(pCreature);
        }

        struct npc_blood_on_the_snow_moira_thaurissanAI : public Scripted_NoMovementAI
        {
            npc_blood_on_the_snow_moira_thaurissanAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                pInstance = me->GetInstanceScript();
                introDone = false;

                me->setActive(true);
            }

            void Reset()
            {

            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!introDone && who->GetTypeId() == TYPEID_PLAYER && who->GetDistance(who) <= 10.0f)
                {
                    introDone = true;
                    events.ScheduleEvent(EVENT_VARIAN_TALK_1, 5000);
                }
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_LAST_PHASE)
                {
                    introDone = true;

                    Talk(SAY_MOIRA_BATTLE);
                    me->NearTeleportTo(moiraNearHekimaPos, true);

                    events.ScheduleEvent(EVENT_MOIRA_BATTLE, 1000);
                }
                else if (action == ACTION_COMPLETE_SCENARIO)
                {
                    me->CastStop();

                    if (Creature* pVarian = GetVarian())
                    {
                        pVarian->NearTeleportTo(varianNearHekimaPos);

                        events.ScheduleEvent(EVENT_VARIAN_TALK_7, 2000);
                    }
                }
            }

            void DamageTaken(Unit* who, uint32& damage)
            {
                damage = 0;
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_VARIAN_TALK_1:
                            TalkVarian(SAY_VARIAN_START_1);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_1, 5000);
                            break;
                        case EVENT_MOIRA_TALK_1:
                            Talk(SAY_MOIRA_START_1);
                            events.ScheduleEvent(EVENT_VARIAN_TALK_2, 8000);
                            break;
                        case EVENT_VARIAN_TALK_2:
                            TalkVarian(SAY_VARIAN_START_2);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_2, 5000);
                            break;
                        case EVENT_MOIRA_TALK_2:
                            Talk(SAY_MOIRA_START_2);
                            events.ScheduleEvent(EVENT_VARIAN_TALK_3, 5000);
                            break;
                        case EVENT_VARIAN_TALK_3:
                            TalkVarian(SAY_VARIAN_START_3);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_3, 4000);
                            break;
                        case EVENT_MOIRA_TALK_3:
                            Talk(SAY_MOIRA_START_3);
                            events.ScheduleEvent(EVENT_VARIAN_TALK_4, 7000);
                            break;
                        case EVENT_VARIAN_TALK_4:
                            TalkVarian(SAY_VARIAN_START_4);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_4, 6500);
                            break;
                        case EVENT_MOIRA_TALK_4:
                            Talk(SAY_MOIRA_START_4);
                            events.ScheduleEvent(EVENT_VARIAN_TALK_5, 6000);
                            break;
                        case EVENT_VARIAN_TALK_5:
                            if (Creature* pVarian = GetVarian())
                            {
                                pVarian->SetFacingToObject(me);
                                pVarian->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                            }

                            TalkVarian(SAY_VARIAN_START_5);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_5, 4500);
                            break;
                        case EVENT_MOIRA_TALK_5:
                            if (Creature* pVarian = GetVarian())
                                me->SetFacingToObject(pVarian);

                            Talk(SAY_MOIRA_START_5);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_6, 8000);
                            break;
                        case EVENT_MOIRA_TALK_6:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_SHOUT);
                            Talk(SAY_MOIRA_START_6);
                            events.ScheduleEvent(EVENT_VARIAN_TALK_6, 15000);
                            break;
                        case EVENT_VARIAN_TALK_6:
                            if (Creature* pVarian = GetVarian())
                            {
                                pVarian->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                            }
                            TalkVarian(SAY_VARIAN_START_6);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_7, 3500);
                            break;
                        case EVENT_MOIRA_TALK_7:
                            me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                            Talk(SAY_MOIRA_START_7);
                            break;
                        case EVENT_MOIRA_BATTLE:
                            DoCast(me, SPELL_MOIRAS_MAJESTY);
                            if (Creature* pHekima = GetHekima())
                            {
                                pHekima->Attack(me, true);
                            }
                            break;
                        case EVENT_VARIAN_TALK_7:
                            TalkVarian(SAY_VARIAN_END_1);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_8, 5000);
                            break;
                        case EVENT_MOIRA_TALK_8:
                            Talk(SAY_MOIRA_END_1);
                            events.ScheduleEvent(EVENT_VARIAN_TALK_8, 5000);
                            break;
                        case EVENT_VARIAN_TALK_8:
                            TalkVarian(SAY_VARIAN_END_2);
                            events.ScheduleEvent(EVENT_MOIRA_TALK_9, 8500);
                            break;
                        case EVENT_MOIRA_TALK_9:
                            Talk(SAY_MOIRA_END_2);
                            events.ScheduleEvent(EVENT_VARIAN_TALK_9, 8500);
                            break;
                        case EVENT_VARIAN_TALK_9:
                            TalkVarian(SAY_VARIAN_END_3);
                            break;

                    }
                }
            }

        private:

            bool introDone;
            InstanceScript* pInstance;

        private:

            void TalkVarian(uint32 id)
            {
                if (pInstance)
                {
                    if (Creature* pVarian = pInstance->instance->GetCreature(pInstance->GetData64(DATA_VARIAN)))
                    {
                        if (pVarian->IsAIEnabled && pVarian->AI())
                            pVarian->AI()->Talk(id);
                    }
                }
            }

            Creature* GetVarian()
            {
                if (!pInstance)
                    return NULL;

                return pInstance->instance->GetCreature(pInstance->GetData64(DATA_VARIAN));    
            }

            Creature* GetHekima()
            {
                if (!pInstance)
                    return NULL;

                return pInstance->instance->GetCreature(pInstance->GetData64(DATA_HEKIMA));   
            }
        };
};

class npc_blood_on_the_snow_mountaineer_grimbolt : public CreatureScript
{
    public:

        npc_blood_on_the_snow_mountaineer_grimbolt() : CreatureScript("npc_blood_on_the_snow_mountaineer_grimbolt") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_mountaineer_grimboltAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {        
                if (pInstance->GetData(DATA_CURRENT_STEP) != STEP_1)
                    return true;

                if (creature->IsAIEnabled && creature->AI())
                    creature->AI()->DoAction(ACTION_TALK_GRIMBOLT);
            }

            return true;
        }

        struct npc_blood_on_the_snow_mountaineer_grimboltAI : public ScriptedAI
        {
            npc_blood_on_the_snow_mountaineer_grimboltAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_TALK_GRIMBOLT)
                {
                    if (pInstance)
                    {
                        pInstance->SetData(DATA_TALK_GRIMBOLT, DONE);
                    }

                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    events.ScheduleEvent(EVENT_GRIMBOLT_TALK_1, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GRIMBOLT_TALK_1:
                            Talk(SAY_GRIMBOLT_1);
                            events.ScheduleEvent(EVENT_GRIMBOLT_TALK_2, 9000);
                            break;
                        case EVENT_GRIMBOLT_TALK_2:
                            Talk(SAY_GRIMBOLT_2);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:

            InstanceScript* pInstance; 
        };
};

class npc_blood_on_the_snow_scout_boldbrew : public CreatureScript
{
    public:

        npc_blood_on_the_snow_scout_boldbrew() : CreatureScript("npc_blood_on_the_snow_scout_boldbrew") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_scout_boldbrewAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {        
                if (pInstance->GetData(DATA_CURRENT_STEP) != STEP_3)
                    return true;

                Map::PlayerList const& players = pInstance->instance->GetPlayers();
                if (!players.isEmpty())
                    for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                        if (Player* player = i->getSource())
                            if (player->isInCombat())
                                return true;

                if (creature->IsAIEnabled && creature->AI())
                    creature->AI()->DoAction(ACTION_RELEASE_BOLDBREW);
            }

            return true;

        }

        struct npc_blood_on_the_snow_scout_boldbrewAI : public Scripted_NoMovementAI
        {
            npc_blood_on_the_snow_scout_boldbrewAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                pInstance = me->GetInstanceScript();

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_RELEASE_BOLDBREW)
                {
                    if (pInstance)
                    {
                        pInstance->SetData(DATA_BOLDBREW, DONE);
                    }

                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    events.ScheduleEvent(EVENT_BOLDBREW_TALK_1, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOLDBREW_TALK_1:
                            Talk(SAY_BOLDBREW_1);
                            events.ScheduleEvent(EVENT_BOLDBREW_TALK_2, 4000);
                            break;
                        case EVENT_BOLDBREW_TALK_2:
                            Talk(SAY_BOLDBREW_2);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:

            InstanceScript* pInstance; 
        };
};

class npc_blood_on_the_snow_scout_forgefellow : public CreatureScript
{
    public:

        npc_blood_on_the_snow_scout_forgefellow() : CreatureScript("npc_blood_on_the_snow_scout_forgefellow") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_scout_forgefellowAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {        
                if (pInstance->GetData(DATA_CURRENT_STEP) != STEP_4)
                    return true;

                Map::PlayerList const& players = pInstance->instance->GetPlayers();
                if (!players.isEmpty())
                    for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                        if (Player* player = i->getSource())
                            if (player->isInCombat())
                                return true;

                if (creature->IsAIEnabled && creature->AI())
                    creature->AI()->DoAction(ACTION_RELEASE_FORGEFELLOW);
            }

            return true;

        }

        struct npc_blood_on_the_snow_scout_forgefellowAI : public Scripted_NoMovementAI
        {
            npc_blood_on_the_snow_scout_forgefellowAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                pInstance = me->GetInstanceScript();

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_RELEASE_FORGEFELLOW)
                {
                    if (pInstance)
                    {
                        pInstance->SetData(DATA_FORGEFELLOW, DONE);
                    }

                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    events.ScheduleEvent(EVENT_FORGEFELLOW_TALK_1, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FORGEFELLOW_TALK_1:
                            Talk(SAY_FORGEFELLOW_1);
                            events.ScheduleEvent(EVENT_FORGEFELLOW_TALK_2, 4000);
                            break;
                        case EVENT_FORGEFELLOW_TALK_2:
                            Talk(SAY_FORGEFELLOW_2);
                            break;
                        default:
                            break;
                    }
                }
            }

        private:

            InstanceScript* pInstance; 
        };
};

class npc_blood_on_the_snow_frostmane_witch_doctor : public CreatureScript
{
    public:

        npc_blood_on_the_snow_frostmane_witch_doctor() : CreatureScript("npc_blood_on_the_snow_frostmane_witch_doctor") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_frostmane_witch_doctorAI(pCreature);
        }

        struct npc_blood_on_the_snow_frostmane_witch_doctorAI : public ScriptedAI
        {
            npc_blood_on_the_snow_frostmane_witch_doctorAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_FROST_SHOCK, urand(3000, 5000));
                events.ScheduleEvent(EVENT_HEKIMAS_WISDOM, urand(10000, 15000));
            }

            void JustDied(Unit* who)
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
                        case EVENT_FROST_SHOCK:
                            DoCastVictim(SPELL_FROST_SHOCK);
                            events.ScheduleEvent(EVENT_FROST_SHOCK, urand(5000, 7000));
                            break;
                        case EVENT_HEKIMAS_WISDOM:
                            if (Unit* ally = me->SelectNearbyAlly((Unit*)NULL, 15.0f))
                                DoCast(ally, SPELL_HEKIMAS_WISDOM);

                            events.ScheduleEvent(EVENT_HEKIMAS_WISDOM, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_frostmane_prowler : public CreatureScript
{
    public:

        npc_blood_on_the_snow_frostmane_prowler() : CreatureScript("npc_blood_on_the_snow_frostmane_prowler") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_frostmane_prowlerAI(pCreature);
        }

        struct npc_blood_on_the_snow_frostmane_prowlerAI : public ScriptedAI
        {
            npc_blood_on_the_snow_frostmane_prowlerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_POUNCE, urand(3000, 5000));
            }

            void JustDied(Unit* who)
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
                        case EVENT_POUNCE:
                            DoCastVictim(SPELL_POUNCE);
                            events.ScheduleEvent(EVENT_POUNCE, urand(7000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_frostmane_mauler : public CreatureScript
{
    public:

        npc_blood_on_the_snow_frostmane_mauler() : CreatureScript("npc_blood_on_the_snow_frostmane_mauler") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_frostmane_maulerAI(pCreature);
        }

        struct npc_blood_on_the_snow_frostmane_maulerAI : public ScriptedAI
        {
            npc_blood_on_the_snow_frostmane_maulerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_POUNCE, urand(3000, 5000));
                events.ScheduleEvent(EVENT_ENRAGE, urand(5000, 7000));
            }

            void JustDied(Unit* who)
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
                        case EVENT_POUNCE:
                            DoCastVictim(SPELL_POUNCE);
                            events.ScheduleEvent(EVENT_POUNCE, urand(7000, 15000));
                            break;
                        case EVENT_ENRAGE:
                            DoCast(me, SPELL_ENRAGE);
                            events.ScheduleEvent(EVENT_ENRAGE, 20000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_frostmane_headhunter : public CreatureScript
{
    public:

        npc_blood_on_the_snow_frostmane_headhunter() : CreatureScript("npc_blood_on_the_snow_frostmane_headhunter") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_frostmane_headhunterAI(pCreature);
        }

        struct npc_blood_on_the_snow_frostmane_headhunterAI : public ScriptedAI
        {
            npc_blood_on_the_snow_frostmane_headhunterAI(Creature* creature) : ScriptedAI(creature)
            {
                SetEquipmentSlots(false, ITEM_FROSTMANE_AXE, ITEM_FROSTMANE_AXE);
            }
        };
};

class npc_blood_on_the_snow_frostmane_flesh_eater : public CreatureScript
{
    public:

        npc_blood_on_the_snow_frostmane_flesh_eater() : CreatureScript("npc_blood_on_the_snow_frostmane_flesh_eater") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_frostmane_flesh_eaterAI(pCreature);
        }

        struct npc_blood_on_the_snow_frostmane_flesh_eaterAI : public ScriptedAI
        {
            npc_blood_on_the_snow_frostmane_flesh_eaterAI(Creature* creature) : ScriptedAI(creature)
            {
                SetEquipmentSlots(false, ITEM_FROSTMANE_AXE, 0);
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_DRAIN_BLOOD, urand(5000, 7000));
            }

            void JustDied(Unit* who)
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
                        case EVENT_DRAIN_BLOOD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_DRAIN_BLOOD);
                            events.ScheduleEvent(EVENT_DRAIN_BLOOD, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_frostmane_berserker : public CreatureScript
{
    public:

        npc_blood_on_the_snow_frostmane_berserker() : CreatureScript("npc_blood_on_the_snow_frostmane_berserker") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_frostmane_berserkerAI(pCreature);
        }

        struct npc_blood_on_the_snow_frostmane_berserkerAI : public ScriptedAI
        {
            npc_blood_on_the_snow_frostmane_berserkerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                DoCast(who, SPELL_DEVASTATING_LEAP);
                events.ScheduleEvent(EVENT_DEVASTATING_LEAP, urand(10000, 15000));
            }

            void JustDied(Unit* who)
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
                        case EVENT_DEVASTATING_LEAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -15.0f, true))
                                DoCast(target, SPELL_DEVASTATING_LEAP);
                            events.ScheduleEvent(EVENT_DEVASTATING_LEAP, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_frostmane_bonechiller : public CreatureScript
{
    public:

        npc_blood_on_the_snow_frostmane_bonechiller() : CreatureScript("npc_blood_on_the_snow_frostmane_bonechiller") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_frostmane_bonechillerAI(pCreature);
        }

        struct npc_blood_on_the_snow_frostmane_bonechillerAI : public ScriptedAI
        {
            npc_blood_on_the_snow_frostmane_bonechillerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();
            }

            void EnterCombat(Unit* who)
            {
                //DoCast(me, SPELL_SUMMON_WATER_ELEMENTAL);

                events.ScheduleEvent(EVENT_FROSTBOLT, 1000);
                events.ScheduleEvent(EVENT_FROST_NOVA, urand(3000, 10000));
                events.ScheduleEvent(EVENT_BLIZZARD, urand(10000, 15000));
            }

            void JustSummoned(Creature* summon)
            {
                if (me->isInCombat())
                    DoZoneInCombat(summon);

                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void JustDied(Unit* who)
            {
                events.Reset();
                summons.DespawnAll();
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
                        case EVENT_FROSTBOLT:
                            DoCastVictim(SPELL_FROSTBOLT);
                            events.ScheduleEvent(EVENT_FROSTBOLT, 2000);
                            break;
                        case EVENT_FROST_NOVA:
                            DoCastAOE(SPELL_FROST_NOVA);
                            events.ScheduleEvent(EVENT_FROST_NOVA, urand(10000, 20000));
                            break;
                        case EVENT_BLIZZARD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_BLIZZARD);
                            events.ScheduleEvent(EVENT_BLIZZARD, 30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_bonechiller_banafu : public CreatureScript
{
    public:

        npc_blood_on_the_snow_bonechiller_banafu() : CreatureScript("npc_blood_on_the_snow_bonechiller_banafu") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_bonechiller_banafuAI(pCreature);
        }

        struct npc_blood_on_the_snow_bonechiller_banafuAI : public ScriptedAI
        {
            npc_blood_on_the_snow_bonechiller_banafuAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, 1000);
                events.ScheduleEvent(EVENT_FROST_NOVA, urand(3000, 10000));
                events.ScheduleEvent(EVENT_BONECHILLING_BLIZZARD, urand(10000, 15000));
            }

            void JustDied(Unit* who)
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
                        case EVENT_FROSTBOLT_VOLLEY:
                            DoCastVictim(SPELL_FROSTBOLT);
                            events.ScheduleEvent(EVENT_FROSTBOLT, 2000);
                            break;
                        case EVENT_FROST_NOVA:
                            DoCastAOE(SPELL_FROST_NOVA);
                            events.ScheduleEvent(EVENT_FROST_NOVA, urand(10000, 20000));
                            break;
                        case EVENT_BONECHILLING_BLIZZARD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_BONECHILLING_BLIZZARD);
                            events.ScheduleEvent(EVENT_BONECHILLING_BLIZZARD, 30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_skullcrusher_gundu : public CreatureScript
{
    public:

        npc_blood_on_the_snow_skullcrusher_gundu() : CreatureScript("npc_blood_on_the_snow_skullcrusher_gundu") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_skullcrusher_gunduAI(pCreature);
        }

        struct npc_blood_on_the_snow_skullcrusher_gunduAI : public ScriptedAI
        {
            npc_blood_on_the_snow_skullcrusher_gunduAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CLEAVE, urand(3000, 5000));
                events.ScheduleEvent(EVENT_SHOCKWAVE, urand(10000, 15000));
            }

            void JustDied(Unit* who)
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
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 7000));
                            break;
                        case EVENT_SHOCKWAVE:
                            DoCastVictim(SPELL_SHOCKWAVE);
                            events.ScheduleEvent(EVENT_SHOCKWAVE, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_farastu : public CreatureScript
{
    public:

        npc_blood_on_the_snow_farastu() : CreatureScript("npc_blood_on_the_snow_farastu") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_farastuAI(pCreature);
        }

        struct npc_blood_on_the_snow_farastuAI : public ScriptedAI
        {
            npc_blood_on_the_snow_farastuAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_ICE_SPIKES, urand(3000, 7000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    pInstance->SetData(DATA_FARASTU, DONE);
                }
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
                        case EVENT_ICE_SPIKES:
                            DoCastAOE(SPELL_ICE_SPIKES);
                            events.ScheduleEvent(EVENT_ICE_SPIKES, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_blood_on_the_snow_ice_spike : public CreatureScript
{
    public:

        npc_blood_on_the_snow_ice_spike() : CreatureScript("npc_blood_on_the_snow_ice_spike") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_ice_spikeAI(pCreature);
        }

        struct npc_blood_on_the_snow_ice_spikeAI : public Scripted_NoMovementAI
        {
            npc_blood_on_the_snow_ice_spikeAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void IsSummonedBy(Unit* owner)
            {
                events.ScheduleEvent(EVENT_ICE_SPIKES, 2000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ICE_SPIKES:
                            DoCastAOE(SPELL_ICE_SPIKE_DMG);
                            me->DespawnOrUnsummon(1000);
                            break;
                        default:
                            break;
                    }
                }
            }
        };
};

class npc_blood_on_the_snow_hekima_the_wise : public CreatureScript
{
    public:

        npc_blood_on_the_snow_hekima_the_wise() : CreatureScript("npc_blood_on_the_snow_hekima_the_wise") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_blood_on_the_snow_hekima_the_wiseAI(pCreature);
        }

        struct npc_blood_on_the_snow_hekima_the_wiseAI : public ScriptedAI
        {
            npc_blood_on_the_snow_hekima_the_wiseAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_HEKIMAS_BRAND, urand(3000, 7000));
                events.ScheduleEvent(EVENT_HEKIMAS_SCORN, urand(5000, 15000));
            }

            void JustSummoned(Creature* summon)
            {
                if (me->isInCombat())
                    DoZoneInCombat(summon);

                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                if (summon->GetEntry() == NPC_ZANDALARI_RAGE_BANNER)
                {
                    events.ScheduleEvent(EVENT_ZANDALARI_BANNER, urand(5000, 10000));
                }

                summons.Despawn(summon);
            }

            void JustDied(Unit* who)
            {
                events.Reset();
                summons.DespawnAll();

                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    pInstance->SetData(DATA_HEKIMA, DONE);
                    pInstance->SetData(DATA_VILLAGE, DONE);
                }
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
                        case EVENT_ZANDALARI_BANNER:
                            DoCast(me, SPELL_ZANDALARI_BANNER);
                            break;
                        case EVENT_HEKIMAS_BRAND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_HEKIMAS_BRAND);
                            events.ScheduleEvent(EVENT_HEKIMAS_BRAND, urand(15000, 20000));
                            break;
                        case EVENT_HEKIMAS_SCORN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_HEKIMAS_SCORN);
                            events.ScheduleEvent(EVENT_HEKIMAS_SCORN, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

void AddSC_blood_on_the_snow()
{
    new npc_blood_on_the_snow_zandalari_rage_banner();  // 70789
    new npc_blood_on_the_snow_roasting_spit();          // 70597
    new npc_blood_on_the_snow_scout_stonebeard();       // 70602
    new npc_blood_on_the_snow_moira_thaurissan();       // 70279
    new npc_blood_on_the_snow_mountaineer_grimbolt();   // 70801
    new npc_blood_on_the_snow_scout_boldbrew();         // 70744
    new npc_blood_on_the_snow_scout_forgefellow();      // 70745
    new npc_blood_on_the_snow_frostmane_witch_doctor(); // 70464
    new npc_blood_on_the_snow_frostmane_prowler();       // 70595
    new npc_blood_on_the_snow_frostmane_mauler();       // 70684
    new npc_blood_on_the_snow_frostmane_headhunter();   // 70473
    new npc_blood_on_the_snow_frostmane_flesh_eater();  // 70746
    new npc_blood_on_the_snow_frostmane_berserker();    // 70471
    new npc_blood_on_the_snow_frostmane_bonechiller();  // 70465
    new npc_blood_on_the_snow_bonechiller_banafu();     // 70468
    new npc_blood_on_the_snow_skullcrusher_gundu();     // 70787
    new npc_blood_on_the_snow_farastu();                // 70474
    new npc_blood_on_the_snow_ice_spike();              // 70988
    new npc_blood_on_the_snow_hekima_the_wise();        // 70544
}

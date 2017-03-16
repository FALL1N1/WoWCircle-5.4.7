#include "ScriptPCH.h"
#include "theramores_fall_h.h"

enum Spells
{
    // Theramore Footman
    // Captain Tellern
    // Captain Dashing
    // Lieutenant Granders
    // Theramore Officer
    SPELL_CLEAVE                = 15496,

    // 7-fleet Marine
    SPELL_SHIELD_BASH           = 11972,

    // Captain Tellern
    // Hedric Evencane
    SPELL_DANCING_BLADES        = 114449,

    // Mad Morden Morgesen
    SPELL_HURLED_DYNAMITE       = 114443,
    SPELL_DYNAMITE_BLAST        = 131838,

    // 7-fleet Squallshaper
    SPELL_RIPTIDE               = 90399,
    SPELL_WATER_BOLT_VOLLEY     = 37924,

    // Captain Brellinger   
    // Hedric Evencrane
    SPELL_BLADE_BARRAGE         = 114456,
    SPELL_CLEAVE_2              = 15284,

    // Captain Dashing
    SPELL_DASHING_SMILE         = 114480,

    // Lieutenant Granders
    SPELL_BLADED_BASTION        = 114472,
    SPELL_BLADED_BASTION_DMG    = 114476,

    // Squallshaper Linara
    // Theramore Arcanist
    SPELL_AURA_OF_ARCANE_HASTE  = 50182,
    SPELL_ARCANE_HASTE          = 47791,
    SPELL_BLIZZARD              = 79860,
    SPELL_FROST_NOVA            = 79850,
    SPELL_FROST_BOLT            = 79858,
    SPELL_ICE_LANCE             = 79859,

    // Theramore Marksman
    SPELL_SHOOT                 = 15620,

    // Theramore Faithful
    SPELL_HEAL                  = 22883,
    SPELL_HOLY_SMITE            = 25054,
    SPELL_POWER_WORD_SHIELD     = 17139,

    // Theramore Arcanist
    SPELL_ARCANE_BLAST          = 114683,
    SPELL_ARCANE_EXPLOSION      = 33860,
    SPELL_POLYMORPH             = 13323,
    SPELL_SLOW                  = 79880,

    // Knight of Theramore
    SPELL_CHARGE                = 22911,
    SPELL_HEAL_OTHER            = 33910,

    // Baldruc
    SPELL_LIGHTNING_SHIELD      = 131844,
    SPELL_STORM_TOTEM           = 127010,

    // Big Bessa
    SPELL_BEAM_TARGET_STATE     = 114581,
    SPELL_WAR_ENGINES_SIGHT     = 114570,

    // Thalen Songweaver
    SPELL_PORTAL_ORGRIMMAR      = 111633,
};

enum Events
{
    EVENT_CLEAVE    = 1,
    EVENT_DANCING_BLADES,

    EVENT_HURLED_DYNAMITE,

    EVENT_SHIELD_BASH,
    EVENT_BLADED_BASTION,

    EVENT_BLIZZARD,
    EVENT_FROST_BOLT,
    EVENT_FROST_NOVA,
    EVENT_ICE_LANCE,

    EVENT_BLADE_BARRAGE,

    EVENT_DASHING_SMILE,

    EVENT_STORM_TOTEM,

};

enum Actions
{
    ACTION_THALEN_FREED = 1,
};

class npc_theramores_fall_captain_tellern : public CreatureScript
{
    public:

        npc_theramores_fall_captain_tellern() : CreatureScript("npc_theramores_fall_captain_tellern") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_captain_tellernAI(pCreature);
        }

        struct npc_theramores_fall_captain_tellernAI : public ScriptedAI
        {
            npc_theramores_fall_captain_tellernAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CLEAVE, urand(3000, 7000));
                events.ScheduleEvent(EVENT_DANCING_BLADES, urand(10000, 15000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_CAPTAIN_TELLERN, DONE);
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
                    switch(eventId)
                    {
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 15000));
                            break;
                        case EVENT_DANCING_BLADES:
                            DoCastAOE(SPELL_DANCING_BLADES);
                            events.ScheduleEvent(EVENT_DANCING_BLADES, urand(25000, 30000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_rig_this_powder_barrel : public CreatureScript
{
    public:

        npc_theramores_fall_rig_this_powder_barrel() : CreatureScript("npc_theramores_fall_rig_this_powder_barrel") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_rig_this_powder_barrelAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pInstance->SetData(DATA_BARREL, DONE);
                creature->DespawnOrUnsummon(100);
            }

            return true;
        }

        struct npc_theramores_fall_rig_this_powder_barrelAI : public ScriptedAI
        {
            npc_theramores_fall_rig_this_powder_barrelAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        };
};

class npc_theramores_fall_mad_morgen_morgensen : public CreatureScript
{
    public:

        npc_theramores_fall_mad_morgen_morgensen() : CreatureScript("npc_theramores_fall_mad_morgen_morgensen") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_mad_morgen_morgensenAI(pCreature);
        }

        struct npc_theramores_fall_mad_morgen_morgensenAI : public ScriptedAI
        {
            npc_theramores_fall_mad_morgen_morgensenAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_HURLED_DYNAMITE, urand(3000, 5000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_MAD_MORGEN_MORGENSEN, DONE);
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
                    switch(eventId)
                    {
                        case EVENT_HURLED_DYNAMITE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_HURLED_DYNAMITE);
                            events.ScheduleEvent(EVENT_HURLED_DYNAMITE, urand(7000, 10000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_lieutenant_granders : public CreatureScript
{
    public:

        npc_theramores_fall_lieutenant_granders() : CreatureScript("npc_theramores_fall_lieutenant_granders") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_lieutenant_grandersAI(pCreature);
        }

        struct npc_theramores_fall_lieutenant_grandersAI : public ScriptedAI
        {
            npc_theramores_fall_lieutenant_grandersAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_SHIELD_BASH, urand(3000, 5000));
                events.ScheduleEvent(EVENT_BLADED_BASTION, urand(7000, 10000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_LIEUTENANT_GRANDERS, DONE);
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
                    switch(eventId)
                    {
                        case EVENT_SHIELD_BASH:
                            if (Unit* target = me->getVictim())
                            {
                                if (!target->HasUnitState(UNIT_STATE_CASTING))
                                {
                                    events.ScheduleEvent(EVENT_SHIELD_BASH, 1000);
                                    break;
                                }

                                DoCast(target, SPELL_SHIELD_BASH);
                                events.ScheduleEvent(EVENT_SHIELD_BASH, urand(6000, 10000));
                            }
                            break;
                        case EVENT_BLADED_BASTION:
                            DoCast(me, SPELL_BLADED_BASTION);
                            events.ScheduleEvent(EVENT_BLADED_BASTION, urand(15000, 20000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_squallshaper_linara : public CreatureScript
{
    public:

        npc_theramores_fall_squallshaper_linara() : CreatureScript("npc_theramores_fall_squallshaper_linara") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_squallshaper_linaraAI(pCreature);
        }

        struct npc_theramores_fall_squallshaper_linaraAI : public ScriptedAI
        {
            npc_theramores_fall_squallshaper_linaraAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                DoCast(me, SPELL_AURA_OF_ARCANE_HASTE);

                events.ScheduleEvent(EVENT_FROST_BOLT, 100);
                events.ScheduleEvent(EVENT_FROST_NOVA, urand(5000, 7000));
                events.ScheduleEvent(EVENT_ICE_LANCE, urand(3000, 6000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_SQUALLSHAPER_LINARA, DONE);
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
                    switch(eventId)
                    {
                        case EVENT_FROST_BOLT:
                            DoCastVictim(SPELL_FROST_BOLT);
                            events.ScheduleEvent(EVENT_FROST_BOLT, 3000);
                            break;
                        case EVENT_FROST_NOVA:
                            DoCastAOE(SPELL_FROST_NOVA);
                            events.ScheduleEvent(EVENT_FROST_NOVA, urand(8000, 14000));
                            break;
                        case EVENT_ICE_LANCE:
                            DoCastVictim(SPELL_ICE_LANCE);
                            events.ScheduleEvent(EVENT_ICE_LANCE, urand(5000, 7000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_captain_brellinger : public CreatureScript
{
    public:

        npc_theramores_fall_captain_brellinger() : CreatureScript("npc_theramores_fall_captain_brellinger") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_captain_brellingerAI(pCreature);
        }

        struct npc_theramores_fall_captain_brellingerAI : public ScriptedAI
        {
            npc_theramores_fall_captain_brellingerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_CLEAVE, urand(3000, 7000));
                events.ScheduleEvent(EVENT_BLADE_BARRAGE, urand(7000, 10000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_CAPTAIN_BRELLINGER, DONE);
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
                    switch(eventId)
                    {
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 15000));
                            break;
                        case EVENT_BLADE_BARRAGE:
                            DoCast(me, SPELL_BLADE_BARRAGE);
                            events.ScheduleEvent(EVENT_BLADE_BARRAGE, urand(15000, 20000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_captain_dashing : public CreatureScript
{
    public:

        npc_theramores_fall_captain_dashing() : CreatureScript("npc_theramores_fall_captain_dashing") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_captain_dashingAI(pCreature);
        }

        struct npc_theramores_fall_captain_dashingAI : public ScriptedAI
        {
            npc_theramores_fall_captain_dashingAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_CLEAVE, urand(3000, 7000));
                events.ScheduleEvent(EVENT_DASHING_SMILE, urand(10000, 15000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_CAPTAIN_DASHING, DONE);
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
                    switch(eventId)
                    {
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 15000));
                            break;
                        case EVENT_DASHING_SMILE:
                            DoCastAOE(SPELL_DASHING_SMILE);
                            events.ScheduleEvent(EVENT_DASHING_SMILE, urand(20000, 30000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_blastmaster_blackfuse : public CreatureScript
{
    public:

        npc_theramores_fall_blastmaster_blackfuse() : CreatureScript("npc_theramores_fall_blastmaster_blackfuse") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_blastmaster_blackfuseAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {            
                if (pInstance->GetData(DATA_CURRENT_STEP) != 1)
                    return true;

                pInstance->SetData(DATA_BLACKFUSE, 1);

                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            return true;
        }

        struct npc_theramores_fall_blastmaster_blackfuseAI : public ScriptedAI
        {
            npc_theramores_fall_blastmaster_blackfuseAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        };
};

class npc_theramores_fall_baldruc : public CreatureScript
{
    public:

        npc_theramores_fall_baldruc() : CreatureScript("npc_theramores_fall_baldruc") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_baldrucAI(pCreature);
        }

        struct npc_theramores_fall_baldrucAI : public ScriptedAI
        {
            npc_theramores_fall_baldrucAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_LIGHTNING_SHIELD);

                events.ScheduleEvent(EVENT_STORM_TOTEM, urand(3000, 6000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_BALDRUC, DONE);
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
                    switch(eventId)
                    {
                        case EVENT_STORM_TOTEM:
                            DoCast(me, SPELL_STORM_TOTEM);
                            events.ScheduleEvent(EVENT_STORM_TOTEM, urand(20000, 25000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_theramore_gryphon : public CreatureScript
{
    public:

        npc_theramores_fall_theramore_gryphon() : CreatureScript("npc_theramores_fall_theramore_gryphon") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_theramore_gryphonAI(pCreature);
        }

        struct npc_theramores_fall_theramore_gryphonAI : public ScriptedAI
        {
            npc_theramores_fall_theramore_gryphonAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_GRYPHONS, DONE);
                }
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_big_bessa : public CreatureScript
{
    public:

        npc_theramores_fall_big_bessa() : CreatureScript("npc_theramores_fall_big_bessa") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_big_bessaAI(pCreature);
        }

        struct npc_theramores_fall_big_bessaAI : public ScriptedAI
        {
            npc_theramores_fall_big_bessaAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_BIG_BESSA, DONE);
                }
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_unmanned_tank : public CreatureScript
{
    public:

        npc_theramores_fall_unmanned_tank() : CreatureScript("npc_theramores_fall_unmanned_tank") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_unmanned_tankAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pInstance->SetData(DATA_TANKS, DONE);
                creature->DespawnOrUnsummon(100);
            }

            return true;
        }

        struct npc_theramores_fall_unmanned_tankAI : public ScriptedAI
        {
            npc_theramores_fall_unmanned_tankAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        };
};

class npc_theramores_fall_hedric_evencane : public CreatureScript
{
    public:

        npc_theramores_fall_hedric_evencane() : CreatureScript("npc_theramores_fall_hedric_evencane") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_hedric_evencaneAI(pCreature);
        }

        struct npc_theramores_fall_hedric_evencaneAI : public ScriptedAI
        {
            npc_theramores_fall_hedric_evencaneAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_HEDRIC, DONE);
                    pInstance->SetData(DATA_KEY, DONE);
                }
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_thalen_songweaver : public CreatureScript
{
    public:

        npc_theramores_fall_thalen_songweaver() : CreatureScript("npc_theramores_fall_thalen_songweaver") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_thalen_songweaverAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {        
                if (pInstance->GetData(DATA_CURRENT_STEP) < 3)
                    return true;

                if (player->HasItemCount(79261))
                {
                    player->DestroyItemCount(79261, 1, true);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    

                    pInstance->SetData(DATA_THALEN, DONE);
                }
            }

            return true;
        }

        struct npc_theramores_fall_thalen_songweaverAI : public ScriptedAI
        {
            npc_theramores_fall_thalen_songweaverAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                isTalked = false;
                isFreed = false;
            }

            void Reset()
            {
                events.Reset();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_THALEN_FREED)
                {
                    if (!isFreed)
                    {
                        isFreed = true;
                        DoCast(me, SPELL_PORTAL_ORGRIMMAR);
                        me->DespawnOrUnsummon(15000);
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                /*if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_JAINA_TALK_1:
                            Talk(SAY_JAINA_1);
                            events.ScheduleEvent(EVENT_JAINA_TALK_2, 5000);
                            break;
                        case EVENT_JAINA_TALK_2:
                            Talk(SAY_JAINA_2);
                            if (InstanceScript* pInstance = me->GetInstanceScript())
                            {
                                pInstance->SetData(DATA_JAINA_LOCATED, DONE);
                            }
                            break;
                    }
                }*/
            }

        private:

            bool isTalked;
            bool isFreed;
        };
};

void AddSC_theramores_fall_h()
{
    new npc_theramores_fall_captain_tellern();          // 59088
    new npc_theramores_fall_rig_this_powder_barrel();   // 58665
    new npc_theramores_fall_mad_morgen_morgensen();     // 59086
    new npc_theramores_fall_lieutenant_granders();      // 58936
    new npc_theramores_fall_squallshaper_linara();      // 58948
    new npc_theramores_fall_captain_brellinger();       // 59087
    new npc_theramores_fall_captain_dashing();          // 59089
    new npc_theramores_fall_blastmaster_blackfuse();    // 58765
    new npc_theramores_fall_baldruc();                  // 58777
    new npc_theramores_fall_theramore_gryphon();        // 58782
    new npc_theramores_fall_big_bessa();                // 58787
    new npc_theramores_fall_unmanned_tank();            // 58788
    new npc_theramores_fall_hedric_evencane();          // 58840
    new npc_theramores_fall_thalen_songweaver();        // 58816

}

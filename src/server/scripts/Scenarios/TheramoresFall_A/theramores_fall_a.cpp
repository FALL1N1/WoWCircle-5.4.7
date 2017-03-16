#include "ScriptPCH.h"
#include "theramores_fall_a.h"

enum ScriptedTexts
{
    SAY_JAINA_1 = 0,
    SAY_JAINA_2 = 1,
};

enum Spells
{
    // Sergeant Grud
    // Rok'nah Grunt
    // Sky-Captain Dazrip
    // Roj'nah Rider
    // Captain Korthok
    SPELL_CLEAVE            = 15496,

    // Wave-Caller Zulga
    SPELL_EARTH_SHIELD      = 79927,
    SPELL_EARTH_SHIELD_HEAL = 79928,
    SPELL_EARTHBIND_TOTEM   = 129411,
    SPELL_FROST_SHOCK       = 79925,
    SPELL_HEALING_WAVE      = 15982,

    // Rok'nah Felcaster
    SPELL_CHAOS_BOLT        = 79939,
    SPELL_FEL_ARMOR         = 79954,
    SPELL_FEL_IMMOLATE      = 84469,
    SPELL_SUMMON_IMP        = 129409,

    // Airship Marine
    SPELL_SHOOT             = 15620,

    // Airship Crewman
    SPELL_SNAP_KICK         = 15618,

    // Sky-Captain Dazrip
    SPELL_DASHING_SMILE     = 114480,

    // Overseer Lurguk
    // Rok'nah Headhunter
    SPELL_THROW             = 15607,

    // Rok'nah Rider
    SPELL_CHARGE            = 22911,

    // Rok'nah Loa-Singer
    SPELL_HEAL              = 22883,
    SPELL_HOLY_SMITE        = 25054,
    SPELL_POWER_WORD_SHIELD = 17139,

    // Blood Guard Grunk
    SPELL_BLADED_BASTION    = 114472,
    SPELL_BLADED_BASTION_DMG    = 114476,
    SPELL_SHIELD_BASH       = 11972,

    // Captain Korthok
    SPELL_DANCING_BLADES    = 114449,

    // Captain Mousson
    SPELL_AURA_OF_ARCANE_HASTE  = 50182,
    SPELL_ARCANE_HASTE          = 47791,
    SPELL_BLIZZARD              = 79860,
    SPELL_FROST_NOVA            = 79850,
    SPELL_FROST_BOLT            = 79858,
    SPELL_ICE_LANCE             = 79859,
    SPELL_SUMMON_WATER_ELEMENTAL    = 17162,

    // Captain Seahoof
    SPELL_BLADE_BARRAGE     = 114456,

    // Vicious Wyvern
    SPELL_PRIDEWING_POISON  = 81376,

    // Gash'nul
    SPELL_LIGHTNING_SHIELD  = 131844,
    SPELL_STORM_TOTEM       = 127010,

    // Gatecrusher
    SPELL_BEAM_TARGET_STATE = 114581,
    SPELL_WAR_ENGINES_SIGHT = 114570,
    SPELL_DEMOLISHER_SHOT   = 125914,
    SPELL_DEMOLISHER_SHOT_DMG   = 125915,

    // Jaina Proudmoore
    SPELL_PORTAL_STORMWIND  = 129467,
};

enum Events
{
    // Sergeant Grud
    // Rok'nah Grunt
    // Sky-Captain Dazrip
    // Roj'nah Rider
    // Captain Korthok
    EVENT_CLEAVE    = 1,

    // Wave-Caller Zulga
    EVENT_EARTHBIND_TOTEM,
    EVENT_FROST_SHOCK,
    EVENT_HEALING_WAVE,

    // Rok'nah Felcaster
    EVENT_CHAOS_BOLT,
    EVENT_FEL_IMMOLATE,
    EVENT_SUMMON_IMP,

    // Airship Crewman
    EVENT_SNAP_KICK,

    // Sky-Captain Dazrip
    EVENT_DASHING_SMILE,

    // Rok'nah Loa-Singer
    EVENT_HEAL,
    EVENT_HOLY_SMITE,
    EVENT_POWER_WORD_SHIELD,

    // Blood Guard Grunk
    EVENT_BLADED_BASTION,
    EVENT_SHIELD_BASH,

    // Captain Korthok
    EVENT_DANCING_BLADES,

    // Captain Mousson
    EVENT_BLIZZARD,
    EVENT_FROST_BOLT,
    EVENT_FROST_NOVA,
    EVENT_ICE_LANCE,

    // Captain Seahoof
    EVENT_BLADE_BARRAGE,

    // Vicious Wyvern
    EVENT_PRIDEWING_POISON,

    // Gash'nul
    EVENT_STORM_TOTEM,

    // Lady Jaina Proudmoore
    EVENT_JAINA_TALK_1,
    EVENT_JAINA_TALK_2,

    // Gatecrusher
    EVENT_DEMOLISHER_SHOT,
};

enum Actions
{
    ACTION_START_TALK   = 1,
};

class npc_theramores_fall_sergeant_grud : public CreatureScript
{
    public:

        npc_theramores_fall_sergeant_grud() : CreatureScript("npc_theramores_fall_sergeant_grud") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_sergeant_grudAI(pCreature);
        }

        struct npc_theramores_fall_sergeant_grudAI : public ScriptedAI
        {
            npc_theramores_fall_sergeant_grudAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();

                isTalked = false;
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_CLEAVE, urand(3000, 5000));
            }

            void JustDied(Unit* /*killer*/)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_SERGEANT_GRUD, DONE);
                }
            }

            void DamageTaken(Unit* who, uint32& damage)
            {
                if (!isTalked)
                {
                    if (me->GetHealthPct() <= 50.0f)
                    {
                        isTalked = true;
                        Talk(0);
                    }
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
                    if (eventId == EVENT_CLEAVE)
                    {
                        if (Unit* l_Victim = me->getVictim())
                            DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 10000));
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;
            bool isTalked;

        };
};

class npc_theramores_fall_roknah_grunt : public CreatureScript
{
    public:

        npc_theramores_fall_roknah_grunt() : CreatureScript("npc_theramores_fall_roknah_grunt") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_roknah_gruntAI(pCreature);
        }

        struct npc_theramores_fall_roknah_gruntAI : public ScriptedAI
        {
            npc_theramores_fall_roknah_gruntAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                if (roll_chance_i(10))
                {
                    Talk(0);
                }
                events.ScheduleEvent(EVENT_CLEAVE, urand(3000, 5000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_ROKNAH_KILLED, DONE);
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
                    if (eventId == EVENT_CLEAVE)
                    {
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 10000));
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_wave_caller_zulga : public CreatureScript
{
    public:

        npc_theramores_fall_wave_caller_zulga() : CreatureScript("npc_theramores_fall_wave_caller_zulga") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_wave_caller_zulgaAI(pCreature);
        }

        struct npc_theramores_fall_wave_caller_zulgaAI : public ScriptedAI
        {
            npc_theramores_fall_wave_caller_zulgaAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_EARTH_SHIELD);

                events.ScheduleEvent(EVENT_FROST_SHOCK, urand(5000, 8000));
                events.ScheduleEvent(EVENT_HEALING_WAVE, urand(10000, 15000));
                events.ScheduleEvent(EVENT_EARTHBIND_TOTEM, urand(10000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_ROKNAH_KILLED, DONE);
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
                        case EVENT_FROST_SHOCK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_FROST_SHOCK);
                            events.ScheduleEvent(EVENT_FROST_SHOCK, urand(8000, 15000));
                            break;
                        case EVENT_EARTHBIND_TOTEM:
                            DoCast(me, SPELL_EARTHBIND_TOTEM);
                            events.ScheduleEvent(EVENT_EARTHBIND_TOTEM, urand(30000, 45000));
                            break;
                        case EVENT_HEALING_WAVE:
                            DoCast(me, SPELL_HEALING_WAVE);
                            events.ScheduleEvent(EVENT_HEALING_WAVE, urand(8000, 15000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_roknah_felcaster : public CreatureScript
{
    public:

        npc_theramores_fall_roknah_felcaster() : CreatureScript("npc_theramores_fall_roknah_felcaster") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_roknah_felcasterAI(pCreature);
        }

        struct npc_theramores_fall_roknah_felcasterAI : public ScriptedAI
        {
            npc_theramores_fall_roknah_felcasterAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_FEL_ARMOR);

                events.ScheduleEvent(EVENT_CHAOS_BOLT, 100);
                events.ScheduleEvent(EVENT_FEL_IMMOLATE, urand(3000, 5000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_ROKNAH_KILLED, DONE);
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
                        case EVENT_CHAOS_BOLT:
                            DoCastVictim(SPELL_CHAOS_BOLT);
                            events.ScheduleEvent(EVENT_CHAOS_BOLT, 3000);
                            break;
                        case EVENT_FEL_IMMOLATE:
                            DoCastVictim(SPELL_FEL_IMMOLATE);
                            events.ScheduleEvent(EVENT_FEL_IMMOLATE, urand(8000, 10000));
                            break;
                    }
                }
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_leaking_oil_drum : public CreatureScript
{
    public:

        npc_theramores_fall_leaking_oil_drum() : CreatureScript("npc_theramores_fall_leaking_oil_drum") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_leaking_oil_drumAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pInstance->SetData(DATA_LEAKING_OIL_DRUM, DONE);
                creature->DespawnOrUnsummon(100);
            }

            return true;
        }

        struct npc_theramores_fall_leaking_oil_drumAI : public ScriptedAI
        {
            npc_theramores_fall_leaking_oil_drumAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        };
};

class npc_theramores_fall_airship_crewman : public CreatureScript
{
    public:

        npc_theramores_fall_airship_crewman() : CreatureScript("npc_theramores_fall_airship_crewman") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_airship_crewmanAI(pCreature);
        }

        struct npc_theramores_fall_airship_crewmanAI : public ScriptedAI
        {
            npc_theramores_fall_airship_crewmanAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_SNAP_KICK, urand(5000, 10000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_ROKNAH_KILLED, DONE);
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
                        case EVENT_SNAP_KICK:
                            DoCastVictim(SPELL_SNAP_KICK);
                            events.ScheduleEvent(EVENT_SNAP_KICK, urand(10000, 15000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_sky_captain_dazrip : public CreatureScript
{
    public:

        npc_theramores_fall_sky_captain_dazrip() : CreatureScript("npc_theramores_fall_sky_captain_dazrip") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_sky_captain_dazripAI(pCreature);
        }

        struct npc_theramores_fall_sky_captain_dazripAI : public ScriptedAI
        {
            npc_theramores_fall_sky_captain_dazripAI(Creature* creature) : ScriptedAI(creature)
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

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_SKY_CAPTAIN_DAZRIP, DONE);
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

class npc_theramores_fall_roknah_rider : public CreatureScript
{
    public:

        npc_theramores_fall_roknah_rider() : CreatureScript("npc_theramores_fall_roknah_rider") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_roknah_riderAI(pCreature);
        }

        struct npc_theramores_fall_roknah_riderAI : public ScriptedAI
        {
            npc_theramores_fall_roknah_riderAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                DoCast(who, SPELL_CHARGE);

                events.ScheduleEvent(EVENT_CLEAVE, urand(3000, 7000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_ROKNAH_KILLED, DONE);
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
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_roknah_loa_singer : public CreatureScript
{
    public:

        npc_theramores_fall_roknah_loa_singer() : CreatureScript("npc_theramores_fall_roknah_loa_singer") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_roknah_loa_singerAI(pCreature);
        }

        struct npc_theramores_fall_roknah_loa_singerAI : public ScriptedAI
        {
            npc_theramores_fall_roknah_loa_singerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_HOLY_SMITE, 100);
                events.ScheduleEvent(EVENT_HEAL, urand(8000, 10000));
                events.ScheduleEvent(EVENT_POWER_WORD_SHIELD, urand(3000, 6000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_ROKNAH_KILLED, DONE);
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
                        case EVENT_HOLY_SMITE:
                            DoCastVictim(SPELL_HOLY_SMITE);
                            events.ScheduleEvent(EVENT_HOLY_SMITE, 2500);
                            break;
                        case EVENT_HEAL:
                            DoCast(me, SPELL_HEAL);
                            events.ScheduleEvent(EVENT_HEAL, urand(10000, 15000));
                            break;
                        case EVENT_POWER_WORD_SHIELD:
                            DoCast(me, SPELL_POWER_WORD_SHIELD);
                            events.ScheduleEvent(EVENT_POWER_WORD_SHIELD, urand(30000, 35000));
                            break;
                    }
                }
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_blood_guard_grunk : public CreatureScript
{
    public:

        npc_theramores_fall_blood_guard_grunk() : CreatureScript("npc_theramores_fall_blood_guard_grunk") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_blood_guard_grunkAI(pCreature);
        }

        struct npc_theramores_fall_blood_guard_grunkAI : public ScriptedAI
        {
            npc_theramores_fall_blood_guard_grunkAI(Creature* creature) : ScriptedAI(creature)
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

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_ROKNAH_KILLED, DONE);
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

class npc_theramores_fall_captain_korthok : public CreatureScript
{
    public:

        npc_theramores_fall_captain_korthok() : CreatureScript("npc_theramores_fall_captain_korthok") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_captain_korthokAI(pCreature);
        }

        struct npc_theramores_fall_captain_korthokAI : public ScriptedAI
        {
            npc_theramores_fall_captain_korthokAI(Creature* creature) : ScriptedAI(creature)
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

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_CAPTAIN_KORTHOK, DONE);
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

class npc_theramores_fall_captain_mousson : public CreatureScript
{
    public:

        npc_theramores_fall_captain_mousson() : CreatureScript("npc_theramores_fall_captain_mousson") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_captain_moussonAI(pCreature);
        }

        struct npc_theramores_fall_captain_moussonAI : public ScriptedAI
        {
            npc_theramores_fall_captain_moussonAI(Creature* creature) : ScriptedAI(creature)
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

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_CAPTAIN_MOUSSON, DONE);
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
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_captain_seahoof : public CreatureScript
{
    public:

        npc_theramores_fall_captain_seahoof() : CreatureScript("npc_theramores_fall_captain_seahoof") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_captain_seahoofAI(pCreature);
        }

        struct npc_theramores_fall_captain_seahoofAI : public ScriptedAI
        {
            npc_theramores_fall_captain_seahoofAI(Creature* creature) : ScriptedAI(creature)
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

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_CAPTAIN_SEAHOOF, DONE);
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

class npc_theramores_fall_roknah_wave_caller : public CreatureScript
{
    public:

        npc_theramores_fall_roknah_wave_caller() : CreatureScript("npc_theramores_fall_roknah_wave_caller") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_roknah_wave_callerAI(pCreature);
        }

        struct npc_theramores_fall_roknah_wave_callerAI : public ScriptedAI
        {
            npc_theramores_fall_roknah_wave_callerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoCast(me, SPELL_EARTH_SHIELD);

                events.ScheduleEvent(EVENT_FROST_SHOCK, urand(5000, 8000));
                events.ScheduleEvent(EVENT_HEALING_WAVE, urand(10000, 15000));
                events.ScheduleEvent(EVENT_EARTHBIND_TOTEM, urand(10000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_ROKNAH_KILLED, DONE);
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
                        case EVENT_FROST_SHOCK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_FROST_SHOCK);
                            events.ScheduleEvent(EVENT_FROST_SHOCK, urand(8000, 15000));
                            break;
                        case EVENT_EARTHBIND_TOTEM:
                            DoCast(me, SPELL_EARTHBIND_TOTEM);
                            events.ScheduleEvent(EVENT_EARTHBIND_TOTEM, urand(30000, 45000));
                            break;
                        case EVENT_HEALING_WAVE:
                            DoCast(me, SPELL_HEALING_WAVE);
                            events.ScheduleEvent(EVENT_HEALING_WAVE, urand(8000, 15000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_vicious_wyvern : public CreatureScript
{
    public:

        npc_theramores_fall_vicious_wyvern() : CreatureScript("npc_theramores_fall_vicious_wyvern") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_vicious_wyvernAI(pCreature);
        }

        struct npc_theramores_fall_vicious_wyvernAI : public ScriptedAI
        {
            npc_theramores_fall_vicious_wyvernAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(0);

                events.ScheduleEvent(EVENT_PRIDEWING_POISON, urand(3000, 7000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_VICIOUS_WYVERN, DONE);
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
                        case EVENT_PRIDEWING_POISON:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_PRIDEWING_POISON);
                            events.ScheduleEvent(EVENT_PRIDEWING_POISON, urand(7000, 10000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_gashnul : public CreatureScript
{
    public:

        npc_theramores_fall_gashnul() : CreatureScript("npc_theramores_fall_gashnul") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_gashnulAI(pCreature);
        }

        struct npc_theramores_fall_gashnulAI : public ScriptedAI
        {
            npc_theramores_fall_gashnulAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(0);

                DoCast(me, SPELL_LIGHTNING_SHIELD);

                events.ScheduleEvent(EVENT_STORM_TOTEM, urand(3000, 6000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_GASHNUL, DONE);
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

class npc_theramores_fall_lady_jaina_proudmoore : public CreatureScript
{
    public:

        npc_theramores_fall_lady_jaina_proudmoore() : CreatureScript("npc_theramores_fall_lady_jaina_proudmoore") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_lady_jaina_proudmooreAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                
                if (creature->IsAIEnabled && creature->AI())
                    creature->AI()->DoAction(ACTION_START_TALK);
            }

            return true;
        }

        struct npc_theramores_fall_lady_jaina_proudmooreAI : public ScriptedAI
        {
            npc_theramores_fall_lady_jaina_proudmooreAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                isTalked = false;
            }

            void Reset()
            {
                events.Reset();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_START_TALK && !isTalked)
                {
                     events.ScheduleEvent(EVENT_JAINA_TALK_1, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
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
                }
            }

        private:

            bool isTalked;
        };
};

class npc_theramores_fall_gatecrusher : public CreatureScript
{
    public:

        npc_theramores_fall_gatecrusher() : CreatureScript("npc_theramores_fall_gatecrusher") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_gatecrusherAI(pCreature);
        }

        struct npc_theramores_fall_gatecrusherAI : public ScriptedAI
        {
            npc_theramores_fall_gatecrusherAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_DEMOLISHER_SHOT, urand(3000, 5000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_GATECRUSHER_DESTROYED, DONE);
                    pInstance->SetData(DATA_STANDARD, DONE);
                    pInstance->SetData(DATA_STANDARD, DONE);
                    pInstance->SetData(DATA_STANDARD, DONE);
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
                        case EVENT_DEMOLISHER_SHOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                me->CastSpell(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), SPELL_DEMOLISHER_SHOT, false);
                            }
                            events.ScheduleEvent(EVENT_DEMOLISHER_SHOT, urand(7000, 10000));
                            break; 
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            InstanceScript* pInstance;

        };
};

class npc_theramores_fall_warlord_roknah : public CreatureScript
{
    public:

        npc_theramores_fall_warlord_roknah() : CreatureScript("npc_theramores_fall_warlord_roknah") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_theramores_fall_warlord_roknahAI(pCreature);
        }

        struct npc_theramores_fall_warlord_roknahAI : public ScriptedAI
        {
            npc_theramores_fall_warlord_roknahAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 8000));
                events.ScheduleEvent(EVENT_BLADE_BARRAGE, urand(15000, 25000));
                events.ScheduleEvent(EVENT_BLADED_BASTION, urand(10000, 25000));
                events.ScheduleEvent(EVENT_DANCING_BLADES, urand(10000, 30000));
                events.ScheduleEvent(EVENT_SHIELD_BASH, urand(3000, 7000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_WARLORD, DONE);
                    pInstance->SetData(DATA_JAINA_PROTECTED, DONE);
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

void AddSC_theramores_fall_a()
{
    new npc_theramores_fall_sergeant_grud();        // 65785
    new npc_theramores_fall_roknah_grunt();         // 64732
    new npc_theramores_fall_wave_caller_zulga();    // 65786
    new npc_theramores_fall_roknah_felcaster();     // 65507
    new npc_theramores_fall_leaking_oil_drum();     // 65571
    new npc_theramores_fall_airship_crewman();      // 65610
    new npc_theramores_fall_sky_captain_dazrip();   // 65609
    new npc_theramores_fall_roknah_rider();         // 64729
    new npc_theramores_fall_roknah_loa_singer();    // 64733
    new npc_theramores_fall_blood_guard_grunk();    // 65154
    new npc_theramores_fall_captain_korthok();      // 65152
    new npc_theramores_fall_captain_mousson();      // 65444
    new npc_theramores_fall_captain_seahoof();      // 65151
    new npc_theramores_fall_roknah_wave_caller();   // 65510
    new npc_theramores_fall_vicious_wyvern();       // 64957
    new npc_theramores_fall_gashnul();              // 64900
    new npc_theramores_fall_lady_jaina_proudmoore();// 64727
    new npc_theramores_fall_gatecrusher();          // 64479
    new npc_theramores_fall_warlord_roknah();       // 65442
}

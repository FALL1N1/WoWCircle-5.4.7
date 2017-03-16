#include "ScriptPCH.h"
#include "assault_on_zanvess.h"

enum ScriptedTextsRogers
{
    SAY_ROGERS_SHIELD_DOWN = 0,
};

enum ScriptedTextsZigg
{
    SAY_ZIGG_SHIELD_DOWN = 0,
};

enum ScriptedTextsTelvrak
{
    SAY_TELVRAK_AGGRO   = 0,
};

enum Spells
{
    // Shield Channel Point
    SPELL_GUIDED_MISSILE_MISSILE    = 135546,
    SPELL_GUIDED_MISSILE_SCRIPT     = 135547,

   // 7th Legion Priest and Kor'kron Priest
    SPELL_HEAL                      = 134003,
    SPELL_HOLY_SMITE                = 134004,
    SPELL_SHADOW_WORD_PAIN          = 134006,

    // 7th Legion Infiltrator and Kor'kron Infiltrator
    SPELL_RIPOSTE                   = 133998,
    SPELL_RUSH                      = 134002,

    // Team Leader Scooter and Squad Leader Bosh
    SPELL_DEVASTATING_SMASH         = 133817,
    SPELL_DEVASTATING_SMASH_AURA    = 133819,
    SPELL_DEVASTATING_SMASH_DMG     = 133820,
    SPELL_FIXATE                    = 133843,
    SPELL_WHIRLWIND                 = 133845,
    SPELL_WHIRLWIND_DMG             = 133847,

    // Zan'thik Brutalizer
    SPELL_STOMP                     = 133859,

    // Zan'thik Ambermancer
    SPELL_GIFT_OF_AMBER             = 125021,
    SPELL_SONIC_SHIELD              = 125001,

    // Commander Tel'vrak
    SPELL_SWAP_WEAPON               = 133966,
    SPELL_BLADEMASTER               = 133969,
    SPELL_IMPALE                    = 133942,
    SPELL_SWIFTNESS                 = 133970,
    SPELL_FLURRY                    = 133964,
    SPELL_BOMBER                    = 133968,
    SPELL_BOMBS                     = 133922,
    SPELL_BOMB_MISSILE              = 133923,
    SPELL_BOMB_DMG                  = 133925,
};

enum Events
{
    // 7th Legion Priest and Kor'kron Priest
    EVENT_HEAL  = 1,
    EVENT_HOLY_SMITE,
    EVENT_SHADOW_WORD_PAIN,

    // 7th Legion Infiltrator and Kor'kron Infiltrator
    EVENT_RIPOSTE,
    EVENT_RUSH,

    // Team Leader Scooter and Squad Leader Bosh
    EVENT_DEVASTATING_SMASH,
    EVENT_FIXATE,

    // Zan'thik Brutalizer
    EVENT_STOMP,

    // Zan'thik Ambermancer
    EVENT_GIFT_OF_AMBER,
    EVENT_SONIC_SHIELD,

    // Commander Tel'vrak
    EVENT_SWAP_WEAPON,
    EVENT_IMPALE,
    EVENT_BOMBER,
    EVENT_BOMBER_END,
};

enum Actions
{
    ACTION_SWAP_WEAPON  = 1,
};

enum Equipments
{
    ITEM_AMBER_FLAMMARD_OF_KLAXXIVESS   = 89398,
    ITEM_AMBER_SLICER_OF_KLAXXIVES      = 89393,
};

enum Adds
{
    NPC_SHIELD_CHANNEL_POINT    = 68105,
    NPC_STRAFING_RUN            = 67838,
};

class npc_assault_on_zanvess_sky_admiral_rogers : public CreatureScript
{
    public:

        npc_assault_on_zanvess_sky_admiral_rogers() : CreatureScript("npc_assault_on_zanvess_sky_admiral_rogers") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_sky_admiral_rogersAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            // don't show menu
            return true;
        }

        struct npc_assault_on_zanvess_sky_admiral_rogersAI : public ScriptedAI
        {
            npc_assault_on_zanvess_sky_admiral_rogersAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
                isJoinedTheAttack = false;

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void Reset() override
            {
                events.Reset();
            }

            void sGossipHello(Player* player)
            {
                ProcessPlayerClicked(player->GetGUID());
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_TOWERS_DESTROYED)
                {
                    Talk(SAY_ROGERS_SHIELD_DOWN);
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
                        default:
                            break;
                    }
                }
            }

        private:

            void ProcessPlayerClicked(uint64 playerGuid)
            {
                if (playersClicked.find(playerGuid) == playersClicked.end())
                    playersClicked.insert(playerGuid);

                if (playersClicked.size() >= 1)
                {
                    SendJoinedTheAttack();
                }
            }

            void SendJoinedTheAttack()
            {
                if (!pInstance)
                    return;

                if (!isJoinedTheAttack)
                {
                    isJoinedTheAttack = true;

                    pInstance->SetData(DATA_JOINED_THE_ATTACK, 1);
                }
            }

        private:

            InstanceScript* pInstance;
            std::set<uint64> playersClicked;
            bool isJoinedTheAttack;

        };
};

class npc_assault_on_zanvess_rear_admiral_zigg : public CreatureScript
{
    public:

        npc_assault_on_zanvess_rear_admiral_zigg() : CreatureScript("npc_assault_on_zanvess_rear_admiral_zigg") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_rear_admiral_ziggAI(pCreature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            // don't show menu
            return true;
        }

        struct npc_assault_on_zanvess_rear_admiral_ziggAI : public ScriptedAI
        {
            npc_assault_on_zanvess_rear_admiral_ziggAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
                isJoinedTheAttack = false;

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void Reset() override
            {
                events.Reset();
            }

            void sGossipHello(Player* player)
            {
                ProcessPlayerClicked(player->GetGUID());
            }

            void JustDied(Unit* who)
            {
                events.Reset();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_TOWERS_DESTROYED)
                {
                    Talk(SAY_ZIGG_SHIELD_DOWN);
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
                        default:
                            break;
                    }
                }
            }

        private:

            void ProcessPlayerClicked(uint64 playerGuid)
            {
                if (playersClicked.find(playerGuid) == playersClicked.end())
                    playersClicked.insert(playerGuid);

                if (playersClicked.size() >= 1)
                {
                    SendJoinedTheAttack();
                }
            }

            void SendJoinedTheAttack()
            {
                if (!pInstance)
                    return;

                if (!isJoinedTheAttack)
                {
                    isJoinedTheAttack = true;

                    pInstance->SetData(DATA_JOINED_THE_ATTACK, 1);
                }
            }

        private:

            InstanceScript* pInstance;
            std::set<uint64> playersClicked;
            bool isJoinedTheAttack;

        };
};

class npc_assault_on_zanvess_skyfire_gyrocopter : public CreatureScript
{
    public:

        npc_assault_on_zanvess_skyfire_gyrocopter() : CreatureScript("npc_assault_on_zanvess_skyfire_gyrocopter") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_skyfire_gyrocopterAI(pCreature);
        }

        struct npc_assault_on_zanvess_skyfire_gyrocopterAI : public ScriptedAI
        {
            npc_assault_on_zanvess_skyfire_gyrocopterAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
                isTowersDestroyed = false;

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void Reset()
            {
                events.Reset();
            }

            void sGossipHello(Player* player)
            {
                ProcessPlayerClicked(player->GetGUID());
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
                        default:
                            break;
                    }
                }
            }

        private:

            void ProcessPlayerClicked(uint64 playerGuid)
            {
                if (pInstance)
                {
                    if (pInstance->GetData(DATA_JOINED_THE_ATTACK) != DONE)
                        return;
                }

                SendDestroyTowers();

                if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
                    player->NearTeleportTo(beachPos);
            }

            void SendDestroyTowers()
            {
                if (!pInstance)
                    return;

                if (!isTowersDestroyed)
                {
                    isTowersDestroyed = true;

                    pInstance->SetData(DATA_DESTROYED_TOWERS_1, 1);
                    pInstance->SetData(DATA_DESTROYED_TOWERS_2, 1);
                    pInstance->SetData(DATA_DESTROYED_TOWERS_3, 1);
                }
            }

        private:

            InstanceScript* pInstance;
            std::set<uint64> playersClicked;
            bool isTowersDestroyed;
        };
};

class npc_assault_on_zanvess_korkron_gunship : public CreatureScript
{
    public:

        npc_assault_on_zanvess_korkron_gunship() : CreatureScript("npc_assault_on_zanvess_korkron_gunship") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_korkron_gunshipAI(pCreature);
        }

        struct npc_assault_on_zanvess_korkron_gunshipAI : public ScriptedAI
        {
            npc_assault_on_zanvess_korkron_gunshipAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
                isTowersDestroyed = false;

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void Reset()
            {
                events.Reset();
            }

            void sGossipHello(Player* player)
            {
                ProcessPlayerClicked(player->GetGUID());
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
                        default:
                            break;
                    }
                }
            }

        private:

            void ProcessPlayerClicked(uint64 playerGuid)
            {
                if (pInstance)
                {
                    if (pInstance->GetData(DATA_JOINED_THE_ATTACK) != DONE)
                        return;
                }

                SendDestroyTowers();

                if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
                    player->NearTeleportTo(beachPos);
            }

            void SendDestroyTowers()
            {
                if (!pInstance)
                    return;

                if (!isTowersDestroyed)
                {
                    isTowersDestroyed = true;

                    pInstance->SetData(DATA_DESTROYED_TOWERS_1, 1);
                    pInstance->SetData(DATA_DESTROYED_TOWERS_2, 1);
                    pInstance->SetData(DATA_DESTROYED_TOWERS_3, 1);
                }
            }

        private:

            InstanceScript* pInstance;
            std::set<uint64> playersClicked;
            bool isTowersDestroyed;
        };
};

class npc_assault_on_zanvess_priest : public CreatureScript
{
    public:

        npc_assault_on_zanvess_priest() : CreatureScript("npc_assault_on_zanvess_priest") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_priestAI(pCreature);
        }

        struct npc_assault_on_zanvess_priestAI : public ScriptedAI
        {
            npc_assault_on_zanvess_priestAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_HEAL, urand(3000, 5000));
                events.ScheduleEvent(EVENT_HOLY_SMITE, 500);
                events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, urand(5000, 8000));
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
                        case EVENT_HEAL:
                            DoCast(me, SPELL_HEAL);
                            events.ScheduleEvent(EVENT_HEAL, urand(8000, 12000));
                            break;
                        case EVENT_HOLY_SMITE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_HOLY_SMITE);
                            events.ScheduleEvent(EVENT_HOLY_SMITE, 2500);
                            break;
                        case EVENT_SHADOW_WORD_PAIN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_SHADOW_WORD_PAIN);
                            events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, urand(5000, 10000));
                            break;
                        default:
                            break;
                    }
                }
            }

        private:

        private:

            InstanceScript* pInstance;
        };
};

class npc_assault_on_zanvess_infiltrator : public CreatureScript
{
    public:

        npc_assault_on_zanvess_infiltrator() : CreatureScript("npc_assault_on_zanvess_infiltrator") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_infiltratorAI(pCreature);
        }

        struct npc_assault_on_zanvess_infiltratorAI : public ScriptedAI
        {
            npc_assault_on_zanvess_infiltratorAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_RIPOSTE, urand(3000, 5000));
                events.ScheduleEvent(EVENT_RUSH, urand(1000, 3000));
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
                        case EVENT_RIPOSTE:
                            DoCast(me, SPELL_RIPOSTE);
                            events.ScheduleEvent(EVENT_RIPOSTE, urand(10000, 12000));
                            break;
                        case EVENT_RUSH:
                            DoCast(me, SPELL_RUSH);
                            events.ScheduleEvent(EVENT_RUSH, urand(8000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

        private:

            InstanceScript* pInstance;
        };
};

class npc_assault_on_zanvess_leader : public CreatureScript
{
    public:

        npc_assault_on_zanvess_leader() : CreatureScript("npc_assault_on_zanvess_leader") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_leaderAI(pCreature);
        }

        struct npc_assault_on_zanvess_leaderAI : public ScriptedAI
        {
            npc_assault_on_zanvess_leaderAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_DEVASTATING_SMASH, urand(5000, 6000));
                events.ScheduleEvent(EVENT_FIXATE, urand(10000, 15000));
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    if (me->GetEntry() == NPC_SQUAD_LEADER_BOSH)
                    {
                        pInstance->SetData(DATA_KILLED_BOSH, DONE);
                    }
                    else if (me->GetEntry() == NPC_TEAM_LEADER_SCOOTER)
                    {
                        pInstance->SetData(DATA_KILLED_SCOOTER, DONE);
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
                    switch (eventId)
                    {
                        case EVENT_DEVASTATING_SMASH:
                            DoCast(me, SPELL_DEVASTATING_SMASH);
                            events.ScheduleEvent(EVENT_DEVASTATING_SMASH, urand(17000, 20000));
                            break;
                        case EVENT_FIXATE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_FIXATE);
                            events.ScheduleEvent(EVENT_FIXATE, urand(15000, 20000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

        private:

            InstanceScript* pInstance;
        };
};

class npc_assault_on_zanvess_zanthik_brutalizer : public CreatureScript
{
    public:

        npc_assault_on_zanvess_zanthik_brutalizer() : CreatureScript("npc_assault_on_zanvess_zanthik_brutalizer") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_zanthik_brutalizerAI(pCreature);
        }

        struct npc_assault_on_zanvess_zanthik_brutalizerAI : public ScriptedAI
        {
            npc_assault_on_zanvess_zanthik_brutalizerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_STOMP, urand(3000, 6000));
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
                        case EVENT_STOMP:
                            DoCastAOE(SPELL_STOMP);
                            events.ScheduleEvent(EVENT_STOMP, urand(10000, 12000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

        private:

            InstanceScript* pInstance;
        };
};

class npc_assault_on_zanvess_zanthik_ambermancer : public CreatureScript
{
    public:

        npc_assault_on_zanvess_zanthik_ambermancer() : CreatureScript("npc_assault_on_zanvess_zanthik_ambermancer") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_zanthik_ambermancerAI(pCreature);
        }

        struct npc_assault_on_zanvess_zanthik_ambermancerAI : public ScriptedAI
        {
            npc_assault_on_zanvess_zanthik_ambermancerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_HOLY_SMITE, 1000);
                events.ScheduleEvent(EVENT_GIFT_OF_AMBER, urand(3000, 6000));
                events.ScheduleEvent(EVENT_SONIC_SHIELD, urand(6000, 10000));
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
                        case EVENT_HOLY_SMITE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_HOLY_SMITE);
                            events.ScheduleEvent(EVENT_HOLY_SMITE, 2500);
                            break;
                        case EVENT_GIFT_OF_AMBER:
                            DoCast(me, SPELL_GIFT_OF_AMBER);
                            break;
                        case EVENT_SONIC_SHIELD:
                            DoCast(me, SPELL_SONIC_SHIELD);
                            events.ScheduleEvent(EVENT_SONIC_SHIELD, urand(20000, 25000));
                            break;
                        default:
                            break;
                    }
                }
            }

        private:

        private:

            InstanceScript* pInstance;
        };
};

class npc_assault_on_zanvess_commander_telvrak : public CreatureScript
{
    public:

        npc_assault_on_zanvess_commander_telvrak() : CreatureScript("npc_assault_on_zanvess_commander_telvrak") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_commander_telvrakAI(pCreature);
        }

        struct npc_assault_on_zanvess_commander_telvrakAI : public ScriptedAI
        {
            npc_assault_on_zanvess_commander_telvrakAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                events.Reset();

                SetCurrentWeapon(-1);
            }

            void EnterCombat(Unit* who)
            {
                Talk(SAY_TELVRAK_AGGRO);

                events.ScheduleEvent(EVENT_SWAP_WEAPON, 3000);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_SWAP_WEAPON)
                {
                    if (isBladeCurrentWeapon())
                    {
                        SetCurrentWeapon(1);
                    }
                    else if (isDaggersCurrentWeapon())
                    {
                        SetCurrentWeapon(0);
                    }
                    else
                    {
                        SetCurrentWeapon(0);
                    }
                }
            }

            void JustDied(Unit* who)
            {
                events.Reset();

                if (pInstance)
                {
                    pInstance->SetData(DATA_KILLED_TELVRAK, DONE);
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
                        case EVENT_SWAP_WEAPON:
                            DoCast(me, SPELL_SWAP_WEAPON);
                            events.ScheduleEvent(EVENT_SWAP_WEAPON, 25000);
                            break;
                        case EVENT_IMPALE:
                            DoCast(me, SPELL_IMPALE);
                            events.ScheduleEvent(EVENT_IMPALE, urand(8000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            bool isBladeCurrentWeapon() const
            {
                return currentWeapon == 0;
            }

            bool isDaggersCurrentWeapon() const
            {
                return currentWeapon == 1;
            }

            void SetCurrentWeapon(uint8 weapon)
            {
                events.CancelEvent(EVENT_IMPALE);
                me->RemoveAura(SPELL_BLADEMASTER);
                me->RemoveAura(SPELL_SWIFTNESS);

                if (weapon == 0)
                {
                    SetEquipmentSlots(false, ITEM_AMBER_FLAMMARD_OF_KLAXXIVESS, 0, 0);
                    me->AddAura(SPELL_BLADEMASTER, me);
                    events.ScheduleEvent(EVENT_IMPALE, urand(5000, 8000));
                }
                else if (weapon == 1)
                {
                    SetEquipmentSlots(false, ITEM_AMBER_SLICER_OF_KLAXXIVES, ITEM_AMBER_SLICER_OF_KLAXXIVES, 0);
                    me->AddAura(SPELL_SWIFTNESS, me);
                }
                else
                {
                    SetEquipmentSlots(false, 0, 0, 0);
                }

                currentWeapon = weapon;
            }

        private:

            InstanceScript* pInstance;
            int8 currentWeapon;
        };
};

#define STRAFING_RUN_OFFSET 20.0f

class npc_assault_on_zanvess_strafing_run : public CreatureScript
{
    public:

        npc_assault_on_zanvess_strafing_run() : CreatureScript("npc_assault_on_zanvess_strafing_run") {}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_assault_on_zanvess_strafing_runAI(pCreature);
        }

        struct npc_assault_on_zanvess_strafing_runAI : public Scripted_NoMovementAI
        {
            npc_assault_on_zanvess_strafing_runAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetCanFly(true);
                me->SetDisableGravity(true);
            }

            void IsSummonedBy(Unit* owner)
            {
                Position pos;
                me->GetPosition(&pos);
                pos.m_positionZ -= STRAFING_RUN_OFFSET;
                me->CastSpell(pos, SPELL_STRAFING_RUN_MISSILE, false);
            }

            void Reset()
            {
                events.Reset();
            }

        private:

        private:

            InstanceScript* pInstance;
        };
};

class spell_assault_on_zanvess_strafing_run: public SpellScriptLoader
{
    public:
        spell_assault_on_zanvess_strafing_run() : SpellScriptLoader("spell_assault_on_zanvess_strafing_run") { }

        class spell_assault_on_zanvess_strafing_run_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_assault_on_zanvess_strafing_run_SpellScript);

            void HandleAfterCast()
            {
                if (!GetCaster())
                    return;

                if (Player* player = GetCaster()->ToPlayer())
                {
                    player->AddSpellCooldown(m_scriptSpellId, 0, 45 * IN_MILLISECONDS);
                    player->SendSpellCooldown(m_scriptSpellId, 45 * IN_MILLISECONDS);
                }
            }

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                WorldLocation summonPos = *GetExplTargetDest();
                summonPos.m_positionZ += STRAFING_RUN_OFFSET;
                SetExplTargetDest(summonPos);

                WorldLocation* dest = GetHitDest();
                if (dest)
                {
                    dest->Relocate(summonPos);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_assault_on_zanvess_strafing_run_SpellScript::HandleAfterCast);
                OnEffectHit += SpellEffectFn(spell_assault_on_zanvess_strafing_run_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_assault_on_zanvess_strafing_run_SpellScript();
        }
};

class spell_assault_on_zanvess_swap_weapon: public SpellScriptLoader
{
    public:
        spell_assault_on_zanvess_swap_weapon() : SpellScriptLoader("spell_assault_on_zanvess_swap_weapon") { }

        class spell_assault_on_zanvess_swap_weapon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_assault_on_zanvess_swap_weapon_SpellScript);

            void HandleOnHit(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    if (auto l_AI = pCreature->AI())
                        l_AI->DoAction(ACTION_SWAP_WEAPON);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_assault_on_zanvess_swap_weapon_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_assault_on_zanvess_swap_weapon_SpellScript();
        }
};

class spell_assault_on_zanvess_devastating_smash : public SpellScriptLoader
{
    public:
        spell_assault_on_zanvess_devastating_smash() : SpellScriptLoader("spell_assault_on_zanvess_devastating_smash") { }

        class spell_assault_on_zanvess_devastating_smash_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_assault_on_zanvess_devastating_smash_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_DEVASTATING_SMASH_AURA, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_assault_on_zanvess_devastating_smash_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_assault_on_zanvess_devastating_smash_AuraScript();
        }
};

void AddSC_assault_on_zanvess()
{
    new npc_assault_on_zanvess_sky_admiral_rogers();    // 68828
    new npc_assault_on_zanvess_rear_admiral_zigg();     // 67799
    new npc_assault_on_zanvess_skyfire_gyrocopter();    // 68115
    new npc_assault_on_zanvess_korkron_gunship();       // 67275

    new npc_assault_on_zanvess_priest();                // 67892 68142
    new npc_assault_on_zanvess_infiltrator();           // 67727 68141
    new npc_assault_on_zanvess_leader();                // 67810 68143

    new npc_assault_on_zanvess_zanthik_brutalizer();    // 67774
    new npc_assault_on_zanvess_zanthik_ambermancer();   // 62838
    new npc_assault_on_zanvess_commander_telvrak();     // 67879

    new npc_assault_on_zanvess_strafing_run();          // 67838

    new spell_assault_on_zanvess_strafing_run();        // 133703
    new spell_assault_on_zanvess_swap_weapon();         // 133966
    new spell_assault_on_zanvess_devastating_smash();   // 133817
}

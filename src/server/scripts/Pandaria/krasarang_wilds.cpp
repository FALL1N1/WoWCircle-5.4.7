#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "krasarang_wilds.h"
#include "SpellScript.h"
#include "GameObjectAI.h"

#define CHECK_STATUS(a) (player->GetQuestStatus(a) == QUEST_STATUS_INCOMPLETE)
#define GOSSIP_CHOICE_1 "Let's fight !"
#define GOSSIP_CHOICE "<Reach out to touch Chi-Ji.>"

// Arness the Scale - 50787
class mob_arness_the_scale : public CreatureScript
{
    public:
        mob_arness_the_scale() : CreatureScript("mob_arness_the_scale")
        {
        }

        struct mob_arness_the_scaleAI : public ScriptedAI
        {
            mob_arness_the_scaleAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventGrapplingHook, 40000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventVanish, 20000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventViciousRend, 15000);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case KrasarangWildsEvents::EventGrapplingHook:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellGrapplingHook, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventGrapplingHook, 40000);
                        break;
                    case KrasarangWildsEvents::EventVanish:
                        me->CastSpell(me, KrasarangWildsSpells::SpellVanish, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventVanish, 40000);
                        break;
                    case KrasarangWildsEvents::EventViciousRend:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellViciousRend, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventViciousRend, 40000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_arness_the_scaleAI(creature);
        }
};

// Gaarn the Toxic - 50340
class mob_gaarn_the_toxic : public CreatureScript
{
    public:
        mob_gaarn_the_toxic() : CreatureScript("mob_gaarn_the_toxic")
        {
        }

        struct mob_gaarn_the_toxicAI : public ScriptedAI
        {
            mob_gaarn_the_toxicAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventShadowBolt, 15000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventVoidCloud, 70000);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case KrasarangWildsEvents::EventShadowBolt:
                            if (Unit*l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(l_Target, KrasarangWildsSpells::SpellShadowBolt, false);
                            m_Events.ScheduleEvent(KrasarangWildsEvents::EventShadowBolt, 15000);
                            break;
                        case KrasarangWildsEvents::EventVoidCloud:
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(l_Target, KrasarangWildsSpells::SpellVoidCloud, false);
                            m_Events.ScheduleEvent(KrasarangWildsEvents::EventVoidCloud, 70000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gaarn_the_toxicAI(creature);
        }
};

// Qu Nas - 50352
class mob_qu_nas : public CreatureScript
{
    public:
        mob_qu_nas() : CreatureScript("mob_qu_nas")
        {
        }

        struct mob_qu_nasAI : public ScriptedAI
        {
            mob_qu_nasAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDevastatingArc, 25000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventSummonQuilen, 15000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventTitanicStrengh, 70000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                switch (m_Events.ExecuteEvent())
                {
                    case KrasarangWildsEvents::EventDevastatingArc:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellDevastingArc, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventDevastatingArc, 30000);
                        break;
                    case KrasarangWildsEvents::EventSummonQuilen:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellSummonQuilen, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventSummonQuilen, 80000);
                        break;
                    case KrasarangWildsEvents::EventTitanicStrengh:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellTitanicStrengh, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventTitanicStrengh, 55000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_qu_nasAI(creature);
        }
};

// Torik Ethis - 50388
class mob_torik_ethis : public CreatureScript
{
    public:
        mob_torik_ethis() : CreatureScript("mob_torik_ethis")
        {
        }

        struct mob_torik_ethisAI : public ScriptedAI
        {
            mob_torik_ethisAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventBladeFury, 8000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventTornado, 40000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventWindSong, 32000);
            }

            void JustSummoned(Creature* p_Summon)
            {
                if (p_Summon->GetEntry() == KrasarangWildsCreatures::NpcTornado)
                {
                    p_Summon->AddAura(KrasarangWildsSpells::SpellTornadoDmg, p_Summon);
                    p_Summon->SetReactState(REACT_PASSIVE);
                    p_Summon->GetMotionMaster()->MoveRandom(20.0f);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);
                switch (m_Events.ExecuteEvent())
                {
                    case KrasarangWildsEvents::EventBladeFury:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellBladeFury, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventBladeFury, 8000);
                        break;
                    case KrasarangWildsEvents::EventTornado:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellTornado, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventTornado, 40000);
                        break;
                    case KrasarangWildsEvents::EventWindSong:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellWindsong, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventWindSong, 32000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_torik_ethisAI(creature);
        }
};

// Go Kan - 50331
class mob_go_kan : public CreatureScript
{
    public:
        mob_go_kan() : CreatureScript("mob_go_kan")
        {
        }

        struct mob_go_kanAI : public ScriptedAI
        {
            mob_go_kanAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventBellowingRage, 8000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventRushingCharge, 17000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventYaungolStomp, 25000);

                me->SetFlag(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_SIT);
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                me->RemoveFlag(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_SIT);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case KrasarangWildsEvents::EventBellowingRage:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellBellowingCRage, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventBellowingRage, 35000);
                        break;
                    case KrasarangWildsEvents::EventRushingCharge:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellRushingCharge, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventRushingCharge, 35000);
                        break;
                    case KrasarangWildsEvents::EventYaungolStomp:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellYaungolStomp, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventYaungolStomp, 35000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_go_kanAI(creature);
        }
};

// Spirit of the Crane - 60487
class mob_spirit_of_the_crane : public CreatureScript
{
    public:
        mob_spirit_of_the_crane() : CreatureScript("mob_spirit_of_the_crane")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(KrasarangWildsQuests::QuestChasingHope) == QUEST_STATUS_INCOMPLETE)
            {
                p_Player->KilledMonsterCredit(KrasarangWildsCreatures::CreditChasingHope);
                p_Creature->DespawnOrUnsummon();
            }

            return true;
        }
};

// Champion of Chi Ji - 60546
class mob_champion_of_chi_ji : public CreatureScript
{
    public:
        mob_champion_of_chi_ji() : CreatureScript("mob_champion_of_chi_ji")
        {
        }

        struct mob_champion_of_chi_jiAI : public ScriptedAI
        {
            mob_champion_of_chi_jiAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventChiTorpedo, 4000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventHundredHandSlap, 17000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventSpinningCraneKick, 27000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventUppercut, 35000);
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (Player* l_Player = p_Who->ToPlayer())
                {
                    if (me->GetDistance(l_Player) <= 40.0f && l_Player->GetQuestStatus(30740) == QUEST_STATUS_INCOMPLETE)
                        me->CastSpell(l_Player, KrasarangWildsSpells::SpellFlyingSerpentKick, false);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case KrasarangWildsEvents::EventChiTorpedo:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellChiTorpedo, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventChiTorpedo, 40000);
                        break;
                    case KrasarangWildsEvents::EventHundredHandSlap:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellHundredHandSlap, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventHundredHandSlap, 40000);
                        break;
                    case KrasarangWildsEvents::EventSpinningCraneKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellSpinningCraneKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventSpinningCraneKick, 40000);
                        break;
                    case KrasarangWildsEvents::EventUppercut:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellUppercut, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventUppercut, 40000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_champion_of_chi_jiAI(creature);
        }
};

// Chi Torpedo - 119539
class spell_chi_torpedo_periodic : public SpellScriptLoader
{
    public:
        spell_chi_torpedo_periodic() : SpellScriptLoader("spell_chi_torpedo_periodic") { }

        class spell_chi_torpedo_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_chi_torpedo_periodic_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = l_Caster->getVictim())
                        l_Caster->CastSpell(l_Target, KrasarangWildsSpells::SpellWheelbarrowRoll, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_chi_torpedo_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_chi_torpedo_periodic_AuraScript();
        }
};

// Andruin Wrynn - 59608
class mob_anduin_wrynn : public CreatureScript
{
    public:
        mob_anduin_wrynn() : CreatureScript("mob_anduin_wrynn")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, Quest const* p_Quest)
        {
            if (p_Quest->GetQuestId() == KrasarangWildsQuests::QuestInTheHouseOfTheRedCrane)
                p_Player->SummonCreature(KrasarangWildsCreatures::NpcAnduinWrynn, p_Player->GetPositionX(), p_Player->GetPositionY(), p_Player->GetPositionZ(), 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0, p_Player->GetGUID());

            return true;
        }
};

// Andruin Wrynn - 66975
class mob_anduin_wrynn_escort : public CreatureScript
{
    public:
        mob_anduin_wrynn_escort() : CreatureScript("mob_anduin_wrynn_escort")
        {
        }

        struct mob_anduin_wrynn_escortAI : public ScriptedAI
        {
            mob_anduin_wrynn_escortAI(Creature* creature) : ScriptedAI(creature)
            {
                m_PlayerGuid = 0;
            }

            uint64 m_PlayerGuid;
            EventMap m_Events;

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_Events.Reset();

                if (Player* l_Plr = p_Summoner->ToPlayer())
                {
                    if (l_Plr->GetQuestStatus(KrasarangWildsQuests::QuestInTheHouseOfTheRedCrane) == QUEST_STATUS_INCOMPLETE)
                    {
                        me->GetMotionMaster()->MoveFollow(l_Plr, 2.0f, 2.0f, MOTION_SLOT_ACTIVE);
                        m_PlayerGuid = l_Plr->GetGUID();
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventCheckTarget, 1000);
                    }
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == KrasarangWildsEvents::EventCheckTarget)
                {
                    if (Player* l_Summoner = sObjectAccessor->FindPlayer(m_PlayerGuid))
                    {
                        if (Unit* l_Target = l_Summoner->getVictim())
                        {
                            if (CAST_CRE(l_Target)->GetEntry() == 59651)
                            {
                                me->Attack(l_Target, true);
                                DoMeleeAttackIfReady();
                            }
                        }
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventCheckTarget, 1000);
                    }
                    else
                        me->DespawnOrUnsummon();

                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_anduin_wrynn_escortAI(creature);
        }
};

// Chi Ji - 59653
class npc_chi_ji : public CreatureScript
{
    public:
        npc_chi_ji() : CreatureScript("npc_chi_ji")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(KrasarangWildsQuests::QuestInTheHouseOfTheRedCrane) == QUEST_STATUS_INCOMPLETE)
            {
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                p_Player->SEND_GOSSIP_MENU(30003, p_Creature->GetGUID());
            }

            return true;
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*sender*/, uint32 p_Action)
        {
            p_Player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                p_Player->CLOSE_GOSSIP_MENU();
                p_Creature->DespawnOrUnsummon();
                p_Player->SummonCreature(KrasarangWildsCreatures::NpcShaOfDespair, p_Creature->GetPositionX(), p_Creature->GetPositionY(), p_Creature->GetPositionZ()); // , 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
            }

            return true;
        }
};

// Sha of Despair - 59651
class mob_sha_of_despair : public CreatureScript
{
    public:
        mob_sha_of_despair() : CreatureScript("mob_sha_of_despair")
        {
        }

        struct mob_sha_of_despairAI : public ScriptedAI
        {
            mob_sha_of_despairAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                me->DespawnOrUnsummon();
                me->SummonCreature(59653, -1813.46f, 1052.33f, -31.115f, 0.0f, TEMPSUMMON_MANUAL_DESPAWN);

                std::list<Creature*> creatureList;
                GetCreatureListWithEntryInGrid(creatureList, me, 66975, 40.0f);

                for (auto creature: creatureList)
                    creature->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sha_of_despairAI(creature);
        }
};

// Thelonius - 60506
class npc_thelonius : public CreatureScript
{
    public:
        npc_thelonius() : CreatureScript("npc_thelonius")
        {
        }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
        {
            switch (quest->GetQuestId())
            {
                case 30725:
                    player->SummonCreature(60530, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30739:
                    player->SummonCreature(60545, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30727:
                    player->SummonCreature(60533, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30732:
                    player->SummonCreature(60538, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30728:
                    player->SummonCreature(60534, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30737:
                    player->SummonCreature(60543, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30734:
                    player->SummonCreature(60540, -2322.529f, 1624.929f, 0.381f, 5.288279f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30730:
                    player->SummonCreature(60536, -2615.302f, 1457.653f, 1.537f, 2.407458f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30729:
                    player->SummonCreature(60535, -2471.108f, 1594.382f, 0.267f, 5.878917f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30731:
                    player->SummonCreature(60537, -2473.856f, 1241.124f, 36.133f, 1.293773f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30735:
                    player->SummonCreature(60541, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30726:
                    player->SummonCreature(60532, -2502.585f, 1449.352f, 15.786f, 0.030857f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30738:
                    player->SummonCreature(60544, -2322.529f, 1624.929f, 0.381f, 5.288279f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30733:
                    player->SummonCreature(60539, -2457.983f, 1352.873f, 20.287f, 0.202678f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30736:
                    player->SummonCreature(60542, -2473.856f, 1241.124f, 36.133f, 1.293773f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                default:
                    break;
            }

            return true;
        }
};

//Ella Ravenmane - 60530/60545/60533/60538
class mob_ellia_ravenmane : public CreatureScript
{
    public:
        mob_ellia_ravenmane() : CreatureScript("mob_ellia_ravenmane")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30725) || CHECK_STATUS(30739) || CHECK_STATUS(30727) || CHECK_STATUS(30732))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ellia_ravenmaneAI(creature);
        }

        struct mob_ellia_ravenmaneAI : public ScriptedAI
        {
            mob_ellia_ravenmaneAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                switch (me->GetEntry())
                {
                    case 60530:
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventJab, 3000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventKick, 6000);
                        break;
                    case 60545:
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventCryOutInDespair, 3000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventShaBlast, 8000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventShaSpike, 13000);
                        break;
                    case 60533:
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFierceJab, 3000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFierceKick, 6000);
                        break;
                    case 60538:
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFierceJabTwo, 3000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFistsOfFury, 8000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventViciousKick, 13000);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    switch (me->GetEntry())
                    {
                        case 60530:
                            if (CHECK_STATUS(30725))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(KrasarangWildsActions::ActionReinitialize);
                                    player->KilledMonsterCredit(60530);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60545:
                            if (CHECK_STATUS(30739))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(KrasarangWildsActions::ActionReinitialize);
                                    player->KilledMonsterCredit(60545);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60533:
                            if (CHECK_STATUS(30727))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(KrasarangWildsActions::ActionReinitialize);
                                    player->KilledMonsterCredit(60533);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60538:
                            if (CHECK_STATUS(30732))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(KrasarangWildsActions::ActionReinitialize);
                                    player->KilledMonsterCredit(60538);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        default:
                            break;
                        
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    switch (me->GetEntry())
                    {
                        case 60530:
                            if (CHECK_STATUS(30725))
                                me->DespawnOrUnsummon();
                            break;
                        case 60545:
                            if (CHECK_STATUS(30739))
                                me->DespawnOrUnsummon();
                            break;
                        case 60533:
                            if (CHECK_STATUS(30727))
                                me->DespawnOrUnsummon();
                            break;
                        case 60538:
                            if (CHECK_STATUS(30732))
                                me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // Entry 60530
                    case KrasarangWildsEvents::EventJab:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellJab, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventJab, 7000);
                            break;
                    case KrasarangWildsEvents::EventKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventKick, 7000);
                            break;
                            // Entry 60545
                    case KrasarangWildsEvents::EventCryOutInDespair:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellCryOutInDespair, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventCryOutInDespair, 15000);
                            break;
                    case KrasarangWildsEvents::EventShaBlast:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellShaBlast, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventShaBlast, 15000);
                            break;
                    case KrasarangWildsEvents::EventShaSpike:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellShaSpike, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventShaSpike, 15000);
                            break;
                            // Entry 60533
                    case KrasarangWildsEvents::EventFierceJab:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellFierceJab, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFierceJab, 7000);
                            break;
                    case KrasarangWildsEvents::EventFierceKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellFierceKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFierceKick, 7000);
                            break;
                            // Entry 60538
                    case KrasarangWildsEvents::EventFierceJabTwo:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellFierceJab, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFierceJabTwo, 15000);
                            break;
                    case KrasarangWildsEvents::EventFistsOfFury:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellFistsOfFury, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFistsOfFury, 15000);
                            break;
                    case KrasarangWildsEvents::EventViciousKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellViciousKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventViciousKick, 15000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Fat Long Fat - 60534/60543
class mob_fat_long_fat : public CreatureScript
{
    public:
        mob_fat_long_fat() : CreatureScript("mob_fat_long_fat")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30728) || CHECK_STATUS(30737))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_fat_long_fatAI(creature);
        }

        struct mob_fat_long_fatAI : public ScriptedAI
        {
            mob_fat_long_fatAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                switch (me->GetEntry())
                {
                    case 60534:
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventBreathOfFire, 3000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventStormStout, 6000);
                        break;
                    case 60543:
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFatLongFlop, 3000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventBreathOfFireTwo, 8000);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventStormStoutTwo, 13000);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    switch (me->GetEntry())
                    {
                        case 60534:
                            if (CHECK_STATUS(30728))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(KrasarangWildsActions::ActionReinitialize);
                                    player->KilledMonsterCredit(60534);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60543:
                            if (CHECK_STATUS(30737))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(KrasarangWildsActions::ActionReinitialize);
                                    player->KilledMonsterCredit(60543);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    switch (me->GetEntry())
                    {
                        case 60534:
                            if (!CHECK_STATUS(30728))
                                me->DespawnOrUnsummon();
                            break;
                        case 60543:
                            if (!CHECK_STATUS(30737))
                                me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // Entry 60534
                    case KrasarangWildsEvents::EventBreathOfFire:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellBreathOfFire, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventBreathOfFire, 7000);
                            break;
                    case KrasarangWildsEvents::EventStormStout:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellStormOut, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventStormStout, 7000);
                            break;
                            // Entry 60543
                    case KrasarangWildsEvents::EventFatLongFlop:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellFatLongFlop, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFatLongFlop, 15000);
                            break;
                    case KrasarangWildsEvents::EventBreathOfFireTwo:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellBreathOfFire, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventBreathOfFireTwo, 15000);
                            break;
                    case KrasarangWildsEvents::EventStormStoutTwo:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellStormOut, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventStormStoutTwo, 15000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Huck Wheelbarrow - 60540
class mob_huck_wheelbarrow : public CreatureScript
{
    public:
        mob_huck_wheelbarrow() : CreatureScript("mob_huck_wheelbarrow")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30734))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_huck_wheelbarrowAI(creature);
        }

        struct mob_huck_wheelbarrowAI : public ScriptedAI
        {
            mob_huck_wheelbarrowAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventWheelbarrowRoll, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventWheelPunch, 8000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventRollingThrow, 13000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30734))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60540);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30734))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case KrasarangWildsEvents::EventWheelbarrowRoll:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellWheelbarrowRollTwo, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventWheelbarrowRoll, 18000);
                            break;
                    case KrasarangWildsEvents::EventWheelPunch:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellWheelPunch, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventWheelPunch, 18000);
                            break;
                    case KrasarangWildsEvents::EventRollingThrow:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellRollingThrow, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventRollingThrow, 18000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Dextrous Izissha - 60536
class mob_dextrous_izissha : public CreatureScript
{
    public:
        mob_dextrous_izissha() : CreatureScript("mob_dextrous_izissha")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30730))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_dextrous_izisshaAI(creature);
        }

        struct mob_dextrous_izisshaAI : public ScriptedAI
        {
            mob_dextrous_izisshaAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventEvasion, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventFluidLikeWater, 8000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventStillWatersRunDeep, 13000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30730))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60536);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30730))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case KrasarangWildsEvents::EventEvasion:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellEvasion, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventEvasion, 18000);
                            break;
                    case KrasarangWildsEvents::EventFluidLikeWater:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellFluidLikeWater, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFluidLikeWater, 18000);
                            break;
                    case KrasarangWildsEvents::EventStillWatersRunDeep:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellStillWatersRunDeep, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventStillWatersRunDeep, 18000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Julia Bates - 60535
class mob_julia_bates : public CreatureScript
{
    public:
        mob_julia_bates() : CreatureScript("mob_julia_bates")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30729))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_julia_batesAI(creature);
        }

        struct mob_julia_batesAI : public ScriptedAI
        {
            mob_julia_batesAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventHundredHandSlap, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventSoothingMist, 8000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30729))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60535);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30729))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case KrasarangWildsEvents::EventHundredHandSlap:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellHundredHandSlap, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventHundredHandSlap, 7000);
                            break;
                    case KrasarangWildsEvents::EventSoothingMist:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, KrasarangWildsSpells::SpellSoothingMist, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventSoothingMist, 7000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Kuo-Na Quillpaw - 60537
class mob_kuo_na : public CreatureScript
{
    public:
        mob_kuo_na() : CreatureScript("mob_kuo_na")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30731))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kuo_naAI(creature);
        }

        struct mob_kuo_naAI : public ScriptedAI
        {
            mob_kuo_naAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventFlyingSerpentKick, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventPawPunch, 8000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventUppercut, 15000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30731))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60537);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30731))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case KrasarangWildsEvents::EventFlyingSerpentKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellFlyingSerpentKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventFlyingSerpentKick, 21000);
                            break;
                    case KrasarangWildsEvents::EventPawPunch:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, KrasarangWildsSpells::SpellPawPunch, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventPawPunch, 21000);
                            break;
                    case KrasarangWildsEvents::EventUppercut:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, KrasarangWildsSpells::SpellUppercut, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventUppercut, 21000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Mindel Sunspeaker - 60541
class mob_mindel_sunspeaker : public CreatureScript
{
    public:
        mob_mindel_sunspeaker() : CreatureScript("mob_mindel_sunspeaker")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30735))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mindel_sunspeakerAI(creature);
        }

        struct mob_mindel_sunspeakerAI : public ScriptedAI
        {
            mob_mindel_sunspeakerAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventSunflareKick, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventSunStrike, 8000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30735))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60541);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30735))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case KrasarangWildsEvents::EventSunflareKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellSunflareKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventSunflareKick, 12000);
                            break;
                    case KrasarangWildsEvents::EventSunStrike:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, KrasarangWildsSpells::SpellSunstrike, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventSunStrike, 12000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Minh Do-Tan - 60532
class mob_minh_do_tan : public CreatureScript
{
    public:
        mob_minh_do_tan() : CreatureScript("mob_minh_do_tan")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30726))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_minh_do_tanAI(creature);
        }

        struct mob_minh_do_tanAI : public ScriptedAI
        {
            mob_minh_do_tanAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventSunflareKick, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventSunStrike, 8000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30726))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60532);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30726))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case KrasarangWildsEvents::EventSpinningCraneKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellSpinningCraneKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventSpinningCraneKick, 12000);
                            break;
                    case KrasarangWildsEvents::EventViciousKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(me, KrasarangWildsSpells::SpellViciousKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventViciousKick, 12000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Thelonius - 60544
class mob_thelonius : public CreatureScript
{
    public:
        mob_thelonius() : CreatureScript("mob_thelonius")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30738))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_theloniusAI(creature);
        }

        struct mob_theloniusAI : public ScriptedAI
        {
            mob_theloniusAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventUppercut, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventSpinningCraneKick, 8000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventHundredHandSlap, 13000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventFlyingSerpentKick, 20000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30738))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60544);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30738))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case KrasarangWildsEvents::EventUppercut:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellUppercut, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventUppercut, 25000);
                            break;
                    case KrasarangWildsEvents::EventSpinningCraneKick:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, KrasarangWildsSpells::SpellSpinningCraneKick, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventSpinningCraneKick, 25000);
                            break;
                    case KrasarangWildsEvents::EventHundredHandSlap:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellHundredHandSlap, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventHundredHandSlap, 25000);
                            break;
                        case KrasarangWildsEvents::EventFlyingSerpentKick:
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, KrasarangWildsSpells::SpellFlyingSerpentKick, false);
                            m_Events.ScheduleEvent(KrasarangWildsEvents::EventFlyingSerpentKick, 25000);
                            break;
                        case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Tukka-Tuk - 60539
class mob_tukka_tuk : public CreatureScript
{
    public:
        mob_tukka_tuk() : CreatureScript("mob_tukka_tuk")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30733))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_tukka_tukAI(creature);
        }

        struct mob_tukka_tukAI : public ScriptedAI
        {
            mob_tukka_tukAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventPoundYou, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventKickYou, 8000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventPunchYou, 13000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30733))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60539);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30733))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case KrasarangWildsEvents::EventPoundYou:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellPoundYou, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventPoundYou, 20000);
                            break;
                    case KrasarangWildsEvents::EventKickYou:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, KrasarangWildsSpells::SpellKickYou, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventKickYou, 20000);
                            break;
                    case KrasarangWildsEvents::EventPunchYou:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, KrasarangWildsSpells::SpellPunchYou, false);
                        m_Events.ScheduleEvent(KrasarangWildsEvents::EventPunchYou, 20000);
                            break;
                    case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Yan Quillpaw - 60542
class mob_yan_quillpaw : public CreatureScript
{
    public:
        mob_yan_quillpaw() : CreatureScript("mob_yan_quillpaw")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 p_Action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30736))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(KrasarangWildsActions::ActionRemoveFlag);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_yan_quillpawAI(creature);
        }

        struct mob_yan_quillpawAI : public ScriptedAI
        {
            mob_yan_quillpawAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap m_Events;
            uint64 playerGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(KrasarangWildsEvents::EventUppercut, 3000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventFlyingSerpentKick, 8000);
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventPawPunch, 13000);
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30736))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(KrasarangWildsActions::ActionReinitialize);
                            player->KilledMonsterCredit(60542);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* summoner)
            {
                m_Events.ScheduleEvent(KrasarangWildsEvents::EventDespawn, 600000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == KrasarangWildsActions::ActionRemoveFlag)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (p_Action == KrasarangWildsActions::ActionReinitialize)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(KrasarangWildsActions::ActionReinitialize);
                        return;
                    }

                    if (!CHECK_STATUS(30736))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                while (uint32 eventId = m_Events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case KrasarangWildsEvents::EventUppercut:
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(l_Target, KrasarangWildsSpells::SpellUppercut, false);
                            m_Events.ScheduleEvent(KrasarangWildsEvents::EventUppercut, 20000);
                            break;
                        case KrasarangWildsEvents::EventFlyingSerpentKick:
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, KrasarangWildsSpells::SpellFlyingSerpentKick, false);
                            m_Events.ScheduleEvent(KrasarangWildsEvents::EventFlyingSerpentKick, 20000);
                            break;
                        case KrasarangWildsEvents::EventPawPunch:
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(l_Target, KrasarangWildsSpells::SpellPawPunch, false);
                            m_Events.ScheduleEvent(KrasarangWildsEvents::EventPawPunch, 20000);
                            break;
                        case KrasarangWildsEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

/// Signal Fire - 216274
class gob_signal_fire_krasarang : public GameObjectScript
{
    public:
        gob_signal_fire_krasarang() : GameObjectScript("gob_signal_fire_krasarang") { }

        struct gob_signal_fire_krasarangAI : public GameObjectAI
        {
            gob_signal_fire_krasarangAI(GameObject* p_Go) : GameObjectAI(p_Go) { }

            bool GossipHello(Player* p_Player)
            {
                if (p_Player->GetQuestStatus(31483) == QUEST_STATUS_INCOMPLETE)
                {
                    p_Player->KilledMonsterCredit(64681);
                    p_Player->SendMovieStart(128);
                }

                return true;
            }
        };

        GameObjectAI* GetAI(GameObject* p_Go) const
        {
            return new gob_signal_fire_krasarangAI(p_Go);
        }
};

// Elder Anli - 58564
class npc_scout_layna : public CreatureScript
{
    public:
        npc_scout_layna() : CreatureScript("npc_scout_layna")
        {
        }

        bool OnQuestReward(Player* p_Player, Creature* p_Creature, Quest* const p_Quest)
        {
            switch (p_Quest->GetQuestId())
            {
                /// Quest Meet the scout should launch an event at reward, can't find enough infos to script it atm
                case 32246: ///< Alliance
                    break;
                case 32249: ///< Horde
                    break;
                default:
                    break;
            }

            return true;
        }
};


// Yan Quillpaw - 60542
class npc_warlord_bloodhilt : public CreatureScript
{
    public:
        npc_warlord_bloodhilt() : CreatureScript("npc_warlord_bloodhilt")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_warlord_bloodhiltAI(creature);
        }

        struct npc_warlord_bloodhiltAI : public ScriptedAI
        {
            npc_warlord_bloodhiltAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap m_Events;

            enum eEvents
            {
                EventBloodThistyStrike = 1,
                EventBloodyKnives,
                EventBrutalCharge,
                EventChokeEmOut,
                EventCoupDeGrace
            };

            enum eSpells
            {
                SpellBloodThistyStrike = 135396,
                SpellBloodyKnives      = 135343,
                SpellBrutalCharge      = 135421,
                SpellChokeEmOut        = 135443,
                SpellCoupDeGrace       = 135397
            };

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(eEvents::EventBloodThistyStrike, 3000);
                m_Events.ScheduleEvent(eEvents::EventBloodyKnives, 8000);
                m_Events.ScheduleEvent(eEvents::EventBrutalCharge, 13000);
                m_Events.ScheduleEvent(eEvents::EventChokeEmOut, 18000);
                m_Events.ScheduleEvent(eEvents::EventCoupDeGrace, 23000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBloodThistyStrike:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellBloodThistyStrike, false);
                        m_Events.ScheduleEvent(eEvents::EventBloodThistyStrike, 30000);
                        break;
                    case eEvents::EventBloodyKnives:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellBloodyKnives, false);
                        m_Events.ScheduleEvent(eEvents::EventBloodyKnives, 30000);
                        break;
                    case eEvents::EventBrutalCharge:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellBrutalCharge, false);
                        m_Events.ScheduleEvent(eEvents::EventBrutalCharge, 30000);
                        break;
                    case eEvents::EventChokeEmOut:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellChokeEmOut, false);
                        m_Events.ScheduleEvent(eEvents::EventChokeEmOut, 30000);
                        break;
                    case eEvents::EventCoupDeGrace:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellCoupDeGrace, false);
                        m_Events.ScheduleEvent(eEvents::EventCoupDeGrace, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_high_marshal_twinbraid : public CreatureScript
{
    public:
        npc_high_marshal_twinbraid() : CreatureScript("npc_high_marshal_twinbraid")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_high_marshal_twinbraidAI(creature);
        }

        struct npc_high_marshal_twinbraidAI : public ScriptedAI
        {
            npc_high_marshal_twinbraidAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap m_Events;

            enum eEvents
            {
                EventCannonballSpin = 1,
                EventChaingun,
                EventExplosivesPlunger,
                EventChokeEmOut
            };

            enum eSpells
            {
                SpellCannonballSpin    = 135154,
                SpellChaingun          = 135417,
                SpellExplosivesPlunger = 135292, ///< self
                SpellSoundTheAlarm     = 135166  ///< self
            };

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(eEvents::EventCannonballSpin, 3000);
                m_Events.ScheduleEvent(eEvents::EventChaingun, 8000);
///                m_Events.ScheduleEvent(eEvents::EventExplosivesPlunger, 13000);
                m_Events.ScheduleEvent(eEvents::EventChokeEmOut, 18000);
            }

            void EnterEvadeMode()
            {
                Reset();
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCannonballSpin:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellCannonballSpin, false);
                        m_Events.ScheduleEvent(eEvents::EventCannonballSpin, 30000);
                        break;
                    case eEvents::EventChaingun:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellChaingun, false);
                        m_Events.ScheduleEvent(eEvents::EventChaingun, 30000);
                        break;
                    case eEvents::EventExplosivesPlunger:
                        me->CastSpell(me, eSpells::SpellExplosivesPlunger, false); ///< TO SCRIPT
                        m_Events.ScheduleEvent(eEvents::EventExplosivesPlunger, 30000);
                        break;
                    case eEvents::EventChokeEmOut:
                        me->CastSpell(me, eSpells::SpellSoundTheAlarm, false); ///< To check
                        m_Events.ScheduleEvent(eEvents::EventChokeEmOut, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };
};

/// Krasarang entering
class playerScript_mapshift_krasarang : public PlayerScript
{
    public:
        playerScript_mapshift_krasarang() : PlayerScript("playerScript_mapshift_krasarang") { }

		void OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 p_OldZoneId, uint32 p_NewAreaId, uint32 p_OldAreaId) override
        {
            if (p_Player && p_NewZoneId == 6134) ///< Krasarang Wilds
            {
                std::set<uint32> l_PhaseId, l_Terrainswap;

                uint16 l_AllianceMapID = 1062; ///< 1103 seems also to be correct, can't find exact difference between those two ones
                uint16 l_HordeMapID = 1063; ///< 1102 seems also to be correct, can't find exact difference between those two ones
                l_Terrainswap.insert((uint32)l_AllianceMapID);
                l_Terrainswap.insert((uint32)l_HordeMapID);
                p_Player->GetSession()->SendSetPhaseShift(l_PhaseId, l_Terrainswap);

                if (p_NewAreaId == 6369)
                {
                }
//                 else if (p_NewAreaId == 6612)
//                 {
//                     l_PhaseId.insert(0);
//                     p_Player->GetSession()->SendSetPhaseShift(l_PhaseId, l_Terrainswap);
//                 }
            }
        }
};

// Spirit of the Crane - 60487
class npc_hilda_hornswaggle : public CreatureScript
{
    public:
        npc_hilda_hornswaggle() : CreatureScript("npc_hilda_hornswaggle")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(KrasarangWildsQuests::QuestAKingAmonMen) == QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(p_Creature->GetEntry());

            return true;
        }
};

// Spirit of the Crane - 60487
class npc_marshall_troteman : public CreatureScript
{
    public:
        npc_marshall_troteman() : CreatureScript("npc_marshall_troteman")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(KrasarangWildsQuests::QuestAKingAmonMen) == QUEST_STATUS_INCOMPLETE)
                p_Player->KilledMonsterCredit(p_Creature->GetEntry());

            return true;
        }
};

/// White Tiger Temple Phases
class playerScript_enter_chi_ji_temple : public PlayerScript
{
    public:
        playerScript_enter_chi_ji_temple() : PlayerScript("playerScript_enter_chi_ji_temple") { }

		/// Save Phasemask and GUID of player entering the area, remove it when leaving
		std::map<uint64, uint32> m_PlayerGUIDs;

		void OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 p_OldZoneId, uint32 p_NewAreaId, uint32 p_OldAreaId) override
        {
            /// Temple of Niuzao
            if (p_NewAreaId == 6048)
            {
				m_PlayerGUIDs.insert(std::make_pair(p_Player->GetGUID(), p_Player->GetPhaseMask()));

				/// Celestial Blessings
                if (p_Player->GetQuestStatus(32805) == QUEST_STATUS_INCOMPLETE)
                    p_Player->SetPhaseMask(2, true);
            }
			else if (p_OldAreaId == 6048)
			{
				for (auto l_Itr = m_PlayerGUIDs.begin(); l_Itr != m_PlayerGUIDs.end(); l_Itr++)
				{
					if (l_Itr->first == p_Player->GetGUID())
					{
						p_Player->SetPhaseMask(l_Itr->second, true);
						l_Itr = m_PlayerGUIDs.erase(l_Itr);

						if (l_Itr == m_PlayerGUIDs.end())
							break;
					}
				}
			}
        }
};



/// Wrathion/58038
class npc_wrathion_chi_ji_temple : public CreatureScript
{
    public:
        npc_wrathion_chi_ji_temple() : CreatureScript("npc_wrathion_chi_ji_temple")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            if (p_Player->GetQuestStatus(32805) == QUEST_STATUS_INCOMPLETE)
            {
                if (!GetClosestCreatureWithEntry(p_Player, 64809, 80.0f))
                {
                    p_Player->KilledMonsterCredit(70926);
                    p_Player->SetPhaseMask(4, true);
                    p_Player->SummonCreature(64809, *p_Creature);
                }
            }
            return true;
        }
};

/// Wrathion/58038
class npc_wrathion_chi_ji_challenge : public CreatureScript
{
    public:
        npc_wrathion_chi_ji_challenge() : CreatureScript("npc_wrathion_chi_ji_challenge")
        {
        }

        struct npc_wrathion_chi_ji_challengeAI : public ScriptedAI
        {
            npc_wrathion_chi_ji_challengeAI(Creature* creature) : ScriptedAI(creature)
            {
                m_PlayerGuid    = 0;
                m_ActionCounter = 0;
                m_SceneDone     = false;
            }

            enum eWrathionDatas
            {
                EventLaunchEvent = 1,
                EventFightOver = 2,
                EventDespawn = 3,
                EventSummonDeathWing = 4,
                SpellEyesOfTheDestroyer = 142349
            };

            uint64 m_PlayerGuid;
            uint32 m_ActionCounter;
            EventMap m_Events;
            bool m_SceneDone;

            void DamageTaken(Unit*p_Attacker, uint32& p_Damage) override
            {
                if (p_Damage >= me->GetHealth())
                {
                    p_Damage = 0;
                    std::list<AreaTrigger*> l_AreatriggerList;

                    me->GetAreaTriggerList(l_AreatriggerList, 141466);

                    for (AreaTrigger* l_Areatrigger : l_AreatriggerList)
                        l_Areatrigger->Remove(0);

                    me->setFaction(35);
                    me->SetFullHealth();
                    me->RemoveAllAuras();

                    if (Creature* l_Xuen = GetClosestCreatureWithEntry(me, 71314, 50.0f))
                    {
                        if (l_Xuen->AI())
                            l_Xuen->AI()->Talk(7);
                    }

                    m_Events.ScheduleEvent(eWrathionDatas::EventFightOver, 500);
                }
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == 1)
                {
                    me->AddAura(eWrathionDatas::SpellEyesOfTheDestroyer, me);
                    m_Events.ScheduleEvent(eWrathionDatas::EventSummonDeathWing, 3000);
                }
            }

            uint64 GetGUID(int32 /* = 0 */)
            {
                return m_PlayerGuid;
            }

            void IsSummonedBy(Unit* p_Summoner) override
            {
                m_PlayerGuid = p_Summoner->GetGUID();
                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 1500);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case eWrathionDatas::EventLaunchEvent:
                    {
                        if (m_SceneDone)
                            return;

                        ++m_ActionCounter;

                        switch (m_ActionCounter)
                        {
                            case 1:
                                Talk(0);
                                me->GetMotionMaster()->MovePoint(1, -1903.301f, 1061.7844f, -4.39893f);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 1200);
                                break;
                            case 2:
                                me->GetMotionMaster()->MovePoint(2, -1898.295f, 1078.7844f, -9.2876f);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 1500);
                                break;
                            case 3:
                                me->GetMotionMaster()->MovePoint(3, -1888.901f, 1075.75635f, -9.335f);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 1600);
                                break;
                            case 4:
                                me->GetMotionMaster()->MovePoint(4, -1853.5467f, 1065.7245f, -23.5743f);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 3500);
                                break;
                            case 5:
                                me->GetMotionMaster()->MovePoint(5, -1815.369f, 1053.9361f, -31.7267f);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 6000);
                                break;
                            case 6:
                                Talk(1);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 3100);
                                break;
                            case 7:
                                Talk(2);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 3500);
                                break;
                            case 8:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(0);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 10000);
                                break;
                            case 9:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(1);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 7000);
                                break;
                            case 10:
                                Talk(3);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 5000);
                                break;
                            case 11:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(2);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 3800);
                                break;
                            case 12:
                                Talk(4);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 3800);
                                break;
                            case 13:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(3);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 3000);
                                break;
                            case 14:
                                Talk(5);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 8000);
                                break;
                            case 15:
                                Talk(6);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 9500);
                                break;
                            case 16:
                                Talk(7);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 11500);
                                break;
                            case 17:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(4);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 9000);
                                break;
                            case 18:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(5);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 5800);
                                break;
                            case 19:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(6);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 8000);
                                break;
                            case 20:
                                Talk(8);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 4000);
                                break;
                            case 21:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(7);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 7000);
                                break;
                            case 22:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(8);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 8500);
                                break;
                            case 23:
                                Talk(9);
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 4500);
                                break;
                            case 24:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(9);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 7000);
                                break;
                            case 25:
                                if (Creature* l_ChiJi = GetClosestCreatureWithEntry(me, 71314, 30.0f))
                                {
                                    if (l_ChiJi->AI())
                                        l_ChiJi->AI()->Talk(10);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 7000);
                                break;
                            case 26:
                                /// Deathwing Summon
                                m_ActionCounter = 0;
                                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                m_SceneDone = true;
                                Talk(10);
                                break;
                            default:
                                break;
                        }
                        break;
                    }
                    case eWrathionDatas::EventFightOver:
                    {
                        ++m_ActionCounter;

                        switch (m_ActionCounter)
                        {
                            case 1:
                                if (Creature* l_Xuen = GetClosestCreatureWithEntry(me, 71107, 50.0f))
                                {
                                    if (l_Xuen->AI())
                                        l_Xuen->AI()->Talk(8);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 12000);
                                break;
                            case 2:
                                if (Creature* l_Xuen = GetClosestCreatureWithEntry(me, 71107, 50.0f))
                                {
                                    if (l_Xuen->AI())
                                        l_Xuen->AI()->Talk(9);
                                }
                                m_Events.ScheduleEvent(eWrathionDatas::EventLaunchEvent, 4500);
                                break;
                            case 3:
                                Talk(7);
                                break;
                            default:
                                break;
                        }

                        break;
                    }
                    case eWrathionDatas::EventSummonDeathWing:
                        me->SummonCreature(71321, me->m_positionX, me->m_positionY + 2.5f, me->m_positionZ);
                        break;
                    case eWrathionDatas::EventDespawn:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }

                if (m_SceneDone)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_wrathion_chi_ji_challengeAI(creature);
        }
};

/// Wrathion/58038
class npc_chi_ji_challenge_gong : public CreatureScript
{
    public:
        npc_chi_ji_challenge_gong() : CreatureScript("npc_chi_ji_challenge_gong")
        {
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*sender*/, uint32 p_Action)
        {
            p_Player->PlayerTalkClass->ClearMenus();

            if (p_Action == 1)
            {
                /// Quest Celestial Blessings
                if (p_Player->GetQuestStatus(32805) == QUEST_STATUS_INCOMPLETE && p_Player->GetRoleConsideringSpe(p_Player->GetSpecializationId(p_Player->GetActiveSpec())) == Roles::ROLES_HEALER)
                {
                    if (Creature* l_Wrathion = GetClosestCreatureWithEntry(p_Creature, 64809, 40.0f))
                    {
                        if (l_Wrathion->AI())
                            l_Wrathion->AI()->DoAction(1);
                    }
                }
                p_Player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }
};

void AddSC_krasarang_wilds()
{
    new mob_gaarn_the_toxic();
    new mob_arness_the_scale();
    new mob_qu_nas();
    new mob_torik_ethis();
    new mob_go_kan();
    new mob_spirit_of_the_crane();
    new mob_champion_of_chi_ji();
    new spell_chi_torpedo_periodic();
    new mob_anduin_wrynn();
    new mob_anduin_wrynn_escort();
    new npc_chi_ji();
    new mob_sha_of_despair();
    new npc_thelonius();
    new mob_ellia_ravenmane();
    new mob_fat_long_fat();
    new mob_huck_wheelbarrow();
    new mob_dextrous_izissha();
    new mob_julia_bates();
    new mob_kuo_na();
    new mob_mindel_sunspeaker();
    new mob_minh_do_tan();
    new mob_thelonius();
    new mob_tukka_tuk();
    new mob_yan_quillpaw();
    new gob_signal_fire_krasarang();
    new npc_scout_layna();
    new npc_warlord_bloodhilt();
    new npc_high_marshal_twinbraid();
    new npc_hilda_hornswaggle();
    new npc_marshall_troteman();
    new playerScript_enter_chi_ji_temple();
    new npc_wrathion_chi_ji_temple();
    new npc_wrathion_chi_ji_challenge();
    new npc_chi_ji_challenge_gong();
///    new playerScript_mapshift_krasarang(); ///< Causes client crash, can't add 2 terrainswaps
}
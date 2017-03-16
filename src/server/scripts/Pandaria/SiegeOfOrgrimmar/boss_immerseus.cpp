#include "siege_of_orgrimmar.hpp"

enum Spells
{
    SPELL_BERSERK               = 145525, // don't use 'castspell' because it's aoe spell

    SPELL_ZERO_POWER            = 96301,

    SPELL_SHA_BOOLT_AOE         = 143290, // targetting
    SPELL_SHA_BOLT_MISSILE      = 143293,
    SPELL_SHA_BOLT_DMG          = 143295,
    SPELL_SHA_SPLASH            = 143297,

    SPELL_SHA_POOL_AURA         = 143462, // use aura for sha bolt pools, but prevent triggered dmg
    SPELL_SHA_POOL_SCRIPT       = 143461,
    SPELL_SHA_POOL_DMG          = 143460,

    SPELL_CORROSIVE_BLAST       = 143436,

    SPELL_SEEPING_SHA           = 143281,
    SPELL_SEEPING_SHA_DMG       = 143286,

    SPELL_SWIRL                 = 143309, 
    SPELL_SWIRL_2               = 143410,
    SPELL_SWIRL_DMG             = 143412, // main swirl from the boss
    SPELL_SWIRL_DMG_2           = 143413, // errupting swirls on the ground
    SPELL_SWIRL_SCRIPT          = 143415,

    SPELL_SHA_CORRUPTION        = 143579,
    
    SPELL_SWELLING_CORRUTION    = 143574,

    SPELL_SPLIT                 = 143020,
    SPELL_SPLIT_MISSILE_1       = 143022, // sha puddle
    SPELL_SPLIT_MISSILE_2       = 143024, // contaminated puddle
    SPELL_SPLIT_SUMMON_1        = 143453,
    SPELL_SPLIT_SUMMON_2        = 143454,

    SPELL_REFORM                = 143469,

    SPELL_SHA_RESIDUE           = 143459,
    SPELL_ERUPTING_SHA          = 143498,

    SPELL_CONGEALING            = 143538,
    SPELL_CONGEALING_AURA       = 143540,
    SPELL_PUTRIFIED_RESUDUE     = 143524,
    SPELL_PURIFIED              = 143523,
    SPELL_ERUPTING_WATER        = 145377,

    SPELL_ACHIEVEMENT           = 145889,

};

enum Adds
{
    NPC_SHA_BOLT            = 71544,
    NPC_CONTAMINATED_PUDDLE = 71604,
    NPC_SHA_PUDDLE          = 71603,
    NPC_SWIRL               = 71548,
    NPC_SWIRL_TARGET        = 71550,
};

enum Events
{
    EVENT_SHA_BOLT  = 1,
    EVENT_SEEPING_SHA,
    EVENT_CORROSIVE_BLAST,
    EVENT_SWIRL,
    EVENT_SPLIT,
    EVENT_REFORM,
    EVENT_REFORM_1,
    EVENT_MOVE_PUDDLE,
    EVENT_CHECK_PUDDLE,
    EVENT_CHECK_MELEE,
    EVENT_BERSERK,
};

enum Actions
{
    ACTION_PUDDLE_DIED  = 1,
    ACTION_SPLIT,
    ACTION_REFORM,
    ACTION_MOVE_SWIRL,
};

enum Phases
{
    PHASE_IMMERSEUS_BOSS,
    PHASE_IMMERSEUS_ADDS
};

#define MAX_PUDDLE_POINTS 32

const Position puddlePos[MAX_PUDDLE_POINTS] = 
{
    {1455.601318f, 661.137329f, 246.85f, 0.356930f},
    {1461.414551f, 663.377380f, 246.86f, 0.378310f},
    {1474.600830f, 670.615051f, 246.86f, 0.665416f},
    {1488.301758f, 681.498596f, 246.84f, 0.741774f},
    {1507.742065f, 700.772583f, 246.84f, 1.041098f},
    {1513.817261f, 711.771057f, 246.84f, 1.065533f},
    {1519.040283f, 723.025330f, 246.84f, 1.270173f},
    {1522.439209f, 735.315369f, 246.84f, 1.380130f},
    {1523.098755f, 758.624695f, 246.84f, 1.624478f},
    {1522.009155f, 773.242310f, 246.84f, 1.719162f},
    {1518.275269f, 785.543518f, 246.84f, 2.003216f},
    {1511.928223f, 796.272034f, 246.84f, 2.189532f},
    {1491.666260f, 816.293823f, 246.84f, 2.470533f},
    {1481.050903f, 824.383911f, 246.84f, 2.601870f},
    {1469.224731f, 829.512512f, 246.84f, 2.922577f},
    {1459.451416f, 830.224121f, 246.84f, 2.968393f},
    {1447.282104f, 831.876282f, 246.84f, 3.060024f},
    {1432.209229f, 832.271667f, 246.84f, 3.182199f},
    {1415.996948f, 830.680725f, 246.84f, 3.389895f},
    {1400.198120f, 823.627197f, 246.84f, 3.689223f},
    {1377.474243f, 803.850769f, 246.84f, 4.135154f},
    {1368.328491f, 789.614624f, 246.84f, 4.208457f},
    {1364.697388f, 780.012390f, 246.84f, 4.443637f},
    {1361.294556f, 767.282104f, 246.84f, 4.483343f},
    {1360.805664f, 745.632629f, 246.76f, 4.837642f},
    {1361.920898f, 736.954041f, 246.84f, 4.840696f},
    {1365.434937f, 720.942383f, 246.84f, 5.100311f},
    {1372.835083f, 707.264648f, 246.84f, 5.338545f},
    {1391.068726f, 684.659241f, 246.84f, 5.317163f},
    {1397.286499f, 678.322754f, 246.84f, 5.607323f},
    {1409.754395f, 669.343140f, 246.86f, 5.839451f},
    {1420.205566f, 665.879211f, 246.86f, 6.016602f}
};

#define MAX_SWIRL_POINTS 69

const Position swirlPos[MAX_SWIRL_POINTS] = 
{
    {1487.464111f, 820.573914f, 246.843847f, 2.851097f},
    {1483.964111f, 821.693237f, 246.843847f, 2.817499f},
    {1468.896118f, 826.547729f, 246.844167f, 2.909131f},
    {1456.378662f, 828.710266f, 246.844213f, 2.979382f},
    {1439.406494f, 830.934082f, 246.844213f, 3.119882f},
    {1428.211304f, 830.142212f, 246.844213f, 3.263438f},
    {1418.107788f, 828.381470f, 246.844213f, 3.382557f},
    {1402.880737f, 823.863464f, 246.844213f, 3.452806f},
    {1399.085571f, 818.854919f, 246.844213f, 5.294566f},
    {1411.434082f, 808.112427f, 246.844213f, 0.178134f},
    {1422.152222f, 809.762329f, 246.844213f, 6.183375f},
    {1436.858887f, 807.700134f, 246.844213f, 6.088694f},
    {1444.952026f, 805.963623f, 246.844213f, 6.055098f},
    {1456.801147f, 795.857666f, 246.844213f, 4.103389f},
    {1444.532104f, 794.825623f, 246.844213f, 3.352025f},
    {1429.040283f, 790.880737f, 246.844213f, 3.461982f},
    {1410.085205f, 775.187012f, 246.845495f, 4.023979f},
    {1393.301880f, 765.024414f, 246.845495f, 3.208473f},
    {1391.872559f, 772.953552f, 246.845495f, 1.217052f},
    {1386.915649f, 784.558105f, 246.845495f, 2.457110f},
    {1377.011108f, 773.724792f, 246.845495f, 4.243890f},
    {1364.892334f, 769.258484f, 246.845495f, 2.915258f},
    {1365.610352f, 785.148743f, 246.845495f, 1.482780f},
    {1358.791992f, 738.885864f, 246.844656f, 4.736073f},
    {1360.505127f, 726.221008f, 246.844656f, 4.946821f},
    {1368.800781f, 712.329407f, 246.844656f, 5.569902f},
    {1375.536621f, 717.005432f, 246.844656f, 1.611060f},
    {1373.879883f, 730.299927f, 246.844656f, 1.507213f},
    {1381.795410f, 733.744568f, 246.844656f, 5.582122f},
    {1392.619751f, 728.715088f, 246.844656f, 0.435146f},
    {1400.361694f, 737.273865f, 246.844656f, 0.899404f},
    {1418.521362f, 716.076782f, 246.845510f, 5.438570f},
    {1423.970337f, 703.457825f, 246.852407f, 4.885737f},
    {1415.678589f, 699.607605f, 246.851675f, 2.845447f},
    {1407.067871f, 695.299500f, 246.848837f, 4.436752f},
    {1412.809814f, 687.349426f, 246.849600f, 5.649319f},
    {1417.447632f, 678.563110f, 246.852484f, 4.546707f},
    {1403.742798f, 676.551514f, 246.853964f, 3.004273f},
    {1455.166992f, 669.881775f, 246.856405f, 5.796364f},
    {1469.832886f, 668.007324f, 246.852448f, 0.218726f},
    {1481.525024f, 673.573853f, 246.858358f, 0.542484f},
    {1485.030884f, 685.873596f, 246.845449f, 1.868062f},
    {1475.905518f, 689.903992f, 246.845999f, 3.285270f},
    {1461.079590f, 687.039856f, 246.851873f, 3.227237f},
    {1456.446899f, 695.258850f, 246.853765f, 1.281632f},
    {1463.775757f, 702.528687f, 246.852346f, 0.609680f},
    {1466.321899f, 710.755859f, 246.847708f, 2.054377f},
    {1456.144287f, 712.550476f, 246.851980f, 3.062304f},
    {1473.011597f, 730.861023f, 246.845693f, 1.052557f},
    {1477.371216f, 738.282532f, 246.845693f, 0.936493f},
    {1486.354492f, 740.582092f, 246.845693f, 6.141500f},
    {1488.745972f, 732.226196f, 246.845693f, 4.516599f},
    {1489.199829f, 722.414978f, 246.845693f, 5.325995f},
    {1494.694580f, 719.655029f, 246.845693f, 0.362278f},
    {1500.481445f, 725.311035f, 246.845693f, 0.930383f},
    {1507.764038f, 733.761292f, 246.845693f, 0.365333f},
    {1514.580200f, 730.206177f, 246.845693f, 4.822032f},
    {1512.615112f, 717.166931f, 246.845693f, 4.504382f},
    {1524.565308f, 764.396484f, 246.844824f, 1.431730f},
    {1524.019897f, 777.253967f, 246.844824f, 1.813523f},
    {1516.852417f, 789.461365f, 246.844824f, 2.864211f},
    {1511.241699f, 781.535645f, 246.844824f, 4.608231f},
    {1504.731567f, 777.073181f, 246.844824f, 2.320541f},
    {1494.234131f, 781.758850f, 246.844824f, 3.218513f},
    {1487.912109f, 779.528564f, 246.844824f, 3.948496f},
    {1485.296631f, 766.771729f, 246.844824f, 4.492164f},
    {1478.048706f, 765.134338f, 246.844824f, 2.363299f},
    {1461.304810f, 787.290466f, 246.845510f, 2.552668f},
    {1453.450806f, 791.219910f, 246.845510f, 2.833669f}
};

class boss_immerseus : public CreatureScript
{
    public:
        boss_immerseus() : CreatureScript("boss_immerseus") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_immerseusAI(creature);
        }

        struct boss_immerseusAI : public BossAI
        {
            boss_immerseusAI(Creature* creature) : BossAI(creature, DATA_IMMERSEUS)
            {
                ApplyAllImmunities(true);

                me->setActive(true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 35.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 35.0f);
            }

            void Reset()
            {
                if (instance->GetBossState(DATA_IMMERSEUS) == DONE)
                {
                    me->SetReactState(REACT_PASSIVE);
                    me->setFaction(35);
                    return;
                }

                _Reset();

                corruptionPct = 100;
                summonedPuddles = 0;

                me->setPowerType(POWER_MANA);
                me->SetMaxPower(POWER_MANA, 100);
                me->SetPower(POWER_MANA, 100);

                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);

                phase = PHASE_IMMERSEUS_BOSS;

                me->AddAura(SPELL_SEEPING_SHA, me);
                me->AddAura(SPELL_ZERO_POWER, me);

                me->SetReactState(REACT_DEFENSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void AttackStart(Unit* who)
            {
                if (me->Attack(who, true))
                    DoStartNoMovement(who);
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_SHA_BOLT, urand(6000, 20000));
                events.ScheduleEvent(EVENT_CORROSIVE_BLAST, 10000);
                events.ScheduleEvent(EVENT_SEEPING_SHA, 1000);
                events.ScheduleEvent(EVENT_SWIRL, 20000);
                events.ScheduleEvent(EVENT_CHECK_MELEE, 5000);
                events.ScheduleEvent(EVENT_BERSERK, 10 * MINUTE * IN_MILLISECONDS);

                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                instance->SetBossState(DATA_IMMERSEUS, IN_PROGRESS);
                DoZoneInCombat();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_PUDDLE_DIED)
                {
                    if (corruptionPct > 0)
                        corruptionPct -= 1;
                }
                else if (action == ACTION_SPLIT)
                {
                    Split();
                }
                else if (action == ACTION_REFORM)
                {
                    Reform();
                }
            }

            void KilledUnit(Unit* who)
            {
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                BossAI::SummonedCreatureDespawn(summon);

                if (summon->GetEntry() == NPC_SHA_PUDDLE ||
                    summon->GetEntry() == NPC_CONTAMINATED_PUDDLE)
                {
                    if (summonedPuddles > 0 && phase == PHASE_IMMERSEUS_ADDS)
                    {
                        summonedPuddles--;

                        if (summonedPuddles == 0)
                        {
                            events.RescheduleEvent(EVENT_REFORM, 1);
                        }
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (damage >= me->GetHealth())
                {
                    damage = 0;

                    if (corruptionPct > 0 && phase == PHASE_IMMERSEUS_BOSS)
                    {
                        phase = PHASE_IMMERSEUS_ADDS;

                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();

                        events.CancelEvent(EVENT_SHA_BOLT);
                        events.CancelEvent(EVENT_CORROSIVE_BLAST);
                        events.CancelEvent(EVENT_SWIRL);
                        events.CancelEvent(EVENT_CHECK_MELEE);

                        events.ScheduleEvent(EVENT_SPLIT, 100);
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
                        case EVENT_BERSERK:
                            // don't use 'castspell' because it's aoe spell
                            me->AddAura(SPELL_BERSERK, me);
                            break;
                        case EVENT_CHECK_MELEE:
                            if (!me->IsWithinMeleeRange(me->getVictim()))
                                DoCast(me, SPELL_CORROSIVE_BLAST);
                            events.ScheduleEvent(EVENT_CHECK_MELEE, 5000);
                            break;
                        case EVENT_SEEPING_SHA:
                            UpdateSeepingSha();
                            events.ScheduleEvent(EVENT_SEEPING_SHA, 1500);
                            break;
                        case EVENT_SHA_BOLT:
                            DoCastAOE(SPELL_SHA_BOOLT_AOE);
                            events.ScheduleEvent(EVENT_SHA_BOLT, urand(7000, 20000));
                            break;
                        case EVENT_CORROSIVE_BLAST:
                            DoCastVictim(SPELL_CORROSIVE_BLAST);
                            events.ScheduleEvent(EVENT_CORROSIVE_BLAST, urand(35000, 40000));
                            break;
                        case EVENT_SWIRL:
                        {
                            uint8 randPos = urand(0, MAX_PUDDLE_POINTS - 1);

                            if (Creature* pSwirlTarget = me->SummonCreature(NPC_SWIRL_TARGET, puddlePos[randPos], TEMPSUMMON_TIMED_DESPAWN, 13000))
                            {
                                SpawnSwirls();

                                me->SetOrientation(me->GetAngle(pSwirlTarget));
                                me->SetFacingToObject(pSwirlTarget);

                                pSwirlTarget->AI()->SetData(0, randPos);

                                DoCast(pSwirlTarget, SPELL_SWIRL);
                            }

                            break;
                        }
                        case EVENT_SPLIT:
                            DoCast(me, SPELL_SPLIT);
                            break;
                        case EVENT_REFORM:
                            DoCast(me, SPELL_REFORM);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            uint8 phase;
            uint32 corruptionPct;
            uint32 summonedPuddles;

        private:

            class PlayersRangeCheck
            {
                public:
                    PlayersRangeCheck(WorldObject const* obj, float range) : _obj(obj), _range(range) {}
                    bool operator()(Player* u)
                    {
                        if (!u->isAlive())
                            return false;

                        if (_obj->GetExactDist(u) >  _range)
                            return false;

                        return true;
                    }

                private:
                    WorldObject const* _obj;
                    float _range;
            };

        private:

            void UpdateSeepingSha()
            {
                std::list<Player*> players;

                PlayersRangeCheck checker(me, 29.0f);
                JadeCore::PlayerListSearcher<PlayersRangeCheck> searcher(me, players, checker);
                me->VisitNearbyWorldObject(29.0f, searcher);

                if (!players.empty())
                {
                    for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    {
                        Unit* player = (*itr);

                        DoCast(player, SPELL_SEEPING_SHA_DMG, true);
                    }
                }
            }

            void Split()
            {
                summons.DespawnEntry(NPC_SHA_BOLT);

                int32 maxPuddles = me->GetPower(POWER_MANA);
                if (maxPuddles > 25)
                {
                    maxPuddles = 25;
                }
                else if (maxPuddles <= 0)
                {
                    CompleteEncounter();
                    return;
                }

                std::list<Position> points;
                for (uint8 i = 0; i < MAX_PUDDLE_POINTS; ++i)
                {
                    Position pos = { puddlePos[i].GetPositionX(), puddlePos[i].GetPositionY(), puddlePos[i].GetPositionZ(), 0.0f };
                    points.push_back(pos);
                }

                JadeCore::Containers::RandomResizeList(points, maxPuddles);

                summonedPuddles = maxPuddles;

                uint32 i = 0;
                for (std::list<Position>::const_iterator itr = points.begin(); itr != points.end(); ++itr)
                {
                    if (i % 2 == 0)
                    {
                        me->CastSpell(itr->GetPositionX(), itr->GetPositionY(), itr->GetPositionZ(), SPELL_SPLIT_MISSILE_1, true);
                    }
                    else
                    {
                        me->CastSpell(itr->GetPositionX(), itr->GetPositionY(), itr->GetPositionZ(), SPELL_SPLIT_MISSILE_2, true);
                    }

                    ++i;
                }

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

                events.ScheduleEvent(EVENT_REFORM, 60000);
            }

            void Reform()
            {
                me->RemoveAura(SPELL_SPLIT);

                phase = PHASE_IMMERSEUS_BOSS;

                if (corruptionPct <= 0)
                {
                    CompleteEncounter();
                    return;
                }

                summons.DespawnEntry(NPC_SHA_PUDDLE);
                summons.DespawnEntry(NPC_CONTAMINATED_PUDDLE);

                me->SetPower(POWER_MANA, corruptionPct);
                me->SetHealth(me->CountPctFromMaxHealth(corruptionPct));

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

                me->SetReactState(REACT_AGGRESSIVE);

                events.ScheduleEvent(EVENT_SHA_BOLT, urand(6000, 20000));
                events.ScheduleEvent(EVENT_CORROSIVE_BLAST, 10000);
                events.ScheduleEvent(EVENT_SWIRL, 48500);
                events.ScheduleEvent(EVENT_CHECK_MELEE, 5000);
            }

            void SpawnSwirls()
            {
                for (uint8 i = 0; i < MAX_SWIRL_POINTS; ++i)
                {
                    if (Creature* pSwirl = me->SummonCreature(NPC_SWIRL, swirlPos[i], TEMPSUMMON_TIMED_DESPAWN, 13000))
                    {
                        pSwirl->CastSpell(pSwirl, SPELL_SWIRL_2);
                    }
                }
            }

            void CompleteEncounter()
            {
                events.Reset();
                summons.DespawnAll();

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                Reward();

                instance->SetBossState(DATA_IMMERSEUS, DONE);
                
                ProcessCompletedEncounter();

                me->SetReactState(REACT_PASSIVE);
                me->RemoveAllAuras();
                me->AttackStop();
                me->setFaction(35);
                me->DeleteThreatList();
                me->CombatStop(true);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

                Map::PlayerList const &  players = instance->instance->GetPlayers();
                if (!players.isEmpty())
                {
                    Player* player = players.getFirst()->getSource();
                    if (player)
                    {
                        if (InstanceMap* instanceMap = instance->instance->ToInstanceMap())
                        {
                            instanceMap->PermBindAllPlayersForRaid(player);

                             
                        }
                    }
                }
            }

            void Reward()
            {
                instance->UpdateEncounterState(ENCOUNTER_CREDIT_CAST_SPELL, SPELL_ACHIEVEMENT, me);
                instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_ACHIEVEMENT);
                instance->DoModifyPlayerCurrencies(396, 4000);

                switch (GetDifficulty())
                {
                    case MAN10_DIFFICULTY:
                        instance->DoRespawnGameObject(instance->GetData64(DATA_TEARS_OF_THE_VALE_10N), DAY);
                        break;
                    case MAN25_DIFFICULTY:
                        instance->DoRespawnGameObject(instance->GetData64(DATA_TEARS_OF_THE_VALE_25N), DAY);
                        break;
                    case MAN10_HEROIC_DIFFICULTY:
                        instance->DoRespawnGameObject(instance->GetData64(DATA_TEARS_OF_THE_VALE_10H), DAY);
                        break;
                    case MAN25_HEROIC_DIFFICULTY:
                        instance->DoRespawnGameObject(instance->GetData64(DATA_TEARS_OF_THE_VALE_25H), DAY);
                        break;
                }
            }
        };  
};

class npc_immerseus_contaminated_puddle : public CreatureScript
{
    public:
        npc_immerseus_contaminated_puddle() : CreatureScript("npc_immerseus_contaminated_puddle") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_immerseus_contaminated_puddleAI(creature);
        }

        struct npc_immerseus_contaminated_puddleAI : public ScriptedAI
        {
            npc_immerseus_contaminated_puddleAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetSpeed(MOVE_RUN, 0.5f);
            }

            void Reset()
            {
                me->AddAura(SPELL_CONGEALING, me);

                me->SetHealth(me->CountPctFromMaxHealth(10));

                isHealed = false;
            }

            void IsSummonedBy(Unit* owner)
            {
                events.ScheduleEvent(EVENT_MOVE_PUDDLE, 1000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE_PUDDLE:
                            if (InstanceScript* pInstance = me->GetInstanceScript())
                            {
                                if (Creature* pImmerseus = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IMMERSEUS)))
                                {
                                    me->GetMotionMaster()->MoveFollowExact(pImmerseus, 0.0f, 0.0f);
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_PUDDLE, 1000);
                            break;
                        case EVENT_CHECK_PUDDLE:
                            if (InstanceScript* pInstance = me->GetInstanceScript())
                            {
                                if (Creature* pImmerseus = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IMMERSEUS)))
                                {
                                    if (me->GetExactDist(pImmerseus) <= 30.0f)
                                    {
                                        me->GetMotionMaster()->MovementExpired(false);
                                        me->GetMotionMaster()->Clear(false);

                                        DoCastAOE(SPELL_ERUPTING_WATER, true);

                                        me->DespawnOrUnsummon(100);
                                        return;
                                    }
                                }
                            }

                            if (!isHealed && me->IsFullHealth())
                            {
                                isHealed = true;

                                me->AddAura(SPELL_PURIFIED, me);
                                DoCastAOE(SPELL_PUTRIFIED_RESUDUE, true);

                                me->RemoveAura(SPELL_CONGEALING);

                                if (InstanceScript* pInstance = me->GetInstanceScript())
                                {
                                    if (Creature* pImmerseus = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IMMERSEUS)))
                                    {
                                        pImmerseus->AI()->DoAction(ACTION_PUDDLE_DIED);
                                    }
                                }

                                me->DespawnOrUnsummon(1000);
                            }

                            events.ScheduleEvent(EVENT_CHECK_PUDDLE, 1000);
                            break;
                    }
                }
            }

            void JustDied(Unit* who)
            {
                me->DespawnOrUnsummon(1000);
            }

        private:

            bool isHealed;
        };
};

class npc_immerseus_sha_puddle : public CreatureScript
{
    public:
        npc_immerseus_sha_puddle() : CreatureScript("npc_immerseus_sha_puddle") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_immerseus_sha_puddleAI(creature);
        }

        struct npc_immerseus_sha_puddleAI : public ScriptedAI
        {
            npc_immerseus_sha_puddleAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);

                me->SetSpeed(MOVE_RUN, 0.9f);
            }

            void Reset()
            {

            }

            void IsSummonedBy(Unit* owner)
            {
                events.ScheduleEvent(EVENT_MOVE_PUDDLE, 1000);
            }

            void JustDied(Unit* who)
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (Creature* pImmerseus = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IMMERSEUS)))
                    {
                        pImmerseus->AI()->DoAction(ACTION_PUDDLE_DIED);
                    }
                }

                DoCastAOE(SPELL_SHA_RESIDUE, true);

                me->DespawnOrUnsummon(1000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE_PUDDLE:
                            if (InstanceScript* pInstance = me->GetInstanceScript())
                            {
                                if (Creature* pImmerseus = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IMMERSEUS)))
                                {
                                    me->GetMotionMaster()->MoveFollowExact(pImmerseus, 0.0f, 0.0f);
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_PUDDLE, 1000);
                            break;
                        case EVENT_CHECK_PUDDLE:
                            if (InstanceScript* pInstance = me->GetInstanceScript())
                            {
                                if (Creature* pImmerseus = pInstance->instance->GetCreature(pInstance->GetData64(DATA_IMMERSEUS)))
                                {
                                    if (me->GetExactDist(pImmerseus) <= 30.0f)
                                    {
                                        me->GetMotionMaster()->MovementExpired(false);
                                        me->GetMotionMaster()->Clear(false);

                                        DoCastAOE(SPELL_ERUPTING_SHA, true);

                                        me->DespawnOrUnsummon(100);
                                        return;
                                    }
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_PUDDLE, 1000);
                            break;
                    }
                }
            }
        };
};

class npc_immerseus_sha_bolt : public CreatureScript
{
    public:
        npc_immerseus_sha_bolt() : CreatureScript("npc_immerseus_sha_bolt") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_immerseus_sha_boltAI(creature);
        }

        struct npc_immerseus_sha_boltAI : public ScriptedAI
        {
            npc_immerseus_sha_boltAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            
                me->AddAura(SPELL_SHA_POOL_AURA, me);

                updateTimer = 1000;
            }

            void UpdateAI(const uint32 diff)
            {
                if (updateTimer <= diff)
                {
                    updateTimer = 1000;

                    std::list<Player*> players;
                    me->GetPlayerListInGrid(players, 1.0f);

                    if (!players.empty())
                    {
                        for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        {
                            Player* player = (*itr);

                            if (player->HasAura(SPELL_SHA_SPLASH))
                                continue;

                            me->AddAura(SPELL_SHA_SPLASH, player);
                        }
                    }
                }
                else
                {
                    updateTimer -= diff;
                }
            }

        private:

            uint32 updateTimer;
        };
};

class npc_immerseus_swirl_target : public CreatureScript
{
    public:
        npc_immerseus_swirl_target() : CreatureScript("npc_immerseus_swirl_target") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_immerseus_swirl_targetAI(creature);
        }

        struct npc_immerseus_swirl_targetAI : public ScriptedAI
        {
            npc_immerseus_swirl_targetAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetDisplayId(11686);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            
                pos = 0;
                isMoving = false;
                updateTimer = 500;
                ownerGUID = 0;

                me->SetSpeed(MOVE_RUN, 10.0f, true);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_MOVE_SWIRL)
                {
                    BeginMovement();                    
                }
            }

            void SetData(uint32 type, uint32 data)
            {
                pos = data;
            }

            void IsSummonedBy(Unit* owner)
            {
                if (owner)
                    ownerGUID = owner->GetGUID();
            }

            void UpdateAI(const uint32 diff)
            {
                if (isMoving)
                {
                    UpdateDamage(diff);
                }
            }

        private:

            void BeginMovement()
            {
                if (isMoving)
                    return;

                isMoving = true;

                Movement::MoveSplineInit init(*me);

                if (pos > 0)
                {
                    for (int8 i = pos; i >= 0;  --i)
                    {
                        G3D::Vector3 point;
                        point.x = puddlePos[i].GetPositionX();
                        point.y = puddlePos[i].GetPositionY();
                        point.z = puddlePos[i].GetPositionZ();
                        init.Path().push_back(point);
                    }
                }

                for (uint8 i = MAX_PUDDLE_POINTS - 1; i > pos; --i)
                {
                    G3D::Vector3 point;
                    point.x = puddlePos[i].GetPositionX();
                    point.y = puddlePos[i].GetPositionY();
                    point.z = puddlePos[i].GetPositionZ();
                    init.Path().push_back(point);
                }

                init.SetCyclic();
                init.SetWalk(false);
                init.SetVelocity(10.0f);
                init.Launch();
            }

            void UpdateDamage(const uint32 diff)
            {
                if (updateTimer <= diff)
                {
                    updateTimer = 500;

                    if (Creature* pOwner = me->GetCreature(*me, ownerGUID))
                    {
                        pOwner->SetOrientation(pOwner->GetAngle(me));
                        //pOwner->SetFacingToObject(me);
                            
                        std::list<Unit*> targets;
                        pOwner->AI()->SelectTargetList(targets, SwirlSelector(pOwner, me), 25, SELECT_TARGET_RANDOM);

                        if (!targets.empty())
                        {
                            for (std::list<Unit*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                            {
                                Unit* target = (*itr);

                                target->CastSpell(target, SPELL_SWIRL_DMG_2, true);
                            }
                        }
                    }
                }
                else
                {
                    updateTimer -= diff;
                }
            }

        private:

            uint32 pos;
            uint32 updateTimer;
            bool isMoving;
            uint64 ownerGUID;

        private:

            struct SwirlSelector : public std::unary_function<Unit*, bool>
            {
                public:
                    SwirlSelector(Creature* source, Creature* swirlTarget) : _source(source), _swirlTarget(swirlTarget) { }
                    bool operator()(Unit const* target) const
                    {
                        if (!target)
                            return false;

                        if (target->GetTypeId() != TYPEID_PLAYER)
                            return false;

                        //if (!target->IsInBetween(_source, _swirlTarget, 15.0f))
                        if (!_source->HasInLine(target, 15.0f))
                            return false;

                        if (target->HasAura(SPELL_SWIRL_DMG))
                            return false;

                        return true;
                    }

                private:
                    Creature const* _source;
                    Creature const* _swirlTarget;
            };

        };
};

class npc_immerseus_swirl : public CreatureScript
{
    public:
        npc_immerseus_swirl() : CreatureScript("npc_immerseus_swirl") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_immerseus_swirlAI(creature);
        }

        struct npc_immerseus_swirlAI : public Scripted_NoMovementAI
        {
            npc_immerseus_swirlAI(Creature* creature) : Scripted_NoMovementAI(creature),
                damageTimer(3000), damageDone(false)
            {
                me->SetDisplayId(11686);
                me->SetSpeed(MOVE_RUN, 0.3f);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            }

            void UpdateAI(const uint32 diff) override
            {
                UpdateDamage(diff);
            }

        private:

            void UpdateDamage(const uint32 diff)
            {
                if (damageDone)
                    return;

                if (damageTimer <= diff)
                {
                    damageDone = true;
                    damageTimer = 3000;

                    std::list<Player*> targets;
                    me->GetPlayerListInGrid(targets, 2.0f);

                    if (!targets.empty())
                    {
                        for (std::list<Player*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                        {
                            Unit* target = (*itr);

                            if (!target->HasAura(SPELL_SWIRL_DMG_2))
                            {
                                target->CastSpell(target, SPELL_SWIRL_DMG_2, true);
                            }
                        }
                    }  
                }
                else
                {
                    damageTimer -= diff;
                }
            }

        private:

            uint32 damageTimer;
            bool damageDone;

        };
};

class spell_immerseus_sha_bolt_aoe : public SpellScriptLoader
{
    public:
        spell_immerseus_sha_bolt_aoe() : SpellScriptLoader("spell_immerseus_sha_bolt_aoe") { }

        class spell_immerseus_sha_bolt_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_immerseus_sha_bolt_aoe_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHA_BOLT_MISSILE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_immerseus_sha_bolt_aoe_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_immerseus_sha_bolt_aoe_SpellScript();
        }
};

class spell_immerseus_split : public SpellScriptLoader
{
    public:
        spell_immerseus_split() : SpellScriptLoader("spell_immerseus_split") { }

        class spell_immerseus_split_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_immerseus_split_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (!GetCaster())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    pCreature->AI()->DoAction(ACTION_SPLIT);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_immerseus_split_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_immerseus_split_SpellScript();
        }
};

class spell_immerseus_reform : public SpellScriptLoader
{
    public:
        spell_immerseus_reform() : SpellScriptLoader("spell_immerseus_reform") { }

        class spell_immerseus_reform_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_immerseus_reform_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (!GetCaster())
                    return;

                if (Creature* pCreature = GetCaster()->ToCreature())
                {
                    pCreature->AI()->DoAction(ACTION_REFORM);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_immerseus_reform_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_immerseus_reform_SpellScript();
        }
};

class spell_immerseus_congealing : public SpellScriptLoader
{
    public:
        spell_immerseus_congealing() : SpellScriptLoader("spell_immerseus_congealing") { }

        class spell_immerseus_congealing_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_immerseus_congealing_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                int32 healthPct = int32(GetUnitOwner()->GetHealthPct() / 10.0f);

                if (healthPct > 7)
                {
                    healthPct = 7;
                }
                else if (healthPct < 0)
                {
                    healthPct = 0;
                }

                
                if (Aura* aur = GetUnitOwner()->GetAura(SPELL_CONGEALING_AURA))
                {
                    uint8 currentStacks = aur->GetStackAmount();
                    if (healthPct != currentStacks)
                    {
                        if (healthPct <= 0)
                        {
                            aur->Remove();
                        }
                        else
                        {
                            aur->SetStackAmount(healthPct);
                        }
                    }
                }
                else if (healthPct > 0)
                {
                    if (Aura* aur = GetUnitOwner()->AddAura(SPELL_CONGEALING_AURA, GetUnitOwner()))
                    {
                        aur->SetStackAmount(healthPct);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_immerseus_congealing_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_immerseus_congealing_AuraScript();
        }
};

class spell_immerseus_sha_pool : public SpellScriptLoader
{
    public:
        spell_immerseus_sha_pool() : SpellScriptLoader("spell_immerseus_sha_pool") { }

        class spell_immerseus_sha_pool_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_immerseus_sha_pool_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (GetUnitOwner()->GetEntry() == NPC_SHA_BOLT)
                {
                    PreventDefaultAction();
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_immerseus_sha_pool_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_immerseus_sha_pool_AuraScript();
        }
};

class spell_immerseus_sha_pool_dmg : public SpellScriptLoader
{
    public:
        spell_immerseus_sha_pool_dmg() : SpellScriptLoader("spell_immerseus_sha_pool_dmg") { }

        class spell_immerseus_sha_pool_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_immerseus_sha_pool_dmg_SpellScript);

            void ScaleRange(std::list<WorldObject*>& targets)
            {
                targets.remove_if(ExactDistanceCheck(GetCaster(), 2.5f * GetCaster()->GetFloatValue(OBJECT_FIELD_SCALE_X)));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_immerseus_sha_pool_dmg_SpellScript::ScaleRange, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }

        private:

            class ExactDistanceCheck
            {
            public:
                ExactDistanceCheck(Unit* source, float dist) : _source(source), _dist(dist) {}

                bool operator()(WorldObject* unit) const
                {
                    return _source->GetExactDist2d(unit) > _dist;
                }

            private:
                Unit* _source;
                float _dist;
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_immerseus_sha_pool_dmg_SpellScript();
        }
};

class spell_immerseus_swirl : public SpellScriptLoader
{
    public:
        spell_immerseus_swirl() : SpellScriptLoader("spell_immerseus_swirl") { }

        class spell_immerseus_swirl_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_immerseus_swirl_AuraScript);

            void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetUnitOwner())
                    return;

                if (Creature* pCreature = GetUnitOwner()->ToCreature())
                {
                    pCreature->AI()->DoAction(ACTION_MOVE_SWIRL);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_immerseus_swirl_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_immerseus_swirl_AuraScript();
        }
};

class spell_immerseus_sha_splash : public SpellScriptLoader
{
    public:
        spell_immerseus_sha_splash() : SpellScriptLoader("spell_immerseus_sha_splash") { }

        class spell_immerseus_sha_splash_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_immerseus_sha_splash_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (!GetUnitOwner()->FindNearestCreature(NPC_SHA_BOLT, 1.0f))
                {
                    Remove();
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_immerseus_sha_splash_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_immerseus_sha_splash_AuraScript();
        }
};

void AddSC_boss_immerseus()
{
    new boss_immerseus();                       // 71543
    new npc_immerseus_contaminated_puddle();    // 71604
    new npc_immerseus_sha_puddle();             // 71603
    new npc_immerseus_sha_bolt();               // 71544
    new npc_immerseus_swirl_target();           // 71550
    new npc_immerseus_swirl();                  // 71548

    new spell_immerseus_sha_bolt_aoe();         // 143290
    new spell_immerseus_split();                // 143020
    new spell_immerseus_reform();               // 143469
    new spell_immerseus_congealing();           // 143538
    new spell_immerseus_sha_pool();             // 143462
    new spell_immerseus_sha_pool_dmg();         // 143460
    new spell_immerseus_swirl();                // 143309
    new spell_immerseus_sha_splash();           // 143297
}
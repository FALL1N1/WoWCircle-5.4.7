#include "siege_of_orgrimmar.hpp"

#define TRIGGER_DISPLAY_ID 169

enum ScriptedTexts
{
    SAY_AGGRO   = 0,
    SAY_WIPE    = 1,
    SAY_WIN     = 2,
    SAY_EVENT_6 = 8,
    SAY_EVENT_7 = 9,
};

enum Spells
{
    SPELL_ENTER_QUARANTINE  = 145633,
    SPELL_QUARANTINE_SAFETY_MEASURES_1  = 145779,
    SPELL_QUARANTINE_SAFETY_MEASURES_2  = 145773, // triggered by 145779
    SPELL_QUARANTINE_SAFETY_MEASURES_3  = 145776, // triggered by 145773 145779

    SPELL_EXTRACT_CORRUPTION_FORCE      = 145139,
    SPELL_EXTRACT_CORRUPTION_DUMMY_1    = 145140, // triggered by 145139
    SPELL_EXTRACT_CORRUPTION            = 145143,

    SPELL_CORRUPTION                    = 144421,

    // Amalgam
    SPELL_UNCHECKED_CORRUPTION          = 145679, // if there are no melee target

    SPELL_UNLEASHED_ANGER               = 145214,
    SPELL_UNLEASHED_ANGER_DUMMY         = 145216, // triggered by 145214
    SPELL_UNLEASHED_ANGER_DMG           = 145212,

    SPELL_SELF_DOUBT                    = 146124,

    SPELL_BLIND_HATRED                  = 145226, // main beam
    SPELL_BLIND_HATRED_2                = 145613, // another beam ?
    SPELL_BLIND_HATRED_DUMMY            = 145571, // some animation, maybe on hit players
    SPELL_BLIND_HATRED_DMG              = 145227,
    SPELL_BLIND_HATRED_PERIODIC         = 145573, // every 250 ms

    SPELL_FUSION                        = 145765, // ?
    SPELL_FUSION_AURA                   = 145132,

    SPELL_ICY_FEAR_PERIODIC             = 145733,
    SPELL_ICY_FEAR_DMG                  = 145735, // requires 145178 target aura (?) seems like quarantine zone mark

    SPELL_FRAYED                        = 146179,

    SPELL_UNLEASH_CORRUPTION_SUMMON_1   = 145005, // triggered by 144478 (72262 unleashed fragment of corruption)
    SPELL_UNLEASH_CORRUPTION_SUMMON_2   = 145006, // triggered by 144490 145768 (72263 essence of corruption)
    SPELL_UNLEASH_CORRUPTION_SUMMON_3   = 145007, // triggered by 144491 145769 (72264 manifestation of corruption)
    SPELL_UNLEASH_CORRUPTION_MISSILE_1  = 145768, // summons 145006
    SPELL_UNLEASH_CORRUPTION_MISSILE_2  = 145769, // summons 145007

    SPELL_SPAWN_VICTORY_ORB_1           = 144478, // triggers 145005
    SPELL_SPAWN_VICTORY_ORB_2           = 144490, // triggers 145006
    SPELL_SPAWN_VICTORY_ORB_3           = 144491, // triggers 145007
    SPELL_SPAWN_VICTORY_ORB_4           = 145241, // big yellow orb
    SPELL_SPAWN_VICTORY_ORB_5           = 145242, // medium yellow orb
    SPELL_SPAWN_VICTORY_ORB_6           = 145243, // small yellow orb
    SPELL_SPAWN_VICTORY_ORB_7           = 146716, // small yellow orb

    SPELL_LOOK_WITHIN_DUMMY             = 144717, // some visual effect, yellow orb
    SPELL_LOOK_WITHIN_1                 = 144724, // phase 173
    SPELL_LOOK_WITHIN_2                 = 144725, // phase 174
    SPELL_LOOK_WITHIN_3                 = 144726, // phase 2439
    SPELL_LOOK_WITHIN_4                 = 144727, // phase 2440
    SPELL_LOOK_WITHIN_5                 = 146826, // phase 2441
    SPELL_LOOK_WITHIN_6                 = 146827, // phase 2510
    SPELL_LOOK_WITHIN_7                 = 146828, // phase 2511
    SPELL_LOOK_WITHIN_8                 = 146829, // phase 2512
    SPELL_LOOK_WITHIN_9                 = 146830, // phase 2513
    SPELL_LOOK_WITHIN_10                = 146831, // phase 2514
    SPELL_LOOK_WITHIN_11                = 146832, // phase 2515
    SPELL_LOOK_WITHIN_12                = 146833, // phase 2516
    SPELL_LOOK_WITHIN_13                = 146834, // phase 2517
    SPELL_LOOK_WITHIN_14                = 146835, // phase 2518
    SPELL_LOOK_WITHIN_15                = 146836, // phase 2519
    SPELL_LOOK_WITHIN_16                = 146837, // phase 2520

    SPELL_TEST_OF_SERENITY              = 144849,
    SPELL_TEST_OF_RELIANCE              = 144850,
    SPELL_TEST_OF_RELIANCE_DUMMY        = 146712, // triggered by 144850
    SPELL_TEST_OF_CONFIDENCE            = 144851,

    SPELL_SPAWN_VICTORY_ORB             = 144490,

    SPELL_CLEANSE_15                    = 144449,
    SPELL_CLEANSE_40                    = 144450,
    SPELL_CLEANSE_100                   = 147657,

    SPELL_PURIFIED                      = 144452,
    SPELL_PURIFIED_IMMUNE               = 146022,

    SPELL_FOUL_LINK_1                   = 148974,
    SPELL_FOUL_LINK_2                   = 149189,

    // Manifestation of Corruption
    SPELL_MANIFESTATION_OF_CORRUPTION_SUMMON = 144739, // player summons it
    SPELL_TEAR_REALITY                  = 144482, // for testing
    SPELL_BURST_OF_ANGER                = 147082, // for all, requires 145178 target aura (?) seems like quarantine zone mark
    SPELL_RESIDUAL_CORRUPTION_AURA      = 145074,
    SPELL_RESIDUAL_CORRUPTION_DMG       = 145073, // requires 145178 target aura (?) seems like quarantine zone mark
    SPELL_CORRUPT                       = 145052,

    // Essense of Corruption
    SPELL_ESSENSE_OF_CORRUPTION_SUMMON  = 144733, // player summons it
    SPELL_ESSENSE_OF_CORRUPTION         = 148452, // for testing, triggers 8279 and 149601
    SPELL_DARK_BULWARK                  = 149601,
    SPELL_EXPEL_CORRUPTION_1            = 144479, // for testing
    SPELL_EXPELLED_CORRUPTION_1         = 144480, // dmg aura
    SPELL_EXPEL_CORRUPTION_2            = 145064, // for all
    SPELL_EXPELLED_CORRUPTION_2         = 145134, // for all, dmg

    // Greater Corruption
    SPELL_GREATER_CORRUPTION_SUMMON     = 144980,
    SPELL_DISHEARTENING_LAUGH           = 146707,
    SPELL_LINGERING_CORRUPTION          = 144514,
    SPELL_BOTTOMLESS_PIT_AOE_1          = 146701, // targetting
    SPELL_BOTTOMLESS_PIT_AOE_2          = 146705,
    SPELL_BOTTOMLESS_PIT_AREATRIGGER    = 146793,
    SPELL_BOTTOMLESS_PIT_DMG            = 146703,

    // Titanic Corruption
    SPELL_TITANIC_CORRUPTION_SUMMON     = 144848,
    SPELL_BURST_OF_CORRUPTION           = 144654,
    SPELL_CORRUPTION_DMG                = 144639,
    SPELL_HURL_CORRUPTION               = 144649,
    SPELL_PIERCING_CORRUPTION           = 144657,
    SPELL_TITANIC_SMASH                 = 144628,

};

enum Adds
{
    NPC_QUARANTINE_MEASURES             = 72669,
    NPC_QUARANTINE_NOVA                 = 72670,
    NPC_BLIND_HATRED_1                  = 72565, // I think it's a creature at the center (near the boss)
    NPC_BLIND_HATRED_2                  = 72595, // and this is a creature that moves around the room
    NPC_PURIFYING_LIGHT                 = 72065, //73224,

    NPC_MANIFESTATION_OF_CORRUPTION_1   = 71977, // for testing
    NPC_ESSENCE_OF_CORRUPTION_1         = 71976, // for testing
    NPC_MANIFESTATION_OF_CORRUPTION_2   = 72264, // for all players
    NPC_ESSENSE_OF_CORRUPTION_2         = 72263, // for all players
    NPC_RESIDUAL_CORRUPTION             = 72550,
    NPC_GREATER_CORRUPTION              = 72001,
    NPC_TITANIC_CORRUPTION              = 72051,
    NPC_UNLEASHED_FRAGMENT_OF_CORRUPTION    = 72262,
};

enum Events
{
    EVENT_BERSERK = 1,

    EVENT_ACTIVATE_AMALGAM,
    EVENT_TELEPORT_TO_EXIT,
    EVENT_NORUSHEN_TALK_1,
    EVENT_NORUSHEN_TALK_2,
    EVENT_NORUSHEN_TALK_3,

    // Amalgam
    EVENT_UNCHECKED_CORRUPTION,
    EVENT_UNLEASHED_ANGER,
    EVENT_SELF_DOUBT,
    EVENT_BLIND_HATRED,
    EVENT_PURIFYING_LIGHT,

    EVENT_MOVE,
    EVENT_TEAR_REALITY,
    EVENT_BURST_OF_ANGER,

    EVENT_EXPEL_CORRUPTION,

    EVENT_DISHEARTENING_LAUGH,
    EVENT_LINGERING_CORRUPTION,
    EVENT_BOTTOMLESS_PIT,

    EVENT_BURST_OF_CORRUPTION,
    EVENT_HURL_CORRUPTION,
    EVENT_PIERCING_CORRUPTION,
    EVENT_TITANIC_SMASH,
};

enum Actions
{
    ACTION_START_BATTLE = 1,
    ACTION_EVADE_COMBAT,
    ACTION_COMPLETE_ENCOUNTER,

    ACTION_ACTIVATE_AMALGAM,
    ACTION_BLIND_HATRED_START,
    ACTION_SPAWN_LIGHT,
    ACTION_SPAWN_MANIFESTATION,
    ACTION_SPAWN_ESSENSE,
};

enum eData
{
    DATA_BLIND_HATRED_ID    = 1,
    DATA_PURIFYING_LIGHT_ID,

    DATA_PLAYER_TEST_ADD,
    DATA_PLAYER_TEST_REMOVE,
    DATA_PLAYER_TEST_DIED,
    DATA_PLAYER_TEST_PASSED,
    DATA_PLAYER_TEST_KILLED_CREATURE,

    DATA_EXPEL_CORRUPTION_TARGET,
};

const Position centerPos = {777.24f, 974.06f, 356.34f, 4.92f};

#define MAX_PURIFYING_LIGHT_POINTS 5
const Position purifyingLightPos[MAX_PURIFYING_LIGHT_POINTS] = 
{
    {770.24f, 1006.57f, 356.90f, 4.96f},
    {745.50f, 982.42f, 356.43f, 5.99f},
    {754.09f, 950.79f, 356.43f, 0.80f},
    {808.88f, 964.13f, 356.43f, 2.78f},
    {802.05f, 996.42f, 356.44f, 3.86f}
};

#define MAX_BLIND_HATRED_POINTS 4
const Position blindHatredPos[MAX_BLIND_HATRED_POINTS] = 
{
    {830.26f, 927.20f, 360.0f, 1.80f},
    {809.09f, 1023.42f, 360.0f, 3.38f},
    {728.46f, 1006.08f, 360.0f, 3.45f},
    {748.99f, 909.47f, 360.0f, 0.30f}
};

#define MAX_PHASE_SPELLS 16
const uint32 phaseSpells[MAX_PHASE_SPELLS] = 
{
    SPELL_LOOK_WITHIN_1,
    SPELL_LOOK_WITHIN_2,
    SPELL_LOOK_WITHIN_3,
    SPELL_LOOK_WITHIN_4,
    SPELL_LOOK_WITHIN_5,
    SPELL_LOOK_WITHIN_6,
    SPELL_LOOK_WITHIN_7,
    SPELL_LOOK_WITHIN_8,
    SPELL_LOOK_WITHIN_9,
    SPELL_LOOK_WITHIN_10,
    SPELL_LOOK_WITHIN_11,
    SPELL_LOOK_WITHIN_12,
    SPELL_LOOK_WITHIN_13,
    SPELL_LOOK_WITHIN_14,
    SPELL_LOOK_WITHIN_15,
    SPELL_LOOK_WITHIN_16
};

#define MAX_ESSENSES 4
const Position essenseOfCorruptionPos[MAX_ESSENSES] = 
{
    {787.456848f, 985.921326f, 356.34f, 2.606646f},
    {765.036438f, 983.373108f, 356.34f, 4.118547f},
    {767.934448f, 962.017456f, 356.34f, 5.700681f},
    {790.376404f, 965.921204f, 356.35f, 2.453494f}
};

const Position manifestationPos = {777.564575f, 974.335632f, 356.35f, 2.584832f};

const Position norushenStartPos = {768.84f, 1013.45f, 356.08f, 4.98f};
const Position norushenExitPos = {794.67f, 892.50f, 371.13f, 1.76f};

#define TIMER_TESTS_UPDATE 1 * IN_MILLISECONDS
#define TIMER_TESTS_DURATION 1 * MINUTE * IN_MILLISECONDS

#define MAX_CONTAINMENTS 6
const uint8 containmentData[MAX_CONTAINMENTS] = 
{
    DATA_LIGHT_CONTAINMENT_1,
    DATA_LIGHT_CONTAINMENT_2,
    DATA_LIGHT_CONTAINMENT_3,
    DATA_LIGHT_CONTAINMENT_4,
    DATA_LIGHT_CONTAINMENT_TOP,
    DATA_LIGHT_CONTAINMENT_BOTTOM
};

class TestController
{
    private:

        enum PlayerTestTypes
        {
            TEST_NONE       = 0,
            TEST_SERENITY   = 1,
            TEST_RELIANCE   = 2,
            TEST_CONFIDENCE = 3,
        };

        class TestPlayer
        {
            public:

                uint64 playerGuid;
                PlayerTestTypes testType;
                uint32 testSpell;
                uint32 phaseSpell;
                uint32 timer;
                uint32 creaturesCount;

            public:

                void DecreaseCreaturesCount()
                {
                    if (creaturesCount > 0)
                        creaturesCount--;
                }

                bool IsAllCreaturesKilled() const
                {
                    return creaturesCount == 0;
                }
        };

    public:

        TestController() 
        {
            m_UpdateTestsTimer = TIMER_TESTS_UPDATE;
        }

    public:

        bool AddPlayerTest(uint64 playerGuid, Roles role)
        {
            Player* player = sObjectAccessor->FindPlayer(playerGuid);
            if (!player)
                return false;

            if (HasPlayerTest(playerGuid))
                return false;

            uint32 phaseSpell = GetFreePhaseSpell();
            if (!phaseSpell)
                return false;

            uint32 testSpell = 0;
            PlayerTestTypes testType = GetTestTypeForPlayerRole(role);
            switch (testType)
            {
                case TEST_SERENITY:
                    testSpell = SPELL_TEST_OF_SERENITY;
                    break;
                case TEST_RELIANCE:
                    testSpell = SPELL_TEST_OF_RELIANCE;
                    break;
                case TEST_CONFIDENCE:
                    testSpell = SPELL_TEST_OF_CONFIDENCE;
                    break;
                default:
                    return false;
            }

            TestPlayer& test = m_testPlayers[playerGuid];
            test.playerGuid = playerGuid;
            test.testType = testType;
            test.testSpell = testSpell;
            test.phaseSpell = phaseSpell;
            test.timer = 0;

            player->AddAura(test.phaseSpell, player);
            player->AddAura(test.testSpell, player);

            uint32 creaturesCount = SummonCreaturesForTest(testType, player);

            test.creaturesCount = creaturesCount;

            return true;
        }

        bool CompletePlayerTest(uint64 playerGuid, bool passed)
        {
            TestPlayer* testPlayer = GetPlayerTest(playerGuid);
            if (!testPlayer)
                return false;

            HandleRemovePlayerTest(testPlayer);

            MarkTestAsCompleted(testPlayer);

            if (Player* player = sObjectAccessor->FindPlayer(playerGuid))
            {
                bool canPassTest = CanPassTest(playerGuid);
                if (canPassTest)
                {
                    player->SetPower(POWER_ALTERNATE_POWER, 0);
                    player->AddAura(SPELL_PURIFIED_IMMUNE, player);
                }
            }

            return true;
        }

        void TestCreatureIsKilled(uint64 playerGuid)
        {
            TestPlayer* testPlayer = GetPlayerTest(playerGuid);
            if (!testPlayer)
                return;

            testPlayer->DecreaseCreaturesCount();

            if (testPlayer->IsAllCreaturesKilled())
            {
                CompletePlayerTest(playerGuid, true);
            }
        }

        bool HasPlayerTest(uint64 playerGuid) const
        {
            return m_testPlayers.find(playerGuid) != m_testPlayers.end();
        }

        uint32 GetPlayerTestsCount() const
        {
            return m_testPlayers.size();
        }

        void RemoveAllPlayerTests()
        {
            for (std::map<uint64, TestPlayer>::iterator itr = m_testPlayers.begin(); itr != m_testPlayers.end(); ++itr)
            {
                MarkTestAsExpired(&itr->second);
            }

            RemoveMarkedTests();

            m_testPlayers.clear();
        }

        void UpdateTests(const uint32 diff)
        {
            if (m_UpdateTestsTimer <= diff)
            {
                m_UpdateTestsTimer = TIMER_TESTS_UPDATE;

                for (std::map<uint64, TestPlayer>::iterator itr = m_testPlayers.begin(); itr != m_testPlayers.end(); ++itr)
                {
                    itr->second.timer += diff;

                    if (itr->second.timer >= TIMER_TESTS_DURATION)
                    {
                        MarkTestAsExpired(&itr->second);
                    }
                }

                RemoveMarkedTests();

            }
            else
            {
                m_UpdateTestsTimer -= diff;
            }
        }

    private:

        uint32 GetFreePhaseSpell()
        {
            std::set<uint32> usedSpells;
            for (std::map<uint64, TestPlayer>::const_iterator itr = m_testPlayers.begin(); itr != m_testPlayers.end(); ++itr)
            {
                usedSpells.insert(itr->second.phaseSpell);
            }

            for (uint8 i = 0; i < MAX_PHASE_SPELLS; ++i)
            {
                uint32 spellId = phaseSpells[i];

                if (usedSpells.find(spellId) == usedSpells.end())
                    return spellId;
            }

            return 0;
        }

        PlayerTestTypes GetTestTypeForPlayerRole(Roles role)
        {
            switch (role)
            {
                case ROLES_DPS:
                case ROLES_RANGED_DPS:
                    return TEST_SERENITY;
                case ROLES_HEALER:
                    return TEST_RELIANCE;
                case ROLES_TANK:
                    return TEST_CONFIDENCE;
            }

            return TEST_NONE;
        }

        uint32 SummonCreaturesForTest(PlayerTestTypes testType, Player* player)
        {
            switch (testType)
            {
                case TEST_SERENITY:
                    player->CastSpell(manifestationPos.GetPositionX(), manifestationPos.GetPositionY(), manifestationPos.GetPositionZ(), SPELL_MANIFESTATION_OF_CORRUPTION_SUMMON, true);
                    for (uint8 i = 0; i < MAX_ESSENSES; ++i)
                    {
                        player->CastSpell(essenseOfCorruptionPos[i].GetPositionX(), essenseOfCorruptionPos[i].GetPositionY(), essenseOfCorruptionPos[i].GetPositionZ(), SPELL_ESSENSE_OF_CORRUPTION_SUMMON, true);
                    }
                    return 1 + MAX_ESSENSES;
                case TEST_RELIANCE:
                    player->CastSpell(manifestationPos.GetPositionX(), manifestationPos.GetPositionY(), manifestationPos.GetPositionZ(), SPELL_GREATER_CORRUPTION_SUMMON, true);
                    return 1;
                case TEST_CONFIDENCE:
                    player->CastSpell(manifestationPos.GetPositionX(), manifestationPos.GetPositionY(), manifestationPos.GetPositionZ(), SPELL_TITANIC_CORRUPTION_SUMMON, true);
                    return 1;
            }

            return 0;
        }

        void HandleRemovePlayerTest(TestPlayer* testPlayer)
        {
            Player* player = sObjectAccessor->FindPlayer(testPlayer->playerGuid);
            if (!player)
                return;

            player->RemoveAura(testPlayer->testSpell);
            player->RemoveAura(testPlayer->phaseSpell);
        }

        bool CanPassTest(uint64 playerGuid)
        {
            TestPlayer* testPlayer = GetPlayerTest(playerGuid);
            if (!testPlayer)
                return false;

            if (testPlayer->testType == TEST_SERENITY)
            {
                if (!testPlayer->IsAllCreaturesKilled())
                    return false;
            }

            return true;
        }

        TestPlayer* GetPlayerTest(uint64 playerGuid)
        {
            std::map<uint64, TestPlayer>::iterator itr = m_testPlayers.find(playerGuid);
            if (itr == m_testPlayers.end())
                return NULL;

            return &itr->second;
        }

        void MarkTestAsCompleted(TestPlayer* testPlayer)
        {
            m_TestsToRemove.insert(testPlayer->playerGuid);
        }

        void MarkTestAsExpired(TestPlayer* testPlayer)
        {
            m_TestsToRemove.insert(testPlayer->playerGuid);
        }

        void RemoveMarkedTests()
        {
            if (m_TestsToRemove.empty())
                return;

            for (std::set<uint64>::const_iterator itr = m_TestsToRemove.begin(); itr != m_TestsToRemove.end(); ++itr)
            {
                uint64 playerGuid = (*itr);

                std::map<uint64 /*playerGuid*/, TestPlayer>::iterator itr_test = m_testPlayers.find(playerGuid);
                if (itr_test != m_testPlayers.end())
                {
                    m_testPlayers.erase(itr_test);
                }                
            }

            m_TestsToRemove.clear();
        }

    private:

        std::map<uint64 /*playerGuid*/, TestPlayer> m_testPlayers;
        std::set<uint64> m_TestsToRemove;
        uint32 m_UpdateTestsTimer;
};

class boss_norushen_amalgam_of_corruption : public CreatureScript
{
    public:
        boss_norushen_amalgam_of_corruption() : CreatureScript("boss_norushen_amalgam_of_corruption") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_norushen_amalgam_of_corruptionAI(creature);
        }

        struct boss_norushen_amalgam_of_corruptionAI : public BossAI
        {
            boss_norushen_amalgam_of_corruptionAI(Creature* creature) : BossAI(creature, DATA_NORUSHEN)
            {
                ApplyAllImmunities(true);

                me->setActive(true);
                me->ModifyAuraState(AURA_STATE_UNKNOWN22, true);

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 11.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 11.0f);

                memset(purifyingLights, 0, sizeof(purifyingLights));
                frayedCount = 0;
                isDespawned = false;
            }

            void Reset()
            {
                _Reset();

                me->SetReactState(REACT_DEFENSIVE);

                memset(purifyingLights, 0, sizeof(purifyingLights));
                frayedCount = 0;
                isDespawned = false;

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTION);
            }

            void AttackStart(Unit* who)
            {
                if (!who)
                    return;

                if (me->Attack(who, true))
                    DoStartNoMovement(who);
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_BERSERK, 7 * MINUTE * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_UNCHECKED_CORRUPTION, 5000);
                events.ScheduleEvent(EVENT_UNLEASHED_ANGER, 11000);
                events.ScheduleEvent(EVENT_SELF_DOUBT, urand(15000, 30000));
                events.ScheduleEvent(EVENT_BLIND_HATRED, 25000);
                events.ScheduleEvent(EVENT_PURIFYING_LIGHT, urand(1000, 3000));

                me->AddAura(SPELL_ICY_FEAR_PERIODIC, me);

                instance->DoAddAuraOnPlayers(SPELL_CORRUPTION);

                instance->SetBossState(DATA_NORUSHEN, IN_PROGRESS);
                DoZoneInCombat();
            }

            void EnterEvadeMode()
            {
                if (isDespawned)
                    return;

                isDespawned = true;

                if (Creature* norushen = sObjectAccessor->GetCreature(*me, instance->GetData64(DATA_NORUSHEN)))
                {
                    norushen->AI()->DoAction(ACTION_EVADE_COMBAT);
                }

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTION);

                instance->SetBossState(DATA_NORUSHEN, NOT_STARTED);

                ClearCreatures();
                summons.DespawnAll();
                events.Reset();

                me->DespawnOrUnsummon(1000);

                //BossAI::EnterEvadeMode();
            }

            void SetData(uint32 type, uint32 data)
            {
                if (type == DATA_PURIFYING_LIGHT_ID)
                {
                    if (data < MAX_PURIFYING_LIGHT_POINTS)
                    {
                        purifyingLights[data] = 0;
                    }
                }
            }

            void SetGUID(uint64 guid, int32 type)
            {
                switch (type)
                {
                    case DATA_PLAYER_TEST_ADD:
                    {
                        if (Player* player = sObjectAccessor->FindPlayer(guid))
                        {
                            Roles role = (Roles)player->GetRoleConsideringSpe(player->GetSpecializationId(player->GetActiveSpec()));
                            m_TestController.AddPlayerTest(guid, role);
                        }
                        break;
                    }
                    case DATA_PLAYER_TEST_REMOVE:
                        m_TestController.CompletePlayerTest(guid, false);
                        break;
                    case DATA_PLAYER_TEST_DIED:
                        m_TestController.CompletePlayerTest(guid, false);
                        SpawnCreaturesAfterFailedTest();
                        break;
                    case DATA_PLAYER_TEST_PASSED:
                        m_TestController.CompletePlayerTest(guid, true);
                        break;
                    case DATA_PLAYER_TEST_KILLED_CREATURE:
                        m_TestController.TestCreatureIsKilled(guid);
                        break;
                }
            }

            void JustDied(Unit* who)
            {
                ClearCreatures();

                _JustDied();

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTION);

                if (Creature* norushen = sObjectAccessor->GetCreature(*me, instance->GetData64(DATA_NORUSHEN)))
                {
                    norushen->AI()->DoAction(ACTION_COMPLETE_ENCOUNTER);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                // prevent entering evade mode while all players in other phases
                if (!IsAllPlayersTesting())
                {
                    if (!UpdateVictim())
                        return;
                }

                events.Update(diff);

                m_TestController.UpdateTests(diff);

                UpdateFrayed();

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BERSERK:
                        {
                            instance->DoCastSpellOnPlayers(SPELL_QUARANTINE_SAFETY_MEASURES_1);
                            Map::PlayerList const& players = instance->instance->GetPlayers();
                            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            {
                                if (Player* player = itr->getSource())
                                    me->Kill(player);
                            }
                            break;
                        }
                        case EVENT_UNCHECKED_CORRUPTION:
                        {
                            Unit* victim = me->getVictim();
                            if (!victim || !me->IsWithinMeleeRange(me->getVictim()))
                            {
                                DoCastAOE(SPELL_UNCHECKED_CORRUPTION);
                            }

                            events.ScheduleEvent(EVENT_UNCHECKED_CORRUPTION, 2000);
                            break;
                        }
                        case EVENT_UNLEASHED_ANGER:
                            DoCastVictim(SPELL_UNLEASHED_ANGER);
                            events.ScheduleEvent(EVENT_UNLEASHED_ANGER, 11000);
                            break;
                        case EVENT_SELF_DOUBT:
                            DoCastVictim(SPELL_SELF_DOUBT);
                            events.ScheduleEvent(EVENT_SELF_DOUBT, urand(30000, 40000));
                            break;
                        case EVENT_BLIND_HATRED:
                            CreateBlindHatred();
                            events.ScheduleEvent(EVENT_BLIND_HATRED, 60000);
                            break;
                        case EVENT_PURIFYING_LIGHT:
                            SpawnPurifyingLights();
                            events.ScheduleEvent(EVENT_PURIFYING_LIGHT, urand(15000, 30000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            uint64 purifyingLights[5];

            TestController m_TestController;

            uint32 frayedCount;

            bool isDespawned = false;

        private:

            bool IsAllPlayersTesting()
            {
                if (m_TestController.GetPlayerTestsCount() == 0)
                    return false;
                
                return m_TestController.GetPlayerTestsCount() >= me->getThreatManager().getThreatList().size();
            }

            void CreateBlindHatred()
            {
                if (Creature* pTrigger = me->SummonCreature(NPC_BLIND_HATRED_1, *me, TEMPSUMMON_TIMED_DESPAWN, 30000))
                {
                    pTrigger->AI()->DoAction(ACTION_BLIND_HATRED_START);
                }
            }

            void SpawnPurifyingLights()
            {
                for (uint8 i = 0; i < MAX_PURIFYING_LIGHT_POINTS; ++i)
                {
                    if (purifyingLights[i])
                        continue;

                    if (Creature* pLight = me->SummonCreature(NPC_PURIFYING_LIGHT, purifyingLightPos[i]))
                    {
                        purifyingLights[i] = pLight->GetGUID();
                        pLight->AI()->SetData(DATA_PURIFYING_LIGHT_ID, i);
                        pLight->AI()->DoAction(ACTION_SPAWN_LIGHT);
                    }
                }
            }

            void SpawnCreaturesAfterFailedTest()
            {
                // 1 manifestation
                DoCast(me, SPELL_UNLEASH_CORRUPTION_MISSILE_2, true);

                // and 4 essenses
                for (uint8 i = 0; i < MAX_ESSENSES; ++i)
                    DoCast(me, SPELL_UNLEASH_CORRUPTION_MISSILE_1, true);
            }

            void UpdateFrayed()
            {
                if (frayedCount == 0 && me->GetHealthPct() <= 50.f)
                {
                    frayedCount = 1;

                    DoCast(me, SPELL_UNLEASH_CORRUPTION_SUMMON_1, true);

                    me->AddAura(SPELL_FRAYED, me);
                }
                else if (frayedCount == 1 && me->GetHealthPct() <= 40.f)
                {
                    frayedCount = 2;

                    DoCast(me, SPELL_UNLEASH_CORRUPTION_SUMMON_1, true);
                }
                else if (frayedCount == 2 && me->GetHealthPct() <= 30.f)
                {
                    frayedCount = 3;

                    DoCast(me, SPELL_UNLEASH_CORRUPTION_SUMMON_1, true);
                }
                else if (frayedCount == 3 && me->GetHealthPct() <= 20.f)
                {
                    frayedCount = 4;

                    DoCast(me, SPELL_UNLEASH_CORRUPTION_SUMMON_1, true);
                }
                else if (frayedCount == 4 && me->GetHealthPct() <= 10.f)
                {
                    frayedCount = 5;

                    DoCast(me, SPELL_UNLEASH_CORRUPTION_SUMMON_1, true);
                }
            }

            void ClearCreatures()
            {
                for (uint8 i = 0; i < MAX_PURIFYING_LIGHT_POINTS; ++i)
                {
                    if (!purifyingLights[i])
                        continue;

                    if (Creature* pLight = sObjectAccessor->GetCreature(*me, purifyingLights[i]))
                    {
                        pLight->RemoveAllAreasTrigger();
                    }
                }

                DespawnCreaturesInArea(NPC_BLIND_HATRED_2, me);
                DespawnCreaturesInArea(NPC_MANIFESTATION_OF_CORRUPTION_1, me);
                DespawnCreaturesInArea(NPC_ESSENCE_OF_CORRUPTION_1, me);
            }
        };
};


class npc_norushen : public CreatureScript
{
    public:
        npc_norushen() : CreatureScript("npc_norushen") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushenAI(creature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                if (pInstance->GetBossState(DATA_NORUSHEN) == IN_PROGRESS ||
                    pInstance->GetBossState(DATA_NORUSHEN) == DONE)
                    return true;

                if (pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION))
                    return true;

                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We are ready, Keeper.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->PlayerTalkClass->SendGossipMenu(71967, creature->GetGUID());

            }
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF+1:
                    creature->AI()->DoAction(ACTION_START_BATTLE);
                    break;
            }

            return true;
        }

        struct npc_norushenAI : public ScriptedAI
        {
            npc_norushenAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);

                pInstance = me->GetInstanceScript();
                amalgamGUID = 0;
            }

            void Reset()
            {
                if (pInstance)
                {
                    if (pInstance->GetBossState(DATA_NORUSHEN) != DONE)
                    {
                        ResetEncounter();
                    }
                    else
                    {
                        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        TeleportToExit();
                    }

                    ActivateContainments(false);
                }
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_START_BATTLE)
                {
                    if (pInstance->GetBossState(DATA_NORUSHEN) == IN_PROGRESS ||
                        pInstance->GetBossState(DATA_NORUSHEN) == DONE)
                        return;

                    if (pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION))
                        return;

                    Talk(SAY_EVENT_6);

                    events.ScheduleEvent(EVENT_NORUSHEN_TALK_1, 6000);
                }
                else if (action == ACTION_EVADE_COMBAT)
                {
                    ResetEncounter();
                }
                else if (action == ACTION_COMPLETE_ENCOUNTER)
                {
                    CompleteEncounter();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_NORUSHEN_TALK_1:
                            Talk(SAY_EVENT_7);
                            events.ScheduleEvent(EVENT_NORUSHEN_TALK_2, 7000);
                            break;
                        case EVENT_NORUSHEN_TALK_2:
                            Talk(SAY_AGGRO);

                            ActivateContainments(true);

                            if (Creature* pAmalgam = SpawnAmalgam())
                            {
                                events.ScheduleEvent(EVENT_ACTIVATE_AMALGAM, 10000);
                                events.ScheduleEvent(EVENT_TELEPORT_TO_EXIT, 10000);

                                DoCast(pAmalgam, SPELL_EXTRACT_CORRUPTION);
                            }

                            break;
                        case EVENT_ACTIVATE_AMALGAM:
                            ActivateAmalgam();
                            break;
                        case EVENT_TELEPORT_TO_EXIT:
                            TeleportToExit();
                            break;
                        default:
                            break;
                    }
                }
            }

        private:

            InstanceScript* pInstance;
            uint64 amalgamGUID;

        private:

            void ResetEncounter()
            {
                Talk(SAY_WIPE);

                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                if (pInstance)
                {
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTION);
                }

                ActivateContainments(false);

                TeleportToStart();
            }

            void CompleteEncounter()
            {
                Talk(SAY_WIN);

                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                if (pInstance)
                {
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CORRUPTION);

                    pInstance->SetBossState(DATA_NORUSHEN, DONE);
                }

                ActivateContainments(false);
            }

            Creature* SpawnAmalgam()
            {
                Creature* pAmalgam = me->SummonCreature(NPC_AMALGAM_OF_CORRUPTION, centerPos);
                if (!pAmalgam)
                    return NULL;

                amalgamGUID = pAmalgam->GetGUID();
                pAmalgam->SetDisplayId(11686);
                pAmalgam->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);

                return pAmalgam;
            }

            void ActivateAmalgam()
            {
                if (Creature* pAmalgam = sObjectAccessor->GetCreature(*me, amalgamGUID))
                {
                    pAmalgam->RestoreDisplayId();
                    pAmalgam->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                }
            }

            void TeleportToExit()
            {
                me->NearTeleportTo(norushenExitPos);
            }

            void TeleportToStart()
            {
                me->NearTeleportTo(norushenStartPos);
            }

            void ActivateContainments(bool apply)
            {
                if (!pInstance)
                    return;

                for (uint8 i = 0; i < MAX_CONTAINMENTS; ++i)
                {
                    if (GameObject* pContainment = pInstance->instance->GetGameObject(pInstance->GetData64(containmentData[i])))
                    {
                        pContainment->SetGoState(apply ? GO_STATE_READY : GO_STATE_ACTIVE);
                        pContainment->EnableCollision(apply);
                    }
                }
            }
        };
};

class npc_norushen_lorewalker_cho_2 : public CreatureScript
{
    public:
        npc_norushen_lorewalker_cho_2() : CreatureScript("npc_norushen_lorewalker_cho_2") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_lorewalker_cho_2AI(creature);
        }

        struct npc_norushen_lorewalker_cho_2AI : public ScriptedAI
        {
            npc_norushen_lorewalker_cho_2AI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
            }

            void DoAction(const int32 action)
            {
                
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

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
        };
};


class npc_norushen_blind_hatred_1 : public CreatureScript
{
    public:
        npc_norushen_blind_hatred_1() : CreatureScript("npc_norushen_blind_hatred_1") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_blind_hatred_1AI(creature);
        }

        struct npc_norushen_blind_hatred_1AI : public Scripted_NoMovementAI
        {
            npc_norushen_blind_hatred_1AI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();

                syncPositionTimer = 1000;
                targetTriggerGuid = 0;
            }

            void Reset()
            {
                syncPositionTimer = 1000;
                targetTriggerGuid = 0;
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_BLIND_HATRED_START)
                {
                    StartBlindHatred();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                UpdateSyncPosition(diff);
            }

        private:

            InstanceScript* pInstance;
            uint32 syncPositionTimer;
            uint64 targetTriggerGuid;

        private:

            void StartBlindHatred()
            {
                uint8 i = urand(0, MAX_BLIND_HATRED_POINTS - 1);
                if (Creature* pTrigger = me->SummonCreature(NPC_BLIND_HATRED_2, blindHatredPos[i], TEMPSUMMON_TIMED_DESPAWN, 30000))
                {
                    targetTriggerGuid = pTrigger->GetGUID();

                    pTrigger->AI()->SetData(DATA_BLIND_HATRED_ID, i);
                    pTrigger->AI()->DoAction(ACTION_BLIND_HATRED_START);
                    
                    // aura should be triggered by 145226
                    //me->AddAura(SPELL_BLIND_HATRED_PERIODIC, me);

                    DoCast(pTrigger, SPELL_BLIND_HATRED);
                }
            }

            void UpdateSyncPosition(const uint32 diff)
            {
                if (syncPositionTimer <= diff)
                {
                    syncPositionTimer = 1000;

                    if (Creature* pTrigger = sObjectAccessor->GetCreature(*me, targetTriggerGuid))
                    {
                        float ori = me->GetAngle(pTrigger);

                        me->SetOrientation(ori);
                        me->SetFacingTo(ori);
                    }
                }
                else
                {
                    syncPositionTimer -= diff;
                }
            }
        };
};

class npc_norushen_blind_hatred_2 : public CreatureScript
{
    public:
        npc_norushen_blind_hatred_2() : CreatureScript("npc_norushen_blind_hatred_2") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_blind_hatred_2AI(creature);
        }

        struct npc_norushen_blind_hatred_2AI : public ScriptedAI
        {
            npc_norushen_blind_hatred_2AI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->SetSpeed(MOVE_RUN, 3.0f);

                pInstance = me->GetInstanceScript();
                m_Id = 0;
            }

            void Reset()
            {
                
            }

            void SetData(uint32 type, uint32 id)
            {
                if (type == DATA_BLIND_HATRED_ID)
                {
                    if (id < MAX_BLIND_HATRED_POINTS)
                    {
                        m_Id = id;
                    }
                }
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_BLIND_HATRED_START)
                {
                    StartBlindHatred();
                }
            }

        private:

            InstanceScript* pInstance;
            uint32 m_Id;

        private:

            void StartBlindHatred()
            {
                Movement::MoveSplineInit init(*me);

                for (uint8 i = m_Id; i < MAX_BLIND_HATRED_POINTS; ++i)
                {
                    G3D::Vector3 point;
                    point.x = blindHatredPos[i].GetPositionX();
                    point.y = blindHatredPos[i].GetPositionY();
                    point.z = blindHatredPos[i].GetPositionZ();
                    init.Path().push_back(point);
                }

                for (uint8 i = 0; i < m_Id; ++i)
                {
                    G3D::Vector3 point;
                    point.x = blindHatredPos[i].GetPositionX();
                    point.y = blindHatredPos[i].GetPositionY();
                    point.z = blindHatredPos[i].GetPositionZ();
                    init.Path().push_back(point);
                }

                init.SetCyclic();
                init.SetWalk(false);
                init.SetVelocity(3.0f);
                init.Launch();
            }
        };
};

class npc_norushen_purifying_light : public CreatureScript
{
    public:
        npc_norushen_purifying_light() : CreatureScript("npc_norushen_purifying_light") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_purifying_lightAI(creature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                if (Creature* pAmalgam = pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION)))
                {
                    // set purifying light used
                    pAmalgam->AI()->SetData(DATA_PURIFYING_LIGHT_ID, creature->AI()->GetData(DATA_PURIFYING_LIGHT_ID));

                    pAmalgam->AI()->SetGUID(player->GetGUID(), DATA_PLAYER_TEST_ADD);
                }
            }

            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            creature->RemoveAllAreasTrigger();
            creature->DespawnOrUnsummon();

            return true;
        }

        struct npc_norushen_purifying_lightAI : public Scripted_NoMovementAI
        {
            npc_norushen_purifying_lightAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                pInstance = me->GetInstanceScript();
                m_Id = 0;
            }

            void Reset()
            {
                
            }

            void SetData(uint32 type, uint32 data)
            {
                if (type == DATA_PURIFYING_LIGHT_ID)
                {
                    if (data < MAX_PURIFYING_LIGHT_POINTS)
                    {
                        m_Id = data;
                    }
                }
            }

            uint32 GetData(uint32 type)
            {
                if (type == DATA_PURIFYING_LIGHT_ID)
                {
                    return m_Id;
                }

                return 0;
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_SPAWN_LIGHT)
                {
                    InitPurifyingLight();
                }
            }

        private:

            InstanceScript* pInstance;
            uint32 m_Id;

        private:

            void InitPurifyingLight()
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_CLIENT_CANNOT_TARGET);
                me->SetDisplayId(TRIGGER_DISPLAY_ID);

                DoCast(me, SPELL_SPAWN_VICTORY_ORB_5, true);
            }
        };
};

class npc_norushen_manifestation_of_corruption_1 : public CreatureScript
{
    public:
        npc_norushen_manifestation_of_corruption_1() : CreatureScript("npc_norushen_manifestation_of_corruption_1") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_manifestation_of_corruption_1AI(creature);
        }

        struct npc_norushen_manifestation_of_corruption_1AI : public ScriptedAI
        {
            npc_norushen_manifestation_of_corruption_1AI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                
            }

            void IsSummonedBy(Unit* owher)
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (pInstance->GetBossState(DATA_NORUSHEN) == IN_PROGRESS)
                    {
                        DoZoneInCombat();
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MOVE, 3000);
            }

            void DamageTaken(Unit* who, uint32& damage)
            {
                if (Creature* pAmalgam = GetAmalgam())
                {
                    me->DealDamage(pAmalgam, pAmalgam->GetHealth() > damage ? damage : 0);
                }
            }

            void EnterEvadeMode()
            {
                me->DespawnOrUnsummon(1000);
            }

            void JustDied(Unit* who)
            {
                DoCastAOE(SPELL_CLEANSE_40, true);

                if (Creature* pAmalgam = GetAmalgam())
                {
                    pAmalgam->AI()->SetGUID(who->GetGUID(), DATA_PLAYER_TEST_KILLED_CREATURE);
                    pAmalgam->CastSpell(pAmalgam, SPELL_UNLEASH_CORRUPTION_MISSILE_2, true);
                }

                me->DespawnOrUnsummon(2000);
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
                        case EVENT_MOVE:
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_TEAR_REALITY, urand(1000, 3000));
                            break;
                        case EVENT_TEAR_REALITY:
                            if (Unit* target = me->getVictim())
                            {
                                if (Creature* pTrigger = me->SummonCreature(WORLD_TRIGGER, *target, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                {
                                    DoCast(pTrigger, SPELL_TEAR_REALITY);
                                }
                            }
                            events.ScheduleEvent(EVENT_TEAR_REALITY, urand(3000, 5000));
                            break;
                    }
                }
            }

        private:

            Creature* GetAmalgam()
            {
                if (!pInstance)
                    return NULL;

                return pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
            }

        private:

            InstanceScript* pInstance;
        };
};

class npc_norushen_essense_of_corruption_1 : public CreatureScript
{
    public:
        npc_norushen_essense_of_corruption_1() : CreatureScript("npc_norushen_essense_of_corruption_1") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_essense_of_corruption_1AI(creature);
        }

        struct npc_norushen_essense_of_corruption_1AI : public ScriptedAI
        {
            npc_norushen_essense_of_corruption_1AI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();

                expelCorriptionTargetGuid = 0;
            }

            void Reset()
            {
                DoCast(me, SPELL_ESSENSE_OF_CORRUPTION, true);
            }

            void IsSummonedBy(Unit* owher)
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (pInstance->GetBossState(DATA_NORUSHEN) == IN_PROGRESS)
                    {
                        DoZoneInCombat();
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MOVE, 3000);
            }

            void DamageTaken(Unit* who, uint32& damage)
            {
                if (Creature* pAmalgam = GetAmalgam())
                {
                    me->DealDamage(pAmalgam, pAmalgam->GetHealth() > damage ? damage : 0);
                }
            }

            void EnterEvadeMode()
            {
                me->DespawnOrUnsummon(1000);
            }

            void JustDied(Unit* who)
            {
                DoCastAOE(SPELL_CLEANSE_15, true);

                if (Creature* pAmalgam = GetAmalgam())
                {
                    pAmalgam->AI()->SetGUID(who->GetGUID(), DATA_PLAYER_TEST_KILLED_CREATURE);
                    pAmalgam->CastSpell(pAmalgam, SPELL_UNLEASH_CORRUPTION_MISSILE_1, true);
                }

                me->DespawnOrUnsummon(2000);
            }

            uint64 GetGUID(int32 id) override
            {
                if (id == DATA_EXPEL_CORRUPTION_TARGET)
                {
                    return expelCorriptionTargetGuid;
                }

                return 0;
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
                        case EVENT_MOVE:
                            events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, urand(1000, 3000));
                            break;
                        case EVENT_EXPEL_CORRUPTION:
                        {
                            Position pos;
                            me->GetNearPosition(pos, frand(40.f, 50.f), 0.0f);
                            if (Creature* pTrigger = me->SummonCreature(WORLD_TRIGGER, pos, TEMPSUMMON_TIMED_DESPAWN, 5000))
                            {
                                expelCorriptionTargetGuid = pTrigger->GetGUID();
                                DoCast(pTrigger, SPELL_EXPEL_CORRUPTION_1);
                            }

                            events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, urand(6000, 9000));
                            break;
                        }
                    }
                }
            }

        private:

            Creature* GetAmalgam()
            {
                if (!pInstance)
                    return NULL;

                return pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
            }

        private:

            InstanceScript* pInstance;
            uint64 expelCorriptionTargetGuid;
        };
};

class npc_norushen_manifestation_of_corruption_2 : public CreatureScript
{
    public:
        npc_norushen_manifestation_of_corruption_2() : CreatureScript("npc_norushen_manifestation_of_corruption_2") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_manifestation_of_corruption_2AI(creature);
        }

        struct npc_norushen_manifestation_of_corruption_2AI : public ScriptedAI
        {
            npc_norushen_manifestation_of_corruption_2AI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MOVE, 3000);
            }

            void DamageTaken(Unit* who, uint32& damage)
            {
                if (Creature* pAmalgam = GetAmalgam())
                {
                    me->DealDamage(pAmalgam, pAmalgam->GetHealth() > damage ? damage : 0);
                }
            }

            void JustDied(Unit* who)
            {
                if (Creature* pAmalgam = GetAmalgam())
                {
                    pAmalgam->SummonCreature(NPC_RESIDUAL_CORRUPTION, *me);
                }
                
                me->DespawnOrUnsummon(2000);
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
                        case EVENT_MOVE:
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_BURST_OF_ANGER, urand(1000, 3000));
                            break;
                        case EVENT_BURST_OF_ANGER:
                            DoCastAOE(SPELL_BURST_OF_ANGER);
                            events.ScheduleEvent(EVENT_BURST_OF_ANGER, 2000);
                            break;
                    }
                }
            }

        private:

            Creature* GetAmalgam()
            {
                if (!pInstance)
                    return NULL;

                return pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
            }

        private:

            InstanceScript* pInstance;
        };
};

class npc_norushen_essense_of_corruption_2 : public CreatureScript
{
    public:
        npc_norushen_essense_of_corruption_2() : CreatureScript("npc_norushen_essense_of_corruption_2") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_essense_of_corruption_2AI(creature);
        }

        struct npc_norushen_essense_of_corruption_2AI : public ScriptedAI
        {
            npc_norushen_essense_of_corruption_2AI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MOVE, 3000);
            }

            void DamageTaken(Unit* who, uint32& damage)
            {
                if (Creature* pAmalgam = GetAmalgam())
                {
                    me->DealDamage(pAmalgam, pAmalgam->GetHealth() > damage ? damage : 0);
                }
            }

            void JustDied(Unit* who)
            {
                me->DespawnOrUnsummon(2000);
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
                        case EVENT_MOVE:
                            events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, urand(1000, 3000));
                            break;
                        case EVENT_EXPEL_CORRUPTION:
                        {
                            float o = frand(0.f, M_PI);
                            me->SetFacingTo(o);
                            me->SetOrientation(o);

                            Position pos;
                            me->GetNearPosition(pos, frand(10.f, 20.f), 0.0f);
                            if (Creature* pTrigger = me->SummonCreature(WORLD_TRIGGER, pos, TEMPSUMMON_TIMED_DESPAWN, 5000))
                            {
                                DoCast(pTrigger, SPELL_EXPEL_CORRUPTION_2);
                            }

                            events.ScheduleEvent(EVENT_EXPEL_CORRUPTION, urand(6000, 9000));
                            break;
                        }
                    }
                }
            }

        private:

            Creature* GetAmalgam()
            {
                if (!pInstance)
                    return NULL;

                return pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
            }

        private:

            InstanceScript* pInstance;
        };
};

class npc_norushen_greater_corruption : public CreatureScript
{
    public:
        npc_norushen_greater_corruption() : CreatureScript("npc_norushen_greater_corruption") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_greater_corruptionAI(creature);
        }

        struct npc_norushen_greater_corruptionAI : public Scripted_NoMovementAI
        {
            npc_norushen_greater_corruptionAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                
            }

            void IsSummonedBy(Unit* owher)
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (pInstance->GetBossState(DATA_NORUSHEN) == IN_PROGRESS)
                    {
                        DoZoneInCombat();
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MOVE, 3000);
            }

            void EnterEvadeMode()
            {
                me->DespawnOrUnsummon(1000);
            }

            void JustDied(Unit* who)
            {
                DoCastAOE(SPELL_CLEANSE_100, true);

                if (Creature* pAmalgam = GetAmalgam())
                {
                    pAmalgam->AI()->SetGUID(who->GetGUID(), DATA_PLAYER_TEST_KILLED_CREATURE);
                }

                me->DespawnOrUnsummon(2000);
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
                        case EVENT_MOVE:
                            events.ScheduleEvent(EVENT_DISHEARTENING_LAUGH, urand(5000, 12000));
                            events.ScheduleEvent(EVENT_LINGERING_CORRUPTION, urand(3000, 10000));
                            events.ScheduleEvent(EVENT_BOTTOMLESS_PIT, urand(2000, 10000));
                            break;
                        case EVENT_DISHEARTENING_LAUGH:
                            DoCastAOE(SPELL_DISHEARTENING_LAUGH);
                            events.ScheduleEvent(EVENT_DISHEARTENING_LAUGH, urand(12000, 13000));
                            break;
                        case EVENT_LINGERING_CORRUPTION:
                            DoCastAOE(SPELL_LINGERING_CORRUPTION);
                            events.ScheduleEvent(EVENT_DISHEARTENING_LAUGH, urand(15500, 17000));
                            break;
                        case EVENT_BOTTOMLESS_PIT:
                            // TODO: realize spell
                            events.ScheduleEvent(EVENT_BOTTOMLESS_PIT, urand(12000, 15000));
                            break;
                    }
                }
            }

        private:

            Creature* GetAmalgam()
            {
                if (!pInstance)
                    return NULL;

                return pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
            }

        private:

            InstanceScript* pInstance;
        };
};

class npc_norushen_titanic_corruption : public CreatureScript
{
    public:
        npc_norushen_titanic_corruption() : CreatureScript("npc_norushen_titanic_corruption") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_titanic_corruptionAI(creature);
        }

        struct npc_norushen_titanic_corruptionAI : public ScriptedAI
        {
            npc_norushen_titanic_corruptionAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetReactState(REACT_PASSIVE);

                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                
            }

            void IsSummonedBy(Unit* owher)
            {
                if (InstanceScript* pInstance = me->GetInstanceScript())
                {
                    if (pInstance->GetBossState(DATA_NORUSHEN) == IN_PROGRESS)
                    {
                        DoZoneInCombat();
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_MOVE, 3000);
            }

            void EnterEvadeMode()
            {
                me->DespawnOrUnsummon(1000);
            }

            void JustDied(Unit* who)
            {
                DoCastAOE(SPELL_CLEANSE_100, true);

                if (Creature* pAmalgam = GetAmalgam())
                {
                    pAmalgam->AI()->SetGUID(who->GetGUID(), DATA_PLAYER_TEST_KILLED_CREATURE);
                }

                me->DespawnOrUnsummon(2000);
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
                        case EVENT_MOVE:
                            me->SetReactState(REACT_AGGRESSIVE);
                            events.ScheduleEvent(EVENT_TITANIC_SMASH, urand(10000, 14000));
                            events.ScheduleEvent(EVENT_HURL_CORRUPTION, urand(12000, 20000));
                            events.ScheduleEvent(EVENT_PIERCING_CORRUPTION, urand(7000, 20000));
                            events.ScheduleEvent(EVENT_BURST_OF_CORRUPTION, urand(15000, 20000));
                            break;
                        case EVENT_TITANIC_SMASH:
                            if (Unit* target = me->getVictim())
                            {
                                if (Creature* pTrigger = me->SummonCreature(WORLD_TRIGGER, *target, TEMPSUMMON_TIMED_DESPAWN, 5000))
                                {
                                    DoCast(pTrigger, SPELL_TITANIC_SMASH);
                                }
                            }
                            events.ScheduleEvent(EVENT_TITANIC_SMASH, urand(14000, 17000));
                            break;
                        case EVENT_HURL_CORRUPTION:
                            DoCastVictim(SPELL_HURL_CORRUPTION);
                            events.ScheduleEvent(EVENT_HURL_CORRUPTION, urand(20000, 21000));
                            break;
                        case EVENT_PIERCING_CORRUPTION:
                            DoCastVictim(SPELL_PIERCING_CORRUPTION);
                            events.ScheduleEvent(EVENT_PIERCING_CORRUPTION, urand(14000, 17000));
                            break;
                        case EVENT_BURST_OF_CORRUPTION:
                            DoCastAOE(SPELL_BURST_OF_CORRUPTION);
                            events.ScheduleEvent(EVENT_BURST_OF_CORRUPTION, urand(20000, 30000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:

            Creature* GetAmalgam()
            {
                if (!pInstance)
                    return NULL;

                return pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
            }

        private:

            InstanceScript* pInstance;
        };
};

class npc_norushen_residual_corruption : public CreatureScript
{
    public:
        npc_norushen_residual_corruption() : CreatureScript("npc_norushen_residual_corruption") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_norushen_residual_corruptionAI(creature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (InstanceScript* pInstance = creature->GetInstanceScript())
            {
                if (Creature* pAmalgam = pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION)))
                {
                    pAmalgam->AI()->SetGUID(player->GetGUID(), DATA_PLAYER_TEST_ADD);
                }
            }

            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            creature->RemoveAllAreasTrigger();
            creature->DespawnOrUnsummon();

            return true;
        }

        struct npc_norushen_residual_corruptionAI : public Scripted_NoMovementAI
        {
            npc_norushen_residual_corruptionAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            
                corruptTimer = 1000;
                corruptDone = false;
            }

            void Reset()
            {
                
            }

            void IsSummonedBy(Unit* owner)
            {
                DoCast(me, SPELL_RESIDUAL_CORRUPTION_AURA, true);
            }

            void UpdateAI(const uint32 diff)
            {
                if (corruptTimer <= diff)
                {
                    corruptTimer = 1000;

                    TryCorrupt();
                }
                else
                {
                    corruptTimer -= diff;
                }
            }

        private:

            class PlayerForCorruptCheck
            {
                public:
                    PlayerForCorruptCheck(WorldObject const* obj) : _obj(obj) {}
                    bool operator()(Player* u)
                    {
                        if (u->GetPower(POWER_ALTERNATE_POWER) > 75)
                            return false;

                        if (!u->isAlive())
                            return false;

                        if (!_obj->IsWithinDistInMap(u, 3.0f))
                            return false;

                        return true;
                    }

                private:
                    WorldObject const* _obj;
            };

            Player* FindPlayerForCorrupt()
            {
                Player* player = NULL;
                PlayerForCorruptCheck check(me);
                JadeCore::PlayerSearcher<PlayerForCorruptCheck> searcher(me, player, check);
                me->VisitNearbyWorldObject(3.0f, searcher);
                return player;
            }

            void TryCorrupt()
            {
                if (corruptDone)
                    return;

                if (Player* player = FindPlayerForCorrupt())
                {
                    int32 bp0 = 25;
                    me->CastCustomSpell(player, SPELL_CORRUPT, &bp0, NULL, NULL, NULL, NULL, NULL, true);
                    
                    corruptDone = true;

                    me->DespawnOrUnsummon(100);
                }
            }

        private:

            InstanceScript* pInstance;
            uint32 corruptTimer;
            bool corruptDone;

        };
};

class spell_norushen_unleashed_anger : public SpellScriptLoader
{
    public:
        spell_norushen_unleashed_anger() : SpellScriptLoader("spell_norushen_unleashed_anger") { }

        class spell_norushen_unleashed_anger_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_norushen_unleashed_anger_AuraScript);

            void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                GetCaster()->CastSpell(GetUnitOwner(), SPELL_UNLEASHED_ANGER_DUMMY, true);
                GetCaster()->CastSpell(GetUnitOwner(), SPELL_UNLEASHED_ANGER_DMG, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_norushen_unleashed_anger_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_norushen_unleashed_anger_AuraScript();
        }
};

class spell_norushen_test_of_serenity : public SpellScriptLoader
{
    public:
        spell_norushen_test_of_serenity() : SpellScriptLoader("spell_norushen_test_of_serenity") { }

        class spell_norushen_test_of_serenity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_norushen_test_of_serenity_AuraScript);

            void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                InstanceScript* pInstance = GetUnitOwner()->GetInstanceScript();
                if (!pInstance)
                    return;

                Creature* pAmalgam = pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
                if (!pAmalgam)
                    return;

                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();

                if (removeMode == AURA_REMOVE_BY_DEATH)
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_DIED);
                }
                else if (removeMode == AURA_REMOVE_BY_EXPIRE)
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_PASSED);
                }
                else
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_REMOVE);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_norushen_test_of_serenity_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_norushen_test_of_serenity_AuraScript();
        }
};

class spell_norushen_test_of_reliance : public SpellScriptLoader
{
    public:
        spell_norushen_test_of_reliance() : SpellScriptLoader("spell_norushen_test_of_reliance") { }

        class spell_norushen_test_of_reliance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_norushen_test_of_reliance_AuraScript);

            void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                InstanceScript* pInstance = GetUnitOwner()->GetInstanceScript();
                if (!pInstance)
                    return;

                Creature* pAmalgam = pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
                if (!pAmalgam)
                    return;

                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();

                if (removeMode == AURA_REMOVE_BY_DEATH)
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_DIED);
                }
                else if (removeMode == AURA_REMOVE_BY_EXPIRE)
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_PASSED);
                }
                else
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_REMOVE);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_norushen_test_of_reliance_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_norushen_test_of_reliance_AuraScript();
        }
};

class spell_norushen_test_of_confidence : public SpellScriptLoader
{
    public:
        spell_norushen_test_of_confidence() : SpellScriptLoader("spell_norushen_test_of_confidence") { }

        class spell_norushen_test_of_confidence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_norushen_test_of_confidence_AuraScript);

            void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (!GetCaster() || !GetUnitOwner())
                    return;

                InstanceScript* pInstance = GetUnitOwner()->GetInstanceScript();
                if (!pInstance)
                    return;

                Creature* pAmalgam = pInstance->instance->GetCreature(pInstance->GetData64(DATA_AMALGAM_OF_CORRUPTION));
                if (!pAmalgam)
                    return;

                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();

                if (removeMode == AURA_REMOVE_BY_DEATH)
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_DIED);
                }
                else if (removeMode == AURA_REMOVE_BY_EXPIRE)
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_PASSED);
                }
                else
                {
                    pAmalgam->AI()->SetGUID(GetUnitOwner()->GetGUID(), DATA_PLAYER_TEST_REMOVE);
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_norushen_test_of_confidence_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_norushen_test_of_confidence_AuraScript();
        }
};

class spell_norushen_blind_hatred_periodic : public SpellScriptLoader
{
    public:
        spell_norushen_blind_hatred_periodic() : SpellScriptLoader("spell_norushen_blind_hatred_periodic") { }

        class spell_norushen_blind_hatred_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_norushen_blind_hatred_periodic_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                GetUnitOwner()->CastSpell(GetUnitOwner(), SPELL_BLIND_HATRED_DMG, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_norushen_blind_hatred_periodic_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_norushen_blind_hatred_periodic_AuraScript();
        }
};

class spell_norushen_blind_hatred_dmg : public SpellScriptLoader
{
    public:
        spell_norushen_blind_hatred_dmg() : SpellScriptLoader("spell_norushen_blind_hatred_dmg") { }

        class spell_norushen_blind_hatred_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_norushen_blind_hatred_dmg_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                Creature* pTrigger = GetCaster()->FindNearestCreature(NPC_BLIND_HATRED_2, 200.0f);
                if (!pTrigger)
                    return;

                targets.remove_if(BlindHatredCheck(GetCaster(), pTrigger));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_norushen_blind_hatred_dmg_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_norushen_blind_hatred_dmg_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            }

        private:

            class BlindHatredCheck
            {
            public:
                BlindHatredCheck(Unit* source, Unit* target) : _source(source), _target(target) {}

                bool operator()(WorldObject* unit) const
                {
                    return !unit->IsInBetween(_source, _target);
                }

            private:
                Unit* _source;
                Unit* _target;
            };
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_norushen_blind_hatred_dmg_SpellScript();
        }
};

class spell_norushen_corruption : public SpellScriptLoader
{
    public:
        spell_norushen_corruption() : SpellScriptLoader("spell_norushen_corruption") { }

        class spell_norushen_corruption_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_norushen_corruption_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if (!GetUnitOwner())
                    return;

                int32 power = GetUnitOwner()->GetPower(POWER_ALTERNATE_POWER);

                if (AuraEffect* aurEff = GetAura()->GetEffect(EFFECT_1))
                {
                    aurEff->SetAmount(-power);
                }

                bool hasOwnerBuff = GetUnitOwner()->HasAura(SPELL_PURIFIED);

                if (power > 0)
                {
                    if (hasOwnerBuff)
                    {
                        GetUnitOwner()->RemoveAura(SPELL_PURIFIED);
                    }
                }
                else
                {
                    if (!hasOwnerBuff)
                    {
                        GetUnitOwner()->AddAura(SPELL_PURIFIED, GetUnitOwner());
                    }
                }
            }

            void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (!GetUnitOwner())
                    return;

                GetUnitOwner()->RemoveAura(SPELL_PURIFIED);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_norushen_corruption_AuraScript::HandlePeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_norushen_corruption_AuraScript::HandleRemove, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_norushen_corruption_AuraScript();
        }
};

class spell_norushen_icy_fear_dmg : public SpellScriptLoader
{
    public:
        spell_norushen_icy_fear_dmg() : SpellScriptLoader("spell_norushen_icy_fear_dmg") { }

        class spell_norushen_icy_fear_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_norushen_icy_fear_dmg_SpellScript);

            void CalculateDamage(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                float multiplier = 1.0f + (100.0f - GetCaster()->GetHealthPct()) / 100.0f;

                SetHitDamage(GetHitDamage() * multiplier);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_norushen_icy_fear_dmg_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_norushen_icy_fear_dmg_SpellScript();
        }
};

class spell_area_norushen_expel_corruption : public SpellAreaTriggerScript
{
    public:
        spell_area_norushen_expel_corruption() : SpellAreaTriggerScript("spell_area_norushen_expel_corruption") {}

        void OnCreate(AreaTrigger* trigger) override
        {
            Unit* caster = ObjectAccessor::GetUnit(*trigger, trigger->GetCasterGUID());
            if (!caster)
                return;

            Creature* pCreature = caster->ToCreature();
            if (!pCreature)
                return;

            uint64 targetGuid = pCreature->AI()->GetGUID(DATA_EXPEL_CORRUPTION_TARGET);
            Unit* target = ObjectAccessor::GetUnit(*trigger, targetGuid);
            if (!target)
                return;

            trigger->Relocate(*pCreature);
            trigger->SetSource(*pCreature);

            trigger->SetDestination(*target);
            trigger->SetTrajectory(AreatriggerInterpolation::AREATRIGGER_INTERPOLATION_LINEAR);
        }

        bool OnAddTarget(AreaTrigger* trigger, Unit* target)
        {
            Unit* caster = trigger->GetCaster();
            if (!caster)
                return false;

            if (!target->isTargetableForAttack())
                return false;

            caster->AddAura(SPELL_EXPELLED_CORRUPTION_1, target);

            return true;
        }

        bool OnRemoveTarget(AreaTrigger* trigger, Unit* target)
        {
            target->RemoveAura(SPELL_EXPELLED_CORRUPTION_1);

            return true;
        }
};

void AddSC_boss_norushen()
{
    new boss_norushen_amalgam_of_corruption();          // 72276
    new npc_norushen();                                 // 71967
    //new npc_norushen_lorewalker_cho_2();                // 72872
    new npc_norushen_blind_hatred_1();                  // 72565
    new npc_norushen_blind_hatred_2();                  // 72595
    new npc_norushen_purifying_light();                 // 72065
    new npc_norushen_manifestation_of_corruption_1();   // 71977
    new npc_norushen_essense_of_corruption_1();         // 71976
    new npc_norushen_manifestation_of_corruption_2();   // 72264
    new npc_norushen_essense_of_corruption_2();         // 72263
    new npc_norushen_greater_corruption();              // 72001
    new npc_norushen_titanic_corruption();              // 72051
    new npc_norushen_residual_corruption();             // 72550

    new spell_norushen_unleashed_anger();               // 145214
    new spell_norushen_test_of_serenity();              // 144849
    new spell_norushen_test_of_reliance();              // 144850
    new spell_norushen_test_of_confidence();            // 144851
    new spell_norushen_blind_hatred_periodic();         // 145573
    new spell_norushen_blind_hatred_dmg();              // 145227
    new spell_norushen_corruption();                    // 144421
    new spell_norushen_icy_fear_dmg();                  // 145735

    new spell_area_norushen_expel_corruption();        // 144479
}
//////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "heart_of_fear.hpp"

enum eZorlokSpells
{
    SPELL_MANTID_WINGS          = 126316,
    SPELL_ATTENUATION           = 122440,
    SPELL_CONVERT               = 122740,
    SPELL_INHALE                = 122852,
    SPELL_EXHALE                = 122761,
    SPELL_EXHALE_DMG            = 122760,
    SPELL_PHEROMONES_OF_ZEAL    = 123812,   // Aura inflicting damages to players
    SPELL_PHEROMONES_CLOUD      = 123811,   // Creating Cloud of pheromones
    SPELL_FORCE_AND_VERVE       = 122713,
    SPELL_NOISE_CANCELLING      = 122706,
    SPELL_MISSILE_NOISE_CANC    = 122707,
    SPELL_SONG_OF_THE_EMPRESS   = 123791,
    SPELL_SONIC_RING_VISUAL     = 122334,
    SPELL_SONIC_RING_AURA       = 122336,
    SPELL_SONIC_PULSE_VISUAL    = 124668,
    SPELL_SONIC_PULSE_AURA      = 124673,
    SPELL_INHALE_PHEROMONES     = 124018,
    SPELL_REINFORCE             = 123833,
    SPELL_ZORLOK_BERSERK        = 120207,
    SPELL_MAGNETIC_PULSE        = 147379,    // Pull the players on the boss
    SPELL_ECHO_OF_ZORLOK        = 127496,
};

enum eZorlokEvent
{
    EVENT_INHALE                    = 1,
    EVENT_EXHALE                    = 2,
    EVENT_BERSERK                   = 3,
    EVENT_ATTENUATION               = 4,
    EVENT_SUMMON_RINGS              = 5,
    EVENT_SUMMON_RINGS1             = 6,
    EVENT_SUMMON_RINGS2             = 7,
    EVENT_PHEROMONES_CLOUD          = 8,
    EVENT_FORCE_AND_VERVE           = 9,
    EVENT_CAST_FANDV                = 10,
    EVENT_CONVERT                   = 11,
    EVENT_PULL_RAID                 = 12,
    EVENT_SONIC_PULSE               = 13,
    EVENT_SUMMON_LAST_ECHO          = 14,
};

enum eZorlokActions
{
    ACTION_SONIC_RING           = 2,
    ACTION_INHALE_PHEROMONES    = 3,
    ACTION_WIPE                 = 4,
    ACTION_SONIC_PULSE          = 5,
};

enum eZorlokTypes
{
    TYPE_EXHALE_TARGET  = 1,
    TYPE_PHASE_ZORLOK   = 2,
};

enum eZorlokAdds
{
    NPC_SONIC_RING              = 62689,
    NPC_SONIC_PULSE             = 63837,
    NPC_ECHO_OF_ATTENUATION     = 65173,
    NPC_ECHO_OF_FORCE_AND_VERVE = 65174,
};

enum eZorlokPhase
{
    PHASE_ZORLOK1               = 1,
    PHASE_ZORLOK2               = 4, // value '4' needed, DON'T CHANGE IT !!!
};

enum ePlatforms
{
    PLATFORM_ZORLOK_SW          = 1,    // Platform South-west (Force and verve)
    PLATFORM_ZORLOK_NE          = 2,    // Platform North-east (Attenuation)
    PLATFORM_ZORLOK_SE          = 3,    // Platform South-east (Convert)
};

enum eTalk
{
    TALK_AGGRO                  = 1,
    TALK_DEATH                  = 2,
    TALK_EVENT_IDLE_1A          = 3,    // 1st phase of trash mobs
    TALK_EVENT_IDLE_1B          = 4,
    TALK_EVENT_IDLE_1C          = 5,
    TALK_EVENT_IDLE_2           = 6,    // 2nd phase of trash mobs
    TALK_EVENT_IDLE_3           = 7,    // 3rd phase of trash mobs
    TALK_EVENT_IDLE_4           = 8,    // 4th phase of trash mobs
    TALK_EVENT_PHASE_1          = 9,
    TALK_EVENT_PHASE_2          = 10,
    TALK_EVENT_PHASE_3          = 11,
    TALK_EVENT_TRASH_A_COMBAT   = 12,
    TALK_EVENT_TRASH_A_DIES     = 13,
    TALK_EVENT_TRASH_B_COMBAT   = 14,
    TALK_EVENT_TRASH_B_DIES     = 15,
    TALK_EVENT_TRASH_C_COMBAT   = 16,
    TALK_EVENT_TRASH_C_DIES     = 17,
    TALK_SLAY_01                = 18,   // Killing a player
    TALK_SLAY_02                = 19,
    TALK_EXHALE                 = 20,
    TALK_INHALE                 = 21,
    TALK_CONVERT                = 22,
    TALK_PITCH                  = 23,   // Echoes of power
};

Position const zorlokPlatforms[3] =
{
    {-2317.21f,     300.67f,    409.90f,    0.0f},  // SW Platform
    {-2234.36f,     216.88f,    409.90f,    0.0f},  // NE Platform
    {-2315.77f,     218.20f,    409.90f,    0.0f},  // SE Platform
};

Position const zorlokReachPoints[3] =
{
    {-2317.21f,     300.67f,    420.0f,     0.0f},  // NE Platform
    {-2234.36f,     216.88f,    420.0f,     0.0f},  // SW Platform
    {-2315.77f,     218.20f,    420.0f,     0.0f},  // SE Platform
};

Position const oratiumCenter[2] =
{
    {-2274.80f,     259.19f,    420.0f,    0.318021f},
    {-2274.80f,     259.19f,    406.5f,    0.318021f}
};

// 212943 - Final Phase Door
Position const finalPhaseWalls1[3] =
{
    {-2299.195f, 282.5938f, 408.5445f, 2.383867f},
    {-2250.401f, 234.0122f, 408.5445f, 2.333440f},
    {-2299.63f,  233.3889f, 408.5445f, 0.7598741f}
};

// 212916 - Arena Walls
Position const finalPhaseWalls2[3] =
{
    {-2255.168f, 308.7326f, 406.0f,   0.7853968f},
    {-2240.0f,   294.0f,    406.0f,   0.7853968f},
    {-2225.753f, 280.1424f, 406.381f, 0.7853968f},
};

float const tabCenter[3] = {-2274.8f, 259.187f, 406.5f};

float const rangeAttenuation1[2][2] =
{
    -2256.0f, -2208.0f,
      190.0f,   240.0f
};

float const rangeAttenuation2[2][2] =
{
    // Coords to redone
    -2297.0f, -2250.0f,
      237.0f,   280.0f
};

// Zorlok - 62980
// Echo of Attenuation - 65173
// Echo of Force and Verve - 65174
class boss_zorlok : public CreatureScript
{
    public:
        boss_zorlok() : CreatureScript("boss_zorlok") { }

        struct boss_zorlokAI : public BossAI
        {
            boss_zorlokAI(Creature* creature) : BossAI(creature, DATA_ZORLOK)
            {
                pInstance = creature->GetInstanceScript();
            }

            // Global
            InstanceScript* pInstance;
            EventMap events;
            bool isActive;
            bool isFlying;
            std::list<uint8> platforms;
            uint8 numPlat;
            uint8 phase;
            uint8 hasTalk;
            uint32 platformToUse;
            uint32 actualPlatform;
            // Inhale - Exhale
            uint64 exhaleTarget;
            bool inhaleDone;
            // Attenuation
            uint8 sonicRingCount;
            uint8 sonicPulseCount;
            float ringOrientation;
            float xr, yr, zr, orientation;       // Starting coords for sonic ring wave
            float pulseOrientation;
            float xp, yp, zp, op;       // Starting coords for sonic pulse wave (Heroic mode)
            bool clocksideRings;
            // Echo
            bool isEcho;
            bool isAttEcho;
            bool isFaVEcho;
            bool hasSummonedLastEcho;

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_ZORLOK);
                // Make sure we can target zorlok
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                // Set Echo
                isAttEcho = me->GetEntry() == NPC_ECHO_OF_ATTENUATION;
                isFaVEcho = me->GetEntry() == NPC_ECHO_OF_FORCE_AND_VERVE;
                isEcho    = (isAttEcho || isFaVEcho);

                // Set fying
                if (!isEcho)
                {
                    DoCast(me, SPELL_MANTID_WINGS);
                    me->SetCanFly(true);
                    me->SetSpeed(MOVE_FLIGHT, 4.5f);
                    me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);
                    me->SetDisableGravity(true);
                    me->SetReactState(REACT_PASSIVE);
                    me->RemoveAllAreasTrigger();
                }
                else
                    me->AddAura(SPELL_ECHO_OF_ZORLOK, me);

                // Zor'lok isn't immediately active, but echoes are.
                isActive = isEcho;

                if (pInstance && !isEcho)
                {
                    EncounterState bossState = pInstance->GetBossState(DATA_ZORLOK);
                    if (bossState != DONE && bossState != NOT_STARTED)
                        pInstance->SetBossState(DATA_ZORLOK, NOT_STARTED);
                }
                numPlat             = 0;

                Creature* zorlok = GetZorlok();
                phase = isEcho && zorlok != nullptr ? zorlok->AI()->GetData(TYPE_PHASE_ZORLOK) : 0;
                platformToUse       = 0;
                hasTalk             = 0;
                actualPlatform      = 0;
                sonicRingCount      = 0;
                sonicPulseCount     = 0;
                xr = 0, yr = 0, zr = 0, orientation = 0;
                xp = 0, yp = 0, zp = 0, op = 0;
                clocksideRings      = true;
                inhaleDone          = false;
                exhaleTarget        = 0;
                pulseOrientation    = 0.0f;
                ringOrientation     = 0.0f;
                hasSummonedLastEcho = false;

                // Echo in phase 2 should not move
                if (isEcho && phase == 2)
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                platforms.clear();
                if (!isEcho)
                    RemoveWalls();

                // In heroic mode, the platforms are ordered, so we just need to increase numPlat and having it matching to ePlatforms values, which
                // are heroic ordered, to get the right platform.
                //
                // In normal mode, platforms are picked in random order, so we need to put them in a list, to remove them when they're used (in order
                // to avoid Zorlok to use them again).
                if (!IsHeroic() && !isEcho)
                {
                    platforms.push_back(PLATFORM_ZORLOK_SW);
                    platforms.push_back(PLATFORM_ZORLOK_NE);
                    platforms.push_back(PLATFORM_ZORLOK_SE);
                }
            }

            // --- Specific Functions ---
            void RemoveWalls()
            {
                std::list<GameObject*> arenaList;
                std::list<GameObject*> wallsList;

                GetGameObjectListWithEntryInGrid(arenaList, me, GOB_ARENA_WALLS, 200.0f); // TODO: adjuste searche range
                GetGameObjectListWithEntryInGrid(wallsList, me, GOB_FINAL_PHASE_WALLS, 200.0f); // TODO: adjuste searche range

                for (auto wall : arenaList)
                    me->RemoveGameObject(wall, true);

                for (auto wall : wallsList)
                    me->RemoveGameObject(wall, true);
            }

            void SetFlying()
            {
                isFlying = true;
                me->AttackStop();
                me->DeleteThreatList();
                me->getThreatManager().clearReferences();
                me->SetCanFly(true);
                me->SetReactState(REACT_PASSIVE);
                me->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                me->SetDisableGravity(true);
                me->SendMovementFlagUpdate();
                me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);
                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                if (isActive)
                {
                    if (phase == PHASE_ZORLOK1 && hasTalk < numPlat)
                    {
                        me->GetMotionMaster()->MovePoint(platformToUse, zorlokReachPoints[platformToUse - 1]);
                        me->MonsterTextEmote("Imperial Vizier Zor'lok is flying to one of hist platforms!", 0, true);
                        hasTalk = numPlat;
                    }
                    else
                        me->GetMotionMaster()->MovePoint(phase, oratiumCenter[0]);
                }
                else
                    me->GetMotionMaster()->MoveTargetedHome();
            }

            void SetLanding(uint8 dest)
            {
                me->SetCanFly(false);
                me->SetDisableGravity(false);
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);
                me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);
                me->SendMovementFlagUpdate();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                events.ScheduleEvent(EVENT_INHALE, 15000);

                if (platformToUse != actualPlatform)
                    actualPlatform = platformToUse;

                if (dest)
                {
                    switch (dest)
                    {
                        // Force & Verve platform
                        case PLATFORM_ZORLOK_SW:
                        {
                            events.ScheduleEvent(EVENT_FORCE_AND_VERVE, urand(10000, 20000));
                            break;
                        }
                        // Attenuation Platform
                        case PLATFORM_ZORLOK_NE:
                        {
                            events.ScheduleEvent(EVENT_ATTENUATION, urand(10000, 20000));
                            break;
                        }
                        // Convert Platform
                        case PLATFORM_ZORLOK_SE:
                        {
                            events.ScheduleEvent(EVENT_CONVERT, urand (10000, 20000));
                            break;
                        }
                        // Phase 2 - Center of the room
                        case PHASE_ZORLOK2:
                        {
                            Talk(TALK_EVENT_PHASE_2);
                            events.ScheduleEvent(ChooseAction(), urand(25000, 35000));
                            events.ScheduleEvent(EVENT_PULL_RAID, 2000);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            // Specific for echo
            Creature* GetZorlok()
            {
                if (Creature* Zorlok = pInstance->instance->GetCreature(pInstance->GetData64(NPC_ZORLOK)))
                    return Zorlok;

                return 0;
            }

            // Return an angle between 0.0f and 6.28f from a non corrected value
            float CorrectAngle(float angle)
            {
                if (angle > 2 * M_PI)
                    angle -= 2 * M_PI;
                else if (angle < 0.0f)
                    angle += 2 * M_PI;
                // angle = (angle > (2 * M_PI)) ? (angle - (2 * M_PI)) : ((angle < 0.0f) ? (angle + (2 * M_PI)) : angle);
                return angle;
            }

            void JustDied(Unit* /*who*/)
            {
                events.Reset();
                summons.DespawnAll();
                me->RemoveAllAreasTrigger();

                if (isEcho)
                    return;

                me->SetCanFly(false);
                me->SetDisableGravity(true);
                RemoveWalls();

                Talk(TALK_DEATH);

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_ZORLOK, DONE);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONVERT);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FORCE_AND_VERVE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_OF_ZEAL);

                    if (IsHeroic())
                        pInstance->DoCompleteAchievement(ACHIEVEMENT_HEROIC_ZORLOK);
                }

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                }

                _JustDied();
            }

            void JustReachedHome()
            {
                if (isEcho)
                    return;

                _JustReachedHome();

                isActive = false;
                events.Reset();
                if (me->HasAura(SPELL_SONG_OF_THE_EMPRESS))
                    me->RemoveAura(SPELL_SONG_OF_THE_EMPRESS);

                if (pInstance)
                    pInstance->SetBossState(DATA_ZORLOK, FAIL);
            }

            void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    if (pInstance->GetBossState(DATA_ZORLOK) != IN_PROGRESS)
                        pInstance->SetBossState(DATA_ZORLOK, IN_PROGRESS);
                }
                // Creating walls
                for (uint8 i = 0; i < 3; ++i)
                    // Arena walls
                    me->SummonGameObject(GOB_ARENA_WALLS, finalPhaseWalls2[i].GetPositionX(), finalPhaseWalls2[i].GetPositionY(), finalPhaseWalls2[i].GetPositionZ(), finalPhaseWalls2[i].GetOrientation(), 0, 0, 0, 0, 7200);


                // 10 minutes enrage timer
                if (!isEcho)
                    events.ScheduleEvent(EVENT_BERSERK, 600000);

                if (isAttEcho)
                    events.ScheduleEvent(EVENT_ATTENUATION, 15000);
                if (isFaVEcho)
                    events.ScheduleEvent(EVENT_FORCE_AND_VERVE, 15000);
            }

            void MoveInLineOfSight(Unit* attacker)
            {
                // Do nothing if not in attack phase (ie. flying), or if the unit beside isn't a player
                if (!isEcho && (!me->HasReactState(REACT_DEFENSIVE) || attacker->GetTypeId() != TYPEID_PLAYER))
                    return;

                // If is using Song of Empress, stop it
                if (me->HasAura(SPELL_SONG_OF_THE_EMPRESS))
                    me->RemoveAura(SPELL_SONG_OF_THE_EMPRESS);

                // Start attacking player
                me->AddThreat(attacker, 0.0f);
            }

            void KilledUnit(Unit* victim)
            {
                if (isEcho)
                    return;

                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_SLAY_01 + urand(0, 1));
            }

            uint32 ChoosePlatform()
            {
                // In Heroic mode, the platforms order is fixed
                if (IsHeroic())
                    return platformToUse = ++numPlat;

                // Using a while loop considering it's possible none of the platforms could be selected due to the rand condition : if so, we need to redo
                // another loop(s) until a platform is picked.
                while (true)
                {
                    // count is used to set the same chance to be chosen for all platforms
                    uint32 count = 0;
                    for (auto platform : platforms)
                    {
                        // can only trigger if urand returns 0
                        if (!urand(0, uint32(platforms.size() - (1 + count))))
                        {
                            platforms.remove(platform);
                            ++numPlat;
                            return platformToUse = platform;
                        }
                        ++count;
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                // Check if trashes are done
                if (pInstance && !isEcho)
                {
                    EncounterState bossState = pInstance->GetBossState(DATA_ZORLOK);
                    if (bossState == NOT_STARTED || bossState == TO_BE_DECIDED)
                    {
                        Creature* ShieldMaster = GetClosestCreatureWithEntry(me, NPC_SRATHIK_SHIELD_MASTER, 200.0f, true);
                        Creature* Zealok       = GetClosestCreatureWithEntry(me, NPC_ZARTHIK_ZEALOT,        200.0f, true);
                        Creature* Fanatic      = GetClosestCreatureWithEntry(me, NPC_SETTHIK_FANATIC,       200.0f, true);
                        Creature* BoneSmasher  = GetClosestCreatureWithEntry(me, NPC_ENSLAVED_BONESMASHER,  200.0f, true);
                        Creature* Supplicant   = GetClosestCreatureWithEntry(me, NPC_ZARTHIK_SUPPLICANT,    200.0f, true);
                        Creature* Supplicant2  = GetClosestCreatureWithEntry(me, NPC_ZARTHIK_SUPPLICANT_2,  200.0f, true);
                        Creature* Supplicant3  = GetClosestCreatureWithEntry(me, NPC_ZARTHIK_SUPPLICANT_3,  200.0f, true);

                        if (ShieldMaster || Zealok || Fanatic || BoneSmasher || Supplicant || Supplicant2 || Supplicant3)
                        {
                            EnterEvadeMode();
                            pInstance->SetBossState(DATA_ZORLOK, NOT_STARTED);
                            return;
                        }
                    }
                }

                if (isEcho || (isFlying && isActive) || (attacker->GetTypeId() != TYPEID_PLAYER))
                    return;

                if (!isActive)
                {
                    isActive = true;
                    if (!isEcho)
                    {
                        Talk(TALK_AGGRO);
                        me->CastSpell(tabCenter[0], tabCenter[1], tabCenter[2], SPELL_PHEROMONES_CLOUD, false);
                        phase = PHASE_ZORLOK1;
                        if (pInstance)
                            pInstance->SetBossState(DATA_ZORLOK, IN_PROGRESS);
                    }
                }

                // Removing song of the empress
                if (me->GetDistance(attacker) < 5.0f)
                {
                    uint32 spell = me->GetCurrentSpell(CURRENT_CHANNELED_SPELL) ? me->GetCurrentSpell(CURRENT_CHANNELED_SPELL)->m_spellInfo->Id : 0;
                    if (me->HasAura(SPELL_SONG_OF_THE_EMPRESS) || spell == SPELL_SONG_OF_THE_EMPRESS)
                    {
                        me->RemoveAurasDueToSpell(SPELL_SONG_OF_THE_EMPRESS);
                        me->InterruptNonMeleeSpells(true, SPELL_SONG_OF_THE_EMPRESS);
                        me->CastStop(SPELL_SONG_OF_THE_EMPRESS);
                        AttackStart(attacker);
                        me->SetInCombatWith(attacker);
                    }
                }

                if (isEcho || phase == PHASE_ZORLOK2)
                    return;

                // Phase 1
                if (me->HealthBelowPctDamaged(100 - 20 * numPlat, damage) && phase == PHASE_ZORLOK1)
                {
                    events.Reset();
                    // Switching platforms at 100%, 80% and 60% remaining life
                    if (numPlat < 3)
                    {
                        if (IsHeroic())
                        {
                            // Leaving Force and Verve Echo
                            if (numPlat == 1)
                                me->SummonCreature(NPC_ECHO_OF_FORCE_AND_VERVE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());

                            if (numPlat == 2)
                                me->SummonCreature(NPC_ECHO_OF_ATTENUATION, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        }
                        uint32 platformToUse = ChoosePlatform();
                        me->GetMotionMaster()->MoveTakeoff(platformToUse, zorlokReachPoints[platformToUse - 1]);
                        me->MonsterTextEmote("Imperial Vizier Zor'lok is flying to one of his platforms!", 0, true);
                    }
                    // At 40% remaining phase, switch on phase 2
                    else
                    {
                        phase = PHASE_ZORLOK2;
                        me->GetMotionMaster()->MoveTakeoff(phase, oratiumCenter[0]);
                    }

                    // Set Flying
                    SetFlying();
                }
            }

            void DoAction (int32 const action)
            {
                switch (action)
                {
                    case ACTION_SONIC_RING:
                    {
                        if (!sonicRingCount)
                        {
                            clocksideRings = (urand(0, 1) ? true : false);
                            xr = me->GetPositionX();
                            yr = me->GetPositionY();
                            zr = me->GetPositionZ();
                            orientation = me->GetOrientation();
                        }

                        float variation = M_PI/2;

                        float frontAngle = CorrectAngle(orientation + ringOrientation);
                        float leftAngle  = CorrectAngle(frontAngle + variation);
                        float rightAngle = CorrectAngle(frontAngle - variation);
                        float backAngle  = CorrectAngle(frontAngle + 2 * variation);

                        me->SummonCreature(NPC_SONIC_RING, xr, yr, zr, frontAngle);
                        me->SummonCreature(NPC_SONIC_RING, xr, yr, zr, leftAngle);
                        me->SummonCreature(NPC_SONIC_RING, xr, yr, zr, rightAngle);
                        me->SummonCreature(NPC_SONIC_RING, xr, yr, zr, backAngle);

                        if (sonicRingCount >= 10)
                        {
                            sonicRingCount  = 0;
                            ringOrientation = 0.0f;
                            // Resetting coords
                            xr = 0.0f;
                            yr = 0.0f;
                            zr = 0.0f;
                            orientation = 0.0f;
                            // Reset Zor'lok in combat
                            me->SetReactState(REACT_DEFENSIVE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED|UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_STUNNED);
                            me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_ENEMY_INTERACT);
                        }
                        else
                        {
                            ++sonicRingCount;
                            ringOrientation = CorrectAngle(ringOrientation + (clocksideRings ? -M_PI / 6 : M_PI / 6));
                            events.ScheduleEvent(EVENT_SUMMON_RINGS, 1000);
                        }
                        break;
                    }
                    case ACTION_SONIC_PULSE:
                    {
                        // To have a proper spawn, we save the position on first pulse, and progressively increase/decrease (according to clocksideRings)
                        // the angle the pulse will have and so, the path they'll follow
                        if (!sonicPulseCount)
                        {
                            xp = me->GetPositionX();
                            yp = me->GetPositionY();
                            zp = me->GetPositionZ();
                            op = me->GetOrientation();
                        }

                        float variation = 2 * M_PI / 3;

                        // Summons in Y (3 branches)
                        float mainAngle = CorrectAngle(op + pulseOrientation);
                        float angLeft   = CorrectAngle(mainAngle + variation);
                        float angRight  = CorrectAngle(mainAngle - variation);

                        me->SummonCreature(NPC_SONIC_PULSE, xp, yp, zp, mainAngle);
                        me->SummonCreature(NPC_SONIC_PULSE, xp, yp, zp, angLeft);
                        me->SummonCreature(NPC_SONIC_PULSE, xp, yp, zp, angRight);

                        if (sonicPulseCount >= 8)
                        {
                            sonicPulseCount = 0;
                            pulseOrientation = 0.0f;
                            // Resetting coords;
                            xp = 0.0f;
                            yp = 0.0f;
                            zp = 0.0f;
                            op = 0.0f;
                        }
                        else
                        {
                            ++sonicPulseCount;
                            pulseOrientation = CorrectAngle(pulseOrientation + (clocksideRings ? -M_PI / 6 : M_PI / 6));
                            events.ScheduleEvent(EVENT_SONIC_PULSE, 1500);
                        }

                        break;
                    }
                    case ACTION_WIPE:
                    {
                        events.Reset();
                        summons.DespawnAll();

                        me->RemoveAllAreasTrigger();
                        me->RemoveAllAuras();
                        me->SetFullHealth();
                        me->SetReactState(REACT_PASSIVE);

                        isActive = false;
                        platforms.clear();
                        if (!IsHeroic())
                        {
                            platforms.push_back(PLATFORM_ZORLOK_SW);
                            platforms.push_back(PLATFORM_ZORLOK_NE);
                            platforms.push_back(PLATFORM_ZORLOK_SE);
                        }
                        numPlat = 0;
                        phase = 0;
                        hasTalk = 0;
                        platformToUse = 0;
                        actualPlatform = 0;
                        exhaleTarget = 0;
                        sonicRingCount = 0;
                        sonicPulseCount = 0;
                        ringOrientation = 0.0f;
                        pulseOrientation = 0.0f;
                        xr = 0.0f, yr = 0.0f, zr = 0.0f, orientation = 0.0f;
                        xp = 0.0f, yp = 0.0f, zp = 0.0f, op = 0.0f;
                        clocksideRings = true;
                        inhaleDone = false;
                        hasSummonedLastEcho = false;
                        RemoveWalls();

                        if (pInstance)
                        {
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_CLOUD);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONVERT);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_NOISE_CANCELLING);
                            pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FORCE_AND_VERVE);

                            pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                        }

                        SetFlying();
                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 ChooseAction()
            {
                uint32 choice = urand(1, 3);
                switch (choice)
                {
                    case 1:
                        return EVENT_ATTENUATION;
                    case 2:
                        return EVENT_CONVERT;
                    case 3:
                        return EVENT_FORCE_AND_VERVE;
                    default:
                        break;
                }
                return 0;
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (isEcho || id == 0 || type != POINT_MOTION_TYPE)
                    return;

                // General rule :
                // * if id < 4, Zor'lok is going on a platform
                // * if id = 4, Zor'lok is going on the center of the room to absorb pheromones

                // Phase 1 : going onto landing points on platforms
                if (id < 4)
                {
                    if (numPlat == 1)
                        Talk(TALK_EVENT_PHASE_1);
                    me->GetMotionMaster()->MoveLand(id + 10, zorlokPlatforms[id - 1]);
                    SetLanding(id);
                }

                // Switching from phase 1 to phase 2
                if (id == 4)
                {
                    me->CastSpell(me, SPELL_INHALE_PHEROMONES, true);
                    me->MonsterTextEmote("Imperial Vizier Zor'lok inhales Pheromones of Zeal!", 0, true);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_CLOUD);
                    events.ScheduleEvent(EVENT_PULL_RAID, 7000);
                }
            }

            uint32 GetData(uint32 type)
            {
                if (type == TYPE_PHASE_ZORLOK)
                    return phase;
                return 0;
            }

            uint64 GetGUID(int32 p_Type)
            {
                if (p_Type == TYPE_EXHALE_TARGET)
                    return exhaleTarget;

                return 0;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!isActive)
                    return;

                // On Wipe
                if (pInstance && !isEcho)
                {
                    if (pInstance->IsWipe())
                    {
                        DoAction(ACTION_WIPE);
                        // We stop here to avoid Zor'lok to cast Song of the empress
                        return;
                    }
                }

                // Remove/Set auras on players
                if (!isEcho)
                {
                    Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
                    if (!PlayerList.isEmpty())
                    {
                        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        {
                            if (Player* playr = i->getSource())
                            {
                                // Remove convert aura for players who have less than 50% remaining health
                                if (playr->HasAura(SPELL_CONVERT) && playr->HealthBelowPct(51))
                                    playr->RemoveAurasDueToSpell(SPELL_CONVERT);
                                // Pheromones of zeal - on phase 1 only
                                if (phase == PHASE_ZORLOK1)
                                {
                                    // Remove pheromones of zeal aura from players who aren't on the bottom floor
                                    if (playr->HasAura(SPELL_PHEROMONES_OF_ZEAL) && playr->GetPositionZ() >= 408.5f)
                                        playr->RemoveAura(SPELL_PHEROMONES_OF_ZEAL);
                                    // Set pheromones of zeal aura on players who are on the bottom floor
                                    else if (!playr->HasAura(SPELL_PHEROMONES_OF_ZEAL) && playr->GetPositionZ() < 408.5f)
                                        playr->AddAura(SPELL_PHEROMONES_OF_ZEAL, playr);
                                }
                            }
                        }
                    }
                }

                UpdateVictim();
                events.Update(diff);

                if (isFlying && platformToUse == actualPlatform && !isEcho && actualPlatform > 0 &&
                    ((me->GetPositionZ() < 410.5f && phase == PHASE_ZORLOK1) ||
                     (me->GetPositionZ() < 407.0f && phase == PHASE_ZORLOK2)))
                {
                    isFlying = false;
                    me->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                    me->SetReactState(REACT_DEFENSIVE);
                }

                // Song of empress
                Unit* target = me->getVictim();

                if (!isFlying && (!target || me->GetDistance(target) > 5.0f) && !me->HasUnitState(UNIT_STATE_CASTING))
                    me->CastSpell(me, SPELL_SONG_OF_THE_EMPRESS, true);
                
                switch (events.ExecuteEvent())
                {
                    // All-time events
                    case EVENT_INHALE:
                    {
                        // Can't inhale if already casting
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            events.RescheduleEvent(EVENT_INHALE, 5000);
                        else
                        {
                            // Inhale (Exhale is triggered when Zor'lok has 3-4 stacks of inhale)
                            Aura* inhale = me->GetAura(SPELL_INHALE);
                            if (!inhale || inhale->GetStackAmount() < 3 || !urand((inhale->GetStackAmount() < 4 ? 0 : 1), 1))
                            {
                                Talk(TALK_INHALE);
                                me->MonsterTextEmote("Imperial Vizier Zor'lok |cFFFF0000|Hspell:122852|h[Inhale]|h|r a big air breath!", 0, true);
                                me->CastSpell(me, SPELL_INHALE, false);
                            }
                            // Exhale
                            else
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                {
                                    exhaleTarget = target->GetGUID();
                                    Talk(TALK_EXHALE);
                                    DoCast(target, SPELL_EXHALE, true);
                                }
                            }
                            events.ScheduleEvent(EVENT_INHALE, urand(25000, 50000));
                        }
                        break;
                    }
                    // Attenuation platform
                    case EVENT_ATTENUATION:
                    {
                        if (!isEcho && me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.RescheduleEvent(EVENT_ATTENUATION, 10000);
                            return;
                        }
                        me->CastSpell(me, SPELL_ATTENUATION, true);
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED|UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_STUNNED);
                        me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                        me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_ENEMY_INTERACT);

                        uint32 action = ((phase == PHASE_ZORLOK1 || isEcho) ? EVENT_ATTENUATION : ChooseAction());
                        events.ScheduleEvent(action, 40000);

                        break;
                    }
                    case EVENT_SUMMON_RINGS:
                    {
                        DoAction(ACTION_SONIC_RING);
                        break;
                    }
                    case EVENT_SONIC_PULSE:
                    {
                        DoAction(ACTION_SONIC_PULSE);
                        break;
                    }
                    // Convert platform
                    case EVENT_CONVERT:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.RescheduleEvent(EVENT_CONVERT, 10000);
                            return;
                        }

                        me->MonsterTextEmote("Imperial Vizier Zor'lok is using is voice to |cFFFF0000|Hspell:122740|h[Convert]|h|r members of the raid and to call them by his side !", 0, true);

                        // Creating target list
                        Talk(TALK_CONVERT);
                        DoCast(SPELL_CONVERT);

                        uint32 action = ((phase == PHASE_ZORLOK1 || isEcho) ? EVENT_CONVERT : ChooseAction());
                        events.ScheduleEvent(action, 40000);

                        break;
                    }
                    // Force and Verve platform
                    case EVENT_FORCE_AND_VERVE:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.RescheduleEvent(EVENT_FORCE_AND_VERVE, 10000);
                            return;
                        }
                        // Creating Zor'lok's Noise Cancelling zones : 3 in normal mode, 4 in heroic
                        if (!isEcho)
                        {
                            for (int i = 0; i < (IsHeroic() ? 4 : 3); ++i)
                            {
                                float x = me->GetPositionX() + frand(-20.0f, 20.0f);
                                float y = me->GetPositionY() + frand(-20.0f, 20.0f);

                                me->CastSpell(x, y, me->GetPositionZ(), SPELL_MISSILE_NOISE_CANC, false);
                            }
                        }
                        // Creating Echo of Attenuation's Noise Cancelling zones : 2 on Echo (me), 2 on Zor'lok
                        else
                        {
                            Creature* caster;
                            for (int i = 0; i < 2; ++i)
                            {
                                caster = (i == 0 ? me : GetZorlok());
                                for (int j = 0; j < 2; ++j)
                                {
                                    float x = caster->GetPositionX() + frand(-20.0f, 20.0f);
                                    float y = caster->GetPositionY() + frand(-20.0f, 20.0f);

                                    caster->CastSpell(x, y, caster->GetPositionZ(), SPELL_MISSILE_NOISE_CANC, false);
                                }
                            }
                        }

                        me->AddUnitState(UNIT_STATE_CASTING);
                        events.ScheduleEvent(EVENT_CAST_FANDV, 2000);
                        break;
                    }
                    case EVENT_CAST_FANDV:
                    {
                        me->SetReactState(REACT_PASSIVE);
                        std::ostringstream text;
                        text << (me->GetName()) << " shouts with |cFFFF0000|Hspell:122713|h[Force et brio]|h|r!";
                        me->MonsterTextEmote(text.str().c_str(), 0, true);
                        me->CastSpell(me, SPELL_FORCE_AND_VERVE, true);
                        uint32 action = ((phase == PHASE_ZORLOK1 || isEcho) ? EVENT_FORCE_AND_VERVE : ChooseAction());
                        // On first Force and Verve during phase 2 in Heroic mode, Zor'lok summons the last echo at the end of spell (10 secs)
                        if (!hasSummonedLastEcho && phase == PHASE_ZORLOK2 && IsHeroic())
                        {
                            hasSummonedLastEcho = true;
                            events.ScheduleEvent(EVENT_SUMMON_LAST_ECHO, 10000);
                        }
                        events.ScheduleEvent(action, 40000);
                        break;
                    }
                    case EVENT_SUMMON_LAST_ECHO:
                    {
                        me->SummonCreature(NPC_ECHO_OF_ATTENUATION, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                        break;
                    }
                    case EVENT_BERSERK:
                    {
                        me->CastSpell(me, SPELL_ZORLOK_BERSERK, false);
                        break;
                    }
                    case EVENT_PULL_RAID:
                    {
                        if (inhaleDone)
                            break;

                        // Removing pheromones cloud
                        me->RemoveAreaTrigger(SPELL_PHEROMONES_CLOUD);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_OF_ZEAL);

                        // Landing
                        me->GetMotionMaster()->MoveLand(PHASE_ZORLOK2 + 10, oratiumCenter[1]);
                        SetLanding(PHASE_ZORLOK2);

                        // Pulling far players
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 300.0f); // TODO: adjuste searche range
                        for (std::list<Player*>::iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                            // The point is that if they're on a platform, they'll be blocked by walls, so they have to be pulled
                            if ((*itr)->GetPositionZ() > 408.5f)
                                (*itr)->CastSpell(me, SPELL_MAGNETIC_PULSE, false);

                        // Creating Walls for final phase
                        for (uint8 i = 0; i < 3; ++i)
                            me->SummonGameObject(GOB_FINAL_PHASE_WALLS, finalPhaseWalls1[i].GetPositionX(), finalPhaseWalls1[i].GetPositionY(), finalPhaseWalls1[i].GetPositionZ(), finalPhaseWalls1[i].GetOrientation(), 0, 0, 0, 0, 7200);

                        inhaleDone = true;

                        break;
                    }
                    default:
                        break;
                }
                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_zorlokAI(creature);
        }
};

// Sonic Ring - 62689
class mob_sonic_ring : public CreatureScript
{
    public:
        mob_sonic_ring() : CreatureScript("mob_sonic_ring") { }

        struct mob_sonic_ringAI : public ScriptedAI
        {
            mob_sonic_ringAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                me->SetDisplayId(DISPLAYID_INVISIBLE);
                me->CastSpell(me, SPELL_SONIC_RING_VISUAL, false);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                // Low speed
                me->SetSpeed(MOVE_WALK, 0.5f);
                me->SetSpeed(MOVE_RUN,  0.5f);

                Position targetPoint = GetTargetPoint(me, 50.0f);
                me->GetMotionMaster()->MovePoint(1, targetPoint);
                // In case the mob is unable to reach the target point
                me->DespawnOrUnsummon(10000);

            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id == 1)
                    me->DespawnOrUnsummon();
            }

            Position GetTargetPoint(Creature* sonic, float dist)
            {
                /*
                 * The main idea is : a circle has 4 quarters; the principle is to define a point at the specified distance forward according the orientation,
                 * and use this point as a destination for MovePoint(). To calculate this point, we use the orientation to get a ratio between X and Y in the
                 * main "quarter" (both positive x and y): the point is that "ratio x" is a proportion of orientation/(pi/2), that is: if orientation = 0,
                 * orientation/(pi/2) = 0 and so ratio-x = 0%. If orientation = pi/2, then orientation/(pi/2) = (pi/2)/(pi/2) = 1, ratio-x = 100%. And
                 * ratio-y = 100% - ratio-x. So if ratio-x = 60%, ratio-y = 40%. That means that when you walk 1 yd in the orientation direction, you'll
                 * made 0.6yd forward and 0.4yd sidewards (more or less). To be precise, we should use pythagorean theorem, but the difference would be
                 * small.
                 *
                 * The range orientation from 0 to pi/2 represents a quarter circle where x and y will be both positives, and we use this quarter circle
                 * to determine general ratio between x and y. Then, we just have to "rotate" to apply this to the right orientation. According to this
                 * initial orientation, we may need to switch x and y ratio (when turned on left or right, moving forward is moving on y-axis, and not
                 * on x-axis, for instance), and/or to apply negatives values (if orientation is pi, we're moving backwards, so the x-value decreases,
                 * while if orientation is 0.0, we're moving forwards, and so, the x-value increases, but we're still on the same axis).
                 */

                float posX = sonic->GetPositionX();
                float posY = sonic->GetPositionY();
                float posZ = sonic->GetPositionZ();
                float orientation = sonic->GetOrientation();

                // Retrieving absolute orientation - turn will be used to know in which quarter we are.
                float absOri = orientation;
                uint8 turn = 0;
                while (absOri > (M_PI / 2))
                {
                    absOri -= (M_PI / 2);
                    turn = ++turn % 4;
                }

                // Looking for ratio between X and Y
                float percentX = ((M_PI / 2) - absOri) / (M_PI / 2);
                float percentY = 1.0f - percentX;

                // Applying negatives directions according to orientation
                switch (turn)
                {
                    // -x / +y / switch
                    case 1:
                    {
                        float tmpVal = percentX;
                        percentX = -percentY;
                        percentY = tmpVal;
                        break;
                    }
                    // -x / -y / no switch
                    case 2:
                    {
                        percentX = -percentX;
                        percentY = -percentY;
                        break;
                    }
                    // +x / -y / switch
                    case 3:
                    {
                        float tmpVal = percentX;
                        percentX = percentY;
                        percentY = -tmpVal;
                        break;
                    }
                    // +x / +y / no switch : no change
                    default:
                        break;
                }

                // Calculating reaching point
                float pointX = posX;
                float pointY = posY;

                Position reachPoint = {pointX, pointY, posZ, orientation};

                do
                {
                    pointX += percentX;
                    pointY += percentY;
                    reachPoint.Relocate(pointX, pointY);
                } while (sonic->GetDistance(reachPoint) < dist);

                return reachPoint;
            }

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sonic_ringAI(creature);
        }
};

// Sonic Pulse (Heroic Mode) - 63837
class mob_sonic_pulse : public CreatureScript
{
    public:
        mob_sonic_pulse() : CreatureScript("mob_sonic_pulse") { }

        struct mob_sonic_pulseAI : public ScriptedAI
        {
            mob_sonic_pulseAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                me->SetDisplayId(DISPLAYID_INVISIBLE);
                me->CastSpell(me, SPELL_SONIC_PULSE_VISUAL, false);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                // Low speed
                me->SetSpeed(MOVE_WALK, 0.3f);
                me->SetSpeed(MOVE_RUN,  0.3f);

                Position targetPoint = GetTargetPoint(me, 50.0f);
                me->GetMotionMaster()->MovePoint(1, targetPoint);
                // In case the mob is unable to reach the target point
                me->DespawnOrUnsummon(10000);
            }

            Position GetTargetPoint(Creature* sonic, float dist)
            {
                /*
                 * The main idea is : a circle has 4 quarters; the principle is to define a point at the specified distance forward according the orientation,
                 * and use this point as a destination for MovePoint(). To calculate this point, we use the orientation to get a ratio between X and Y in the
                 * main "quarter" (both positive x and y): the point is that "ratio x" is a proportion of orientation/(pi/2), that is: if orientation = 0,
                 * orientation/(pi/2) = 0 and so ratio-x = 0%. If orientation = pi/2, then orientation/(pi/2) = (pi/2)/(pi/2) = 1, ratio-x = 100%. And
                 * ratio-y = 100% - ratio-x. So if ratio-x = 60%, ratio-y = 40%. That means that when you walk 1 yd in the orientation direction, you'll
                 * made 0.6yd forward and 0.4yd sidewards (more or less). To be precise, we should use pythagorean theorem, but the difference would be
                 * small.
                 *
                 * The range orientation from 0 to pi/2 represents a quarter circle where x and y will be both positives, and we use this quarter circle
                 * to determine general ratio between x and y. Then, we just have to "rotate" to apply this to the right orientation. According to this
                 * initial orientation, we may need to switch x and y ratio (when turned on left or right, moving forward is moving on y-axis, and not
                 * on x-axis, for instance), and/or to apply negatives values (if orientation is pi, we're moving backwards, so the x-value decreases,
                 * while if orientation is 0.0, we're moving forwards, and so, the x-value increases, but we're still on the same axis).
                 */

                float posX = sonic->GetPositionX();
                float posY = sonic->GetPositionY();
                float posZ = sonic->GetPositionZ();
                float orientation = sonic->GetOrientation();

                // Retrieving absolute orientation - turn will be used to know in which quarter we are.
                float absOri = orientation;
                uint8 turn = 0;
                while (absOri > (M_PI / 2))
                {
                    absOri -= (M_PI / 2);
                    turn = ++turn % 4;
                }

                // Looking for ratio between X and Y
                float percentX = ((M_PI / 2) - absOri) / (M_PI / 2);
                float percentY = 1.0f - percentX;

                // Applying negatives directions according to orientation
                switch (turn)
                {
                    // -x / +y / switch
                    case 1:
                    {
                        float tmpVal = percentX;
                        percentX = -percentY;
                        percentY = tmpVal;
                        break;
                    }
                    // -x / -y / no switch
                    case 2:
                    {
                        percentX = -percentX;
                        percentY = -percentY;
                        break;
                    }
                    // +x / -y / switch
                    case 3:
                    {
                        float tmpVal = percentX;
                        percentX = percentY;
                        percentY = -tmpVal;
                        break;
                    }
                    // +x / +y / no switch : no change
                    default:
                        break;
                }

                // Calculating reaching point
                float pointX = posX;
                float pointY = posY;

                Position reachPoint = {pointX, pointY, posZ, orientation};

                do
                {
                    pointX += percentX;
                    pointY += percentY;
                    reachPoint.Relocate(pointX, pointY);
                } while (sonic->GetDistance(reachPoint) < dist);

                return reachPoint;
            }

            void MovementInform(uint8 point, uint8 type)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (point)
                    me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sonic_pulseAI(creature);
        }
};

// Inhale - 122852
class spell_inhale : public SpellScriptLoader
{
    public:
        spell_inhale() : SpellScriptLoader("spell_inhale") { }

        class spell_inhale_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_inhale_SpellScript);

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, GetSpellInfo()->Effects[effIndex].BasePoints, false);
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_inhale_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_inhale_SpellScript();
        }
};

// Attenuation - 122440
class spell_attenuation : public SpellScriptLoader
{
    public:
        spell_attenuation() : SpellScriptLoader("spell_attenuation") { }

        class spell_attenuation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_attenuation_SpellScript);

            void Apply()
            {
                if (Creature* zorlok = GetCaster()->ToCreature())
                {
                    // Summoning Sonic Rings
                    zorlok->AI()->DoAction(ACTION_SONIC_RING);
                    // Summoning additional Sonic Pulses in Heroic mode
                    if (zorlok->GetInstanceScript()->instance->IsHeroic())
                        zorlok->AI()->DoAction(ACTION_SONIC_PULSE);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_attenuation_SpellScript::Apply);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_attenuation_SpellScript();
        }
};

// Force and Verve (Aura) - 122718
class spell_force_verve : public SpellScriptLoader
{
    public:
        spell_force_verve() : SpellScriptLoader("spell_force_verve") { }

        class spell_force_verve_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_force_verve_SpellScript);

            void ApplyEffect()
            {
                if (Player* target = GetHitPlayer())
                    if (target->HasAura(SPELL_NOISE_CANCELLING))
                        SetHitDamage(GetHitDamage() * 0.25);
            }

            void SetReact()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                    caster->SetReactState(REACT_DEFENSIVE);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_force_verve_SpellScript::ApplyEffect);
                AfterCast += SpellCastFn(spell_force_verve_SpellScript::SetReact);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_force_verve_SpellScript();
        }
};

// Sonic Ring (Aura) - 122336
class spell_sonic_ring : public SpellScriptLoader
{
    public:
        spell_sonic_ring() : SpellScriptLoader("spell_sonic_ring") { }

        class spell_sonic_ring_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sonic_ring_AuraScript);

            void ApplyAura(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_SONIC_RING_AURA, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sonic_ring_AuraScript::ApplyAura, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_sonic_ring_AuraScript::ApplyAura, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        class spell_sonic_ring_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sonic_ring_SpellScript);

            void Effect()
            {
                if (Player* target = GetHitPlayer())
                    if (target->HasAura(SPELL_NOISE_CANCELLING))
                        SetHitDamage(GetHitDamage() * 0.4);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sonic_ring_SpellScript::Effect);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sonic_ring_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sonic_ring_SpellScript();
        }
};

// Sonic Pulse (Aura) - 124673
class spell_sonic_pulse : public SpellScriptLoader
{
    public:
        spell_sonic_pulse() : SpellScriptLoader("spell_sonic_pulse") { }

        class spell_sonic_pulse_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sonic_pulse_AuraScript);

            void ApplyAura(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_SONIC_RING_AURA, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sonic_pulse_AuraScript::ApplyAura, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_sonic_pulse_AuraScript::ApplyAura, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        class spell_sonic_pulse_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sonic_pulse_SpellScript);

            void Effect()
            {
                if (Player* target = GetHitPlayer())
                    if (target->HasAura(SPELL_NOISE_CANCELLING))
                        SetHitDamage(GetHitDamage() * 0.4);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sonic_pulse_SpellScript::Effect);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sonic_pulse_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sonic_pulse_SpellScript();
        }
};

// Exhale: 122761
class spell_zorlok_exhale : public SpellScriptLoader
{
    public:
        spell_zorlok_exhale() : SpellScriptLoader("spell_zorlok_exhale") { }

        class spell_zorlok_exhale_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_zorlok_exhale_SpellScript);

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                p_Targets.clear();

                Unit* l_Caster = GetCaster();

                if (!l_Caster || l_Caster->GetEntry() != NPC_ZORLOK)
                    return;

                uint64 l_TargetGuid = l_Caster->GetAI()->GetGUID(TYPE_EXHALE_TARGET);
                if (!l_TargetGuid)
                    return;

                if (Player* l_Target = ObjectAccessor::GetPlayer(*l_Caster, l_TargetGuid))
                {
                    if (!l_Target->isAlive())
                        return;

                    p_Targets.push_back(l_Target);
                }
            }

            void RemoveStacks()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Aura* l_Aura = l_Caster->GetAura(SPELL_INHALE))
                        l_Aura->Remove();
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_zorlok_exhale_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
                AfterCast += SpellCastFn(spell_zorlok_exhale_SpellScript::RemoveStacks);
            }
        };

        class spell_zorlok_exhale_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_zorlok_exhale_AuraScript);

            void UpdateTarget(AuraEffect const* /*aurEff*/)
            {
                // Retrieve caster and current target (owner of the aura)
                Unit* caster = GetCaster();
                WorldObject* owner = GetOwner();

                if (!caster || !owner)
                    return;

                // target should be a Unit
                Unit* target = owner->ToUnit();
                if (!target)
                    return;

                // Assume the target would remain the same
                Unit* newTarget = target;

                // Distance to check, that is between the current target and the caster
                float range = caster->GetExactDist2d(target);

                // Get all players in this radius
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, caster, range);

                // No player found? Nothing to do, the target remains the same
                if (!playerList.empty())
                {
                    // Players have been found, we order them by distance from the caster
                    playerList.sort(JadeCore::DistanceCompareOrderPred(caster));

                    // Remove players that are in the range but not between the caster and the current target
                    playerList.remove_if([this, owner, caster](Player* player) -> bool
                    {
                        if (player->IsInBetween(owner, caster, 3.f))
                            return false;

                        return true;
                    });

                    // There's at least one player between the current target and the caster: we pick the closest player from the caster
                    if (!playerList.empty())
                        newTarget = playerList.front();
                }

                // If the picked target is already the current target, nothing to do
                if (target != newTarget)
                {
                    // We have a new target: we remove the aura from the old target and put it on the new one
                    target->RemoveAura(SPELL_EXHALE);
                    caster->AddAura(SPELL_EXHALE, newTarget);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_zorlok_exhale_AuraScript::UpdateTarget, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_zorlok_exhale_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_zorlok_exhale_AuraScript();
        }
};

// Exhale (damage): 122760
class spell_zorlok_exhale_damage : public SpellScriptLoader
{
    public:
        spell_zorlok_exhale_damage() : SpellScriptLoader("spell_zorlok_exhale_damage") { }

        class spell_zorlok_exhale_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_zorlok_exhale_damage_SpellScript);

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Caster->GetAI() || l_Caster->GetEntry() != NPC_ZORLOK)
                    return;

                uint64 l_TargetGuid = l_Caster->GetAI()->GetGUID(TYPE_EXHALE_TARGET);
                if (!l_TargetGuid)
                    return;

                Unit* l_CurrentTarget = Player::GetPlayer(*l_Caster, l_TargetGuid);
                if (!l_CurrentTarget || !l_CurrentTarget->isAlive())
                    return;

                WorldObject* l_NewTarget = nullptr;
                for (WorldObject* l_Object : p_Targets)
                {
                    if (l_Object->IsInBetween(l_Caster, l_CurrentTarget, 4.0f))
                    {
                        if (!l_NewTarget || l_Caster->GetDistance(l_NewTarget) > l_Caster->GetDistance(l_Object))
                            l_NewTarget = l_Object;
                    }
                }

                if (!l_NewTarget)
                    l_NewTarget = l_CurrentTarget;

                p_Targets.clear();
                p_Targets.push_back(l_NewTarget);
            }

            void HandleHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Aura* l_Aura = l_Caster->GetAura(SPELL_INHALE))
                    {
                        uint8 l_Stacks = l_Aura->GetStackAmount();
                        SetHitDamage(GetHitDamage() * (1 + l_Stacks * 0.5f));
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_zorlok_exhale_damage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_129);
                OnHit += SpellHitFn(spell_zorlok_exhale_damage_SpellScript::HandleHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_zorlok_exhale_damage_SpellScript();
        }
};

// 122740 - Convert
class spell_convert : public SpellScriptLoader
{
    public :
        spell_convert() : SpellScriptLoader("spell_convert") { }

        class spell_convertSpellScript : public SpellScript
        {
            PrepareSpellScript(spell_convertSpellScript);

            void SelectTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();

                if (Unit* caster = GetCaster())
                {
                    InstanceScript* pInstance = caster->GetInstanceScript();
                    if (!pInstance)
                        return;

                    std::list<Player*> playerList;
                    std::list<Player*> playerToRemove;
                    
                    /// Get Players
                    GetPlayerListInGrid(playerList, caster, 60.0f);
                    
                    /// Remove invalid ones
                    for (Player* itr : playerList)
                    {
                        if (!(itr)->isAlive() || (itr)->HasAura(SPELL_CONVERT))
                            playerToRemove.push_back(itr);
                    }
                    
                    for (Player* itr : playerToRemove)
                        playerList.remove(itr);

                    uint8 maxVictims = pInstance->instance->Is25ManRaid() ? 5 : 2;
                    uint8 victimCounter = 0;
                    
                    for (auto player : playerList)
                    {
                        victimCounter++;
                        if (victimCounter > maxVictims)
                            break;
                        
                        targets.push_back(player);
                    }             
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_convertSpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_convertSpellScript();
        }
};

/// Created by spell 122731 - Create Noise Cancelling Area Trigger
class at_cancelling_noise : public MS::AreaTriggerEntityScript
{
    public:
        at_cancelling_noise() : MS::AreaTriggerEntityScript("at_cancelling_noise") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (!l_Caster)
                return;

            std::list<Player*> l_PlayerList;
            GetPlayerListInGrid(l_PlayerList, p_AreaTrigger, 10.f);

            if (!l_PlayerList.empty())
            {
                for (Player* l_Target : l_PlayerList)
                {
                    // Periodic absorption for Imperial Vizier Zor'lok's Force and Verve and Sonic Rings
                    if (l_Target->GetDistance(p_AreaTrigger) > 2.0f && l_Target->HasAura(SPELL_NOISE_CANCELLING))
                        l_Target->RemoveAura(SPELL_NOISE_CANCELLING);
                    else if (l_Target->GetDistance(p_AreaTrigger) <= 2.0f && !l_Target->HasAura(SPELL_NOISE_CANCELLING))
                        l_Caster->AddAura(SPELL_NOISE_CANCELLING, l_Target);
                }
            }
        }

};


void AddSC_boss_imperial_vizier_zorlok()
{
    new boss_zorlok();                  ///< 62980 - Imperial Vizier Zor'lok
    new mob_sonic_ring();               ///< 62689 - Sonic Ring
    new mob_sonic_pulse();              ///< 63837 - Sonic Pulse
    new spell_inhale();                 ///< 122852 - Inhale
    new spell_attenuation();            ///< 122440 - Attenuation
    new spell_force_verve();            ///< 122718 - Force and verve
    new spell_sonic_ring();             ///< 122336 - Sonic Ring
    new spell_sonic_pulse();            ///< 124673 - Sonic Pulse
    new spell_zorlok_exhale();          ///< 122761 - Exhale
    new spell_zorlok_exhale_damage();   ///< 122760 - Exhale (damage aura)
    new spell_convert();                ///< 122740 - Convert
    new at_cancelling_noise();          ///< 122731 - Create Noise Cancelling Area Trigger
}

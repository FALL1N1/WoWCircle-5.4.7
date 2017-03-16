
#include "ScriptPCH.h"
#include "PetAI.h"
#include "MoveSplineInit.h"
#include "LFGMgr.h"

enum Items
{
    ITEM_SEALED_TOME_OF_THE_LOST_LEGION = 92426, // dropped by elite mobs

    ITEM_THE_CODEX_OF_XERRATH           = 92441, // begins the first quest

    ITEM_LEGACY_OF_THE_MASTERS          = 83078,
};

enum Quests
{
    QUEST_THE_UNUSUAL_TOME              = 32295, // 
    QUEST_READER_FOR_THE_DEAD_TONQUE    = 32307,
    QUEST_A_TALE_OF_SIX_MASTERS_A       = 32310,
    QUEST_A_TALE_OF_SIX_MASTERS_H       = 32309,
    QUEST_SEEKING_THE_SOULSTONES        = 32317,
    QUEST_SEEK_THE_SIGNAL               = 32324,
    QUEST_INFILTRATING_THE_BLACK_TEMPLE = 32325,
};

enum QuestCredits
{
    // The Unusual Tome
    LANGUAGE_CREDIT     = 67944,
    PURPOSE_CREDIT      = 67945,

    // Seeking the Soulstones
    NETHERSTORM_MEMORY  = 68783, // does not equals name of the creature template, but works
    HELLFIRE_MEMORY     = 68782, // does not equals name of the creature template, but works
    UNK_MEMORY          = 68780,

    // Seek the Signal
    BLACK_TEMPLATE_ENTRANCE_CREDIT  = 68053,
};

enum Spells
{
    // Seeking the Soulstones
    SPELL_COLD                  = 134614,
    SPELL_WARM                  = 134615,
    SPELL_HOT                   = 134616,
    SPELL_RITUAL_ENSLAVEMENT    = 22987,

    // Jubeka Shadowbreaker trader
    SPELL_PURGE_XERRATH         = 139366,   // remove green fire from warlock
    SPELL_CODEX_OF_XERRATH_1    = 101508,
    SPELL_CODEX_OF_XERRATH_2    = 137206,
};

enum Creatures
{
    NPC_KANRETHAD_EBONLOCKE             = 68019, // npc is at all scenes
    NPC_KANRETHAD_EBONLOCKE_AT_END      = 69964, // at the end of the scenario
    NPC_KANRETHAD_EBONLOCKE_PRISON      = 70271, // at Jubeka trader

    NPC_JUBEKA_SHADOWBREAKER            = 68018, // npc is at all scenes
    NPC_JUBEKA_SHADOWBREAKER_AT_END     = 70166, // at the end of the scenario
    NPC_JUBEKA_SHADOWBREAKER_TRADER     = 70270, // npc can switch on/off the green fire for warlocks those completed the quest

    NPC_DOOMGUARD                       = 11859, // used as demon at blade's edge memory
};

enum GameObjects
{
    // A Tale of Six Masters
    GO_LEGACY_OF_THE_MASTERS            = 212212,

    // Seeking the Soulstones
    GO_SOULSTONE_FRAGMENT_SHADOWMOON    = 216325,
    GO_SOULSTONE_FRAGMENT_BLADES_EDGE   = 216326,
    GO_SOULSTONE_FRAGMENT_HELLFIRE      = 216327,
    GO_SOULSTONE_FRAGMENT_NETHERSTORM   = 216328,

    //
    GO_JUBEKAS_MARK                     = 216481,
};

enum Actions
{
    ACTION_NETHERSTORM_MEMORY   = 1,
    ACTION_BLADES_EDGE_MEMORY,
    ACTION_SHADOWMOON_MEMORY,
};

const Position sceneJubekaPos[10] = 
{
    // Netherstorm
    {4660.41f, 2499.44f, 242.53f, 6.11f},
    {4663.05f, 2501.06f, 242.53f, 0.56f},
    {4664.80f, 2501.97f, 241.26f, 0.48f},
    {4666.37f, 2502.63f, 240.04f, 0.40f},
    {4673.67f, 2492.13f, 240.03f, 5.43f},
    {4693.09f, 2502.92f, 237.64f, 0.49f},
    {4689.16f, 2501.44f, 238.77f, 3.47f},
    {4694.06f, 2506.39f, 236.95f, 0.78f},

    // Blade's Edge
    {3272.55f, 4638.30f, 216.22f, 0.20f},

    // Shadowmoon
    {-3588.13f, 1858.85f, 47.25f, 3.48f}
};

const Position sceneKanrethanPos[11] = 
{
    // Netherstorm
    {4668.06f, 2509.81f, 240.04f, 3.69f},
    {4667.71f, 2503.45f, 240.03f, 3.62f},
    {4675.37f, 2502.73f, 234.00f, 6.12f},
    {4685.02f, 2504.75f, 238.90f, 0.22f},

    // Blade's Edge
    {3276.03f, 4635.43f, 216.40f, 0.88f},
    {3272.25f, 4635.08f, 216.32f, 3.30f},
    {3272.42f, 4636.47f, 216.29f, 1.40f},

    // Shadowmoon
    {-3598.91f, 1857.59f, 47.25f, 0.09f},
    {-3593.49f, 1858.09f, 47.25f, 0.09f},
    {-3592.94f, 1854.28f, 47.25f, 4.85f},
    {-3591.66f, 1855.22f, 47.25f, 0.63f}
};

const Position sceneAddsPos[1] = 
{
    // Blade's Edge, Doomguard
    {3280.35f, 4640.53f, 216.53f, 3.64f}
};

enum ScriptTextsKanrethad
{
    // Netherstorm
    SAY_KANRETHAD_NETHERSTORM_SCENE_1   = 0,
    SAY_KANRETHAD_NETHERSTORM_SCENE_2   = 1,
    SAY_KANRETHAD_NETHERSTORM_SCENE_3   = 2,
    SAY_KANRETHAD_NETHERSTORM_SCENE_4   = 3,

    // Blade's Edge
    SAY_KANRETHAD_BLADES_EDGE_SCENE_1   = 4,
    SAY_KANRETHAD_BLADES_EDGE_SCENE_2   = 5,
    SAY_KANRETHAD_BLADES_EDGE_SCENE_3   = 6,
    SAY_KANRETHAD_BLADES_EDGE_SCENE_4   = 7,
    SAY_KANRETHAD_BLADES_EDGE_SCENE_5   = 8,
    SAY_KANRETHAD_BLADES_EDGE_SCENE_6   = 9,
    SAY_KANRETHAD_BLADES_EDGE_SCENE_7   = 10,

    SAY_KANRETHAD_SHADOWMOON_SCENE_1    = 11,
    SAY_KANRETHAD_SHADOWMOON_SCENE_2    = 12,
    SAY_KANRETHAD_SHADOWMOON_SCENE_3    = 13,
    SAY_KANRETHAD_SHADOWMOON_SCENE_4    = 14,
    SAY_KANRETHAD_SHADOWMOON_SCENE_5    = 15,
    SAY_KANRETHAD_SHADOWMOON_SCENE_6    = 16,
    SAY_KANRETHAD_SHADOWMOON_SCENE_7    = 17,
    SAY_KANRETHAD_SHADOWMOON_SCENE_8    = 18,

    SAY_KANRETHAD_MAX,
};

enum ScriptTextsJubeka
{
    // Netherstorm
    SAY_JUBEKA_NETHERSTORM_SCENE_1   = 0,
    SAY_JUBEKA_NETHERSTORM_SCENE_2   = 1,
    SAY_JUBEKA_NETHERSTORM_SCENE_3   = 2,
    SAY_JUBEKA_NETHERSTORM_SCENE_4   = 3,
    SAY_JUBEKA_NETHERSTORM_SCENE_5   = 4,
    SAY_JUBEKA_NETHERSTORM_SCENE_6   = 5,
    SAY_JUBEKA_NETHERSTORM_SCENE_7   = 6,

    // Blade's Edge
    SAY_JUBEKA_BLADES_EDGE_SCENE_1   = 7,

    // Shadowmoon
    SAY_JUBEKA_SHADOWMOON_SCENE_1    = 8,
    SAY_JUBEKA_SHADOWMOON_SCENE_2    = 9,
    SAY_JUBEKA_SHADOWMOON_SCENE_3    = 10,
    SAY_JUBEKA_SHADOWMOON_SCENE_4    = 11,
    SAY_JUBEKA_SHADOWMOON_SCENE_5    = 12,

    SAY_JUBEKA_MAX,
};

enum ScriptTextsAdds
{
    SAY_DOOMGUARD_BLADES_EDGE_SCENE_1   = 0,
    SAY_DOOMGUARD_BLADES_EDGE_SCENE_2   = 1,
    SAY_DOOMGUARD_BLADES_EDGE_SCENE_3   = 2,
    SAY_DOOMGUARD_BLADES_EDGE_SCENE_4   = 3,
    SAY_DOOMGUARD_BLADES_EDGE_SCENE_5   = 4,
};

enum SceneDurations
{
    SCENE_DURATION_NETHERSTORM  = 64000,
    SCENE_DURATION_BLADES_EDGE  = 94000,
    SCENE_DURATION_SHADOWMOON   = 65000,
};

enum SceneEntries
{
    
    SCENE_BLADES_EDGE   = 286,
    SCENE_NETHERSTORM   = 287,
    SCENE_SHADOWMOON    = 285,
    SCENE_RELIQUARY     = 291,
    SCENE_XIRI          = 298,
};

class warlock_pet_32295_quest : public CreatureScript
{
    enum Values
    {
        MENU_ENTRY = 65000,
    };

    public:
        warlock_pet_32295_quest() : CreatureScript("warlock_pet_32295_quest") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
        {
            switch (player->PlayerTalkClass->GetGossipMenu().GetMenuId())
            {
                case Values::MENU_ENTRY + 10:
                case Values::MENU_ENTRY + 11:
                case Values::MENU_ENTRY + 12:
                    player->RewardPlayerAndGroupAtEvent(QuestCredits::LANGUAGE_CREDIT, player);
                    return true;
                case Values::MENU_ENTRY + 13:
                case Values::MENU_ENTRY + 14:
                case Values::MENU_ENTRY + 16:
                    player->RewardPlayerAndGroupAtEvent(QuestCredits::PURPOSE_CREDIT, player);
                    return true;
            }
            return false;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new warlock_pet_32295_questAI(creature);
        }

        struct warlock_pet_32295_questAI : public PetAI
        {
            warlock_pet_32295_questAI(Creature* creature) : PetAI(creature)
            {
            }

            void Reset()
            {
                PetAI::Reset();

                Unit* owner = me->GetOwner();
                if (!owner)
                    return;

                Player* player = owner->ToPlayer();
                if (!player)
                    return;

                if (player->GetQuestStatus(QUEST_THE_UNUSUAL_TOME) == QUEST_STATUS_INCOMPLETE)
                {
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
                else
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }
        };
};

class go_soulstone_fragment : public GameObjectScript
{
    public:
        go_soulstone_fragment() : GameObjectScript("go_soulstone_fragment") 
        { 
            sceneCreatureGuid = 0;
        }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (player->GetQuestStatus(QUEST_SEEKING_THE_SOULSTONES) == QUEST_STATUS_INCOMPLETE)
            {
                switch (go->GetEntry())
                {
                    case GO_SOULSTONE_FRAGMENT_HELLFIRE:
                        player->KilledMonsterCredit(HELLFIRE_MEMORY, 0);
                        player->KilledMonsterCredit(UNK_MEMORY, 0);
                        break;
                    case GO_SOULSTONE_FRAGMENT_NETHERSTORM:
                        player->KilledMonsterCredit(NETHERSTORM_MEMORY, 0);
                        player->PlayScene(SCENE_NETHERSTORM, player);
                        //SpawnSceneForNetherstormMemory(go->GetMap());
                        break;
                    case GO_SOULSTONE_FRAGMENT_BLADES_EDGE:
                        player->PlayScene(SCENE_BLADES_EDGE, player);
                        //SpawnSceneForBladesEdgeMemory(go->GetMap());
                        break;
                    case GO_SOULSTONE_FRAGMENT_SHADOWMOON:
                        player->PlayScene(SCENE_SHADOWMOON, player);
                        //SpawnSceneForShadowmoonMemory(go->GetMap());
                        break;
                }
            }

            return false;
        }

    private:

        void SpawnSceneForNetherstormMemory(Map* map)
        {
            if (map->GetCreature(sceneCreatureGuid))
                return;

            sceneCreatureGuid = 0;

            if (Creature* pKanrethad = map->SummonCreature(NPC_KANRETHAD_EBONLOCKE, sceneKanrethanPos[0], NULL, SCENE_DURATION_NETHERSTORM))
            {
                sceneCreatureGuid = pKanrethad->GetGUID();
                pKanrethad->AI()->DoAction(ACTION_NETHERSTORM_MEMORY);
            }

            if (Creature* pJubeka = map->SummonCreature(NPC_JUBEKA_SHADOWBREAKER, sceneJubekaPos[0], NULL, SCENE_DURATION_NETHERSTORM))
            {
                pJubeka->AI()->DoAction(ACTION_NETHERSTORM_MEMORY);
            }
        }

        void SpawnSceneForBladesEdgeMemory(Map* map)
        {
            if (map->GetCreature(sceneCreatureGuid))
                return;

            sceneCreatureGuid = 0;

            if (Creature* pKanrethad = map->SummonCreature(NPC_KANRETHAD_EBONLOCKE, sceneKanrethanPos[4], NULL, SCENE_DURATION_BLADES_EDGE))
            {
                sceneCreatureGuid = pKanrethad->GetGUID();
                pKanrethad->AI()->DoAction(ACTION_BLADES_EDGE_MEMORY);
            }

            if (Creature* pJubeka = map->SummonCreature(NPC_JUBEKA_SHADOWBREAKER, sceneJubekaPos[8], NULL, SCENE_DURATION_BLADES_EDGE))
            {
                pJubeka->AI()->DoAction(ACTION_BLADES_EDGE_MEMORY);
            }
        }

        void SpawnSceneForShadowmoonMemory(Map* map)
        {
            if (map->GetCreature(sceneCreatureGuid))
                return;

            sceneCreatureGuid = 0;

            if (Creature* pKanrethad = map->SummonCreature(NPC_KANRETHAD_EBONLOCKE, sceneKanrethanPos[7], NULL, SCENE_DURATION_SHADOWMOON))
            {
                sceneCreatureGuid = pKanrethad->GetGUID();
                pKanrethad->AI()->DoAction(ACTION_SHADOWMOON_MEMORY);
            }

            if (Creature* pJubeka = map->SummonCreature(NPC_JUBEKA_SHADOWBREAKER, sceneJubekaPos[9], NULL, SCENE_DURATION_SHADOWMOON))
            {
                pJubeka->AI()->DoAction(ACTION_SHADOWMOON_MEMORY);
            }
        }

    private:

        uint64 sceneCreatureGuid;
};

/*class SceneHelper
{
    private:

        enum ActionTypes
        {
            ACTION_TALK,
            ACTION_MOVE,
            ACTION_CAST,
        };

        class SceneAction
        {
            public:

                SceneAction(uint64 actorGuid, Map* actionMap, ActionTypes actionType, uint32 actionTimer) :
                    m_ActorGuid(actorGuid), m_ActionMap(actionMap), m_ActionType(actionType), m_ActionTimer(actionTimer) { }

                ActionTypes GetActionType() const { return m_ActionType; }
                uint32 GetActionTimer() const { return m_ActionTimer; }
                virtual void DoAction() = 0;

            protected:

                Creature* GetActor()
                {
                    return m_ActionMap->GetCreature(m_ActorGuid);
                }

                uint64 GetActorGuid()
                {
                    return m_ActorGuid;
                }

            protected:

                uint64 m_ActorGuid;
                Map* m_ActionMap;
                ActionTypes m_ActionType;
                uint32 m_ActionTimer;
        };

        class SceneActionTalk : public SceneAction
        {
            public:

                SceneActionTalk(uint64 actorGuid, Map* actionMap, uint32 talkIndex, uint32 actionTimer) : 
                    SceneAction(actorGuid, actionMap, ACTION_TALK, actionTimer), m_TalkIndex(talkIndex)
                {
                    
                }

                void DoAction()
                {
                    if (Creature* pActor = GetActor())
                    {
                        pActor->AI()->Talk(m_TalkIndex);
                    }
                }

        private:

            uint32 m_TalkIndex;

        };

        class SceneActionMove : public SceneAction
        {
            public:

                SceneActionMove(uint64 actorGuid, Map* actionMap, const Position& movePosition, bool isWalk, uint32 actionTimer) : 
                    SceneAction(actorGuid, actionMap, ACTION_MOVE, actionTimer), m_IsWalk(isWalk), m_IsManyPoints(false)
                {
                    m_MovePosition = movePosition;
                }

                SceneActionMove(uint64 actorGuid, Map* actionMap, std::vector<Position>& movePositions, bool isWalk, uint32 actionTimer) : 
                    SceneAction(actorGuid, actionMap, ACTION_MOVE, actionTimer), m_IsWalk(isWalk), m_IsManyPoints(true)
                {
                    m_MovePositions = movePositions;
                }

                void DoAction()
                {
                    Creature* pActor = GetActor();
                    if (!pActor)
                        return;

                    pActor->GetMotionMaster()->MovementExpired(false);

                    if (m_IsManyPoints)
                    {
                        Movement::MoveSplineInit init(*pActor);

                        for (std::vector<Position>::const_iterator itr = m_MovePositions.begin(); itr != m_MovePositions.end(); ++itr)
                        {
                            G3D::Vector3 point;
                            point.x = itr->GetPositionX();
                            point.y = itr->GetPositionY();
                            point.z = itr->GetPositionZ();
                            init.Path().push_back(point);
                        }

                        init.SetWalk(m_IsWalk);
                        init.Launch();
                    }
                    else
                    {
                        Movement::MoveSplineInit init(*pActor);
                        init.MoveTo(m_MovePosition.GetPositionX(), m_MovePosition.GetPositionY(), m_MovePosition.GetPositionZ());
                        init.SetWalk(m_IsWalk);
                        init.Launch();
                    }
                }

        private:

            Position m_MovePosition;
            std::vector<Position> m_MovePositions;
            bool m_IsWalk;
            bool m_IsManyPoints;

        };

    public:

        SceneHelper(uint64 actorGuid, Map* actionMap) : m_DefaultActorGuid(actorGuid), m_ActionMap(actionMap), 
            m_IsActive(false), m_SceneTimer(1000)
        {
        }

        SceneHelper(Map* actionMap) : m_DefaultActorGuid(0), m_ActionMap(actionMap), 
            m_IsActive(false), m_SceneTimer(1000)
        {
        }

        ~SceneHelper()
        {
            Clear();
        }

        void SetDefaultActorGuid(uint64 actorGuid) { m_DefaultActorGuid = actorGuid; }

        bool IsActive() const { return m_IsActive; }
        bool IsStopped() const { return !m_IsActive; }
        bool IsEmpty() const { return m_SceneActions.empty(); }

        void Activate() 
        {
            if (IsEmpty())
                return;

            m_SceneTimer = GetCurrentAction()->GetActionTimer();

            m_IsActive = true; 
        }
        void Stop() { m_IsActive = false; }

        void AddSceneActionTalk(uint32 talkIndex, uint32 actionTimer = 1000)
        {
            m_SceneActions.push_back(new SceneActionTalk(m_DefaultActorGuid, m_ActionMap, talkIndex, actionTimer));
        }

        void AddSceneActionMove(const Position& movePosition, bool isWalk, uint32 actionTimer = 1000)
        {
            m_SceneActions.push_back(new SceneActionMove(m_DefaultActorGuid, m_ActionMap, movePosition, isWalk, actionTimer));
        }

        void AddSceneActionMove(std::vector<Position>& positions, bool isWalk, uint32 actionTimer = 1000)
        {
            m_SceneActions.push_back(new SceneActionMove(m_DefaultActorGuid, m_ActionMap, positions, isWalk, actionTimer));
        }

        void Clear()
        {
            while (!m_SceneActions.empty())
            {
                delete m_SceneActions.front();
                m_SceneActions.pop_front();
            }
        }

        void Update(const uint32 diff)
        {
            if (IsStopped())
                return;

            if (m_SceneActions.empty())
            {
                Stop();
                return;
            }

            if (m_SceneTimer <= diff)
            {
                if (SceneAction* action = GetCurrentAction())
                {
                    action->DoAction();

                    delete action;
                    m_SceneActions.pop_front();
                }

                if (SceneAction* nextAction = GetCurrentAction())
                {
                    m_SceneTimer = nextAction->GetActionTimer();
                }
                else
                {
                    Stop();
                    return;
                }
            }
            else
            {
                m_SceneTimer -= diff;
            }
        }

    private:

        SceneAction* GetCurrentAction()
        {
            if (m_SceneActions.empty())
                return NULL;

            return m_SceneActions.front();
        }

    private:

        bool m_IsActive;
        uint64 m_DefaultActorGuid;
        Map* m_ActionMap;
        uint32 m_SceneTimer;
        std::list<SceneAction*> m_SceneActions;
};*/

class npc_kabrethad_ebonlocke_scene : public CreatureScript
{
    public:

        npc_kabrethad_ebonlocke_scene() : CreatureScript("npc_kabrethad_ebonlocke_scene") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_kanrethad_ebonlocke_sceneAI(creature);
        }

        struct npc_kanrethad_ebonlocke_sceneAI : public ScriptedAI
        {
            npc_kanrethad_ebonlocke_sceneAI(Creature* creature) : ScriptedAI(creature), m_SceneHelper(creature->GetGUID(), creature->GetMap()), m_DoomGuardSceneHelper(creature->GetMap())
            {

            }

            void Reset()
            {
                
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_NETHERSTORM_MEMORY)
                {
                    m_SceneHelper.Clear();
                    m_SceneHelper.AddSceneActionMove(sceneKanrethanPos[1], true,        6000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_NETHERSTORM_SCENE_1, 3000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_NETHERSTORM_SCENE_2, 10000);
                    m_SceneHelper.AddSceneActionMove(sceneKanrethanPos[2], true,        7000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_NETHERSTORM_SCENE_3, 3000);
                    m_SceneHelper.AddSceneActionMove(sceneKanrethanPos[3], false,       7000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_NETHERSTORM_SCENE_4, 5000);
                    m_SceneHelper.Activate();
                }
                else if (action == ACTION_BLADES_EDGE_MEMORY)
                {
                    if (Creature* pDoomGuard = me->SummonCreature(NPC_DOOMGUARD, sceneAddsPos[0], TEMPSUMMON_TIMED_DESPAWN, 93000))
                    {
                        pDoomGuard->AddAura(SPELL_RITUAL_ENSLAVEMENT, pDoomGuard);

                        m_DoomGuardSceneHelper.Clear();
                        m_DoomGuardSceneHelper.SetDefaultActorGuid(pDoomGuard->GetGUID());
                        m_DoomGuardSceneHelper.AddSceneActionTalk(SAY_DOOMGUARD_BLADES_EDGE_SCENE_1, 11000);
                        m_DoomGuardSceneHelper.AddSceneActionTalk(SAY_DOOMGUARD_BLADES_EDGE_SCENE_2, 30000);
                        m_DoomGuardSceneHelper.AddSceneActionTalk(SAY_DOOMGUARD_BLADES_EDGE_SCENE_3, 15000);
                        m_DoomGuardSceneHelper.AddSceneActionTalk(SAY_DOOMGUARD_BLADES_EDGE_SCENE_4, 7000);
                        m_DoomGuardSceneHelper.AddSceneActionTalk(SAY_DOOMGUARD_BLADES_EDGE_SCENE_5, 9000);
                        m_DoomGuardSceneHelper.Activate();
                    }

                    m_SceneHelper.Clear();
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_BLADES_EDGE_SCENE_1, 6000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_BLADES_EDGE_SCENE_2, 11000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_BLADES_EDGE_SCENE_3, 6000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_BLADES_EDGE_SCENE_4, 12000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_BLADES_EDGE_SCENE_5, 13000);
                    m_SceneHelper.AddSceneActionMove(sceneKanrethanPos[5], true,        29000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_BLADES_EDGE_SCENE_6, 1000);
                    m_SceneHelper.AddSceneActionMove(sceneKanrethanPos[6], true,        4000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_BLADES_EDGE_SCENE_7, 1000);
                    m_SceneHelper.Activate();
                }
                else if (action == ACTION_SHADOWMOON_MEMORY)
                {
                    m_SceneHelper.Clear();
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_SHADOWMOON_SCENE_1, 5000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_SHADOWMOON_SCENE_2, 8000);
                    m_SceneHelper.AddSceneActionMove(sceneKanrethanPos[8], true,       4000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_SHADOWMOON_SCENE_3, 1000);
                    m_SceneHelper.AddSceneActionMove(sceneKanrethanPos[9], true,       9000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_SHADOWMOON_SCENE_4, 1000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_SHADOWMOON_SCENE_5, 4000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_SHADOWMOON_SCENE_6, 6000);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_SHADOWMOON_SCENE_7, 12000);
                    m_SceneHelper.AddSceneActionMove(sceneKanrethanPos[10], true,      0);
                    m_SceneHelper.AddSceneActionTalk(SAY_KANRETHAD_SHADOWMOON_SCENE_8, 7000);
                    m_SceneHelper.Activate();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                m_SceneHelper.Update(diff);
                m_DoomGuardSceneHelper.Update(diff);
            }

        private:

        private:

            SceneHelper m_SceneHelper;

            // HACK!!! use scenehelper for doomguard
            // we cannot use a script for that pet, so do it this way (((
            SceneHelper m_DoomGuardSceneHelper;
        };
};

class npc_jubeka_shadowbreaker_scene : public CreatureScript
{
    public:

        npc_jubeka_shadowbreaker_scene() : CreatureScript("npc_jubeka_shadowbreaker_scene") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_jubeka_shadowbreaker_sceneAI(creature);
        }

        struct npc_jubeka_shadowbreaker_sceneAI : public ScriptedAI
        {
            npc_jubeka_shadowbreaker_sceneAI(Creature* creature) : ScriptedAI(creature), m_SceneHelper(creature->GetGUID(), creature->GetMap())
            {
            }

            void Reset()
            {
                ScriptedAI::Reset();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_NETHERSTORM_MEMORY)
                {
                    m_SceneHelper.Clear();
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_NETHERSTORM_SCENE_1, 5000);
                    std::vector<Position> positions;
                    positions.push_back(sceneJubekaPos[1]);
                    positions.push_back(sceneJubekaPos[2]);
                    m_SceneHelper.AddSceneActionMove(positions, true,                7000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_NETHERSTORM_SCENE_2, 4000);
                    positions.clear();
                    positions.push_back(sceneJubekaPos[3]);
                    positions.push_back(sceneJubekaPos[4]);
                    m_SceneHelper.AddSceneActionMove(positions, false,               5000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_NETHERSTORM_SCENE_3, 3000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_NETHERSTORM_SCENE_4, 9000);
                    m_SceneHelper.AddSceneActionMove(sceneJubekaPos[5], false,       0);
                    m_SceneHelper.AddSceneActionMove(sceneJubekaPos[6], true,        8000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_NETHERSTORM_SCENE_5, 4000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_NETHERSTORM_SCENE_6, 6000);
                    m_SceneHelper.AddSceneActionMove(sceneJubekaPos[7], true,        6000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_NETHERSTORM_SCENE_7, 2000);
                    m_SceneHelper.Activate();
                }
                else if (action == ACTION_BLADES_EDGE_MEMORY)
                {
                    m_SceneHelper.Clear();
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_BLADES_EDGE_SCENE_1, 30000);
                    m_SceneHelper.Activate();
                }
                else if (action == ACTION_SHADOWMOON_MEMORY)
                {
                    m_SceneHelper.Clear();
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_SHADOWMOON_SCENE_1, 1000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_SHADOWMOON_SCENE_2, 8000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_SHADOWMOON_SCENE_3, 10000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_SHADOWMOON_SCENE_4, 4000);
                    m_SceneHelper.AddSceneActionTalk(SAY_JUBEKA_SHADOWMOON_SCENE_5, 31000);
                    m_SceneHelper.Activate();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                m_SceneHelper.Update(diff);
            }

        private:

            SceneHelper m_SceneHelper;
        };
};

class go_jubekas_mark : public GameObjectScript
{
    public:
        go_jubekas_mark() : GameObjectScript("go_jubekas_mark") 
        { 
        }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            // HACK:
            if (player->GetQuestStatus(QUEST_SEEK_THE_SIGNAL) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(BLACK_TEMPLATE_ENTRANCE_CREDIT, 0);
                return true;
            }

            if (player->GetQuestStatus(QUEST_INFILTRATING_THE_BLACK_TEMPLE) != QUEST_STATUS_INCOMPLETE)
                return true;

            return false;
        }

        bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 /*sender*/, uint32 /*action*/)
        {
            player->CLOSE_GOSSIP_MENU();

            if (player->GetQuestStatus(QUEST_INFILTRATING_THE_BLACK_TEMPLE) != QUEST_STATUS_INCOMPLETE)
                return true;

            if (player->GetGroup())
                return true;

            LfgDungeonSet newDungeons;
            newDungeons.insert(594);
            
            std::string comment = "";

            sLFGMgr->Join(player, ROLE_DAMAGE, newDungeons, comment);

            return false;
        }

    private:

    private:
};

class npc_kanrethad_ebonlocke_at_prison : public CreatureScript
{
    public:

        npc_kanrethad_ebonlocke_at_prison() : CreatureScript("npc_kanrethad_ebonlocke_at_prison") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_kanrethad_ebonlocke_at_prisonAI(creature);
        }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (player->GetQuestStatus(QUEST_INFILTRATING_THE_BLACK_TEMPLE) != QUEST_STATUS_REWARDED)
                return true;

            if (player->HasSpell(SPELL_CODEX_OF_XERRATH_1) &&
                player->HasSpell(SPELL_CODEX_OF_XERRATH_2))
                return true;

            player->CastSpell(creature, 139200);

            return true;
        }

        struct npc_kanrethad_ebonlocke_at_prisonAI : public ScriptedAI
        {
            npc_kanrethad_ebonlocke_at_prisonAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void Reset()
            {
                
            }

            void DoAction(const int32 action)
            {

            }

            void UpdateAI(const uint32 diff)
            {

            }

        private:

        private:

        };
};

class npc_jubeka_shadowbreaker_trader : public CreatureScript
{
    public:

        npc_jubeka_shadowbreaker_trader() : CreatureScript("npc_jubeka_shadowbreaker_trader") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_jubeka_shadowbreaker_traderAI(creature);
        }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (!player->HasSpell(SPELL_CODEX_OF_XERRATH_1) &&
               ! player->HasSpell(SPELL_CODEX_OF_XERRATH_2))
            {
               return true;
            }

            return false;
        }

        struct npc_jubeka_shadowbreaker_traderAI : public ScriptedAI
        {
            npc_jubeka_shadowbreaker_traderAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void Reset()
            {
                ScriptedAI::Reset();
            }

            void DoAction(const int32 action)
            {

            }

            void sGossipSelect(Player* player, uint32 /*sender*/, uint32 action) override
            {
                player->CLOSE_GOSSIP_MENU();

                if (action == 0)
                {
                    DoCast(player, SPELL_PURGE_XERRATH);
                }
            }

            void UpdateAI(const uint32 diff)
            {

            }

        private:


        };
};


void AddSC_green_fire_scripts()
{
    new warlock_pet_32295_quest();

    new go_soulstone_fragment();
    new npc_kabrethad_ebonlocke_scene();        // 
    new npc_jubeka_shadowbreaker_scene();       // 
    new npc_kanrethad_ebonlocke_at_prison();    // 
    new npc_jubeka_shadowbreaker_trader();      // 

    new go_jubekas_mark();
}

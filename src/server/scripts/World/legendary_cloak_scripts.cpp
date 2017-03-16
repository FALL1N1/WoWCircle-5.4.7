
#include "ScriptPCH.h"
#include "MoveSplineInit.h"
#include "LFGMgr.h"

enum ScriptedTexts
{
    SAY_WRATHION_1_FEAR_ITSELF_INTRO                = 0,
    SAY_WRATHION_1_FEAR_ITSELF_OUTRO                = 1,
    SAY_WRATHION_1_BREATH_OF_THE_BLACK_PRINCE_INTRO = 2,
};

enum Items
{

};

enum Quests
{
    // Chapter 1
    QUEST_STRANGER_IN_A_STRANGE_LAND            = 31488,
    QUEST_STRANGER_IN_A_STRANGE_LAND_1          = 31489, // for rogue only
    QUEST_A_LEGEND_IN_THE_MAKING                = 31454,
    QUEST_TRIAL_OF_THE_BLACK_PRINCE             = 31468,
    QUEST_THE_STRENGTH_OF_ONES_FOES             = 31473,
    QUEST_FEAR_ITSELF                           = 31481,
    QUEST_BREATH_OF_THE_BLACK_PRINCE            = 31482,

    // Chapter 2
    QUEST_INCOMING                              = 31483,
    QUEST_THE_MEASURE_OF_A_LEADER_ALLIANCE      = 32373,
    QUEST_THE_PRINCES_PURSUIT_ALLIANCE          = 32374,
    QUEST_A_TEST_OF_VALOR_ALLIANCE              = 32474,
    QUEST_A_CHANGE_OF_COMMAND_ALLIANCE          = 32388,
    QUEST_THE_LION_ROARS                        = 32389,
    QUEST_CALL_OF_THE_PACKMASTER                = 32390,

    QUEST_THE_MEASURE_OF_A_LEADER_HORDE         = 32427,
    QUEST_THE_PRINCES_PURSUIT_HORDE             = 32429,
    QUEST_A_TEST_OF_VALOR_HORDE                 = 32476,
    QUEST_A_CHANGE_OF_COMMAND_HORDE             = 32430,
    QUEST_GLORY_TO_THE_HORDE                    = 32431,
    QUEST_THE_SOUL_OF_THE_HORDE                 = 32432,

    QUEST_THE_THUNDER_KING                      = 32457,

    // Chapter 3
    QUEST_MEET_ME_UPSTAIRS                      = 32590,
    QUEST_I_NEED_A_CHAMPION                     = 32592,
    QUEST_SECRETS_OF_THE_FIRST_EMPIRE           = 32591,
    QUEST_THE_THUNDER_FORGE                     = 32593,
    QUEST_SPIRIT_OF_THE_STORM_LORD              = 32594,
    QUEST_THE_CROWN_OF_HEAVEN                   = 32595,
    QUEST_ECHOES_OF_THE_TITANS                  = 32596,
    QUEST_HEART_OF_THE_THUNDER_KING             = 32597,

    // Chapter 4
    QUEST_A_RECKONING                           = 32598,
    QUEST_CELESTIAL_BLESSINGS                   = 32805,
    QUEST_CLOAK_OF_VIRTUE                       = 32861,

    // Chapter 5
    QUEST_MEET_ME_BACK_AT_THE_INN               = 33087,
    QUEST_A_TIMELESS_DISCOVERY                  = 33088,
    QUEST_SECRETS_OF_THE_TIMELESS_ISLE          = 33098,
    QUEST_THE_EMPERORS_WAY                      = 33100,
    QUEST_A_PANDAREN_LEGEND                     = 33104,
    QUEST_JUDGMENT_OF_THE_BLACK_PRINCE          = 33105,

};

enum QuestCredits
{
    // A Legent in the Making
    QUEST_WRATHION_INTRO_KILL_CREDIT            = 64649,

    // Breath of the Black Prince
    QUEST_WRATHION_FINALE_MEETING_KILL_CREDIT   = 66586,
    QUEST_WRATHION_FINALE_KILL_CREDIT           = 64664,

    // Incoming
    QUEST_WRATION_NAVAL_FLEET_KILL_CREDIT       = 64681,

    // The Measure of a Leader
    QUEST_WRATHION_INTRO_KILL_CREDIT_2          = 68316,

    // The Lion Roars & Glory to the Horde
    QUEST_TEMPLE_OF_KOTMOGU_KILL_CREDIT         = 68931,
    QUEST_SILVERSHINE_MINES_KILL_CREDIT         = 68932,

    // Call of the Packmaster & The Soul of the Horde
    QUEST_WRATHION_FINALE_KILL_CREDIT_2         = 69053,

    // The Thunder Forge
    QUEST_UNLOCK_FORGE_KILL_CREDIT              = 70093,
    QUEST_COMPLETE_SCENARIO_KILL_CREDIT         = 70094,
};

enum Spells
{
    
};

enum Creatures
{
    NPC_WRATHION_1  = 64616, // first npc standing at the tavern
    NPC_WRATHION_2  = 64822, // npc is at the veiled stair, 'breath of the black prince' rewarder
    NPC_WRATHION_3  = 69782, // npc is at the top of the tavern
    NPC_WRATHION_4  = 70438, // npc is at the thunder forge
    NPC_WRATHION_5  = 70100, // npc is at the thunder forge scenario
    NPC_WRATHION_6  = 70476, // npc is at the veiled stair, 'the crown of heaven' rewarder
    NPC_WRATHION_7  = 71317, // npc is at kun-lai summit, for the 'celestial blessings' quest
    NPC_WRATHION_8  = 71352, // npc is at jade forest, for the 'celestial blessings' quest
    NPC_WRATHION_9  = 71387, // npc it at townlong steppes, for the 'celestial blessings' quest
    NPC_WRATHION_10 = 71404, // npc is at krasarang wilds, for the 'celestial blessings' quest
    NPC_WRATHION_11 = 71424, // npc is at the veiled stair, 'celestial blessings' rewarder
    NPC_WRATHION_12 = 71426, // npc is at the veiled stair, 'cloak of virtue' rewarder
    NPC_WRATHION_13 = 72695, // npc is at the timeless isle
    NPC_WRATHION_14 = 72723, // npc it at the valley of eternal blossoms, 'the pandaren legend' rewarder
    NPC_WRATHION_15 = 73138, // npc is at the veiled stair, 'judgement of the black prince' rewarder

    NPC_ANDUIN_WRYNN    = 69257, // npc is at the top of the tavern
};

enum GameObjects
{
    
};

enum Actions
{

};

enum SceneDurations
{

};

enum SceneEntries
{
    // Chapter 1
    SCENE_TAVERN_INTRO_ALLIANCE     = 208,
    SCENE_TAVERN_INTRO_HORDE        = 234,
    SCENE_FINAL_EFFECTS             = 245,

    // Chapter 2
    SCENE_TAVERN_INTRO_ALLIANCE_2   = 315,
    SCENE_TAVERN_INTRO_HORDE_2      = 319,
    SCENE_TAVERN_FINALE_ALLIANCE_2  = 320,
    SCENE_TAVERN_FINALE_HORDE_2     = 321,

    // Chapter 3
    SCENE_SECRETS_INTRO             = 410,
    SCENE_FORGE_INTRO               = 415,
    SCENE_CROWN_INTRO               = 416,
};

bool HasIncompleteQuest(Player* player, uint32 questId)
{
    bool result = (player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE);
    return result;
}

void CompleteCredit(Player* player, uint32 creditId)
{
    player->KilledMonsterCredit(creditId, 0);
}

class npc_wrathion_64616 : public CreatureScript
{
    public:

        npc_wrathion_64616() : CreatureScript("npc_wrathion_64616") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wrathion_64616AI(creature);
        }

        struct npc_wrathion_64616AI : public ScriptedAI
        {
            npc_wrathion_64616AI(Creature* creature) : ScriptedAI(creature)
            {

            }

            void sGossipHello(Player* player)
            {
                if (HasIncompleteQuest(player, QUEST_INCOMING))
                {
                    CompleteCredit(player, QUEST_WRATION_NAVAL_FLEET_KILL_CREDIT);
                }
            }

            void sGossipSelect(Player* player, uint32 menuId, uint32 gossipListId)
            {
                player->CLOSE_GOSSIP_MENU();

                if (gossipListId == 1)
                {
                    if (HasIncompleteQuest(player, QUEST_A_LEGEND_IN_THE_MAKING))
                        CompleteCredit(player, QUEST_WRATHION_INTRO_KILL_CREDIT);

                    uint32 sceneId = player->GetTeam() == ALLIANCE ? SCENE_TAVERN_INTRO_ALLIANCE : SCENE_TAVERN_INTRO_HORDE;

                    player->PlayScene(sceneId, player);
                }
                else if (gossipListId == 2)
                {
                    if (HasIncompleteQuest(player, QUEST_THE_MEASURE_OF_A_LEADER_ALLIANCE) ||
                        HasIncompleteQuest(player, QUEST_THE_MEASURE_OF_A_LEADER_HORDE))
                    {
                        CompleteCredit(player, QUEST_WRATHION_INTRO_KILL_CREDIT_2);

                        uint32 sceneId = player->GetTeam() == ALLIANCE ? SCENE_TAVERN_INTRO_ALLIANCE_2 : SCENE_TAVERN_INTRO_HORDE_2;

                        player->PlayScene(sceneId, player);
                    }
                }
                else if (gossipListId == 3)
                {
                    if (HasIncompleteQuest(player, QUEST_CALL_OF_THE_PACKMASTER) || 
                        HasIncompleteQuest(player, QUEST_THE_SOUL_OF_THE_HORDE))
                    {
                        CompleteCredit(player, QUEST_WRATHION_FINALE_KILL_CREDIT_2);

                        uint32 sceneId = player->GetTeam() == ALLIANCE ? SCENE_TAVERN_FINALE_ALLIANCE_2 : SCENE_TAVERN_FINALE_HORDE_2;

                        player->PlayScene(sceneId, player);
                    }
                }
            }

            void sQuestAccept(Player* player, Quest const* quest) override
            {
                if (quest->GetQuestId() == QUEST_FEAR_ITSELF)
                {
                    Talk(SAY_WRATHION_1_FEAR_ITSELF_INTRO);
                }
                else if (quest->GetQuestId() == QUEST_BREATH_OF_THE_BLACK_PRINCE)
                {
                    Talk(SAY_WRATHION_1_BREATH_OF_THE_BLACK_PRINCE_INTRO);
                }
            }

            void sQuestReward(Player* player, Quest const* quest, uint32 opt) override
            {
                if (quest->GetQuestId() == QUEST_FEAR_ITSELF)
                {
                    Talk(SAY_WRATHION_1_FEAR_ITSELF_OUTRO);
                }
            }

        private:

        private:

        };
};

class npc_wrathion_64822 : public CreatureScript
{
    public:

        npc_wrathion_64822() : CreatureScript("npc_wrathion_64822") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wrathion_64822AI(creature);
        }

        struct npc_wrathion_64822AI : public ScriptedAI
        {
            npc_wrathion_64822AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void sGossipHello(Player* player) override
            {
                if (HasIncompleteQuest(player, QUEST_BREATH_OF_THE_BLACK_PRINCE))
                {
                    CompleteCredit(player, QUEST_WRATHION_FINALE_MEETING_KILL_CREDIT);
                    CompleteCredit(player, QUEST_WRATHION_FINALE_KILL_CREDIT);

                    player->PlayScene(SCENE_FINAL_EFFECTS, player);
                }
            }

        private:

        private:

        };
};

class npc_wrathion_69782 : public CreatureScript
{
    public:

        npc_wrathion_69782() : CreatureScript("npc_wrathion_69782") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wrathion_69782AI(creature);
        }

        struct npc_wrathion_69782AI : public ScriptedAI
        {
            npc_wrathion_69782AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void sQuestAccept(Player* player, Quest const* quest) override
            {
                if (quest->GetQuestId() == QUEST_SECRETS_OF_THE_FIRST_EMPIRE)
                {
                    player->PlayScene(SCENE_SECRETS_INTRO, player);
                }
                else if (quest->GetQuestId() == QUEST_THE_THUNDER_FORGE)
                {
                    player->PlayScene(SCENE_FORGE_INTRO, player);
                }
            }

        private:

        private:

        };
};

class npc_wrathion_70438 : public CreatureScript
{
    public:

        npc_wrathion_70438() : CreatureScript("npc_wrathion_70438") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wrathion_70438AI(creature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            // HACK:
            if (HasIncompleteQuest(player, QUEST_THE_THUNDER_FORGE))
            {
                CompleteCredit(player, QUEST_UNLOCK_FORGE_KILL_CREDIT);
            }

            if (!HasIncompleteQuest(player, QUEST_THE_THUNDER_FORGE))
                return true;

            return false;
        }

        bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/)
        {
            player->CLOSE_GOSSIP_MENU();

            if (!HasIncompleteQuest(player, QUEST_THE_THUNDER_FORGE))
                return true;

            if (player->GetGroup())
                return true;

            LfgDungeonSet newDungeons;
            newDungeons.insert(615);
            
            std::string comment = "";

            sLFGMgr->Join(player, ROLE_DAMAGE, newDungeons, comment);

            return false;
        }

        struct npc_wrathion_70438AI : public ScriptedAI
        {
            npc_wrathion_70438AI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

        private:

        private:

        };
};

class npc_wrathion_70100 : public CreatureScript
{
    public:

        npc_wrathion_70100() : CreatureScript("npc_wrathion_70100") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wrathion_70100AI(creature);
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (!HasIncompleteQuest(player, QUEST_THE_THUNDER_FORGE))
                return false;

            return false;
        }

        bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/)
        {
            player->CLOSE_GOSSIP_MENU();

            if (!HasIncompleteQuest(player, QUEST_THE_THUNDER_FORGE))
                return true;

            if (HasIncompleteQuest(player, QUEST_THE_THUNDER_FORGE))
            {
                CompleteCredit(player, QUEST_COMPLETE_SCENARIO_KILL_CREDIT);
            }

            return false;
        }

        struct npc_wrathion_70100AI : public ScriptedAI
        {
            npc_wrathion_70100AI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

        private:

        private:

        };
};

void AddSC_legendary_cloak_scripts()
{
    new npc_wrathion_64616();       // 64616
    new npc_wrathion_64822();       // 64822
    new npc_wrathion_69782();       // 69782
    new npc_wrathion_70438();       // 70438
    new npc_wrathion_70100();       // 70100
}

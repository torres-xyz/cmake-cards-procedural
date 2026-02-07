#pragma once
#include <vector>
#include "card.hpp"
#include "game_turn.hpp"

enum class PlayerAction;

struct Player
{
    int id{-1};
    std::vector<Card> deck{};
    std::vector<Card> hand{};
    std::vector<Card> cardsInPlayStack{};
    std::vector<PlayerActionAndHandCardPair> availableActions{};
    PlayerActionAndHandCardPair chosenAction{};
    bool isHoldingACard{false};
    unsigned long int heldCardUid{0};
    unsigned long int hoveredCardUid{0};
    bool isHoveringOverACard{false};
};

static const std::vector<int> deckP1
{
    StringToCardId("Cancer Pagurus"),
    StringToCardId("Pierrotten"),
    StringToCardId("Malmelet"),
    StringToCardId("Twisted Gauntlet"),
    StringToCardId("Nether Thief"),
    StringToCardId("Forgotten Crew Member"),
    StringToCardId("Mygalomorphae"),
    StringToCardId("Body Plus"),
    StringToCardId("Body Plus Plus Plus"),
    StringToCardId("Mind Plus Plus"),
    StringToCardId("Soul Plus"),
    StringToCardId("Soul Plus Plus Plus"),
    StringToCardId("Body Soul Plus"),
    StringToCardId("Body Mind Plus Plus"),
    StringToCardId("Mind Soul Plus Plus"),
    StringToCardId("Draw Two")
};

static const std::vector<int> deckP2
{
    StringToCardId("Blitz Lancer"),
    StringToCardId("Sand Fang"),
    StringToCardId("Cephalopriest"),
    StringToCardId("Biohazard"),
    StringToCardId("Sand Claw"),
    StringToCardId("Sand Tusk"),
    StringToCardId("Temple Shield"),
    StringToCardId("Body Plus Plus"),
    StringToCardId("Mind Plus"),
    StringToCardId("Mind Plus Plus Plus"),
    StringToCardId("Soul Plus Plus"),
    StringToCardId("Body Mind Plus"),
    StringToCardId("Mind Soul Plus"),
    StringToCardId("Body Soul Plus Plus"),
    StringToCardId("All Plus")
};

static const std::vector<int> mulliganRequiredDeck
{
    StringToCardId("Body Plus Plus"),
    StringToCardId("Mind Plus"),
    StringToCardId("Mind Plus Plus Plus"),
    StringToCardId("Soul Plus Plus"),
    StringToCardId("Body Mind Plus"),
    StringToCardId("Mind Soul Plus"),
    StringToCardId("Body Soul Plus Plus"),
    StringToCardId("All Plus"),
    StringToCardId("Blitz Lancer"),
    StringToCardId("Sand Fang"),
    StringToCardId("Cephalopriest"),
    StringToCardId("Biohazard"),
    StringToCardId("Sand Claw"),
    StringToCardId("Sand Tusk"),
    StringToCardId("Temple Shield")
};

void InitializePlayer(Player &player);

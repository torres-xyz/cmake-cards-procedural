#pragma once
#include <random>
#include <vector>
#include "card.hpp"

struct Player
{
    int id{-1};
    int score{};
    std::vector<Card> deck{};
    std::vector<Card> hand{};
    std::vector<Card> cardsInPlayStack{};
    bool isHoldingACard{};
    int heldCardIndex{-1};
    unsigned long int hoveredCardUid{};
    bool isHoveringOverACard{false};
    bool hasDrawnThisTurn{false};
    bool hasEndedTheTurn{false};
    int cardsPlayed{0};
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
    StringToCardId("Mind Soul Plus Plus")
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

void InitializePlayerWithAdvancedDeck(Player &player, std::random_device &rd);

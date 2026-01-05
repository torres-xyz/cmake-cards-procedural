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
    int hoveredCardIndex{-1};
    bool hasDrawnThisTurn{false};
    bool hasEndedTheTurn{false};
    int cardsPlayed{0};
};

void InitializePlayerWithAdvancedDeck(Player &player, std::random_device &rd);

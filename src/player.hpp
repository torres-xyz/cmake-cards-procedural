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
    Card cardInPlay{};
    bool isHoldingACard{};
    int heldCardIndex{-1};
};

void InitializePlayerWithAdvancedDeck(Player &player, std::random_device &rd);

#pragma once
#include <vector>
#include "card.hpp"

namespace std
{
    class random_device;
}

struct Player
{
    int id{-1};
    int score{};
    std::vector<Card> deck{};
    std::vector<Card> hand{};
    Card cardInPlay{};
    int heldCardIndex{-1};
};

void InitializePlayer(Player &player, std::random_device &rd);

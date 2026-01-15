#pragma once
#include <vector>

struct GameStatus
{
    int roundsPlayed{};
    std::vector<int> roundWinnerHistory{};
};

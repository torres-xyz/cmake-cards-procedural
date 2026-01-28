#pragma once
#include <string>
#include <vector>
#include "game_turn.hpp"


enum class PlayerAction;
struct ActionLog;

struct GameStatus
{
    int currentTurnOwner{};
    int turnsPlayed{};
    int roundsPlayed{};
    std::vector<int> roundWinnerHistory{};
    std::vector<ActionLog> actionLogs{};
};

struct ActionLog
{
    PlayerActionAndHandCardPair actionCardPairTaken{};
    int turnNumber{};
    int roundNumber{};
    int playerID{};
    std::string details{};
};

std::string ActionLogEntryToString(ActionLog log);

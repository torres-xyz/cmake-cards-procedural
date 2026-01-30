#pragma once
#include <string>
#include <vector>

#include "game_turn.hpp"

enum class TurnPhase;

struct ActionLog
{
    PlayerActionAndHandCardPair actionCardPairTaken{};
    TurnPhase turnPhase{};
    int turnNumber{};
    int roundNumber{};
    int playerID{};
};

struct GameStatus
{
    int currentTurnOwner{};
    int turnsPlayed{};
    int roundsPlayed{};
    std::vector<int> roundWinnerHistory{};
    std::vector<ActionLog> actionLogs{};
};


std::string ActionLogEntryToString(ActionLog log);

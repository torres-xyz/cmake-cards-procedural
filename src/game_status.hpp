#pragma once
#include <string>
#include <vector>

#include "game_turn.hpp"

enum class GameplayPhase;
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
    int pointsPlayer1{};
    int pointsPlayer2{};
    std::vector<int> roundWinnerHistory{};
    std::vector<ActionLog> actionLogs{};
    bool gameIsOver{false};
};

void ResetGame(Player &player1, Player &player2, GameStatus &gameStatus, TurnPhase &turnPhase);

std::string ActionLogEntryToString(const ActionLog &log);

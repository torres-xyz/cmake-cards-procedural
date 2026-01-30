#pragma once
#include <string>
#include <vector>

#include "card.hpp"

struct PlayerActionAndHandCardPair;
struct GameStatus;
struct GameRules;
struct Player;

enum class TurnPhase
{
    initialSetup,
    drawStep,
    mainPhase, //playing cards
    endPhase
};

enum class PlayerAction
{
    null,
    drawCard,
    mulligan,
    shuffleHandIntoDeck,
    shuffleDeck,
    playCard,
    passTheTurn,
    forfeit,
};

struct PlayerActionAndHandCardPair
{
    PlayerAction action{};
    // Card playableHandCard{};
    Card card{};
    //In the future, this could also include usable cards in the playfield that the player can activate
};


std::vector<PlayerActionAndHandCardPair> CalculateAvailableActions(const Player &player, const TurnPhase &turnPhase, const GameRules &gameRules);

void ExecuteTurn(Player &player, TurnPhase &currentTurnPhase, const GameRules &gameRules, GameStatus &gameStatus);

void ExecuteChosenPlayerAction(Player &player, const TurnPhase &turnPhase, GameStatus &gameStatus);

std::string PlayerActionToString(PlayerAction playerAction);

std::string TurnPhaseToString(TurnPhase phase);

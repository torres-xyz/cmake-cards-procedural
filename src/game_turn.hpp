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
    mainPhase, //playing cards
    endTurnPhase,
    battlePhase,
    endRoundPhase
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
    finishRound,
    forfeitTheRound,
    forfeitTheGame,
};

struct PlayerActionAndHandCardPair
{
    PlayerAction action{};
    Card card{};
    //In the future, this could also include usable cards in the playfield that the player can activate
};


std::vector<PlayerActionAndHandCardPair> CalculateAvailableActions(const Player &player, const TurnPhase &turnPhase, const GameRules &gameRules, const GameStatus &gameStatus);

void ExecuteTurn(Player &player, Player &opponentPlayer, TurnPhase &currentTurnPhase, const GameRules &gameRules, GameStatus &gameStatus);

void ExecuteChosenPlayerAction(Player &player, Player &opponentPlayer, TurnPhase &currentTurnPhase, const GameRules &gameRules, GameStatus &gameStatus);

void ExecutePlayedCardEffects(int cardID, Player &player, Player &opponent, TurnPhase &turnPhase, const GameRules &gameRules, const GameStatus &gameStatus);

std::string PlayerActionToString(PlayerAction playerAction);

std::string TurnPhaseToString(TurnPhase phase);

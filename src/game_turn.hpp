#pragma once
#include <string>
#include <vector>

struct GameStatus;
struct GameRules;
struct Player;
enum class PlayerAction;

enum class TurnPhase
{
    initialSetup,
    startOfTheTurnDraw,
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
    PlayerAction action;
    unsigned long int playableHandCardsUid;
    //In the future, this could also include usable cards in the playfield that the player can activate
};

std::vector<PlayerActionAndHandCardPair> CalculateAvailableActions(const Player &player, const TurnPhase &turnPhase, const GameRules &gameRules);

void ExecuteTurn(Player &player, TurnPhase &currentTurnPhase, const GameRules &gameRules, GameStatus &gameStatus);

void ExecutePlayerAction(Player &player, GameStatus &gameStatus);

std::string PlayerActionToString(PlayerActionAndHandCardPair actionCardPair);

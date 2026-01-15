#pragma once
#include <random>

struct GameStatus;
struct GameRules;
struct PlayingScene;
struct Card;
struct Player;

enum class GameplayPhase
{
    uninitialized,
    initialHandDraw,
    playerOneDrawing,
    playerTwoDrawing,
    playerOneFirstTurn,
    playerTwoFirstTurn,
    playerOnePlayingAndOpponentPlayed,
    playerTwoPlayingAndOpponentPlayed,
    playerOnePlayingAndOpponentPassed,
    playerTwoPlayingAndOpponentPassed,
    battle,
    endPhase,
    gameOver
};

void UpdateGameplayPhases(PlayingScene &playingScene, GameplayPhase &currentPhase, GameStatus &gameStatus,
                          Player &player1, Player &player2, const GameRules &gameRules, std::random_device &rd);

void DrawCardsFromDeckToHand(Player &player, int amount);

void PutCardInPlay(Player &player);

std::string GameplayPhaseToString(GameplayPhase phase);

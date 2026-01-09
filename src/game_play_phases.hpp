#pragma once
#include <random>

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
    gameOver
};

void UpdateGameplayPhases(PlayingScene &playingScene, GameplayPhase &currentPhase, Player &player1, Player &player2,
                          GameRules gameRules, std::random_device &rd);

void DrawCardsFromDeckToHand(Player &player, int amount);

void PutCardInPlay(Player &player);

std::string GameplayPhaseToString(GameplayPhase phase);

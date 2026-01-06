#pragma once
#include <random>
#include <string>

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

void UpdateGameplayPhases(GameplayPhase &currentPhase, Player &player1, Player &player2,
                          int playerGoingFirst, std::random_device &rd);

void UpdateGameplayPhases(PlayingScene &playingScene, GameplayPhase &currentPhase, Player &player1, Player &player2,
                          int playerGoingFirst, std::random_device &rd);

void DrawCardsFromDeckToHand(Player &player, int amount);

void PutCardInPlay(Player &player);

std::string GameplayPhaseToString(GameplayPhase phase);

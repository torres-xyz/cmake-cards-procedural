#pragma once
#include <random>
#include <string>

struct Card;
struct Player;

enum class GameplayPhase
{
    uninitialized,
    initialHandDraw,
    playerOneDrawing,
    playerOnePlaying,
    playerTwoDrawing,
    playerTwoPlaying,
    battle,
    gameOver
};

void UpdateGameplayPhases(GameplayPhase &currentPhase, Player &player1, Player &player2,
                          int goingFirst, std::random_device &rd);

void DrawCardsFromDeckToHand(Player &player, int amount);

void PutCardInPlay(Player &player);

int BattleCards(const std::vector<Card> &cardStack1, const std::vector<Card> &cardStack2);

std::string GameplayPhaseToString(GameplayPhase phase);

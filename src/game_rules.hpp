#pragma once
#include <random>
#include <vector>

#include "card.hpp"

struct GameStatus;
enum class GameplayPhase;
struct Card;
struct Player;

struct GameRules
{
    const int playerGoingFirst{2}; // CPU plays first
    const int pointsNeededToWin{2}; //Best of 3 rounds game as a default
    const int initialHandSize{4};
};

void ShuffleDeckAndMakeSureTopCardIsAUnit(std::vector<Card> &deck, std::random_device &rd);

CardStats CalculateCardStackTotalStats(const std::vector<Card> &stack);

int CalculateRoundWinnerId(const Player &playerA, const Player &playerB);

bool IsInitialHandValid(const std::vector<Card> &playerHand, GameRules gameRules);

int HasAPlayerWon(const GameStatus &gameStatus, const GameRules &gameRules);

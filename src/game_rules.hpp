#pragma once
#include <random>
#include <vector>

enum class GameplayPhase;
struct Card;
struct Player;

struct GameRules
{
    int playerGoingFirst{2}; // CPU plays first
    int pointsNeededToWin{2}; //Best of 3 rounds game as a default
};

void ShuffleDeckAndMakeSureTopCardIsAUnit(std::vector<Card> &deck, std::random_device &rd);

int GetCardStackTotalBody(const std::vector<Card> &stack);

int GetCardStackTotalMind(const std::vector<Card> &stack);

int GetCardStackTotalSoul(const std::vector<Card> &stack);

int CalculateRoundWinner(const std::vector<Card> &stack1, const std::vector<Card> &stack2);

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, const GameplayPhase &gameplayPhase);

bool IsInitialHandValid(const Player &player);

#pragma once
#include <vector>

enum class GameplayPhase;
struct Card;
struct Player;

int GetCardStackTotalBody(const std::vector<Card> &stack);

int GetCardStackTotalMind(const std::vector<Card> &stack);

int GetCardStackTotalSoul(const std::vector<Card> &stack);

int CalculateRoundWinner(const std::vector<Card> &stack1, const std::vector<Card> &stack2);

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, const GameplayPhase &gameplayPhase);

bool IsInitialHandValid(const Player &player);

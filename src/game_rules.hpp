#pragma once

enum class GameplayPhase;
struct Card;
struct Player;

int CalculateRoundWinner(const Player &player1, const Player &player2);

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, const GameplayPhase &gameplayPhase);

bool IsInitialHandValid(const Player &player);

#pragma once

enum class GameplayPhase;
struct Card;
struct Player;

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, const GameplayPhase &gameplayPhase);

bool IsInitialHandValid(const Player &player);

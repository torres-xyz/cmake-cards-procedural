#pragma once

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
    battle
};

void UpdateGameplayPhases(GameplayPhase &currentPhase, Player &player1, Player &player2, int goingFirst);

void DrawCardsFromDeckToHand(Player &player, int amount);

void PutCardInPlay(Player &player);

int BattleCards(const Card &card1, const Card &card2);

#pragma once

enum class TurnPhase;
struct GameStatus;
struct Player;
enum class GameplayPhase;

namespace ImGuiSideBar
{
    void DrawSideBar(bool &muteGame, const TurnPhase &currentTurnPhase, GameplayPhase &currentPhase, GameStatus &gameStatus, const Player &player1, const Player &player2);
}

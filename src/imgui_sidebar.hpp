#pragma once

struct GameStatus;
struct Player;
enum class GameplayPhase;

namespace ImGuiSideBar
{
    void DrawSideBar(bool &muteGame, GameplayPhase &currentPhase, GameStatus &gameStatus, const Player &player1, const Player &player2);
}

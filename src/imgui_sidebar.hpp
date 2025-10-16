#pragma once

struct Player;
enum class GameplayPhase;

namespace ImGuiSideBar
{
    void DrawSideBar(bool &muteGame, const GameplayPhase &currentPhase,
                     const Player &player1, const Player &player2);
}

#pragma once

enum class GameplayPhase;

namespace ImGuiSideBar
{
    void DrawSideBar(bool &muteGame, const GameplayPhase &currentPhase);
}

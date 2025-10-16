#include "imgui_sidebar.hpp"
#include "imgui.h"
#include "rlImGui.h"
#include "raylib-cpp.hpp"
#include "constants.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"

namespace ImGuiSideBar
{
    void DrawSideBar(bool &muteGame, const GameplayPhase &currentPhase,
                     const Player &player1, const Player &player2)
    {
        // start ImGui Content
        rlImGuiBegin();

        // show ImGui Content
        bool open = false;
        ImGui::SetNextWindowPos(ImVec2(constants::windowScreenWidth - constants::imguiSideBarWidth,
                                       0));
        ImGui::SetNextWindowSize(
            ImVec2(constants::imguiSideBarWidth, constants::windowScreenHeight));
        if (ImGui::Begin("Test Window", &open,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::TextUnformatted(("FPS: " + std::to_string(GetFrameTime())).c_str());
            ImGui::TextUnformatted(("Mouse X: " + std::to_string(GetMouseX())
                                    + " / Mouse Y: " + std::to_string(GetMouseY())).c_str());
            ImGui::SeparatorText("Game State");
            ImGui::TextUnformatted(("Game Phase = " + GameplayPhaseToString(currentPhase)).c_str());
            ImGui::TextUnformatted(("Player 1 score = " + std::to_string(player1.score)).c_str());
            ImGui::TextUnformatted(("Player 2 score = " + std::to_string(player2.score)).c_str());
            ImGui::SeparatorText("Options");
            ImGui::Checkbox("Mute", &muteGame);
            ImGui::SeparatorText("Debug Actions");
            ImGui::TextUnformatted("My Text inside the window");
        }
        ImGui::End();

        // ImGui::ShowDemoWindow();
        // end ImGui Content
        rlImGuiEnd();
    }
}

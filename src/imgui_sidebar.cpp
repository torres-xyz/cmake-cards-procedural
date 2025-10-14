#include "imgui_sidebar.hpp"
#include "imgui.h"
#include "rlImGui.h"
#include "raylib-cpp.hpp"
#include "constants.hpp"

namespace ImGuiSideBar
{
    void DrawSideBar()
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
            ImGui::TextUnformatted(
                ("FPS: " + std::to_string(GetFrameTime())).c_str());
            ImGui::TextUnformatted(
                ("Mouse X: " + std::to_string(GetMouseX())
                 + " / Mouse Y: " + std::to_string(GetMouseY())).c_str());
            ImGui::SeparatorText("Debug Actions");
            ImGui::TextUnformatted("My Text inside the window");
        }
        ImGui::End();

        // end ImGui Content
        rlImGuiEnd();
    }
}

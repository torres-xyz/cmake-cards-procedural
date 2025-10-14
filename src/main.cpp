#include <iostream>
#include <random> // for std::random_device
#include "constants.hpp"
// #include "raylib.h"
#include "raylib-cpp.hpp"
#include "imgui.h"
#include "imgui_sidebar.hpp"
#include "rlImGui.h"

int main()
{
    // Initialization ----------------------------------------------------------
    std::random_device rd{};
    SetRandomSeed(rd());

    SetConfigFlags(FLAG_VSYNC_HINT);
    raylib::Window window(constants::windowScreenWidth, constants::windowScreenHeight,
                          "Raylib-cpp Template");

    SetTargetFPS(GetMonitorRefreshRate(0));

#if (DEBUG)
    rlImGuiSetup(true);
#endif

    const raylib::Texture2D mewTex("resources/mew.png");


    // Main game loop
    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        // Update --------------------------------------------------------------


        // Draw ----------------------------------------------------------------
        window.ClearBackground(RAYWHITE);

#if (DEBUG)
        ImGuiSideBar::DrawSideBar();
#endif

        mewTex.Draw(constants::screenWidth / 2 - mewTex.width / 2,
                    constants::screenHeight / 2 - mewTex.height / 2,
                    WHITE);


        EndDrawing();
    }

    // De-Initialization -------------------------------------------------------
    // UnloadTexture() and CloseWindow() are called automatically.

    return 0;
}

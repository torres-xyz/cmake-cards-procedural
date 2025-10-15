#include <iostream>
#include <random> // for std::random_device
#include "constants.hpp"
// #include "raylib.h"
#include <map>
#include "raylib-cpp.hpp"
#include "imgui.h"
#include "imgui_sidebar.hpp"
#include "rlImGui.h"
#include "button.hpp"
#include "audio.hpp"

enum class GameTexture
{
    invalid,
    metal08,
    metal22,
    metal35,
    panel01,
    panel05,
    cardBack,
    paperCard,
    rockCard,
    scissorsCard
};


struct Scene
{
    GameTexture background{};
    GameMusic music{};
};


raylib::Texture2D &GetTexture(const GameTexture tex)
{
    static const std::map<GameTexture, std::string> gameTextureToPathMap
    {
        {GameTexture::metal08, "resources/textures/metal_08.jpg"},
        {GameTexture::metal22, "resources/textures/metal_22.jpg"},
        {GameTexture::metal35, "resources/textures/metal_35.jpg"},
        {GameTexture::panel01, "resources/textures/panel_01.png"},
        {GameTexture::panel05, "resources/textures/panel_05.png"},
        {GameTexture::cardBack, "resources/textures/cards/card_back.png"},
        {GameTexture::paperCard, "resources/textures/cards/paper_card.png"},
        {GameTexture::rockCard, "resources/textures/cards/rock_card.png"},
        {GameTexture::scissorsCard, "resources/textures/cards/scissors_card.png"},
    };
    static std::map<GameTexture, raylib::Texture2D> gameTextureToTexture2DMap{};

    if (!gameTextureToTexture2DMap.contains(tex))
    {
        gameTextureToTexture2DMap.insert(
            {
                tex,
                raylib::Texture2D(gameTextureToPathMap.at(tex))
            });
    }

    return gameTextureToTexture2DMap.at(tex);
};


int main()
{
    // Initialization ----------------------------------------------------------
    std::random_device rd{};
    SetRandomSeed(rd());

    SetConfigFlags(FLAG_VSYNC_HINT);
    raylib::Window window(constants::windowScreenWidth, constants::windowScreenHeight, "Raylib-cpp Template");
    SetTargetFPS(GetMonitorRefreshRate(0));
#if (DEBUG)
    rlImGuiSetup(true);
#endif
    InitAudioDevice();
    bool muteGame{true};

    //Scene 1
    constexpr Scene startScene
    {
        .background = GameTexture::metal35,
        .music = GameMusic::start
    };
    constexpr int startGameButtonWidth{200};
    constexpr int startGameButtonHeight{100};
    Button startButton
    {
        .rectangle = raylib::Rectangle
        {
            constants::screenWidth * 0.5f - static_cast<float>(startGameButtonWidth) * 0.5f,
            constants::screenHeight * 0.5f - static_cast<float>(startGameButtonHeight) * 0.5f,
            static_cast<float>(startGameButtonWidth),
            static_cast<float>(startGameButtonHeight)
        },
        .text = "Star Game",
        .fontSize = 20,
        .background = GameTexture::panel01,
        .state = ButtonState::enabled
    };
    //Scene 1 end

    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        Vector2 mousePosition = GetMousePosition();
        SetMasterVolume(!muteGame);

        // Update --------------------------------------------------------------
        //Updating Scene 1
        UpdateButtonState(startButton,
                          mousePosition,
                          IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                          IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
        if (startButton.wasPressed)
        {
            PlaySound(GameSound::buttonPress01);
        }
        PlayMusic(startScene.music);

        // Draw ----------------------------------------------------------------
        window.ClearBackground(RAYWHITE);

        //Drawing Scene 1
        GetTexture(startScene.background).Draw();
        DrawButton(startButton, GetTexture(startButton.background));


#if (DEBUG)
        ImGuiSideBar::DrawSideBar(muteGame);
#endif
        EndDrawing();

        // De-Initialization -------------------------------------------------------
        // UnloadTexture() and CloseWindow() are called automatically.
    };


    return 0;
}

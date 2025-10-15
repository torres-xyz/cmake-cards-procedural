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
    wall13,
    panel01,
    panel05,
    cardBack,
    paperCard,
    rockCard,
    scissorsCard
};

enum class GameScene
{
    invalid,
    start,
    playing,
    gameOver
};

enum class CardType
{
    invalid,
    rock,
    paper,
    scissors
};

struct Scene
{
    GameScene gameScene{};
    GameTexture background{};
    GameMusic music{};
};

struct Card
{
    raylib::Vector2 size{};
    raylib::Vector2 pos{};
    CardType type{};
    bool faceUp{true};
};

struct Player
{
    int id{-1};
    int score{};
    std::vector<Card> deck{};
    std::vector<Card> hand{};
};

void InitializePlayer(Player &player, std::random_device &rd)
{
    player.score = 0;
    player.hand.clear();
    //Use default deck for players for now.
    player.deck.clear();
    for (const int cardType: constants::defaultDeck)
    {
        player.deck.emplace_back(Card{.type = static_cast<CardType>(cardType)});
    }

    std::shuffle(player.deck.begin(), player.deck.end(), rd);
}


raylib::Texture2D &GetTexture(const GameTexture tex)
{
    static const std::map<GameTexture, std::string> gameTextureToPathMap
    {
        {GameTexture::metal08, "resources/textures/metal_08.jpg"},
        {GameTexture::metal22, "resources/textures/metal_22.jpg"},
        {GameTexture::metal35, "resources/textures/metal_35.jpg"},
        {GameTexture::wall13, "resources/textures/wall_13.png"},
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

    GameScene currentScene{GameScene::start};

    //Player 1
    Player player1
    {
        .id = 1,
        .score = 0
    };
    Player player2
    {
        .id = 2,
        .score = 0
    };

    InitializePlayer(player1, rd);
    InitializePlayer(player2, rd);

    // int playerGoingFirst{2};


    //GameScene::start ---------------------------------------------------------
    constexpr Scene startScene
    {
        .gameScene = GameScene::start,
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
    //GameScene::start end -----------------------------------------------------
    //GameScene::playing -------------------------------------------------------
    constexpr Scene playingScene
    {
        .gameScene = GameScene::playing,
        .background = GameTexture::wall13,
        .music = GameMusic::playing

    };

    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        Vector2 mousePosition = GetMousePosition();
        SetMasterVolume(!muteGame);

        // Update --------------------------------------------------------------
        switch (currentScene)
        {
            case GameScene::invalid:
                break;
            case GameScene::start:
            {
                UpdateButtonState(startButton,
                                  mousePosition,
                                  IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                                  IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
                if (startButton.wasPressed)
                {
                    PlaySound(GameSound::buttonPress01);
                    currentScene = GameScene::playing;
                }
                PlayMusic(startScene.music);
                break;
            }
            case GameScene::playing:
            {
                PlayMusic(playingScene.music);
                break;
            }
            case GameScene::gameOver:
                break;
        }
        //Updating Scene 1

        // Draw ----------------------------------------------------------------
        window.ClearBackground(RAYWHITE);

        switch (currentScene)
        {
            case GameScene::invalid:
                break;
            case GameScene::start:
            {
                GetTexture(startScene.background).Draw();
                DrawButton(startButton, GetTexture(startButton.background));

                break;
            }
            case GameScene::playing:
            {
                GetTexture(playingScene.background).Draw();

                break;
            }
            case GameScene::gameOver:
                break;
        }


#if (DEBUG)
        ImGuiSideBar::DrawSideBar(muteGame);
#endif
        EndDrawing();

        // De-Initialization -------------------------------------------------------
        // UnloadTexture() and CloseWindow() are called automatically.
    };


    return 0;
}

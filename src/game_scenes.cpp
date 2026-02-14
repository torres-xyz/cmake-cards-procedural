#include "game_scenes.hpp"

#include <algorithm>
#include <cassert>
#include <format>

#include "audio.hpp"
#include "constants.hpp"
#include "cpu_brain.hpp"
#include "debug_globals.hpp"
#include "player.hpp"
#include "textures.hpp"
#include "helper_functions.hpp"
#include "fonts.hpp"
#include "game_rules.hpp"
#include "game_status.hpp"
#include "game_turn.hpp"

struct GameStatus;

void RunStartingScene(StartingScene &startingScene, GameScene &currentScene, Player &player1, Player &player2, GameStatus &gameStatus)
{
    const raylib::Vector2 mousePosition = GetMousePosition();
    Button &startButton = startingScene.startButton;

    // Setup background shader
    // Load shader and setup location points and values
    // note: vsFileName means vertex shader. For this effect, we are only using the Fragment Shader
    static const Shader waveShader = LoadShader(nullptr, "resources/shaders/wave.frag");

    static const int secondsLoc = GetShaderLocation(waveShader, "seconds");
    static float seconds{};

    if (static bool initShader{false}; initShader == false)
    {
        // Shader uniform values that can be updated at any time
        const float screenSize[2] = {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
        constexpr float freqX = 5.0f;
        constexpr float freqY = 5.0f;
        constexpr float ampX = 5.0f;
        constexpr float ampY = 5.0f;
        constexpr float speedX = 2.0f;
        constexpr float speedY = 2.0f;

        static const int sizeLoc = GetShaderLocation(waveShader, "size");
        static const int freqXLoc = GetShaderLocation(waveShader, "freqX");
        static const int freqYLoc = GetShaderLocation(waveShader, "freqY");
        static const int ampXLoc = GetShaderLocation(waveShader, "ampX");
        static const int ampYLoc = GetShaderLocation(waveShader, "ampY");
        static const int speedXLoc = GetShaderLocation(waveShader, "speedX");
        static const int speedYLoc = GetShaderLocation(waveShader, "speedY");

        SetShaderValue(waveShader, sizeLoc, &screenSize, SHADER_UNIFORM_VEC2);
        SetShaderValue(waveShader, freqXLoc, &freqX, SHADER_UNIFORM_FLOAT);
        SetShaderValue(waveShader, freqYLoc, &freqY, SHADER_UNIFORM_FLOAT);
        SetShaderValue(waveShader, ampXLoc, &ampX, SHADER_UNIFORM_FLOAT);
        SetShaderValue(waveShader, ampYLoc, &ampY, SHADER_UNIFORM_FLOAT);
        SetShaderValue(waveShader, speedXLoc, &speedX, SHADER_UNIFORM_FLOAT);
        SetShaderValue(waveShader, speedYLoc, &speedY, SHADER_UNIFORM_FLOAT);

        initShader = true;
    }

    // region UPDATE

    //Doing this to avoid a stutter the first time the game loads.
    if (seconds == 0) seconds += std::min(GetFrameTime(), 1.f / static_cast<float>(GetMonitorRefreshRate(0)));
    else seconds += GetFrameTime();

    SetShaderValue(waveShader, secondsLoc, &seconds, SHADER_UNIFORM_FLOAT);

    // Update Buttons
    UpdateButtonState(startButton,
                      mousePosition,
                      IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                      IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
    if (startButton.wasPressed)
    {
        // Initialize the gameplay components
        InitializePlayer(player1, deckP1);
        InitializePlayer(player2, deckP2);
        gameStatus.currentTurnOwner = GetRandomValue(1, 2);
#if (DEBUG)
        gameStatus.currentTurnOwner = debugGlobals::g_debug_startingPlayer;
#endif


        PlaySound(GameSound::buttonPress01);
        currentScene = GameScene::playing;
    }

    // Update Music
    PlayMusic(startingScene.music);

    //endregion Update

    //Draw
    BeginShaderMode(waveShader);

    GetTexture(startingScene.background).Draw(-20, -20);

    EndShaderMode();

    DrawButton(startButton, GetTexture(startButton.background));
}

void RunPlayingScene(PlayingScene &playingScene, GameScene &currentScene, TurnPhase &currentTurnPhase, GameStatus &gameStatus, Player &player1, Player &player2, const GameRules &gameRules)
{
    // region Lambdas
    auto UpdateSceneButton = [](Button &button, Player &player, const PlayerAction playerAction) -> void {
        button.state = ButtonState::disabled;

        bool playerHasAvailableAction{false};
        for (const auto &[action, card]: player.availableActions)
        {
            if (action == playerAction) playerHasAvailableAction = true;
        }

        if (playerHasAvailableAction)
        {
            button.state = ButtonState::enabled;
            UpdateButtonState(button, GetMousePosition(), IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

            if (button.wasPressed)
            {
                PlaySound(GameSound::buttonPress01);
                player.chosenAction.action = playerAction;
            }
        }
    };
    auto UpdatePlayStackCards = [](Player &player)-> void {
        for (size_t i = 0; i < player.cardsInPlayStack.size(); ++i)
        {
            player.cardsInPlayStack.at(i).faceUp = true;

            constexpr float verticalOffset = 30;
            if (player.id == 1)
            {
                player.cardsInPlayStack.at(i).rect = constants::playerOnePlayfieldCardZoneRect;
            }
            else if (player.id == 2)
            {
                player.cardsInPlayStack.at(i).rect = constants::playerTwoPlayfieldCardZoneRect;
            }

            player.cardsInPlayStack.at(i).rect.y += verticalOffset * static_cast<float>(i);
        }
    };
    auto PlayerHasAvailableAction = [](const Player &player, const PlayerAction &action)-> bool {
        return std::ranges::any_of(
            player.availableActions,
            [action](const PlayerActionAndHandCardPair &actionCardPair) {
                return actionCardPair.action == action;
            });
    };
    //endregion Lambdas

    // Static Variables
    static raylib::Vector2 heldCardOffset{};

    // UPDATE ------------------------------------------------------------------
    PlayMusic(playingScene.music);

    if (gameStatus.gameIsOver)
    {
        currentScene = GameScene::gameOver;
    }

    // Picking up a card from the Hand and Holding it.
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        for (size_t i = 0; i < player1.hand.size(); ++i)
        {
            raylib::Vector2 mousePos = GetMousePosition();

            //Move the card we are holding with the mouse
            if (player1.heldCardUid == player1.hand.at(i).uid)
            {
                player1.hand.at(i).rect.SetPosition(mousePos - heldCardOffset);
                player1.hand.at(i).rect.SetSize(constants::inHandCardWidth, constants::inHandCardHeight);
                continue;
            }

            //If trying to click and hold a card
            if (HelperFunctions::CheckCollisionPointCard(mousePos, player1.hand.at(i)) &&
                !player1.isHoldingACard)
            {
                player1.heldCardUid = player1.hand.at(i).uid;
                player1.isHoldingACard = true;

                heldCardOffset = mousePos - player1.hand.at(i).rect.GetPosition();
            }
        }
    }
    // Trying To Play Held Card
    else
    {
        if (player1.isHoldingACard && CheckCollisionPointRec(GetMousePosition(), constants::playfieldRec))
        {
            for (const PlayerActionAndHandCardPair &availableAction: player1.availableActions)
            {
                if (availableAction.card.uid == player1.heldCardUid)
                {
                    player1.chosenAction = availableAction;
                }
            }
        }
        player1.heldCardUid = 0;
        player1.isHoldingACard = false;
    }

    // Execute the player's turns
    if (gameStatus.currentTurnOwner == 1)
    {
        player1.availableActions = CalculateAvailableActions(player1, currentTurnPhase, gameRules, gameStatus);
        player2.availableActions.clear(); // No actions for the other player

        //Enable to have two CPUs play against each other.
        // RunCpuBrain(player1, constants::cpuPlayerOptions);

        ExecuteTurn(player1, player2, currentTurnPhase, gameRules, gameStatus);
    }
    if (gameStatus.currentTurnOwner == 2)
    {
        player2.availableActions = CalculateAvailableActions(player2, currentTurnPhase, gameRules, gameStatus);
        player1.availableActions.clear(); // No actions for the other player

        RunCpuBrain(player2, constants::cpuPlayerOptions);

        ExecuteTurn(player2, player1, currentTurnPhase, gameRules, gameStatus);
    }

    // region Update the button states
    if (PlayerHasAvailableAction(player1, PlayerAction::mulligan))
    {
        UpdateSceneButton(playingScene.mulliganButton, player1, PlayerAction::mulligan);
    }
    else
    {
        UpdateSceneButton(playingScene.playerDeckButton, player1, PlayerAction::drawCard);
    }

    if (currentTurnPhase == TurnPhase::endRoundPhase)
    {
        //Player 1 controls both player's ability to end the round.
        if (gameStatus.currentTurnOwner == 1)
        {
            UpdateSceneButton(playingScene.nextRoundButton, player1, PlayerAction::finishRound);
        }
        if (!constants::cpuPlayerOptions.autoStartNewRound && gameStatus.currentTurnOwner == 2)
        {
            UpdateSceneButton(playingScene.nextRoundButton, player2, PlayerAction::finishRound);
        }
    }

    UpdateSceneButton(playingScene.endTurnButton, player1, PlayerAction::passTheTurn);

    //Special button - Undo Action
    //// Undo-able actions: Play Card
    playingScene.undoPlayerActionButton.state = ButtonState::disabled;
    if (!gameStatus.actionLogs.empty())
    {
        if (gameStatus.actionLogs.back().playerID == player1.id &&
            gameStatus.actionLogs.back().actionCardPairTaken.action == PlayerAction::playCard)
        {
            playingScene.undoPlayerActionButton.state = ButtonState::enabled;
            UpdateButtonState(playingScene.undoPlayerActionButton, GetMousePosition(), IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

            if (playingScene.undoPlayerActionButton.wasPressed)
            {
                player1.hand.emplace_back(player1.cardsInPlayStack.back());
                player1.cardsInPlayStack.erase(player1.cardsInPlayStack.end() - 1);

                gameStatus.actionLogs.erase(gameStatus.actionLogs.end() - 1);


                PlaySound(GameSound::buttonPress01);
            }
        }
    }
    //endregion Update the button states

    // Update Player 1's hand cards. Put them in place, slightly apart from each other.
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        const int int_i = static_cast<int>(i);
        //except for the held card.
        if (player1.hand.at(i).uid == player1.heldCardUid) continue;

        constexpr int spaceBetweenCards{7};
        const raylib::Rectangle newCardRect
        {
            static_cast<float>(constants::handZonePosX + spaceBetweenCards * int_i + 1 + constants::inHandCardWidth * int_i),
            static_cast<float>(constants::handZonePosY + 4),
            constants::inHandCardWidth,
            constants::inHandCardHeight
        };

        player1.hand.at(i).rect = newCardRect;
        player1.hand.at(i).faceUp = true;
    }

    // Update Player 2's hand cards. Put them in place, slightly apart from each other.
    for (std::size_t i = 0; i < player2.hand.size(); ++i)
    {
        const auto float_i = static_cast<float>(i);

        //Set cards in the hand slightly apart from each other.
        const raylib::Rectangle newCardRect
        {
            constants::player2HandZone.x + 2 * float_i + 1 + constants::cardWidth * float_i,
            constants::player2HandZone.y + 4,
            constants::cardWidth,
            constants::cardHeight
        };

        player2.hand.at(i).rect = newCardRect;
        player2.hand.at(i).faceUp = false;
    }

    // Update Players stacks
    UpdatePlayStackCards(player1);
    UpdatePlayStackCards(player2);

    // Update Hovered Card
    player1.isHoveringOverACard = false;
    if (player1.isHoldingACard == false)
    {
        auto CheckHoveringCards = [](Player &p1, const std::vector<Card> &cards)-> void {
            for (const auto &card: cards)
            {
                //Handle hovering
                if (HelperFunctions::CheckCollisionPointCard(GetMousePosition(), card))
                {
                    p1.hoveredCardUid = card.uid;
                    p1.isHoveringOverACard = true;
                }
            }
        };
        CheckHoveringCards(player1, player1.hand);
        CheckHoveringCards(player1, player1.cardsInPlayStack);
        CheckHoveringCards(player1, player2.cardsInPlayStack);
    }
    // DRAW ---------------------------------------------------------------------
    GetTexture(playingScene.background).Draw();

    // Draw Buttons
    DrawButton(playingScene.endTurnButton, GetTexture(playingScene.endTurnButton.background));

    if (PlayerHasAvailableAction(player1, PlayerAction::mulligan))
    {
        DrawButton(playingScene.mulliganButton, GetTexture(playingScene.mulliganButton.background));
    }
    else
    {
        DrawButton(playingScene.playerDeckButton, GetTexture(playingScene.playerDeckButton.background));
    }
    DrawButton(playingScene.undoPlayerActionButton, GetTexture(playingScene.undoPlayerActionButton.background));


    // Draw turn indicator
    std::string turnIndicatorMessage{gameStatus.currentTurnOwner == 1 ? "Your Turn" : "CPU Turn"};
    raylib::Color turnIndicatorTextColor{gameStatus.currentTurnOwner == 1 ? GREEN : RED};
    const raylib::Color turnIndicatorRecColor{10, 10, 10, 200};
    raylib::Rectangle turnIndicatorRect
    {
        0, 50,
        200,
        40
    };
    DrawRectangleRec(turnIndicatorRect, turnIndicatorRecColor);
    HelperFunctions::DrawTextBoxed(GetFont(GameFont::aobashiOne), turnIndicatorMessage.c_str(),
                                   turnIndicatorRect, 30, 2, 1, false, turnIndicatorTextColor);

    GetTexture(playingScene.playfield).Draw(Rectangle{0, 0, constants::playfieldRec.width, constants::playfieldRec.height}, constants::playfieldRec);
    // Draw stack stats total
    //// Rectangle
    const raylib::Rectangle statsTotalRec
    {
        constants::playfieldRec.x,
        constants::playfieldRec.y + constants::playfieldRec.height - 50,
        constants::playfieldRec.width,
        50
    };
    const raylib::Color statsTotalRecColor{10, 10, 10, 200};
    DrawRectangleRec(statsTotalRec, statsTotalRecColor);

    //// Player 1 Stats
    const CardStats player1TotalCardStats = CalculateCardStackTotalStats(player1, gameStatus);
    const std::string player1TotalCardStatsMessage
    {
        std::format("Total Stats \nB: {0} | M: {1} | S: {2}",
                    std::to_string(player1TotalCardStats.body),
                    std::to_string(player1TotalCardStats.mind),
                    std::to_string(player1TotalCardStats.soul))
    };

    const raylib::Rectangle player1StatsRec
    {
        constants::playfieldRec.x,
        constants::playfieldRec.y + constants::playfieldRec.height - 50,
        constants::playfieldRec.width * 0.5,
        50
    };
    HelperFunctions::DrawTextBoxed
    (
        GetFont(GameFont::aobashiOne),
        player1TotalCardStatsMessage.c_str(),
        player1StatsRec,
        20,
        0.1f,
        0.5f,
        true,
        WHITE
    );
    //// Player 2 Stats
    const CardStats player2TotalCardStats = CalculateCardStackTotalStats(player2, gameStatus);
    const std::string player2TotalCardStatsMessage
    {
        std::format("Total Stats \nB: {0} | M: {1} | S: {2}",
                    std::to_string(player2TotalCardStats.body),
                    std::to_string(player2TotalCardStats.mind),
                    std::to_string(player2TotalCardStats.soul))
    };
    const raylib::Rectangle player2StatsRec
    {
        constants::playfieldRec.x + constants::playfieldRec.width * 0.5,
        constants::playfieldRec.y + constants::playfieldRec.height - 50,
        constants::playfieldRec.width * 0.5,
        50
    };
    HelperFunctions::DrawTextBoxed
    (
        GetFont(GameFont::aobashiOne),
        player2TotalCardStatsMessage.c_str(),
        player2StatsRec,
        20,
        0.1f,
        0.5f,
        true,
        WHITE
    );

    //Join all cards in the order we want to draw them in
    //and draw them in a single for loop, skipping over the hovered card, which is drawn last.
    std::vector<Card> allCards{};
    //// Player 2 Hand
    allCards.insert(std::end(allCards), std::begin(player2.hand), std::end(player2.hand));
    //// Cards in player 1 hand
    allCards.insert(std::end(allCards), std::begin(player1.hand), std::end(player1.hand));
    //// Cards in the playfield
    const std::size_t playfieldCardsIndexOffset{allCards.size()};
    allCards.insert(std::end(allCards), std::begin(player1.cardsInPlayStack), std::end(player1.cardsInPlayStack));
    allCards.insert(std::end(allCards), std::begin(player2.cardsInPlayStack), std::end(player2.cardsInPlayStack));

    // Render all cards
    std::vector<unsigned long int> playableCardsUIDs{};
    for (const auto &[action, card]: player1.availableActions)
    {
        playableCardsUIDs.push_back(card.uid);
    }

    auto IsCardPlayable = [](const Card &card, const std::vector<unsigned long int> &playableUIDs) {
        return std::ranges::find(playableUIDs, card.uid) != playableUIDs.end();
    };

    if (allCards.empty()) return;
    std::size_t hoveredCardIndex{};
    std::size_t heldCardIndex{};
    for (std::size_t i = 0; i < allCards.size(); ++i)
    {
        //Hovered cards are drawn differently
        if (player1.isHoveringOverACard && player1.hoveredCardUid == allCards.at(i).uid)
        {
            hoveredCardIndex = i;
            continue;
        }
        if (player1.isHoldingACard && player1.heldCardUid == allCards.at(i).uid)
        {
            heldCardIndex = i;
            continue;
        }
        bool highlighted{IsCardPlayable(allCards.at(i), playableCardsUIDs)};
        RenderCard(allCards.at(i), allCards.at(i).rect, highlighted);
    }
    // Draw Held Card
    if (player1.isHoldingACard)
    {
        bool highlighted{IsCardPlayable(allCards.at(heldCardIndex), playableCardsUIDs)};
        RenderCard(allCards.at(heldCardIndex), allCards.at(heldCardIndex).rect, highlighted);
    }

    // Draw hovered card
    if (player1.isHoveringOverACard)
    {
        if (hoveredCardIndex < playfieldCardsIndexOffset)
        {
            //Expand the cards in the hand in place, but move it up a bit.
            const raylib::Rectangle expandedCardRect
            {
                std::max(10.0f, allCards.at(hoveredCardIndex).rect.x - (constants::zoomedCardWidth - allCards.at(hoveredCardIndex).rect.width) * 0.5f),
                allCards.at(hoveredCardIndex).rect.y - allCards.at(hoveredCardIndex).rect.height * 1.3f,
                constants::zoomedCardWidth,
                constants::zoomedCardHeight
            };
            bool highlighted{IsCardPlayable(allCards.at(hoveredCardIndex), playableCardsUIDs)};
            RenderCard(allCards.at(hoveredCardIndex), expandedCardRect, highlighted);
        }
        else
        {
            //Expand the cards in the play field in place
            const raylib::Rectangle expandedCardInFieldRect
            {
                allCards.at(hoveredCardIndex).rect.x - (constants::zoomedCardWidth - allCards.at(hoveredCardIndex).rect.width) * 0.5f,
                allCards.at(hoveredCardIndex).rect.y,
                constants::zoomedCardWidth,
                constants::zoomedCardHeight
            };

            bool highlighted{IsCardPlayable(allCards.at(hoveredCardIndex), playableCardsUIDs)};
            RenderCard(allCards.at(hoveredCardIndex), expandedCardInFieldRect, highlighted);
        }
    }

    if (currentTurnPhase == TurnPhase::endRoundPhase)
    {
        //Render a semi-transparent black overlay on the scene
        const raylib::Rectangle endPhaseOverlayRec{0, 0, constants::screenWidth, constants::screenHeight};
        const raylib::Color transparentBlack{0, 0, 0, 180};
        endPhaseOverlayRec.Draw(transparentBlack);

        DrawButton(playingScene.nextRoundButton, GetTexture(playingScene.nextRoundButton.background));

        constexpr float rectLength{200};
        const raylib::Rectangle message
        {
            constants::screenWidth / 2.0 - rectLength / 2.0,
            10,
            rectLength,
            200
        };

        assert(!gameStatus.roundWinnerHistory.empty() && "gameStatus.roundWinnerHistory is empty.");

        std::string endRoundMessage{"Uninitialized"};
        if (gameStatus.actionLogs.back().actionCardPairTaken.action == PlayerAction::forfeitTheRound)
        {
            endRoundMessage = std::format("Player {0} forfeited because they had no other moves left.\n"
                                          "Player {1} wind this round",
                                          std::to_string(gameStatus.actionLogs.back().playerID),
                                          std::to_string(gameStatus.roundWinnerHistory.back()));
        }
        else
        {
            if (gameStatus.roundWinnerHistory.back() == 0)
            {
                endRoundMessage = "Round ended in a tie.";
            }
            if (gameStatus.roundWinnerHistory.back() == 1)
            {
                endRoundMessage = "Player 1 wins this round.";
            }
            else if (gameStatus.roundWinnerHistory.back() == 2)
            {
                endRoundMessage = "Player 2 wins this round.";
            }
        }

        HelperFunctions::DrawTextCenteredInRec(endRoundMessage.c_str(), 20, RAYWHITE, message);
    }
}

void RunGameOverScene(GameOverScene &gameOverScene, GameScene &currentScene, TurnPhase &turnPhase, GameStatus &gameStatus,
                      Player &player1, Player &player2)
{
    //  UPDATE -----------------------------------------------------------------
    Button &playAgainButton = gameOverScene.playAgainButton;
    // Update
    // Update Buttons
    UpdateButtonState(playAgainButton,
                      GetMousePosition(),
                      IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                      IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
    if (playAgainButton.wasPressed)
    {
        PlaySound(GameSound::buttonPress01);

        ResetGame(player1, player2, gameStatus, turnPhase);

        currentScene = GameScene::starting;
    }

    // Update Music
    PlayMusic(gameOverScene.music);

    // DRAW --------------------------------------------------------------------
    GetTexture(gameOverScene.background).Draw();
    DrawButton(playAgainButton, GetTexture(playAgainButton.background));


    constexpr float rectLength{200};
    const raylib::Rectangle winMessageRect
    {
        constants::screenWidth / 2.0 - rectLength / 2.0,
        10,
        rectLength,
        100
    };
    if (gameStatus.pointsPlayer1 > gameStatus.pointsPlayer2)
    {
        HelperFunctions::DrawTextCenteredInRec("Player 1 Wins!", 20, BLACK, winMessageRect);
    }
    else if (gameStatus.pointsPlayer1 < gameStatus.pointsPlayer2)
    {
        HelperFunctions::DrawTextCenteredInRec("Player 2 Wins!", 20, BLACK, winMessageRect);
    }
    else
    {
        HelperFunctions::DrawTextCenteredInRec("It's a Draw!", 20, BLACK, winMessageRect);
    }
}

void RunPrototypingScene(const PrototypingScene &scene)
{
    // Testing Cards
    static Card cardTest = GetCardFromDB("Cancer Pagurus");
    cardTest.rect.SetPosition(200, 200);
    cardTest.rect.SetSize(constants::cardTextureWidth / 2, constants::cardTextureHeight / 2);

    //Draw
    GetTexture(scene.background).Draw();

    RenderCard(cardTest, cardTest.rect, true);
}

std::string GameSceneToString(const GameScene &gameScene)
{
    switch (gameScene)
    {
        case GameScene::invalid:
            return "Invalid";
        case GameScene::starting:
            return "Starting";
        case GameScene::playing:
            return "Playing";
        case GameScene::gameOver:
            return "GameOver";
        case GameScene::prototyping:
            return "Prototyping";
    }
    return "null";
}

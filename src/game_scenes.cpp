#include "game_scenes.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <format>

#include "audio.hpp"
#include "constants.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"
#include "textures.hpp"
#include "helper_functions.hpp"
#include "fonts.hpp"
#include "game_rules.hpp"
#include "game_status.hpp"
#include "game_turn.hpp"

struct GameStatus;

void RunStartingScene(StartingScene &startingScene, GameScene &currentScene)
{
    const raylib::Vector2 mousePosition = GetMousePosition();
    Button &startButton = startingScene.startButton;
    // Update
    // Update Buttons
    UpdateButtonState(startButton,
                      mousePosition,
                      IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                      IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
    if (startButton.wasPressed)
    {
        PlaySound(GameSound::buttonPress01);
        currentScene = GameScene::playing;
    }

    // Update Music
    PlayMusic(startingScene.music);

    //Draw
    GetTexture(startingScene.background).Draw();
    DrawButton(startButton, GetTexture(startButton.background));
}

void RunPlayingScene(PlayingScene &playingScene, TurnPhase &currentTurnPhase, const GameplayPhase &currentPhase, GameStatus &gameStatus, Player &player1, Player &player2, const GameRules &gameRules, [[maybe_unused]] std::random_device &rd)
{
    auto UpdateSceneButton = [](Button &bt, Player &p1, const PlayerAction pAction) {
        bt.state = ButtonState::disabled;

        bool playerHasAvailableAction{false};
        for (const auto &availableActions: p1.availableActions)
        {
            if (availableActions.action == pAction) playerHasAvailableAction = true;
        }

        if (playerHasAvailableAction)
        {
            bt.state = ButtonState::enabled;
            UpdateButtonState(bt, GetMousePosition(), IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

            if (bt.wasPressed)
            {
                PlaySound(GameSound::buttonPress01);
                p1.chosenAction.action = pAction;
            }
        }
    };

    static raylib::Vector2 heldCardOffset{};

    // UpdateGameplayPhases(playingScene, currentPhase, gameStatus, player1, player2, gameRules, rd);
    // UPDATE ------------------------------------------------------------------

    if (gameStatus.roundsPlayed == 0)
    {
        gameStatus.currentTurnOwner = gameRules.playerGoingFirst;
        gameStatus.currentTurnOwner = 1; // TODO: DELETE LATER
    }

    if (gameStatus.currentTurnOwner == 1)
    {
        player1.availableActions = CalculateAvailableActions(player1, currentTurnPhase, gameRules);


        ExecuteTurn(player1, currentTurnPhase, gameRules, gameStatus);
    }
    if (gameStatus.currentTurnOwner == 2)
    {
        player2.availableActions = CalculateAvailableActions(player2, currentTurnPhase, gameRules);

        //Insert CPU brain here

        ExecuteTurn(player2, currentTurnPhase, gameRules, gameStatus);
    }

    // Update the button states
    UpdateSceneButton(playingScene.endTurnButton, player1, PlayerAction::passTheTurn);
    UpdateSceneButton(playingScene.playerDeckButton, player1, PlayerAction::drawCard);

    // Update Music
    PlayMusic(playingScene.music);

    // Update Holding a Card From Hand
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
    // Update Trying To Play Card
    else
    {
        if (player1.isHoldingACard)
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

    // Update Player 1's hand cards. Put them in place, slightly apart from each other.
    for (size_t i = 0; i < player1.hand.size(); ++i)
    {
        const int int_i = static_cast<int>(i);
        //except for the held card.
        if (player1.hand.at(i).uid == player1.heldCardUid) continue;

        const raylib::Rectangle newCardRect
        {
            static_cast<float>(constants::handZonePosX + 2 * int_i + 1 + constants::inHandCardWidth * int_i),
            static_cast<float>(constants::handZonePosY + 4),
            constants::inHandCardWidth,
            constants::inHandCardHeight
        };

        player1.hand.at(i).rect = newCardRect;
        player1.hand.at(i).faceUp = true;
    }

    // Update Players stacks
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
    UpdatePlayStackCards(player1);
    UpdatePlayStackCards(player2);

    // Update Hovered Card
    player1.isHoveringOverACard = false;
    if (player1.isHoldingACard == false)
    {
        auto CheckHoveringCards = [](Player &p1, const std::vector<Card> &cards)-> void {
            for (std::size_t i = 0; i < cards.size(); ++i)
            {
                //Handle hovering
                if (HelperFunctions::CheckCollisionPointCard(GetMousePosition(), cards.at(i)))
                {
                    p1.hoveredCardUid = cards.at(i).uid;
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
    DrawButton(playingScene.playerDeckButton, GetTexture(playingScene.playerDeckButton.background));
    DrawButton(playingScene.endTurnButton, GetTexture(playingScene.endTurnButton.background));
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
    const std::string player1TotalCardStats
    {
        std::format("Total Stats \nB: {0} | M: {1} | S: {2}",
                    std::to_string(GetCardStackTotalBody(player1.cardsInPlayStack)),
                    std::to_string(GetCardStackTotalMind(player1.cardsInPlayStack)),
                    std::to_string(GetCardStackTotalSoul(player1.cardsInPlayStack)))
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
        player1TotalCardStats.c_str(),
        player1StatsRec,
        20,
        0.1f,
        0.5f,
        true,
        WHITE
    );
    //// Player 2 Stats
    const std::string player2TotalCardStats{
        std::format("Total Stats \nB: {0} | M: {1} | S: {2}",
                    std::to_string(GetCardStackTotalBody(player2.cardsInPlayStack)),
                    std::to_string(GetCardStackTotalMind(player2.cardsInPlayStack)),
                    std::to_string(GetCardStackTotalSoul(player2.cardsInPlayStack)))
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
        player2TotalCardStats.c_str(),
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

    // Draw all cards
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

        RenderCard(allCards.at(i), allCards.at(i).rect);
    }
    // Draw Held Card
    if (player1.isHoldingACard)
    {
        RenderCard(allCards.at(heldCardIndex), allCards.at(heldCardIndex).rect);
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
            RenderCard(allCards.at(hoveredCardIndex), expandedCardRect);
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
            RenderCard(allCards.at(hoveredCardIndex), expandedCardInFieldRect);
        }
    }

    if (currentPhase == GameplayPhase::endPhase)
    {
        //Render a semi-transparent black overlay on the scene
        const raylib::Rectangle endPhaseOverlayRec{0, 0, constants::screenWidth, constants::screenHeight};
        const raylib::Color transparentBlack{0, 0, 0, 180};
        endPhaseOverlayRec.Draw(transparentBlack);

        const Button &nextRoundButton = playingScene.nextRoundButton;
        DrawButton(nextRoundButton, GetTexture(nextRoundButton.background));

        constexpr float rectLength{200};
        const raylib::Rectangle message
        {
            constants::screenWidth / 2.0 - rectLength / 2.0,
            10,
            rectLength,
            200
        };

        assert(!gameStatus.roundWinnerHistory.empty() && "gameStatus.roundWinnerHistory is empty.");

        if (gameStatus.roundWinnerHistory.back() == 0)
        {
            HelperFunctions::DrawTextCenteredInRec("Round ended in a tie.", 20, RAYWHITE, message);
        }
        if (gameStatus.roundWinnerHistory.back() == 1)
        {
            HelperFunctions::DrawTextCenteredInRec("Player 1 wins this round.", 20, RAYWHITE, message);
        }
        else if (gameStatus.roundWinnerHistory.back() == 2)
        {
            HelperFunctions::DrawTextCenteredInRec("Player 2 wins this round.", 20, RAYWHITE, message);
        }
    }
}

void RunGameOverScene(GameOverScene &gameOverScene, GameScene &currentScene, GameplayPhase &gameplayPhase,
                      const Player &player1, const Player &player2)
{
    const raylib::Vector2 mousePosition = GetMousePosition();

    Button &playAgainButton = gameOverScene.playAgainButton;
    // Update
    // Update Buttons
    UpdateButtonState(playAgainButton,
                      mousePosition,
                      IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                      IsMouseButtonReleased(MOUSE_BUTTON_LEFT));
    if (playAgainButton.wasPressed)
    {
        PlaySound(GameSound::buttonPress01);

        gameplayPhase = GameplayPhase::uninitialized;
        currentScene = GameScene::playing;
    }

    // Update Music
    PlayMusic(gameOverScene.music);

    //Draw
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
    if (player1.score > player2.score)
    {
        HelperFunctions::DrawTextCenteredInRec("Player 1 Wins!", 20, BLACK, winMessageRect);
    }
    else if (player2.score > player1.score)
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
    static Card cardProt = GetCardFromDB("Cancer Pagurus");

    cardProt.rect.SetSize(
        GetMousePosition().x - cardProt.rect.x,
        (GetMousePosition().x - cardProt.rect.x) * (1 / constants::cardAspectRatio)
    );

    //Draw
    GetTexture(scene.background).Draw();
    RenderCard(cardProt, cardProt.rect);
}

#include "game_play_phases.hpp"
#include <cassert>
#include <iostream>

#include "audio.hpp"
#include "button.hpp"
#include "constants.hpp"
#include "game_rules.hpp"
#include "game_scenes.hpp"
#include "helper_functions.hpp"
#include "raylib-cpp.hpp"
#include "player.hpp"

void UpdateGameplayPhases(PlayingScene &playingScene, GameplayPhase &currentPhase, GameStatus &gameStatus, Player &player1,
                          Player &player2, const GameRules &gameRules, std::random_device &rd)
{
    const raylib::Vector2 mousePosition = GetMousePosition();
    static GameplayPhase previousPhase{GameplayPhase::uninitialized};
    static raylib::Vector2 heldCardOffset{};

    static bool player1HasPlayedThisPhase{false};
    [[maybe_unused]] static int player1TurnsPlayed{};
    [[maybe_unused]] static int player2TurnsPlayed{};
    static int roundsPlayed{};
    static int previousRoundWinner{};

    static float timeSinceStartOfPhase{};
    static float player2ActionDelay{0.5};

    auto RemovePlayer1HeldCard{
        [&player1]()-> void {
            player1.heldCardIndex = -1;
            player1.isHoldingACard = false;
        }
    };

    auto TryToPlayCard{
        [&player1, currentPhase, RemovePlayer1HeldCard]()-> void {
            if (player1.isHoldingACard && !player1HasPlayedThisPhase)
            {
                const Card &heldCard = player1.hand.at(static_cast<size_t>(player1.heldCardIndex));

                if (CheckCollisionRecs(heldCard.rect, constants::playfieldRec) &&
                    CanCardBePlayedByPlayer(heldCard, player1, currentPhase))
                {
                    PutCardInPlay(player1);
                    player1HasPlayedThisPhase = true;
                }
            }
            player1.isHoveringOverACard = false;
            player1.hoveredCardUid = 0;
            RemovePlayer1HeldCard(); //whether we can play it or not, we always clear the card from the hand.
        }
    };

    auto SetPlayerHeldCard{
        [&player1](const int index)-> void {
            assert(index >= 0 && "Trying to set a Held Card index that is negative.");
            assert(index < static_cast<int>(player1.hand.size()) && "Trying to set a Held Card index that is outside of the players hand bounds.");
            player1.heldCardIndex = index;
            player1.isHoldingACard = true;
        }
    };

    //TODO: design problem where this has to always come after UpdateHandAndHeldCard because it
    // depends on it to reset the position of the card, it seems.
    auto UpdateHoveringCardInHand{
        [&player1, mousePosition, player2]()-> void {
            //If player 1 is holding a card, do not update the hovered card
            if (player1.isHoldingACard)
            {
                return;
            }

            // player1.hoveredCardUid = 0;
            // player1.isHoveringOverACard = false;
            for (std::size_t i = 0; i < player1.hand.size(); ++i)
            {
                //Handle hovering
                if (HelperFunctions::CheckCollisionPointCard(mousePosition, player1.hand.at(i)))
                {
                    player1.hoveredCardUid = player1.hand.at(i).uid;
                    player1.isHoveringOverACard = true;
                }
            }
            for (std::size_t i = 0; i < player1.cardsInPlayStack.size(); ++i)
            {
                //Handle hovering
                if (HelperFunctions::CheckCollisionPointCard(mousePosition, player1.cardsInPlayStack.at(i)))
                {
                    player1.hoveredCardUid = player1.cardsInPlayStack.at(i).uid;
                    player1.isHoveringOverACard = true;
                }
            }
            for (std::size_t i = 0; i < player2.cardsInPlayStack.size(); ++i)
            {
                //Handle hovering
                if (HelperFunctions::CheckCollisionPointCard(mousePosition, player2.cardsInPlayStack.at(i)))
                {
                    player1.hoveredCardUid = player2.cardsInPlayStack.at(i).uid;
                    player1.isHoveringOverACard = true;
                }
            }
        }
    };

    auto UpdateHandAndHeldCard{
        [&player1, mousePosition, SetPlayerHeldCard]()-> void {
            //Place cards in the Hand Zone or Update Held Card
            for (size_t i = 0; i < player1.hand.size(); ++i)
            {
                const int int_i = static_cast<int>(i);

                //Move the card we are holding with the mouse
                if (player1.heldCardIndex == int_i)
                {
                    player1.hand.at(i).rect.SetPosition(mousePosition - heldCardOffset);
                    player1.hand.at(i).rect.SetSize(constants::inHandCardWidth, constants::inHandCardHeight);
                    continue;
                }

                //If trying to click and hold a card
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                    HelperFunctions::CheckCollisionPointCard(mousePosition, player1.hand.at(i)) &&
                    !player1.isHoldingACard)
                {
                    SetPlayerHeldCard(int_i);
                    heldCardOffset = mousePosition - player1.hand.at(i).rect.GetPosition();

                    //Don't position this card in the hand zone.
                    continue;
                }

                //Set cards in the hand slightly apart from each other.
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
        }
    };

    auto UpdatePlayer2HandCards{
        [&player2]() -> void {
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
        }
    };

    auto ChangePhase{
        [&currentPhase, &player1, &player2](const GameplayPhase nextPhase) {
            previousPhase = currentPhase;
            currentPhase = nextPhase;
            timeSinceStartOfPhase = 0;
            player1.hasEndedTheTurn = false;
            player2.hasEndedTheTurn = false;

            player1HasPlayedThisPhase = false;

            if (previousPhase != GameplayPhase::playerOneDrawing)
            {
                player1.hasDrawnThisTurn = false;
            }

            std::cout << "Game Phase = " << GameplayPhaseToString(currentPhase).c_str() << std::endl;
        }
    };

    auto UpdateEndTurnButton{
        [&player1, &playingScene, mousePosition]()-> void {
            Button &endTurnButton = playingScene.endTurnButton;

            if (player1.cardsInPlayStack.empty()) return;

            endTurnButton.state = ButtonState::enabled;
            UpdateButtonState(endTurnButton, mousePosition, IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

            if (endTurnButton.wasPressed)
            {
                PlaySound(GameSound::buttonPress01);
                player1.hasEndedTheTurn = true;
            }
        }
    };

    playingScene.playerDeckButton.state = ButtonState::disabled;
    playingScene.endTurnButton.state = ButtonState::disabled;

    timeSinceStartOfPhase += GetFrameTime();
    switch (currentPhase)
    {
        case GameplayPhase::uninitialized:
        {
            //Reset everything gameplay wise
            InitializePlayerWithAdvancedDeck(player1, rd);
            ShuffleDeckAndMakeSureTopCardIsAUnit(player1.deck, rd);

            InitializePlayerWithAdvancedDeck(player2, rd);
            ShuffleDeckAndMakeSureTopCardIsAUnit(player2.deck, rd);


            player1.hoveredCardUid = 0;
            player1.isHoveringOverACard = false;
            player2.hoveredCardUid = 0; //redundant for now
            player2.isHoveringOverACard = false; //redundant for now

            player1.cardsInPlayStack.clear();
            player2.cardsInPlayStack.clear();

            player1.hasDrawnThisTurn = false;
            player2.hasDrawnThisTurn = false;

            ChangePhase(GameplayPhase::initialHandDraw);
            break;
        }
        case GameplayPhase::initialHandDraw:
        {
            UpdateHandAndHeldCard();

            //Auto draw initial hand for P2
            if (player2.hand.size() != constants::initialHandSize)
            {
                ChangePhase(GameplayPhase::playerTwoDrawing);
                break;
            }
            assert(IsInitialHandValid(player2) && "Player 2 initial hand is not valid");

            if (player1.hand.size() != constants::initialHandSize)
            {
                ChangePhase(GameplayPhase::playerOneDrawing);
                break;
            }
            assert(IsInitialHandValid(player1) && "Player 1 initial hand is not valid");

            if (gameRules.playerGoingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerOneFirstTurn);
                break;
            }
            //else if (playerGoingFirst == 2)
            ChangePhase(GameplayPhase::playerTwoFirstTurn);
            break;
        }
        case GameplayPhase::playerOneDrawing:
        {
            //Don't force a card draw phase if the deck is empty
            if (player1.deck.empty())
            {
                ChangePhase(previousPhase);
                break;
            }

            playingScene.playerDeckButton.state = ButtonState::enabled;
            UpdateButtonState(playingScene.playerDeckButton, mousePosition, IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

            if (playingScene.playerDeckButton.wasPressed)
            {
                PlaySound(GameSound::buttonPress01);
                DrawCardsFromDeckToHand(player1, 1);
                player1.hasDrawnThisTurn = true;
                ChangePhase(previousPhase);
                break;
            }

            //Only handle hovering
            if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
            {
                player1.isHoveringOverACard = false;
                player1.hoveredCardUid = 0;
                RemovePlayer1HeldCard(); //whether we can play it or not, we always clear the card from the hand.
            }
            UpdateHandAndHeldCard();
            UpdateHoveringCardInHand();
            break;
        }
        case GameplayPhase::playerTwoDrawing:
        {
            if (timeSinceStartOfPhase < player2ActionDelay) break;

            //Don't force a card draw phase if the deck is empty
            if (player2.deck.empty())
            {
                ChangePhase(previousPhase);
                break;
            }

            DrawCardsFromDeckToHand(player2, 1);
            UpdatePlayer2HandCards();
            ChangePhase(previousPhase);
            break;
        }
        case GameplayPhase::playerOneFirstTurn:
        {
            // First, draw a card
            if (previousPhase != GameplayPhase::playerOneDrawing) //!player1.hasDrawnThisTurn)
            {
                ChangePhase(GameplayPhase::playerOneDrawing);
                break;
            }
            if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
            {
                TryToPlayCard();
            }
            UpdateHandAndHeldCard();
            UpdateHoveringCardInHand();
            UpdateEndTurnButton();

            //Wait until player 1 has pressed the end turn button
            if (!player1.hasEndedTheTurn)
            {
                break;
            }

            if (roundsPlayed == 0)
            {
                if (gameRules.playerGoingFirst == 1)
                {
                    ChangePhase(GameplayPhase::playerTwoFirstTurn);
                    break;
                }
                if (gameRules.playerGoingFirst == 2)
                {
                    ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPlayed);
                    break;
                }
            }
            if (previousRoundWinner == 1)
            {
                ChangePhase(GameplayPhase::playerTwoFirstTurn);
                break;
            }
            ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPlayed);
            break;
        }
        case GameplayPhase::playerTwoFirstTurn:
        {
            if (timeSinceStartOfPhase < player2ActionDelay) break;

            // First, draw a card
            if (previousPhase != GameplayPhase::playerTwoDrawing) //!player1.hasDrawnThisTurn)
            {
                ChangePhase(GameplayPhase::playerTwoDrawing);
                break;
            }

            if (timeSinceStartOfPhase < player2ActionDelay) break;

            //On the first turn, pick a Unit from the hand and play it
            for (size_t i = 0; i < player2.hand.size(); ++i)
            {
                if (player2.hand.at(i).type == CardType::unit)
                {
                    player2.heldCardIndex = static_cast<int>(i);
                    break;
                }
            }
            assert(player2.hand.at(static_cast<size_t>(player2.heldCardIndex)).type == CardType::unit &&
                "Player 2 was about to play a non-unit card as its first card.");
            PutCardInPlay(player2);
            UpdatePlayer2HandCards();

            if (roundsPlayed == 0)
            {
                if (gameRules.playerGoingFirst == 1)
                {
                    ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPlayed);
                    break;
                }
                if (gameRules.playerGoingFirst == 2)
                {
                    ChangePhase(GameplayPhase::playerOneFirstTurn);
                    break;
                }
            }
            if (previousRoundWinner == 1)
            {
                ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPlayed);
                break;
            }
            ChangePhase(GameplayPhase::playerOneFirstTurn);
            break;
        }
        case GameplayPhase::playerOnePlayingAndOpponentPlayed:
        {
            // First, draw a card
            if (previousPhase != GameplayPhase::playerOneDrawing) //!player1.hasDrawnThisTurn)
            {
                ChangePhase(GameplayPhase::playerOneDrawing);
                break;
            }

            if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
            {
                TryToPlayCard();
            }
            UpdateHandAndHeldCard();
            UpdateHoveringCardInHand();
            UpdateEndTurnButton();

            //Wait until player 1 has pressed the end turn button
            if (!player1.hasEndedTheTurn)
            {
                break;
            }

            if (player1HasPlayedThisPhase)
            {
                ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPlayed);
                break;
            }
            //Player 1 passed without playing:
            ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPassed);
            break;
        }
        case GameplayPhase::playerTwoPlayingAndOpponentPlayed:
        {
            if (timeSinceStartOfPhase < player2ActionDelay) break;

            // First, draw a card
            if (previousPhase != GameplayPhase::playerTwoDrawing) //!player1.hasDrawnThisTurn)
            {
                ChangePhase(GameplayPhase::playerTwoDrawing);
                break;
            }

            if (timeSinceStartOfPhase < player2ActionDelay) break;

            //If no cards in hand, pass the turn without playing
            if (player2.hand.empty())
            {
                player2.hasDrawnThisTurn = false;
                ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPassed);
                break;
            }

            //To start with, Player 2 will always try to buff up it's Units if
            //it has Action cards available to use.
            for (size_t i = 0; i < player2.hand.size(); ++i)
            {
                if (player2.hand.at(i).type == CardType::action)
                {
                    player2.heldCardIndex = static_cast<int>(i);
                    PutCardInPlay(player2);
                    UpdatePlayer2HandCards();

                    player2.hasDrawnThisTurn = false;
                    ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPlayed);
                    break;
                }
            }

            //If not, it will pass the turn.
            player2.hasDrawnThisTurn = false;
            ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPassed);
            break;
        }
        case GameplayPhase::playerOnePlayingAndOpponentPassed:
        {
            // First, draw a card
            if (previousPhase != GameplayPhase::playerOneDrawing) //!player1.hasDrawnThisTurn)
            {
                ChangePhase(GameplayPhase::playerOneDrawing);
                break;
            }

            if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
            {
                TryToPlayCard();
            }
            UpdateHandAndHeldCard();
            UpdateHoveringCardInHand();
            UpdateEndTurnButton();

            //Wait until player 1 has pressed the end turn button
            if (!player1.hasEndedTheTurn)
            {
                break;
            }

            if (player1HasPlayedThisPhase)
            {
                ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPlayed);
                break;
            }
            //Player 1 passed without playing:
            ChangePhase(GameplayPhase::battle);
            break;
        }
        case GameplayPhase::playerTwoPlayingAndOpponentPassed:
        {
            if (timeSinceStartOfPhase < player2ActionDelay) break;

            // First, draw a card
            if (previousPhase != GameplayPhase::playerTwoDrawing) //!player1.hasDrawnThisTurn)
            {
                ChangePhase(GameplayPhase::playerTwoDrawing);
                break;
            }
            if (!player2.hasDrawnThisTurn)
            {
                player2.hasDrawnThisTurn = true;
                ChangePhase(GameplayPhase::playerTwoDrawing);
                break;
            }

            if (timeSinceStartOfPhase < player2ActionDelay) break;

            //If no cards in hand, pass the turn without playing
            if (player2.hand.empty())
            {
                player2.hasDrawnThisTurn = false;
                ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPassed);
                break;
            }

            //To start with, Player 2 will always try to buff up it's Units if it has Action cards
            //available to use.
            for (size_t i = 0; i < player2.hand.size(); ++i)
            {
                if (player2.hand.at(i).type == CardType::action)
                {
                    player2.heldCardIndex = static_cast<int>(i);
                    PutCardInPlay(player2);
                    UpdatePlayer2HandCards();

                    player2.hasDrawnThisTurn = false;
                    ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPlayed);
                    break;
                }
            }

            //If not, it will pass the turn.

            player2.hasDrawnThisTurn = false;
            ChangePhase(GameplayPhase::battle);
            break;
        }
        case GameplayPhase::battle:
        {
            if (timeSinceStartOfPhase < player2ActionDelay) break;

            roundsPlayed++;
            const int winner = CalculateRoundWinner(player1.cardsInPlayStack, player2.cardsInPlayStack);
            previousRoundWinner = winner;

            if (winner == 1) player1.score++;
            if (winner == 2) player2.score++;

            gameStatus.roundsPlayed++;
            gameStatus.roundWinnerHistory.push_back(winner);

            ChangePhase(GameplayPhase::endPhase);
            break;
        }
        case GameplayPhase::endPhase:
        {
            Button &nextRoundButton{playingScene.nextRoundButton};

            UpdateButtonState(nextRoundButton, mousePosition,
                              IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                              IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

            if (!nextRoundButton.wasPressed) break;

            // Reset stuff and start over
            player1.hoveredCardUid = 0;
            player1.isHoveringOverACard = false;
            player2.hoveredCardUid = 0; //redundant for now
            player2.isHoveringOverACard = false; //redundant for now

            player1.cardsInPlayStack.clear();
            player2.cardsInPlayStack.clear();

            player1.hasDrawnThisTurn = false;
            player2.hasDrawnThisTurn = false;

            //Best of 3 Game
            if (player1.score >= gameRules.pointsNeededToWin ||
                player2.score >= gameRules.pointsNeededToWin)
            {
                ChangePhase(GameplayPhase::gameOver);
                break;
            }
            // The player who just won starts playing first (disadvantage) TODO: turn into a game_rule
            if (previousRoundWinner == 0) //Tie
            {
                if (gameRules.playerGoingFirst == 1)
                {
                    ChangePhase(GameplayPhase::playerOneFirstTurn);
                    break;
                }
                ChangePhase(GameplayPhase::playerTwoFirstTurn);
                break;
            }
            if (previousRoundWinner == 1)
            {
                ChangePhase(GameplayPhase::playerOneFirstTurn);
                break;
            }
            if (previousRoundWinner == 2)
            {
                ChangePhase(GameplayPhase::playerTwoFirstTurn);
                break;
            }
        }
        case GameplayPhase::gameOver:
            break;
    }
}

void DrawCardsFromDeckToHand(Player &player, const int amount)
{
    assert(amount <= static_cast<int>(player.deck.size())
        && "Trying to draw more cards than player has in its deck.\n");

    for (int i = 0; i < amount; ++i)
    {
        Card drawnCard = player.deck.at(0);
        drawnCard.rect.SetPosition(raylib::Vector2{-1000, -1000});
        drawnCard.faceUp = false;
        player.hand.push_back(drawnCard);
        player.deck.erase(player.deck.begin());
    }
}

void PutCardInPlay(Player &player)
{
    assert(player.heldCardIndex > -1 && "Trying to play a card of negative index.");

    player.cardsPlayed++;

    //Copy the card from the hand to the Play field.
    player.cardsInPlayStack.emplace_back(player.hand.at(static_cast<size_t>(player.heldCardIndex)));

    //Place the cards in their respective player's play zone, and offset them vertically as they stack
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


    //Then remove that card from the hand.
    player.hand.erase(player.hand.begin() + player.heldCardIndex);
    //The player is now not holding any card.
    player.heldCardIndex = -1;

    //Play sound
    PlaySound(GameSound::cardPlace01);
}

std::string GameplayPhaseToString(const GameplayPhase phase)
{
    switch (phase)
    {
        case GameplayPhase::uninitialized:
            return "Uninitialized";
        case GameplayPhase::initialHandDraw:
            return "Initial Hand Draw";
        case GameplayPhase::playerOneDrawing:
            return "Player One Drawing";
        case GameplayPhase::playerTwoDrawing:
            return "Player Two Drawing";
        case GameplayPhase::playerOneFirstTurn:
            return "Player One First Turn";
        case GameplayPhase::playerTwoFirstTurn:
            return "Player Two First Turn";
        case GameplayPhase::playerOnePlayingAndOpponentPlayed:
            return "Player One Playing And Opponent Played";
        case GameplayPhase::playerTwoPlayingAndOpponentPlayed:
            return "Player Two Playing And Opponent Played";
        case GameplayPhase::playerOnePlayingAndOpponentPassed:
            return "Player One Playing And Opponent Passed (did NOT play)";
        case GameplayPhase::playerTwoPlayingAndOpponentPassed:
            return "Player Two Playing And Opponent Passed (did NOT play)";
        case GameplayPhase::battle:
            return "Battle";
        case GameplayPhase::endPhase:
            return "End Phase";
        case GameplayPhase::gameOver:
            return "Game Over";
    }
    return "Error";
}

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


void NEW_UpdateGameplayPhases(
    PlayingScene &playingScene,
    GameplayPhase &currentPhase,
    Player &player1,
    Player &player2,
    [[maybe_unused]] const int playerGoingFirst,
    [[maybe_unused]] std::random_device &rd)
{
    const raylib::Vector2 mousePosition = GetMousePosition();
    [[maybe_unused]] static float timeSinceStartOfPhase{};
    [[maybe_unused]] static GameplayPhase previousPhase{GameplayPhase::uninitialized};
    static raylib::Vector2 heldCardOffset{};
    [[maybe_unused]] int hoveredCardIndex{-1};


    auto SetPlayerHeldCard{
        [&player1](const int index)-> void {
            assert(index >= 0 && "Trying to set a Held Card index that is negative.");
            assert(index < static_cast<int>(player1.hand.size()) && "Trying to set a Held Card index that is outside of the players hand bounds.");
            player1.heldCardIndex = index;
            player1.isHoldingACard = true;
        }

    };

    auto UpdateHandAndHeldCard{
        [&player1, mousePosition, SetPlayerHeldCard, &hoveredCardIndex]()-> void {
            constexpr int cardWidth{constants::cardWidth * 2};
            constexpr int cardHeight{constants::cardHeight * 2};

            //Place cards in the Hand Zone or Update Held Card
            for (size_t i = 0; i < player1.hand.size(); ++i)
            {
                //Move the card we are holding with the mouse
                if (static_cast<int>(i) == player1.heldCardIndex)
                {
                    player1.hand.at(i).rect.SetPosition(mousePosition - heldCardOffset);
                    player1.hand.at(i).rect.SetSize(cardWidth, cardHeight);
                    continue;
                }

                //If trying to click and hold a card
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                    HelperFunctions::CheckCollisionPointCard(mousePosition, player1.hand.at(i)) &&
                    !player1.isHoldingACard)
                {
                    SetPlayerHeldCard(static_cast<int>(i));
                    heldCardOffset = mousePosition - player1.hand.at(i).rect.GetPosition();

                    //Don't position this card in the hand zone.
                    continue;
                }

                //Set cards in the hand slightly apart from each other.
                const int int_i = static_cast<int>(i);
                const raylib::Rectangle newCardRect
                {
                    static_cast<float>(constants::handZonePosX + 2 * int_i + 1 + cardWidth * int_i),
                    static_cast<float>(constants::handZonePosY + 4),
                    cardWidth,
                    cardHeight
                };

                player1.hand.at(i).rect = newCardRect;
                player1.hand.at(i).faceUp = true;

                //Handle hovering
                if (HelperFunctions::CheckCollisionPointCard(mousePosition, player1.hand.at(i)))
                {
                    hoveredCardIndex = static_cast<int>(i);
                }
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
        }
    };

    playingScene.playerDeckButton.state = ButtonState::disabled;

    switch (currentPhase)
    {
        case GameplayPhase::uninitialized:
        {
            //Reset everything gameplay wise
            heldCardOffset = raylib::Vector2{0, 0};
            hoveredCardIndex = -1;
            InitializePlayerWithAdvancedDeck(player1, rd);
            InitializePlayerWithAdvancedDeck(player2, rd);

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
            //TODO: Fix this later. Make sure initial hand is always valid. Shuffle and redraw if needed.
            assert(IsInitialHandValid(player2) && "Player 2 initial hand is not valid");

            if (player1.hand.size() != constants::initialHandSize)
            {
                ChangePhase(GameplayPhase::playerOneDrawing);
                break;
            }
            //TODO: Fix this later. Make sure initial hand is always valid. Shuffle and redraw if needed.
            assert(IsInitialHandValid(player1) && "Player 1 initial hand is not valid");


            if (playerGoingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerOneFirstTurn);
            }
            else //if (playerGoingFirst == 2)
            {
                ChangePhase(GameplayPhase::playerTwoFirstTurn);
            }

            break;
        }
        case GameplayPhase::playerOneDrawing:
        {
            playingScene.playerDeckButton.state = ButtonState::enabled;
            UpdateButtonState(playingScene.playerDeckButton, mousePosition, IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT));

            if (playingScene.playerDeckButton.wasPressed)
            {
                PlaySound(GameSound::buttonPress01);
                DrawCardsFromDeckToHand(player1, 1);
                player1.hasDrawnThisTurn = true;
                ChangePhase(previousPhase);
            }

            UpdateHandAndHeldCard();
            break;
        }
        case GameplayPhase::playerTwoDrawing:
        {
            DrawCardsFromDeckToHand(player2, 1);
            ChangePhase(previousPhase);
            break;
        }
        case GameplayPhase::playerOneFirstTurn:
            break;
        case GameplayPhase::playerTwoFirstTurn:
        {
            //On the first turn, pick a Unit from the hand and play it
            for (size_t i = 0; i < player2.hand.size(); ++i)
            {
                if (player2.hand.at(i).type == CardType::unit)
                {
                    player2.heldCardIndex = static_cast<int>(i);
                    break;
                }
            }
            PutCardInPlay(player2);

            if (playerGoingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPlayed);
            }
            else
            {
                ChangePhase(GameplayPhase::playerOneFirstTurn);
            }

            break;
        }
        case GameplayPhase::playerOnePlayingAndOpponentPlayed:
            break;
        case GameplayPhase::playerTwoPlayingAndOpponentPlayed:
            break;
        case GameplayPhase::playerOnePlayingAndOpponentPassed:
            break;
        case GameplayPhase::playerTwoPlayingAndOpponentPassed:
            break;
        case GameplayPhase::battle:
            break;
        case GameplayPhase::gameOver:
            break;
    }
}

/*
void UpdateGameplayPhases(GameplayPhase &currentPhase, Player &player1, Player &player2,
                          const int playerGoingFirst, std::random_device &rd)
{
    static float timeSinceStartOfPhase{};
    static constexpr float playerActionWaitTime{1};
    static int playerOnePreviousCardsPlayed{};
    static int playerTwoPreviousCardsPlayed{};
    [[maybe_unused]] static GameplayPhase previousPhase{GameplayPhase::uninitialized};

    auto ChangePhase
    {
        [&currentPhase, &player1, &player2](const GameplayPhase nextPhase) {
            previousPhase = currentPhase;
            currentPhase = nextPhase;
            timeSinceStartOfPhase = 0;
            player1.hasEndedTheTurn = false;
            player2.hasEndedTheTurn = false;
        }
    };

    auto HasPlayerPlayed
    {
        [](const Player &player) {
            if (player.id == 1 && playerOnePreviousCardsPlayed < player.cardsPlayed)
            {
                return true;
            }
            if (player.id == 2 && playerTwoPreviousCardsPlayed < player.cardsPlayed)
            {
                return true;
            }
            return false;
        }
    };


    timeSinceStartOfPhase += GetFrameTime();

    switch (currentPhase)
    {
        case GameplayPhase::uninitialized:
        {
            //Reset everything gameplay wise
            InitializePlayerWithAdvancedDeck(player1, rd);
            InitializePlayerWithAdvancedDeck(player2, rd);

            ChangePhase(GameplayPhase::initialHandDraw);

            break;
        }
        case GameplayPhase::initialHandDraw:
        {
            player1.cardsInPlayStack.clear();
            player2.cardsInPlayStack.clear();

            player1.hasDrawnThisTurn = false;
            player2.hasDrawnThisTurn = false;

            //Auto draw initial hand for P2
            if (player2.hand.size() != constants::initialHandSize)
            {
                DrawCardsFromDeckToHand(player2, constants::initialHandSize);

                //TODO: Fix this later. Make sure initial hand is always valid. Shuffle and redraw if needed.
                assert(IsInitialHandValid(player2) && "Player 2 initial hand is not valid");
                // break;
            }
            //Wait for P1 to click the draw button enough times
            if (player1.hand.size() != constants::initialHandSize)
            {
                break;
            }
            else
            {
                //TODO: Fix this later. Make sure initial hand is always valid. Shuffle and redraw if needed.
                assert(IsInitialHandValid(player1) && "Player 1 initial hand is not valid");
            }
            if (playerGoingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerOneFirstTurn);
                break;
            }
            if (playerGoingFirst == 2)
            {
                ChangePhase(GameplayPhase::playerTwoFirstTurn);
                break;
            }

            break;
        }
        case GameplayPhase::playerOneFirstTurn:
        {
            if (!player1.deck.empty())
            {
                if (player1.hasDrawnThisTurn == false)
                {
                    break;
                }
            }

            //TODO: Make sure the card played is a Unit card.
            //Wait until player 1 has played
            if (!HasPlayerPlayed(player1))
            {
                break;
            }

            //Wait until player 1 has pressed the end turn button
            if (!player1.hasEndedTheTurn)
            {
                break;
            }

            if (playerGoingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerTwoFirstTurn);
                break;
            }
            if (playerGoingFirst == 2)
            {
                ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPlayed);
                break;
            }
            break;
        }
        case GameplayPhase::playerTwoFirstTurn:
        {
            //play only if there are cards in the deck
            if (!player2.deck.empty() && player2.hasDrawnThisTurn == false)
            {
                DrawCardsFromDeckToHand(player2, 1);
                player2.hasDrawnThisTurn = true;
            }

            //wait a little bit
            if (timeSinceStartOfPhase < playerActionWaitTime) break;

            //TODO: Make sure the card played is a Unit card.
            //Player 2 plays a card, always index 0 for now
            player2.heldCardIndex = 0;
            PutCardInPlay(player2);

            if (playerGoingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPlayed);
                break;
            }
            if (playerGoingFirst == 2)
            {
                ChangePhase(GameplayPhase::playerOneFirstTurn);
                break;
            }
            break;
        }
        case GameplayPhase::playerOnePlayingAndOpponentPlayed:
        {
            //Draw a card first
            if (!player1.deck.empty() && player1.hasDrawnThisTurn == false)
            {
                break;
            }

            //Wait until player 1 has pressed the end turn button
            if (!player1.hasEndedTheTurn)
            {
                break;
            }

            if (HasPlayerPlayed(player1))
            {
                ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPlayed);
                break;
            }
            else
            {
                ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPassed);
                break;
            }
        }
        case GameplayPhase::playerOnePlayingAndOpponentPassed:
        {
            //Draw a card first
            if (!player1.deck.empty() && player1.hasDrawnThisTurn == false)
            {
                break;
            }

            //Wait until player 1 has pressed the end turn button
            if (!player1.hasEndedTheTurn)
            {
                break;
            }

            if (HasPlayerPlayed(player1))
            {
                ChangePhase(GameplayPhase::playerTwoPlayingAndOpponentPlayed);
                break;
            }
            else
            {
                ChangePhase(GameplayPhase::battle); //When both pass in a row, they battle
                break;
            }
        }

        case GameplayPhase::playerTwoPlayingAndOpponentPlayed: //not yet implemented
        {
            if (!player2.deck.empty() && player2.hasDrawnThisTurn == false)
            {
                DrawCardsFromDeckToHand(player2, 1);
                player2.hasDrawnThisTurn = true;
            }

            if (timeSinceStartOfPhase < playerActionWaitTime) break;

            //Some logic here that makes the player 2 randomly player another card or pass.
            std::uniform_int_distribution coinFlip{0, 1};
            if (coinFlip(rd) == 0) //Pass
            {
                ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPassed);
                break;
            }
            else //Play
            {
                player2.heldCardIndex = 0;
                PutCardInPlay(player2);
                ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPlayed);
                break;
            }
        }
        case GameplayPhase::playerTwoPlayingAndOpponentPassed:
        {
            if (!player2.deck.empty() && player2.hasDrawnThisTurn == false)
            {
                DrawCardsFromDeckToHand(player2, 1);
                player2.hasDrawnThisTurn = true;
            }

            if (timeSinceStartOfPhase < playerActionWaitTime) break;

            //Some logic here that makes the player 2 randomly player another card or pass.
            std::uniform_int_distribution coinFlip{0, 1};
            if (coinFlip(rd) == 0) //Pass
            {
                ChangePhase(GameplayPhase::battle);
                break;
            }
            else //Play
            {
                player2.heldCardIndex = 0;
                PutCardInPlay(player2);
                ChangePhase(GameplayPhase::playerOnePlayingAndOpponentPlayed);
                break;
            }
        }
        case GameplayPhase::battle:
        {
            // Wait a bit before clearing the field
            if (timeSinceStartOfPhase < playerActionWaitTime)
                break;

            const int turnWinner = BattleCards(player1.cardsInPlayStack, player2.cardsInPlayStack);
            if (turnWinner == 1)
            {
                player1.score++;
            }
            if (turnWinner == 2)
            {
                player2.score++;
            }

            //Remove the played cards
            player1.cardsInPlayStack.clear();
            player2.cardsInPlayStack.clear();
            player1.hasDrawnThisTurn = false;
            player2.hasDrawnThisTurn = false;
            player1.hasEndedTheTurn = false;

            if (player1.hand.empty() && player1.deck.empty() &&
                player2.hand.empty() && player2.deck.empty())
            {
                ChangePhase(GameplayPhase::gameOver);
                break;
            }

            //After the battle, give the turn to the player who went first
            if (playerGoingFirst == 1)
            {
                ChangePhase(GameplayPhase::playerOneFirstTurn);
                break;
            }
            if (playerGoingFirst == 2)
            {
                ChangePhase(GameplayPhase::playerTwoFirstTurn);
                break;
            }
            break;
        }
        case GameplayPhase::gameOver:
        {
            break;
        }
        case GameplayPhase::playerOneDrawing:
            break;
        case GameplayPhase::playerTwoDrawing:
            break;
    }
}
*/

void DrawCardsFromDeckToHand(Player &player, const int amount)
{
    assert(amount <= static_cast<int>(player.deck.size())
        && "Trying to draw more cards than player has in its deck.\n");

    for (int i = 0; i < amount; ++i)
    {
        Card drawnCard = player.deck.back();
        drawnCard.rect.SetPosition(raylib::Vector2{-1000, -1000});
        drawnCard.faceUp = false;
        player.hand.push_back(drawnCard);
        player.deck.pop_back();
    }
}

void PutCardInPlay(Player &player)
{
    assert(player.heldCardIndex > -1 && "Trying to play a card of negative index.");

    //Can only play Non-Unit cards after a Unit card is already in play
    // if (player.hand.at(static_cast<size_t>(player.heldCardIndex)).type == CardType::action) return;

    player.cardsPlayed++;

    //Copy the card from the hand to the Play field.
    player.cardsInPlayStack.emplace_back(player.hand.at(static_cast<size_t>(player.heldCardIndex)));
    player.cardsInPlayStack.at(0).faceUp = true;

    if (player.id == 1)
    {
        player.cardsInPlayStack.at(0).rect = raylib::Rectangle{
            constants::playerOnePlayfieldCardZoneRect.x,
            constants::playerOnePlayfieldCardZoneRect.y,
            constants::cardWidth,
            constants::cardHeight
        };
    }
    else if (player.id == 2)
    {
        player.cardsInPlayStack.at(0).rect = raylib::Rectangle{
            constants::playerTwoPlayfieldCardZoneRect.x,
            constants::playerTwoPlayfieldCardZoneRect.y,
            constants::cardWidth,
            constants::cardHeight
        };
    }

    //Then remove that card from the hand.
    player.hand.erase(player.hand.begin() + player.heldCardIndex);
    //The player is now not holding any card.
    player.heldCardIndex = -1;

    //Play sound
    PlaySound(GameSound::cardPlace01);
}

int BattleCards(const std::vector<Card> &cardStack1, const std::vector<Card> &cardStack2)
{
    const Card &card1 = cardStack1.at(0);
    int highestStatCard1{0};
    if (card1.body > highestStatCard1) highestStatCard1 = card1.body;
    if (card1.mind > highestStatCard1) highestStatCard1 = card1.mind;
    if (card1.soul > highestStatCard1) highestStatCard1 = card1.soul;

    const Card &card2 = cardStack2.at(0);
    int highestStatCard2{0};
    if (card2.body > highestStatCard2) highestStatCard2 = card2.body;
    if (card2.mind > highestStatCard2) highestStatCard2 = card2.mind;
    if (card2.soul > highestStatCard2) highestStatCard2 = card2.soul;

    if (highestStatCard1 > highestStatCard2)
    {
        return 1;
    }
    if (highestStatCard2 > highestStatCard1)
    {
        return 2;
    }
    //Draw
    return 0;
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
            return "Player One Playing And Opponent Passed";
        case GameplayPhase::playerTwoPlayingAndOpponentPassed:
            return "Player Two Playing And Opponent Passed";
        case GameplayPhase::battle:
            return "Battle";
        case GameplayPhase::gameOver:
            return "Game Over";
    }
    return "Error";
}



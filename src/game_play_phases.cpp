#include "game_play_phases.hpp"

#include <cassert>
#include <iostream>

#include "constants.hpp"
#include "raylib-cpp.hpp"
#include "player.hpp"

void UpdateGameplayPhases(GameplayPhase &currentPhase, Player &player1, Player &player2, const int goingFirst)
{
    [[maybe_unused]] static float timeSinceStartOfPhase{};
    timeSinceStartOfPhase += GetFrameTime();

    static bool player1HasDrawnThisTurn{};

    static float playerActionWaitTime{1};

    switch (currentPhase)
    {
        case GameplayPhase::uninitialized:
        {
            std::cerr << "Gameplay Phase is uninitialized. Something went wrong\n";
            break;
        }
        case GameplayPhase::initialHandDraw:
        {
            //Auto draw initial hand for P2
            if (player2.hand.size() != constants::initialHandSize)
            {
                DrawCardsFromDeckToHand(player2, constants::initialHandSize);
                break;
            }
            //Wait for P1 to click the draw button enough times
            if (player1.hand.size() != constants::initialHandSize)
            {
                break;
            }
            if (goingFirst == 1)
            {
                currentPhase = GameplayPhase::playerOnePlaying;
                timeSinceStartOfPhase = 0;
                break;
            }
            if (goingFirst == 2)
            {
                currentPhase = GameplayPhase::playerTwoPlaying;
                timeSinceStartOfPhase = 0;
                break;
            }
            break;
        }
        case GameplayPhase::playerOneDrawing:
        {
            //if player's deck is empty, skip drawing phase, or
            //wait for Player 1 to draw a card (by clicking the deck button).
            if (player1.deck.empty() || player1HasDrawnThisTurn)
            {
                currentPhase = GameplayPhase::playerOnePlaying;
                timeSinceStartOfPhase = 0;
                break;
            }
            break;
        }
        case GameplayPhase::playerOnePlaying:
        {
            if (player1.cardInPlay.type == CardType::invalid &&
                player1.heldCardIndex > -1)
            {
                PutCardInPlay(player1);

                //Have both played
                if (player1.cardInPlay.type != CardType::invalid &&
                    player2.cardInPlay.type != CardType::invalid)
                {
                    timeSinceStartOfPhase = 0;
                    currentPhase = GameplayPhase::battle;
                }
                else
                {
                    timeSinceStartOfPhase = 0;
                    currentPhase = GameplayPhase::playerTwoDrawing;
                }
                break;
            }
            break;
        }
        case GameplayPhase::playerTwoDrawing:
        {
            //if player's deck is empty, skip drawing phase
            if (player2.deck.empty())
            {
                currentPhase = GameplayPhase::playerTwoPlaying;
                break;
            }
            //Player 2 draws automatically
            DrawCardsFromDeckToHand(player2, 1);

            timeSinceStartOfPhase = 0;
            currentPhase = GameplayPhase::playerTwoPlaying;
            break;
        }
        case GameplayPhase::playerTwoPlaying:
        {
            if (timeSinceStartOfPhase < playerActionWaitTime) break;

            //Player 2 plays a card, always index 0 for now
            player2.heldCardIndex = 0;
            PutCardInPlay(player2);

            //Have both played
            if (player1.cardInPlay.type != CardType::invalid &&
                player2.cardInPlay.type != CardType::invalid)
            {
                timeSinceStartOfPhase = 0;
                currentPhase = GameplayPhase::battle;
            }
            else
            {
                timeSinceStartOfPhase = 0;
                currentPhase = GameplayPhase::playerOneDrawing;
            }
            break;
        }
        case GameplayPhase::battle:
        {
            // Wait a bit before clearing the field
            if (timeSinceStartOfPhase < playerActionWaitTime)
                break;

            //reveal played cards
            player1.cardInPlay.faceUp = true;
            player2.cardInPlay.faceUp = true;

            //Wait a bit more after revealing the cards
            if (timeSinceStartOfPhase < playerActionWaitTime + 1)
                break;

            const int turnWinner = BattleCards(player1.cardInPlay, player2.cardInPlay);
            if (turnWinner == 1)
            {
                player1.score++;
            }
            if (turnWinner == 2)
            {
                player2.score++;
            }

            //Remove the played cards
            player1.cardInPlay.type = CardType::invalid;
            player2.cardInPlay.type = CardType::invalid;

            player1HasDrawnThisTurn = false;

            if (player1.hand.empty() || player2.hand.empty())
            {
                //GameOver
            }

            //After the battle, give the turn to the player who went first
            if (goingFirst == 1)
            {
                timeSinceStartOfPhase = 0;
                currentPhase = GameplayPhase::playerOneDrawing;
                break;
            }
            if (goingFirst == 2)
            {
                timeSinceStartOfPhase = 0;
                currentPhase = GameplayPhase::playerTwoDrawing;
            }
            break;
        }
    }
}

void DrawCardsFromDeckToHand(Player &player, int amount)
{
    assert(amount <= static_cast<int>(player.deck.size())
        && "Trying to draw more cards than player has in its deck.\n");

    for (int i = 0; i < amount; ++i)
    {
        Card drawnCard = player.deck.back();
        drawnCard.pos = raylib::Vector2{-1000, -1000};
        player.hand.push_back(drawnCard);
        player.deck.pop_back();
    }
}

void PutCardInPlay(Player &player)
{
    assert(player.heldCardIndex < 0 &&
        "Trying to play a card of negative index.");

    //Copy the card from the hand to the inPlay field.
    player.cardInPlay = player.hand.at(static_cast<size_t>(player.heldCardIndex));
    // //Make sure it is facing up
    // player.cardInPlay.faceUp = true;
    //Then remove that card from the hand.
    player.hand.erase(player.hand.begin() + static_cast<long>(player.heldCardIndex));
    //The player is now not holding any card.
    player.heldCardIndex = -1;
}

int BattleCards(const Card &card1, const Card &card2)
{
    const CardType typeCard1 = card1.type;
    const CardType typeCard2 = card2.type;

    if (typeCard1 == CardType::invalid || typeCard2 == CardType::invalid)
    {
        std::cerr << "Invalid card" << "\n";
        return -1;
    }

    if (typeCard1 == typeCard2) return 0; //draw
    if (typeCard1 == CardType::paper)
    {
        if (typeCard2 == CardType::rock) return 1;
        if (typeCard2 == CardType::scissors) return 2;
    }
    if (typeCard1 == CardType::rock)
    {
        if (typeCard2 == CardType::scissors) return 1;
        if (typeCard2 == CardType::paper) return 2;
    }
    if (typeCard1 == CardType::scissors)
    {
        if (typeCard2 == CardType::paper) return 1;
        if (typeCard2 == CardType::rock) return 2;
    }
    std::cerr << "Something went wrong with the Rock-Paper-Scissors logic." << "\n";
    return -1;
}



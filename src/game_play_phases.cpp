#include "game_play_phases.hpp"
#include <cassert>
#include <iostream>
#include <bits/atomic_timed_wait.h>

#include "audio.hpp"
#include "constants.hpp"
#include "raylib-cpp.hpp"
#include "player.hpp"

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
                break;
            }
            //Wait for P1 to click the draw button enough times
            if (player1.hand.size() != constants::initialHandSize)
            {
                break;
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
                ChangePhase(GameplayPhase::playerTwoPlayingAndPlayerOnePlayed);
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
                ChangePhase(GameplayPhase::playerOnePlayingAndPlayerTwoPlayed);
                break;
            }
            if (playerGoingFirst == 2)
            {
                ChangePhase(GameplayPhase::playerOneFirstTurn);
                break;
            }
            break;
        }
        case GameplayPhase::playerOnePlayingAndPlayerTwoPlayed:
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
                ChangePhase(GameplayPhase::playerTwoPlayingAndPlayerOnePlayed);
                break;
            }
            else
            {
                ChangePhase(GameplayPhase::playerTwoPlayingAndPlayerOnePassed);
                break;
            }
        }
        case GameplayPhase::playerOnePlayingAndPlayerTwoPassed:
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
                ChangePhase(GameplayPhase::playerTwoPlayingAndPlayerOnePlayed);
                break;
            }
            else
            {
                ChangePhase(GameplayPhase::battle); //When both pass in a row, they battle
                break;
            }
        }

        case GameplayPhase::playerTwoPlayingAndPlayerOnePlayed: //not yet implemented
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
                ChangePhase(GameplayPhase::playerOnePlayingAndPlayerTwoPassed);
                break;
            }
            else //Play
            {
                player2.heldCardIndex = 0;
                PutCardInPlay(player2);
                ChangePhase(GameplayPhase::playerOnePlayingAndPlayerTwoPlayed);
                break;
            }
        }
        case GameplayPhase::playerTwoPlayingAndPlayerOnePassed:
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
                ChangePhase(GameplayPhase::playerOnePlayingAndPlayerTwoPlayed);
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
    }
}

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
        case GameplayPhase::playerOneFirstTurn:
            return "Player One First Turn";
        case GameplayPhase::playerTwoFirstTurn:
            return "Player Two First Turn";
        case GameplayPhase::playerOnePlayingAndPlayerTwoPlayed:
            return "Player One Playing And Player Two Played";
        case GameplayPhase::playerTwoPlayingAndPlayerOnePlayed:
            return "Player Two Playing And Player One Played";
        case GameplayPhase::playerOnePlayingAndPlayerTwoPassed:
            return "Player One Playing And Player Two Passed";
        case GameplayPhase::playerTwoPlayingAndPlayerOnePassed:
            return "Player Two Playing And Player One Passed";
        case GameplayPhase::battle:
            return "Battle";
        case GameplayPhase::gameOver:
            return "Game Over";
    }
    return "Error";
}



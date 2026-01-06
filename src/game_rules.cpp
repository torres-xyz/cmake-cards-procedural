#include "game_rules.hpp"

#include <cassert>

#include "card.hpp"
#include "game_play_phases.hpp"
#include "player.hpp"

struct Card;

int CalculateRoundWinner(const Player &player1, const Player &player2)
{
    //Calculate Player 1 Top Stat
    Card player1FinalCard = player1.cardsInPlayStack.at(0);
    assert(player1FinalCard.type == CardType::unit && "Bottom card in player 1's stack was not a Unit.");

    for (std::size_t i = 1; i < player1.cardsInPlayStack.size(); ++i) //starting at 1, since 0 is the bottom Unit.
    {
        player1FinalCard.body += player1.cardsInPlayStack.at(i).body;
        player1FinalCard.mind += player1.cardsInPlayStack.at(i).mind;
        player1FinalCard.soul += player1.cardsInPlayStack.at(i).soul;
    }
    int player1TopStat{player1FinalCard.body};
    if (player1FinalCard.mind > player1TopStat) player1TopStat = player1FinalCard.mind;
    if (player1FinalCard.soul > player1TopStat) player1TopStat = player1FinalCard.soul;

    //Calculate Player 2 Top Stat
    Card player2FinalCard = player2.cardsInPlayStack.at(0);
    assert(player2FinalCard.type == CardType::unit && "Bottom card in player 2's stack was not a Unit.");

    for (std::size_t i = 1; i < player2.cardsInPlayStack.size(); ++i) //starting at 1, since 0 is the bottom Unit.
    {
        player2FinalCard.body += player2.cardsInPlayStack.at(i).body;
        player2FinalCard.mind += player2.cardsInPlayStack.at(i).mind;
        player2FinalCard.soul += player2.cardsInPlayStack.at(i).soul;
    }
    int player2TopStat{player2FinalCard.body};
    if (player2FinalCard.mind > player2TopStat) player2TopStat = player2FinalCard.mind;
    if (player2FinalCard.soul > player2TopStat) player2TopStat = player2FinalCard.soul;

    //Decide the winner
    if (player1TopStat > player2TopStat)
    {
        return 1;
    }
    if (player2TopStat > player1TopStat)
    {
        return 2;
    }
    //Draw
    return 0;
}

bool CanCardBePlayedByPlayer(const Card &selectedCard, const Player &player, [[maybe_unused]] const GameplayPhase &gameplayPhase)
{
    //Only 1 card can be played per turn.

    // Playstack is empty ------------------------------------------------------
    // The first card in the playstack has to be a Unit.
    if (player.cardsInPlayStack.empty())
    {
        if (selectedCard.type == CardType::unit) return true;
        return false;
    }

    // Playstack is NOT empty --------------------------------------------------
    //On the first turn, players can only play 1 unit.

    if (gameplayPhase == GameplayPhase::playerOneFirstTurn)
    {
        return false;
    }

    //then we can assume (but still verify) the first/bottom card is a Unit.
    if (player.cardsInPlayStack.at(0).type == CardType::unit)
    {
        //only Action cards can be played on top of a Unit card:
        if (selectedCard.type == CardType::action) return true;
    }


    return false;
}

bool IsInitialHandValid(const Player &player)
{
    //An initial hand needs to have only one Unit.
    for (const Card &card: player.hand)
    {
        if (card.type == CardType::unit) return true;
    }

    return false;
}

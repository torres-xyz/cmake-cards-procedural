#pragma once
#include "cpu_brain.hpp"
#include "raylib-cpp.hpp"

/* Inline variables have external linkage by default,
 * so that they are visible to the linker.
 * This is necessary so the linker can de-duplicate the definitions.
 */
namespace constants
{
    // Screen
    inline constexpr int screenWidthOriginal{1000};
    inline constexpr int screenHeight{800};
    inline constexpr int imguiSideBarWidth{300};
    inline constexpr int screenWidth{screenWidthOriginal};
    inline constexpr int windowScreenWidth{
#if(DEBUG)
        screenWidthOriginal + imguiSideBarWidth
#else
        1000
#endif
    };
    inline constexpr int windowScreenHeight{screenHeight};

    //CPU Players' Options
    inline constexpr CpuPlayerOptions cpuPlayerOptions
    {
        .actionDelay = 0.25f,
        .autoStartNewRound = false,
    };

    // Card
    inline constexpr int cardWidth{72};
    inline constexpr int cardHeight{102};
    inline constexpr float cardAspectRatio{static_cast<float>(cardWidth) / static_cast<float>(cardHeight)};
    inline constexpr int cardTextureWidth{cardWidth * 10};
    inline constexpr int cardTextureHeight{cardHeight * 10};
    inline constexpr int cardArtTextureWidth{630};
    inline constexpr int cardArtTextureHeight{388};
    inline constexpr int inHandCardWidth{cardWidth * 2};
    inline constexpr int inHandCardHeight{cardHeight * 2};
    inline constexpr float zoomedCardWidth{cardWidth * 3.5};
    inline constexpr float zoomedCardHeight{cardHeight * 3.5};

    // UI
    //// Player 1 HandZone
    inline constexpr int handZonePaddingRight{200};
    inline constexpr int handZoneBottomPadding{20};
    inline constexpr int handZoneHeight{100};
    inline constexpr int handZoneWidth{screenWidth - handZonePaddingRight};
    inline constexpr int handZonePosX{20};
    inline constexpr int handZonePosY{
        screenHeight - handZoneBottomPadding - handZoneHeight
    };
    inline constexpr Rectangle handZoneRec
    {
        handZonePosX,
        handZonePosY,
        handZoneWidth,
        handZoneHeight
    };
    ////Player 2 Hand Zone
    inline constexpr float player2HandZoneWidth{500};
    inline constexpr Rectangle player2HandZone
    {
        constants::screenWidth - player2HandZoneWidth - 20,
        -20,
        player2HandZoneWidth,
        constants::handZoneRec.height
    };

    inline constexpr Color handZoneColor{BLUE};
    //// Players' Scores
    inline constexpr int scoreDisplayPaddingX{20};
    inline constexpr int scoreDisplayPaddingY{20};
    inline constexpr int scoreDisplayFontSize{20};
    inline constexpr Color scoreDisplayColor{GREEN};
    //// Turn owner
    inline constexpr int turnOwnerTextPaddingX{20};
    inline constexpr int turnOwnerTextPaddingY{60};
    inline constexpr int turnOwnerTextFontSize{20};
    inline constexpr Color turnOwnerTextColor{GREEN};
    //// PlayField
    inline constexpr int playfieldWidth{cardWidth * 10 + 10};
    inline constexpr int playfieldHeight{cardHeight * 5 + 10};
    inline constexpr int playfieldPosX{50};
    inline constexpr int playfieldPosY{screenHeight / 2 - playfieldHeight / 2 - 30};
    inline constexpr Rectangle playfieldRec
    {
        playfieldPosX,
        playfieldPosY,
        playfieldWidth,
        playfieldHeight
    };

    inline constexpr Color playfieldColor{SKYBLUE};
    //// Playfield - P1
    inline constexpr Rectangle playerOnePlayfieldCardZoneRect
    {
        playfieldPosX + 10,
        playfieldPosY + 10,
        cardWidth * 3,
        cardHeight * 3
    };
    //// Playfield - P2
    inline constexpr Rectangle playerTwoPlayfieldCardZoneRect
    {
        playfieldPosX - 10 + playfieldWidth - cardWidth * 3,
        playfieldPosY + 10,
        cardWidth * 3,
        cardHeight * 3
    };
}

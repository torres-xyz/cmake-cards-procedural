#include "fonts.hpp"
#include <unordered_map>

raylib::Font const &GetFont(GameFont gameFont)
{
    static const std::unordered_map<GameFont, std::string> gameFontToPathMap
    {
        {GameFont::aobashiOne, "resources/font/AoboshiOne-Regular.ttf"},
    };
    static std::unordered_map<GameFont, raylib::Font> gameFontToFontMap{};

    if (!gameFontToFontMap.contains(gameFont))
    {
        gameFontToFontMap.insert(
            {
                gameFont,
                raylib::Font(gameFontToPathMap.at(gameFont))
            });
        SetTextureFilter(gameFontToFontMap.at(gameFont).texture, TEXTURE_FILTER_BILINEAR);
    }

    return gameFontToFontMap.at(gameFont);
}

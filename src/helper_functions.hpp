#pragma once
#include "raylib-cpp.hpp"

class HelperFunctions
{
public:
    static void DrawTextCenteredInRec(
        const char *text, const float fontSize, const Color color, const Rectangle &rectangle)
    {
        // Raylib gets the spacing by dividing the font size by defaultFontSize (10).
        const float textSpacing = fontSize * 0.1f;

        const raylib::Vector2 textMeasure = MeasureTextEx(
            GetFontDefault(),
            text,
            fontSize,
            textSpacing);

        const raylib::Vector2 centeredTextPos
        {
            rectangle.x + (rectangle.width - textMeasure.x) / 2,
            rectangle.y + (rectangle.height - textMeasure.y) / 2
        };
        raylib::DrawTextEx(GetFontDefault(), text, centeredTextPos, fontSize, textSpacing, color);
    }
};

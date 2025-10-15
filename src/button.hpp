#pragma once
#include <map>

#include "raylib-cpp.hpp"

enum class GameTexture;

enum class ButtonState
{
    disabled,
    enabled,
    hovered,
    pressed
};

struct Button
{
    raylib::Rectangle rectangle{};
    std::string text{};
    float fontSize{};
    GameTexture background{};
    ButtonState state{};
    bool wasPressed{false};
};

void DrawButton(const Button &button, const raylib::Texture2D &texture);

void UpdateButtonState(Button &button, const Vector2 &mousePos, bool lMouseBtnDown, bool lMouseBtnRlsd);

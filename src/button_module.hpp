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
    GameTexture background{};
    ButtonState state{};
    bool wasPressed{false};
};

void DrawButton(const Button &button, const std::map<GameTexture, raylib::Texture2D> &texMap);

void UpdateButtonState(Button &button, const Vector2 &mousePos, bool lMouseBtnDown, bool lMouseBtnRlsd);

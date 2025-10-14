#include "button_module.hpp"
#include <map>

void DrawButton(const Button &button, const std::map<GameTexture, raylib::Texture2D> &texMap)
{
    Color currentColor{MAGENTA};
    switch (button.state)
    {
        case ButtonState::disabled:
        {
            currentColor = BLACK;
            break;
        }
        case ButtonState::enabled:
        {
            currentColor = RAYWHITE;
            break;
        }
        case ButtonState::hovered:
        {
            currentColor = WHITE;
            break;
        }
        case ButtonState::pressed:
        {
            currentColor = GRAY;
            break;
        }
    }

    texMap.at(button.background).Draw(
        button.rectangle,
        button.rectangle,
        {0, 0},
        0,
        currentColor);
}

void UpdateButtonState(Button &button, const Vector2 &mousePos, const bool lMouseBtnDown, const bool lMouseBtnRlsd)
{
    button.wasPressed = false;

    if (CheckCollisionPointRec(mousePos, button.rectangle))
    {
        if (lMouseBtnDown)
        {
            button.state = ButtonState::pressed;
        }
        else
        {
            button.state = ButtonState::hovered;
        }
        if (lMouseBtnRlsd)
        {
            button.wasPressed = true;
        }
    }
    else
    {
        button.state = ButtonState::enabled;
    }
}

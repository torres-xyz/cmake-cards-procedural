#include "button.hpp"
#include <map>
#include "helper_functions.hpp"

void DrawButton(const Button &button, const raylib::Texture2D &texture)
{
    Color currentColor{MAGENTA};
    switch (button.state)
    {
        case ButtonState::disabled:
        {
            currentColor = DARKGRAY;
            break;
        }
        case ButtonState::enabled:
        {
            currentColor = WHITE;
            break;
        }
        case ButtonState::hovered:
        {
            currentColor = LIGHTGRAY;
            break;
        }
        case ButtonState::pressed:
        {
            currentColor = GRAY;
            break;
        }
    }

    //TODO: Remove magic numbers
    //Right now these are hardcoded to match panel_xx.png textures
    constexpr NPatchInfo panelNPatchInfo
    {
        {0, 0, 48, 48},
        12, 12, 12, 12,
        0
    };

    DrawTextureNPatch(texture, panelNPatchInfo, button.rectangle, {0, 0}, 0, currentColor);

    HelperFunctions::DrawTextCenteredInRec(button.text.c_str(), button.fontSize, BLACK, button.rectangle);
}

///
/// @param button Button reference
/// @param mousePos Use GetMousePosition()
/// @param pressed Use IsMouseButtonDown(MOUSE_BUTTON_LEFT)
/// @param released Use IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
void UpdateButtonState(Button &button, const Vector2 &mousePos, const bool pressed, const bool released)
{
    if (button.state == ButtonState::disabled) return;

    button.wasPressed = false;

    if (CheckCollisionPointRec(mousePos, button.rectangle))
    {
        if (pressed)
        {
            button.state = ButtonState::pressed;
        }
        else
        {
            button.state = ButtonState::hovered;
        }
        if (released)
        {
            button.wasPressed = true;
        }
    }
    else
    {
        button.state = ButtonState::enabled;
    }
}

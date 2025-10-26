#include <catch2/catch_test_macros.hpp>
#include "../src/button.hpp"

TEST_CASE("New Button state is set to enabled", "[button]")
{
    const Button buttonSUT{};

    REQUIRE(buttonSUT.state == ButtonState::enabled);
}

TEST_CASE("UpdateButtonState with mouse pressed = true sets button state to pressed", "[button]")
{
    Button buttonSUT
    {
        .rectangle = {0,0, 10, 10}
    };

    constexpr Vector2 mousePos {5,5}; //inside the button rect

    UpdateButtonState(buttonSUT, mousePos, true, false);

    REQUIRE(buttonSUT.state == ButtonState::pressed);
    REQUIRE(buttonSUT.wasPressed == false); //mouse button was not released yet.
}

TEST_CASE("UpdateButtonState with mouse released = true sets .wasPressed to true", "[button]")
{
    Button buttonSUT
    {
        .rectangle = {0,0, 10, 10}
    };

    constexpr Vector2 mousePos {5,5}; //inside the button rect

    UpdateButtonState(buttonSUT, mousePos, false, true);

    REQUIRE(buttonSUT.wasPressed == true);
}

TEST_CASE("UpdateButtonState click outside of button does not change its state", "[button]")
{
    Button buttonSUT
    {
        .rectangle = {0,0, 10, 10}
    };

    constexpr Vector2 mousePos {15,15}; //outside the button rect
    const ButtonState originalState = buttonSUT.state;

    UpdateButtonState(buttonSUT, mousePos, true, false);

    REQUIRE(buttonSUT.state == originalState);
}

TEST_CASE("UpdateButtonState mouse inside button rec sets its state to hovered", "[button]")
{
    Button buttonSUT
    {
        .rectangle = {0,0, 10, 10}
    };

    constexpr Vector2 mousePos {5,5}; //inside the button rect

    UpdateButtonState(buttonSUT, mousePos, false, false);

    REQUIRE(buttonSUT.state == ButtonState::hovered);
}
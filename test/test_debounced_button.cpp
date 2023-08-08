#include "debounced_button.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO("Initialized state", "[debounce]")
{
    BUTTON_T test_button;
    GIVEN("An initialized button")
    {
        BUTTON_Initialize(&test_button);
        THEN("The button should not be pressed")
        {
            REQUIRE(BUTTON_INITIALIZED == test_button.state);
        }
        THEN("Debounce starts as 0x55")
        {
            REQUIRE(0x55 == test_button.debounce);
        }
    }
}

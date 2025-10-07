#include "debounced_button.h"

#include <catch2/catch_test_macros.hpp>

// User defines polarity of input for any given button.
#define IO_PRESSED (0U)
#define IO_RELEASED (1U)

SCENARIO("Initial behavior", "[debounce]")
{
    BUTTON_T test_button;
    GIVEN("An initialized button")
    {
        BUTTON_Initialize(&test_button, 8);
        THEN("The button should not be pressed")
        {
            REQUIRE(BUTTON_INITIALIZED == test_button.state);
        }
        THEN("Debounce starts as 0")
        {
            REQUIRE(0 == test_button.debounce_count);
        }
        WHEN("The button is pressed")
        {
            for (int i = 0; i < 8; i++) {
                BUTTON_Debounce(IO_PRESSED, &test_button);
            }
            THEN("The state goes low")
            {
                REQUIRE(IO_PRESSED == test_button.state);
            }
            THEN("The current state is returned from the debounce function")
            {
                REQUIRE(IO_PRESSED
                        == BUTTON_Debounce(IO_PRESSED, &test_button));
            }
        }
        WHEN("The button is released")
        {
            for (int i = 0; i < 8; i++) {
                BUTTON_Debounce(IO_RELEASED, &test_button);
            }
            THEN("The state goes low")
            {
                REQUIRE(IO_RELEASED == test_button.state);
            }
        }
    }
}

SCENARIO("State Transitions", "[debounce]")
{
    BUTTON_T test_button;
    BUTTON_Initialize(&test_button, 8);
    GIVEN("A pressed button")
    {
        for (int i = 0; i < 8; i++) {
            BUTTON_Debounce(IO_PRESSED, &test_button);
        }
        THEN("We have an initial edge")
        {
            CHECK(IO_PRESSED == test_button.state);
            REQUIRE(EDGE_INITIAL == test_button.edge);
        }
        WHEN("The button is released")
        {
            for (int i = 0; i < 8; i++) {
                BUTTON_Debounce(IO_RELEASED, &test_button);
            }
            THEN("The state changes to released")
            {
                REQUIRE(IO_RELEASED == test_button.state);
            }
            THEN("The edge changes to released")
            {
                REQUIRE(IO_RELEASED == test_button.edge);
            }
            WHEN("The button remains released")
            {
                BUTTON_Debounce(IO_RELEASED, &test_button);
                THEN("The state stays released")
                {
                    REQUIRE(IO_RELEASED == test_button.state);
                }
                THEN("The edge changes to none")
                {
                    REQUIRE(EDGE_NONE == test_button.edge);
                }
            }
            WHEN("The button first switches to pressed")
            {
                BUTTON_Debounce(IO_PRESSED, &test_button);
                THEN("The state stays released")
                {
                    REQUIRE(IO_RELEASED == test_button.state);
                }
                THEN("The edge changes to none")
                {
                    REQUIRE(EDGE_NONE == test_button.edge);
                }
            }
        }
    }
    GIVEN("A released button")
    {
        for (int i = 0; i < 8; i++) {
            BUTTON_Debounce(IO_RELEASED, &test_button);
        }
        THEN("We have an initial edge")
        {
            CHECK(IO_RELEASED == test_button.state);
            REQUIRE(EDGE_INITIAL == test_button.edge);
        }
        WHEN("The button is pressed")
        {
            for (int i = 0; i < 8; i++) {
                BUTTON_Debounce(IO_PRESSED, &test_button);
            }
            THEN("The state changes to pressed")
            {
                REQUIRE(IO_PRESSED == test_button.state);
            }
            THEN("The edge changes to pressed")
            {
                REQUIRE(IO_PRESSED == test_button.edge);
            }
            WHEN("The button remains pressed")
            {
                BUTTON_Debounce(IO_PRESSED, &test_button);
                THEN("The state stays pressed")
                {
                    REQUIRE(IO_PRESSED == test_button.state);
                }
                THEN("The edge changes to none")
                {
                    REQUIRE(EDGE_NONE == test_button.edge);
                }
            }
            WHEN("The button first switches to released")
            {
                BUTTON_Debounce(IO_RELEASED, &test_button);
                THEN("The state stays released")
                {
                    REQUIRE(IO_PRESSED == test_button.state);
                }
                THEN("The edge changes to none")
                {
                    REQUIRE(EDGE_NONE == test_button.edge);
                }
            }
        }
    }
}

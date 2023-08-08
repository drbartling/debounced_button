
#include "debounced_button.h"
#include "unity.h"

// User defines polarity of input for any given button.
#define IO_PRESSED (0U)
#define IO_RELEASED (1U)

BUTTON_T testButton;

void
setUp(void)
{
    BUTTON_Initialize(&testButton);
}

void
tearDown(void)
{
}

void
test_Initialize_should_MakeButtonNotPressed(void)
{
    BUTTON_Initialize(&testButton);
    TEST_ASSERT_EQUAL(BUTTON_INITIALIZED, testButton.state);
}

void
test_Initialize_should_MakeDebounce0x55(void)
{
    BUTTON_Initialize(&testButton);
    TEST_ASSERT_EQUAL_HEX(0x55, testButton.debounce);
}

void
test_Debounce_should_ChangeStateToHigh_when_InputIsConsistently1(void)
{
    BUTTON_STATE_T state;
    (void)BUTTON_Debounce(IO_RELEASED, &testButton);
    int i;
    for (i = 0; i < 7; i++) {
        state = BUTTON_Debounce(IO_PRESSED, &testButton);
        TEST_ASSERT_EQUAL(BUTTON_INITIALIZED, state);
    }
    state = BUTTON_Debounce(IO_PRESSED, &testButton);
    TEST_ASSERT_EQUAL(IO_PRESSED, state);
}

void
test_Debounce_should_ChangeStateToLow_when_InputIsConsistently0(void)
{
    BUTTON_STATE_T state;
    (void)BUTTON_Debounce(IO_PRESSED, &testButton);
    int i;
    for (i = 0; i < 7; i++) {
        state = BUTTON_Debounce(IO_RELEASED, &testButton);
        TEST_ASSERT_EQUAL(BUTTON_INITIALIZED, state);
    }
    state = BUTTON_Debounce(IO_RELEASED, &testButton);
    TEST_ASSERT_EQUAL(IO_RELEASED, state);
}

void
test_Debounce_should_Take8ConsistentInputsToChangeStateFromHighToLow(void)
{
    BUTTON_STATE_T state;
    testButton.state    = IO_RELEASED; // Force state to "button released"
    testButton.debounce = testButton.state ? 0xFFU : 0x00U; // Force debounce to
                                                            // match state.

    int i;
    for (i = 0; i < 7; i++) {
        // state remains unchanged for 7 readings
        state = BUTTON_Debounce(IO_PRESSED, &testButton);
        TEST_ASSERT_EQUAL(IO_RELEASED, state);
    }
    // And on the eighth consistent reading, the state changes.
    state = BUTTON_Debounce(IO_RELEASED, &testButton);
    TEST_ASSERT_EQUAL(IO_RELEASED, state);
}

void
test_Debounce_should_Take8ConsistentInputsToChangeStateFromLowToHigh(void)
{
    BUTTON_STATE_T state;
    testButton.state    = IO_PRESSED; // Force state to "button released"
    testButton.debounce = testButton.state ? 0xFFU : 0x00U; // Force debounce to
                                                            // match state.

    int i;
    for (i = 0; i < 7; i++) {
        // state remains unchanged for 7 readings
        state = BUTTON_Debounce(IO_RELEASED, &testButton);
        TEST_ASSERT_EQUAL(IO_PRESSED, state);
    }
    // And on the eighth consistent reading, the state changes.
    state = BUTTON_Debounce(IO_PRESSED, &testButton);
    TEST_ASSERT_EQUAL(IO_PRESSED, state);
}

void
test_Edge_should_beNone_when_intialized(void)
{
    BUTTON_Initialize(&testButton);
    TEST_ASSERT_EQUAL_MESSAGE(EDGE_NONE, testButton.edge,
                              "edge should be \"None\"");
}

void
test_Edge_should_beRising_when_stateChangesFromLowToHigh(void)
{
    testButton.state    = BUTTON_LOW;
    testButton.debounce = 0x7F;

    (void)BUTTON_Debounce(IO_RELEASED, &testButton);
    TEST_ASSERT_EQUAL_MESSAGE(EDGE_RISING, testButton.edge,
                              "edge should be \"Rising\"");
}

void
test_Edge_should_beFalling_when_stateChangesFromHighToLow(void)
{
    testButton.state    = BUTTON_HIGH;
    testButton.debounce = 0x80;

    (void)BUTTON_Debounce(IO_PRESSED, &testButton);
    TEST_ASSERT_EQUAL_MESSAGE(EDGE_FALLING, testButton.edge,
                              "edge should be \"Falling\"");
}

void
test_Edge_should_beNone_when_stateRemainsLow(void)
{
    testButton.state    = BUTTON_LOW;
    testButton.debounce = 0x00;
    testButton.edge     = EDGE_FALLING;

    (void)BUTTON_Debounce(IO_PRESSED, &testButton);
    TEST_ASSERT_EQUAL_MESSAGE(EDGE_NONE, testButton.edge,
                              "edge should be \"None\"");
}

void
test_Edge_should_beNone_when_stateRemainsHigh(void)
{
    testButton.state    = BUTTON_HIGH;
    testButton.debounce = 0xFF;
    testButton.edge     = EDGE_RISING;

    (void)BUTTON_Debounce(IO_RELEASED, &testButton);
    TEST_ASSERT_EQUAL_MESSAGE(EDGE_NONE, testButton.edge,
                              "edge should be \"None\"");
}

void
test_Edge_should_matchTheButtonStateValue_when_theStateChanges(void)
{
    TEST_ASSERT_EQUAL(BUTTON_LOW, EDGE_FALLING);
    TEST_ASSERT_EQUAL(BUTTON_HIGH, EDGE_RISING);

    // This allows the user to use the same defines for state and edge checking
    // Example code not relevent to the test:
    if (IO_PRESSED == testButton.state) {
        // Do stuff when ever the button is held.
    }
    if (IO_PRESSED == testButton.edge) {
        // Do stuff only when the button is first pressed.
    }
    if (IO_RELEASED == testButton.state) {
        // Do stuff when ever the button is not pressed.
    }
    if (IO_RELEASED == testButton.edge) {
        // Do stuff only when the button is let go of.
    }
}

int
main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Initialize_should_MakeButtonNotPressed);
    RUN_TEST(test_Initialize_should_MakeDebounce0x55);
    RUN_TEST(test_Debounce_should_ChangeStateToHigh_when_InputIsConsistently1);
    RUN_TEST(test_Debounce_should_ChangeStateToLow_when_InputIsConsistently0);
    RUN_TEST(
        test_Debounce_should_Take8ConsistentInputsToChangeStateFromHighToLow);
    RUN_TEST(
        test_Debounce_should_Take8ConsistentInputsToChangeStateFromLowToHigh);
    RUN_TEST(test_Edge_should_beNone_when_intialized);
    RUN_TEST(test_Edge_should_beRising_when_stateChangesFromLowToHigh);
    RUN_TEST(test_Edge_should_beFalling_when_stateChangesFromHighToLow);
    RUN_TEST(test_Edge_should_beNone_when_stateRemainsLow);
    RUN_TEST(test_Edge_should_beNone_when_stateRemainsHigh);
    RUN_TEST(test_Edge_should_matchTheButtonStateValue_when_theStateChanges);
    return UNITY_END();
}

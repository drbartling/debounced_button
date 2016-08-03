
#include "unity.h"
#include "debounced_button.h"

#define IO_PRESSED (0U)
#define IO_RELEASED (1U)

BUTTON_T testButton;

void setUp(void) {
    BUTTON_Initialize(&testButton);
}

void tearDown(void) {
}

void test_Initialize_should_MakeButtonNotPressed(void) {
    BUTTON_Initialize(&testButton);
    TEST_ASSERT_EQUAL(BUTTON_INITIALIZED, testButton.state);
}

void test_Initialize_should_MakeDebounce0x55(void) {
    BUTTON_Initialize(&testButton);
    TEST_ASSERT_EQUAL_HEX(0x55, testButton.debounce);
}

void test_Debounce_should_ChangeStateToHigh_when_InputIsConsistently1(void) {
    BUTTON_STATE_T state;
    (void) BUTTON_Debounce(IO_RELEASED, &testButton);
    int i;
    for (i = 0; i < 7; i++) {
        state = BUTTON_Debounce(IO_PRESSED, &testButton);
        TEST_ASSERT_EQUAL(BUTTON_INITIALIZED, state);
    }
    state = BUTTON_Debounce(IO_PRESSED, &testButton);
    TEST_ASSERT_EQUAL(IO_PRESSED, state);
}

void test_Debounce_should_ChangeStateToLow_when_InputIsConsistently0(void) {
    BUTTON_STATE_T state;
    (void) BUTTON_Debounce(IO_PRESSED, &testButton);
    int i;
    for (i = 0; i < 7; i++) {
        state = BUTTON_Debounce(IO_RELEASED, &testButton);
        TEST_ASSERT_EQUAL(BUTTON_INITIALIZED, state);
    }
    state = BUTTON_Debounce(IO_RELEASED, &testButton);
    TEST_ASSERT_EQUAL(IO_RELEASED, state);
}

void test_Debounce_should_Take8ConsistentInputsToChangeStateFromHighToLow(void) {
    BUTTON_STATE_T state;
    testButton.state = IO_RELEASED; // Force state to "button released"
    testButton.debounce = testButton.state ? 0xFFU : 0x00U; // Force debounce to match state.

    int i;
    for (i = 0; i < 7; i++) {
        //state remains unchanged for 7 readings
        state = BUTTON_Debounce(IO_PRESSED, &testButton);
        TEST_ASSERT_EQUAL(IO_RELEASED, state);
    }
    // And on the eighth consistent reading, the state changes.
    state = BUTTON_Debounce(IO_RELEASED, &testButton);
    TEST_ASSERT_EQUAL(IO_RELEASED, state);
}

void test_Debounce_should_Take8ConsistentInputsToChangeStateFromLowToHigh(void) {
    BUTTON_STATE_T state;
    testButton.state = IO_PRESSED; // Force state to "button released"
    testButton.debounce = testButton.state ? 0xFFU : 0x00U; // Force debounce to match state.

    int i;
    for (i = 0; i < 7; i++) {
        //state remains unchanged for 7 readings
        state = BUTTON_Debounce(IO_RELEASED, &testButton);
        TEST_ASSERT_EQUAL(IO_PRESSED, state);
    }
    // And on the eighth consistent reading, the state changes.
    state = BUTTON_Debounce(IO_PRESSED, &testButton);
    TEST_ASSERT_EQUAL(IO_PRESSED, state);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Initialize_should_MakeButtonNotPressed);
    RUN_TEST(test_Initialize_should_MakeDebounce0x55);
    RUN_TEST(test_Debounce_should_ChangeStateToHigh_when_InputIsConsistently1);
    RUN_TEST(test_Debounce_should_ChangeStateToLow_when_InputIsConsistently0);
    RUN_TEST(test_Debounce_should_Take8ConsistentInputsToChangeStateFromHighToLow);
    RUN_TEST(test_Debounce_should_Take8ConsistentInputsToChangeStateFromLowToHigh);
    return UNITY_END();
}

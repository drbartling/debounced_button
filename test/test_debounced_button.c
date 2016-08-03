
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

void test_Debounce_should_ChangeStateToPressed_when_ButtonIsPressedForAWhile(void) {
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

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Initialize_should_MakeButtonNotPressed);
    RUN_TEST(test_Initialize_should_MakeDebounce0x55);
    RUN_TEST(test_Debounce_should_ChangeStateToPressed_when_ButtonIsPressedForAWhile);
    return UNITY_END();
}

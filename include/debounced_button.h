/**
 * @file debounced_button.h
 * @author D. Ryan Bartling
 * @brief Debounces a button
 *
 * Provides simple interface with which to read a button and debounce the
 * result.
 */

/*******************************************************************************
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 D. Ryan Bartling
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#ifndef DEBOUNCED_BUTTON_H
#define DEBOUNCED_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * Button press states.
 */
typedef enum {
    BUTTON_LOW  = 0,
    BUTTON_HIGH = 1,
    BUTTON_INITIALIZED,
    BUTTON_ERROR,
} BUTTON_STATE_T;

/**
 * Edge detection states
 */
typedef enum {
    EDGE_FALLING = 0, ///< Button state has transitioned from high to low
    EDGE_RISING  = 1, ///< Button state transitioned from low to high
    EDGE_NONE, ///< Button state did not change during last call to Debounce()
} BUTTON_EDGE_T;

/**
 * Structure for button object used by API functions.
 */
typedef struct BUTTON_S {
    int            debounce_count;
    int            debounce_limit;
    BUTTON_STATE_T state;
    BUTTON_EDGE_T  edge;
} BUTTON_T;

/**
 * Function takes a button (I/O) reading and debounces it.
 * @param reading - direct reading of I/O pin.
 * @param button - the button object to handle the debouncing.
 * @returns the current button state
 * @see BUTTON_STATE_T
 */
BUTTON_STATE_T BUTTON_Debounce(uint8_t reading, BUTTON_T *button);

/**
 * Initializes a button.  Initial state is not pressed or released.
 * @param button object to initialize.
 * @param debounce_limit Number of consecutive identical samples required to
 * change state
 */
void BUTTON_Initialize(BUTTON_T *button, int debounce_limit);

#ifdef __cplusplus
}
#endif
#endif // DEBOUNCED_BUTTON_H

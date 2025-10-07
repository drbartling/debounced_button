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
 * Copyright (c) 2016-2024 D. Ryan Bartling
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

#include "debounced_button.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

BUTTON_STATE_T
BUTTON_Debounce(uint8_t reading, BUTTON_T *button)
{
    reading = reading ? 1U : 0U;

    if (reading == button->state) {
        button->debounce_count = 0;
    } else {
        button->debounce_count++;
    }

    assert(button->debounce_count <= button->debounce_limit);

    if (button->debounce_count == button->debounce_limit) {
        button->edge =
            button->state == BUTTON_INITIALIZED ? EDGE_INITIAL : reading;
        button->state          = reading;
        button->debounce_count = 0;
    } else {
        button->edge = EDGE_NONE;
    }
    return button->state;
}

void
BUTTON_Initialize(BUTTON_T *button, int debounce_limit)
{
    button->debounce_count = 0;
    button->debounce_limit = debounce_limit;
    button->state          = BUTTON_INITIALIZED;
    button->edge           = EDGE_NONE;
}

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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

//
// Section: Included Files
//

// TODO: Update header file
#include "debounced_button.h"
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

//
// Section: Debounced Button API
//

BUTTON_STATE_T BUTTON_Debounce(uint8_t reading, BUTTON_T *button) {
    reading = reading ? 1U : 0U;
    button->debounce <<= 1U;
    button->debounce |= reading;
    switch (button->state) {
        case BUTTON_LOW:
            if (0xFFU == button->debounce) {
                button->state = BUTTON_HIGH;
            }
            break;
        case BUTTON_HIGH:
            if (0x00U == button->debounce) {
                button->state = BUTTON_LOW;
            }
            break;
        case BUTTON_INITIALIZED:
            switch (button->debounce) {
                case 0x00U:
                    button->state = BUTTON_LOW;
                    break;
                case 0xFFU:
                    button->state = BUTTON_HIGH;
                    break;
                default:
                    break;
            }
            break;
        default:
            button->state = BUTTON_ERROR;
            break;
    }
    return button->state;
}

void BUTTON_Initialize(BUTTON_T *button) {
    button->debounce = 0x55U;
    button->state = BUTTON_INITIALIZED;
}

//
// End of File
//

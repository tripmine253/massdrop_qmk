/* Copyright 2017 REPLACE_WITH_YOUR_NAME
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "simontester.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = KEYMAP(
        KC_1,  F(2),   \
        F(0),  F(1) \
    ),
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
    static bool backlight;

    switch (id) {
        case 0:
            palSetPad(GPIOA, 6);
            palSetPad(GPIOA, 3);
            palSetPad(GPIOA, 15);
            palSetPad(GPIOB, 5);
            backlight = false;
            break;
        case 1:
            palClearPad(GPIOA, 6);
            palClearPad(GPIOA, 3);
            palClearPad(GPIOA, 15);
            palClearPad(GPIOB, 5);
            backlight = true;
            break;
        case 2:
            if (backlight) {
                palSetPad(GPIOA, 6);
                palSetPad(GPIOA, 3);
                palSetPad(GPIOA, 15);
                palSetPad(GPIOB, 5);
                backlight = false;
            } else {
                palClearPad(GPIOA, 6);
                palClearPad(GPIOA, 3);
                palClearPad(GPIOA, 15);
                palClearPad(GPIOB, 5);
                backlight = true;
            }
    }
}

/* Copyright 2017 skully <skullydazed@gmail.com>
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
#include "clueboard2.h"
#include "printf.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = KEYMAP(
        F(0),  F(1)
    ),
};

const uint16_t PROGMEM fn_actions[] = {
  [0] = ACTION_FUNCTION(0),
  [1] = ACTION_FUNCTION(1),
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
    /* Turn LEDs on and off as the button is pressed.
     */
    if (record->event.pressed) {
        switch (id) {
            case 0:
                palSetPad(GPIOA, 4);
                break;
            case 1:
                palSetPad(GPIOB, 7);
                break;
        }
    } else {
        switch (id) {
            case 0:
                palClearPad(GPIOA, 4);
                break;
            case 1:
                palClearPad(GPIOB, 7);
                break;
        }
    }

/*
    switch (id) {
        case 0:
            print("Turning backlight off.\n");
            palClearPad(GPIOA, 4);
            palClearPad(GPIOB, 7);
            break;
        case 1:
            print("Turning backlight on.\n");
            palSetPad(GPIOA, 4);
            palSetPad(GPIOB, 7);
            break;
    }
*/
}

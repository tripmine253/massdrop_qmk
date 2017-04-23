/*
 * Copyright 2017 skully <skullydazed@gmail.com>
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

#include "hal.h"
#include "backlight.h"
#include "led.h"
#include "print.h"

void backlight_init_ports(void) {
    print("backlight_init_ports()\n");
/*
    palSetPadMode(GPIOA, 6,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 3,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 15, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 5,  PAL_MODE_OUTPUT_PUSHPULL);

    // Set them high to turn off the LEDs
    palClearPad(GPIOA, 6);
    //palSetPad(GPIOA, 6);
    palSetPad(GPIOA, 3);
    palSetPad(GPIOA, 15);
    palSetPad(GPIOB, 5);
*/
}

void backlight_set(uint8_t level) {
    printf("backlight_set(%d)\n", level);
/*
    if (level == 0) {
        palSetPad(GPIOA, 6);
        palSetPad(GPIOA, 3);
        palSetPad(GPIOA, 15);
        palSetPad(GPIOB, 5);
    } else {
        palClearPad(GPIOA, 6);
        palClearPad(GPIOA, 3);
        palClearPad(GPIOA, 15);
        palClearPad(GPIOB, 5);
    }
*/
}

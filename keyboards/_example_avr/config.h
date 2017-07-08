/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
          2017 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

#define VENDOR_ID       0x1234
#define PRODUCT_ID      0x5678
#define DEVICE_VER      0x0001
#define MANUFACTURER    Example Company
#define PRODUCT         Example Keyboard
#define DESCRIPTION     A short description

#define MATRIX_ROWS 2
#define MATRIX_COLS 2

#define MATRIX_ROW_PINS { F0, F5 }
#define MATRIX_COL_PINS { F1, F7 }
#define UNUSED_PINS

#define MATRIX_HAS_GHOST // define is matrix has ghost (unlikely)
#define DIODE_DIRECTION COL2ROW // COL2ROW or ROW2COL - how your matrix is configured
// COL2ROW means the black mark on your diode is facing to the rows, and between the switch and the rows

// #define AUDIO_VOICES // turns on the alternate audio voices (to cycle through)
// #define C6_AUDIO // enables audio on pin C6
// #define B5_AUDIO // enables audio on pin B5 (duophony is enable if both are enabled)

#define BACKLIGHT_PIN B7 // pin of the backlight - B5, B6, B7 use PWM, others use softPWM
#define BACKLIGHT_LEVELS 3 // number of levels your backlight will have (not including off)

#define DEBOUNCING_DELAY 5 // the delay when reading the value of the pin (5 is default)

// #define LOCKING_SUPPORT_ENABLE // mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap
// #define LOCKING_RESYNC_ENABLE // tries to keep switch state consistent with keyboard LED state

// key combination that allows the use of magic commands (useful for debugging)
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

// the following options can save on file size at the expense of that feature
// #define NO_DEBUG // disable debuging (saves on file size)
// #define NO_PRINT // disable printing (saves of file size)
// #define NO_ACTION_LAYER // no layers
// #define NO_ACTION_TAPPING // no tapping for layers/mods
// #define NO_ACTION_ONESHOT // no oneshot for layers/mods
// #define NO_ACTION_MACRO // no macros
// #define NO_ACTION_FUNCTION // no functions

// #define FORCE_NKRO // NKRO by default requires to be turned on, this forces it to be on always

// #define PREVENT_STUCK_MODIFIERS // when switching layers, this will release all mods

// #define TAPPING_TERM 200 // how long before a tap becomes a hold
// #define TAPPING_TOGGLE 2 // how many taps before triggering the toggle

// #define PERMISSIVE_HOLD // makes tap and hold keys work better for fast typers who don't want tapping term set above 500

// #define LEADER_TIMEOUT 300 // how long before the leader key times out

// #define ONESHOT_TIMEOUT 300 // how long before oneshot times out
// #define ONESHOT_TAP_TOGGLE 2 // how many taps before oneshot toggle is triggered

// #define IGNORE_MOD_TAP_INTERRUPT // makes it possible to do rolling combos (zx) with keys that convert to other keys on hold

// ws2812 options
// #define RGB_DI_PIN D7 // pin the DI on the ws2812 is hooked-up to
// #define RGBLIGHT_ANIMATIONS // run RGB animations
// #define RGBLED_NUM 15 // number of LEDs
// #define RGBLIGHT_HUE_STEP 12 // units to step when in/decreasing hue
// #define RGBLIGHT_SAT_STEP 25 // units to step when in/decresing saturation
// #define RGBLIGHT_VAL_STEP 12 // units to step when in/decreasing value (brightness)

// #define RGBW_BB_TWI // bit-bangs twi to EZ RGBW LEDs (only required for Ergodox EZ)

// mousekey options (self-describing)
// #define MOUSEKEY_INTERVAL 20
// #define MOUSEKEY_DELAY 0
// #define MOUSEKEY_TIME_TO_MAX 60
// #define MOUSEKEY_MAX_SPEED 7
// #define MOUSEKEY_WHEEL_DELAY 0

#endif

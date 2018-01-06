#include "1.h"
#include "print.h"

#define BUTTON SAFE_RANGE

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    //[0] = LAYOUT(BL_STEP)
    //[0] = LAYOUT(KC_1)
    [0] = LAYOUT(BUTTON)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case BUTTON:
      if (record->event.pressed) {
        print("BUTTON was pressed!\n");
        palClearPad(GPIOB, 5);
        palClearPad(GPIOB, 3);
        palClearPad(GPIOA, 2);
      } else {
        print("BUTTON was released!\n");
        palSetPad(GPIOB, 5);
        palSetPad(GPIOB, 3);
        palSetPad(GPIOA, 2);
      }
      return false;
  }
  return true;
}

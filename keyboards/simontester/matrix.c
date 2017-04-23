#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "timer.h"
#include "wait.h"
#include "printf.h"
#include "backlight.h"
#include "matrix.h"


/* Clueboard SimonTester
 *
 * Column pins are input with internal pull-down. 
 * Row pins are output and strobe with high.
 * Key is high or 1 when it turns on.
 *
 *     col: { PA5, PA4, PB6, PB3 }
 *     row: { PA7 }
 */
/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;


void matrix_init(void) {
    printf("matrix init\n");
    //debug_matrix = true;

    /* Column(sense) */
    palSetPadMode(GPIOA, 5,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, 4,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 6,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 3,  PAL_MODE_INPUT_PULLDOWN);

    /* Row(strobe) */
    palSetPadMode(GPIOA, 7,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOA, 7);  // Only one row, so leave it in strobe state all the time.
                          // I did it this way so hardware hackers would have another
                          // pin to play with.

    memset(matrix, 0, MATRIX_ROWS);
    memset(matrix_debouncing, 0, MATRIX_ROWS);

    /* Setup the backlight */
    palSetPadMode(GPIOA, 6,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 3,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 15, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 5,  PAL_MODE_OUTPUT_PUSHPULL);

    // Set them high to turn off the LEDs
    palClearPad(GPIOA, 6);
    palSetPad(GPIOA, 3);
    palSetPad(GPIOA, 15);
    palSetPad(GPIOB, 5);
}

uint8_t matrix_scan(void) {
    matrix_row_t data = 0;

    // read col data: { PA5, PA4, PB6, PB3 }
    data = (palReadPad(GPIOA, 5) |
        (palReadPad(GPIOA, 4) << 1) |
        (palReadPad(GPIOB, 6) << 2) |
        (palReadPad(GPIOB, 3) << 3));

    if (matrix_debouncing[0] != data) {
        matrix_debouncing[0] = data;
        debouncing = true;
        debouncing_time = timer_read();
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        matrix[0] = matrix_debouncing[0];
        debouncing = false;
    }

    return 1;
}

bool matrix_is_on(uint8_t row, uint8_t col) {
    return (matrix[row] & (1<<col));
}

matrix_row_t matrix_get_row(uint8_t row) {
    return matrix[row];
}

void matrix_print(void) {
    printf("\nr/c 01234567\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        printf("%X0: ", row);
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                printf("1");
            else
                printf("0");
        }
        printf("\n");
    }
}

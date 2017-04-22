#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "timer.h"
#include "wait.h"
#include "print.h"
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
//debug_matrix = true;
    /* Column(sense) */
    palSetPadMode(GPIOA, 5,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, 4,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 7,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, 3,  PAL_MODE_INPUT_PULLDOWN);

    /* Row(strobe) */
    palSetPadMode(GPIOA, 7,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOA, 7);  // Only one row, so leave it in strobe state all the time
                          // I did it this way so hardware hackers would have another
                          // pin to play with.

    memset(matrix, 0, MATRIX_ROWS);
    memset(matrix_debouncing, 0, MATRIX_ROWS);
}

uint8_t matrix_scan(void) {
    for (int row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t data = 0;

        // read col data: { PA5, PA4, PB6, PB3 }
        data = (palReadPad(GPIOA, 5) |
                   palReadPad(GPIOA, 4) |
                   palReadPad(GPIOB, 6) |
                   palReadPad(GPIOB, 3));

        if (matrix_debouncing[row] != data) {
            matrix_debouncing[row] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < MATRIX_ROWS; row++) {
            matrix[row] = matrix_debouncing[row];
        }
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
    xprintf("\nr/c 01234567\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%X0: ", row);
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                xprintf("1");
            else
                xprintf("0");
        }
        xprintf("\n");
    }
}

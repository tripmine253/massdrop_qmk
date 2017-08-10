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
#include "clueboard60.h"
#include "ch.h"
#include "hal.h"

/*
 * DAC streaming callback.
 */
size_t nx = 0, ny = 0, nz = 0;


static void gpt_cb6(GPTDriver *gptp);
static void gpt_cb7(GPTDriver *gptp);
static void gpt_cb8(GPTDriver *gptp);
float frequency[2] = { 440.00, 523.25 };

/*
 * GPT6 configuration.
 */
GPTConfig gpt6cfg1 = {
  .frequency    = 440.00,
  // .frequency    = 1000000,
  .callback     = gpt_cb6,
  // .callback     = NULL,
  .cr2          = TIM_CR2_MMS_1,    /* MMS = 010 = TRGO on Update Event.    */
  .dier         = 0U
};

GPTConfig gpt7cfg1 = {
  .frequency    = 523.25,
  .callback     = gpt_cb7,
  // .callback     = NULL,
  .cr2          = TIM_CR2_MMS_1,    /* MMS = 010 = TRGO on Update Event.    */
  .dier         = 0U
};

GPTConfig gpt8cfg1 = {
  .frequency    = 44100,
  .callback     = gpt_cb8,
  .cr2          = TIM_CR2_MMS_1,    /* MMS = 010 = TRGO on Update Event.    */
  .dier         = 0U
};

static void gpt_cb6(GPTDriver *gptp) {
    palTogglePad(GPIOA, 4);

  // nz++;

  // if ((nz % 1000) == 0) {
  //   palTogglePad(GPIOB, 7);
  //   nz = 0;
  //   gpt6cfg1.frequency *= 1.05946;
  //   gptStopTimer(&GPTD6);

  //   gptStart(&GPTD6, &gpt6cfg1);
  //   gptStartContinuous(&GPTD6, 2U);
    
  //   gpt7cfg1.frequency *= 1.05946;
  //   gptStopTimer(&GPTD7);

  //   gptStart(&GPTD7, &gpt7cfg1);
  //   gptStartContinuous(&GPTD7, 2U);
  // }
}

static void gpt_cb7(GPTDriver *gptp) {
    palTogglePad(GPIOA, 5);

  // ny++;

  // if ((ny % 1000) == 0) {
  //   palTogglePad(GPIOB, 7);
  //   ny = 0;
  // }
}

static void gpt_cb8(GPTDriver *gptp) {

  nz++;

  if (nz == 5000) {
    palTogglePad(GPIOB, 7);
    nz = 0;
    frequency[0] *= 1.05946;
    frequency[1] *= 1.05946;

    gpt6cfg1.frequency = frequency[0];
    gptStopTimer(&GPTD6);

    gptStart(&GPTD6, &gpt6cfg1);
    gptStartContinuous(&GPTD6, 2U);


    gpt7cfg1.frequency = frequency[1];
    gptStopTimer(&GPTD7);

    gptStart(&GPTD7, &gpt7cfg1);
    gptStartContinuous(&GPTD7, 2U);
  }
}


void matrix_init_kb(void) {

  palSetPadMode(GPIOA, 4, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);

  gptStart(&GPTD6, &gpt6cfg1);
  gptStart(&GPTD7, &gpt7cfg1);
  gptStart(&GPTD8, &gpt8cfg1);

  gptStartContinuous(&GPTD6, 2U);  
  gptStartContinuous(&GPTD7, 2U);
  gptStartContinuous(&GPTD8, 2U);
}

void matrix_scan_kb(void) {

}

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

#define DAC_BUFFER_SIZE 360

/*
 * DAC test buffer (sine wave).
 */
static const dacsample_t dac_buffer[DAC_BUFFER_SIZE] = {
  2047, 2082, 2118, 2154, 2189, 2225, 2260, 2296, 2331, 2367, 2402, 2437,
  2472, 2507, 2542, 2576, 2611, 2645, 2679, 2713, 2747, 2780, 2813, 2846,
  2879, 2912, 2944, 2976, 3008, 3039, 3070, 3101, 3131, 3161, 3191, 3221,
  3250, 3278, 3307, 3335, 3362, 3389, 3416, 3443, 3468, 3494, 3519, 3544,
  3568, 3591, 3615, 3637, 3660, 3681, 3703, 3723, 3744, 3763, 3782, 3801,
  3819, 3837, 3854, 3870, 3886, 3902, 3917, 3931, 3944, 3958, 3970, 3982,
  3993, 4004, 4014, 4024, 4033, 4041, 4049, 4056, 4062, 4068, 4074, 4078,
  4082, 4086, 4089, 4091, 4092, 4093, 4094, 4093, 4092, 4091, 4089, 4086,
  4082, 4078, 4074, 4068, 4062, 4056, 4049, 4041, 4033, 4024, 4014, 4004,
  3993, 3982, 3970, 3958, 3944, 3931, 3917, 3902, 3886, 3870, 3854, 3837,
  3819, 3801, 3782, 3763, 3744, 3723, 3703, 3681, 3660, 3637, 3615, 3591,
  3568, 3544, 3519, 3494, 3468, 3443, 3416, 3389, 3362, 3335, 3307, 3278,
  3250, 3221, 3191, 3161, 3131, 3101, 3070, 3039, 3008, 2976, 2944, 2912,
  2879, 2846, 2813, 2780, 2747, 2713, 2679, 2645, 2611, 2576, 2542, 2507,
  2472, 2437, 2402, 2367, 2331, 2296, 2260, 2225, 2189, 2154, 2118, 2082,
  2047, 2012, 1976, 1940, 1905, 1869, 1834, 1798, 1763, 1727, 1692, 1657,
  1622, 1587, 1552, 1518, 1483, 1449, 1415, 1381, 1347, 1314, 1281, 1248,
  1215, 1182, 1150, 1118, 1086, 1055, 1024,  993,  963,  933,  903,  873,
   844,  816,  787,  759,  732,  705,  678,  651,  626,  600,  575,  550,
   526,  503,  479,  457,  434,  413,  391,  371,  350,  331,  312,  293,
   275,  257,  240,  224,  208,  192,  177,  163,  150,  136,  124,  112,
   101,   90,   80,   70,   61,   53,   45,   38,   32,   26,   20,   16,
    12,    8,    5,    3,    2,    1,    0,    1,    2,    3,    5,    8,
    12,   16,   20,   26,   32,   38,   45,   53,   61,   70,   80,   90,
   101,  112,  124,  136,  150,  163,  177,  192,  208,  224,  240,  257,
   275,  293,  312,  331,  350,  371,  391,  413,  434,  457,  479,  503,
   526,  550,  575,  600,  626,  651,  678,  705,  732,  759,  787,  816,
   844,  873,  903,  933,  963,  993, 1024, 1055, 1086, 1118, 1150, 1182,
  1215, 1248, 1281, 1314, 1347, 1381, 1415, 1449, 1483, 1518, 1552, 1587,
  1622, 1657, 1692, 1727, 1763, 1798, 1834, 1869, 1905, 1940, 1976, 2012
};

/*
 * DAC streaming callback.
 */
size_t nx = 0, ny = 0, nz = 0;

static void end_cb1(DACDriver *dacp, const dacsample_t *buffer, size_t n) {

  (void)dacp;
}

/*
 * DAC error callback.
 */

static void error_cb1(DACDriver *dacp, dacerror_t err) {

  (void)dacp;
  (void)err;

  chSysHalt("DAC failure");
}


// static void gpt_cb6(GPTDriver *gptp);
// static void gpt_cb7(GPTDriver *gptp);
static void gpt_cb8(GPTDriver *gptp);
float frequency[2] = { 110.00, 130.81 };

/*
 * GPT6 configuration.
 */
GPTConfig gpt6cfg1 = {
  .frequency    = 110.00*DAC_BUFFER_SIZE,
  // .frequency    = 1000000,
  // .callback     = gpt_cb6,
  .callback     = NULL,
  .cr2          = TIM_CR2_MMS_1,    /* MMS = 010 = TRGO on Update Event.    */
  .dier         = 0U
};

GPTConfig gpt7cfg1 = {
  .frequency    = 130.81*DAC_BUFFER_SIZE,
  // .callback     = gpt_cb7,
  .callback     = NULL,
  .cr2          = TIM_CR2_MMS_1,    /* MMS = 010 = TRGO on Update Event.    */
  .dier         = 0U
};

GPTConfig gpt8cfg1 = {
  .frequency    = 44100,
  .callback     = gpt_cb8,
  .cr2          = TIM_CR2_MMS_1,    /* MMS = 010 = TRGO on Update Event.    */
  .dier         = 0U
};

// static void gpt_cb6(GPTDriver *gptp) {
//     palTogglePad(GPIOA, 4);

//   nz++;

//   if ((nz % 1000) == 0) {
//     palTogglePad(GPIOB, 7);
//     nz = 0;
//     gpt6cfg1.frequency *= 1.05946;
//     gptStopTimer(&GPTD6);

//     gptStart(&GPTD6, &gpt6cfg1);
//     gptStartContinuous(&GPTD6, 2U);
    
//     gpt7cfg1.frequency *= 1.05946;
//     gptStopTimer(&GPTD7);

//     gptStart(&GPTD7, &gpt7cfg1);
//     gptStartContinuous(&GPTD7, 2U);
//   }
// }

// static void gpt_cb7(GPTDriver *gptp) {
//     palTogglePad(GPIOA, 5);

//   // ny++;

//   // if ((ny % 1000) == 0) {
//   //   palTogglePad(GPIOB, 7);
//   //   ny = 0;
//   // }
// }

static void gpt_cb8(GPTDriver *gptp) {

  nz++;

  if ((nz % 5000) == 0) {
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



static const DACConfig dac1cfg1 = {
  .init         = 2047U,
  .datamode     = DAC_DHRM_12BIT_RIGHT,
  // .datamode     = DAC_DHRM_12BIT_RIGHT_DUAL,
  // .cr           = 0
};

static const DACConversionGroup dacgrpcfg1 = {
  .num_channels = 2U,
  .end_cb       = end_cb1,
  .error_cb     = error_cb1,
  .trigger      = DAC_TRG(0)
};



void matrix_init_kb(void) {
  /*
   * Starting DAC1 driver, setting up the output pin as analog as suggested
   * by the Reference Manual.
   */
  palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_ANALOG);
  // palSetPadMode(GPIOA, 4, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);
  dacStart(&DACD1, &dac1cfg1);
  dacStart(&DACD2, &dac1cfg1);

  /*
   * Starting GPT6 driver, it is used for triggering the DAC.
   */
  gptStart(&GPTD6, &gpt6cfg1);
  gptStart(&GPTD7, &gpt7cfg1);
  gptStart(&GPTD8, &gpt8cfg1);

  /*
   * Starting a continuous conversion.
   */
  dacStartConversion(&DACD1, &dacgrpcfg1,
                     (dacsample_t *)dac_buffer, DAC_BUFFER_SIZE);
  dacStartConversion(&DACD2, &dacgrpcfg1,
                     (dacsample_t *)dac_buffer, DAC_BUFFER_SIZE);
  gptStartContinuous(&GPTD6, 2U);  
  gptStartContinuous(&GPTD7, 2U);
  gptStartContinuous(&GPTD8, 2U);
}

void matrix_scan_kb(void) {

}

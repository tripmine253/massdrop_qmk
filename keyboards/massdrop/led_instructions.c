#include <math.h>
#include <stdint.h>
#include "led_matrix.h"
#include "led_instructions.h"
#include "led_patterns.c"
#include "quantum.h"

uint8_t highest_active_layer = 0;
uint32_t temp_layer_state = 0;

__attribute__((weak))
led_instruction_t led_instructions[] = { { .end = 1 } };

float breathe_mult;
float position_offset;

uint8_t led_animation_id;
uint8_t led_lighting_mode;
float led_animation_speed;
uint8_t led_animation_direction;
uint8_t led_animation_breathe_cur;
uint8_t breathe_step;
uint8_t breathe_dir;

led_setup_t *led_animation;

void rgb_matrix_init_user(void) {
  led_animation_id = 0;
  led_lighting_mode = LED_MODE_NORMAL;
  led_animation_speed = 4.0f;
  led_animation_direction = 0;
  led_animation_breathe_cur = BREATHE_MIN_STEP;
  breathe_step = 1;
  breathe_dir = 1;
}

void rgb_matrix_pattern(led_setup_t *f, float* ro, float* go, float* bo, issi3733_led_t *led) {
  float led_px = led->px;
  float led_py = led->py;
  float px;
  float py;

  while (f->end != 1)
  {
    // Reset px, py for the new frame
    px = led_px;
    py = led_py;

    bool is_x_axis = f->ef & (EF_AXIS_X | EF_AXIS_X_INVERT);
    bool is_x_axis_invert = f->ef & EF_AXIS_X_INVERT;
    bool is_y_axis = f->ef & (EF_AXIS_Y | EF_AXIS_Y_INVERT);
    bool is_y_axis_invert = f->ef & EF_AXIS_Y_INVERT;

    if (f->ef & (EF_SCROLL | EF_SCROLL_INVERT)) {
      float offset = position_offset;

      if (
        (led_animation_direction && (f->ef & EF_SCROLL_INVERT)) || (!led_animation_direction && (f->ef & EF_SCROLL))
      ) {
          offset = -position_offset;
      }

      if (is_x_axis_invert) {
        px += offset;
      } else if (is_x_axis) {
        px -= offset;
      }

      if (is_y_axis_invert) {
        py += offset;
      } else if (is_y_axis) {
        py -= offset;
      }
    }

    if (f->ef & EF_AXIS_X_INVERT) {
      px = 100.0f - px;
    }

    if (f->ef & EF_AXIS_Y_INVERT) {
      py = 100.0f - py;
    }

    float effect_pct = 0;

    // Calculate the px within the start-stop percentage for color blending
    if (is_x_axis && is_y_axis) {
      float pd = fmod(px + py + 200.0f, 200.0f) / 2.0f;
      if (pd < f->hs || pd > f->he) { f++; continue; }

      // Scrolled on both axes
      effect_pct = (pd - f->hs) / (f->he - f->hs);
    } else if (is_x_axis) {
      px = fmod(px + 100.0f, 100.0f);
      if (px < f->hs || px > f->he) { f++; continue; }

      // Scrolled horizontally
      effect_pct = (px - f->hs) / (f->he - f->hs);
    } else if (is_y_axis) {
      py = fmod(px + 100.0f, 100.0f);
      if (py < f->hs || py > f->he) { f++; continue; }

      // Scrolled vertically
      effect_pct = (py - f->hs) / (f->he - f->hs);
    }

    //Add in any color effects
    if (f->ef & EF_OVER)
    {
      *ro = (effect_pct * (f->re - f->rs)) + f->rs;// + 0.5;
      *go = (effect_pct * (f->ge - f->gs)) + f->gs;// + 0.5;
      *bo = (effect_pct * (f->be - f->bs)) + f->bs;// + 0.5;
    }
    else if (f->ef & EF_SUBTRACT)
    {
      *ro -= (effect_pct * (f->re - f->rs)) + f->rs;// + 0.5;
      *go -= (effect_pct * (f->ge - f->gs)) + f->gs;// + 0.5;
      *bo -= (effect_pct * (f->be - f->bs)) + f->bs;// + 0.5;
    }
    else
    {
      *ro += (effect_pct * (f->re - f->rs)) + f->rs;// + 0.5;
      *go += (effect_pct * (f->ge - f->gs)) + f->gs;// + 0.5;
      *bo += (effect_pct * (f->be - f->bs)) + f->bs;// + 0.5;
    }

    f++;
  }
}

void rgb_matrix_run_user(led_disp_t disp) {
  led_animation = (led_setup_t*) led_setups[led_animation_id];

  breathe_mult = 1;

  if (led_animation_breathing)
  {
    //+60us 119 LED
    led_animation_breathe_cur += breathe_step * breathe_dir;

    if (led_animation_breathe_cur >= BREATHE_MAX_STEP)
      breathe_dir = -1;
    else if (led_animation_breathe_cur <= BREATHE_MIN_STEP)
      breathe_dir = 1;

    //Brightness curve created for 256 steps, 0 - ~98%
    breathe_mult = 0.000015 * led_animation_breathe_cur * led_animation_breathe_cur;
    if (breathe_mult > 1) breathe_mult = 1;
    else if (breathe_mult < 0) breathe_mult = 0;
  }

  //Only needs to be calculated once per frame
  position_offset = (float)(disp.frame % (uint32_t)(1000.0f / led_animation_speed)) / 10.0f * led_animation_speed;
  position_offset *= 100.0f;
  position_offset = (uint32_t) position_offset % 10000;
  position_offset /= 100.0f;

  highest_active_layer = 0;
  temp_layer_state = layer_state;

  while (temp_layer_state >> 1 != 0) {
      highest_active_layer++;
      temp_layer_state = temp_layer_state >> 1;
  }
}

void rgb_run(issi3733_led_t *led)
{
  float ro = 0;
  float go = 0;
  float bo = 0;

  if (led_lighting_mode == LED_MODE_KEYS_ONLY && led->scan == 255)
  {
    //Do not act on this LED
  }
  else if (led_lighting_mode == LED_MODE_NON_KEYS_ONLY && led->scan != 255)
  {
    //Do not act on this LED
  }
  else if (led_lighting_mode == LED_MODE_INDICATORS_ONLY)
  {
    //Do not act on this LED (Only show indicators)
  }
  else
  {
    led_instruction_t *led_cur_instruction;
    led_cur_instruction = led_instructions;

    //Act on LED
    if (led_cur_instruction->end) {
      // If no instructions, use normal pattern
      rgb_matrix_pattern(led_animation, &ro, &go, &bo, led);
    } else {
      uint8_t skip;
      uint8_t modid = (led->id - 1) / 32;                         //PS: Calculate which id# contains the led bit
      uint32_t modidbit = 1 << ((led->id - 1) % 32);              //PS: Calculate the bit within the id#
      uint32_t *bitfield;                                             //PS: Will point to the id# within the current instruction

      while (!led_cur_instruction->end) {
        skip = 0;

        //PS: Check layer active first
        if (led_cur_instruction->flags & LED_FLAG_MATCH_LAYER) {
        if (led_cur_instruction->layer != highest_active_layer) {
            skip = 1;
        }
        }

        if (!skip)
        {
          if (led_cur_instruction->flags & LED_FLAG_MATCH_ID) {
            bitfield = &led_cur_instruction->id0 + modid;       //PS: Add modid as offset to id0 address. *bitfield is now idX of the led id
            if (~(*bitfield) & modidbit) {                      //PS: Check if led bit is not set in idX
              skip = 1;
            }
          }
        }

        if (!skip) {
          if (led_cur_instruction->flags & LED_FLAG_USE_RGB) {
            ro = led_cur_instruction->r;
            go = led_cur_instruction->g;
            bo = led_cur_instruction->b;
          } else if (led_cur_instruction->flags & LED_FLAG_USE_PATTERN) {
            rgb_matrix_pattern(led_setups[led_cur_instruction->pattern_id], &ro, &go, &bo, led);
          } else if (led_cur_instruction->flags & LED_FLAG_USE_ROTATE_PATTERN) {
            rgb_matrix_pattern(led_animation, &ro, &go, &bo, led);
          }
        }

        led_cur_instruction++;
      }
    }
  }

  //Clamp values 0-255
  if (ro > 255) ro = 255; else if (ro < 0) ro = 0;
  if (go > 255) go = 255; else if (go < 0) go = 0;
  if (bo > 255) bo = 255; else if (bo < 0) bo = 0;

  if (led_animation_breathing)
  {
    ro *= breathe_mult;
    go *= breathe_mult;
    bo *= breathe_mult;
  }

  *led->rgb.r = (uint8_t)ro;
  *led->rgb.g = (uint8_t)go;
  *led->rgb.b = (uint8_t)bo;

// #ifdef USB_LED_INDICATOR_ENABLE
//   if (keyboard_leds())
//   {
//       uint8_t kbled = keyboard_leds();
//       if (
//           #if USB_LED_NUM_LOCK_SCANCODE != 255
//           (led->scan == USB_LED_NUM_LOCK_SCANCODE && kbled & (1<<USB_LED_NUM_LOCK)) ||
//           #endif //NUM LOCK
//           #if USB_LED_CAPS_LOCK_SCANCODE != 255
//           (led->scan == USB_LED_CAPS_LOCK_SCANCODE && kbled & (1<<USB_LED_CAPS_LOCK)) ||
//           #endif //CAPS LOCK
//           #if USB_LED_SCROLL_LOCK_SCANCODE != 255
//           (led->scan == USB_LED_SCROLL_LOCK_SCANCODE && kbled & (1<<USB_LED_SCROLL_LOCK)) ||
//           #endif //SCROLL LOCK
//           #if USB_LED_COMPOSE_SCANCODE != 255
//           (led->scan == USB_LED_COMPOSE_SCANCODE && kbled & (1<<USB_LED_COMPOSE)) ||
//           #endif //COMPOSE
//           #if USB_LED_KANA_SCANCODE != 255
//           (led->scan == USB_LED_KANA_SCANCODE && kbled & (1<<USB_LED_KANA)) ||
//           #endif //KANA
//           (0))
//       {
//           if (*led->rgb.r > 127) *led->rgb.r = 0;
//           else *led->rgb.r = 255;
//           if (*led->rgb.g > 127) *led->rgb.g = 0;
//           else *led->rgb.g = 255;
//           if (*led->rgb.b > 127) *led->rgb.b = 0;
//           else *led->rgb.b = 255;
//       }
//   }
// #endif //USB_LED_INDICATOR_ENABLE
}

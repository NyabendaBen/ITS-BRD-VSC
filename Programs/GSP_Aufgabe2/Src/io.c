#include "io.h"
#include "timebase.h"
#include <stm32f4xx.h>

#define PIN_ENC_A 0U
#define PIN_ENC_B 1U
#define PIN_BTN_S6 6U

#define LED_BAR_MASK 0x00FFU
#define BSRR_RESET_SHIFT 16U

#define PIN_SCOPE_DISP 0U
#define PIN_SCOPE_LOOP 1U
#define PIN_LED_ERROR 5U
#define PIN_LED_BACKWARD 6U
#define PIN_LED_FORWARD 7U

static const Phase RAW_TO_PHASE[4] = {
    PHASE_A, /* Index 0: A=0, B=0 */
    PHASE_B, /* Index 1: A=1, B=0 */
    PHASE_D, /* Index 2: A=0, B=1 */
    PHASE_C, /* Index 3: A=1, B=1 */
};

InputSample io_read_inputs(void) {
   InputSample sample;

   sample.phase.timestamp = timebase_now();
   uint32_t idr = GPIOF->IDR;
   uint32_t raw_bits = idr & 0x03U;
   sample.phase.phase = RAW_TO_PHASE[raw_bits];
   sample.reset_pressed = ((idr & (1U << PIN_BTN_S6)) == 0U);

   return sample;
}

void io_write_counter_leds(uint8_t value) {
   uint32_t set_mask = (uint32_t)value & LED_BAR_MASK;
   uint32_t reset_mask = ((~(uint32_t)value) & LED_BAR_MASK)
                         << BSRR_RESET_SHIFT;

   GPIOD->BSRR = set_mask | reset_mask;
}

void io_write_direction_leds(bool forward_on, bool backward_on) {
   uint32_t bsrr_val = 0;

   if (forward_on) {
      bsrr_val |= (1U << PIN_LED_FORWARD);
   } else {
      bsrr_val |= (1U << (PIN_LED_FORWARD + BSRR_RESET_SHIFT));
   }
   if (backward_on) {
      bsrr_val |= (1U << PIN_LED_BACKWARD);
   } else {
      bsrr_val |= (1U << (PIN_LED_BACKWARD + BSRR_RESET_SHIFT));
   }
   GPIOE->BSRR = bsrr_val;
}

void io_write_error_led(bool err_on) {
   if (err_on) {
      GPIOE->BSRR = (1U << PIN_LED_ERROR);
   } else {
      GPIOE->BSRR = (1U << (PIN_LED_ERROR + BSRR_RESET_SHIFT));
   }
}

void io_set_scope_pin(bool high) {
   if (high) {
      GPIOE->BSRR = (1U << PIN_SCOPE_DISP);
   } else {
      GPIOE->BSRR = (1U << (PIN_SCOPE_DISP + BSRR_RESET_SHIFT));
   }
}

void io_toggle_loop_pin(void) { GPIOE->ODR ^= (1U << PIN_SCOPE_LOOP); }

void io_init(void) {
   io_write_counter_leds(0U);
   io_write_direction_leds(false, false);
   io_write_error_led(false);
   io_set_scope_pin(false);

   GPIOE->BSRR = (1U << (PIN_SCOPE_LOOP + BSRR_RESET_SHIFT));
}

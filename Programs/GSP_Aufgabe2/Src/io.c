#include "io.h"
#include "timebase.h"
#include <stm32f4xx.h>

// Pin-Definitionen
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

// Tabelle zur Umwandlung von 2-Bit Rohwerten in Phasen
static const Phase RAW_TO_PHASE[4] = {
    PHASE_A, /* 00: A=0, B=0 */
    PHASE_B, /* 01: A=1, B=0 */
    PHASE_D, /* 10: A=0, B=1 */
    PHASE_C, /* 11: A=1, B=1 */
};

// Eingänge Zeitstempel einlesen (DDC-Prinzip)
InputSample io_read_inputs(void) {
   InputSample sample;

   sample.phase.timestamp = timebase_now();
   uint32_t idr = GPIOF->IDR;

   // Bits für Kanal A und B maskieren
   uint32_t encoder_pins = idr & 0x03U;
   sample.phase.phase = RAW_TO_PHASE[encoder_pins];

   // Taster S6 ist low aktiv
   sample.reset_pressed = ((idr & (1U << PIN_BTN_S6)) == 0U);

   return sample;
}

// Phasenwechsel auf LEDs D8..D15 ausgeben
void io_write_counter_leds(uint8_t value) {
   uint32_t set_mask = (uint32_t)value & LED_BAR_MASK;
   uint32_t reset_mask = ((~(uint32_t)value) & LED_BAR_MASK)
                         << BSRR_RESET_SHIFT;

   GPIOD->BSRR = set_mask | reset_mask;
}

// Richtungs LEDs ansteuern (D22 Rückwärts, D23 Vorwärts)
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

// Rote Fehler LED D21 ansteuern
void io_write_error_led(bool err_on) {
   if (err_on) {
      GPIOE->BSRR = (1U << PIN_LED_ERROR);
   } else {
      GPIOE->BSRR = (1U << (PIN_LED_ERROR + BSRR_RESET_SHIFT));
   }
}

// Messpin OUT8 für Oszilloskop setzen
void io_set_scope_pin(bool high) {
   if (high) {
      GPIOE->BSRR = (1U << PIN_SCOPE_DISP);
   } else {
      GPIOE->BSRR = (1U << (PIN_SCOPE_DISP + BSRR_RESET_SHIFT));
   }
}

// Messpin OUT9 am Schleifenende toggeln
void io_toggle_loop_pin(void) { GPIOE->ODR ^= (1U << PIN_SCOPE_LOOP); }

// GPIOs und Anfangszustände der LEDs initialisieren
void io_init(void) {
   io_write_counter_leds(0U);
   io_write_direction_leds(false, false);
   io_write_error_led(false);
   io_set_scope_pin(false);

   GPIOE->BSRR = (1U << (PIN_SCOPE_LOOP + BSRR_RESET_SHIFT));
}

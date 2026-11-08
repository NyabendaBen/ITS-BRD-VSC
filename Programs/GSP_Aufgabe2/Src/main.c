/**
 ******************************************************************************
 * @file    main.c
 * @author  Franz Korf / Praktikumsteam
 * @brief   Aufgabe 2: Drehgeber (Polling nach DDC-Prinzip)
 ******************************************************************************
 */

#include "calc.h"
#include "display2.h"
#include "encoder.h"
#include "init.h"
#include "io.h"
#include "timebase.h"
#include <stdbool.h>
#include <stdint.h>

#define COUNTER_LSB_MASK 0xFFU

int main(void) {
   /* 1. Hardware & Module initialisieren */
   initITSboard();
   timebase_init();
   io_init();
   display_init();

   /* 2. Kaltstart-Synchronisation: Auf Ist-Phase des Gebers eichen */
   InputSample init_sample = io_read_inputs();
   encoder_reset(init_sample.phase);

   EncoderState init_state = encoder_get_state();
   calc_reset(init_state.counter, init_sample.phase.timestamp);

   /* Initiale Anzeige: 0.0 Grad und 0.0 Grad/s */
   static const Measurement ZERO_MEAS = {0.0, 0.0};
   display_request(&ZERO_MEAS);

   /* Zustand des S6-Tasters fuer Flankenerkennung merken */
   bool s6_prev = init_sample.reset_pressed;

   /* 3. DDC Super-Loop (EVA-Prinzip) */
   while (1) {

      InputSample input_sample = io_read_inputs();


      bool s6_pressed = input_sample.reset_pressed;
      bool s6_edge = s6_pressed && !s6_prev;
      s6_prev = s6_pressed;

      EncoderState enc_state;

      if (s6_edge) {
         /* S6-Reset: Encoder zuruecksetzen und Rechner neu synchronisieren */
         encoder_reset(input_sample.phase);
         enc_state = encoder_get_state();
         calc_reset(enc_state.counter, input_sample.phase.timestamp);
         display_request(&ZERO_MEAS);
      } else {
         /* Normaler Schritt der Zustandsmaschine */
         encoder_update(input_sample.phase);
         enc_state = encoder_get_state();

         /* Nur rechnen & Display aktualisieren, wenn kein Fehler anliegt */
         if (!enc_state.error) {
            Measurement meas;
            bool updated = calc_update(&enc_state.counter,
                                       input_sample.phase.timestamp, &meas);
            if (updated) {
               display_request(&meas);
            }
         }
      }
      if (enc_state.error) {
         /* Zentrales Fehler-Gate: D21 einschalten, alle anderen Ausgaben
          * einfrieren */
         io_write_error_led(true);
      } else {
         /* Normalbetrieb: Fehler-LED aus, Zaehler auf D8..D15, Richtungs-LEDs
          */
         io_write_error_led(false);
         io_write_counter_leds(
             (uint8_t)(enc_state.counter.value & COUNTER_LSB_MASK));

         bool forward_on = (enc_state.direction == DIR_FORWARD);
         bool backward_on = (enc_state.direction == DIR_BACKWARD);
         io_write_direction_leds(forward_on, backward_on);
      }

      /* Zeitoptimierte LCD-Ausgabe: Maximal 1 Zeichen pro Loop-Durchlauf */
      if (display_is_busy()) {
         io_set_scope_pin(true);
         display_process();
         io_set_scope_pin(false);
      }

      /* Messpin fuer Oszilloskop: Schleifendurchlauf toggeln */
      io_toggle_loop_pin();
   }
}

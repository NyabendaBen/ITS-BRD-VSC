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
   // Hardware und Module initialisieren
   initITSboard();
   timebase_init();
   io_init();
   display_init();

   // Geber auf die aktuelle Startphase setzen
   InputSample init_sample = io_read_inputs();
   encoder_reset(init_sample.phase);

   EncoderState init_state = encoder_get_state();
   calc_reset(init_state.counter, init_sample.phase.timestamp);

   // Initiale Anzeige: 0.0 Grad und 0.0 Grad/s
   static const Measurement ZERO_MEAS = {0.0, 0.0};
   display_request(&ZERO_MEAS);

   // Zustand von S6 für Flankenerkennung merken
   bool s6_prev = init_sample.reset_pressed;

   // Super Loop nach dem DDC-Prinzip
   while (1) {
      // Eingabe: Schnappschuss aller Eingänge
      InputSample input_sample = io_read_inputs();

      // Verarbeitung: Taster und Drehgeber auswerten
      bool s6_pressed = input_sample.reset_pressed;
      bool s6_was_pressed = s6_pressed && !s6_prev;
      s6_prev = s6_pressed;

      EncoderState enc_state;

      // Bei Tastendruck: Zähler und Berechnung zurücksetzen
      if (s6_was_pressed) {
         encoder_reset(input_sample.phase);
         enc_state = encoder_get_state();
         calc_reset(enc_state.counter, input_sample.phase.timestamp);
         display_request(&ZERO_MEAS);
      } else {
         // FSM aktualisieren
         encoder_update(input_sample.phase);
         enc_state = encoder_get_state();

         // Bei gültigen Phasen: Winkel und Geschwindigkeit berechnen
         if (!enc_state.error) {
            Measurement meas;
            bool updated = calc_update(&enc_state.counter,
                                       input_sample.phase.timestamp, &meas);
            if (updated) {
               display_request(&meas);
            }
         }
      }

      // Ausgabe: LEDs setzen
      if (enc_state.error) {
         // Im Fehlerfall: Rote LED D21 an, restliche LEDs einfrieren
         io_write_error_led(true);
      } else {
         // Normal: Zähler auf D8..D15 und Richtung anzeigen
         io_write_error_led(false);
         io_write_counter_leds(
             (uint8_t)(enc_state.counter.value & COUNTER_LSB_MASK));

         bool forward_on = (enc_state.direction == DIR_FORWARD);
         bool backward_on = (enc_state.direction == DIR_BACKWARD);
         io_write_direction_leds(forward_on, backward_on);
      }

      // Maximal 1 Zeichen pro Durchlauf aufs LCD schreiben
      if (display_is_busy()) {
         io_set_scope_pin(true);
         display_process();
         io_set_scope_pin(false);
      }

      // Pin für die Oszilloskop Schleifenmessung toggeln
      io_toggle_loop_pin();
   }
}

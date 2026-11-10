#ifndef INC_IO_H_
#define INC_IO_H_

#include "phase.h"
#include <stdbool.h>
#include <stdint.h>

// Schnappschuss der Eingaenge (DDC-Prinzip)
typedef struct {
   PhaseSample phase;
   bool reset_pressed;
} InputSample;

/**
 * @brief Liest alle Eingänge atomar ein (PF0, PF1 und S6).
 * @return Eingangs Schnappschuss mit Phase, Zeitstempel und Tasterzustand.
 */
InputSample io_read_inputs(void);

/**
 * @brief Schreibt die unteren 8 Bit des Phasenzählers auf D8...D15.
 * @param[in] value 8-Bit Binärwert (Zweierkomplement bei negativen Werten).
 */
void io_write_counter_leds(uint8_t value);

/**
 * @brief Steuert die Richtungs-LEDs (D23 = Vorwaerts, D22 = Rueckwaerts).
 * @param[in] forward_on  True wenn D23 an sein soll.
 * @param[in] backward_on True wenn D22 an sein soll.
 */
void io_write_direction_leds(bool forward_on, bool backward_on);

/**
 * @brief Schaltet die Fehler LED D21.
 * @param[in] err_on True schaltet D21 ein, False aus.
 */
void io_write_error_led(bool err_on);

/**
 * @brief Setzt den Mess-Pin OUT8 (PE0) für die Display-Zeitmessung.
 * @param[in] high True = High (Start), False = Low (Ende).
 */
void io_set_scope_pin(bool high);

/**
 * @brief Toggelt den Mess-Pin OUT9 (PE1) für die Schleifenzeitmessung.
 */
void io_toggle_loop_pin(void);

/**
 * @brief Initialisiert die GPIO-Pins und setzt Standardzustände.
 */
void io_init(void);

#endif /* INC_IO_H_ */

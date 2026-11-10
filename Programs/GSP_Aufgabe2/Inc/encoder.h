#ifndef ENCODER_H
#define ENCODER_H

#include "phase.h"
#include <stdbool.h>
#include <stdint.h>

// Erkannte Drehrichtung
typedef enum {
   DIR_UNKNOWN = 0,
   DIR_FORWARD,
   DIR_BACKWARD,
} Direction;

// Phasenübergangs Veränderung laut Aufgabenblatt Tabelle 1
typedef enum {
   EVT_NONE = 0,
   EVT_FORWARD,
   EVT_BACKWARD,
   EVT_ERROR,
} PhaseEvent;

// Zähler für Phasenwechsel mit Zeitstempel des letzten Wechsels
typedef struct {
   int64_t value;
   uint64_t timestamp;
} PhaseCounter;

// Gesamtzustand des Drehgebers
typedef struct {
   PhaseCounter counter;
   Direction direction;
   bool error;
} EncoderState;

/**
 * @brief Setzt den Encoder auf den Anfangszustand zurück.
 * @param[in] start Startphase mit Zeitstempel.
 */
void encoder_reset(PhaseSample start);

/**
 * @brief Liefert den aktuellen Zustand des Encoders.
 * @return Aktueller Zustand (Zähler, Richtung, Fehler).
 */
EncoderState encoder_get_state(void);

/**
 * @brief Aktualisiert die Zustandsmaschine mit der neu eingelesenen Phase.
 * @param[in] sample Neu eingelesene Phase mit Zeitstempel.
 * @return Erkanntes Ereignis (Vorwärts, Rückwärts, Stillstand, Fehler).
 */
PhaseEvent encoder_update(PhaseSample sample);

#endif /* ENCODER_H */
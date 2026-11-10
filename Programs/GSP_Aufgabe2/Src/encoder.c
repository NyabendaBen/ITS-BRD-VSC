#include "encoder.h"
#include "phase.h"

// Interner Zähler und Status des Drehgebers
static PhaseCounter counter = {0, 0};
static Direction direction = DIR_UNKNOWN;
static bool error = false;
static Phase last_phase = PHASE_A;

// Transitionstabelle der FSM laut Aufgabenblatt Tabelle 1
static const PhaseEvent TRANSITION_TABLE[PHASE_COUNT][PHASE_COUNT] = {
    [PHASE_A] =
        {
            [PHASE_A] = EVT_NONE,
            [PHASE_B] = EVT_FORWARD,
            [PHASE_C] = EVT_ERROR,
            [PHASE_D] = EVT_BACKWARD,
        },

    [PHASE_B] =
        {
            [PHASE_A] = EVT_BACKWARD,
            [PHASE_B] = EVT_NONE,
            [PHASE_C] = EVT_FORWARD,
            [PHASE_D] = EVT_ERROR,
        },

    [PHASE_C] =
        {
            [PHASE_A] = EVT_ERROR,
            [PHASE_B] = EVT_BACKWARD,
            [PHASE_C] = EVT_NONE,
            [PHASE_D] = EVT_FORWARD,
        },

    [PHASE_D] =
        {
            [PHASE_A] = EVT_FORWARD,
            [PHASE_B] = EVT_ERROR,
            [PHASE_C] = EVT_BACKWARD,
            [PHASE_D] = EVT_NONE,
        },
};

// Zähler und Zustand zurücksetzen
void encoder_reset(PhaseSample start) {
   counter.value = 0;
   counter.timestamp = start.timestamp;
   last_phase = start.phase;
   direction = DIR_UNKNOWN;
   error = false;
}

// Aktuellen Zustand zurückgeben
EncoderState encoder_get_state(void) {
   EncoderState state;

   state.counter = counter;
   state.direction = direction;
   state.error = error;

   return state;
}

// Neuen Phasenwert auswerten und Zähler nachführen
PhaseEvent encoder_update(PhaseSample sample) {
   if (error) {
      return EVT_ERROR;
   }

   PhaseEvent event = TRANSITION_TABLE[last_phase][sample.phase];

   switch (event) {
   case EVT_FORWARD:
      counter.value++;
      counter.timestamp = sample.timestamp;
      direction = DIR_FORWARD;
      last_phase = sample.phase;
      break;

   case EVT_BACKWARD:
      counter.value--;
      counter.timestamp = sample.timestamp;
      direction = DIR_BACKWARD;
      last_phase = sample.phase;
      break;

   case EVT_NONE:
      // Keine Änderung
      break;

   case EVT_ERROR:
      // Unzulässiger Schritt: Fehler
      error = true;
      break;
   }
   return event;
}
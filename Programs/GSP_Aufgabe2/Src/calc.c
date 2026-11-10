#include "calc.h"
#include "timebase.h"
#include <stddef.h>
#include <stdint.h>

// 360 Grad / 1200 Phasen = 0.3 Grad pro Phase
#define DEG_PER_PHASE 0.3

// Zeitfenster aus der Aufgabenstellung
#define DISPLAY_UPDATE_MS 300U
#define TIMEOUT_STANDSTILL_MS 3000U

#define WINDOW_FAST_TICKS ((uint64_t)DISPLAY_UPDATE_MS * TICKS_PER_MS)
#define WINDOW_SLOW_TICKS ((uint64_t)TIMEOUT_STANDSTILL_MS * TICKS_PER_MS)

static PhaseCounter prev_counter = {0, 0};
static uint64_t last_calc_time = 0;

// Berechnungsreferenz synchronisieren
void calc_reset(PhaseCounter start, uint64_t now) {
   prev_counter = start;
   last_calc_time = now;
}

// Winkel und Geschwindigkeit berechnen
bool calc_update(const PhaseCounter *counter, uint64_t now,
                 Measurement *result) {
   if (counter == NULL || result == NULL) {
      return false;
   }

   // Prüfen, ob die 300 ms Mindestzeit vergangen sind
   uint64_t elapsed_since_calc = now - last_calc_time;
   if (elapsed_since_calc < WINDOW_FAST_TICKS) {
      return false;
   }

   int64_t delta_phases = counter->value - prev_counter.value;
   bool wheel_turned = (delta_phases != 0);

   // Bei Stillstand bis zu 3 Sekunden warten für 0,1 deg/s Auflösung
   bool is_timeout = (elapsed_since_calc >= WINDOW_SLOW_TICKS);
   if (!wheel_turned && !is_timeout) {
      return false;
   }

   // Winkel berechnen
   result->angle_deg = (double)counter->value * DEG_PER_PHASE;
   result->speed_deg_per_s = 0.0;

   // Geschwindigkeit über Phasen Zeitstempel berechnen
   uint64_t delta_ticks = counter->timestamp - prev_counter.timestamp;
   if (delta_ticks > 0ULL) {
      double delta_time_sec = timebase_ticks_to_sec(delta_ticks);
      double delta_angle_deg = (double)delta_phases * DEG_PER_PHASE;
      result->speed_deg_per_s = delta_angle_deg / delta_time_sec;
   } else {
      result->speed_deg_per_s = 0.0;
   }

   prev_counter = *counter;
   last_calc_time = now;
   return true;
}

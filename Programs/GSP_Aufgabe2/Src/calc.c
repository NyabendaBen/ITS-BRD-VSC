#include "calc.h"
#include "timebase.h"
#include <stddef.h>
#include <stdint.h>

#define DEG_PER_TRANSITION 0.3
#define WINDOW_FAST_MS 300U
#define WINDOW_SLOW_MS 3000U

#define WINDOW_FAST_TICKS ((uint64_t)WINDOW_FAST_MS * TICKS_PER_MS)
#define WINDOW_SLOW_TICKS ((uint64_t)WINDOW_SLOW_MS * TICKS_PER_MS)

static PhaseCounter ref_counter = {0, 0};
static uint64_t last_calc_time = 0;

void calc_reset(PhaseCounter start, uint64_t now) {
   ref_counter = start;
   last_calc_time = now;
}

bool calc_update(const PhaseCounter *counter, uint64_t now,
                 Measurement *result) {
   if (counter == NULL || result == NULL) {
      return false;
   }
   uint64_t elapsed_since_calc = now - last_calc_time;
   if (elapsed_since_calc < WINDOW_FAST_TICKS) {
      return false;
   }

   int64_t delta_phases = counter->value - ref_counter.value;
   bool is_movement = (delta_phases != 0);

   bool is_timeout = (elapsed_since_calc >= WINDOW_SLOW_TICKS);
   if (!is_movement && !is_timeout) {
      return false;
   }

   result->angle_deg = (double)counter->value * DEG_PER_TRANSITION;
   result->speed_deg_per_s = 0.0;
   uint64_t delta_ticks = counter->timestamp - ref_counter.timestamp;
   if (delta_ticks > 0ULL) {
      double dtt = timebase_ticks_to_sec(delta_ticks);
      double dphi = (double)delta_phases * DEG_PER_TRANSITION;
      result->speed_deg_per_s = dphi / dtt;
   } else {
      result->speed_deg_per_s = 0.0;
   }
   ref_counter = *counter;
   last_calc_time = now;
   return true;
}
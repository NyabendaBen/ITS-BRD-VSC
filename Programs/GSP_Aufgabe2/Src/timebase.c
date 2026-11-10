#include "timebase.h"

// Letzter gelesener Wert des 32-Bit Timers
static uint32_t last_timer_val = 0U;

// 64-Bit Ticks zur Vermeidung des 47-Sekunden-Überlaufs
static uint64_t now_ticks = 0U;

// Timer starten und initialen Zeitstempel merken
void timebase_init(void) {
   initTimer();
   last_timer_val = getTimeStamp();
   now_ticks = 0U;
}

// Differenz berechnen und auf 64 Bit aufaddieren
uint64_t timebase_now(void) {
   uint32_t current_raw = getTimeStamp();
   uint32_t timer_diff = current_raw - last_timer_val;
   now_ticks += (uint64_t)timer_diff;
   last_timer_val = current_raw;
   return now_ticks;
}

// Ticks in Sekunden umrechnen
double timebase_ticks_to_sec(uint64_t ticks) {
   return (double)ticks / (double)TICKS_PER_S;
}

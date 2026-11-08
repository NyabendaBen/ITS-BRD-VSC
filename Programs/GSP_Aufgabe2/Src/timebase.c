#include "timebase.h"

static uint32_t last_raw = 0U;
static uint64_t now_ticks = 0U;

void timebase_init(void) {
   initTimer();
   last_raw = getTimeStamp();
   now_ticks = 0U;
}

uint64_t timebase_now(void) {
   uint32_t current_raw = getTimeStamp();
   uint32_t delta = current_raw - last_raw;
   now_ticks += (uint64_t)delta;
   last_raw = current_raw;
   return now_ticks;
}

double timebase_ticks_to_sec(uint64_t ticks) {
   return (double)ticks / (double)TICKS_PER_S;
}
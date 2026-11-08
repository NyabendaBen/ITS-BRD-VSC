#ifndef TIMEBASE_H
#define TIMEBASE_H

#include "timer.h"
#include <stdint.h>

#define US_PER_MS 1000U
#define US_PER_S 1000000U

#define TICKS_PER_MS ((uint64_t)TICKS_PER_US * US_PER_MS)
#define TICKS_PER_S ((uint64_t)TICKS_PER_US * US_PER_S)

void timebase_init(void);

uint64_t timebase_now(void);

double timebase_ticks_to_sec(uint64_t ticks);

#endif /* TIMEBASE_H */
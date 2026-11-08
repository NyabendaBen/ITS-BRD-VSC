#ifndef PHASE_H
#define PHASE_H

#include <stdint.h>

#define PHASE_COUNT 4U

typedef enum {
   PHASE_A = 0,
   PHASE_B,
   PHASE_C,
   PHASE_D,

} Phase;

typedef struct {
   Phase phase;
   uint64_t timestamp;

} PhaseSample;

#endif /* PHASE_H */
#ifndef PHASE_H
#define PHASE_H

#include <stdint.h>

// 4 Phasen pro Schlitz (2-Bit Gray-Code)
#define PHASE_COUNT 4U

// Zustände der beiden Kanaele A und B
typedef enum {
   PHASE_A = 0, // A=0, B=0
   PHASE_B,     // A=1, B=0
   PHASE_C,     // A=1, B=1
   PHASE_D,     // A=0, B=1
} Phase;

// Phase zusammen mit dem Zeitstempel
typedef struct {
   Phase phase;
   uint64_t timestamp;
} PhaseSample;

#endif /* PHASE_H */

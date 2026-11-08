#ifndef ENCODER_H
#define ENCODER_H

#include "phase.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
   DIR_UNKNOWN = 0,
   DIR_FORWARD,
   DIR_BACKWARD,

} Direction;

typedef enum {
   EVT_NONE = 0,
   EVT_FORWARD,
   EVT_BACKWARD,
   EVT_ERROR,
} PhaseEvent;

typedef struct {
   int64_t value;
   uint64_t timestamp;

} PhaseCounter;

typedef struct {
   PhaseCounter counter;
   Direction direction;
   bool error;

} EncoderState;

void encoder_reset(PhaseSample start);

EncoderState encoder_get_state(void);

PhaseEvent encoder_update(PhaseSample sample);

#endif /* ENCODER_H */
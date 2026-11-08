#ifndef INC_CALC_H_
#define INC_CALC_H_
#include "encoder.h"

typedef struct {
   double angle_deg;
   double speed_deg_per_s;

} Measurement;

void calc_reset(PhaseCounter start, uint64_t now);

bool calc_update(const PhaseCounter *counter, uint64_t now,
                 Measurement *result);

#endif /* ENCODER_H */
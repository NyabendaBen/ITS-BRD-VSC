#ifndef DISPLAY2_H_
#define DISPLAY2_H_

#include "calc.h"

void display_init(void);

void display_request(const Measurement *meas);

void display_process(void);

bool display_is_busy(void);

#endif /* INC_DISPLAY_H_ */
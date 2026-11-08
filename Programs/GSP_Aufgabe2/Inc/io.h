#ifndef INC_IO_H_
#define INC_IO_H_

#include "phase.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
   PhaseSample phase;
   bool reset_pressed;
} InputSample;

InputSample io_read_inputs(void);

void io_write_counter_leds(uint8_t value);

void io_write_direction_leds(bool forward_on, bool backward_on);

void io_write_error_led(bool err_on);

void io_set_scope_pin(bool high);

void io_toggle_loop_pin(void);

void io_init(void);

#endif /* INC_IO_H_ */
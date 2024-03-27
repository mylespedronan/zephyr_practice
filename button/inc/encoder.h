#ifndef ENCODER_H
#define ENCODER_H

// #include "main.h"
#include <stdint.h>
#include "led.h"

#define STEPSIZE          100000u

extern uint32_t encoder_val;
// extern uint32_t max_period;
// extern uint32_t min_period;

int initEncoder(void);
// int32_t updateEncoder(void);
void updateEncoder(void);

#endif // ENCODER_H
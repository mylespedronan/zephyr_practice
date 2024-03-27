#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "encoder.h"

int initLED(void);
int initPWM(void);
void updatePwmLED(uint32_t enc_val);

extern uint32_t max_period;
extern uint32_t min_period;

#endif // LED_H
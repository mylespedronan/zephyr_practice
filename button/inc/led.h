#ifndef LED_H
#define LED_H

#include <stdio.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

#include "encoder.h"

extern uint32_t max_period;
extern uint32_t min_period;

int initLED(void);
int initPWM(void);
void updatePwmLED(uint32_t enc_val);

#endif // LED_H
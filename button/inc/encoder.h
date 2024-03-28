#ifndef ENCODER_H
#define ENCODER_H

#include <stdio.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#include "led.h"

#define STEPSIZE          100000u

extern uint32_t encoder_val;

int initEncoder(void);
void updateEncoder(void);

#endif // ENCODER_H
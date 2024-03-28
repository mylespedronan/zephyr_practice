#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#include "led.h"
#include "encoder.h"

int initButton(void);

extern uint32_t encoder_val;

#endif // BUTTON_H
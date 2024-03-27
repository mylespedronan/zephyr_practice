#ifndef BUTTON_H
#define BUTTON_H

// #include "main.h"
#include "led.h"
#include "encoder.h"

#include <stdint.h>

// Like GPIO_DT_SPEC_GET_BY_IDX(), with a fallback to a default value.
// struct gpio_dt_spec button;

int initButton(void);

extern uint32_t encoder_val;

#endif // BUTTON_H
// Using external button as input, turn on LED0 when button pressed
// After, use external button as input, turn on external LED when button pressed
#include "main.h"

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

// 1000 msec = 1 sec
#define SLEEP_TIME_MS     1u

int initialize(void)
{
  int ret = 0u;

  ret = initLED();
  if (ret != 0) {
    return ret;
  }

  ret = initButton();
  if (ret != 0) {
    return ret;
  }

  ret = initEncoder();
  if (ret != 0) {
    return ret;
  }

  ret = initPWM();
  if (ret != 0) {
    return ret;
  }

  return ret;
}

int main(void)
{
  int ret = 0u;

  ret = initialize();
  if (ret != 0) {
    printk("Error initializing");
    return ret;
  }

  while (1) {
    // Update encoder every 1ms
    updateEncoder();
    
    // Delay
    k_msleep(SLEEP_TIME_MS);
  }
  
  return 0;
}

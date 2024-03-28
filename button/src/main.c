/**
 * This program uses external buttons as inputs for setting and resetting the LED controlled by PWM.
 * An encoder is used to alter the intensity of the LED which is set after a button is pressed. If the
 * user wants to reset the intensity to the minimum value, a reset button can be pressed.
*/
#include "main.h"

// 1000 msec = 1 sec
#define SLEEP_TIME_MS     5u

/**
 * @brief Initialize modules
*/
int initialize(void)
{
  int ret = 0u;

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
  
  return ret;
}

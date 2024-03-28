/**
 * This module contains code pertaining to the LED controlled by PWM
 * LED - PC6 (PWM/TIM3_CH1)
*/

#include "led.h"

// The devicetree node identifiers for aliases
#define EXT_PWM_LED1      DT_ALIAS(pwm_ext_led1)
#define MIN_PERIOD        PWM_SEC(1U) / 128U
#define MAX_PERIOD        PWM_SEC(1U)

// LED
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(EXT_PWM_LED1);
static uint8_t dir;

// Global definitions
uint32_t max_period = STEPSIZE;
uint32_t min_period = MIN_PERIOD;

/**
 * @brief Initialize PWM for the external LED and ensure it is calibrated correctly
*/
int initPWM(void)
{
  dir = 0u;

  if (!pwm_is_ready_dt(&pwm_led0)) {
		printk("Error: PWM device %s is not ready\n", pwm_led0.dev->name);
		return 1;
	}

  /*
	 * In case the default MAX_PERIOD value cannot be set for
	 * some PWM hardware, decrease its value until it can.
	 *
	 * Keep its value at least MIN_PERIOD * 4 to make sure
	 * the sample changes frequency at least once.
	 */
	printk("Calibrating for channel %d...\n", pwm_led0.channel);
	max_period = MAX_PERIOD;
	while (pwm_set_dt(&pwm_led0, max_period, max_period / 2U)) {
		max_period /= 2U;
		if (max_period < (4U * MIN_PERIOD)) {
			printk("Error: PWM device does not support a period at least %lu\n",
			       4U * MIN_PERIOD);
			return 1;
		}
	}

	printk("Done calibrating; maximum/minimum periods %u/%lu nsec\n",
	       max_period, MIN_PERIOD);

  return 0;
}

/** 
 * @brief   Update PWM values based on encoder's value
 *          https://docs.zephyrproject.org/latest/hardware/peripherals/pwm.html
 * @param encoder_val		Value from encoder
*/
void updatePwmLED(uint32_t encoder_val)
{
  int ret;
  uint32_t step = pwm_led0.period / 50u;

  if (dir) {
    if (encoder_val >= pwm_led0.period) {
      encoder_val = pwm_led0.period - step;
      dir = 0U;
    }
  } else {
    if (encoder_val >= step) {
      encoder_val -= step;
    } else {
      encoder_val = step;
      dir = 1U;
    }
  }

  ret = pwm_set_pulse_dt(&pwm_led0, encoder_val);
  if (ret) {
    printk("Error %d: failed to set pulse width\n", ret);
  }
}
#include "led.h"

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

// The devicetree node identifiers for aliases
#define LED0_NODE         DT_ALIAS(led0)
#define EXT_LED1_NODE	    DT_ALIAS(ext_led1)

// PC6 - PWM/TIM3_CH1
#define EXT_PWM_LED1      DT_ALIAS(pwm_ext_led1)
#define MIN_PERIOD        PWM_SEC(1U) / 128U
#define MAX_PERIOD        PWM_SEC(1U)

// LED
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(EXT_PWM_LED1);
static struct gpio_dt_spec led           = GPIO_DT_SPEC_GET(EXT_LED1_NODE, gpios);
static uint8_t dir;

// Global definitions
uint32_t max_period = STEPSIZE * 100;
uint32_t min_period = MIN_PERIOD;

int initLED(void) 
{
  int ret;

  // led = GPIO_DT_SPEC_GET(EXT_LED1_NODE, gpios);

  // Setup LED
  ret = gpio_is_ready_dt(&led);
	if (led.port && !ret) {
		printk("Error %d: LED device %s is not ready; ignoring it\n",
		       ret, led.port->name);
		led.port = NULL;
	}

	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			led.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}

  return ret;
}

int initPWM(void)
{
  dir = 0u;

  if (!pwm_is_ready_dt(&pwm_led0)) {
		printk("Error: PWM device %s is not ready\n",
		       pwm_led0.dev->name);
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
			printk("Error: PWM device "
			       "does not support a period at least %lu\n",
			       4U * MIN_PERIOD);
			return 1;
		}
	}

	printk("Done calibrating; maximum/minimum periods %u/%lu nsec\n",
	       max_period, MIN_PERIOD);

  return 0;
}

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
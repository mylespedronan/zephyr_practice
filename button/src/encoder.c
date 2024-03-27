/**
 * Contains encoder data
 * Encoder layout:
 * - COM  -> GND
 * - B    -> PC9
 * - A    -> PC7
*/
#include "encoder.h"

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

// The devicetree node identifiers for aliases
#define ENC_RIGHT         DT_ALIAS(enc_right)
#define ENC_LEFT          DT_ALIAS(enc_left)

// Encoder left and right pins
static const struct gpio_dt_spec enc_right = GPIO_DT_SPEC_GET(ENC_RIGHT, gpios);
static const struct gpio_dt_spec enc_left = GPIO_DT_SPEC_GET(ENC_LEFT, gpios);

// Callback functions
static struct gpio_callback encoder_cb_right;
static struct gpio_callback encoder_cb_left;

// Encoder state/direction
static uint8_t lastState;
static uint8_t dir;

// Global definitions
uint32_t encoder_val = STEPSIZE;

// Local Prototypes
void encoder_right(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void encoder_left(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

int initEncoder(void)
{
  int ret = 0u;
  lastState = 0u;
  dir = 0u;

  // Check if GPIO for encoder right is ready
  if (!gpio_is_ready_dt(&enc_right)) {
    printk("Error: Enc Right Device %s is not ready\n", 
            enc_right.port->name);
    return 1;
  }

  // Configure GPIO as input/output
  ret = gpio_pin_configure_dt(&enc_right, GPIO_INPUT);
  if (ret != 0) {
    printk("Error %d: Failed to configure %s pin %d\n", 
            ret, enc_right.port->name, enc_right.pin);
    return ret;
  }

  // Configure interrupt
  ret = gpio_pin_interrupt_configure_dt(&enc_right, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, enc_right.port->name, enc_right.pin);
		return ret;
  }

  // Check if GPIO for encoder left is ready
  if (!gpio_is_ready_dt(&enc_left)) {
    printk("Error: Enc Left Device %s is not ready\n", 
            enc_left.port->name);
    return ret;
  }

  // Configure GPIO as input/output
  ret = gpio_pin_configure_dt(&enc_left, GPIO_INPUT);
  if (ret != 0) {
    printk("Error %d: Failed to configure %s pin %d\n", 
            ret, enc_left.port->name, enc_left.pin);
    return ret;
  }

  // Configure interrupt
  ret = gpio_pin_interrupt_configure_dt(&enc_left, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, enc_left.port->name, enc_left.pin);
		return ret;
  }

  // Configure encoder right callback functions
  gpio_init_callback(&encoder_cb_right, encoder_right, BIT(enc_right.pin));
  gpio_add_callback(enc_right.port, &encoder_cb_right);
	printk("Set up pin at %s pin %d\n", enc_right.port->name, enc_right.pin);

  // Configure encoder left callback functions
  gpio_init_callback(&encoder_cb_left, encoder_left, BIT(enc_left.pin));
  gpio_add_callback(enc_left.port, &encoder_cb_left);
	printk("Set up pin at %s pin %d\n", enc_left.port->name, enc_left.pin);

  return ret;
}

void encoder_right(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
  printk("Encoder moved right: %d\n", encoder_val);
}

void encoder_left(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
  printk("Encoder moved left: %d\n", encoder_val);
}

void updateEncoder(void)
{
  uint8_t enc_a = (uint8_t)gpio_pin_get_dt(&enc_left);
  uint8_t enc_b = (uint8_t)gpio_pin_get_dt(&enc_right) << 1;
  uint8_t state = enc_a | enc_b;

  if (lastState != state) {
    switch (state) {
      case 0:
        if (lastState == 2) {
          encoder_val -= (STEPSIZE * 4);
          dir = 0u;
        } else if (lastState == 1) {
          encoder_val += (STEPSIZE * 4);
          dir = 1u;
        }
        break;

      case 1:
        if (lastState == 0) {
          encoder_val -= (STEPSIZE * 4);
          dir = 0u;
        } else if (lastState == 1) {
          encoder_val += (STEPSIZE * 4);
          dir = 1u;
        }
        break;
      
      case 2:
        if (lastState == 3) {
          encoder_val -= (STEPSIZE * 4);
          dir = 0u;
        } else if (lastState == 0) {
          encoder_val += (STEPSIZE * 4);
          dir = 1u;
        }
        break;

      case 3:
        if (lastState == 1) {
          encoder_val -= (STEPSIZE * 4);
          dir = 0u;
        } else if (lastState == 2) {
          encoder_val += (STEPSIZE * 4);
          dir = 1u;
        }
        break;
      default:
        break;
    }
  }

  if (encoder_val <= (STEPSIZE + (STEPSIZE * 0.5))) {
    encoder_val = STEPSIZE;
  } else if (encoder_val >= min_period) {
    if (dir) {
      encoder_val = min_period;
    } else {
      encoder_val = STEPSIZE;
    }
  }

  lastState = state;
}
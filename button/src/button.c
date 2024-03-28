/**
 * This module contains code pertaining to the external buttons used to set and reset the LED
 * Set Button   - PE1
 * Reset Button - PE3
*/
#include "button.h"

// The devicetree node identifiers for aliases
#define EXT_BUTTON0       DT_ALIAS(ext_button0)
#define EXT_BUTTON1       DT_ALIAS(ext_button1)

// GPIO Callback
static struct gpio_callback set_button_cb_data;
static struct gpio_callback reset_button_cb_data;

// Set and reset buttons
const struct gpio_dt_spec setButton = GPIO_DT_SPEC_GET_OR(EXT_BUTTON0, gpios, {0});
const struct gpio_dt_spec resetButton = GPIO_DT_SPEC_GET_OR(EXT_BUTTON1, gpios, {0});

// Local Prototype
void set_button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void reset_button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

/**
 * @brief     Initialize buttons
 * @return    Error/Success code
*/
int initButton(void)
{
  int ret;

  // Setup Set button
  // Check if GPIO is ready
  if (!gpio_is_ready_dt(&setButton)) {
    printk("Error: Button Device %s is not ready\n", 
            setButton.port->name);
    return 1;
  }

  // Configure GPIO as input/output
  ret = gpio_pin_configure_dt(&setButton, GPIO_INPUT);
  if (ret != 0) {
    printk("Error %d: Failed to configure %s pin %d\n", 
            ret, setButton.port->name, setButton.pin);
    return ret;
  }

  // Configure interrupt
  ret = gpio_pin_interrupt_configure_dt(&setButton, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, setButton.port->name, setButton.pin);
		return ret;
  }

  // Setup Reset button
  // Check if GPIO is ready
  if (!gpio_is_ready_dt(&resetButton)) {
    printk("Error: Button Device %s is not ready\n", 
            resetButton.port->name);
    return 1;
  }

  // Configure GPIO as input/output
  ret = gpio_pin_configure_dt(&resetButton, GPIO_INPUT);
  if (ret != 0) {
    printk("Error %d: Failed to configure %s pin %d\n", 
            ret, resetButton.port->name, resetButton.pin);
    return ret;
  }

  // Configure interrupt
  ret = gpio_pin_interrupt_configure_dt(&resetButton, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, resetButton.port->name, resetButton.pin);
		return ret;
  }

  // Configure set button callback functions
  gpio_init_callback(&set_button_cb_data, set_button_pressed, BIT(setButton.pin));
  gpio_add_callback(setButton.port, &set_button_cb_data);
	printk("Set up button at %s pin %d\n", setButton.port->name, setButton.pin);
  
  // Configure reset button callback functions
  gpio_init_callback(&reset_button_cb_data, reset_button_pressed, BIT(resetButton.pin));
  gpio_add_callback(resetButton.port, &reset_button_cb_data);
	printk("Reset up button at %s pin %d\n", resetButton.port->name, resetButton.pin);

  return ret;
}

/**
 * @brief         Callback function for when the set button is pressed.
 *                This interrupt updates the PWM LED's value to the current encoder value
 * @param dev     Device Info
 * @param cb      GPIO Callback struct
 * @param pins    GPIO Pins
*/
void set_button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Set Button pressed at %" PRIu32 "\n", k_cycle_get_32());
  printk("Encoder Val: %d \n", encoder_val);
  updatePwmLED(encoder_val);
}

/**
 * @brief         Callback function for when the reset button is pressed.
 *                This interrupt updates the PWM LED's value to the minimum value
 * @param dev     Device Info
 * @param cb      GPIO Callback struct
 * @param pins    GPIO Pins
*/
void reset_button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Reset Button pressed at %" PRIu32 "\n", k_cycle_get_32());
  encoder_val = STEPSIZE;
  printk("Encoder Val: %d \n", encoder_val);
  updatePwmLED(encoder_val);
}

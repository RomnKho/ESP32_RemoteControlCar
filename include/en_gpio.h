/**
 * @file    gpio.h
 * @brief   Header file for digital input / output
 * @author  Roman Khomenko
 **/

#pragma once

#include "driver/gpio.h"

typedef enum {
    GPIO_OUTPUT_HIGH,
    GPIO_OUTPUT_LOW
} gpio_output_state_t;

/**
 * @brief       Initializes an output pin
 * @param[in]   gpio_pin => output pin
 **/
void gpio_output_init(gpio_num_t output_pin);

/**
 * @brief       Sets the pin to HIGH or LOW
 * @param[in]   state => HIGH / LOW
 **/
void gpio_output_set(gpio_num_t output_pin, gpio_output_state_t state);
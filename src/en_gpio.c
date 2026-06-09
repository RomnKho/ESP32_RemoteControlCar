/**
 * @file    gpio.c
 * @brief   Implementation file for input / output gpio
 * @author  Roman Khomenko
 **/

#include "driver/gpio.h"
#include "esp_log.h"
#include "../include/en_gpio.h"

static const char *TAG = "gpio";

void gpio_output_init(gpio_num_t output_pin)
{
    gpio_config_t io_conf = {
        .mode           = GPIO_MODE_OUTPUT,
        .pin_bit_mask   = 1ULL << output_pin,
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
        .pull_up_en     = GPIO_PULLUP_DISABLE,
        .intr_type      = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_LOGI(TAG, "output gpio config done");
}

void gpio_output_set(gpio_num_t output_pin, gpio_output_state_t state)
{
    if(state == GPIO_OUTPUT_HIGH)
    {
        ESP_ERROR_CHECK(gpio_set_level(output_pin, 1));
        ESP_LOGI(TAG, "GPIO_NUM_%d is HIGH", output_pin);
    }
    else
    {
        ESP_ERROR_CHECK(gpio_set_level(output_pin, 0));
        ESP_LOGI(TAG, "GPIO_NUM_%d is LOW", output_pin);
    }
}
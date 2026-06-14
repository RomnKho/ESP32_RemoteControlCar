/**
 * @file    main.c
 * @brief   Remote control car
 * @author  Roman Khomenko
 **/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "../include/motor_control.h"
#include "../include/joystick.h"

#define     INIT_DELAY      100
#define     MAIN_DELAY      100

// static const int16_t CENTER_ERROR = 10;

static int16_t  x_raw;
static int16_t  y_raw;
static uint32_t x_mV;
static uint32_t y_mV;

const char *TAG = "main";

void app_main(void)
{
    joystick_init();
    mcpwm_init();
    vTaskDelay(pdMS_TO_TICKS(INIT_DELAY));

    mcpwm_set_duty_forward(0);

    for(;;)
    {
        joystick_adc_read(&x_raw, &y_raw, &x_mV, &y_mV);

        if (x_raw >= -10 && x_raw <= 0)
        {
            mcpwm_set_duty_forward(0);
            ESP_LOGI(TAG, "Not moving motors");
        }
        else if (x_raw > 0)
        {
            mcpwm_set_duty_forward(x_raw);
            ESP_LOGI(TAG, "Moving forward %d%%", x_raw);
        }
        else // x_raw < -10 
        {
            mcpwm_set_duty_backwards(abs(x_raw));
            ESP_LOGI(TAG, "Moving backwards %d%%", abs(x_raw));
        }

        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
}
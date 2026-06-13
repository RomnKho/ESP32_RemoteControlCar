#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "../include/motor_control.h"
#include "../include/joystick.h"

const char *TAG = "main";

static int16_t x_raw;
static int16_t y_raw;
static uint32_t x_mV;
static uint32_t y_mV;

void app_main(void)
{
    joystick_init();

    for(;;)
    {
        joystick_adc_read(&x_raw, &y_raw, &x_mV, &y_mV);
        ESP_LOGI(TAG, "raw (x,y): %d | %d", x_raw, y_raw);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
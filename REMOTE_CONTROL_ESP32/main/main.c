/**
 * @file    main.c
 * @brief   Remote control ESP32
 * @author  Roman Khomenko
 **/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "../include/joystick.h"
#include "../include/esp_now_connection.h"

#define     MAIN_DELAY      50

const char *TAG = "main";

static int16_t x_raw;
static int16_t y_raw;

static uint32_t x_mV;
static uint32_t y_mV;

static esp_now_remote_control_data_t send_data;

void app_main(void)
{
    joystick_init();
    esp_now_connection_init();
    
    for(;;)
    {
        joystick_adc_read(&x_raw, &y_raw, &x_mV, &y_mV);

        send_data.payload.x_joystick = x_raw;
        send_data.payload.y_joystick = y_raw;
        
        esp_now_connection_send(&send_data);

        ESP_LOGI(TAG, "x: %d | y: %d", send_data.payload.x_joystick, send_data.payload.y_joystick);

        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
    
}
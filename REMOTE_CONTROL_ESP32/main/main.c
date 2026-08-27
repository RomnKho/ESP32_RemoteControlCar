/**
 * @file    main.c
 * @brief   Remote control ESP32
 * @author  Roman Khomenko
 **/

/* ========= INCLUDES =========== */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "../include/joystick.h"
#include "../include/esp_now_connection.h"


/* ========== DEFINES =========== */

#define     MAIN_DELAY      50

/* ====== PRIVATE VARIABLES ====== */

const char *TAG = "main";

static int16_t x_raw;
static int16_t y_raw;

static uint32_t x_mV;
static uint32_t y_mV;

static int16_t x_current = 0;
static int16_t y_current = 0;

static esp_now_remote_control_data_t send_data;

const float smoothout = 0.1f;

/* ====== MAIN ====== */

void app_main(void)
{
    joystick_init();
    esp_now_connection_init();
    
    for(;;)
    {
        joystick_adc_read(&x_raw, &y_raw, &x_mV, &y_mV);

        // As I have cheap motors I can't make them accelerate from 0 to 100 quickly as
        // it causes brownout 
        
        if (x_raw == 0)
        {
            x_current = x_raw;
        }
        else
        {
            x_current += (int16_t) ((x_raw - x_current) * smoothout);
        }
        
        if (y_raw == 0)
        {
            y_current = y_raw;
        }
        else
        {
            y_current += (int16_t) ((y_raw - y_current) * smoothout);
        }

        send_data.payload.x_joystick = x_current;
        send_data.payload.y_joystick = y_current;
        
        esp_now_connection_send(&send_data);

        ESP_LOGI(TAG, "x: %d | y: %d", send_data.payload.x_joystick, send_data.payload.y_joystick);

        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
    
}


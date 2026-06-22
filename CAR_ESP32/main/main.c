/**
 * @file    main.c
 * @brief   Car ESP32
 * @author  Roman Khomenko
 **/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "../include/motor_control.h"
#include "../include/esp_now_connection.h"

#define     MAIN_DELAY      50

static esp_now_remote_control_data_t recv_data;

const char *TAG = "main";

void app_main(void)
{
    mcpwm_init();
    esp_now_connection_init();

    for(;;)
    {
        if (xQueueReceive(esp_now_recv_queue, &recv_data, portMAX_DELAY) != pdTRUE)
        {
            ESP_LOGW(TAG, "Error receving data");
        }

        ESP_LOGI(TAG, "x: %d | y: %d", recv_data.payload.x_joystick, recv_data.payload.y_joystick);

        mcpwm_set_duty(recv_data.payload.y_joystick);

        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
}
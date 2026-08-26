/**
 * @file    main.c
 * @brief   Car ESP32
 * @author  Roman Khomenko
 **/

/** 
 * @todo: 
 *      - Leer el joystick e interpretar 
 *      - Añadir lado derecho de las ruedas tantos hardware como software
 *      - Setear el duty a cada lado por separado (funcion nueva) => mcpwm_set_duty_right & mcpwm_set_duty_left
 */


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "../include/motor_control.h"
#include "../include/esp_now_connection.h"

#define     MAIN_DELAY      2000
#define     LEFT            1
#define     RIGHT           2

// static esp_now_remote_control_data_t recv_data;

const char *TAG = "main";

void app_main(void)
{
    mcpwm_init(LEFT);  
    mcpwm_init(RIGHT);  
    // esp_now_connection_init();

    for(;;)
    {
        #if 0

        if (xQueueReceive(esp_now_recv_queue, &recv_data, portMAX_DELAY) != pdTRUE)
        {
            ESP_LOGW(TAG, "Error receving data");
        }

        ESP_LOGI(TAG, "x: %d | y: %d", recv_data.payload.x_joystick, recv_data.payload.y_joystick);

        #endif

        mcpwm_set_duty(LEFT, 20);
        mcpwm_set_duty(RIGHT, 20);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, 50);
        mcpwm_set_duty(RIGHT, 50);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, 70);
        mcpwm_set_duty(RIGHT, 70);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, 100);
        mcpwm_set_duty(RIGHT, 100);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, 0);
        mcpwm_set_duty(RIGHT, 0);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, -20);
        mcpwm_set_duty(RIGHT, -20);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, -50);
        mcpwm_set_duty(RIGHT, -50);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, -70);
        mcpwm_set_duty(RIGHT, -70);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, -100);
        mcpwm_set_duty(RIGHT, -100);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));

        mcpwm_set_duty(LEFT, 0);
        mcpwm_set_duty(RIGHT, 0);
        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
}
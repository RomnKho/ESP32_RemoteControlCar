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

#define     MAX_JOYSTICK    100
#define     MAIN_DELAY      50
#define     LEFT            1
#define     RIGHT           2

static esp_now_remote_control_data_t recv_data;

const char *TAG = "main";

void app_main(void)
{
    mcpwm_init(LEFT);  
    mcpwm_init(RIGHT);  
    esp_now_connection_init();

    for(;;)
    {
        if (xQueueReceive(esp_now_recv_queue, &recv_data, portMAX_DELAY) != pdTRUE)
        {
            ESP_LOGW(TAG, "Error receving data");
        }

        ESP_LOGI(TAG, "x: %d | y: %d", recv_data.payload.x_joystick, recv_data.payload.y_joystick);

        if (recv_data.payload.x_joystick == 0)
        {
            mcpwm_set_duty(LEFT, recv_data.payload.y_joystick);
            mcpwm_set_duty(RIGHT, recv_data.payload.y_joystick);
        }
        else if (recv_data.payload.x_joystick < 0)
        {
            mcpwm_set_duty(LEFT, recv_data.payload.y_joystick);
            mcpwm_set_duty(RIGHT, (int16_t)(recv_data.payload.y_joystick * ((float) abs(recv_data.payload.x_joystick) / MAX_JOYSTICK)));
        }
        else 
        {
            mcpwm_set_duty(LEFT, (int16_t)(recv_data.payload.y_joystick * ((float) recv_data.payload.x_joystick / MAX_JOYSTICK)));
            mcpwm_set_duty(RIGHT, recv_data.payload.y_joystick);
        }

        vTaskDelay(pdMS_TO_TICKS(MAIN_DELAY));
    }
}
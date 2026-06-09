#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "../include/motor_control.h"

const char *TAG = "main";

void app_main(void)
{
    mcpwm_init();

    ESP_LOGI(TAG, "testing forward");
    mcpwm_set_duty_forward(25);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(50);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(75);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(100);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(0);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(200);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(100);
    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_LOGI(TAG, "testing backwards");
    mcpwm_set_duty_backwards(0);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(25);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(50);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(75);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(100);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(0);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(200);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(100);
    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_LOGI(TAG, "testing intercalation");
    mcpwm_set_duty_forward(25);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(25);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(50);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(50);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(75);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(75);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(100);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_backwards(100);
    vTaskDelay(pdMS_TO_TICKS(2000));

    mcpwm_set_duty_forward(0);
    vTaskDelay(pdMS_TO_TICKS(2000));
}
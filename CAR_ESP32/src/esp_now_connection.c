/**
 * @file    connect.c
 * @brief   Implementation file for connection between two ESP32 using ESP-NOW (Receiver)
 * @author  Roman Khomenko
 **/

#include <string.h>
#include "esp_now.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "../include/esp_now_connection.h"

#define     WIFI_MODE               WIFI_MODE_STA       // In the future, add WIFI_MODE_STA_SOFTAP for camera udp
#define     WIFI_IF_MODE            WIFI_IF_STA
#define     WIFI_ESP_NOW_CHANNEL    1        
#define     RECV_QUEUE_SIZE         200           

QueueHandle_t esp_now_recv_queue = NULL;

static esp_now_remote_control_data_t recv_data;

static const char *TAG = "esp_now_connection.c";

/**
 * - Registrar callbacks privados al modulo
 * - Despues esp_now_send() y esp_now_receive() se encargan de llamarlas
 * - Hay que definir la estructura que mando
 * - En el repo de github esta como definir las funciones
 * - Iniciar wifi en station mode antes
 * - Tarea de recibir y de enviar por separado
 **/

// Initialize wifi before ESP-NOW
static void wifi_init(void)
{
    // Initialize netif => manages connection events
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize station mode
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(WIFI_ESP_NOW_CHANNEL, WIFI_SECOND_CHAN_NONE));

    // Maybe long range in some moment?
}

/**
 * @brief   Callback that grabs quickly all the recv information and puts it in a queue
 *          where a task with less priority treats it
 **/
static void esp_now_connection_recv_cb(const esp_now_recv_info_t *rx_info, const uint8_t *data, int len)
{
    if (rx_info == NULL)
    {   
        ESP_LOGE(TAG, "receive info arg error");
        return;
    }

    // Copy in memory of payload
    memcpy(&recv_data, data, len);
    if (xQueueSend(esp_now_recv_queue, &recv_data, 0) != pdTRUE)
    {
        ESP_LOGW(TAG, "recv queue full");
    }

}

// Don't register anything as it will only receiving data
esp_err_t esp_now_connection_init(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    wifi_init();
    ESP_LOGI(TAG, "wifi initialized");

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_connection_recv_cb));

    esp_now_recv_queue = xQueueCreate(RECV_QUEUE_SIZE, sizeof(esp_now_remote_control_data_t));
    if (esp_now_recv_queue == NULL)
    {
        ESP_LOGE(TAG, "create recv queue fail");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "wifi and esp_now initialized correctly");
    return ESP_OK;
}

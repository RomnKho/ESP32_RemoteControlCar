/**
 * @file    connect.h
 * @brief   Header file for connection between two ESP32 using ESP-NOW (Receiver)
 * @author  Roman Khomenko
 **/

#pragma once

/* ===== INCLUDES ===== */

#include "esp_now.h"
#include "freertos/queue.h"

/* ===== DEFINES ===== */
#define     MAC_ADDR_BYTES      6

/* ===== EXPORTED VARIABLES ===== */

extern QueueHandle_t esp_now_recv_queue;

/* ===== STRUCTURES ===== */

// What is sent to the other ESP32
typedef struct 
{
    int16_t x_joystick;
    int16_t y_joystick;
} __attribute__((packed)) joystick_info_t;

typedef struct 
{
    joystick_info_t payload;
} esp_now_remote_control_data_t;

/* ===== EXPORTED FUNCTIONS ===== */

/**
 * @brief   Initializes esp_now and wifi
 **/
esp_err_t esp_now_connection_init(void);

/**
 * @brief   Sends the info to the other ESP
 **/
esp_err_t esp_now_connection_recv(const esp_now_recv_info_t *rx_info, const uint8_t *data, int len);
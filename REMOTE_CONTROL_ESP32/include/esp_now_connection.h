/**
 * @file    connect.h
 * @brief   Header file for connection between two ESP32 using ESP-NOW (Sender)
 * @author  Roman Khomenko
 **/

#pragma once

/* ===== DEFINES ===== */
#define     MAC_ADDR_BYTES      6

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
esp_err_t esp_now_connection_send(esp_now_remote_control_data_t *send_data);
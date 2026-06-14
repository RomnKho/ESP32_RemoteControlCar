/**
 * @file    joystick.h
 * @brief   Header file to read correctly a joystick
 * @author  Roman Khomenko
 **/

#pragma once

/** 
 * @brief       Initiliazes both 'x' and 'y' channels with One-Shot Mode Driver and Calibrates
 * @important   Joystick to 3.3V
 **/
void joystick_init(void);

/**
 * @brief   Reads both 'x' and 'y' channels
 * @param[out]  x_read => x channel read [-100,100]
 * @param[out]  y_read => y channel read [-100,100]
 * @param[out]  mV_x   => x channel mV
 * @param[out]  mV_y   => y channel mV
 **/
void joystick_adc_read(int16_t *x_read, int16_t *y_read, uint32_t *mV_x, uint32_t *mV_y);

void joystick_button_read(void);
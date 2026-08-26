/**
 * @file    motor_control.h
 * @brief   Header file for MCPWM to control brushed DC motors
 * @author  Roman Khomenko
 **/

#pragma once

/** 
 * @brief       Initializes the needed submodules for MCPWM
 * @param       side => 1 = left / 2 = right / anything else = error
 **/
void mcpwm_init(uint8_t side); 

/**
 * @brief       Sets the duty cycle for the PWM to go forward / backwards on the chosen side of the car
 * @if          per_duty is not in range, it does not do anything
 * @param[in]   per_duty => Integer in range [-100, 100]
 * @param[in]   side => 1 = left / 2 = right / anything else = error
 **/
void mcpwm_set_duty(uint8_t side, int16_t per_duty);


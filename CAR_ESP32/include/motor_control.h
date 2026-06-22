/**
 * @file    motor_control.h
 * @brief   Header file for MCPWM to control brushed DC motors
 * @author  Roman Khomenko
 **/

#pragma once

/** 
 * @brief       Initializes the needed submodules for MCPWM
 **/
void mcpwm_init(void); 

/**
 * @brief       Sets the duty cycle for the PWM to go forward / backwards => If per_duty is not in range, it does not do anything
 * @param[in]   per_duty => Integer in range [-100, 100]
 **/
void mcpwm_set_duty(int8_t per_duty);

/**
 * @file    motor_control.c
 * @brief   Implementation file to control brushed DC motor with MCPWM
 * @author  Roman Khomenko
 **/

/* ====== INCLUDES ============== */

#include "driver/gpio.h"
#include "driver/mcpwm_timer.h"
#include "driver/mcpwm_oper.h"
#include "driver/mcpwm_cmpr.h"
#include "driver/mcpwm_gen.h"
#include "esp_log.h"
#include "../include/motor_control.h"

/* ====== DEFINES =============== */

#define     MCPWM_GROUP_ID      0
#define     MCPWM_CLK_SOURCE    MCPWM_TIMER_CLK_SRC_DEFAULT
#define     MCPWM_CLK_HZ        1000000                     // 1 MHz
#define     MCPWM_FREQ          2000
#define     MCPWM_COUNT_MODE    MCPWM_TIMER_COUNT_MODE_UP   // 1, 2, 3, ... , 500, 1, 2, ...
#define     MCPWM_GPIO          GPIO_NUM_25

static const char *TAG = "motor_control";

/* ====== EXPORTED FUNCTIONS ===== */

void init_mcpwm(void)
{
    // Timer config => Sets the timing of other submodules
    mcpwm_timer_handle_t timer_handle = NULL;
    mcpwm_timer_config_t timer_conf = {
        .group_id       = MCPWM_GROUP_ID,               // All the config needs to be done on the same group
        .clk_src        = MCPWM_CLK_SOURCE,        
        .resolution_hz  = MCPWM_CLK_HZ,                 // Sets the precision in which the duty cicle works
        .period_ticks   = MCPWM_CLK_HZ / MCPWM_FREQ,    // Counts how many ticks fit in one period 
        .count_mode     = MCPWM_COUNT_MODE
    };

    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_conf, &timer_handle));
    ESP_LOGI(TAG, "timer config done");

    // Operator config => Responsible for generating the PWM waveforms => Connects timer with generators
    mcpwm_oper_handle_t oper_handle = NULL;
    mcpwm_operator_config_t oper_conf = {
        .group_id = MCPWM_GROUP_ID
    };

    ESP_ERROR_CHECK(mcpwm_new_operator(&oper_conf, &oper_handle));
    ESP_LOGI(TAG, "operator config done");

    // Connect timer with operator
    mcpwm_operator_connect_timer(oper_handle, timer_handle);

    // Comparator config => Compares timer value with threshold to decide HIGH or LOW
    mcpwm_cmpr_handle_t cmpr_handle = NULL;
    mcpwm_comparator_config_t cmpr_config = {
        .flags.update_cmp_on_tez = true                 // pwm only changes when ticks = 0 (Every period)
    };

    ESP_ERROR_CHECK(mcpwm_new_comparator(oper_handle, &cmpr_config, &cmpr_handle));
    ESP_LOGI(TAG, "comparator config done");

    // Generator config => Connects with gpio
    mcpwm_gen_handle_t gen_handle = NULL;
    mcpwm_generator_config_t gen_config = {
        .gen_gpio_num = MCPWM_GPIO
    };

    ESP_ERROR_CHECK(mcpwm_new_generator(oper_handle, &gen_config, &gen_handle));

    // Config how the generator works => When HIGH and when LOW
    mcpwm_generator_set_actions_on_timer_event(gen_handle, 
        
        // When ticks = 0 => Generator goes HIGH
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
                                     MCPWM_TIMER_EVENT_EMPTY,
                                     MCPWM_GEN_ACTION_HIGH),

        // When it gets to threshold it changes to low
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_DOWN,
                                        cmpr_handle,
                                        MCPWM_GEN_ACTION_LOW),
        
        // End of actions
        MCPWM_GEN_TIMER_EVENT_ACTION_END()
    );
    ESP_LOGI(TAG, "generator config done");

    // Establish initial compare value at 0
    mcpwm_comparator_set_compare_value(cmpr_handle, 0);

    // Start timer
    mcpwm_timer_enable(timer_handle);
    mcpwm_timer_start_stop(timer_handle, MCPWM_TIMER_START_NO_STOP);

    ESP_LOGI(TAG, "MCPWM config done");
}
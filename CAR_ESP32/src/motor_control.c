    /**
     * @file    motor_control.c
     * @brief   Implementation file to control brushed DC motor with MCPWM
     * @author  Roman Khomenko
     **/

    /* ========= INCLUDES =========== */

    #include "driver/gpio.h"
    #include "driver/mcpwm_timer.h"
    #include "driver/mcpwm_oper.h"
    #include "driver/mcpwm_cmpr.h"
    #include "driver/mcpwm_gen.h"
    #include "esp_log.h"
    #include "../include/motor_control.h"
    #include "../include/en_gpio.h"

    /* ========== DEFINES =========== */

    #define     MCPWM_GROUP_ID      0
    #define     MCPWM_CLK_SOURCE    MCPWM_TIMER_CLK_SRC_DEFAULT
    #define     MCPWM_CLK_HZ        1000000                     // 1 MHz
    #define     MCPWM_FREQ          2000                        // More freq less beep but doesnt work with low duties (At 2000 Hz - 50 % or more no beep)
    #define     MCPWM_PERIOD_TICKS  (MCPWM_CLK_HZ / MCPWM_FREQ)
    #define     MCPWM_COUNT_MODE    MCPWM_TIMER_COUNT_MODE_UP   // 1, 2, 3, ... , 500, 1, 2, ...
    #define     MCPWM_GPIO_LEFT     GPIO_NUM_14
    #define     IN1_GPIO            GPIO_NUM_27
    #define     IN2_GPIO            GPIO_NUM_26
    #define     MCPWM_GPIO_RIGHT    GPIO_NUM_32
    #define     IN3_GPIO            GPIO_NUM_25
    #define     IN4_GPIO            GPIO_NUM_33

    /* ====== PRIVATE VARIABLES ====== */

    static mcpwm_timer_handle_t timer_handle         = NULL;
    static mcpwm_oper_handle_t  oper_handle          = NULL;
    static mcpwm_cmpr_handle_t  cmpr_handle_left     = NULL;
    static mcpwm_cmpr_handle_t  cmpr_handle_right    = NULL;
    static mcpwm_gen_handle_t   gen_handle_left      = NULL;
    static mcpwm_gen_handle_t   gen_handle_right     = NULL;

    static const char *TAG = "motor_control";

    /* ====== EXPORTED FUNCTIONS ===== */

    static void mcpwm_system_init()
    {
        // Timer config => Sets the timing of other submodules
        mcpwm_timer_config_t timer_conf = {
            .group_id       = MCPWM_GROUP_ID,               // All the config needs to be done on the same group
            .clk_src        = MCPWM_CLK_SOURCE,        
            .resolution_hz  = MCPWM_CLK_HZ,                 // Sets the precision in which the duty cicle works
            .period_ticks   = MCPWM_PERIOD_TICKS,           // Counts how many ticks fit in one period 
            .count_mode     = MCPWM_COUNT_MODE
        };

        ESP_ERROR_CHECK(mcpwm_new_timer(&timer_conf, &timer_handle));
        ESP_LOGI(TAG, "timer config done");

        // Operator config => Responsible for generating the PWM waveforms => Connects timer with generators
        mcpwm_operator_config_t oper_conf = {
            .group_id = MCPWM_GROUP_ID
        };

        
        ESP_ERROR_CHECK(mcpwm_new_operator(&oper_conf, &oper_handle));
        ESP_LOGI(TAG, "operator config done");

        // Connect timer with operator
        ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper_handle, timer_handle));

        gpio_output_init(IN1_GPIO);
        gpio_output_init(IN2_GPIO);
        gpio_output_init(IN3_GPIO);
        gpio_output_init(IN4_GPIO);

    }

    void mcpwm_init(uint8_t side)
    {

        gpio_num_t mcpwm_side = GPIO_NUM_NC; // Not connected
        mcpwm_cmpr_handle_t *cmpr_handle = NULL;
        mcpwm_gen_handle_t  *gen_handle  = NULL;

        if (side == 1) // left
        {
            mcpwm_side = MCPWM_GPIO_LEFT;
            cmpr_handle = &cmpr_handle_left;
            gen_handle = &gen_handle_left;
        }
        else if (side == 2) // right
        {
            mcpwm_side = MCPWM_GPIO_RIGHT;
            cmpr_handle = &cmpr_handle_right;
            gen_handle = &gen_handle_right;
        }
        else
        {
            mcpwm_side = GPIO_NUM_NC;
            ESP_LOGE(TAG, "Wrong input side in mcpwm_init");
            return;
        }

        if (timer_handle == NULL)
        {
            mcpwm_system_init();
        }

        // Comparator config => Compares timer value with threshold to decide HIGH or LOW
        mcpwm_comparator_config_t cmpr_config = {
            .flags.update_cmp_on_tez = true                 // pwm only changes when ticks = 0 (Every period)
        };

        ESP_ERROR_CHECK(mcpwm_new_comparator(oper_handle, &cmpr_config, cmpr_handle));
        ESP_LOGI(TAG, "comparator config done");

        // Generator config => Connects with gpio
        mcpwm_generator_config_t gen_config = {
            .gen_gpio_num = mcpwm_side
        };

        ESP_ERROR_CHECK(mcpwm_new_generator(oper_handle, &gen_config, gen_handle));

        // Config how the generator behaves on timer events => When HIGH and when LOW 
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_timer_event(*gen_handle, // * => value (dereferenced) / & => changes value (function)
            
            // When ticks = 0 => Generator goes HIGH
            MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
                                        MCPWM_TIMER_EVENT_EMPTY,
                                        MCPWM_GEN_ACTION_HIGH),
            
            MCPWM_GEN_TIMER_EVENT_ACTION_END()
        ));    

        // Config how the generator behaves on comaparator events => When HIGH and when LOW 
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(*gen_handle,
            // When it gets to threshold it changes to low
            MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
                                            *cmpr_handle,
                                            MCPWM_GEN_ACTION_LOW)
        ));
        ESP_LOGI(TAG, "generator config done");

        // Establish initial compare value at 0
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(*cmpr_handle, 0));

        ESP_LOGI(TAG, "MCPWM config done");

        if((cmpr_handle_left != NULL) && (cmpr_handle_right != NULL))
        {
            // Start timer
            ESP_ERROR_CHECK(mcpwm_timer_enable(timer_handle));
            ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer_handle, MCPWM_TIMER_START_NO_STOP));
        }
    }

    void mcpwm_set_duty(uint8_t side, int16_t per_duty)
    {

        mcpwm_cmpr_handle_t cmpr_handle = NULL;
        gpio_num_t INA_GPIO = GPIO_NUM_NC;
        gpio_num_t INB_GPIO = GPIO_NUM_NC;   
        
        if (side == 1)
        {
            cmpr_handle = cmpr_handle_left;
            INA_GPIO = IN1_GPIO;
            INB_GPIO = IN2_GPIO; 
        }
        else if (side == 2)
        {
            cmpr_handle = cmpr_handle_right;
            INA_GPIO = IN3_GPIO;
            INB_GPIO = IN4_GPIO; 
        }
        else
        {
            cmpr_handle = NULL;
            INA_GPIO = GPIO_NUM_NC;
            INB_GPIO = GPIO_NUM_NC; 
            ESP_LOGE(TAG, "Wrong input side in set duty");
            return;
        }

        if (abs(per_duty) > 100)
        {
            ESP_LOGE(TAG, "duty percentage error: %d, side: %d", per_duty, side);
            return;
        }
        
        uint32_t threshold = (((uint32_t)abs(per_duty) * MCPWM_PERIOD_TICKS) / 100);
        if (threshold >= MCPWM_PERIOD_TICKS) threshold = MCPWM_PERIOD_TICKS - 1;

        if (per_duty == 0)
        {
            gpio_output_set(INA_GPIO, GPIO_OUTPUT_LOW);
            gpio_output_set(INB_GPIO, GPIO_OUTPUT_LOW);
            ESP_LOGI(TAG, "stop moving side: %d", side);
        }
        else if (per_duty >= 0)
        {
            gpio_output_set(INB_GPIO, GPIO_OUTPUT_LOW);
            gpio_output_set(INA_GPIO, GPIO_OUTPUT_HIGH);
            ESP_LOGI(TAG, "forward duty set succesfully, per_duty: %d, side: %d", per_duty, side);
        }
        else
        {
            gpio_output_set(INA_GPIO, GPIO_OUTPUT_LOW);
            gpio_output_set(INB_GPIO, GPIO_OUTPUT_HIGH);
            ESP_LOGI(TAG, "backwards duty set succesfully, per_duty: %d, side: %d", per_duty, side);
        }

        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(cmpr_handle, (uint16_t)threshold));
    }

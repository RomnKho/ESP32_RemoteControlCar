idf_component_register(
    SRCS
        "../src/en_gpio.c"
        "../src/motor_control.c" 
        "../src/esp_now_connection.c"
        "main.c"
                    
    INCLUDE_DIRS 
        "." 
        "../include"
)
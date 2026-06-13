idf_component_register(
    SRCS
        "../src/en_gpio.c"
        "../src/motor_control.c" 
        "../src/joystick.c"
        "main.c"
                    
    INCLUDE_DIRS 
        "." 
        "../include"
)
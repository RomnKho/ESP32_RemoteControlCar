idf_component_register(
    SRCS 
        "../src/en_gpio.c"
        "../src/joystick.c"
        "../src/esp_now_connection.c"
        "main.c"
                    
    INCLUDE_DIRS 
        "."
        "../include"
    )
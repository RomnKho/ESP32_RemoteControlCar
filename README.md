## Remote control car project

Project where the main focus is to build a remote control car with two ESP32s. 

### Materials

- ESP32 (Any)
- L298N Motor driver module (Get better!)
- 6 battery shield (Alternatively 2 Li-On Battery shield)
- Arduino ultrasound sensor
- Geared DC motors (Get better!)

### Things to add

- UDP Client / Server to communicate both ESP32s with bigger / slower packages
- Confgure the ultrasound sensor
- Configure the init button in the car
- Dash camera

### Things done

- MCPWM to control the motors
- ADC to read the joystick
- Basic ESP-NOW
- Car direction
- Materials

### Pinout

#### Controller ESP32
- ADC1: GPIO32
- ADC2: GPIO33

#### Car ESP32
- MCPWM_LEFT: GPIO14
- MCPWM_RIGHT: GPIO32
- IN1: GPIO27
- IN2: GPIO26
- IN3: GPIO25
- IN4: GPIO33

#### Joystick must be connected to 3.3V
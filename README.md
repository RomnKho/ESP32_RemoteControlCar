## Remote control car project

Project where the main focus is to build a remote control car with two ESP32s. 

### Materials

- ESP32 (Any)
- L298N Motor driver module
- 6 battery shield (Alternatively 2 Li-On Battery shield)
- Arduino ultrasound sensor
- Geared DC motors

### Things to add

- ESP-NOW to communicate bowth ESP32s with fast / small packages
- UDP Client / Server to communicate both ESP32s with bigger / slower packages
- Confgure the ultrasound sensor
- Configure the init button in the car
- Dash camera
- Car direction
- Materials

### Things done

- MCPWM to control the motors
- ADC to read the joystick
#ifndef BOARD_DEFAULT_ESP32_H
#define BOARD_DEFAULT_ESP32_H

#define BOARD_NAME "ESP32 DevKit Standard"

// LED & Buzzer
#define PIN_STATUS_LED   2
#define PIN_BUZZER       18

// Relays / Actuators
#define PIN_RELAY_1      23
#define PIN_RELAY_2      19
#define PIN_RELAY_3      18
#define PIN_RELAY_4      5

// Buttons / Inputs
#define PIN_BUTTON_USER  0
#define PIN_BUTTON_CONFIG 0

// Sensors / Analog
#define PIN_ANALOG_1     34
#define PIN_ANALOG_2     35
#define PIN_ANALOG_3     32
#define PIN_ANALOG_4     33

// I2C Bus
#define PIN_I2C_SDA      21
#define PIN_I2C_SCL      22

// UART Modbus RS485
#define PIN_RS485_RX     16
#define PIN_RS485_TX     17
#define PIN_RS485_DE_RE  4

#endif /* BOARD_DEFAULT_ESP32_H */


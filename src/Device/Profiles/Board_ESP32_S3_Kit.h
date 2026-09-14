#ifndef BOARD_ESP32_S3_KIT_H
#define BOARD_ESP32_S3_KIT_H

#define BOARD_NAME "ESP32-S3 AIoT DevKit"

// LED & Buzzer
#define PIN_STATUS_LED   2
#define PIN_RGB_LED      48 // WS2812 RGB LED trên ESP32-S3
#define PIN_BUZZER       21

// Relays / Actuators
#define PIN_RELAY_1      4
#define PIN_RELAY_2      5
#define PIN_RELAY_3      6
#define PIN_RELAY_4      7

// Buttons
#define PIN_BUTTON_USER  0
#define PIN_BUTTON_CONFIG 0

// Sensors / Analog (ADC1 trên ESP32-S3)
#define PIN_ANALOG_1     1
#define PIN_ANALOG_2     2
#define PIN_ANALOG_3     3
#define PIN_ANALOG_4     8

// I2C Bus
#define PIN_I2C_SDA      8
#define PIN_I2C_SCL      9

// I2S Microphone (Cho Voice / Sound AI)
#define PIN_I2S_MIC_SCK  41
#define PIN_I2S_MIC_WS   42
#define PIN_I2S_MIC_SD   40

// UART Modbus RS485
#define PIN_RS485_RX     17
#define PIN_RS485_TX     18
#define PIN_RS485_DE_RE  16

#endif /* BOARD_ESP32_S3_KIT_H */

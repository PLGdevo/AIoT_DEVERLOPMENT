#ifndef BOARD_AIOT_INDUSTRIAL_H
#define BOARD_AIOT_INDUSTRIAL_H

#define BOARD_NAME "AIoT Industrial Controller"

// LED & Buzzer
#define PIN_STATUS_LED   2
#define PIN_BUZZER       25

// Industrial Optocoupler Isolated Relays
#define PIN_RELAY_1      26
#define PIN_RELAY_2      27
#define PIN_RELAY_3      14
#define PIN_RELAY_4      12

// Opto-Isolated Digital Inputs
#define PIN_DIGITAL_IN_1 36
#define PIN_DIGITAL_IN_2 39
#define PIN_DIGITAL_IN_3 34
#define PIN_DIGITAL_IN_4 35

// Industrial 0-10V / 4-20mA Analog Inputs
#define PIN_ANALOG_1     32
#define PIN_ANALOG_2     33

// RS485 Modbus Industrial Port
#define PIN_RS485_RX     16
#define PIN_RS485_TX     17
#define PIN_RS485_DE_RE  4

// Buttons
#define PIN_BUTTON_USER  0
#define PIN_BUTTON_CONFIG 0

#endif /* BOARD_AIOT_INDUSTRIAL_H */


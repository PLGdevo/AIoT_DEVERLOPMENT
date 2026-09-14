#ifndef BOARD_ESP32_CAM_H
#define BOARD_ESP32_CAM_H

#define BOARD_NAME "ESP32-CAM AI Vision"

// On-board Flash LED & Red Status LED
#define PIN_FLASH_LED    4
#define PIN_STATUS_LED   33

// Relay
#define PIN_RELAY_1      12
#define PIN_RELAY_2      13

// Camera OV2640 Pinout
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

#endif /* BOARD_ESP32_CAM_H */


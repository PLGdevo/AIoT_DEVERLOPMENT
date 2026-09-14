#include <Arduino.h>

#ifndef DEBUG_COLOR
#define DEBUG_COLOR
#endif

#ifndef BUTTON_CONFIG
#define BUTTON_CONFIG // Bật chế độ cấu hình Captive Portal AP qua nút bấm
#endif

#include <AIoT.h>

void setup()
{
    Serial.begin(115200);

    // Bắt đầu chế độ cấu hình nếu chưa có WiFi đã lưu trong Flash
    // ESP32 sẽ phát WiFi AP để quét và cấu hình mạng
    AIoT.begin("", "");
}

void loop()
{
    AIoT.run();
}

#include <Arduino.h>
#define DEBUG_COLOR
#define BUTTON_CONFIG // Bật chế độ cấu hình Captive Portal AP qua nút bấm
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


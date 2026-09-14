#include <Arduino.h>
#define DEBUG_COLOR
#include <AIoT.h>

const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASS = "YOUR_WIFI_PASSWORD";

#define RELAY_PIN 2

// Bắt sự kiện điều khiển từ Web UI / Cloud AI qua MQTT topic: device/<MAC>/control
Virtual_WRITE(relay1)
{
    int state = param.getInt();
    digitalWrite(RELAY_PIN, state ? HIGH : LOW);
    Serial.printf("[CONTROL] Relay 1 da chuyen sang: %d\n", state);

    // Xác nhận lại trạng thái với Server
    AIoT.writeControl("relay1", state);
}

void setup()
{
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    // Khởi tạo thư viện
    AIoT.begin(WIFI_SSID, WIFI_PASS);
}

void loop()
{
    AIoT.run();
}


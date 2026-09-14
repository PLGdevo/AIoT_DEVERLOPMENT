#include <Arduino.h>

#ifndef DEBUG_COLOR
#define DEBUG_COLOR
#endif

#include <AIoT.h>

const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASS = "YOUR_WIFI_PASSWORD";

void sendSensorData()
{
    if (!AIoT.CheckConnect()) return;

    float chipTemp = temperatureRead();
    uint32_t freeRam = ESP.getFreeHeap();

    Serial.printf("[TELEMETRY] Temp: %.2f *C | Free RAM: %u bytes\n", chipTemp, freeRam);
    
    // Gửi dữ liệu lên HiveMQ Cloud: device/<MAC>/telemetry
    AIoT.writeTelemetry("chip_temp", chipTemp);
    AIoT.writeTelemetry("free_ram", (int)freeRam);
}

void setup()
{
    Serial.begin(115200);

    // Khởi tạo kết nối WiFi & HiveMQ Cloud TLS
    AIoT.begin(WIFI_SSID, WIFI_PASS);

    // Gửi định kỳ mỗi 3 giây
    AIoT.addTimeEvent(3000, sendSensorData);
}

void loop()
{
    AIoT.run();
}

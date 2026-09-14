/**
 * ============================================================================
 * AIoT_LIB - Ví Dụ 01: Cơ Bản Về IoT & Đồng Bộ Dữ Liệu Lên Cloud
 * ============================================================================
 * Mô tả:
 * - Kết nối WiFi tự động (qua code hoặc qua Captive Portal Web AP: 192.168.21.6)
 * - Đồng bộ dữ liệu nhiệt độ chip, RAM, RSSI lên HiveMQ Cloud (TLS Port 8883)
 * - Lắng nghe sự kiện điều khiển từ xa qua macro Virtual_WRITE
 */

#include <Arduino.h>
#include <AIoT.h>

const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASS = "YOUR_WIFI_PASS";

// Thông tin tài khoản HiveMQ Cloud
const char *MQTT_USER = "IoT_TEST";
const char *MQTT_PASS = "mt21062005";

#define RELAY_PIN 2

// Bắt sự kiện điều khiển Relay từ Cloud / App
Virtual_WRITE(relay1)
{
    int state = param.getInt();
    digitalWrite(RELAY_PIN, state ? HIGH : LOW);
    Serial.printf("[MQTT RECV] Điều khiển Relay: %d\n", state);

    // Xác nhận phản hồi trạng thái ngược lại Server
    AIoT.writeControl("relay1", state);
}

// Gửi Telemetry định kỳ mỗi 3 giây
void sendTelemetryData()
{
    if (!AIoT.CheckConnect())
        return;

    float chipTemp = temperatureRead();
    uint32_t freeRam = ESP.getFreeHeap();
    int8_t wifiRssi = WiFi.RSSI();
    unsigned long uptime = millis() / 1000;

    Serial.printf("📡 [TELEMETRY] Temp: %.2f *C | RAM: %u | RSSI: %d dBm | Uptime: %lu s\n",
                  chipTemp, freeRam, wifiRssi, uptime);

    AIoT.updateTelemetry("chip_temp", chipTemp);
    AIoT.updateTelemetry("free_ram", (int)freeRam);
    AIoT.updateTelemetry("wifi_rssi", wifiRssi);
    AIoT.updateTelemetry("uptime", (int)uptime);

    // Gửi đúng 1 gói tin JSON duy nhất
    AIoT.sendTelemetry();
}

void setup()
{
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    // Khởi tạo thư viện AIoT
    AIoT.begin(WIFI_SSID, WIFI_PASS, MQTT_USER, MQTT_PASS);

    // Hẹn giờ gửi định kỳ mỗi 3000ms
    AIoT.addTimeEvent(3000, sendTelemetryData);
}

void loop()
{
    AIoT.run();
}


#include <Arduino.h>

#define DEBUG_COLOR
#define BUTTON_CONFIG

#include <TZIoT.h>

// ======================================================
// 1. THÔNG TIN KẾT NỐI WIFI
// ======================================================
const char *WIFI_SSID = "MakerSpaceLab_2.4Ghz";
const char *WIFI_PASS = "Maker2025";

// Chân LED báo trạng thái hoặc Relay
#define STATUS_LED_PIN 2

// ======================================================
// 2. NHẬN LỆNH ĐIỀU KHIỂN TỪ WEB UI / AI (MQTT)
// ======================================================
// Bắt sự kiện khi nhận lệnh điều khiển "relay1" hoặc "led"
Virtual_WRITE(relay1)
{
    int state = param.getInt();
    digitalWrite(STATUS_LED_PIN, state ? HIGH : LOW);
    Serial.printf("[MQTT RECV] Relay/LED: %d\n", state);

    // Phản hồi lại trạng thái xác nhận về Topic control
    TZIoT.writeControl("relay1", state);
}

// ======================================================
// 3. ĐỌC NHIỆT ĐỘ CHIP & THÔNG SỐ HỆ THỐNG GỬI LÊN BROKER
// ======================================================
void sendChipTelemetry()
{
    if (!TZIoT.CheckConnect())
    {
        return; // Bỏ qua nếu chưa kết nối WiFi
    }

    // 1. Đọc cảm biến nhiệt độ bên trong chip ESP32 (Đơn vị: °C)
    float chipTemp = temperatureRead();

    // 2. Đọc thêm các thông số hệ thống hữu ích
    uint32_t freeRam = ESP.getFreeHeap();       // Dung lượng RAM còn trống (bytes)
    int8_t wifiRssi = WiFi.RSSI();              // Cường độ sóng WiFi (dBm)
    unsigned long uptimeSec = millis() / 1000;  // Thời gian chạy (giây)

    Serial.println("\n--- [TELEMETRY UPDATE] ---");
    Serial.printf("🌡️ Nhiet do chip ESP32: %.2f *C\n", chipTemp);
    Serial.printf("💾 RAM trong (Free Heap): %u bytes\n", freeRam);
    Serial.printf("📶 Tin hieu WiFi (RSSI): %d dBm\n", wifiRssi);
    Serial.printf("⏱️ Thoi gian hoat dong: %lu s\n", uptimeSec);
    Serial.println("--------------------------");

    // 3. Đóng gói & gửi lên Topic: device/<MAC>/telemetry
    TZIoT.writeTelemetry("chip_temp", chipTemp);
    TZIoT.writeTelemetry("free_ram", (int)freeRam);
    TZIoT.writeTelemetry("wifi_rssi", wifiRssi);
    TZIoT.writeTelemetry("uptime", (int)uptimeSec);
}

// ======================================================
// 4. SETUP & LOOP
// ======================================================
void setup()
{
    Serial.begin(115200);
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);

    // Khởi tạo và kết nối thư viện với WiFi & HiveMQ Broker
    TZIoT.begin(WIFI_SSID, WIFI_PASS);

    // Hẹn giờ tự động đọc và gửi nhiệt độ mỗi 3 giây (3000ms)
    TZIoT.addTimeEvent(3000, sendChipTelemetry);
}

void loop()
{
    // Duy trì toàn bộ hoạt động của thư viện
    TZIoT.run();
}
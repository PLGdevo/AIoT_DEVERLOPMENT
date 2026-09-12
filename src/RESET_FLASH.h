#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <SPIFFS.h>
#include "nvs_flash.h"
#include "esp_wifi.h"

void eraseAllStorage() {
    Serial.println("\n=================================");
    Serial.println("BAT DAU XOA TOAN BO BO NHO ESP32");
    Serial.println("=================================");

    // 1. Xóa toàn bộ cấu hình WiFi lưu trong Flash
    Serial.print("1. Xoa cau hinh WiFi... ");
    WiFi.disconnect(true, true); // (wifioff = true, eraseAP = true)
    esp_wifi_restore();
    Serial.println("[OK]");

    // 2. Xóa và định dạng lại vùng nhớ EEPROM (giả lập)
    Serial.print("2. Xoa EEPROM... ");
    EEPROM.begin(512);
    for (int i = 0; i < 512; i++) {
        EEPROM.write(i, 0xFF);
    }
    EEPROM.commit();
    EEPROM.end();
    Serial.println("[OK]");

    // 3. Format vùng nhớ hệ thống file SPIFFS (nếu có sử dụng)
    Serial.print("3. Format SPIFFS (Flash Filesystem)... ");
    if (SPIFFS.begin(true)) {
        SPIFFS.format();
        SPIFFS.end();
        Serial.println("[OK]");
    } else {
        Serial.println("[SKIPPED / KHONG CO]");
    }

    // 4. Xóa sạch toàn bộ phân vùng NVS (Non-Volatile Storage)
    Serial.print("4. Xoa sach toan bo NVS Flash... ");
    esp_err_t err = nvs_flash_erase();
    if (err == ESP_OK) {
        nvs_flash_init(); // Khởi tạo lại NVS sau khi xóa
        Serial.println("[OK]");
    } else {
        Serial.printf("[FAILED - Error: 0x%X]\n", err);
    }

    Serial.println("=================================");
    Serial.println("DA XOA THANH CONG TOAN BO FLASH!");
    Serial.println("=================================");
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    eraseAllStorage();
}

void loop() {
    // Không làm gì thêm sau khi đã xóa xong
    delay(1000);
}
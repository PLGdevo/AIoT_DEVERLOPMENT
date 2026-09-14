# 🚀 AIoT Library (AIoT_Lib)

<p align="center">
  <b>Framework AIoT & IoT Mã Nguồn Mở Hiệu Năng Cao Cho Các Dòng Vi Điều Khiển ESP32 & Đa Nền Tảng Cloud / AI</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32%20%7C%20ESP32--S2%20%7C%20ESP32--S3%20%7C%20ESP32--C3%20%7C%20ESP32--C6-blue?style=for-the-badge&logo=espressif" alt="ESP32 Chips" />
  <img src="https://img.shields.io/badge/Framework-Arduino%20%7C%20PlatformIO-orange?style=for-the-badge&logo=platformio" alt="Framework" />
  <img src="https://img.shields.io/badge/Security-TLS%2FSSL%20Port%208883-green?style=for-the-badge&logo=letsencrypt" alt="TLS Security" />
  <img src="https://img.shields.io/badge/AI-Generative%20AI%20Ready-purple?style=for-the-badge&logo=google" alt="AI Ready" />
  <img src="https://img.shields.io/badge/License-MIT-brightgreen?style=for-the-badge" alt="License" />
</p>

---

## 📖 1. Giới thiệu (Overview)

**AIoT_Lib** là một framework mã nguồn mở được thiết kế chuyên biệt cho việc xây dựng các nút mạng **AIoT (Artificial Intelligence of Things)** và **IoT công nghiệp**. 

Thư viện đơn giản hóa tối đa quy trình kết nối phần cứng với máy chủ Cloud, cung cấp cơ chế bảo mật **MQTT TLS/SSL**, giao diện cấu hình mạng thông minh **Captive Portal Web UI**, quản lý truyền nhận dữ liệu qua định dạng **JSON chuẩn hóa**, và sẵn sàng kết nối trực tiếp với các mô hình **Generative AI** (như Google Gemini, ChatGPT) để điều khiển và phân tích dữ liệu theo thời gian thực.

### ✨ Ưu điểm cốt lõi:
- 🔌 **Plug & Play (PnP)**: Cấu hình WiFi và MQTT trực quan qua giao diện Web Captive Portal (tự động phát Access Point khi mất kết nối).
- 🔒 **Bảo mật đám mây**: Hỗ trợ kết nối MQTT mã hóa TLS/SSL (mặc định tương thích HiveMQ Cloud, EMQX, AWS IoT Core, Mosquitto,...).
- ⚡ **Non-blocking Architecture**: Không gây nghẽn vòng lặp `loop()`, tích hợp Timer Scheduler thay thế hoàn toàn `delay()`.
- 🧠 **AI & Cloud-Ready**: Cấu trúc dữ liệu JSON 2 chiều phân tách rõ ràng giữa `telemetry` (dữ liệu giám sát) và `control` (lệnh điều khiển).
- 🏷️ **Dynamic Dispatching**: Lập trình hướng sự kiện linh hoạt với Macro `Virtual_WRITE(key)` hoặc chân ảo `WRITE(V_pin)`.

---

## 💻 2. Hỗ trợ phần cứng & Nền tảng

### 🔹 Vi điều khiển (Hardware Support)
Thư viện hỗ trợ toàn diện hệ sinh thái chip **Espressif (ESP32 Series)**:
- **ESP32** (Dual-Core, Solo-1, WROOM, WROVER)
- **ESP32-S2 / ESP32-S3** (Hỗ trợ tăng tốc AI Vector, USB OTG, Camera, NVS Flash dung lượng lớn)
- **ESP32-C3 / ESP32-C6** (Kiến trúc RISC-V thế hệ mới, hỗ trợ WiFi 6 & Bluetooth 5)

### 🔹 Giao thức & Ngoại vi mở rộng
- **WiFi 802.11 b/g/n & SoftAP**: Hỗ trợ bộ nhớ NVS lưu trữ đa điểm mạng (lưu tối đa 3 WiFi gần nhất).
- **MQTT qua TLS/SSL (Port 8883)**: Kèm cơ chế tự động kết nối lại (Auto Reconnect & Failover).
- **Modbus RTU / RS485**: Tích hợp sẵn module truyền thông công nghiệp Modbus qua UART.

### 🔹 Nền tảng phát triển & Máy chủ
- **IDE / Toolchain**: PlatformIO (khuyên dùng), Arduino IDE 2.x, ESP-IDF (Arduino Core).
- **Cloud / Broker**: HiveMQ Cloud, EMQX, Mosquitto, ThingsBoard, Adafruit IO, Node-RED, FastAPI Backend.

---

## 🏗️ 3. Kiến trúc hệ thống & Định dạng Topic

Thư viện chuẩn hóa luồng giao tiếp dữ liệu theo địa chỉ phần cứng (MAC Address) của từng thiết bị:

```text
MQTT Base Topic: device/<MAC_ADDRESS>/...
```

| Loại Topic | Hướng truyền | Đường dẫn Topic | Mô tả |
| :--- | :---: | :--- | :--- |
| **Telemetry** | Thiết bị $\rightarrow$ Cloud | `device/<MAC>/telemetry` | Đẩy dữ liệu cảm biến, trạng thái RAM, RSSI, nhiệt độ định kỳ. |
| **Control** | Cloud $\rightarrow$ Thiết bị | `device/<MAC>/control` | Nhận lệnh điều khiển từ Web, App di động hoặc AI Agent. |
| **Control Ack** | Thiết bị $\rightarrow$ Cloud | `device/<MAC>/control` | Phản hồi xác nhận trạng thái thực tế sau khi thực thi lệnh. |

### Cấu trúc Payload JSON mẫu:
```json
// Telemetry Payload
{
  "mac_address": "EC:DA:3B:54:F1:20",
  "data": {
    "chip_temp": 42.5,
    "humidity": 68.2,
    "free_ram": 215040,
    "wifi_rssi": -55
  }
}
```

---

## 📦 4. Hướng dẫn cài đặt (Installation)

### Dành cho PlatformIO
Thêm thư viện vào dự án trong file `platformio.ini`:

```ini
[env:esp32-s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
upload_speed = 921600
board_build.partitions = min_spiffs.csv
lib_deps = 
    https://github.com/ThangNguyen2106-dash/AIoT_DEVERLOPMENT.git
build_flags = 
    -Os
    -ffunction-sections
    -fdata-sections
    -Wl,--gc-sections
    -D DEBUG_COLOR
    -D DEBUG
```

### Dành cho Arduino IDE
1. Tải repository này về dưới dạng file `.zip`.
2. Mở Arduino IDE $\rightarrow$ **Sketch** $\rightarrow$ **Include Library** $\rightarrow$ **Add .ZIP Library...**
3. Thêm `#include <AIoT.h>` vào đầu chương trình của bạn.

---

## 📚 5. Bảng tra cứu Cú pháp & API (API Reference)

### 1️⃣ Khởi tạo & Quản lý vòng lặp

```cpp
#include <AIoT.h>
```

| Cú pháp API | Chức năng |
| :--- | :--- |
| `AIoT.begin(ssid, pass);` | Khởi tạo với WiFi xác định và tài khoản MQTT mặc định. |
| `AIoT.begin(ssid, pass, mqtt_user, mqtt_pass);` | Khởi tạo với thông tin WiFi và tài khoản MQTT tùy chỉnh. |
| `AIoT.begin("", "");` | Khởi chạy chế độ Web Captive Portal (nếu chưa có WiFi lưu trong Flash). |
| `AIoT.run();` | Hàm duy trì toàn bộ tác vụ mạng, MQTT, DNS, Web và Timer. Đặt trong `loop()`. |
| `AIoT.CheckConnect();` | Trả về `true` nếu thiết bị đã kết nối thành công tới mạng WiFi. |

---

### 2️⃣ Gửi dữ liệu lên Cloud (Telemetry & Control)

```cpp
// Gửi dữ liệu giám sát định kỳ (Telemetry)
AIoT.writeTelemetry(const char *key, const Param value);

// Gửi trạng thái xác nhận điều khiển (Control Acknowledgment)
AIoT.writeControl(const char *key, const Param value);
```

*Hỗ trợ đa dạng kiểu dữ liệu: `int`, `float`, `double`, `bool`, `const char*`, `String`.*

---

### 3️⃣ Nhận lệnh điều khiển (Event Dispatching)

#### Cách 1: Đăng ký theo tên biến / Khóa JSON (Khuyên dùng)
```cpp
Virtual_WRITE(ten_khoa)
{
    // Lấy giá trị qua đối tượng param
    int val_int       = param.getInt();
    float val_float   = param.getFloat();
    bool val_bool     = param.getBool();
    String val_string = param.getString();
}
```

#### Cách 2: Đăng ký theo Virtual Pin (V0 - V49 / V99)
```cpp
WRITE(V1)
{
    int state = param.getInt();
    digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
}
```

---

### 4️⃣ Đối tượng dữ liệu `Param`

Đối tượng `param` tự động ép kiểu thông minh từ JSON nhận được:
- `param.getInt()`: Lấy giá trị nguyên (Integer).
- `param.getFloat()`: Lấy giá trị số thực (Float).
- `param.getDouble()`: Lấy số thực độ chính xác kép (Double).
- `param.getBool()`: Lấy giá trị logic (`true`/`false`, `1`/`0`).
- `param.getString()`: Lấy chuỗi ký tự (String).
- `param.getType()`: Trả về kiểu dữ liệu gốc (`Param::Type::INT`, `FLOAT`, `BOOL`, `STRING`,...).

---

### 5️⃣ Hẹn giờ không chặn (Non-blocking Timer Scheduler)

Thay vì dùng hàm `delay()` làm đơ hệ thống, sử dụng cơ chế Timer của thư viện:
```cpp
// Đăng ký gọi hàm định kỳ sau mỗi số mili-giây (ms)
AIoT.addTimeEvent(3000, sendDataCallback);
```

---

## 🛠️ 6. Các ví dụ mẫu (Practical Examples)

### 📌 Ví dụ 1: Gửi dữ liệu cảm biến & Thông số Chip (Telemetry Stream)

```cpp
#include <Arduino.h>
#define DEBUG_COLOR
#include <AIoT.h>

const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASS = "YOUR_WIFI_PASSWORD";

void sendSensorData()
{
    if (!AIoT.CheckConnect()) return;

    // Đọc nhiệt độ nội tại của chip ESP32 và dung lượng RAM trống
    float chipTemp = temperatureRead();
    uint32_t freeRam = ESP.getFreeHeap();
    int wifiRssi = WiFi.RSSI();

    Serial.printf("[TELEMETRY] Temp: %.2f *C | RAM: %u B | RSSI: %d dBm\n", chipTemp, freeRam, wifiRssi);
    
    // Đẩy dữ liệu lên Cloud: device/<MAC>/telemetry
    AIoT.writeTelemetry("chip_temp", chipTemp);
    AIoT.writeTelemetry("free_ram", (int)freeRam);
    AIoT.writeTelemetry("wifi_rssi", wifiRssi);
}

void setup()
{
    Serial.begin(115200);

    // Khởi tạo WiFi & kết nối MQTT TLS
    AIoT.begin(WIFI_SSID, WIFI_PASS);

    // Lập lịch gửi dữ liệu tự động mỗi 3 giây (3000ms)
    AIoT.addTimeEvent(3000, sendSensorData);
}

void loop()
{
    // Bắt buộc gọi trong loop để duy trì kết nối và nhận sự kiện
    AIoT.run();
}
```

---

### 📌 Ví dụ 2: Điều khiển thiết bị 2 chiều (Bidirectional Control)

```cpp
#include <Arduino.h>
#define DEBUG_COLOR
#include <AIoT.h>

const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASS = "YOUR_WIFI_PASSWORD";

#define RELAY_PIN 2

// Bắt sự kiện khi Cloud/Web/AI gửi lệnh: {"data": {"relay1": 1}}
Virtual_WRITE(relay1)
{
    int state = param.getInt();
    digitalWrite(RELAY_PIN, state ? HIGH : LOW);
    
    Serial.printf("[CONTROL] Relay 1 changed state to: %d\n", state);

    // Xác nhận lại trạng thái với Server để đồng bộ giao diện người dùng
    AIoT.writeControl("relay1", state);
}

void setup()
{
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    AIoT.begin(WIFI_SSID, WIFI_PASS);
}

void loop()
{
    AIoT.run();
}
```

---

### 📌 Ví dụ 3: Cấu hình WiFi & MQTT qua Web Captive Portal (Smart AP)

Khi chuyển thiết bị đến môi trường mạng mới, thiết bị sẽ tự động phát Access Point để người dùng dùng điện thoại kết nối và cài đặt:

```cpp
#include <Arduino.h>
#define DEBUG_COLOR
#define BUTTON_CONFIG // Bật tính năng cấu hình AP
#include <AIoT.h>

void setup()
{
    Serial.begin(115200);

    // Để trống SSID & Pass để ưu tiên đọc từ Flash NVS.
    // Nếu chưa có, thiết bị sẽ phát AP: "AIoT: <MAC>" (IP: 192.168.21.6)
    AIoT.begin("", "");
}

void loop()
{
    AIoT.run();
}
```

---

### 📌 Ví dụ 4: Sẵn sàng tích hợp Generative AI (AI Agent / Gemini)

Dữ liệu đẩy lên broker dưới dạng JSON tiêu chuẩn giúp các dịch vụ AI Agent dễ dàng đọc và ra quyết định:

```text
[Cảm biến nhiệt độ gửi lên]
--> device/EC:DA:3B:54:F1:20/telemetry: {"data": {"temperature": 39.8}}

[AI Agent phân tích ngữ cảnh và quyết định bật quạt làm mát]
--> device/EC:DA:3B:54:F1:20/control: {"data": {"fan_speed": 100, "alert": "HIGH_TEMP"}}

[ESP32 bắt sự kiện và thực thi tức thì]
Virtual_WRITE(fan_speed) {
    int speed = param.getInt();
    analogWrite(FAN_PWM_PIN, speed);
}
```

---

## ⚙️ 7. Các cờ tiền xử lý (Pre-processor Flags)

| Flag | Mô tả |
| :--- | :--- |
| `#define DEBUG` | Bật xuất log chẩn đoán hệ thống qua cổng Serial. |
| `#define DEBUG_COLOR` | Bật xuất log Serial có màu sắc trực quan (ANSI color codes). |
| `#define BUTTON_CONFIG` | Bật xử lý nút bấm vật lý để chuyển đổi chế độ cấu hình Captive Portal. |
| `#define Virtual_100_PINS` | Mở rộng số lượng chân ảo `WRITE(V_pin)` từ 50 lên 100 chân. |

---

## 👨‍💻 Tác giả & Đóng góp (Author & License)

- **Tác giả:** ThangNguyen2106
- **Email:** `0309231068@caothang.edu.vn`
- **GitHub:** [ThangNguyen2106-dash](https://github.com/ThangNguyen2106-dash)
- **Repository:** [AIoT_DEVERLOPMENT](https://github.com/ThangNguyen2106-dash/AIoT_DEVERLOPMENT)

Phát hành dưới giấy phép mã nguồn mở **MIT License**. Mọi đóng góp, báo lỗi (Issue) và Pull Request đều được chào đón!
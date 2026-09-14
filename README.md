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
    
    // Cập nhật các trường và gửi 1 gói JSON duy nhất
    AIoT.updateTelemetry("chip_temp", chipTemp);
    AIoT.updateTelemetry("free_ram", (int)freeRam);
    AIoT.updateTelemetry("wifi_rssi", wifiRssi);
    AIoT.sendTelemetry();
}

void setup()
{
    Serial.begin(115200);

    // Khởi tạo WiFi & kết nối MQTT TLS
    AIoT.begin(WIFI_SSID, WIFI_PASS, "IoT_TEST", "mt21062005");

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

### 📌 Ví dụ 2: Phát hiện bất thường bằng Edge AI (Anomaly Detection)

```cpp
#include <Arduino.h>
#define BOARD_ESP32_S3_KIT
#include <AIoT.h>
#include <EdgeAI/EdgeAI.h>

EdgeAI::Engine edgeAI;

void setup()
{
    Serial.begin(115200);
    AIoT_Device.begin();
    
    // Tự học đường cơ sở (Baseline Calibration) với 30 mẫu ban đầu
    edgeAI.begin(3.0f, 30);
    AIoT.begin("WiFi_SSID", "WiFi_PASS", "IoT_TEST", "mt21062005");
}

void loop()
{
    AIoT.run();

    float sensorSample = AIoT_Device.readVoltage(1) * 10.0f;
    EdgeAI::InferenceResult res = edgeAI.process(sensorSample);

    // Phản xạ tức thì tại chỗ (< 1ms) không cần Internet
    if (res.isEmergency) {
        AIoT_Device.relay(1, LOW); // Ngắt tải khẩn cấp
        AIoT_Device.beep(100);
    }

    AIoT.updateTelemetry("anomaly_score", res.score);
    AIoT.updateTelemetry("state", res.label);
    AIoT.sendTelemetry();

    delay(100);
}
```

---

### 📌 Ví dụ 3: Điện toán & Học thuật Toán học AI (`AI_Math`)

```cpp
#include <Arduino.h>
#include <AI_Math/AI_Math.h>

void setup()
{
    Serial.begin(115200);

    // 1. Thuật toán Welford học phân phối chuẩn trực tuyến O(1) bộ nhớ
    AI_Math::WelfordEstimator welford;
    welford.update(10.5);
    welford.update(11.2);
    welford.update(10.8);
    Serial.printf("Mean: %.2f | StdDev: %.2f\n", welford.getMean(), welford.getStdDev());

    // 2. Thuật toán k-NN tự học và phân loại mẫu trên chip
    AI_Math::OnlineKNN<2, 10, 3> knn;
    float normal[] = {1.0, 25.0};
    float fault[] = {8.5, 75.0};
    knn.addSample(normal, 0); // Lớp 0: Bình thường
    knn.addSample(fault, 1);  // Lớp 1: Sự cố

    float query[] = {8.7, 78.0};
    int predicted = knn.predict(query);
    Serial.printf("Predicted Class: %d\n", predicted);
}

void loop() {}
```

---

### 📌 Ví dụ 4: Toàn diện Hybrid AIoT (Edge AI + HiveMQ Cloud + Gemini Agent)

```cpp
#include <Arduino.h>
#define BOARD_AIOT_INDUSTRIAL
#include <AIoT.h>
#include <HybridAI/HybridAI.h>

HybridAIEngine hybridAI;

// Lắng nghe lệnh điều khiển sâu từ Gemini Cloud Agent
Virtual_WRITE(relay1)
{
    int state = param.getInt();
    AIoT_Device.relay(1, state ? HIGH : LOW);
    AIoT.writeControl("relay1", state);
}

void setup()
{
    Serial.begin(115200);
    AIoT_Device.begin();
    hybridAI.begin(3.0f, 50);
    AIoT.begin("WiFi_SSID", "WiFi_PASS", "IoT_TEST", "mt21062005");
}

void loop()
{
    AIoT.run();

    float sensorSample = AIoT_Device.readVoltage(1) * 10.0f;

    // Edge AI tự động ngắt Relay 1 nếu phát hiện rung giật/nhiệt độ nguy cấp
    EdgeAI::InferenceResult res = hybridAI.process(sensorSample, 1);

    AIoT.updateTelemetry("score", res.score);
    AIoT.updateTelemetry("status", res.label);
    AIoT.sendTelemetry();

    delay(100);
}
```

---

## 🧠 7. Các Phân Hệ Mở Rộng Dành Cho Developers

| Module | Đường dẫn | Mục đích |
| :--- | :--- | :--- |
| **`AI_Math`** | `src/AI_Math/` | Cung cấp nền tảng toán học máy học: Ma trận, Thống kê, DSP (FFT, Windowing), Hàm kích hoạt (ReLU/Softmax), Thuật toán tự học trực tuyến (Welford, Online k-NN, Online K-Means). |
| **`EdgeAI`** | `src/EdgeAI/` | Bộ máy suy luận TinyML tại biên: Anomaly Detection, State Classifier, nạp model C-array. |
| **`CloudAI`** | `src/CloudAI/` | Chuẩn hóa kết nối Google Gemini API, OpenAI, Prompt Templates tự động, giao thức Agent 2 chiều. |
| **`HybridAI`** | `src/HybridAI/` | Tích hợp đa tầng: Phản xạ Edge tức thì (< 1ms) + Đồng bộ insight lên Cloud LLM suy luận sâu. |
| **`Device`** | `src/Device/` | Trừu tượng hóa phần cứng (HAL), quản lý Actuators (Relay, PWM, Buzzer), Sensors, và Board Profiles định sẵn. |

---

## ⚙️ 8. Các cờ tiền xử lý (Pre-processor Flags)

| Flag | Mô tả |
| :--- | :--- |
| `#define BOARD_ESP32_S3_KIT` | Chọn sơ đồ chân cho kit phát triển ESP32-S3. |
| `#define BOARD_AIOT_INDUSTRIAL` | Chọn sơ đồ chân cho kit công nghiệp (Modbus RS485 + Relay Opto). |
| `#define BOARD_ESP32_CAM` | Chọn sơ đồ chân cho kit ESP32-CAM / ESP32-S3-EYE. |
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
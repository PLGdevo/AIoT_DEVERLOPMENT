# ARCHITECTURE.md

Kiến trúc hệ thống `AIoT_Lib`. Đọc file này khi làm việc liên quan tới thiết kế module, luồng dữ liệu giữa các tầng, hoặc khi thêm/sửa ranh giới giữa các phân hệ. Không cần đọc cho việc sửa lỗi cục bộ trong một file.

## Tổng quan

Thư viện Arduino/PlatformIO cho ESP32, domain-agnostic (không gán cứng loại cảm biến/bài toán vào core). 5 phân hệ chính + 1 tầng điều phối:

```
Sensors (bất kỳ) --> Device (HAL) --> EdgeAI (inference biên)
                                            |
                                            v
                                    HybridAI Orchestrator --> Middleware --> Device (actuator)
                                            ^
                                            |
                                       CloudAI (Gemini)
```

## Module map (`src/`)

| Module | Trách nhiệm | File chính |
|---|---|---|
| `AI_Math/` | Toán nền: Matrix, Statistics, DSP, Activations, OnlineLearning | `Matrix.hpp`, `Statistics.hpp`, `DSP.hpp` |
| `Device/` | HAL: Device/Sensor/Actuator trừu tượng + pin map theo board | `Device.hpp`, `Profiles/*.h` |
| `EdgeAI/` | Suy luận biên model-agnostic: sliding window, AnomalyDetector, Classifier | `EdgeAI.hpp`, `AnomalyDetector.hpp`, `Classifier.hpp` |
| `CloudAI/` | Gọi Gemini qua HTTPS REST, provider abstraction, prompt template | `GeminiClient.hpp`, `Provider.hpp` |
| `HybridAI/` | Điều phối Edge/Cloud, policy, quyết định — v2, mới thêm | `AIOrchestrator.hpp`, `PolicyEngine.hpp`, `Types.hpp` |
| `Middleware/` | Boundary tích hợp ROS2/MQTT/HTTP/Serial/Modbus | `Middleware.hpp`, `ROS2/` |
| `IoT/` | Dispatch dữ liệu theo key, parse Param, giao thức nội bộ | `Handler.cpp`, `API.hpp`, `Protocol.hpp` |
| `MQTT/` | Wrapper PubSubClient, kết nối TLS tới broker | `ESP32_MQTT.hpp`, `PubSubClient/` (vendored) |
| `WiFi/` | Provisioning, captive portal cấu hình Wi-Fi/MQTT | `AIoT_PnP_ESP32.hpp`, `CONFIG_UI.h` |
| `Ultility/` | cJSON (vendored), Modbus RTU | `Param/cJSON.cpp`, `Communicate/MODBUS/` |

## Luồng HybridAI (v2)

Tài liệu gốc: `HYBRID_AI_V2_ARCHITECTURE.md`, `src/HybridAI/README.md`.

```
Edge AI ----\
             > AIOrchestrator -> PolicyEngine -> Decision -> Middleware -> Device
Cloud AI ---/
```

Logic `AIOrchestrator::route()` (`src/HybridAI/AIOrchestrator.hpp`):
1. `severity == CRITICAL` → luôn ép về `EDGE` + `LOCAL_ACTION`, bỏ qua policy (an toàn khẩn cấp không phụ thuộc policy).
2. Ngược lại, `PolicyEngine::selectPath()` quyết định `EDGE`/`CLOUD`/`HYBRID` dựa trên `confidence >= cloudThreshold` (mặc định 0.70) và cờ `realtime`.

Interface (`src/HybridAI/Types.hpp`): `AIInput{value, confidence, severity, realtime}` → `AIResult{path, decision, severity, score, emergency, label, reason}`. Pure logic, không phụ thuộc hardware — unit-test được độc lập trên host, không cần ESP32 thật.

Định hướng tương lai (ghi trong `HYBRID_AI_V2_ARCHITECTURE.md`, **chưa triển khai**): tách rõ hơn 4 lớp EdgeAI (inference) / CloudAI (provider abstraction) / HybridAI (routing) / Middleware (transport), với `Device` là HAL riêng.

## Interface chính (API surface)

- `DataHandler::add(key, callback)` / `DataHandler::dispatch(key, param)` — bảng handler tuyến tính **cố định 50 slot** (`src/IoT/Handler.cpp`). Không dynamic alloc.
- Macro X-macro `VPIN_LIST` tự sinh weak handler `WidgetWriteN`, fallback `WidgetWrite_Default`.
- `AIoT_MQTT_ESP32<PubSubClient>::config()/begin()/run()/PublishData_tele()/PublishData_control()/SubscribeTopic()`.
- `CloudAI::GeminiClient::ask(prompt, systemInstruction) -> String`, tự retry 429 → fallback edge.
- `HybridAI::AIOrchestrator(PolicyConfig).route(AIInput) -> AIResult`.
- `EdgeAI::Engine` — sliding window + AnomalyDetector + Classifier, model-agnostic.

## Dependencies

`platformio.ini` không khai `lib_deps` — PubSubClient và cJSON được **vendor trực tiếp trong `src/`**, không qua package manager. Cập nhật các lib này phải sửa tay.

Cloud dependency duy nhất: Google Gemini API (`generativelanguage.googleapis.com`) qua HTTPS REST thuần, không qua SDK.

`library.json` khai `headers: ["AIoT.h", "TZIoT.h"]` — **`TZIoT.h` chưa xác nhận tồn tại trong `src/`**, kiểm tra nếu build lỗi thiếu header khi dùng như Arduino library chuẩn.

# DECISIONS.md

Quyết định kỹ thuật quan trọng đã chốt cho `AIoT_Lib`. Đọc file này trước khi đề xuất thay đổi lớn về kiến trúc/dependency — tránh đề xuất lại thứ đã được cân nhắc và quyết định có chủ đích. Thêm entry mới khi có quyết định đáng ghi nhớ (không phải mọi commit).

Format mỗi entry: **Quyết định** — **Lý do** — **Phương án khác đã xét** — **Hệ quả**.

---

## D-001: Core domain-agnostic, không gán cứng loại cảm biến/bài toán

- **Quyết định:** EdgeAI/HybridAI core không biết gì về loại cảm biến hay bài toán cụ thể; lập trình viên tự đưa dữ liệu vào.
- **Lý do:** thư viện dùng cho nhiều dự án IoT khác nhau (công nghiệp, học thuật, vision...), gán cứng sẽ giới hạn tái sử dụng.
- **Phương án khác:** giữ code chuyên biệt theo domain trong core (dễ dùng ngay hơn nhưng không tái sử dụng được).
- **Hệ quả:** ví dụ domain cụ thể (motor vibration, v.v.) chỉ nằm trong `examples/`, không trong core. Commit liên quan: `f7284e1`, `ddd3f8c`, `8576037`.

## D-002: Model Gemini mặc định = `gemini-1.5-flash`, không phải bản mới nhất

- **Quyết định:** default model trong `GeminiClient` là `gemini-1.5-flash`, có fallback về Edge khi gặp HTTP 429.
- **Lý do:** quota miễn phí 1500 request/ngày, ổn định hơn cho thiết bị chạy liên tục so với model mới hơn nhưng quota thấp.
- **Phương án khác:** dùng model mới nhất (đã thử ở commit `9aa8bb8`, sau đó revert vì quota).
- **Hệ quả:** README hiện quảng cáo "Gemini 3.6 Flash" — **lệch so với default thực tế**, cần cập nhật (xem TODO.md). Commit: `633ff69`.

## D-003: ROS2 là optional middleware, không phải dependency của HybridAI core

- **Quyết định:** `Middleware/ROS2/` là boundary tích hợp tuỳ chọn; HybridAI core không require ROS2.
- **Lý do:** giữ core nhẹ, không ép người dùng không dùng ROS2 phải kéo theo dependency đó.
- **Phương án khác:** tích hợp ROS2 sâu vào HybridAI (chặt hơn nhưng làm core nặng và kém di động).
- **Hệ quả:** ai cần ROS2 tự include `Middleware/ROS2/ROS2Bridge.hpp`, core vẫn build được không có ROS2.

## D-004: Không dùng PlatformIO `lib_deps` — vendor thư viện phụ trợ trực tiếp trong `src/`

- **Quyết định:** PubSubClient và cJSON được copy trực tiếp vào `src/MQTT/PubSubClient/` và `src/Ultility/Param/`, không khai trong `platformio.ini`.
- **Lý do:** không rõ ràng trong lịch sử — có thể để tránh version drift khi publish như Arduino library, hoặc để tuỳ biến source.
- **Phương án khác:** dùng `lib_deps` chuẩn PlatformIO (dễ update version hơn, nhưng có thể xung đột với cách phân phối Arduino Library Manager).
- **Hệ quả:** cập nhật PubSubClient/cJSON phải sửa tay, không có version pin qua package manager; các FIXME/TODO gốc của cJSON vẫn còn nguyên trong code (không phải bug tự viết).

## D-005: Handler dispatch dùng mảng tuyến tính cố định 50 phần tử

- **Quyết định:** `DataHandler::handlers[50]`, tìm kiếm tuyến tính bằng `strcmp`, không dynamic allocation.
- **Lý do:** an toàn bộ nhớ cho embedded, tránh phân mảnh heap từ cấp phát động lặp lại.
- **Phương án khác:** map/hash động (nhanh hơn khi nhiều key, nhưng tốn RAM và có rủi ro fragmentation trên ESP32).
- **Hệ quả:** giới hạn cứng 50 key; nếu vượt quá, `add()` âm thầm bỏ qua (không log lỗi) — cần lưu ý khi debug nếu handler "không chạy".

## D-006: `.claude/` skill pack merge vào `main` của repo gốc qua PR, không phải nhánh riêng

- **Quyết định:** bộ 12 skill ESP32 (init → hardware analysis → ... → release) được merge vào `main` của `ThangNguyen2106-dash/AIoT_DEVERLOPMENT` qua PR #5, không giữ riêng trên fork.
- **Lý do:** dùng chung cho cả team, không chỉ cá nhân.
- **Phương án khác:** giữ trên nhánh `PLG` cá nhân (chỉ mình dùng được).
- **Hệ quả:** `.claude/` không phải code chạy trên thiết bị — không ảnh hưởng build firmware, chỉ là hướng dẫn quy trình cho Claude Code.

## D-007: MQTT dùng `setInsecure()` để tiết kiệm RAM (⚠ cần xem lại)

- **Quyết định:** `WiFiClientSecure::setInsecure()` được gọi ở cả MQTT (`ESP32_MQTT.hpp:178`) và Gemini client (`GeminiClient.hpp:55`), bỏ qua xác thực chứng chỉ CA.
- **Lý do (ghi trong comment gốc):** "nhẹ RAM" — lưu trữ CA cert tốn RAM trên ESP32.
- **Phương án khác:** `setCACert()` với chứng chỉ cụ thể (an toàn hơn, tốn thêm RAM/flash để lưu cert).
- **Hệ quả:** **rủi ro bảo mật MITM thật** — xem `HARDWARE.md`/review bảo mật. Đây là quyết định cũ, chưa được đánh giá lại với yêu cầu bảo mật hiện tại. Xem TODO.md để re-evaluate.

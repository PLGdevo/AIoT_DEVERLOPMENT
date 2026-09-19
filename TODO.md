# TODO.md

Danh sách việc cần làm. Cập nhật bằng `/checkpoint` hoặc `/end-session`. Header section giữ nguyên tiếng Anh để script hook parse ổn định — nội dung bên dưới có thể tiếng Việt.

Không dùng file này để chép lại toàn bộ lịch sử — chỉ giữ việc **chưa xong**. Việc đã xong chuyển ghi chú ngắn vào `SESSION_SUMMARY.md` của phiên hoàn thành nó rồi xoá khỏi đây.

## In Progress

*(trống — chưa có việc nào đang dang dở giữa phiên)*

## Pending

- [ ] **[Bảo mật - cao]** Xoay vòng mật khẩu broker HiveMQ Cloud đang lộ (`src/MQTT/ESP32_MQTT.hpp:34-37`), gỡ hardcode, chuyển sang `SECRETS.example`/NVS/captive portal config.
- [ ] **[Bảo mật - cao]** Xem lại `setInsecure()` ở MQTT (`ESP32_MQTT.hpp:178`) và Gemini client (`GeminiClient.hpp:55`) — cân nhắc `setCACert()`.
- [ ] **[Hardware]** Xác nhận và sửa xung đột GPIO8 (`PIN_ANALOG_4` vs `PIN_I2C_SDA`) trên `Board_ESP32_S3_Kit.h`.
- [ ] Dọn commit rác `13b8798` (8 dòng comment trùng lặp cuối `src/AIoT.h`).
- [ ] Cập nhật README cho khớp model Gemini thực tế (`gemini-1.5-flash`, không phải "3.6 Flash").
- [ ] Kiểm tra `TZIoT.h` có tồn tại trong `src/` không, hoặc bỏ khỏi `library.json` nếu không còn dùng.
- [ ] Cân nhắc tách rõ 4 tầng EdgeAI/CloudAI/HybridAI/Middleware như định hướng trong `HYBRID_AI_V2_ARCHITECTURE.md` (hiện chỉ là đề xuất, chưa làm).
- [ ] Đồng bộ nhánh `PLG` trên upstream với các commit mới nhất nếu còn dùng làm nhánh cá nhân.

## Blocked

*(trống)*

## Done (recent — xoá khi SESSION_SUMMARY tương ứng đã ổn định)

- [x] Merge `.claude/` ESP32 skill pack vào `main` của repo gốc qua PR #5.
- [x] Thiết lập hệ thống context-management (CLAUDE.md, PROJECT_STATE.md, ARCHITECTURE.md, HARDWARE.md, DECISIONS.md, TODO.md, SESSION_SUMMARY.md + skill + hooks + slash commands).

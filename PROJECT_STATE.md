# PROJECT_STATE.md

Trạng thái **hiện tại** của project. File này + `CLAUDE.md` là 2 file luôn được đọc đầu mỗi phiên. Giữ file này NGẮN — chi tiết kiến trúc/hardware/quyết định/việc-cần-làm nằm ở các file riêng (xem bảng điều hướng trong `CLAUDE.md`). Cập nhật bằng `/checkpoint` hoặc `/end-session`, không sửa tay ngoài quy trình đó trừ khi cần thiết.

Last updated: 2026-09-17

## Snapshot

- **Project:** `AIoT_Lib` — thư viện Arduino/PlatformIO cho ESP32/ESP32-S3, framework AIoT domain-agnostic (Edge AI + Cloud AI qua HybridAI orchestrator).
- **Git:** repo `AIoT_DEVERLOPMENT`. `origin` = fork cá nhân (`PLGdevo`), `upstream` = repo gốc (`ThangNguyen2106-dash`, user có quyền push trực tiếp). Nhánh làm việc cá nhân trên upstream: `PLG`. Quy trình fork/PR: `docs/GIT_WORKFLOW.md`.
- **Build:** PlatformIO, board `esp32-s3-devkitc-1`, chưa build-test trong phiên gần nhất (chỉ review code tĩnh).

## Completed

- Kiến trúc HybridAI v2 (orchestrator + policy engine + provider abstraction) — đã merge vào `main` upstream.
- Domain-agnostic hoá EdgeAI (decouple khỏi use-case cụ thể).
- `.claude/` ESP32 skill pack (12 skill theo giai đoạn dev) — đã merge vào `main` upstream qua PR #5.
- Hệ thống context-management (file này + CLAUDE.md/ARCHITECTURE.md/HARDWARE.md/DECISIONS.md/TODO.md/SESSION_SUMMARY.md + skill `context-manager` + 4 slash command + hook tự động) — vừa dựng xong, xem `SESSION_SUMMARY.md`.

## In Progress

*(trống — không có việc dang dở giữa phiên tại thời điểm này)*

## Blockers

*(trống — các vấn đề bảo mật/hardware đã biết KHÔNG chặn việc tiếp tục dev, xem TODO.md để xử lý theo ưu tiên)*

## Next Actions

1. Xử lý mục ưu tiên cao trong `TODO.md` (credential MQTT lộ trên GitHub public — xoay vòng mật khẩu HiveMQ + gỡ hardcode).
2. Xem lại `setInsecure()` ở MQTT/Gemini client.
3. Xác nhận xung đột GPIO8 trên `Board_ESP32_S3_Kit.h` với hardware thật.

Chi tiết đầy đủ từng mục: `TODO.md`. Bối cảnh/lý do quyết định: `DECISIONS.md`. Pin mapping: `HARDWARE.md`. Sơ đồ module: `ARCHITECTURE.md`.

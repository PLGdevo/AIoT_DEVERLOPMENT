# CLAUDE.md

Đây là file luôn được nạp tự động mỗi phiên. Giữ ngắn có chủ đích — chi tiết nằm ở các file khác, chỉ đọc khi cần (xem bảng load-tier bên dưới).

## 1. Quy tắc dự án (permanent)

- **Không hardcode credential thật** (API key, mật khẩu MQTT/Wi-Fi, chứng chỉ) vào source hay example. Dùng `SECRETS.example` làm mẫu, đọc từ NVS/runtime config/captive portal.
- **Không tắt xác thực TLS** (`setInsecure()`) khi thêm code mới mà không ghi rõ lý do + đánh dấu trong `DECISIONS.md`. Chỗ đã tồn tại (MQTT, Gemini client) là nợ kỹ thuật đã biết, xem `TODO.md`, không phải mẫu để copy.
- **Không sửa code vendored** (`src/MQTT/PubSubClient/`, `src/Ultility/Param/cJSON.*`) trừ khi thực sự cần — đây là thư viện third-party copy nguyên trạng, TODO/FIXME bên trong là của upstream, không phải việc của project này.
- **Domain-agnostic core:** không thêm logic gán cứng cho một loại cảm biến/bài toán cụ thể vào `EdgeAI/`, `HybridAI/`, `Device/` (core). Code chuyên biệt theo use-case thuộc về `examples/`.
- **Không commit build artifact** (`.pio/`) hay xung đột pin chưa xác nhận — kiểm tra `HARDWARE.md` trước khi thêm board profile mới hoặc đổi pin default.

## 2. Coding conventions

- C++ header-heavy (`.hpp` cho template/inline, `.h` cho pure interface/macro define), namespace theo module (`CloudAI::`, `HybridAI::`, `EdgeAI::`).
- Conventional Commits: `type(scope): mô tả ngắn` — `feat`, `fix`, `docs`, `refactor`, `test`, `chore`. Xem `git log` để tham khảo style đang dùng trong repo.
- Board pin mapping đặt trong `src/Device/Profiles/Board_*.h` dạng `#define PIN_XXX <gpio>`, không rải rác trong code logic.
- Handler/dispatch dùng mảng tuyến tính cố định (xem `DECISIONS.md#D-005`), không tự ý đổi sang dynamic allocation trừ khi có lý do rõ ràng.

## 3. Quy tắc workflow (git)

- `origin` = fork cá nhân, `upstream` = repo gốc (`ThangNguyen2106-dash/AIoT_DEVERLOPMENT`). Chi tiết đầy đủ: `docs/GIT_WORKFLOW.md`.
- Không push thẳng vào `main` chung — qua Pull Request. Nhánh cá nhân (`PLG`) thì push thẳng được.
- Không `--force` lên nhánh chung; chỉ `--force-with-lease` trên nhánh riêng của mình.

## 4. Quy tắc quản lý context (đọc kỹ — áp dụng mọi phiên)

Mục tiêu: một phiên Claude Code mới phải làm việc tiếp được **chỉ từ các file trong repo**, không cần lịch sử hội thoại trước. Ngược lại, **không đọc mọi file tài liệu trong mọi task** — tốn context vô ích.

### Load tier — file nào đọc khi nào

| Tier | File | Khi nào đọc |
|---|---|---|
| **ALWAYS** | `CLAUDE.md` (tự động), `PROJECT_STATE.md` | Mọi phiên, ngay từ đầu. `SessionStart` hook đã bơm sẵn một bản tóm tắt git-state + next action vào context — nếu bản tóm tắt đó đủ cho task, **không cần mở lại `PROJECT_STATE.md` đầy đủ**. |
| **LOAD WHEN RELEVANT** | `ARCHITECTURE.md` | Task liên quan thiết kế module, luồng dữ liệu, ranh giới giữa các phân hệ. |
| | `HARDWARE.md` | Task liên quan pin mapping, board profile, driver ngoại vi, GPIO. |
| | `DECISIONS.md` | Trước khi đề xuất đổi kiến trúc/dependency lớn — tránh đề xuất lại thứ đã cân nhắc. |
| **TASK-SPECIFIC** | `TODO.md` | Khi lập kế hoạch việc tiếp theo, hoặc khi chạy `/checkpoint`/`/end-session`. |
| | `SESSION_SUMMARY.md` | Đầu phiên, chỉ khi cần chi tiết phiên trước ngoài phần tóm tắt SessionStart đã đưa. |
| | `docs/GIT_WORKFLOW.md` | Khi thao tác git phức tạp (rebase, PR, xung đột remote). |

**Không** đọc `ARCHITECTURE.md`/`HARDWARE.md`/`DECISIONS.md`/`TODO.md` một cách mặc định ở đầu mọi phiên — chỉ đọc khi task thực sự chạm tới nội dung đó.

### Nguồn sự thật ưu tiên

Khi cập nhật các file trạng thái, luôn ưu tiên theo thứ tự: **source code hiện tại > `git status`/`git diff` > kết quả build/test > file cấu hình** — hơn là giả định đã nói trong hội thoại. Hội thoại có thể sai hoặc lỗi thời; code và git state thì không.

### 4 lệnh quản lý phiên

| Lệnh | Khi dùng |
|---|---|
| `/start-session` | Đầu phiên mới — nạp context tối thiểu, xem git state, xác định task hiện tại. |
| `/checkpoint` | Giữa phiên, sau một mốc công việc — cập nhật `PROJECT_STATE.md`/`TODO.md`/`DECISIONS.md` (nếu cần), KHÔNG tạo tài liệu mới ngoài kế hoạch. |
| `/end-session` | Cuối phiên — soát `git diff`/`git status`, viết `SESSION_SUMMARY.md`, cập nhật `PROJECT_STATE.md`/`TODO.md`. |
| `/project-status` | Bất kỳ lúc nào — xem nhanh trạng thái/blocker/next action, không sửa file. |

Cả 4 lệnh đều gọi skill `context-manager` (`.claude/skills/context-manager/SKILL.md`) — đó là nơi chứa quy tắc chi tiết về cách trích xuất và ghi thông tin. **Không copy nguyên văn hội thoại vào bất kỳ file trạng thái nào** — chỉ trích thông tin hữu ích cho việc tiếp theo.

### Giới hạn tự động hoá (thành thật, không phóng đại)

Hook tự động chỉ làm được phần **quan sát/log nhẹ** (git diff/status, snapshot trước khi nén context) — không thể tự viết `SESSION_SUMMARY.md` hay cập nhật `DECISIONS.md` một cách có ý nghĩa, vì việc đó cần suy luận của model. Vì vậy **`SessionEnd` hook chỉ log**, không tự sinh tài liệu — luôn chủ động chạy `/end-session` trước khi kết thúc phiên nếu muốn context được chốt đầy đủ cho phiên sau.

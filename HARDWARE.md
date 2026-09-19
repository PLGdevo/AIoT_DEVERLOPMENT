# HARDWARE.md

Giả định phần cứng của `AIoT_Lib`. Đọc file này khi làm việc liên quan tới pin mapping, board profile, hoặc driver ngoại vi. Không cần đọc cho việc thuần logic phần mềm (HybridAI, parsing...).

## MCU mục tiêu

- **Chính:** ESP32-S3 (`esp32-s3-devkitc-1`, khai trong `platformio.ini`).
- **Hỗ trợ qua board profile:** ESP32 DevKit chuẩn, ESP32-CAM, ESP32-S3 Kit, AIoT Industrial Controller (custom board).
- Framework: Arduino (qua PlatformIO). Partition: `min_spiffs.csv`.
- Build flags: `-Os -ffunction-sections -fdata-sections -Wl,--gc-sections` (tối ưu kích thước binary, cắt code không dùng).

## Board profiles (`src/Device/Profiles/`)

| Board | File | Đặc điểm |
|---|---|---|
| ESP32 DevKit chuẩn | `Board_Default_ESP32.h` | Relay 23/19/18/5, I2C SDA=21/SCL=22, RS485 RX=16/TX=17/DE_RE=4, Analog 34/35/32/33 |
| AIoT Industrial Controller | `Board_AIoT_Industrial.h` | Relay opto-isolated 26/27/14/12, DI opto 36/39/34/35, Analog 0-10V/4-20mA 32/33, RS485 16/17/4 |
| ESP32-CAM AI Vision | `Board_ESP32_CAM.h` | Camera OV2640 full pinout (PWDN=32, XCLK=0, SIOD=26, SIOC=27, Y2..Y9, VSYNC=25, HREF=23, PCLK=22), Relay 12/13, Flash LED=4 |
| ESP32-S3 AIoT DevKit | `Board_ESP32_S3_Kit.h` | RGB LED (WS2812) pin=48, Relay 4/5/6/7, I2S mic SCK=41/WS=42/SD=40, RS485 RX=17/TX=18/DE_RE=16 |

## ⚠ Xung đột pin đã biết (chưa sửa)

`Board_ESP32_S3_Kit.h`: `PIN_ANALOG_4 = 8` và `PIN_I2C_SDA = 8` **dùng trùng GPIO8**. Cần xác nhận với hardware thật trước khi build cho use-case dùng cả I2C và ADC4 đồng thời trên board này. Xem thêm `DECISIONS.md` / `TODO.md`.

## Ngoại vi và giao thức

- **I2C:** SDA/SCL khai trong từng board profile (không đồng nhất giữa các board — ESP32-S3 Kit dùng 8/9, board chuẩn dùng 21/22).
- **RS485 Modbus RTU:** mọi board (trừ CAM) đều có RX/TX/DE_RE riêng — dùng cho công nghiệp (`Ultility/Communicate/MODBUS/modbus.h`).
- **I2S:** chỉ ESP32-S3 Kit có mic pin (SCK/WS/SD) cho voice/sound AI.
- **Camera:** chỉ ESP32-CAM, full pinout OV2640 riêng biệt, không tái dùng khung GPIO của board khác.
- **RGB LED (WS2812):** chỉ ESP32-S3 Kit, driver dùng RMT routing động, hỗ trợ dual-pin (theo `745f408`, chưa audit sâu logic).
- **Relay:** mọi board đều có, số lượng và pin khác nhau; board Industrial dùng opto-isolated (an toàn công nghiệp cao hơn).
- **Analog input:** board Industrial hỗ trợ dải công nghiệp 0-10V / 4-20mA (không phải ADC thô 0-3.3V như board thường).

## Giả định vận hành

- Toàn bộ board profile giả định nguồn 3.3V logic chuẩn ESP32; không có ghi chú riêng về mức điện áp cách ly ngoài optocoupler của board Industrial.
- Không có tài liệu về pull-up/pull-down ngoại vi ngoài mặc định ESP32 (strapping pins GPIO0 dùng cho nút BUTTON_USER/BUTTON_CONFIG — cần cẩn thận vì GPIO0 là strapping pin boot mode).
- Chưa có ghi chú về dòng tiêu thụ / ngân sách pin — dự án hiện có vẻ nhắm tới thiết bị cắm nguồn liên tục (industrial/IoT), không phải battery-powered.

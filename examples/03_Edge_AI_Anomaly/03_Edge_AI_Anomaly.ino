/**
 * ============================================================================
 * AIoT_LIB - Ví Dụ 03: Phát Hiện Bất Thường Bằng Edge AI (Anomaly Detection)
 * ============================================================================
 * Mô tả:
 * - Tự động học đường cơ sở (Baseline Calibration) từ cảm biến
 * - Phát hiện rung giật / quá nhiệt / bất thường tức thời trong < 1ms
 * - Phản xạ bảo vệ phần cứng tại chỗ (cắt Relay, bật Buzzer) mà không cần mạng
 */

#include <Arduino.h>
#define BOARD_ESP32_S3_KIT
#include <Device/Device.h>
#include <EdgeAI/EdgeAI.h>

EdgeAI::Engine edgeAI;

// Giả lập đọc cảm biến rung động hoặc nhiệt độ
float readVibrationSensor()
{
    // Mô phỏng tín hiệu bình thường quanh 10.0 kèm nhiễu trắng
    static unsigned long cycle = 0;
    cycle++;

    float noise = (float)(random(-10, 10)) / 10.0f;
    float value = 10.0f + noise;

    // Giả lập tạo lỗi bất thường mỗi 20 chu kỳ
    if (cycle % 25 == 0)
    {
        value = 28.5f; // Đột biến rung động mạnh
    }
    return value;
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // Khởi tạo phần cứng (Relay, LED, Buzzer)
    AIoT_Device.begin();

    // Khởi tạo Edge AI: Ngưỡng 3.0 sigma, 30 mẫu ban đầu để tự cân chỉnh baseline
    edgeAI.begin(3.0f, 30);

    Serial.println("==========================================");
    Serial.println("   AIoT_LIB: Edge AI Anomaly Detection");
    Serial.println("==========================================");
    Serial.println("Phase 1: Calibrating baseline (30 samples)...");
}

void loop()
{
    float sensorSample = readVibrationSensor();

    // Chạy suy luận Edge AI tại chỗ
    EdgeAI::InferenceResult result = edgeAI.process(sensorSample);

    if (edgeAI.getDetector().isCalibrated())
    {
        Serial.printf("Val: %.2f | Anomaly Score: %.2f | State: %s\n",
                      sensorSample, result.score, result.label);

        // Phản xạ tức thời bảo vệ phần cứng
        if (result.isEmergency)
        {
            Serial.println("🚨 [EMERGENCY REFLEX] Ngắt Relay 1 & Bật còi cảnh báo!");
            AIoT_Device.relay(1, LOW); // Cắt tải bảo vệ máy
            AIoT_Device.beep(200);
        }
        else
        {
            AIoT_Device.relay(1, HIGH); // Giữ máy hoạt động bình thường
        }
    }
    else
    {
        Serial.printf("Calibrating... [%u/30 samples]\n", edgeAI.getDetector().getSampleCount());
    }

    delay(200);
}

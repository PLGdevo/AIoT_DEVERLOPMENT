/**
 * ============================================================================
 * AIoT_LIB - Ví Dụ 05: Toàn Diện Hybrid AIoT (Edge AI + HiveMQ + Gemini)
 * ============================================================================
 * Mô tả:
 * - Tầng 1 (Edge AI): Xử lý tín hiệu cảm biến tần số cao, tự học và phản xạ khẩn cấp tại chỗ
 * - Tầng 2 (AIoT Core): Đóng gói chỉ số Edge AI và gửi lên HiveMQ Cloud TLS 8883
 * - Tầng 3 (Cloud AI): Nhận lệnh điều khiển tối ưu thông minh từ Backend Gemini Agent
 */

#include <Arduino.h>

#define BOARD_AIOT_INDUSTRIAL
#include <AIoT.h>
#include <HybridAI/HybridAI.h>

const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASS = "YOUR_WIFI_PASS";
const char *MQTT_USER = "IoT_TEST";
const char *MQTT_PASS = "mt21062005";

HybridAIEngine hybridAI;

// Lắng nghe lệnh điều khiển từ Gemini Agent trên Cloud
Virtual_WRITE(relay1)
{
    int state = param.getInt();
    AIoT_Device.relay(1, state ? HIGH : LOW);
    Serial.printf("[GEMINI AGENT ACTION] Relay 1: %d\n", state);
    AIoT.writeControl("relay1", state);
}

// Chu kỳ gửi Telemetry & Insight lên Cloud (mỗi 3 giây)
void sendHybridTelemetry()
{
    if (!AIoT.CheckConnect())
        return;

    // Trích xuất đặc trưng từ Edge AI
    float meanVal, rmsVal, p2pVal, stdDevVal;
    hybridAI.edge.extractFeatures(meanVal, rmsVal, p2pVal, stdDevVal);

    float confidence = 0.0f;
    int motorState = hybridAI.edge.predictMotorState(confidence);

    AIoT.updateTelemetry("chip_temp", AIoT_Device.readChipTemp());
    AIoT.updateTelemetry("free_ram", (int)AIoT_Device.readFreeRam());
    AIoT.updateTelemetry("vibr_rms", rmsVal);
    AIoT.updateTelemetry("vibr_p2p", p2pVal);
    AIoT.updateTelemetry("edge_state", motorState);
    AIoT.updateTelemetry("edge_conf", confidence);

    // Gửi đúng 1 gói tin JSON tổng hợp lên Cloud
    AIoT.sendTelemetry();
}

void setup()
{
    Serial.begin(115200);

    // Khởi tạo phần cứng
    AIoT_Device.begin();

    // Khởi tạo Hybrid AI: Ngưỡng Z-Score 3.0, 50 mẫu cân chỉnh
    hybridAI.begin(3.0f, 50);

    // Khởi tạo kết nối AIoT IoT Core
    AIoT.begin(WIFI_SSID, WIFI_PASS, MQTT_USER, MQTT_PASS);

    // Hẹn giờ gửi telemetry mỗi 3000ms
    AIoT.addTimeEvent(3000, sendHybridTelemetry);

    Serial.println("AIoT Industrial Hybrid System Ready.");
}

void loop()
{
    AIoT.run();

    // 1. Đọc mẫu cảm biến rung động / dòng điện (tần số cao)
    float rawSensor = AIoT_Device.readVoltage(1) * 10.0f; // Scale giả lập

    // 2. Chạy Edge AI: tự động phản xạ ngắt Relay 1 nếu phát hiện sự cố nguy cấp (CRITICAL)
    EdgeAI::InferenceResult res = hybridAI.process(rawSensor, 1);

    if (res.isEmergency)
    {
        AIoT_Device.led(true);
    }
    else
    {
        AIoT_Device.led(false);
    }

    delay(50); // Lấy mẫu 20Hz
}


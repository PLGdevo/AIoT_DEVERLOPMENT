/**
 * ============================================================================
 * AIoT_LIB - Ví Dụ 04: Tích Hợp Google Gemini & Cloud AI Agent Protocol
 * ============================================================================
 * Mô tả:
 * - Chuẩn hóa gói tin Telemetry và tạo Prompt tự động cho Gemini LLM
 * - Mô phỏng cơ chế Tool-Calling / Action Dispatching nhận từ AI Agent
 */

#include <Arduino.h>
#include <AIoT.h>
#include <CloudAI/CloudAI.h>

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("==========================================");
    Serial.println("   AIoT_LIB: Cloud AI & Gemini Demo");
    Serial.println("==========================================\n");

    // 1. Tạo Telemetry JSON giả lập
    String telemetryData = "{\"chip_temp\": 68.5, \"motor_vibration_rms\": 8.9, \"current_amp\": 15.2}";

    // 2. Sinh Prompt chuyên dụng cho Gemini 1.5 Flash
    String contextRules = "Rule 1: If motor_vibration_rms > 5.0, set relay1 to 0 (stop motor).\n"
                          "Rule 2: If chip_temp > 60.0, set relay2 to 1 (turn on cooling fan).";

    String geminiPrompt = CloudAI::PromptTemplates::buildTelemetryPrompt(telemetryData, contextRules);

    Serial.println("--- [GENERATED GEMINI PROMPT] ---");
    Serial.println(geminiPrompt);
    Serial.println("---------------------------------\n");

    // 3. Đóng gói báo cáo Edge AI gửi lên Cloud Agent
    String edgeReport = CloudAI::AgentProtocol::packEdgeReport("E0:72:A1:D5:ED:1C", 68.5f, 0.92f, "CRITICAL");
    Serial.println("--- [STANDARDIZED AGENT PAYLOAD] ---");
    Serial.println(edgeReport);
    Serial.println("------------------------------------\n");
}

void loop()
{
    delay(1000);
}

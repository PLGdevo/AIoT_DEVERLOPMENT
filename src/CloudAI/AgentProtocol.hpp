#ifndef CLOUD_AI_AGENT_PROTOCOL_HPP
#define CLOUD_AI_AGENT_PROTOCOL_HPP

#include <Arduino.h>

namespace CloudAI
{
    // Cấu trúc gói tin chỉ thị từ Cloud AI Agent gửi về ESP32
    struct AgentCommand
    {
        char action[32];      // Tên hành động ("relay1", "set_pwm", "reboot", "alert")
        int value;            // Giá trị (0/1, mức PWM, tốc độ)
        char reason[128];     // Lý do suy luận của AI
    };

    class AgentProtocol
    {
    public:
        // Đóng gói thông điệp gửi lên Agent kèm thông số Edge AI
        static String packEdgeReport(const char *mac, float sensorVal, float anomalyScore, const char *edgeState)
        {
            String json = "{";
            json += "\"mac_address\":\"" + String(mac) + "\",";
            json += "\"edge_ai\":{";
            json += "\"raw_sensor\":" + String(sensorVal, 2) + ",";
            json += "\"anomaly_score\":" + String(anomalyScore, 3) + ",";
            json += "\"state\":\"" + String(edgeState) + "\"";
            json += "}}";
            return json;
        }
    };
}

#endif /* CLOUD_AI_AGENT_PROTOCOL_HPP */

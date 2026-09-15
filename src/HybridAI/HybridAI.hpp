#ifndef HYBRID_AI_HPP
#define HYBRID_AI_HPP

#include <Arduino.h>
#include <AI_Math/AI_Math.h>
#include <Device/Device.h>
#include <EdgeAI/EdgeAI.h>
#include <CloudAI/CloudAI.h>

// --- Hybrid AI v2 Architecture: Types, Policy, Orchestrator ---
#include "Types.hpp"
#include "PolicyEngine.hpp"
#include "AIOrchestrator.hpp"

namespace HybridAI {

class Engine {
public:
    explicit Engine(const PolicyConfig& config = PolicyConfig{})
        : _orchestrator(config) {}

    AIResult process(const AIInput& input) const {
        return _orchestrator.route(input);
    }

private:
    AIOrchestrator _orchestrator;
};

} // namespace HybridAI

// --- Classic HybridAIEngine (Đảm bảo 100% tương thích ngược với main.cpp và các example) ---
class HybridAIEngine
{
public:
    HybridAIEngine() : _autoEmergencyActuation(true) {}

    void begin(float zThreshold = 3.0f, uint32_t calibSamples = 50)
    {
        edge.begin(zThreshold, calibSamples);
    }

    // Xử lý mẫu cảm biến: Chạy Edge AI + Phản xạ bảo vệ tại chỗ + Trả về kết quả
    EdgeAI::InferenceResult process(float sensorSample, uint8_t emergencyRelayIndex = 1)
    {
        EdgeAI::InferenceResult res = edge.process(sensorSample);

        // Phản xạ bảo vệ phần cứng tức thì tại chỗ nếu trạng thái nguy cấp (CRITICAL)
        if (res.isEmergency && _autoEmergencyActuation)
        {
            AIoT_Device.relay(emergencyRelayIndex, LOW); // Cắt tải bảo vệ máy
            AIoT_Device.buzzer(true);                    // Kích hoạt còi
        }

        return res;
    }

    void setAutoEmergencyActuation(bool enable)
    {
        _autoEmergencyActuation = enable;
    }

    void setGeminiApiKey(const char *key)
    {
        gemini.setApiKey(key);
    }

    EdgeAI::Engine edge;
    CloudAI::GeminiClient gemini;
    HybridAI::Engine orchestrator;

private:
    bool _autoEmergencyActuation;
};

#endif /* HYBRID_AI_HPP */

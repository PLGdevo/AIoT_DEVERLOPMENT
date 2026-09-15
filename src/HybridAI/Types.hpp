#pragma once
#include <stdint.h>

namespace HybridAI {

enum class AIPath : uint8_t {
    EDGE = 0,
    CLOUD = 1,
    HYBRID = 2,
    NONE = 3
};

enum class Severity : uint8_t {
    NORMAL = 0,
    WARNING = 1,
    CRITICAL = 2
};

enum class Decision : uint8_t {
    NONE = 0,
    LOCAL_ACTION = 1,
    CLOUD_ANALYSIS = 2,
    HYBRID_ACTION = 3
};

struct AIInput {
    float value = 0.0f;
    float confidence = 0.0f;
    Severity severity = Severity::NORMAL;
    bool realtime = true;
};

struct AIResult {
    AIPath path = AIPath::NONE;
    Decision decision = Decision::NONE;
    Severity severity = Severity::NORMAL;
    float score = 0.0f;
    bool emergency = false;
    const char* label = nullptr;
    const char* reason = nullptr;
};

} // namespace HybridAI

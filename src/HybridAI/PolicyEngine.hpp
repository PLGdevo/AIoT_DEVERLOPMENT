#pragma once
#include "Types.hpp"

namespace HybridAI {

struct PolicyConfig {
    float cloudThreshold = 0.70f;
    bool allowCloud = true;
    bool allowLocalEmergency = true;
    bool preferEdgeForRealtime = true;
};

class PolicyEngine {
public:
    explicit PolicyEngine(const PolicyConfig& config = PolicyConfig{})
        : _config(config) {}

    AIPath selectPath(const AIInput& input) const {
        if (input.severity == Severity::CRITICAL &&
            _config.allowLocalEmergency) {
            return AIPath::EDGE;
        }

        if (input.realtime && _config.preferEdgeForRealtime &&
            input.confidence >= _config.cloudThreshold) {
            return AIPath::EDGE;
        }

        if (_config.allowCloud)
            return AIPath::CLOUD;

        return AIPath::EDGE;
    }

    Decision selectDecision(AIPath path, Severity severity) const {
        if (severity == Severity::CRITICAL &&
            _config.allowLocalEmergency) {
            return Decision::LOCAL_ACTION;
        }

        if (path == AIPath::CLOUD)
            return Decision::CLOUD_ANALYSIS;

        if (path == AIPath::HYBRID)
            return Decision::HYBRID_ACTION;

        return Decision::LOCAL_ACTION;
    }

private:
    PolicyConfig _config;
};

} // namespace HybridAI

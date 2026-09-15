#pragma once
#include "Types.hpp"
#include "PolicyEngine.hpp"

namespace HybridAI {

class AIOrchestrator {
public:
    explicit AIOrchestrator(const PolicyConfig& policy = PolicyConfig{})
        : _policy(policy) {}

    AIResult route(const AIInput& input) const {
        AIResult result;
        result.severity = input.severity;
        result.score = input.value;

        if (input.severity == Severity::CRITICAL) {
            result.path = AIPath::EDGE;
            result.decision = Decision::LOCAL_ACTION;
            result.emergency = true;
            result.label = "CRITICAL";
            result.reason = "Local emergency policy";
            return result;
        }

        result.path = _policy.selectPath(input);
        result.decision =
            _policy.selectDecision(result.path, input.severity);

        if (result.path == AIPath::EDGE) {
            result.label = "EDGE";
            result.reason = "Low-latency local processing";
        } else if (result.path == AIPath::CLOUD) {
            result.label = "CLOUD";
            result.reason = "Cloud processing selected";
        } else {
            result.label = "HYBRID";
            result.reason = "Combined local/cloud processing";
        }

        return result;
    }

private:
    PolicyEngine _policy;
};

} // namespace HybridAI

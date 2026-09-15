#include <HybridAI/HybridAI.hpp>

HybridAI::Engine hybridAI;

void setup() {
    Serial.begin(115200);

    HybridAI::AIInput input;
    input.value = 0.82f;
    input.confidence = 0.91f;
    input.severity = HybridAI::Severity::WARNING;
    input.realtime = true;

    HybridAI::AIResult result = hybridAI.process(input);

    Serial.print("PATH: ");
    Serial.println(result.label ? result.label : "NONE");

    Serial.print("DECISION: ");
    Serial.println(static_cast<int>(result.decision));

    Serial.print("REASON: ");
    Serial.println(result.reason ? result.reason : "NONE");
}

void loop() {
}

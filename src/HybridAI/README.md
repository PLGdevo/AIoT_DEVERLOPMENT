# HybridAI

HybridAI is the orchestration layer between Edge AI and Cloud AI.

## Responsibilities

- Route AI workloads between Edge and Cloud.
- Apply safety/emergency policies.
- Keep AI decisions independent from hardware drivers.
- Provide a clean boundary for MQTT, ROS 2, HTTP, or other middleware.

Architecture:

    Edge AI ----\
                 > AI Orchestrator -> Policy -> Decision -> Middleware -> Device
    Cloud AI ---/

HybridAI does not require ROS 2. ROS 2 is an optional middleware integration.

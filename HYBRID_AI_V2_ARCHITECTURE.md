# AIoT_LIB - Hybrid AI v2.0.0

The library is organized around Hybrid AI rather than S.A.I.

Core model:

    Edge AI ----\
                 > Hybrid AI Orchestrator -> Policy -> Decision -> Middleware
    Cloud AI ---/

Edge AI handles low-latency/local inference.
Cloud AI handles larger models and complex reasoning.
HybridAI decides where work should be processed.
Middleware transports commands/data to applications and devices.

ROS 2 is optional and is kept as a middleware integration boundary.
It is not a dependency of the HybridAI core.

Recommended future separation:

- EdgeAI: inference and local models
- CloudAI: provider abstraction + Gemini/other providers
- HybridAI: orchestration, policy, routing and fallback
- Middleware: ROS 2, MQTT, HTTP, Serial, Modbus
- Device: hardware abstraction

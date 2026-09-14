// ============================================================================
// File tự động sinh bởi công cụ huấn luyện: tools/train_edge_ai.py
// Ma trận trọng số TinyML cho bộ phân loại Edge AI trên ESP32 / ESP32-S3
// ============================================================================
#ifndef CUSTOM_MODEL_WEIGHTS_H
#define CUSTOM_MODEL_WEIGHTS_H

#include <stddef.h>

namespace EdgeModels
{
    namespace CustomModel
    {
        constexpr size_t NUM_FEATURES = 4;
        constexpr size_t NUM_CLASSES = 3;

        // Ma trận trọng số W [3 x 4]
        static const float W[3][4] = {
            {-2.200892f, -1.408454f, -0.464787f, -2.552185f},
            {0.659548f, 0.426855f, 0.047554f, 0.361001f},
            {1.541344f, 0.981599f, 0.417233f, 2.191185f}
        };

        // Vector độ lệch Bias b [3]
        static const float b[3] = {
            13.231339f, 0.708301f, -13.939640f
        };

        // Tên các nhãn phân loại
        static const char *CLASS_NAMES[NUM_CLASSES] = {
            "NORMAL",
            "WARNING",
            "CRITICAL"
        };
    }
}

#endif /* CUSTOM_MODEL_WEIGHTS_H */

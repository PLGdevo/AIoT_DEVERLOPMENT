#ifndef MOTOR_VIBRATION_MODEL_H
#define MOTOR_VIBRATION_MODEL_H

#include <Arduino.h>

// Mô hình mẫu chuẩn hóa hệ số phân loại rung động động cơ
// Trọng số hồi quy / phân loại 3 lớp: [0: Normal, 1: Unbalanced/Warning, 2: Bearing Fault/Critical]
namespace EdgeModels
{
    struct ModelWeights
    {
        static constexpr size_t NUM_INPUTS = 4;   // [Mean, RMS, Peak-to-Peak, StdDev]
        static constexpr size_t NUM_CLASSES = 3;  // [NORMAL, WARNING, CRITICAL]

        // Trọng số phân loại lớp (Weights W: 3x4)
        static constexpr float W[NUM_CLASSES][NUM_INPUTS] = {
            {-0.2f, -0.5f, -0.4f, -0.3f},  // Lớp 0: Normal
            { 0.3f,  0.4f,  0.5f,  0.2f},  // Lớp 1: Warning
            { 0.8f,  1.2f,  1.5f,  1.1f}   // Lớp 2: Critical
        };

        // Độ lệch (Bias b: 3)
        static constexpr float b[NUM_CLASSES] = {
            0.5f,
            -0.2f,
            -1.0f
        };
    };
}

#endif /* MOTOR_VIBRATION_MODEL_H */

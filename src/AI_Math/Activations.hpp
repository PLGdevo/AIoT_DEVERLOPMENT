#ifndef AI_MATH_ACTIVATIONS_HPP
#define AI_MATH_ACTIVATIONS_HPP

#include <Arduino.h>
#include <math.h>

namespace AI_Math
{
    class Activations
    {
    public:
        // Hàm ReLU: f(x) = max(0, x)
        static inline float relu(float x)
        {
            return (x > 0.0f) ? x : 0.0f;
        }

        // Áp dụng ReLU trên toàn mảng
        static void reluArray(float *data, size_t length)
        {
            if (data == nullptr)
                return;
            for (size_t i = 0; i < length; i++)
            {
                if (data[i] < 0.0f)
                    data[i] = 0.0f;
            }
        }

        // Hàm Leaky ReLU: f(x) = x nếu x > 0, alpha * x nếu x <= 0
        static inline float leakyRelu(float x, float alpha = 0.01f)
        {
            return (x > 0.0f) ? x : (alpha * x);
        }

        // Hàm Sigmoid: f(x) = 1 / (1 + e^(-x))
        static inline float sigmoid(float x)
        {
            if (x > 45.0f)
                return 1.0f;
            if (x < -45.0f)
                return 0.0f;
            return 1.0f / (1.0f + expf(-x));
        }

        // Hàm Tanh: f(x) = tanh(x)
        static inline float tanhAct(float x)
        {
            return tanhf(x);
        }

        // Hàm Softmax chuẩn hóa xác suất: exp(x_i) / sum(exp(x))
        static void softmax(float *data, size_t length)
        {
            if (data == nullptr || length == 0)
                return;

            // Tìm max để tránh tràn số lũy thừa (Numerical Stability)
            float maxVal = data[0];
            for (size_t i = 1; i < length; i++)
            {
                if (data[i] > maxVal)
                    maxVal = data[i];
            }

            float sumExp = 0.0f;
            for (size_t i = 0; i < length; i++)
            {
                data[i] = expf(data[i] - maxVal);
                sumExp += data[i];
            }

            if (sumExp > 1e-6f)
            {
                for (size_t i = 0; i < length; i++)
                {
                    data[i] /= sumExp;
                }
            }
        }

        // Lấy chỉ số có giá trị lớn nhất (ArgMax)
        static size_t argmax(const float *data, size_t length)
        {
            if (data == nullptr || length == 0)
                return 0;
            size_t maxIdx = 0;
            float maxVal = data[0];
            for (size_t i = 1; i < length; i++)
            {
                if (data[i] > maxVal)
                {
                    maxVal = data[i];
                    maxIdx = i;
                }
            }
            return maxIdx;
        }
    };
}

#endif /* AI_MATH_ACTIVATIONS_HPP */

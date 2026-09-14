#ifndef AI_MATH_STATISTICS_HPP
#define AI_MATH_STATISTICS_HPP

#include <Arduino.h>
#include <math.h>

namespace AI_Math
{
    class Statistics
    {
    public:
        // Tính giá trị trung bình (Mean)
        static float mean(const float *data, size_t length)
        {
            if (data == nullptr || length == 0)
                return 0.0f;
            float sum = 0.0f;
            for (size_t i = 0; i < length; i++)
            {
                sum += data[i];
            }
            return sum / (float)length;
        }

        // Tính phương sai (Variance)
        static float variance(const float *data, size_t length)
        {
            if (data == nullptr || length <= 1)
                return 0.0f;
            float m = mean(data, length);
            float sumSq = 0.0f;
            for (size_t i = 0; i < length; i++)
            {
                float diff = data[i] - m;
                sumSq += diff * diff;
            }
            return sumSq / (float)(length - 1);
        }

        // Tính độ lệch chuẩn (Standard Deviation)
        static float stdDev(const float *data, size_t length)
        {
            return sqrtf(variance(data, length));
        }

        // Tính giá trị hiệu dụng (Root Mean Square - RMS)
        static float rms(const float *data, size_t length)
        {
            if (data == nullptr || length == 0)
                return 0.0f;
            float sumSq = 0.0f;
            for (size_t i = 0; i < length; i++)
            {
                sumSq += data[i] * data[i];
            }
            return sqrtf(sumSq / (float)length);
        }

        // Tìm giá trị nhỏ nhất và lớn nhất (Min & Max)
        static void minMax(const float *data, size_t length, float &minVal, float &maxVal)
        {
            if (data == nullptr || length == 0)
            {
                minVal = 0.0f;
                maxVal = 0.0f;
                return;
            }
            minVal = data[0];
            maxVal = data[0];
            for (size_t i = 1; i < length; i++)
            {
                if (data[i] < minVal)
                    minVal = data[i];
                if (data[i] > maxVal)
                    maxVal = data[i];
            }
        }

        // Tính biên độ đỉnh-đỉnh (Peak-to-Peak)
        static float peakToPeak(const float *data, size_t length)
        {
            float minVal, maxVal;
            minMax(data, length, minVal, maxVal);
            return maxVal - minVal;
        }

        // Chuẩn hóa Min-Max Scaling [0, 1]
        static float minMaxScale(float value, float minVal, float maxVal)
        {
            if (fabsf(maxVal - minVal) < 1e-6f)
                return 0.0f;
            float scaled = (value - minVal) / (maxVal - minVal);
            if (scaled < 0.0f)
                scaled = 0.0f;
            if (scaled > 1.0f)
                scaled = 1.0f;
            return scaled;
        }

        // Chuẩn hóa Z-Score: (x - mean) / stdDev
        static float zScore(float value, float meanVal, float stdDevVal)
        {
            if (stdDevVal < 1e-6f)
                return 0.0f;
            return (value - meanVal) / stdDevVal;
        }
    };
}

#endif /* AI_MATH_STATISTICS_HPP */

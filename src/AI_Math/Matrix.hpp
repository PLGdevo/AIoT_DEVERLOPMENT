#ifndef AI_MATH_MATRIX_HPP
#define AI_MATH_MATRIX_HPP

#include <Arduino.h>
#include <math.h>

namespace AI_Math
{
    class Matrix
    {
    public:
        // Tích vô hướng của 2 vector (Dot Product)
        static float dotProduct(const float *a, const float *b, size_t length)
        {
            if (a == nullptr || b == nullptr || length == 0)
                return 0.0f;
            float sum = 0.0f;
            for (size_t i = 0; i < length; i++)
            {
                sum += a[i] * b[i];
            }
            return sum;
        }

        // Khoảng cách Euclid giữa 2 vector (Euclidean Distance)
        static float euclideanDistance(const float *a, const float *b, size_t length)
        {
            if (a == nullptr || b == nullptr || length == 0)
                return 0.0f;
            float sumSq = 0.0f;
            for (size_t i = 0; i < length; i++)
            {
                float diff = a[i] - b[i];
                sumSq += diff * diff;
            }
            return sqrtf(sumSq);
        }

        // Khoảng cách Manhattan giữa 2 vector
        static float manhattanDistance(const float *a, const float *b, size_t length)
        {
            if (a == nullptr || b == nullptr || length == 0)
                return 0.0f;
            float sum = 0.0f;
            for (size_t i = 0; i < length; i++)
            {
                sum += fabsf(a[i] - b[i]);
            }
            return sum;
        }

        // Nhân Vector với Ma trận: y = W * x + b
        // W có kích thước [outRows x inCols], x có kích thước [inCols], b có kích thước [outRows]
        static void denseForward(const float *W, const float *x, const float *b,
                                 float *y, size_t outRows, size_t inCols)
        {
            if (W == nullptr || x == nullptr || y == nullptr)
                return;
            for (size_t r = 0; r < outRows; r++)
            {
                float sum = (b != nullptr) ? b[r] : 0.0f;
                const float *weightRow = &W[r * inCols];
                for (size_t c = 0; c < inCols; c++)
                {
                    sum += weightRow[c] * x[c];
                }
                y[r] = sum;
            }
        }
    };
}

#endif /* AI_MATH_MATRIX_HPP */

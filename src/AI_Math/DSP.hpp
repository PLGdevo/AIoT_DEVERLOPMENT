#ifndef AI_MATH_DSP_HPP
#define AI_MATH_DSP_HPP

#include <Arduino.h>
#include <math.h>

namespace AI_Math
{
    // Cửa sổ trượt lưu trữ mẫu tín hiệu (Circular Sliding Window)
    template <size_t CAPACITY>
    class SlidingWindow
    {
    public:
        SlidingWindow() : head(0), count(0) {}

        void push(float value)
        {
            buffer[head] = value;
            head = (head + 1) % CAPACITY;
            if (count < CAPACITY)
                count++;
        }

        void clear()
        {
            head = 0;
            count = 0;
        }

        bool isFull() const { return count == CAPACITY; }
        size_t size() const { return count; }
        size_t capacity() const { return CAPACITY; }

        // Sao chép dữ liệu tuyến tính từ cũ nhất đến mới nhất vào mảng đích
        void toArray(float *dest) const
        {
            if (dest == nullptr || count == 0)
                return;
            size_t startIdx = (count < CAPACITY) ? 0 : head;
            for (size_t i = 0; i < count; i++)
            {
                dest[i] = buffer[(startIdx + i) % CAPACITY];
            }
        }

        // Lấy giá trị mới nhất
        float latest() const
        {
            if (count == 0)
                return 0.0f;
            size_t idx = (head == 0) ? (CAPACITY - 1) : (head - 1);
            return buffer[idx];
        }

    private:
        float buffer[CAPACITY];
        size_t head;
        size_t count;
    };

    // Bộ lọc trung bình trượt (Moving Average Filter)
    class MovingAverageFilter
    {
    public:
        MovingAverageFilter(size_t windowSize = 5) : size(windowSize), index(0), count(0), sum(0.0f)
        {
            if (size > 32)
                size = 32;
            for (size_t i = 0; i < 32; i++)
                history[i] = 0.0f;
        }

        float update(float input)
        {
            sum -= history[index];
            history[index] = input;
            sum += input;
            index = (index + 1) % size;
            if (count < size)
                count++;
            return sum / (float)count;
        }

        void reset()
        {
            index = 0;
            count = 0;
            sum = 0.0f;
            for (size_t i = 0; i < 32; i++)
                history[i] = 0.0f;
        }

    private:
        float history[32];
        size_t size;
        size_t index;
        size_t count;
        float sum;
    };

    // Bộ lọc thông thấp Exponential IIR Low-Pass Filter
    class LowPassFilter
    {
    public:
        // alpha trong khoảng [0.0, 1.0]: alpha càng nhỏ lọc càng mượt
        LowPassFilter(float alpha = 0.2f) : _alpha(alpha), _output(0.0f), _initialized(false) {}

        float update(float input)
        {
            if (!_initialized)
            {
                _output = input;
                _initialized = true;
                return _output;
            }
            _output = _alpha * input + (1.0f - _alpha) * _output;
            return _output;
        }

        void reset() { _initialized = false; }
        void setAlpha(float alpha) { _alpha = alpha; }

    private:
        float _alpha;
        float _output;
        bool _initialized;
    };

    // Bộ biến đổi Fourier nhanh (Cooley-Tukey Radix-2 FFT)
    class FastFourierTransform
    {
    public:
        // N phải là lũy thừa của 2 (ví dụ: 16, 32, 64, 128, 256)
        // inputReal: mảng tín hiệu đầu vào kích thước N
        // outputMagnitude: mảng biên độ phổ tần số đầu ra kích thước N/2
        static void computeMagnitude(const float *inputReal, float *outputMagnitude, size_t N)
        {
            if (inputReal == nullptr || outputMagnitude == nullptr || N < 2)
                return;

            float real[N];
            float imag[N];

            for (size_t i = 0; i < N; i++)
            {
                real[i] = inputReal[i];
                imag[i] = 0.0f;
            }

            // Bit-reversal permutation
            size_t j = 0;
            for (size_t i = 0; i < N - 1; i++)
            {
                if (i < j)
                {
                    float tempR = real[i];
                    real[i] = real[j];
                    real[j] = tempR;

                    float tempI = imag[i];
                    imag[i] = imag[j];
                    imag[j] = tempI;
                }
                size_t k = N >> 1;
                while (k <= j)
                {
                    j -= k;
                    k >>= 1;
                }
                j += k;
            }

            // Cooley-Tukey FFT
            for (size_t len = 2; len <= N; len <<= 1)
            {
                float angle = -2.0f * (float)M_PI / (float)len;
                float wlen_r = cosf(angle);
                float wlen_i = sinf(angle);

                for (size_t i = 0; i < N; i += len)
                {
                    float w_r = 1.0f;
                    float w_i = 0.0f;

                    for (size_t k = 0; k < len / 2; k++)
                    {
                        float u_r = real[i + k];
                        float u_i = imag[i + k];

                        float v_r = real[i + k + len / 2] * w_r - imag[i + k + len / 2] * w_i;
                        float v_i = real[i + k + len / 2] * w_i + imag[i + k + len / 2] * w_r;

                        real[i + k] = u_r + v_r;
                        imag[i + k] = u_i + v_i;

                        real[i + k + len / 2] = u_r - v_r;
                        imag[i + k + len / 2] = u_i - v_i;

                        float temp_w_r = w_r * wlen_r - w_i * wlen_i;
                        w_i = w_r * wlen_i + w_i * wlen_r;
                        w_r = temp_w_r;
                    }
                }
            }

            // Tính biên độ Magnitude cho nửa dải tần [0, N/2)
            for (size_t i = 0; i < N / 2; i++)
            {
                outputMagnitude[i] = sqrtf(real[i] * real[i] + imag[i] * imag[i]) / (float)(N / 2);
            }
        }
    };
}

#endif /* AI_MATH_DSP_HPP */

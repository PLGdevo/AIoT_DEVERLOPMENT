#ifndef EDGE_AI_HPP
#define EDGE_AI_HPP

#include <Arduino.h>
#include <AI_Math/AI_Math.h>
#include "AnomalyDetector.hpp"
#include "Classifier.hpp"
#include "Models/MotorVibrationModel.h"

namespace EdgeAI
{
    struct InferenceResult
    {
        float score;           // Điểm số bất thường [0.0, 1.0]
        DeviceState state;     // Trạng thái: NORMAL, WARNING, CRITICAL
        bool isEmergency;      // Cần xử lý khẩn cấp tại chỗ
        const char *label;     // Chuỗi trạng thái ("NORMAL", "WARNING", "CRITICAL")
    };

    class Engine
    {
    public:
        Engine() : _slidingWindow(), _detector(), _classifier() {}

        void begin(float zThreshold = 3.0f, uint32_t calibrationSamples = 50)
        {
            _detector.setThreshold(zThreshold);
            _slidingWindow.clear();
        }

        // Đưa một mẫu cảm biến vào và thực thi suy luận tại chỗ
        InferenceResult process(float sample)
        {
            _slidingWindow.push(sample);

            bool isAnomaly = false;
            float score = _detector.processSample(sample, isAnomaly);
            DeviceState state = _classifier.classify(score);

            InferenceResult res;
            res.score = score;
            res.state = state;
            res.isEmergency = (state == STATE_CRITICAL);
            res.label = stateToString(state);
            return res;
        }

        // Trích xuất vector đặc trưng từ cửa sổ mẫu hiện tại
        void extractFeatures(float &meanVal, float &rmsVal, float &p2pVal, float &stdDevVal) const
        {
            size_t n = _slidingWindow.size();
            if (n == 0)
            {
                meanVal = rmsVal = p2pVal = stdDevVal = 0.0f;
                return;
            }
            float data[64];
            size_t count = (n > 64) ? 64 : n;
            _slidingWindow.toArray(data);

            meanVal = AI_Math::Statistics::mean(data, count);
            rmsVal = AI_Math::Statistics::rms(data, count);
            p2pVal = AI_Math::Statistics::peakToPeak(data, count);
            stdDevVal = AI_Math::Statistics::stdDev(data, count);
        }

        // Chạy suy luận mạng nơ-ron Dense Layer phân loại động cơ
        int predictMotorState(float &confidenceOut) const
        // Suy luận mô hình nơ-ron tổng quát: Cho phép nạp bất kỳ ma trận trọng số W, b của người dùng
        int predict(const float *features, const float *W, const float *b, size_t numClasses, size_t numFeatures, float &confidenceOut) const
        {
            float feat[4];
            extractFeatures(feat[0], feat[1], feat[2], feat[3]);
            if (numClasses == 0 || numFeatures == 0 || W == nullptr || b == nullptr || features == nullptr)
            {
                confidenceOut = 0.0f;
                return -1;
            }

            float logits[EdgeModels::MotorVibration::NUM_CLASSES];
            AI_Math::Matrix::denseForward(
                (const float *)EdgeModels::MotorVibration::W,
                feat,
                EdgeModels::MotorVibration::b,
                logits,
                EdgeModels::MotorVibration::NUM_CLASSES,
                EdgeModels::MotorVibration::NUM_INPUTS);
            float logits[16];
            size_t classes = (numClasses > 16) ? 16 : numClasses;

            AI_Math::Activations::softmax(logits, EdgeModels::MotorVibration::NUM_CLASSES);
            size_t bestClass = AI_Math::Activations::argmax(logits, EdgeModels::MotorVibration::NUM_CLASSES);
            AI_Math::Matrix::denseForward(W, features, b, logits, classes, numFeatures);
            AI_Math::Activations::softmax(logits, classes);
            size_t bestClass = AI_Math::Activations::argmax(logits, classes);
            confidenceOut = logits[bestClass];
            return (int)bestClass;
        }

        // Suy luận với các đặc trưng tự động trích xuất từ cửa sổ trượt
        int predict(const float *W, const float *b, size_t numClasses, size_t numFeatures, float &confidenceOut) const
        {
            float feat[4];
            extractFeatures(feat[0], feat[1], feat[2], feat[3]);
            return predict(feat, W, b, numClasses, (numFeatures < 4) ? numFeatures : 4, confidenceOut);
        }

        AnomalyDetector &getDetector() { return _detector; }
        DeviceStateClassifier &getClassifier() { return _classifier; }

    private:
        AI_Math::SlidingWindow<64> _slidingWindow;
        AnomalyDetector _detector;
        DeviceStateClassifier _classifier;
    };
}

#endif /* EDGE_AI_HPP */


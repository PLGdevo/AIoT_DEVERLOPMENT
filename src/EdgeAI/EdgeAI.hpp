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
        {
            float feat[4];
            extractFeatures(feat[0], feat[1], feat[2], feat[3]);

            float logits[EdgeModels::ModelWeights::NUM_CLASSES];
            AI_Math::Matrix::denseForward(
                (const float *)EdgeModels::ModelWeights::W,
                feat,
                EdgeModels::ModelWeights::b,
                logits,
                EdgeModels::ModelWeights::NUM_CLASSES,
                EdgeModels::ModelWeights::NUM_INPUTS);

            AI_Math::Activations::softmax(logits, EdgeModels::ModelWeights::NUM_CLASSES);
            size_t bestClass = AI_Math::Activations::argmax(logits, EdgeModels::ModelWeights::NUM_CLASSES);
            confidenceOut = logits[bestClass];
            return (int)bestClass;
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


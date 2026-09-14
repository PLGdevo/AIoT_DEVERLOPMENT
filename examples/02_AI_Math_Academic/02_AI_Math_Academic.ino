/**
 * ============================================================================
 * AIoT_LIB - Ví Dụ 02: Nền Tảng Học Thuật & Điện Toán AI (AI_Math)
 * ============================================================================
 * Mô tả:
 * - Minh họa các phép toán ma trận, hàm kích hoạt (ReLU, Softmax)
 * - Xử lý tín hiệu số DSP (Sliding Window, FFT phân tích phổ tần số)
 * - Thuật toán Welford học phân phối chuẩn trực tuyến O(1) bộ nhớ
 * - Thuật toán k-NN tự học và phân loại mẫu trên chip
 */

#include <Arduino.h>
#include <AI_Math/AI_Math.h>

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n==========================================");
    Serial.println("   AIoT_LIB: AI_Math Academic Demo");
    Serial.println("==========================================\n");

    // 1. Thống kê & Chuẩn hóa
    float dataSamples[] = {10.5, 12.3, 11.8, 14.2, 9.9, 13.1, 12.0, 11.5};
    size_t n = sizeof(dataSamples) / sizeof(dataSamples[0]);

    float meanVal = AI_Math::Statistics::mean(dataSamples, n);
    float stdVal = AI_Math::Statistics::stdDev(dataSamples, n);
    float rmsVal = AI_Math::Statistics::rms(dataSamples, n);
    float p2pVal = AI_Math::Statistics::peakToPeak(dataSamples, n);

    Serial.println("--- [1. STATISTICAL DESCRIPTORS] ---");
    Serial.printf("Mean: %.2f | StdDev: %.2f | RMS: %.2f | Peak-to-Peak: %.2f\n\n",
                  meanVal, stdVal, rmsVal, p2pVal);

    // 2. Thuật toán Welford (Online Incremental Learning)
    Serial.println("--- [2. WELFORD INCREMENTAL LEARNING] ---");
    AI_Math::WelfordEstimator welford;
    for (size_t i = 0; i < n; i++)
    {
        welford.update(dataSamples[i]);
    }
    Serial.printf("Welford Mean: %.2f (vs Real Mean: %.2f)\n", welford.getMean(), meanVal);
    Serial.printf("Welford StdDev: %.2f (vs Real StdDev: %.2f)\n", welford.getStdDev(), stdVal);

    // Kiểm tra Z-Score của mẫu mới
    float testAnomaly = 25.0; // Giá trị bất thường cao
    float zScore = welford.computeZScore(testAnomaly);
    Serial.printf("Z-Score for %.1f: %.2f sigma (Anomaly: %s)\n\n",
                  testAnomaly, zScore, zScore > 3.0f ? "YES" : "NO");

    // 3. Xử lý tín hiệu FFT (Fast Fourier Transform)
    Serial.println("--- [3. FAST FOURIER TRANSFORM (FFT)] ---");
    constexpr size_t FFT_SIZE = 16;
    float timeSignal[FFT_SIZE];
    // Tạo tín hiệu sin giả lập tần số
    for (size_t i = 0; i < FFT_SIZE; i++)
    {
        timeSignal[i] = sinf(2.0f * PI * 2.0f * (float)i / (float)FFT_SIZE);
    }
    float spectrum[FFT_SIZE / 2];
    AI_Math::FastFourierTransform::computeMagnitude(timeSignal, spectrum, FFT_SIZE);
    Serial.println("Frequency Spectrum Magnitudes (First 8 Bins):");
    for (size_t i = 0; i < FFT_SIZE / 2; i++)
    {
        Serial.printf("Bin[%u]: %.3f\n", i, spectrum[i]);
    }
    Serial.println();

    // 4. Phân loại On-device k-NN (k-Nearest Neighbors)
    Serial.println("--- [4. ON-DEVICE k-NN CLASSIFIER] ---");
    // [Feature 1, Feature 2]
    AI_Math::OnlineKNN<2, 10, 3> knn;

    // Huấn luyện Lớp 0 (Bình thường: rung thấp, nhiệt thấp)
    float sample0_1[] = {1.0, 25.0};
    float sample0_2[] = {1.2, 26.0};
    float sample0_3[] = {0.9, 24.5};
    knn.addSample(sample0_1, 0);
    knn.addSample(sample0_2, 0);
    knn.addSample(sample0_3, 0);

    // Huấn luyện Lớp 1 (Lỗi động cơ: rung cao, nhiệt cao)
    float sample1_1[] = {8.5, 75.0};
    float sample1_2[] = {9.0, 80.0};
    float sample1_3[] = {8.2, 72.0};
    knn.addSample(sample1_1, 1);
    knn.addSample(sample1_2, 1);
    knn.addSample(sample1_3, 1);

    // Thử nghiệm dự đoán mẫu mới
    float query[] = {8.7, 78.0};
    float minDist = 0.0f;
    int predictedClass = knn.predict(query, &minDist);
    Serial.printf("Query sample [8.7, 78.0] -> Predicted Class: %d (0: Normal, 1: Fault) | Nearest Dist: %.2f\n",
                  predictedClass, minDist);

    Serial.println("\nDemo completed successfully.");
}

void loop()
{
    delay(1000);
}


#ifndef AIOT_DEVICE_SENSOR_HPP
#define AIOT_DEVICE_SENSOR_HPP

#include <Arduino.h>

class SensorManager
{
public:
    SensorManager()
    {
        for (int i = 0; i < 8; i++)
        {
            _analogPins[i] = -1;
            _digitalPins[i] = -1;
        }
    }

    void setAnalogPin(uint8_t index, int pin)
    {
        if (index < 8)
        {
            _analogPins[index] = pin;
        }
    }

    void setDigitalPin(uint8_t index, int pin, int mode = INPUT)
    {
        if (index < 8)
        {
            _digitalPins[index] = pin;
            if (pin >= 0)
            {
                pinMode(pin, mode);
            }
        }
    }

    int readAnalog(uint8_t index) const
    {
        if (index < 8 && _analogPins[index] >= 0)
        {
            return analogRead(_analogPins[index]);
        }
        return 0;
    }

    float readAnalogVoltage(uint8_t index, float vRef = 3.3f, int maxAdc = 4095) const
    {
        int raw = readAnalog(index);
        return ((float)raw / (float)maxAdc) * vRef;
    }

    bool readDigital(uint8_t index) const
    {
        if (index < 8 && _digitalPins[index] >= 0)
        {
            return digitalRead(_digitalPins[index]) == HIGH;
        }
        return false;
    }

    // Đọc nhiệt độ tích hợp của chip ESP32 (°C)
    static float readChipTemperature()
    {
#if defined(ESP32)
        return temperatureRead();
#else
        return 0.0f;
#endif
    }

    // Đọc dung lượng RAM khả dụng (bytes)
    static uint32_t readFreeRam()
    {
#if defined(ESP32) || defined(ESP8266)
        return ESP.getFreeHeap();
#else
        return 0;
#endif
    }

private:
    int _analogPins[8];
    int _digitalPins[8];
};

#endif /* AIOT_DEVICE_SENSOR_HPP */


#ifndef AIOT_DEVICE_ACTUATOR_HPP
#define AIOT_DEVICE_ACTUATOR_HPP

#include <Arduino.h>

class ActuatorManager
{
public:
    ActuatorManager() : _buzzerPin(-1), _ledPin(-1), _rgbPin(-1)
    {
        for (int i = 0; i < 8; i++)
        {
            _relayPins[i] = -1;
            _relayStates[i] = false;
        }
    }

    void setRgbPin(int pin)
    {
        _rgbPin = pin;
        if (_rgbPin >= 0)
        {
            pinMode(_rgbPin, OUTPUT);
#if defined(ESP32)
            neopixelWrite(_rgbPin, 0, 0, 0); // Tắt ban đầu
#endif
        }
    }

    int getRgbPin() const { return _rgbPin; }

    void setLedPin(int pin)
    {
        _ledPin = pin;
        if (_ledPin >= 0)
        {
            pinMode(_ledPin, OUTPUT);
            digitalWrite(_ledPin, LOW);
        }
    }

    void setBuzzerPin(int pin)
    {
        _buzzerPin = pin;
        if (_buzzerPin >= 0)
        {
            pinMode(_buzzerPin, OUTPUT);
            digitalWrite(_buzzerPin, LOW);
        }
    }

    void setRelayPin(uint8_t index, int pin)
    {
        if (index < 8)
        {
            _relayPins[index] = pin;
            if (pin >= 0)
            {
                pinMode(pin, OUTPUT);
                digitalWrite(pin, LOW);
                _relayStates[index] = false;
            }
        }
    }

    void setLed(bool state)
    {
        if (_ledPin >= 0)
        {
            digitalWrite(_ledPin, state ? HIGH : LOW);
        }
    }

    void toggleLed()
    {
        if (_ledPin >= 0)
        {
            digitalWrite(_ledPin, !digitalRead(_ledPin));
        }
    }

    void setRelay(uint8_t index, bool state)
    {
        if (index < 8 && _relayPins[index] >= 0)
        {
            digitalWrite(_relayPins[index], state ? HIGH : LOW);
            _relayStates[index] = state;
        }
    }

    bool getRelay(uint8_t index) const
    {
        if (index < 8)
            return _relayStates[index];
        return false;
    }

    void toggleRelay(uint8_t index)
    {
        if (index < 8)
        {
            setRelay(index, !_relayStates[index]);
        }
    }

    void buzzerBeep(unsigned int durationMs = 100)
    {
        if (_buzzerPin >= 0)
        {
            digitalWrite(_buzzerPin, HIGH);
            delay(durationMs);
            digitalWrite(_buzzerPin, LOW);
        }
    }

    void setBuzzer(bool state)
    {
        if (_buzzerPin >= 0)
        {
            digitalWrite(_buzzerPin, state ? HIGH : LOW);
        }
    }

private:
    int _ledPin;
    int _buzzerPin;
    int _rgbPin;
    int _relayPins[8];
    bool _relayStates[8];
};

#endif /* AIOT_DEVICE_ACTUATOR_HPP */


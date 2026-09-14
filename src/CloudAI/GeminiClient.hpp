#ifndef CLOUD_AI_GEMINI_CLIENT_HPP
#define CLOUD_AI_GEMINI_CLIENT_HPP

#include <Arduino.h>
#include <WiFiClientSecure.h>

namespace CloudAI
{
    class GeminiClient
    {
    public:
        GeminiClient(const char *apiKey = "", const char *model = "gemini-1.5-flash")
            : _model(model), _timeoutMs(10000)
        {
            if (apiKey != nullptr && strlen(apiKey) > 0)
            {
                strncpy(_apiKey, apiKey, sizeof(_apiKey) - 1);
                _apiKey[sizeof(_apiKey) - 1] = '\0';
            }
            else
            {
                _apiKey[0] = '\0';
            }
        }

        void setApiKey(const char *apiKey)
        {
            if (apiKey != nullptr)
            {
                strncpy(_apiKey, apiKey, sizeof(_apiKey) - 1);
                _apiKey[sizeof(_apiKey) - 1] = '\0';
            }
        }

        void setModel(const char *model)
        {
            if (model != nullptr)
                _model = model;
        }

        bool hasApiKey() const { return strlen(_apiKey) > 0; }
        const char *getModel() const { return _model.c_str(); }

    private:
        char _apiKey[96];
        String _model;
        uint32_t _timeoutMs;
    };
}

#endif /* CLOUD_AI_GEMINI_CLIENT_HPP */

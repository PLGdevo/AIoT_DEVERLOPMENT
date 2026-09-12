#ifndef ESP32_MQTT_HPP
#define ESP32_MQTT_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <MQTT/PubSubClient/PubSubClient.h>
#include <stdint.h>
#include <IoT/API.hpp>
#include <IoT/Param.hpp>

template <class MQTT>
class MQTTESP32
{
public:
    void config(const char *mqtt_userName, const char *mqtt_pass);
    void begin();
    void disconnect();
    void run();
    void receiveData(String Topic_r);

    void PublishData_tele(const char *data);
    void PublishData_control(const char *data);
    bool Check_mode_sub(char *topic, char *mess);
    bool check_connect();

    void SubscribeTopic(const char *baseTopic, const char *Topic_ne);
    void UnsubscribeTopic(const char *baseTopic, const char *Topic_ne);
    void PublishTopic(const char *baseTopic, const char *Topic_ne);
    void UnpublishTopic(const char *baseTopic, const char *Topic_ne);

private:
    const char *MQTT_Server = "broker.hivemq.com";
    const int16_t MQTT_PORT = 1883;
    char MQTT_ID[64];
    char MQTT_USERNAME[64] = "";
    char MQTT_PASS[64] = "";

    char MQTT_BASE_TOPIC[64] = BASE_TOPIC;
    char _mac[32];

    unsigned long Time_connect_MQTT = 0;
    unsigned long Timeout_MQTT = 20000;
};

WiFiClient server;
PubSubClient mqttClient(server);
MQTTESP32<PubSubClient> mqtt;

void IoT_Callback(char *topic, byte *payload, unsigned int length)
{
    char *msg = (char *)malloc(length + 1);
    if (!msg)
        return;
    memcpy(msg, payload, length);
    msg[length] = '\0';
    API_MESS.handleMessage(topic, msg);
    free(msg);
}

template <class MQTT>
inline void MQTTESP32<MQTT>::SubscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.subscribe(NameTopic);
}
template <class MQTT>
inline void MQTTESP32<MQTT>::UnsubscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.unsubscribe(NameTopic);
}

/* Publish Topic */
template <class MQTT>
inline void MQTTESP32<MQTT>::PublishTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
}
template <class MQTT>
inline void MQTTESP32<MQTT>::UnpublishTopic(const char *baseTopic, const char *Topic_ne)
{
}

template <class MQTT>
inline void MQTTESP32<MQTT>::PublishData_tele(const char *data)
{
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s/%s", BASE_TOPIC, _mac, PUB_PREFIX_TELEMETRY_TOPIC);
    mqttClient.publish(NameTopic, data);
}
template <class MQTT>
inline void MQTTESP32<MQTT>::PublishData_control(const char *data)
{
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s/%s", BASE_TOPIC, _mac, PUB_PREFIX_CONTROL_TOPIC);
    mqttClient.publish(NameTopic, data);
}

template <class MQTT>
inline void MQTTESP32<MQTT>::config(const char *mqtt_userName, const char *mqtt_pass)
{
    strncpy(MQTT_USERNAME, mqtt_userName, sizeof(MQTT_USERNAME) - 1);
    MQTT_USERNAME[sizeof(MQTT_USERNAME) - 1] = '\0';
    strncpy(MQTT_PASS, mqtt_pass, sizeof(MQTT_PASS) - 1);
    MQTT_PASS[sizeof(MQTT_PASS) - 1] = '\0';
    String MAC = WiFi.macAddress();
    strncpy(_mac, MAC.c_str(), sizeof(_mac) - 1);
    _mac[sizeof(_mac) - 1] = '\0';
    strncpy(MQTT_ID, _mac, sizeof(MQTT_ID) - 1);
    MQTT_ID[sizeof(MQTT_ID) - 1] = '\0';
}

template <class MQTT>
inline bool MQTTESP32<MQTT>::check_connect()
{
    return mqttClient.connected();
}

template <class MQTT>
inline void MQTTESP32<MQTT>::disconnect()
{
    snprintf(MQTT_BASE_TOPIC, sizeof(MQTT_BASE_TOPIC), "%s%s", BASE_TOPIC, _mac);
    this->UnsubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_TELEMETRY_TOPIC);
    this->UnsubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_CONTROL_TOPIC);
    delay(100);
}

template <class MQTT>
inline void MQTTESP32<MQTT>::begin()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Time_connect_MQTT = 0;
        return;
    }
    // Tự sinh Client ID ngẫu nhiên nếu không sử dụng ID
    if (strlen(MQTT_ID) == 0)
    {
        snprintf(MQTT_ID, sizeof(MQTT_ID), "ESP32_%04X%04X", (uint16_t)random(0xFFFF), (uint16_t)random(0xFFFF));
    }
    
    Time_connect_MQTT = millis();
    server.stop();
    disconnect();
    vTaskDelay(pdMS_TO_TICKS(100));
    mqttClient.setServer(MQTT_Server, MQTT_PORT);
    mqttClient.setKeepAlive(15);
    mqttClient.setSocketTimeout(2);
    mqttClient.setCallback(IoT_Callback);
    LOG_MQTT("MQTT", "ANONYMOUS CLIENT ID: %s", MQTT_ID);
    LOG_MQTT("MQTT", "CONNECTING TO BROKER (%s:%d)...", MQTT_Server, MQTT_PORT);
    while (WiFi.status() == WL_CONNECTED && !mqttClient.connected() && (millis() - Time_connect_MQTT <= Timeout_MQTT))
    {
        bool connected = false;
        if (strlen(MQTT_USERNAME) > 0)
        {
            connected = mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS);
        }
        else
        {
            // Kết nối hoàn toàn không cần ID / Username / Password (Anonymous mode)
            connected = mqttClient.connect(MQTT_ID);
        }

        if (connected)
        {
            snprintf(MQTT_BASE_TOPIC, sizeof(MQTT_BASE_TOPIC), "%s%s", BASE_TOPIC, _mac);
            this->SubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_TELEMETRY_TOPIC);
            this->SubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_CONTROL_TOPIC);
            LOG_MQTT("MQTT", "CONNECTED TO BROKER SUCCESSFULLY");
            return;
        }
        LOG_MQTT("MQTT", "TRY CONNECT TO SERVER...");

        LOG_MQTT("MQTT", "MQTT_ID: %s", MQTT_ID);
        if (strlen(MQTT_USERNAME) > 0)
        {
            LOG_MQTT("MQTT", "MQTT_USER: %s", MQTT_USERNAME);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    if (!mqttClient.connected())
    {
        LOG_ERROR("MQTT", "CONNECT TIMEOUT OR FAILED");
        LOG_ERROR("MQTT", "FAILED CONNECT, rc=%d", mqttClient.state());
    }
}

template <class MQTT>
inline void MQTTESP32<MQTT>::run()
{
    mqttClient.loop();
}

MQTTESP32<PubSubClient> serverMQTT;

#endif /*ESP32_MQTT_HPP*/
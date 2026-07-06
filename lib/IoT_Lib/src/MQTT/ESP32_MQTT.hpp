#ifndef ESP32_MQTT_HPP
#define ESP32_MQTT_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <MQTT/NPT_Client/NTPClient.h>
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
    const char *MQTT_Server = "mqtt.ait.caothang.edu.vn";
    const int16_t MQTT_PORT = 8883;
    char MQTT_ID[30];
    char MQTT_USERNAME[30];
    char MQTT_PASS[30];

    char MQTT_BASE_TOPIC[30] = BASE_TOPIC;
    char _mac[30];

    unsigned long Time_connect_MQTT = 0;
    unsigned long Timeout_MQTT = 20000;
};

WiFiClientSecure server;
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
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.subscribe(NameTopic);
}
template <class MQTT>
inline void MQTTESP32<MQTT>::UnsubscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.unsubscribe(NameTopic);
}

/* Publish Topic */
template <class MQTT>
inline void MQTTESP32<MQTT>::PublishTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
}
template <class MQTT>
inline void MQTTESP32<MQTT>::UnpublishTopic(const char *baseTopic, const char *Topic_ne)
{
}

template <class MQTT>
inline void MQTTESP32<MQTT>::PublishData_tele(const char *data)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", BASE_TOPIC, PUB_PREFIX_TELEMETRY_TOPIC);
    mqttClient.publish(NameTopic, data);
}
template <class MQTT>
inline void MQTTESP32<MQTT>::PublishData_control(const char *data)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", BASE_TOPIC, PUB_PREFIX_CONTROL_TOPIC);
    mqttClient.publish(NameTopic, data);
}

template <class MQTT>
inline void MQTTESP32<MQTT>::config(const char *mqtt_userName, const char *mqtt_pass)
{
    strcpy(MQTT_USERNAME, mqtt_userName);
    strcpy(MQTT_PASS, mqtt_pass);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    strcpy(MQTT_ID, _mac);
}

template <class MQTT>
inline bool MQTTESP32<MQTT>::check_connect()
{
    if (mqttClient.connected())
    {
        return true;
    }
    else
    {
        return false;
    }
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
    // Lấy MAC tại đây
    String MAC = WiFi.macAddress();
    strncpy(_mac, MAC.c_str(), sizeof(_mac) - 1);
    _mac[sizeof(_mac) - 1] = '\0';
    strncpy(MQTT_ID, _mac, sizeof(MQTT_ID) - 1);
    MQTT_ID[sizeof(MQTT_ID) - 1] = '\0';
    Time_connect_MQTT = millis();
    server.stop();
    disconnect();
    vTaskDelay(pdMS_TO_TICKS(100));
    server.setInsecure();
    mqttClient.setServer(MQTT_Server, MQTT_PORT);
    mqttClient.setKeepAlive(10);
    mqttClient.setSocketTimeout(2);
    mqttClient.setCallback(IoT_Callback);
    LOG_MQTT("MQTT", "CLIENT ID = %s", MQTT_ID);
    LOG_MQTT("MQTT", "CONNECTING TO SERVER.....");
    while (WiFi.status() == WL_CONNECTED && !mqttClient.connected() && (millis() - Time_connect_MQTT <= Timeout_MQTT))
    {
        if (mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
        {
            snprintf(MQTT_BASE_TOPIC, sizeof(MQTT_BASE_TOPIC), "%s%s", BASE_TOPIC, _mac);
            this->SubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_TELEMETRY_TOPIC);
            this->SubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_CONTROL_TOPIC);
            LOG_MQTT("MQTT", "CONNECTED TO SERVER MQTT");
            return;
        }
        LOG_MQTT("MQTT", "TRY CONNECT TO SERVER MQTT...");

        LOG_MQTT("MQTT", "MQTT_ID: %s", MQTT_ID);
        LOG_MQTT("MQTT", "MQTT_USER: %s", MQTT_USERNAME);
        LOG_MQTT("MQTT", "MQTT_PASS: %s", MQTT_PASS);

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
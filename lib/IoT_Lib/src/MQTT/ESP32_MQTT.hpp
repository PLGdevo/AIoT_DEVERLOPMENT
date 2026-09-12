#ifndef ESP32_MQTT_HPP
#define ESP32_MQTT_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
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
    const char *MQTT_Server = "74f78261a2504f078425eb1b85f3eaed.s1.eu.hivemq.cloud";
    const int16_t MQTT_PORT = 8883;
    char MQTT_USERNAME[64] = "IoT_TEST";
    char MQTT_PASS[64] = "mt21062005";

    char MQTT_BASE_TOPIC[64] = BASE_TOPIC;
    char _mac[32];

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
    if (mqtt_userName != NULL)
    {
        strncpy(MQTT_USERNAME, mqtt_userName, sizeof(MQTT_USERNAME) - 1);
        MQTT_USERNAME[sizeof(MQTT_USERNAME) - 1] = '\0';
    }
    if (mqtt_pass != NULL)
    {
        strncpy(MQTT_PASS, mqtt_pass, sizeof(MQTT_PASS) - 1);
        MQTT_PASS[sizeof(MQTT_PASS) - 1] = '\0';
    }
    String MAC = WiFi.macAddress();
    strncpy(_mac, MAC.c_str(), sizeof(_mac) - 1);
    _mac[sizeof(_mac) - 1] = '\0';
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

    String MAC = WiFi.macAddress();
    strncpy(_mac, MAC.c_str(), sizeof(_mac) - 1);
    _mac[sizeof(_mac) - 1] = '\0';

    Time_connect_MQTT = millis();
    server.stop();
    disconnect();
    vTaskDelay(pdMS_TO_TICKS(100));

    // Cấu hình kết nối bảo mật TLS/SSL cho HiveMQ Cloud port 8883
    server.setInsecure(); // Bỏ qua xác thực chứng chỉ CA
    mqttClient.setServer(MQTT_Server, MQTT_PORT);
    mqttClient.setBufferSize(512); // Đảm bảo buffer đủ chứa JSON telemetry
    mqttClient.setKeepAlive(15);
    mqttClient.setSocketTimeout(5);
    mqttClient.setCallback(IoT_Callback);

    char clientId[32];
    snprintf(clientId, sizeof(clientId), "ESP32_%08X", (uint32_t)ESP.getEfuseMac());

    LOG_MQTT("MQTT", "CONNECTING TO HIVEMQ CLOUD (%s:%d)...", MQTT_Server, MQTT_PORT);
    LOG_MQTT("MQTT", "CLIENT ID: %s | USER: %s", clientId, MQTT_USERNAME);
    while (WiFi.status() == WL_CONNECTED && !mqttClient.connected() && (millis() - Time_connect_MQTT <= Timeout_MQTT))
    {
        // HiveMQ Cloud bắt buộc phải có Client ID (không được để chuỗi rỗng)
        bool connected = mqttClient.connect(clientId, MQTT_USERNAME, MQTT_PASS);

        if (connected)
        {
            snprintf(MQTT_BASE_TOPIC, sizeof(MQTT_BASE_TOPIC), "%s%s", BASE_TOPIC, _mac);
            this->SubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_TELEMETRY_TOPIC);
            this->SubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_CONTROL_TOPIC);
            LOG_MQTT("MQTT", "CONNECTED TO HIVEMQ CLOUD SUCCESSFULLY!");
            return;
        }
        LOG_MQTT("MQTT", "TRYING TO CONNECT... (state=%d)", mqttClient.state());

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    if (!mqttClient.connected())
    {
        LOG_ERROR("MQTT", "CONNECT TIMEOUT OR FAILED, rc=%d", mqttClient.state());
    }
}

template <class MQTT>
inline void MQTTESP32<MQTT>::run()
{
    mqttClient.loop();
}

MQTTESP32<PubSubClient> serverMQTT;

#endif /*ESP32_MQTT_HPP*/
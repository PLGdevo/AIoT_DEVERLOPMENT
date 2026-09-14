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
class AIoT_MQTT_ESP32
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

template <class MQTT>
using MQTTESP32 = AIoT_MQTT_ESP32<MQTT>;

WiFiClientSecure server;
PubSubClient mqttClient(server);
AIoT_MQTT_ESP32<PubSubClient> mqtt;

void AIoT_Callback(char *topic, byte *payload, unsigned int length)
{
    char *msg = (char *)malloc(length + 1);
    if (!msg)
        return;
    memcpy(msg, payload, length);
    msg[length] = '\0';
    LOG_MQTT("MQTT_RECV", "[%s]: %s", topic, msg);
    API_MESS.handleMessage(topic, msg);
    free(msg);
}

template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::SubscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.subscribe(NameTopic);
    LOG_MQTT("MQTT_SUB", "Subscribed: %s", NameTopic);
}
template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::UnsubscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.unsubscribe(NameTopic);
}

/* Publish Topic */
template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::PublishTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
}
template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::UnpublishTopic(const char *baseTopic, const char *Topic_ne)
{
}

template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::PublishData_tele(const char *data)
{
    if (_mac[0] == '\0')
    {
        String MAC = WiFi.macAddress();
        strncpy(_mac, MAC.c_str(), sizeof(_mac) - 1);
        _mac[sizeof(_mac) - 1] = '\0';
    }
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s/%s", BASE_TOPIC, _mac, PUB_PREFIX_TELEMETRY_TOPIC);
    bool ok = mqttClient.publish(NameTopic, data);
    LOG_MQTT("TELEMETRY", "PUBLISH -> [%s]: %s (status: %s)", NameTopic, data, ok ? "OK" : "FAIL");
    mqttClient.loop();
    delay(20);
}
template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::PublishData_control(const char *data)
{
    if (_mac[0] == '\0')
    {
        String MAC = WiFi.macAddress();
        strncpy(_mac, MAC.c_str(), sizeof(_mac) - 1);
        _mac[sizeof(_mac) - 1] = '\0';
    }
    char NameTopic[128];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s/%s", BASE_TOPIC, _mac, PUB_PREFIX_CONTROL_TOPIC);
    bool ok = mqttClient.publish(NameTopic, data);
    LOG_MQTT("CONTROL", "PUBLISH -> [%s]: %s (status: %s)", NameTopic, data, ok ? "OK" : "FAIL");
    mqttClient.loop();
    delay(20);
}

template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::config(const char *mqtt_userName, const char *mqtt_pass)
{
    if (mqtt_userName != NULL && strlen(mqtt_userName) > 0)
    {
        strncpy(MQTT_USERNAME, mqtt_userName, sizeof(MQTT_USERNAME) - 1);
        MQTT_USERNAME[sizeof(MQTT_USERNAME) - 1] = '\0';
    }
    if (mqtt_pass != NULL && strlen(mqtt_pass) > 0)
    {
        strncpy(MQTT_PASS, mqtt_pass, sizeof(MQTT_PASS) - 1);
        MQTT_PASS[sizeof(MQTT_PASS) - 1] = '\0';
    }
    String MAC = WiFi.macAddress();
    strncpy(_mac, MAC.c_str(), sizeof(_mac) - 1);
    _mac[sizeof(_mac) - 1] = '\0';
}

template <class MQTT>
inline bool AIoT_MQTT_ESP32<MQTT>::check_connect()
{
    return mqttClient.connected();
}

template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::disconnect()
{
    snprintf(MQTT_BASE_TOPIC, sizeof(MQTT_BASE_TOPIC), "%s%s", BASE_TOPIC, _mac);
    this->UnsubscribeTopic(MQTT_BASE_TOPIC, SUB_PREFIX_CONTROL_TOPIC);
    mqttClient.disconnect();
    delay(100);
}

template <class MQTT>
inline void AIoT_MQTT_ESP32<MQTT>::begin()
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
    mqttClient.setCallback(AIoT_Callback);

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
            // ESP32 chỉ subscribe topic control để nhận lệnh từ Cloud (không tự subscribe telemetry để tránh echo loop)
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
inline void AIoT_MQTT_ESP32<MQTT>::run()
{
    mqttClient.loop();
}

AIoT_MQTT_ESP32<PubSubClient> serverMQTT;

#endif /*ESP32_MQTT_HPP*/
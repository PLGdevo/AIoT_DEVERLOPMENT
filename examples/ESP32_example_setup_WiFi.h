/*
AIoT Example
Example: Setup WiFi + MQTT Connection
Designed by AIoT
*/

#define DEBUG_COLOR
#define BUTTON_CONFIG

#include <AIoT.h>

//=========================//
//    WIFI CONFIGURATION  //
//=========================//
const char *WIFI_SSID = "YOUR_WIFI_NAME";
const char *WIFI_PASS = "YOUR_WIFI_PASSWORD";

//========================//
//   MQTT CONFIGURATION  //
//======================//
const char *MQTT_USER = "YOUR_MQTT_USERNAME";
const char *MQTT_PASS = "YOUR_MQTT_PASSWORD";

void setup()
{
    Serial.begin(115200);

    // Initialize AIoT
    AIoT.begin(
        WIFI_SSID,
        WIFI_PASS,
        MQTT_USER,
        MQTT_PASS);
}

void loop()
{
    // Run AIoT background tasks
    AIoT.run();
}
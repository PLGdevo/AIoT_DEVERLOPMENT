#define DEBUG_COLOR
#define BUTTON_CONFIG
#include <TZIoT.h>
#include <TZ_KIT.h>
const char *WIFI_SSID = "MakerSpaceLab_2.4Ghz";
const char *WIFI_PASS = "Maker2025";

const char *MQTT_USER = "0309231068@caothang.edu.vn";
const char *MQTT_PASS = "4dfAudwntoocVw6rJU7B";

void ledWiFi()
{
    if (TZIoT.CheckConnect())
    {
        analogWrite(Statusled, 10);
    }
    else
    {
        digitalWrite(Statusled, LOW);
    }
}

int programButton_ENTER_BACK()
{
    static bool lastStateButton = LOW;
    static unsigned long timepressButton = 0;
    static bool HoldDone = false;
    static int eventButton = 0;
    bool button = digitalRead(BUTTON2);
    if (button == HIGH && lastStateButton == LOW)
    {
        timepressButton = millis();
        HoldDone = false;
    }
    if (button == HIGH)
    {
        if (!HoldDone &&
            millis() - timepressButton >= 2000)
        {
            HoldDone = true;

            eventButton = 2;
        }
    }
    if (button == LOW &&
        lastStateButton == HIGH)
    {
        if (!HoldDone)
        {
            eventButton = 1;
        }
    }
    lastStateButton = button;
    int key = eventButton;
    eventButton = 0;
    return key;
}
int programButton_UP_DOWN()
{
    static unsigned long lastButton = 0;
    if (millis() - lastButton < 200)
        return 0;
    if (digitalRead(BUTTON1))
    {
        lastButton = millis();
        return 4; // UP
    }
    if (digitalRead(BUTTON3))
    {
        lastButton = millis();
        return 3; // DOWN
    }
    return 0;
}

// PROGRAM SCREEN BY FUNCTION
#include <TZ_SCREEN.h>
void programScreen()
{
    static bool startup = true;
    static unsigned long timestart = 0;
    if (startup)
    {
        if (timestart == 0)
        {
            timestart = millis();
            Screen_CONNECT();
        }
        if (millis() - timestart < 1500)
        {
            return;
        }
        startup = false;
        lcd.clear();
    }
    if (!startup)
    {
        SCREEN();
    }
}

// PROGRAM SYSTEM BY FUNCTION
void programSystem()
{
}

void setup()
{
    Serial.begin(115200);
    trans_Screen = 0;
    Wire.begin(SDA_, SCL_);
    lcd.init();
    lcd.backlight();
    Screen_STARTUP();
    Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
    // Cài chân cho cảm biến
    for (int i = 0; i < 4; i++)
    {
        pinMode(SENSOR[i], INPUT);
        delay(200);
    }
    // Cài chân ngõ ra tải && Cấu hình PWM
    for (int i = 0; i < 4; i++)
    {
        pinMode(OUT[i], OUTPUT);
        digitalWrite(OUT[i], LOW);
        delay(200);
    }
    // Cài nút bấm
    for (int i = 0; i < 3; i++)
    {
        pinMode(BUTTON[i], INPUT);
        delay(200);
    }
    pinMode(Statusled, OUTPUT);
    digitalWrite(Statusled, LOW);
    pinMode(Buzzer, OUTPUT);
    digitalWrite(Buzzer, LOW);

    TZIoT.begin(WIFI_SSID, WIFI_PASS, MQTT_USER, MQTT_PASS);
    TZModbus.beginModbus(Serial2, 4800, RXD2, TXD2, SERIAL_8N1);
    TZModbus.setTimeout(1000);
}
void loop()
{
    ledWiFi();
    TZIoT.run();
    timeprocess = millis();
    if (timeprocess - lasttimeprocess >= 10)
    {
        lasttimeprocess = timeprocess;
        programScreen();
        programSystem();
    }
}
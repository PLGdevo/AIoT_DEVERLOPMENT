#include <Arduino.h>
#include <PZEM004Tv30.h>

#define RXD2 18
#define TXD2 19

PZEM004Tv30 pzem(Serial2, RXD2, TXD2);

void setup()
{
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

    Serial.println("PZEM V3 START");
}

void loop()
{
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();

    if (isnan(voltage))
    {
        Serial.println("FAIL / NO DATA");
    }
    else
    {
        Serial.print("V: ");
        Serial.print(voltage);
        Serial.print(" V | ");
        Serial.print("I: ");
        Serial.print(current);
        Serial.print(" A | ");
        Serial.print("P: ");
        Serial.print(power);
        Serial.print(" W | ");
        Serial.print("E: ");
        Serial.print(energy);
        Serial.println(" kWh");
    }

    delay(1000);
}
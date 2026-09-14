/*
 * AIoT Example
 * Example: Setup MODBUS + Communicate MODBUS
 * Designed by AIoT
 */

#define DEBUG_COLOR
#define BUTTON_CONFIG

#include <AIoT.h>

/*=========================
    MODBUS CONFIGURATION
=========================*/
#define RX_ 16         // RX pin of RS485 Module
#define TX_ 17         // TX pin of RS485 Module
#define BAUD_RATE 9600 // Modbus baudrate

float Humidity = 0;
float Temperature = 0;

/*=========================
    READ SENSOR
=========================*/
void readSensor()
{
    uint16_t DATA[2];

    int RS = TZModbus.readHoldingRegisterValue(
        1,      // Slave ID
        0x0000, // Start Register
        2,      // Number of Registers
        DATA);  // Return Buffer

    if (RS > 0)
    {
        Humidity = DATA[0] / 10.0;
        Temperature = DATA[1] / 10.0;

        Serial.println("Humidity: " + String(Humidity) + " %");
        Serial.println("Temperature: " + String(Temperature) + " C");
    }
    else
    {
        Serial.print("MODBUS ERROR: ");
        Serial.println(RS);
    }
}

void setup()
{
    Serial.begin(115200);

    TZModbus.beginModbus(
        Serial2,
        BAUD_RATE,
        RX_,
        TX_,
        SERIAL_8N1);

    Serial.println("MODBUS START");
}

void loop()
{
    readSensor();

    delay(1000);
}
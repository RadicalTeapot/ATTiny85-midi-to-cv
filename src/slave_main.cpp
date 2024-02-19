#ifdef SLAVE
#include <Arduino.h>
#include <Wire.h>

#define LATCH_PIN (1) // Connect to ST_CP (RCLK, pin 12)
#define CLOCK_PIN (3) // Connect to SH_CP (SRCLK, pin 11)
#define DATA_PIN (4)  // Connect to DS (SER, pin 14)

void receiveCallback(uint8_t data);
void handleReceive(int len);

void setup()
{
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);

    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(handleReceive);
}

void loop() {}

void handleReceive(int len)
{
    uint8_t data = 0;
    while (Wire.available())
    {
        data = Wire.read();

        digitalWrite(LATCH_PIN, LOW);
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
        digitalWrite(LATCH_PIN, HIGH);
    }
}

#endif

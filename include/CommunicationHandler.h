#include "Arduino.h"
#include "Wire.h"

namespace CommunicationHandler
{
    void init(TwoWire *i2c = &Wire)
    {
        i2c->begin();
    }

    template <uint8_t address>
    void sendData(uint8_t data, TwoWire *i2c = &Wire)
    {
        i2c->beginTransmission(address);
        i2c->write(data);
        i2c->endTransmission();
    }
}

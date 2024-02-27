#ifndef MidiSerialCommunication_h
#define MidiSerialCommunication_h

#include <Arduino.h>
#include <ReceiveOnlySoftwareSerial.h>
#include "MidiParser.h"

namespace MidiSerialCommunication
{
    template <uint16_t baudRate>
    void begin(ReceiveOnlySoftwareSerial *serial)
    {
        serial->begin(baudRate);
    }

    bool recv(MidiEventLib::Event *MidiEventLib::Event, ReceiveOnlySoftwareSerial *serial, Parser *parser)
    {
        if (serial->available())
        {
            return parser->parse(serial->read(), MidiEventLib::Event);
        }
        return false;
    }
}

#endif // MidiSerialCommunication_h

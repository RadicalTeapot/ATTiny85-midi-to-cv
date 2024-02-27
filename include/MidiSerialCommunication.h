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

    bool recv(MidiEventLib::Event *midiEvent, ReceiveOnlySoftwareSerial *serial, MidiEventLib::Parser *parser)
    {
        if (serial->available())
        {
            return parser->parse(serial->read(), midiEvent);
        }
        return false;
    }
}

#endif // MidiSerialCommunication_h

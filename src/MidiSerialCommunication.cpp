#ifdef ARDUINO
#include "MidiSerialCommunication.h"

#define BAUD_RATE 31250

MidiSerialCommunication::MidiSerialCommunication(ReceiveOnlySoftwareSerial *serial, MidiParser *parser)
{
    _serial = serial;
    _parser = parser;
}

void MidiSerialCommunication::begin()
{
    _serial->begin(BAUD_RATE);
}

bool MidiSerialCommunication::recv(MidiEvent *midiEvent)
{
    if (_serial->available())
    {
        return _parser->parse(_serial->read(), midiEvent);
    }
    return false;
}
#endif

#ifndef MidiSerialCommunication_h
#define MidiSerialCommunication_h

#include <Arduino.h>
#include <ReceiveOnlySoftwareSerial.h>
#include "MidiParser.h"

class MidiSerialCommunication
{
public:
    MidiSerialCommunication(ReceiveOnlySoftwareSerial *serial, MidiParser *parser);
    void begin();
    bool recv(MidiEvent *midiEvent);
private:
    ReceiveOnlySoftwareSerial *_serial;
    MidiParser *_parser;
};

#endif // MidiSerialCommunication_h

#ifndef MIDIPARSER_H
#define MIDIPARSER_H

#include "MidiEvent.h"
#include <ReceiveOnlySoftwareSerial.h>

class MidiParser {
public:
    typedef struct _MIDI_DATA
    {
        MIDIEventType type;
        uint8_t note;
        uint8_t velocity;
    } MIDI_DATA;

    MidiParser(ReceiveOnlySoftwareSerial *serial, uint8_t midiChannel = 1);
    void begin();
    bool recv(MIDI_DATA *data);
private:
    ReceiveOnlySoftwareSerial *_serial;

    uint8_t _lastMidiByte;
    uint8_t _midiChannel;
    MIDI_DATA _lastMidiData;

    void parseMidiCommand(uint8_t midiByte);
    bool parseMidiData(uint8_t midiByte);

    inline bool isMidiCommand(uint8_t midiByte) {
        return (midiByte & MIDI_COMMAND_MASK) == MIDI_COMMAND_MASK;
    }
    inline bool isMidiSystemCommand(uint8_t midiByte) {
        return (midiByte & MIDI_CHANNEL_SYSTEM_COMMAND_MASK) == MIDI_CHANNEL_SYSTEM_COMMAND_MASK;
    }
    inline bool isMatchingMidiOn() {
        return _lastMidiByte == (MIDI_ON_MASK | _midiChannel);
    }
    inline bool isMatchingMidiOff() {
        return _lastMidiByte == (MIDI_OFF_MASK | _midiChannel);
    }
    inline bool isMatchingMidiCC() {
        return _lastMidiByte == (MIDI_CONTROL_CHANGE_MASK | _midiChannel);
    }

    inline void resetMidiData() {
        _lastMidiData.type = MIDIEventType::INVALID;
        _lastMidiData.note = 0;
        _lastMidiData.velocity = 0;
    }
    inline void resetLastMidiByte() {
        _lastMidiByte = 0;
    }
};

#endif

#ifndef MidiParser_h
#define MidiParser_h

#include "MidiEvent.h"
#include <ReceiveOnlySoftwareSerial.h>

class MidiParser {
public:
    MidiParser(ReceiveOnlySoftwareSerial *serial, uint8_t midiChannel = 1);
    void begin();
    bool recv(MidiEvent *midiEvent);
private:
    ReceiveOnlySoftwareSerial *_serial;

    uint8_t _lastMidiCommand;
    uint8_t _midiData[2];

    void parseMidiCommand(uint8_t midiByte);
    bool parseMidiData(uint8_t midiByte, MidiEvent *midiEvent);

    inline uint8_t getMidiChannel() {
        return _lastMidiCommand & MIDI_CHANNEL_COMMAND_MASK;
    }

    inline bool isMidiCommand(uint8_t midiByte) {
        return (midiByte & MIDI_COMMAND_MASK) == MIDI_COMMAND_MASK;
    }
    inline bool isMidiSystemCommand(uint8_t midiByte) {
        return (midiByte & MIDI_CHANNEL_SYSTEM_COMMAND_MASK) == MIDI_CHANNEL_SYSTEM_COMMAND_MASK;
    }
    inline bool isMidiNoteOn() {
        return (_lastMidiCommand & MIDI_ON_MASK) == MIDI_ON_MASK;
    }
    inline bool isMidiNoteOff() {
        return (_lastMidiCommand & MIDI_OFF_MASK) == MIDI_OFF_MASK;
    }
    inline bool isMidiCC() {
        return (_lastMidiCommand & MIDI_CONTROL_CHANGE_MASK) == MIDI_CONTROL_CHANGE_MASK;
    }

    inline void resetMidiData() {
        _lastMidiCommand = MidiEventType::INVALID;
        _midiData[0] = 0;
        _midiData[1] = 0;
    }

    inline void resetLastMidiByte() {
        _lastMidiCommand = 0;
    }
};

#endif // MidiParser_h

#ifndef MIDIRECV_H
#define MIDIRECV_H

#include <ReceiveOnlySoftwareSerial.h>

#define MIDI_COMMAND_MASK (0x80)
#define MIDI_CHANNEL_SYSTEM_COMMAND_MASK (0xF0)
#define MIDI_ON_MASK (0x90)
#define MIDI_OFF_MASK (0x80)
#define MIDI_CONTROL_CHANGE_MASK (0xB0)

class MidiRecv {
public:
    enum EVENT_TYPE {
        EVENT_NOTE_ON,
        EVENT_NOTE_OFF,
        EVENT_CONTROL_CHANGE,
        EVENT_NONE
    };

    typedef struct _MIDI_DATA
    {
        EVENT_TYPE type;
        uint8_t note;
        uint8_t velocity;
    } MIDI_DATA;

    MidiRecv(ReceiveOnlySoftwareSerial *serial, uint8_t midiChannel = 1);
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
        _lastMidiData.type = EVENT_NONE;
        _lastMidiData.note = 0;
        _lastMidiData.velocity = 0;
    }
    inline void resetLastMidiByte() {
        _lastMidiByte = 0;
    }
};

#endif

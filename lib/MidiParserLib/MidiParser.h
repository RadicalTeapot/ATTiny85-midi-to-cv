#ifndef MidiParser_h
#define MidiParser_h

#include "MidiEvent.h"

class MidiParser
{
public:
    bool parse(uint8_t midiByte, MidiEvent *midiEvent);

private:
    uint8_t _lastMidiCommand;
    uint8_t _midiData[2];

    void parseMidiCommand(uint8_t midiByte);
    bool parseMidiData(uint8_t midiByte, MidiEvent *midiEvent);

    inline uint8_t getMidiChannel()
    {
        return _lastMidiCommand & MIDI_CHANNEL_COMMAND_MASK;
    }

    inline bool isMidiCommand(uint8_t midiByte)
    {
        return midiByte >> 7 == 1;
    }
    inline bool isMidiSystemCommand(uint8_t midiByte)
    {
        return (midiByte & MIDI_COMMAND_MASK) == MIDI_CHANNEL_SYSTEM_COMMAND;
    }
    inline bool isMidiNoteOn()
    {
        return (_lastMidiCommand & MIDI_COMMAND_MASK) == MIDI_NOTE_ON;
    }
    inline bool isMidiNoteOff()
    {
        return (_lastMidiCommand & MIDI_COMMAND_MASK) == MIDI_NOTE_OFF;
    }
    inline bool isMidiCC()
    {
        return (_lastMidiCommand & MIDI_COMMAND_MASK) == MIDI_CONTROL_CHANGE;
    }

    inline void resetMidiData()
    {
        _midiData[0] = 0;
        _midiData[1] = 0;
    }

    inline void resetLastMidiCommand()
    {
        _lastMidiCommand = MidiEventType::INVALID;
    }
};

#endif // MidiParser_h

#ifndef MidiParser_h
#define MidiParser_h

#include "MidiEvent.h"

namespace MidiEventLib
{
    class Parser
    {
    public:
        bool parse(uint8_t midiByte, Event *midiEvent);

    private:
        uint8_t _lastMidiCommand = 0;
        uint8_t _midiData[2] = {0, 0};

        void parseMidiCommand(uint8_t midiByte);
        bool parseMidiData(uint8_t midiByte, Event *midiEvent);

        inline uint8_t getMidiChannel()
        {
            return _lastMidiCommand & CHANNEL_COMMAND_MASK;
        }

        inline bool isMidiCommand(uint8_t midiByte)
        {
            return midiByte >> 7 == 1;
        }
        inline bool isMidiSystemCommand(uint8_t midiByte)
        {
            return (midiByte & COMMAND_MASK) == CHANNEL_SYSTEM_COMMAND;
        }
        inline bool isMidiNoteOn()
        {
            return (_lastMidiCommand & COMMAND_MASK) == NOTE_ON;
        }
        inline bool isMidiNoteOff()
        {
            return (_lastMidiCommand & COMMAND_MASK) == NOTE_OFF;
        }
        inline bool isMidiCC()
        {
            return (_lastMidiCommand & COMMAND_MASK) == CONTROL_CHANGE_COMMAND;
        }

        inline void resetMidiData()
        {
            _midiData[0] = 0;
            _midiData[1] = 0;
        }

        inline void resetLastMidiCommand()
        {
            _lastMidiCommand = EventType::INVALID;
        }
    };
}

#endif // MidiParser_h

#ifndef StaticNotesHandler_h
#define StaticNotesHandler_h

#include "MidiEvent.h"
#include "ShiftRegisterEventHandler.h"

#define MIDI_CHANNEL 0x00

template <uint8_t size, uint8_t offset>
class StaticNotesHandler : public ShiftRegisterEventHandler
{
public:
    StaticNotesHandler(const uint8_t notes[size])
    {
        for (uint8_t i = 0; i < size; ++i)
        {
            _notes[i] = notes[i];
        }
    }

    bool handleEvent(const MidiEvent *event, uint8_t *values)
    {
        if (!isHandleEvent(event))
        {
            return false;
        }

        uint8_t note = event->firstByte;
        for (uint8_t i = 0; i < size; ++i)
        {
            if (note != _notes[i])
            {
                continue;
            }

            if (event->type == MidiEventType::NOTE_ON)
            {
                *values |= 1 << (offset + i);
                return true;
            }
            else // Note off
            {
                *values &= ~(1 << (offset + i));
                return true;
            }
        }

        return false;
    }

private:
    uint8_t _notes[size];
    inline bool isHandleEvent(const MidiEvent *event)
    {
        return event->channel == MIDI_CHANNEL && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF);
    }
};

#endif // StaticNotesHandler_h

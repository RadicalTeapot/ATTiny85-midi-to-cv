#ifndef ShiftRegisterNoteEventHandler_h
#define ShiftRegisterNoteEventHandler_h

#include "MidiEvent.h"
#include "ShiftRegisterEventHandler.h"

template <uint8_t midiChannel, uint8_t note>
class ShiftRegisterNoteEventHandler : public ShiftRegisterEventHandler
{
public:
    bool handleEvent(const MidiEvent *event)
    {
        if (shouldHandleEvent(event))
        {
            _currentNoteState = event->type == MidiEventType::NOTE_ON;
        }
        return _currentNoteState;
    }

private:
    bool _currentNoteState = false;
    inline bool shouldHandleEvent(const MidiEvent *event)
    {
        return event->channel == midiChannel && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF) && event->firstByte == note;
    }
};

#endif // ShiftRegisterNoteEventHandler_h

#ifndef ShiftRegisterNoteEventHandler_h
#define ShiftRegisterNoteEventHandler_h

#include "MidiEvent.h"
#include "ShiftRegisterEventHandler.h"

template <uint8_t midiChannel>
class ShiftRegisterNoteEventHandler : public ShiftRegisterEventHandler
{
public:
    ShiftRegisterNoteEventHandler(uint8_t note) : _note(note) {}
    bool processEvent(const MidiEvent *event)
    {
        if (shouldHandleEvent(event))
        {
            _currentNoteState = event->type == MidiEventType::NOTE_ON;
        }
        return _currentNoteState;
    }

private:
    uint8_t _note;
    bool _currentNoteState = false;
    inline bool shouldHandleEvent(const MidiEvent *event) const
    {
        return event->channel == midiChannel && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF) && event->firstByte == _note;
    }
};

#endif // ShiftRegisterNoteEventHandler_h

#ifndef ShiftRegisterNoteEventHandler_h
#define ShiftRegisterNoteEventHandler_h

#include "MidiEvent.h"
#include "ShiftRegisterEventHandler.h"

template <uint8_t midiChannel>
class ShiftRegisterNoteEventHandler : public ShiftRegisterEventHandler
{
public:
    ShiftRegisterNoteEventHandler(uint8_t note) : _note(note) {}
private:
    uint8_t _note;
    inline bool shouldProcessEvent(const MidiEvent *event) const override
    {
        return event->channel == midiChannel && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF) && event->firstByte == _note;
    }
};

#endif // ShiftRegisterNoteEventHandler_h

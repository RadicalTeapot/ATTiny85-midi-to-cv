#ifndef ShiftRegisterEventHandler_h
#define ShiftRegisterEventHandler_h

#include "MidiEvent.h"

class ShiftRegisterEventHandler {
public:
    virtual ~ShiftRegisterEventHandler() {}
    inline bool processEvent(const MidiEvent *event, const uint8_t *result) const
    {
        if (shouldProcessEvent(event))
            return event->type == MidiEventType::NOTE_ON;
        return *result == 1;
    }
private:
    virtual inline bool shouldProcessEvent(const MidiEvent *event) const
    {
        return false;
    }
};

#endif // ShiftRegisterEventHandler_h

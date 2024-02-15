#ifndef ShiftRegisterEventHandler_h
#define ShiftRegisterEventHandler_h

#include "MidiEvent.h"

class ShiftRegisterEventHandler {
public:
    virtual ~ShiftRegisterEventHandler() {}
    virtual bool processEvent(const MidiEvent *event) = 0;
};

#endif // ShiftRegisterEventHandler_h

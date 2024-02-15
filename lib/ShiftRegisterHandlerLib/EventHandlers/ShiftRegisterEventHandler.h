#ifndef ShiftRegisterEventHandler_h
#define ShiftRegisterEventHandler_h

#include "MidiEvent.h"

class ShiftRegisterEventHandler {
public:
    virtual bool handleEvent(const MidiEvent *event) = 0;
};

#endif // ShiftRegisterEventHandler_h

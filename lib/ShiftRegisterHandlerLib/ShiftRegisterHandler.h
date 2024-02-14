#ifndef ShiftRegisterHandler_h
#define ShiftRegisterHandler_h

#include "MidiEvent.h"
#include "EventHandlers/ShiftRegisterEventHandler.h"

#define HANDLER_COUNT 3

class ShiftRegisterHandler
{
public:
    typedef void (*WriteValuesToShiftRegister)(uint8_t values);
    static WriteValuesToShiftRegister writeValuesToShiftRegister;

    // TODO set event handlers
    bool handleEvent(const MidiEvent *event);
private:
    ShiftRegisterEventHandler *_handlers;
    uint8_t shiftRegisterValues = 0B00000000;

    static void defaultWriteValuesToShiftRegister(uint8_t values) {}
};

#endif // ShiftRegisterHandler_h

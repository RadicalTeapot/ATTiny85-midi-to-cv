#ifndef DacHandler_h
#define DacHandler_h

#include "MidiEvent.h"
#include "DacValues.h"
#include "eventHandlers/DacEventHandler.h"

class DacHandler
{
public:
    typedef void (*WriteValuesToDac)(DacValues *dacValues);

    DacHandler(WriteValuesToDac writeValuesToDac = defaultWriteValuesToDac) : _writeValuesToDac(writeValuesToDac) {};
    ~DacHandler();
    void setHandler(DacEventHandler *handler);
    bool handleEvent(const MidiEvent *event);
private:
    DacEventHandler *_handler = NULL;
    DacValues _dacValues;

    WriteValuesToDac _writeValuesToDac;
    static void defaultWriteValuesToDac(DacValues *dacValues) {}
};

#endif // DacHandler_h

#ifndef DacEventHandler_h
#define DacEventHandler_h

#include "MidiEvent.h"
#include "DacValues.h"

typedef void (*DacValueMapper)(const uint8_t[4], DacValues *);

class DacEventHandler
{
public:
    DacEventHandler(DacValueMapper valueMapper): _valueMapper(valueMapper) {}
    virtual ~DacEventHandler() {}
    virtual bool handleEvent(const MidiEvent *event, DacValues *dacValues) = 0;
protected:
    DacValueMapper _valueMapper;
};

#endif // DacEventHandler_h

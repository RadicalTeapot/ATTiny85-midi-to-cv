#ifndef DacEventHandler_h
#define DacEventHandler_h

#include "MidiEvent.h"
#include "DacValues.h"

class DacEventHandler {
    public:
        virtual ~DacEventHandler() {}
        virtual bool handleEvent(const MidiEvent *event, DacValues *dacValues) = 0;
};

#endif // DacEventHandler_h

#ifndef DacEventHandler_h
#define DacEventHandler_h

#include <MIDIEvent.h>
#include "../DacValues.h"

class DacEventHandler {
    public:
        virtual ~DacEventHandler() {}
        virtual bool handleEvent(MidiEvent *event, DacValues *dacValues) = 0;
};

#endif // DacEventHandler_h

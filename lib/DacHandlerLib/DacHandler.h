#ifndef DacHandler_h
#define DacHandler_h

#include <MIDIEvent.h>
#include "DacValues.h"
#include "eventHandlers/DacEventHandler.h"

class DacHandler {
    public:
        ~DacHandler();
        void setHandler(DacEventHandler *handler);
        bool handleEvent(MIDIEvent *event);
    private:
        DacEventHandler *_handler = NULL;
        DacValues _dacValues;
        void writeValuesToDac();
};

#endif // DacHandler_h

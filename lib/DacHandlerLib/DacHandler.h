#ifndef DacHandler_h
#define DacHandler_h

#include <MIDIEvent.h>
#include "DacValues.h"
#include "eventHandlers/DacEventHandler.h"


class DacHandler {
    public:
        typedef void (*WriteValuesToDac)(DacValues *dacValues);
        static WriteValuesToDac writeValuesToDac;

        ~DacHandler();
        void setHandler(DacEventHandler *handler);
        bool handleEvent(MIDIEvent *event);
    private:
        DacEventHandler *_handler = NULL;
        DacValues _dacValues;

        static void defaultWriteValuesToDac(DacValues *dacValues) {}
};

#endif // DacHandler_h

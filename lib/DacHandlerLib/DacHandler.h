#ifndef DacHandler_h
#define DacHandler_h

#include "MidiEvent.h"
#include "DacValues.h"
#include "DacEventHandlerContainer.h"

class DacHandler
{
public:
    using WriteValuesToDac = void(*)(DacValues *dacValues);

    DacHandler(WriteValuesToDac writeValuesToDac = defaultWriteValuesToDac) : _writeValuesToDac(writeValuesToDac) {};
    void configure(const DacPresetConfig *dacConfig, bool isNoteHandler) { _handlerContainer.configureHandlers(dacConfig, isNoteHandler); }
    void handleEvent(const MidiEventLib::Event *event);
private:
    DacEventHandlerContainer _handlerContainer;
    DacValues _dacValues;

    WriteValuesToDac _writeValuesToDac;
    static void defaultWriteValuesToDac(DacValues *dacValues) {}
};

#endif // DacHandler_h

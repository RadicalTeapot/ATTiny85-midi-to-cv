#ifndef ShiftRegisterHandler_h
#define ShiftRegisterHandler_h

#include "Preset.h"
#include "MidiEvent.h"
#include "ShiftRegisterEventHandlerContainer.h"

namespace
{
    void defaultWriteValuesToShiftRegister(uint8_t values) {}
}

class ShiftRegisterHandler
{
public:
    using WriteValuesToShiftRegister = void(*)(uint8_t values);

    ShiftRegisterHandler(WriteValuesToShiftRegister writeValuesToShiftRegister = defaultWriteValuesToShiftRegister): _writeValuesToShiftRegister(writeValuesToShiftRegister) {}
    void updateHandlersFromFirstDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler);
    void updateHandlersFromSecondDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler);
    void processEvent(const MidiEventLib::Event *event);

private:
    uint8_t _state = 0;
    ShiftRegisterEventHandlerContainer _handlerContainer;
    WriteValuesToShiftRegister _writeValuesToShiftRegister;
};

#endif // ShiftRegisterHandler_h

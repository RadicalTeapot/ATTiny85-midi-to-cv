#ifndef ShiftRegisterHandler_h
#define ShiftRegisterHandler_h

#include "Preset.h"
#include "MidiEvent.h"
#include "ShiftRegisterEventHandlerContainer.h"

class ShiftRegisterHandler
{
public:
    using WriteValuesToShiftRegister = void(*)(uint8_t values);
    static WriteValuesToShiftRegister writeValuesToShiftRegister;

    void updateHandlersFromFirstDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler);
    void updateHandlersFromSecondDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler);
    void processEvent(const MidiEventLib::Event *event);

private:
    uint8_t _state = 0;
    ShiftRegisterEventHandlerContainer _handlerContainer;
    static void defaultWriteValuesToShiftRegister(uint8_t values) {}
};

#endif // ShiftRegisterHandler_h

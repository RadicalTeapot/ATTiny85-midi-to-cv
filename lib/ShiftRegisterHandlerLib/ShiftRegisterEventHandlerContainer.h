#ifndef ShiftRegisterEventHandlerContainer_h
#define ShiftRegisterEventHandlerContainer_h

#include "EventHandlers/ShiftRegisterEventHandler.h"
#include "MidiEvent.h"
#include "Preset.h"

const uint8_t HANDLER_COUNT = 8;
const uint8_t DEFAULT_CHANNEL = 0;
const uint8_t DEFAULT_NOTES[HANDLER_COUNT] = {24, 25, 26, 27, 28, 29, 30, 31};

class ShiftRegisterEventHandlerContainer
{
public:
    ShiftRegisterEventHandlerContainer();

    void setFirstHandlersPairFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
    {
        setHandlersFromDacConfig(dacConfig, isNoteHandler, 0);
    }

    void setSecondHandlersPairFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
    {
        setHandlersFromDacConfig(dacConfig, isNoteHandler, 2);
    }

    uint8_t processEvent(const MidiEvent *event, const uint8_t previousState) const;

private:
    ShiftRegisterEventHandler _handlers[HANDLER_COUNT];
    void setHandlersFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler, uint8_t index);
};

#endif // ShiftRegisterEventHandlerContainer_h

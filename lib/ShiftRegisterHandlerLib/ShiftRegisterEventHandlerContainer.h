#ifndef ShiftRegisterEventHandlerContainer_h
#define ShiftRegisterEventHandlerContainer_h

#include "EventHandlers/ShiftRegisterEventHandler.h"
#include "MidiEvent.h"
#include "Preset.h"

#define HANDLER_COUNT 8
#define DEFAULT_CHANNEL 0
#define DEFAULT_NOTES                  \
    {                                  \
        24, 25, 26, 27, 28, 29, 30, 31 \
    }

class ShiftRegisterEventHandlerContainer
{
public:
    ShiftRegisterEventHandlerContainer();
    ~ShiftRegisterEventHandlerContainer();

    void setFirstHandlersPairFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler) {
        setHandlersFromDacConfig(dacConfig, isNoteHandler, 0);
    }
    void setSecondHandlersPairFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler) {
        setHandlersFromDacConfig(dacConfig, isNoteHandler, 2);
    }
    void processEvent(const MidiEvent *event, uint8_t *state) const;

private:
    ShiftRegisterEventHandler *_handlers[HANDLER_COUNT];
    const uint8_t _notes[8] = DEFAULT_NOTES;
    void setHandlersFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler, uint8_t index);
};

#endif // ShiftRegisterEventHandlerContainer_h

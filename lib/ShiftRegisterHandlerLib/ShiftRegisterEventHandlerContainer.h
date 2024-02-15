#ifndef ShiftRegisterEventHandlerContainer_h
#define ShiftRegisterEventHandlerContainer_h

#include "EventHandlers/ShiftRegisterEventHandler.h"
#include "EventHandlers/ShiftRegisterNoteEventHandler.h"
#include "EventHandlers/ShiftRegisterChannelEventHandler.h"
#include "MidiEvent.h"
#include "Preset.h"

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

    void setFirstTwoHandlersFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler);
    void setSecondTwoHandlersFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler);
    uint8_t processEvent(const MidiEvent *event) const;

private:
    ShiftRegisterEventHandler *handlers[8];
    const uint8_t notes[8] = DEFAULT_NOTES;
};

#endif // ShiftRegisterEventHandlerContainer_h

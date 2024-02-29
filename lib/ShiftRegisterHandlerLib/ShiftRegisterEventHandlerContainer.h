#ifndef ShiftRegisterEventHandlerContainer_h
#define ShiftRegisterEventHandlerContainer_h

#include "EventHandlers/ShiftRegisterEventHandler.h"
#include "MidiEvent.h"
#include "Preset.h"

// Local constants
namespace
{
    const uint8_t HANDLER_COUNT = 8;
    const uint8_t NOTE_CHANNEL = 0;
    const uint8_t DEFAULT_NOTES[HANDLER_COUNT] = {24, 25, 26, 27, 28, 29, 30, 31};
}

class ShiftRegisterEventHandlerContainer
{
public:
    ShiftRegisterEventHandlerContainer();

    void setFirstHandlersPairFromDacConfig(const DacPresetConfig *dacConfig, const bool isChannelHandler)
    {
        setHandlersFromDacConfig(dacConfig, isChannelHandler, 0);
    }

    void setSecondHandlersPairFromDacConfig(const DacPresetConfig *dacConfig, const bool isChannelHandler)
    {
        setHandlersFromDacConfig(dacConfig, isChannelHandler, 2);
    }

    uint8_t processEvent(const MidiEventLib::Event *event, const uint8_t previousState) const;

private:
    ShiftRegisterEventHandler<NOTE_CHANNEL> _handlers[HANDLER_COUNT];
    void setHandlersFromDacConfig(const DacPresetConfig *dacConfig, const bool isChannelHandler, const uint8_t index);
};

#endif // ShiftRegisterEventHandlerContainer_h

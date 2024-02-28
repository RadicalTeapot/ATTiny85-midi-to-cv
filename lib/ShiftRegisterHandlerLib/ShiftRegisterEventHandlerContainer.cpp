#include "ShiftRegisterEventHandlerContainer.h"

ShiftRegisterEventHandlerContainer::ShiftRegisterEventHandlerContainer()
{
    uint8_t i = ShiftRegisterEventHandlerContainerConstants::HANDLER_COUNT;
    do
    {
        i--;
        _handlers[i].configure(ShiftRegisterEventHandlerContainerConstants::DEFAULT_NOTES[i], false);
    } while (i);
}

void ShiftRegisterEventHandlerContainer::setHandlersFromDacConfig(const DacPresetConfig *dacConfig, const bool isChannelHandler, const uint8_t index)
{
    if (isChannelHandler)
    {
        _handlers[index].configure(dacConfig->NoteChannels >> 4, true);
        _handlers[index + 1].configure(dacConfig->NoteChannels & 0x0F, true);
    }
    else
    {
        _handlers[index].configure(ShiftRegisterEventHandlerContainerConstants::DEFAULT_NOTES[index], false);
        _handlers[index + 1].configure(ShiftRegisterEventHandlerContainerConstants::DEFAULT_NOTES[index + 1], false);
    }
}

uint8_t ShiftRegisterEventHandlerContainer::processEvent(const MidiEventLib::Event *event, const uint8_t previousState) const
{
    uint8_t maskedPreviousState = 0;
    uint8_t i = 8, mask = 0, result = 0;
    do
    {
        i--;
        mask = 1U << i;
        maskedPreviousState = (previousState >> i) & 1U;
        if (_handlers[i].processEvent(event, maskedPreviousState))
        {
            result |= mask;
        }
    } while (i);

    return result;
}

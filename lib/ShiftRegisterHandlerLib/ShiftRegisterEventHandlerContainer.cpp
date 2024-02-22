#include "ShiftRegisterEventHandlerContainer.h"

ShiftRegisterEventHandlerContainer::ShiftRegisterEventHandlerContainer()
{
    // TODO This should probably be in an init function rather than the constructor
    uint8_t i = HANDLER_COUNT;
    do
    {
        i--;
        _handlers[i] = ShiftRegisterEventHandler(shouldProcessNoteEvent<DEFAULT_CHANNEL>, DEFAULT_NOTES[i]);
    } while (i);
}

void ShiftRegisterEventHandlerContainer::setHandlersFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler, uint8_t index)
{
    if (isNoteHandler)
    {
        _handlers[index].setShouldProcessEvent(shouldProcessChannelEvent, dacConfig->NoteChannels >> 4);
        _handlers[index + 1].setShouldProcessEvent(shouldProcessChannelEvent, dacConfig->NoteChannels & 0x0F);
    }
    else
    {
        _handlers[index].setShouldProcessEvent(shouldProcessNoteEvent<DEFAULT_CHANNEL>, DEFAULT_NOTES[index]);
        _handlers[index + 1].setShouldProcessEvent(shouldProcessNoteEvent<DEFAULT_CHANNEL>, DEFAULT_NOTES[index + 1]);
    }
}

uint8_t ShiftRegisterEventHandlerContainer::processEvent(const MidiEvent *event, const uint8_t previousState) const
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

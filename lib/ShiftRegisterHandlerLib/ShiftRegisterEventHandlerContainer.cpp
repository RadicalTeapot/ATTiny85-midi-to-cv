#include "ShiftRegisterEventHandlerContainer.h"

ShiftRegisterEventHandlerContainer::ShiftRegisterEventHandlerContainer()
{
    uint8_t i = HANDLER_COUNT;
    do
    {
        i--;
        _handlers[i] = new ShiftRegisterEventHandler(shouldProcessNoteEvent<DEFAULT_CHANNEL>, _notes[i]);
    } while (i);
}

ShiftRegisterEventHandlerContainer::~ShiftRegisterEventHandlerContainer()
{
    uint8_t i = HANDLER_COUNT;
    do
    {
        i--;
        delete _handlers[i];
    } while (i);
}

void ShiftRegisterEventHandlerContainer::setHandlersFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler, uint8_t index)
{
    if (isNoteHandler)
    {
        _handlers[index]->setShouldProcessEvent(shouldProcessChannelEvent, dacConfig->NoteChannels >> 4);
        _handlers[index+1]->setShouldProcessEvent(shouldProcessChannelEvent, dacConfig->NoteChannels & 0x0F);
    } else {
        _handlers[index]->setShouldProcessEvent(shouldProcessNoteEvent<DEFAULT_CHANNEL>, _notes[index]);
        _handlers[index+1]->setShouldProcessEvent(shouldProcessNoteEvent<DEFAULT_CHANNEL>, _notes[index+1]);
    }
}

void ShiftRegisterEventHandlerContainer::processEvent(const MidiEvent *event, uint8_t *state) const
{
    uint8_t maskedPreviousState = 0;
    uint8_t i = 8, j = 0;
    do
    {
        i--;
        j = (uint8_t)(1 << i);
        maskedPreviousState = (uint8_t)((*state >> i) & 1);
        *state &= ~j;
        if (_handlers[i] && _handlers[i]->processEvent(event, &maskedPreviousState))
        {
            *state |= j;
        }
    } while (i);
}

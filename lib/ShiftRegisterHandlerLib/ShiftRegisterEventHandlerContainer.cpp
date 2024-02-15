#include "ShiftRegisterEventHandlerContainer.h"

ShiftRegisterEventHandlerContainer::ShiftRegisterEventHandlerContainer()
{
    for (int i = 0; i < 8; ++i)
    {
        handlers[i] = new ShiftRegisterNoteEventHandler<DEFAULT_CHANNEL>(notes[i]);
    }
}

ShiftRegisterEventHandlerContainer::~ShiftRegisterEventHandlerContainer()
{
    for (int i = 0; i < 8; ++i)
    {
        delete handlers[i];
    }
}

void ShiftRegisterEventHandlerContainer::setFirstTwoHandlersFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    if (isNoteHandler)
    {
        delete handlers[0];
        handlers[0] = new ShiftRegisterChannelEventHandler(dacConfig->NoteChannels >> 4);
        delete handlers[1];
        handlers[1] = new ShiftRegisterChannelEventHandler(dacConfig->NoteChannels & 0x0F);
    }
}

void ShiftRegisterEventHandlerContainer::setSecondTwoHandlersFromDacConfig(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    if (isNoteHandler)
    {
        delete handlers[2];
        handlers[2] = new ShiftRegisterChannelEventHandler(dacConfig->NoteChannels >> 4);
        delete handlers[3];
        handlers[3] = new ShiftRegisterChannelEventHandler(dacConfig->NoteChannels & 0x0F);
    }
}

uint8_t ShiftRegisterEventHandlerContainer::processEvent(const MidiEvent *event) const
{
    uint8_t result = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (handlers[i] && handlers[i]->processEvent(event))
        {
            result |= (1 << i);
        }
    }
    return result;
}

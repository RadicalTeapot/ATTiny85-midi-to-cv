
#include "DacEventHandlerContainer.h"

void DacEventHandlerContainer::configureHandlers(const DacPresetConfig *dacConfig, bool isNoteHandler)
{
    _useNoteHandlers = isNoteHandler;
    if (_useNoteHandlers)
    {
        _noteEventHandlers[0].configure(dacConfig->NoteChannels >> 4);
        _noteEventHandlers[1].configure(dacConfig->NoteChannels & 0x0F);
    }
    else
    {
        _ccEventHandlers[0].configure(dacConfig->CCChannels1 >> 4, dacConfig->CCNumber1);
        _ccEventHandlers[1].configure(dacConfig->CCChannels1 & 0x0F, dacConfig->CCNumber2);
        _ccEventHandlers[2].configure(dacConfig->CCChannels2 >> 4, dacConfig->CCNumber3);
        _ccEventHandlers[3].configure(dacConfig->CCChannels2 & 0x0F, dacConfig->CCNumber4);
    }
}

bool DacEventHandlerContainer::handleEvent(const MidiEvent *event, DacValues *dacValues)
{
    if (_useNoteHandlers)
    {
        return _noteEventHandlers[0].handleEvent(event, dacValues->values) || _noteEventHandlers[1].handleEvent(event, dacValues->values + 2);
    }
    else
    {
        return _ccEventHandlers[0].handleEvent(event, dacValues->values) || _ccEventHandlers[1].handleEvent(event, dacValues->values + 1)
            || _ccEventHandlers[2].handleEvent(event, dacValues->values + 2) || _ccEventHandlers[3].handleEvent(event, dacValues->values + 3);
    }
}

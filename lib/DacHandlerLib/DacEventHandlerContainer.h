#ifndef DacEventHandlerContainer_h
#define DacEventHandlerContainer_h

#include "Preset.h"
#include "EventHandlers/NoteEventHandler.h"
#include "EventHandlers/CCEventHandler.h"

class DacEventHandlerContainer
{
public:
    void configureHandlers(const DacPresetConfig *dacConfig, bool isNoteHandler);
    bool handleEvent(const MidiEvent *event, DacValues *dacValues);

private:
    NoteEventHandler _noteEventHandlers[2];
    CCEventHandler _ccEventHandlers[4];
    bool _useNoteHandlers = false;
};

#endif // DacEventHandlerContainer_h

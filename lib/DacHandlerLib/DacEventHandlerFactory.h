#ifndef DacEventHandlerFactory_h
#define DacEventHandlerFactory_h

#include "Preset.h"
#include "Utils/ValueRemapper.h"
#include "EventHandlers/NoteEventHandler.h"
#include "EventHandlers/CCEventHandler.h"

template <uint8_t midiMinValue, uint8_t midiMaxValue, uint8_t lowerMidiNote, uint8_t range>
class
DacEventHandlerFactory
{
public:
    const ValueRemapper valueRemapper = ValueRemapper(midiMinValue, midiMaxValue, lowerMidiNote, range);

    DacEventHandler *createEventHandler(const DacPresetConfig *dacConfig, bool isNoteHandler)
    {
        if (isNoteHandler)
        {
            return new NoteEventHandler(dacConfig->NoteChannels >> 4, dacConfig->NoteChannels & 0x0F, &valueRemapper);
        }
        else
        {
            return new CCEventHandler(
                dacConfig->CCChannels1 >> 4, dacConfig->CCNumber1,
                dacConfig->CCChannels1 & 0x0F, dacConfig->CCNumber2,
                dacConfig->CCChannels2 >> 4, dacConfig->CCNumber3,
                dacConfig->CCChannels2 & 0x0F, dacConfig->CCNumber4, &valueRemapper);
        }
    }
};

#endif // DacEventHandlerFactory_h

#include "DacEventHandlerFactory.h"
#include "eventHandlers/NoteEventHandler.h"
#include "eventHandlers/CCEventHandler.h"

namespace DacEventHandlerFactory
{
    DacEventHandler *Factory::createEventHandler(const DacPresetConfig *dacConfig, bool isNoteHandler)
    {
        if (isNoteHandler)
        {
            return new NoteEventHandler(_noteValueMapper, dacConfig->NoteChannels >> 4, dacConfig->NoteChannels & 0x0F);
        }
        else
        {
            return new CCEventHandler(
                _ccValueMapper,
                dacConfig->CCChannels1 >> 4, dacConfig->CCNumber1,
                dacConfig->CCChannels1 & 0x0F, dacConfig->CCNumber2,
                dacConfig->CCChannels2 >> 4, dacConfig->CCNumber3,
                dacConfig->CCChannels2 & 0x0F, dacConfig->CCNumber4);
        }
    }
}

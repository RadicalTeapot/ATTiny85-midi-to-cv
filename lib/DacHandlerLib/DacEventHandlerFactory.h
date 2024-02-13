#ifndef DacEventHandlerFactory_h
#define DacEventHandlerFactory_h

#include "Preset.h"
#include "eventHandlers/DacEventHandler.h"

class DacEventHandlerFactory {
    public:
        static DacEventHandler *createEventHandler(const DacPresetConfig *dacConfig, bool isNoteHandler);
};

#endif // DacEventHandlerFactory_h

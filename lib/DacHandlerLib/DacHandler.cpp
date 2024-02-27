#include "DacHandler.h"

void DacHandler::handleEvent(const MidiEventLib::Event *event)
{
    if (event == nullptr)
    {
        return;
    }

    if (_handlerContainer.handleEvent(event, &_dacValues))
    {
        _writeValuesToDac(&_dacValues);
    }
}

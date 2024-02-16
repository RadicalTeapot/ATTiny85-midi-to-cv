#include "DacHandler.h"

DacHandler::~DacHandler()
{
    delete _handler;
}

void DacHandler::setHandler(DacEventHandler *handler)
{
    delete _handler;
    _handler = handler;
}

bool DacHandler::handleEvent(const MidiEvent *event)
{
    if (_handler == nullptr || event == nullptr)
    {
        return false;
    }

    if (_handler->handleEvent(event, &_dacValues))
    {
        _writeValuesToDac(&_dacValues);
        return true;
    }
    return false;
}

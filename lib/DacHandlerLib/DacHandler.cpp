#include "DacHandler.h"

DacHandler::WriteValuesToDac DacHandler::writeValuesToDac = DacHandler::defaultWriteValuesToDac;

DacHandler::~DacHandler()
{
    delete _handler;
}

void DacHandler::setHandler(DacEventHandler *handler)
{
    delete _handler;
    _handler = handler;
}

bool DacHandler::handleEvent(MIDIEvent *event)
{
    if (_handler == nullptr || event == nullptr)
    {
        return false;
    }

    if (_handler->handleEvent(event, &_dacValues))
    {
        writeValuesToDac(&_dacValues);
        return true;
    }
    return false;
}

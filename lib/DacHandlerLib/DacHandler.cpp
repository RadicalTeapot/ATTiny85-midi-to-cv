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

bool DacHandler::handleEvent(MIDIEvent *event)
{
    if (_handler == nullptr || event == nullptr)
    {
        return false;
    }

    if (_handler->handleEvent(event, &_dacValues))
    {
        writeValuesToDac();
        return true;
    }
    return false;
}

void DacHandler::writeValuesToDac() {}

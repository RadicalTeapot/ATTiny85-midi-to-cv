#include "CCEventHandler.h"

bool CCEventHandler::handleEvent(const MidiEventLib::Event *event, uint16_t *values) const
{
    if (values == nullptr || event == nullptr || event->type != MidiEventLib::EventType::CC)
    {
        return false;
    }

    if (_channel == event->channel && _number == ((MidiEventLib::CCEvent *)event)->number)
    {
        values[0] = _ccValueRemapper(((MidiEventLib::CCEvent *)event)->value);
        return true;
    }
    return false;
}

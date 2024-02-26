#include "CCEventHandler.h"

bool CCEventHandler::handleEvent(const MidiEvent *event, uint16_t *values) const
{
    if (values == nullptr || event == nullptr || event->type != MidiEventType::CC)
    {
        return false;
    }

    if (_channel == event->channel && _number == ((MidiCCEvent *)event)->number)
    {
        values[0] = _ccValueRemapper(((MidiCCEvent *)event)->value);
        return true;
    }
    return false;
}

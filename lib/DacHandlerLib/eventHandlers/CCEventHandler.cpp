#include "CCEventHandler.h"

CCEventHandler::CCEventHandler(
    uint8_t channel1, uint8_t number1,
    uint8_t channel2, uint8_t number2,
    uint8_t channel3, uint8_t number3,
    uint8_t channel4, uint8_t number4)
{
    _cc[0].channel = channel1;
    _cc[0].number = number1;
    _cc[1].channel = channel2;
    _cc[1].number = number2;
    _cc[2].channel = channel3;
    _cc[2].number = number3;
    _cc[3].channel = channel4;
    _cc[3].number = number4;
}

bool CCEventHandler::handleEvent(const MidiEvent *event, DacValues *dacValues)
{
    if (dacValues == nullptr || event == nullptr || event->type != MidiEventType::CC)
    {
        return 0;
    }

    for (uint8_t i = 0; i < CC_CONFIG_NUMBER; i++)
    {
        if (_handleEvent(i, (MidiCCEvent *)event, dacValues))
        {
            return true;
        }
    }
    return false;
}

bool CCEventHandler::_handleEvent(uint8_t index, const MidiCCEvent *event, DacValues *dacValues)
{
    if (_cc[index].channel == event->channel && _cc[index].number == event->number)
    {
        dacValues->values[index] = event->value;
        return true;
    }
    return false;
}

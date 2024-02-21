#include "CCEventHandler.h"

CCEventHandler::CCEventHandler(
    DacValueMapper valueMapper,
    uint8_t channel1, uint8_t number1,
    uint8_t channel2, uint8_t number2,
    uint8_t channel3, uint8_t number3,
    uint8_t channel4, uint8_t number4): DacEventHandler(valueMapper)
{
    _cc[0].channel = channel1;
    _cc[0].number = number1;
    _cc[1].channel = channel2;
    _cc[1].number = number2;
    _cc[2].channel = channel3;
    _cc[2].number = number3;
    _cc[3].channel = channel4;
    _cc[3].number = number4;

    _values[0] = 0;
    _values[1] = 0;
    _values[2] = 0;
    _values[3] = 0;
}

bool CCEventHandler::handleEvent(const MidiEvent *event, DacValues *dacValues)
{
    if (dacValues == nullptr || event == nullptr || event->type != MidiEventType::CC)
    {
        return false;
    }

    uint8_t i = CC_CONFIG_NUMBER;
    do
    {
        i--;
        if (_handleEvent(i, (MidiCCEvent *)event))
        {
            _valueMapper(_values, dacValues);
            return true;
        }
    } while (i);

    return false;
}

bool CCEventHandler::_handleEvent(uint8_t index, const MidiCCEvent *event)
{
    if (_cc[index].channel == event->channel && _cc[index].number == event->number)
    {
        _values[index] = event->value;
        return true;
    }
    return false;
}

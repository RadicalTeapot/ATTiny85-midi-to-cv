#ifndef ShiftRegisterEventHandler_h
#define ShiftRegisterEventHandler_h

#include "MidiEvent.h"

namespace ShiftRegisterEventHandlerConstants
{
    const uint8_t NOTE_CHANNEL = 0;
}

static bool shouldProcessChannelEvent(const MidiEventLib::Event *event, const uint8_t channel)
{
    return event->channel == channel && (event->type == MidiEventLib::EventType::NOTE_ON || event->type == MidiEventLib::EventType::NOTE_OFF);
};

template <uint8_t midiChannel = ShiftRegisterEventHandlerConstants::NOTE_CHANNEL>
static bool shouldProcessNoteEvent(const MidiEventLib::Event *event, const uint8_t note)
{
    return shouldProcessChannelEvent(event, midiChannel) && event->firstByte == note;
};

class ShiftRegisterEventHandler {
public:
    typedef bool (*ShouldProcessEventFunc)(const MidiEventLib::Event *event, const uint8_t checkedValue);

    ShiftRegisterEventHandler(ShouldProcessEventFunc shouldProcessEvent = shouldProcessChannelEvent, uint8_t checkedValue = ShiftRegisterEventHandlerConstants::NOTE_CHANNEL)
        : _shouldProcessEvent(shouldProcessEvent), _checkedValue(checkedValue) {}

    void configure(ShouldProcessEventFunc shouldProcessEvent, uint8_t checkedValue)
    {
        _shouldProcessEvent = shouldProcessEvent;
        _checkedValue = checkedValue;
    }

    inline bool processEvent(const MidiEventLib::Event *event, const uint8_t result) const
    {
        if (_shouldProcessEvent(event, _checkedValue))
            return event->type == MidiEventLib::EventType::NOTE_ON;
        return result == 1;
    }
private:
    ShouldProcessEventFunc _shouldProcessEvent;
    uint8_t _checkedValue;
};

#endif // ShiftRegisterEventHandler_h

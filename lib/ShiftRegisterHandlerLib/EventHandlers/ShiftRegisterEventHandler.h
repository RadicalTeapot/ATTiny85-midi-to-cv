#ifndef ShiftRegisterEventHandler_h
#define ShiftRegisterEventHandler_h

#include "MidiEvent.h"

static bool defaultShouldProcessEvent(const MidiEventLib::Event *event, const uint8_t checkedValue)
{
    return false;
}

static bool shouldProcessChannelEvent(const MidiEventLib::Event *event, const uint8_t channel)
{
    return event->channel == channel && (event->type == MidiEventLib::EventType::NOTE_ON || event->type == MidiEventLib::EventType::NOTE_OFF);
};

template <uint8_t midiChannel>
static bool shouldProcessNoteEvent(const MidiEventLib::Event *event, const uint8_t note)
{
    return event->channel == midiChannel && (event->type == MidiEventLib::EventType::NOTE_ON || event->type == MidiEventLib::EventType::NOTE_OFF) && event->firstByte == note;
};

class ShiftRegisterEventHandler {
public:
    typedef bool (*ShouldProcessEventFunc)(const MidiEventLib::Event *event, const uint8_t checkedValue);

    ShiftRegisterEventHandler() : _shouldProcessEvent(defaultShouldProcessEvent), _checkedValue(0) {}
    ShiftRegisterEventHandler(ShouldProcessEventFunc shouldProcessEvent, uint8_t checkedValue) : _shouldProcessEvent(shouldProcessEvent), _checkedValue(checkedValue) {}

    void setShouldProcessEvent(ShouldProcessEventFunc shouldProcessEvent, uint8_t checkedValue)
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

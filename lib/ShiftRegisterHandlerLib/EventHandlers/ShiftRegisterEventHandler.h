#ifndef ShiftRegisterEventHandler_h
#define ShiftRegisterEventHandler_h

#include "MidiEvent.h"

static bool defaultShouldProcessEvent(const MidiEvent *event, const uint8_t checkedValue)
{
    return false;
}

static bool shouldProcessChannelEvent(const MidiEvent *event, const uint8_t channel)
{
    return event->channel == channel && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF);
};

template <uint8_t midiChannel>
static bool shouldProcessNoteEvent(const MidiEvent *event, const uint8_t note)
{
    return event->channel == midiChannel && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF) && event->firstByte == note;
};

class ShiftRegisterEventHandler {
public:
    typedef bool (*ShouldProcessEventFunc)(const MidiEvent *event, const uint8_t checkedValue);

    ShiftRegisterEventHandler() : _shouldProcessEvent(defaultShouldProcessEvent), _checkedValue(0) {}
    ShiftRegisterEventHandler(ShouldProcessEventFunc shouldProcessEvent, uint8_t checkedValue) : _shouldProcessEvent(shouldProcessEvent), _checkedValue(checkedValue) {}

    void setShouldProcessEvent(ShouldProcessEventFunc shouldProcessEvent, uint8_t checkedValue)
    {
        _shouldProcessEvent = shouldProcessEvent;
        _checkedValue = checkedValue;
    }
    inline bool processEvent(const MidiEvent *event, const uint8_t result) const
    {
        if (_shouldProcessEvent(event, _checkedValue))
            return event->type == MidiEventType::NOTE_ON;
        return result == 1;
    }
private:
    ShouldProcessEventFunc _shouldProcessEvent;
    uint8_t _checkedValue;
};

#endif // ShiftRegisterEventHandler_h

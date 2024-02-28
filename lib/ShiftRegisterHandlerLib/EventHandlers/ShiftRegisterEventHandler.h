#ifndef ShiftRegisterEventHandler_h
#define ShiftRegisterEventHandler_h

#include "MidiEvent.h"

template <uint8_t midiChannel>
class ShiftRegisterEventHandler
{
public:
    typedef bool (*ShouldProcessEventFunc)(const MidiEventLib::Event *event, const uint8_t checkedValue);

    ShiftRegisterEventHandler(const uint8_t checkedValue = midiChannel, const bool isChannelHandler = true)
        : _checkedValue(checkedValue), _isChannelHandler(isChannelHandler) {}

    void configure(const uint8_t checkedValue, const bool isChannelHandler)
    {
        _checkedValue = checkedValue;
        _isChannelHandler = isChannelHandler;
    }

    inline bool processEvent(const MidiEventLib::Event *event, const uint8_t result) const
    {
        const bool shouldProcessEvent = _isChannelHandler ? shouldProcessChannelEvent(event, _checkedValue) : shouldProcessNoteEvent(event, _checkedValue);
        return shouldProcessEvent ? event->type == MidiEventLib::EventType::NOTE_ON : result == 1;
    }

private:
    uint8_t _checkedValue;
    bool _isChannelHandler;

    static inline bool shouldProcessChannelEvent(const MidiEventLib::Event *event, const uint8_t channel)
    {
        return event->channel == channel && (event->type == MidiEventLib::EventType::NOTE_ON || event->type == MidiEventLib::EventType::NOTE_OFF);
    };

    static inline bool shouldProcessNoteEvent(const MidiEventLib::Event *event, const uint8_t note)
    {
        return shouldProcessChannelEvent(event, midiChannel) && event->firstByte == note;
    };
};

#endif // ShiftRegisterEventHandler_h

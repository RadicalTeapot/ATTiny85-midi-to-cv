#ifndef ShiftRegisterChannelEventHandler_h
#define ShiftRegisterChannelEventHandler_h

#include "MidiEvent.h"
#include "ShiftRegisterEventHandler.h"

class ShiftRegisterChannelEventHandler : public ShiftRegisterEventHandler
{
public:
    ShiftRegisterChannelEventHandler(uint8_t channel) : _channel(channel) {}
    bool processEvent(const MidiEvent *event)
    {
        if (shouldHandleEvent(event))
        {
            _currentChannelState = event->type == MidiEventType::NOTE_ON;
        }
        return _currentChannelState;
    }

private:
    uint8_t _channel;
    bool _currentChannelState = false;
    inline bool shouldHandleEvent(const MidiEvent *event) const
    {
        return event->channel == _channel && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF);
    }
};

#endif // DacPitchModeHandler_h

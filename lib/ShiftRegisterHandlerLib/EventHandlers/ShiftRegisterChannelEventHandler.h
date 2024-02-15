#ifndef ShiftRegisterChannelEventHandler_h
#define ShiftRegisterChannelEventHandler_h

#include "MidiEvent.h"
#include "ShiftRegisterEventHandler.h"

template<uint8_t channel>
class ShiftRegisterChannelEventHandler : public ShiftRegisterEventHandler
{
public:
    bool handleEvent(const MidiEvent *event)
    {
        if (shouldHandleEvent(event))
        {
            _currentChannelState = event->type == MidiEventType::NOTE_ON;
        }
        return _currentChannelState;
    }
private:
    bool _currentChannelState = false;
    inline bool shouldHandleEvent(const MidiEvent *event) {
        return event->channel == channel && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF);
    }
};

#endif // DacPitchModeHandler_h

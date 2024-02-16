#ifndef ShiftRegisterChannelEventHandler_h
#define ShiftRegisterChannelEventHandler_h

#include "MidiEvent.h"
#include "ShiftRegisterEventHandler.h"

class ShiftRegisterChannelEventHandler : public ShiftRegisterEventHandler
{
public:
    ShiftRegisterChannelEventHandler(uint8_t channel) : _channel(channel) {}
private:
    uint8_t _channel;
    inline bool shouldProcessEvent(const MidiEvent *event) const
    {
        return event->channel == _channel && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF);
    }
};

#endif // DacPitchModeHandler_h

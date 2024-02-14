#ifndef DacPitchModeHandler_h
#define DacPitchModeHandler_h

#include "MidiEvent.h"
#include "ShiftRegisterEventHandler.h"

template<uint8_t offset>
class DacPitchModeHandler : public ShiftRegisterEventHandler
{
public:
    DacPitchModeHandler(uint8_t channels): _channels(channels) {}

    bool handleEvent(const MidiEvent *event, uint8_t *values)
    {
        if (!isHandleEvent(event))
        {
            return false;
        }

        uint8_t effectiveOffset = event->channel == getFirstChannel() ? offset + 1 : offset;
        if (event->type == MidiEventType::NOTE_ON)
        {
            *values |= 1 << effectiveOffset;
        }
        else if (event->type == MidiEventType::NOTE_OFF)
        {
            *values &= ~(1 << effectiveOffset);
        }
        return true;
    }
private:
    const uint8_t _channels;
    inline uint8_t getFirstChannel() { return _channels & 0x0F; }
    inline uint8_t getSecondChannel() { return _channels >> 4; }
    inline bool isHandleEvent(const MidiEvent *event) {
        return (event->channel == getFirstChannel() || event->channel == getSecondChannel())
            && (event->type == MidiEventType::NOTE_ON || event->type == MidiEventType::NOTE_OFF);
    }
};

#endif // DacPitchModeHandler_h

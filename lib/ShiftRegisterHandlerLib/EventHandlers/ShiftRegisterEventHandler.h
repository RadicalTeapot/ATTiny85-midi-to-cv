
#ifndef ShiftRegisterEventHandler_h
#define ShiftRegisterEventHandler_h

#include "Preset.h"
#include "MidiEvent.h"

#define DEFAULT_NOTES 24,25,26,27,28,29,30,31
#define DEFAULT_CHANNELS 0x00,0x00,0x00,0x00

class ShiftRegisterEventHandler {
public:
    virtual bool handleEvent(const MidiEvent *event, uint8_t *values) = 0;
private:
    uint8_t notes[8];
    uint8_t channels[4];
    const uint8_t defaultChannels[4] = {DEFAULT_CHANNELS};
    const uint8_t defaultNotes[8] = {DEFAULT_NOTES};
};

#endif // ShiftRegisterEventHandler_h

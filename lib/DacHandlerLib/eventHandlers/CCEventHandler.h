#ifndef CCEventHandler_h
#define CCEventHandler_h

#include "MidiEvent.h"
#include "DacValues.h"
#include "../Utils/ValueRemapper.h"

class CCEventHandler
{
    public:
        CCEventHandler(uint8_t channel = 0, uint8_t number = 0, const ValueRemapper::RemapMidiValue ccValueRemapper = ValueRemapper::remapMidiValue)
            : _channel(channel), _number(number), _ccValueRemapper(ccValueRemapper) {}
        void configure(uint8_t channel, uint8_t number) { _channel = channel; _number = number; }

        bool handleEvent(const MidiEventLib::Event *event, uint16_t *values) const;
    private:
        uint8_t _channel;
        uint8_t _number;
        ValueRemapper::RemapMidiValue _ccValueRemapper;
};

#endif // CCEventHandler_h

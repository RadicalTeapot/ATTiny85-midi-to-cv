#ifndef CCEventHandler_h
#define CCEventHandler_h

#include "DacEventHandler.h"
#include "MidiEvent.h"
#include "DacValues.h"

#define CC_CONFIG_NUMBER 4

typedef struct _CC_EVENT_CONFIG {
    uint8_t channel;
    uint8_t number;
} CCConfig;

class CCEventHandler : public DacEventHandler
{
    public:
        CCEventHandler(uint8_t channel1, uint8_t number1, uint8_t channel2, uint8_t number2, uint8_t channel3, uint8_t number3, uint8_t channel4, uint8_t number4);
        bool handleEvent(const MidiEvent *event, DacValues *dacValues);
    private:
        bool _handleEvent(uint8_t index, const MidiCCEvent *event, DacValues *dacValues) const;
        CCConfig _cc[CC_CONFIG_NUMBER];
};

#endif // CCEventHandler_h

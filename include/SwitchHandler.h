#ifndef SwitchHandler_h
#define SwitchHandler_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

namespace SwitchHandler
{
    template <uint8_t pinA, uint8_t pinB>
    void initSwitches()
    {
        pinMode(pinA, INPUT_PULLUP);
        pinMode(pinB, INPUT_PULLUP);
    }

    template <uint8_t pinA, uint8_t pinB>
    void getSwitchesState(uint8_t *state)
    {
        *state = (digitalRead(pinB) << 1) | digitalRead(pinA);
    }
}

#endif // SwitchHandler_h

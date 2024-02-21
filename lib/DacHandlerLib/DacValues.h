#ifndef DacValues_h
#define DacValues_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

struct DacValues {
    uint16_t values[4] = {0, 0, 0, 0};
};

#endif // DacValues_h

#ifndef DacValues_h
#define DacValues_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

struct DacValues {
    uint8_t values[4];
};

#endif // DacValues_h

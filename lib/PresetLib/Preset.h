#ifndef Preset_h
#define Preset_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

typedef struct _DAC_PRESET_CONFIG
{
    uint8_t NoteChannels;
    uint8_t CCNumber1;
    uint8_t CCNumber2;
    uint8_t CCNumber3;
    uint8_t CCNumber4;
    uint8_t CCChannels1;
    uint8_t CCChannels2;
} DacPresetConfig;

typedef struct _PRESET
{
    DacPresetConfig dacConfigA;
    DacPresetConfig dacConfigB;
} Preset;

#endif // Preset_h

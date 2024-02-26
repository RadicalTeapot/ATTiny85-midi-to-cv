#ifndef DacPitchCalibrationLookUpTable_h
#define DacPitchCalibrationLookUpTable_h

#ifdef ARDUINO
#include <Arduino.h>
#endif

namespace DacPitchCalibration
{
    const uint8_t RANGE = 56;
    const uint8_t LOW_MIDI_NOTE = 36;

#ifdef ARDUINO
    const PROGMEM uint16_t LookUpTable[RANGE]
#else
    const uint16_t LookUpTable[RANGE]
#endif
        = {
            0, // 36
            68,
            138,
            209,
            275,
            345,
            415,
            486,
            554,
            620,
            692,
            762,
            829, // 48 (C2)
            895,
            966,
            1033,
            1101,
            1170,
            1239,
            1312,
            1380,
            1447,
            1518,
            1585,
            1654, // 60 (C3)
            1721,
            1790,
            1861,
            1931,
            1997,
            2066,
            2134,
            2205,
            2270,
            2339,
            2411,
            2478, // 72 (C4)
            2546,
            2618,
            2687,
            2756,
            2822,
            2890,
            2959,
            3028,
            3094,
            3167,
            3237,
            3309, // 84 (C5)
            3416,
            3500, // 86 Calibration stops here as OSC/d doesn't go higher
            3590,
            3680,
            3770,
            3860,
            3950 // 91
    };
}

#endif // DacPitchCalibrationLookUpTable_h

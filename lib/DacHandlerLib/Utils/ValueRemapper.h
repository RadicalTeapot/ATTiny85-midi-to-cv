#ifndef ValueRemapper_h
#define ValueRemapper_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#include <algorithm>
#endif

#include "DacPitchCalibrationLookUpTable.h"

namespace ValueRemapper
{
    typedef uint16_t (*RemapMidiValue)(const uint8_t);

    const uint8_t MIDI_MIN_VALUE = 0;
    const uint8_t MIDI_MAX_VALUE = 127;

    template <uint8_t lowerMidiNote = DacPitchCalibration::LOW_MIDI_NOTE, uint8_t range = DacPitchCalibration::RANGE>
    inline uint16_t remapNote(const uint8_t midiNote)
    {
#ifdef ARDUINO
        return pgm_read_word_near(DacPitchCalibration::LookUpTable + min(midiNote - lowerMidiNote, range - 1));
#else
        return DacPitchCalibration::LookUpTable[std::min(midiNote - lowerMidiNote, range - 1)];
#endif
    }
    template <uint8_t midiMinValue = MIDI_MIN_VALUE, uint8_t midiMaxValue = MIDI_MAX_VALUE>
    inline uint16_t remapMidiValue(const uint8_t midiNote)
    {
        return static_cast<uint16_t>(midiMinValue + midiMaxValue * (static_cast<float>(midiNote - midiMinValue) / (midiMaxValue - midiMinValue)));
    }
}

#endif // ValueRemapper_h

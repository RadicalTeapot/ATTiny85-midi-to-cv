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
    using RemapMidiValue = uint16_t(*)(const uint8_t);

    const uint8_t MIDI_MIN_VALUE = 0;
    const uint8_t MIDI_MAX_VALUE = 127;
    const uint16_t DAC_MIN_VALUE = 0;
    const uint16_t DAC_MAX_VALUE = 4095;

    template <uint8_t lowerMidiNote = DacPitchCalibration::LOW_MIDI_NOTE, uint8_t range = DacPitchCalibration::RANGE>
    inline uint16_t remapNote(const uint8_t midiNote)
    {
#ifdef ARDUINO
        return pgm_read_word_near(DacPitchCalibration::LookUpTable + constrain(midiNote - lowerMidiNote, 0, range - 1));
#else
        return DacPitchCalibration::LookUpTable[std::min(std::max(midiNote - lowerMidiNote, 0), range - 1)];
#endif
    }
    template <uint8_t inputMinValue = MIDI_MIN_VALUE, uint8_t inputMaxValue = MIDI_MAX_VALUE, uint16_t outputMinValue = DAC_MIN_VALUE, uint16_t outputMaxValue = DAC_MAX_VALUE>
    inline uint16_t remapMidiValue(const uint8_t midiNote)
    {
#ifdef ARDUINO
        const uint8_t clampedMidiNote = constrain(midiNote, inputMinValue, inputMaxValue);
#else
        const uint8_t clampedMidiNote = std::max(inputMinValue, std::min(midiNote, inputMaxValue));
#endif
        const float mappedValue = static_cast<float>(clampedMidiNote - inputMinValue) / (inputMaxValue - inputMinValue);
        return static_cast<uint16_t>(outputMinValue + (outputMaxValue - outputMinValue) * mappedValue);
    }
}

#endif // ValueRemapper_h

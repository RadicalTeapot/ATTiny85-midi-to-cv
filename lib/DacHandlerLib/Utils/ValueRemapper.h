#ifndef ValueRemapper_h
#define ValueRemapper_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#include <algorithm>
#endif

#include "DacPitchCalibrationLookUpTable.h"

class IValueRemapper
{
public:
    virtual inline uint16_t remapNote(const uint8_t midiNote) const = 0;
    virtual inline uint16_t remapCC(const uint8_t midiCC) const = 0;
    virtual inline uint16_t remapVelocity(const uint8_t midiValue) const = 0;
};

class ValueRemapper: public IValueRemapper
{
public:
    ValueRemapper(const uint16_t midiMinValue, const uint16_t midiMaxValue, const uint16_t lowerMidiNote, const uint16_t range)
        : midiMinValue(midiMinValue), midiMaxValue(midiMaxValue), lowerMidiNote(lowerMidiNote), range(range) {}

    inline uint16_t remapNote(const uint8_t midiNote) const override
    {
        return remapMidiNote(midiNote);
    }

    inline uint16_t remapCC(const uint8_t midiCC) const override
    {
        return remapMidiValue(midiCC);
    }

    inline uint16_t remapVelocity(const uint8_t midiValue) const override
    {
        return remapMidiValue(midiValue);
    }

private:
    const uint16_t midiMinValue;
    const uint16_t midiMaxValue;
    const uint16_t lowerMidiNote;
    const uint16_t range;

    inline uint16_t remapMidiNote(const uint8_t midiNote) const
    {
#ifdef ARDUINO
        return pgm_read_word_near(DacPitchCalibration::LookUpTable + min(midiNote - lowerMidiNote, range - 1));
#else
        return DacPitchCalibration::LookUpTable[std::min(midiNote - lowerMidiNote, range - 1)];
#endif
    }

    inline uint16_t remapMidiValue(const uint8_t midiNote) const
    {
        return static_cast<uint16_t>(midiMinValue + midiMaxValue * (static_cast<float>(midiNote - midiMinValue) / (midiMaxValue - midiMinValue)));
    }
};

#endif // ValueRemapper_h

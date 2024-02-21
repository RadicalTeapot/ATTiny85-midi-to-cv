#ifndef DacEventHandlerFactory_h
#define DacEventHandlerFactory_h

#include "Preset.h"
#include "DacPitchCalibrationLookUpTable.h"
#include "eventHandlers/DacEventHandler.h"

#ifndef ARDUINO
#include <algorithm>
#endif

namespace DacEventHandlerFactory
{
    static void defaultDacValueMapper(const uint8_t[4], DacValues *dacValues);
    class Factory
    {
    public:
        Factory(DacValueMapper noteValueMapper = defaultDacValueMapper, DacValueMapper ccValueMapper = defaultDacValueMapper)
            : _noteValueMapper(noteValueMapper), _ccValueMapper(ccValueMapper) {}
        DacEventHandler *createEventHandler(const DacPresetConfig *dacConfig, bool isNoteHandler);

    private:
        DacValueMapper _noteValueMapper;
        DacValueMapper _ccValueMapper;
    };

    static inline void defaultDacValueMapper(const uint8_t values[4], DacValues *dacValues)
    {
        dacValues->values[0] = values[0];
        dacValues->values[1] = values[1];
        dacValues->values[2] = values[2];
        dacValues->values[3] = values[3];
    }

    template <uint8_t lowerMidiNote, uint8_t range>
    static inline uint16_t remapMidiNote(uint8_t midiNote)
    {
#ifdef ARDUINO
        return pgm_read_word_near(DacPitchCalibrationLookUpTable + min(midiNote - lowerMidiNote, range - 1));
#else
        return DacPitchCalibrationLookUpTable[std::min(midiNote - lowerMidiNote, range - 1)];
#endif
    }

    template <uint8_t lowerBound, uint8_t upperBound>
    static inline uint16_t remapMidiValue(uint8_t midiNote)
    {
        return static_cast<uint32_t>(midiNote - lowerBound) * 4095U / (upperBound - lowerBound);
    }

    template <uint8_t lowerMidiNote, uint8_t range, uint8_t midiMaxValue>
    void dacNoteValueMapper(const uint8_t values[4], DacValues *dacValues)
    {
        dacValues->values[0] = remapMidiNote<lowerMidiNote, range>(dacValues->values[0]);
        dacValues->values[1] = remapMidiValue<0, midiMaxValue>(dacValues->values[1]);
        dacValues->values[2] = remapMidiNote<lowerMidiNote, range>(dacValues->values[2]);
        dacValues->values[3] = remapMidiValue<0, midiMaxValue>(dacValues->values[3]);
    }

    template <uint8_t midiMaxValue>
    void dacCCValueMapper(const uint8_t values[4], DacValues *dacValues)
    {
        dacValues->values[0] = remapMidiValue<0, midiMaxValue>(dacValues->values[0]);
        dacValues->values[1] = remapMidiValue<0, midiMaxValue>(dacValues->values[1]);
        dacValues->values[2] = remapMidiValue<0, midiMaxValue>(dacValues->values[2]);
        dacValues->values[3] = remapMidiValue<0, midiMaxValue>(dacValues->values[3]);
    }
}

#endif // DacEventHandlerFactory_h

#ifndef NoteEventHandler_h
#define NoteEventHandler_h

#include "EventHandlers/DacEventHandler.h"
#include <MidiEvent.h>
#include "../DacValues.h"
#include <CyclicCompactArray.h>
#include "../Utils/ValueRemapper.h"

#define NOTE_EVENTS_NUMBER 2
#define MAX_LEGATO_NOTE_NUMBER 4

class NoteEventHandler : public DacEventHandler {
    public:
        NoteEventHandler(uint8_t channel1, uint8_t channel2, const IValueRemapper *valueRemapper);
        bool handleEvent(const MidiEvent *event, DacValues *dacValues);
    private:
        uint8_t _channels[NOTE_EVENTS_NUMBER];
        CyclicCompactArray<MAX_LEGATO_NOTE_NUMBER> _notes;
        uint8_t _values[4];
        const IValueRemapper *_valueRemapper;

        bool _handleNoteOnEvent(uint8_t index, const MidiNoteOnEvent *event);
        bool _handleNoteOffEvent(uint8_t index, const MidiNoteOffEvent *event);
        inline void setDacValues(const uint8_t values[4], DacValues *dacValues) const;
};

#endif // NoteEventHandler_h

#ifndef NoteEventHandler_h
#define NoteEventHandler_h

#include "EventHandlers/DacEventHandler.h"
#include <MidiEvent.h>
#include "../DacValues.h"
#include <CyclicCompactArray.h>

#define NOTE_EVENTS_NUMBER 2
#define MAX_LEGATO_NOTE_NUMBER 4

class NoteEventHandler : public DacEventHandler {
    public:
        NoteEventHandler(DacValueMapper valueMapper, uint8_t channel1, uint8_t channel2);
        bool handleEvent(const MidiEvent *event, DacValues *dacValues);
    private:
        uint8_t _channels[NOTE_EVENTS_NUMBER];
        CyclicCompactArray<MAX_LEGATO_NOTE_NUMBER> _notes;
        uint8_t _values[4];

        bool _handleNoteOnEvent(uint8_t index, const MidiNoteOnEvent *event);
        bool _handleNoteOffEvent(uint8_t index, const MidiNoteOffEvent *event);
};

#endif // NoteEventHandler_h

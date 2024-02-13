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
        NoteEventHandler(uint8_t channel1, uint8_t channel2);
        bool handleEvent(MidiEvent *event, DacValues *dacValues);
    private:
        uint8_t _channels[NOTE_EVENTS_NUMBER];
        CyclicCompactArray<MAX_LEGATO_NOTE_NUMBER> _notes;

        bool _handleNoteOnEvent(uint8_t index, MidiNoteOnEvent *event, DacValues *dacValues);
        bool _handleNoteOffEvent(uint8_t index, MidiNoteOffEvent *event, DacValues *dacValues);
};

#endif // NoteEventHandler_h

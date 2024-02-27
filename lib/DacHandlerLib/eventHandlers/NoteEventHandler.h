#ifndef NoteEventHandler_h
#define NoteEventHandler_h

#include "MidiEvent.h"
#include "../DacValues.h"
#include <CyclicCompactArray.h>
#include "../Utils/ValueRemapper.h"

const uint8_t MAX_LEGATO_NOTE_NUMBER = 4;

class NoteEventHandler
{
public:
    NoteEventHandler(uint8_t channel = 0, const ValueRemapper::RemapMidiValue noteRemapper = ValueRemapper::remapNote, const ValueRemapper::RemapMidiValue velocityRemapper = ValueRemapper::remapMidiValue)
        : _channel(channel), _noteRemapper(noteRemapper), _velocityRemapper(velocityRemapper) {}
    void configure(uint8_t channel) { _channel = channel; }

    bool handleEvent(const MidiEventLib::Event *event, uint16_t *values);
private:
    uint8_t _channel;
    CyclicCompactArray<MAX_LEGATO_NOTE_NUMBER> _notes;
    ValueRemapper::RemapMidiValue _noteRemapper;
    ValueRemapper::RemapMidiValue _velocityRemapper;

    bool inline _handleNoteOnEvent(const MidiEventLib::NoteOnEvent *event, uint16_t *values);
    bool _handleNoteOffEvent(const MidiEventLib::NoteOffEvent *event, uint16_t *values);
};

#endif // NoteEventHandler_h

#include "NoteEventHandler.h"

bool NoteEventHandler::handleEvent(const MidiEvent *event, uint16_t *values)
{
    if (values == nullptr || event == nullptr)
    {
        return 0;
    }

    return (event->type == MidiEventType::NOTE_ON && _handleNoteOnEvent((MidiNoteOnEvent *)event, values))
        || (event->type == MidiEventType::NOTE_OFF && _handleNoteOffEvent((MidiNoteOffEvent *)event, values));
}

bool NoteEventHandler::_handleNoteOnEvent(const MidiNoteOnEvent *event, uint16_t *values)
{
    if (_channel == event->channel)
    {
        _notes.append(event->note);
        values[0] = _noteRemapper(event->note);
        values[1] = _velocityRemapper(event->velocity);
        return true;
    }
    return false;
}

bool NoteEventHandler::_handleNoteOffEvent(const MidiNoteOffEvent *event, uint16_t *values)
{
    if (_channel == event->channel)
    {
        uint8_t tail = _notes.getTail();
        _notes.remove(tail);

        // If the removed note is the same as the event note, send a new note (most recent active note)
        if (tail == event->note)
        {
            values[0] = _noteRemapper(_notes.getTail());
            values[1] = _velocityRemapper(event->velocity);
            return true;
        }
    }
    return false;
}

#include "NoteEventHandler.h"

bool NoteEventHandler::handleEvent(const MidiEventLib::Event *event, uint16_t *values)
{
    if (values == nullptr || event == nullptr || event->channel != _channel)
    {
        return 0;
    }

    return (event->type == MidiEventLib::EventType::NOTE_ON && _handleNoteOnEvent((MidiEventLib::NoteOnEvent *)event, values))
        || (event->type == MidiEventLib::EventType::NOTE_OFF && _handleNoteOffEvent((MidiEventLib::NoteOffEvent *)event, values));
}

bool inline NoteEventHandler::_handleNoteOnEvent(const MidiEventLib::NoteOnEvent *event, uint16_t *values)
{
    _notes.append(event->note);
    values[0] = _noteRemapper(event->note);
    values[1] = _velocityRemapper(event->velocity);
    return true;
}

bool inline NoteEventHandler::_handleNoteOffEvent(const MidiEventLib::NoteOffEvent *event, uint16_t *values)
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
    return false;
}

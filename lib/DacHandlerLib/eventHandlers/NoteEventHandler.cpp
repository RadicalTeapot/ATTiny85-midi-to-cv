#include "NoteEventHandler.h"

NoteEventHandler::NoteEventHandler(uint8_t channel1, uint8_t channel2)
{
    _channels[0] = channel1;
    _channels[1] = channel2;
}

bool NoteEventHandler::handleEvent(const MidiEvent *event, DacValues *dacValues)
{
    if (dacValues == nullptr || event == nullptr)
    {
        return 0;
    }

    if (event->type == MidiEventType::NOTE_ON)
    {
        for (uint8_t i = 0; i < NOTE_EVENTS_NUMBER; i++)
        {
            if (_handleNoteOnEvent(i, (MidiNoteOnEvent *)event, dacValues))
            {
                return true;
            }
        }
    }
    else if (event->type == MidiEventType::NOTE_OFF)
    {
        for (uint8_t i = 0; i < NOTE_EVENTS_NUMBER; i++)
        {
            if (_handleNoteOffEvent(i, (MidiNoteOffEvent *)event, dacValues))
            {
                return true;
            }
        }
    }
    return false;
}

bool NoteEventHandler::_handleNoteOnEvent(uint8_t index, const MidiNoteOnEvent *event, DacValues *dacValues)
{
    if (_channels[index] == event->channel)
    {
        dacValues->values[index * 2] = event->note;
        dacValues->values[index * 2 + 1] = event->velocity;

        _notes.append(event->note);
        return true;
    }
    return false;
}

bool NoteEventHandler::_handleNoteOffEvent(uint8_t index, const MidiNoteOffEvent *event, DacValues *dacValues)
{
    if (_channels[index] == event->channel)
    {
        uint8_t tail = _notes.getTail();
        _notes.remove(tail);

        // If the removed note is the same as the event note, send a new note (most recent active note)
        if (tail == event->note)
        {
            dacValues->values[index * 2] = _notes.getTail();
            dacValues->values[index * 2 + 1] = event->velocity;
            return true;
        }
    }
    return false;
}

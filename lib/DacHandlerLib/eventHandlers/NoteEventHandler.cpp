#include "NoteEventHandler.h"

NoteEventHandler::NoteEventHandler(uint8_t channel1, uint8_t channel2, const IValueRemapper *valueRemapper): _valueRemapper(valueRemapper)
{
    _channels[0] = channel1;
    _channels[1] = channel2;

    _values[0] = 0;
    _values[1] = 0;
    _values[2] = 0;
    _values[3] = 0;
}

bool NoteEventHandler::handleEvent(const MidiEvent *event, DacValues *dacValues)
{
    if (dacValues == nullptr || event == nullptr)
    {
        return 0;
    }

    if (event->type == MidiEventType::NOTE_ON)
    {
        uint8_t i = NOTE_EVENTS_NUMBER;
        do
        {
            i--;
            if (_handleNoteOnEvent(i, (MidiNoteOnEvent *)event))
            {
                setDacValues(_values, dacValues);
                return true;
            }
        } while (i);
    }
    else if (event->type == MidiEventType::NOTE_OFF)
    {
        uint8_t i = NOTE_EVENTS_NUMBER;
        do
        {
            i--;
            if (_handleNoteOffEvent(i, (MidiNoteOffEvent *)event))
            {
                setDacValues(_values, dacValues);
                return true;
            }
        } while (i);
    }
    return false;
}

bool NoteEventHandler::_handleNoteOnEvent(uint8_t index, const MidiNoteOnEvent *event)
{
    if (_channels[index] == event->channel)
    {
        _values[index * 2] = event->note;
        _values[index * 2 + 1] = event->velocity;

        _notes.append(event->note);
        return true;
    }
    return false;
}

bool NoteEventHandler::_handleNoteOffEvent(uint8_t index, const MidiNoteOffEvent *event)
{
    if (_channels[index] == event->channel)
    {
        uint8_t tail = _notes.getTail();
        _notes.remove(tail);

        // If the removed note is the same as the event note, send a new note (most recent active note)
        if (tail == event->note)
        {
            _values[index * 2] = _notes.getTail();
            _values[index * 2 + 1] = event->velocity;
            return true;
        }
    }
    return false;
}

inline void NoteEventHandler::setDacValues(const uint8_t values[4], DacValues *dacValues) const
{
    dacValues->values[0] = _valueRemapper->remapNote(values[0]);
    dacValues->values[1] = _valueRemapper->remapVelocity(values[1]);
    dacValues->values[2] = _valueRemapper->remapNote(values[2]);
    dacValues->values[3] = _valueRemapper->remapVelocity(values[3]);
}

#include <MidiRecv.h>

#define BAUD_RATE 31250

MidiRecv::MidiRecv(ReceiveOnlySoftwareSerial *serial, uint8_t midiChannel)
{
    _serial = serial;
    _midiChannel = midiChannel - 1;
}

void MidiRecv::begin()
{
    _serial->begin(BAUD_RATE);
}

bool MidiRecv::recv(MIDI_DATA *data)
{
    if (_serial->available())
    {
        uint8_t midiByte = _serial->read();
        if (isMidiCommand(midiByte))
        {
            parseMidiCommand(midiByte);
        }
        else
        {
            if (parseMidiData(midiByte))
            {
                data->note = _lastMidiData.note;
                data->velocity = _lastMidiData.velocity;
                data->type = _lastMidiData.type;
                resetMidiData();
                return true;
            }
        }
    }
    return false;
}

void MidiRecv::parseMidiCommand(uint8_t midiByte)
{
    if (isMidiSystemCommand(midiByte))
    {
        resetLastMidiByte();
    }
    else // Channel command, store the command and reset note and velocity
    {
        _lastMidiByte = midiByte;
        resetMidiData();
    }
}

bool MidiRecv::parseMidiData(uint8_t midiByte)
{
    if (isMatchingMidiOn())
    {
        if (_lastMidiData.note == 0)
        {
            _lastMidiData.note = midiByte;
        }
        else
        {
            _lastMidiData.velocity = midiByte;

            if (_lastMidiData.velocity == 0)
            {
                _lastMidiData.type = EVENT_NOTE_OFF;
            }
            else
            {
                _lastMidiData.type = EVENT_NOTE_ON;
            }
            return true;
        }
    }
    else if (isMatchingMidiOff())
    {
        if (_lastMidiData.note == 0)
        {
            _lastMidiData.note = midiByte;
        }
        else
        {
            _lastMidiData.velocity = midiByte;
            _lastMidiData.type = EVENT_NOTE_OFF;
            return true;
        }
    }
    else if (isMatchingMidiCC())
    {
        if (_lastMidiData.note == 0)
        {
            _lastMidiData.note = midiByte;
        }
        else
        {
            _lastMidiData.velocity = midiByte;
            _lastMidiData.type = EVENT_CONTROL_CHANGE;
            return true;
        }
    }
    return false;
}

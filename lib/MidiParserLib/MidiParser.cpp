#include <MidiParser.h>

bool MidiParser::parse(uint8_t midiByte, MidiEvent *midiEvent)
{
    if (isMidiCommand(midiByte))
    {
        parseMidiCommand(midiByte);
    }
    else
    {
        if (parseMidiData(midiByte, midiEvent))
        {
            resetMidiData();
            return true;
        }
    }
    return false;
}

void MidiParser::parseMidiCommand(uint8_t midiByte)
{
    if (isMidiSystemCommand(midiByte))
    {
        resetLastMidiByte();
    }
    else // Channel command, store the command and reset note and velocity
    {
        _lastMidiCommand = midiByte;
        resetMidiData();
    }
}

bool MidiParser::parseMidiData(uint8_t midiByte, MidiEvent *midiEvent)
{
    if (_midiData[0] == 0)
    {
        _midiData[0] = midiByte;
    }
    else
    {
        _midiData[1] = midiByte;
        if (isMidiNoteOn() && _midiData[1] != 0)
        {
            MidiEvent::SetNoteOnEventData(midiEvent, getMidiChannel(), _midiData[0], _midiData[1]);
            return true;
        }
        else if (isMidiNoteOff() || (isMidiNoteOn() && _midiData[1] == 0))
        {
            MidiEvent::setNoteOffEventData(midiEvent, getMidiChannel(), _midiData[0], _midiData[1]);
            return true;
        }
        else if (isMidiCC())
        {
            MidiEvent::setCCEventData(midiEvent, getMidiChannel(), _midiData[0], _midiData[1]);
            return true;
        }
    }
    return false;
}

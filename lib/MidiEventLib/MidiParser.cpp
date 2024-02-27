#include <MidiParser.h>

bool MidiEventLib::Parser::parse(uint8_t midiByte, MidiEventLib::Event *midiEvent)
{
    if (isMidiCommand(midiByte))
    {
        parseMidiCommand(midiByte);
    }
    else if (parseMidiData(midiByte, midiEvent))
    {
        resetMidiData();
        return true;
    }
    return false;
}

void MidiEventLib::Parser::parseMidiCommand(uint8_t midiByte)
{
    if (isMidiSystemCommand(midiByte))
    {
        resetLastMidiCommand();
    }
    else // Channel command, store the command and reset note and velocity
    {
        _lastMidiCommand = midiByte;
        resetMidiData();
    }
}

bool MidiEventLib::Parser::parseMidiData(uint8_t midiByte, MidiEventLib::Event *midiEvent)
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
            MidiEventLib::Event::setNoteOnEventData(midiEvent, getMidiChannel(), _midiData[0], _midiData[1]);
            return true;
        }
        else if (isMidiNoteOff() || (isMidiNoteOn() && _midiData[1] == 0))
        {
            MidiEventLib::Event::setNoteOffEventData(midiEvent, getMidiChannel(), _midiData[0], _midiData[1]);
            return true;
        }
        else if (isMidiCC())
        {
            MidiEventLib::Event::setCCEventData(midiEvent, getMidiChannel(), _midiData[0], _midiData[1]);
            return true;
        }
    }
    return false;
}

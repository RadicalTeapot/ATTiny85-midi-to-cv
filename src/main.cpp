#include <Arduino.h>
#include <MidiRecv.h>
#include <Adafruit_MCP4728.h>
#include "calibration.h"

#define MIDI_IN_PIN (3)
#define GATE_PIN (4)

#define LOW_MIDI_NOTE (36)
#define FIRST_MIDI_CC_NUMBER (1) // ModWheel CC
#define SECOND_MIDI_CC_NUMBER (7) // Expression CC

ReceiveOnlySoftwareSerial serial(MIDI_IN_PIN);
MidiRecv midi(&serial);
MidiRecv::MIDI_DATA midiData;
Adafruit_MCP4728 dac;

uint16_t remapMidiNote(uint8_t midiNote, uint8_t lowerMidiNote);
uint16_t remapMidiValue(uint8_t midiNote, uint8_t lowerBound = 0, uint8_t upperBound = 127);

void setup()
{
    dac.begin();
    midi.begin();
    pinMode(GATE_PIN, OUTPUT);
    digitalWrite(GATE_PIN, LOW);
}

void loop()
{
    if (midi.recv(&midiData))
    {
        if (midiData.type == MidiRecv::EVENT_NOTE_ON)
        {
            dac.setChannelValue(MCP4728_CHANNEL_A, remapMidiNote(midiData.note, LOW_MIDI_NOTE));
            dac.setChannelValue(MCP4728_CHANNEL_B, remapMidiValue(midiData.velocity));
            digitalWrite(GATE_PIN, HIGH);
        }
        else if (midiData.type == MidiRecv::EVENT_NOTE_OFF)
        {
            dac.setChannelValue(MCP4728_CHANNEL_A, remapMidiNote(midiData.note, LOW_MIDI_NOTE));
            dac.setChannelValue(MCP4728_CHANNEL_B, remapMidiValue(midiData.velocity));
            digitalWrite(GATE_PIN, LOW);
        }
        else if (midiData.type == MidiRecv::EVENT_CONTROL_CHANGE)
        {
            if (midiData.note == FIRST_MIDI_CC_NUMBER)
            {
                dac.setChannelValue(MCP4728_CHANNEL_C, remapMidiValue(midiData.velocity));
            }
            else if (midiData.note == SECOND_MIDI_CC_NUMBER)
            {
                dac.setChannelValue(MCP4728_CHANNEL_D, remapMidiValue(midiData.velocity));
            }
        }
    }
}

// const float noteScaling = 4096.0f / (4.536f * 12.0f);
uint16_t remapMidiNote(uint8_t midiNote, uint8_t lowerMidiNote)
{
    // uint16_t remappedNote = static_cast<uint16_t>((midiNote - lowerMidiNote) * noteScaling);
    uint16_t remappedNote = pgm_read_word_near(lookUpValues + min(midiNote - lowerMidiNote, RANGE - 1));
    return remappedNote;
}

uint16_t remapMidiValue(uint8_t midiNote, uint8_t lowerBound, uint8_t upperBound)
{
    uint16_t remappedNote = map(midiNote, lowerBound, upperBound, 0, 4095);
    return remappedNote;
}

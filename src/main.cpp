#ifdef ARDUINO
#include <Arduino.h>
#include <MidiParser.h>
#include <Adafruit_MCP4728.h>
#include "calibration.h"
#include "DacHandler.h"

#define MIDI_IN_PIN (3)
#define GATE_PIN (4)

#define LOW_MIDI_NOTE (36)
#define FIRST_MIDI_CC_NUMBER (1) // ModWheel CC
#define SECOND_MIDI_CC_NUMBER (7) // Expression CC

ReceiveOnlySoftwareSerial serial(MIDI_IN_PIN);
MidiParser midi(&serial);
MidiEvent midiEvent;
Adafruit_MCP4728 dac;
DacHandler dacHandler;

uint16_t remapMidiNote(uint8_t midiNote, uint8_t lowerMidiNote);
uint16_t remapMidiValue(uint8_t midiNote, uint8_t lowerBound = 0, uint8_t upperBound = 127);
void writeValuesToDac(DacValues *dacValues);

void setup()
{
    // Set dac handler write function
    DacHandler::writeValuesToDac = writeValuesToDac;

    dac.begin();
    midi.begin();
    pinMode(GATE_PIN, OUTPUT);
    digitalWrite(GATE_PIN, LOW);
}

void loop()
{
    if (midi.recv(&midiEvent))
    {
        dacHandler.handleEvent(&midiEvent);
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

void writeValuesToDac(DacValues *dacValues)
{
    dac.fastWrite(dacValues->values[0], dacValues->values[1], dacValues->values[2], dacValues->values[3]);
}

#else
int main() { return 0; }
#endif

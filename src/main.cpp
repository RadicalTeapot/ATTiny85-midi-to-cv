#ifdef ARDUINO
#include <Arduino.h>
#include <Adafruit_MCP4728.h>

#include "calibration.h"
#include "DacEventHandlerFactory.h"
#include "DacHandler.h"
#include "MidiParser.h"
#include "Preset.h"

#define MIDI_IN_PIN (3)
#define GATE_PIN (4)

#define LOW_MIDI_NOTE (36)

ReceiveOnlySoftwareSerial serial(MIDI_IN_PIN);
MidiParser midi(&serial);
MidiEvent midiEvent;
Adafruit_MCP4728 dac;
DacHandler dacHandler;

Preset preset0 = {
    {
        0x01, // Note channels
        0x00, // CC number 1
        0x01, // CC number 2
        0x02, // CC number 3
        0x03, // CC number 4
        0x00, // CC channels 1
        0x00, // CC channels 2
    },
    {
        0x23, // Note channels
        0x04, // CC number 1
        0x05, // CC number 2
        0x06, // CC number 3
        0x07, // CC number 4
        0x00, // CC channels 1
        0x00, // CC channels 2
    }
};

uint16_t remapMidiNote(uint8_t midiNote, uint8_t lowerMidiNote);
uint16_t remapMidiValue(uint8_t midiNote, uint8_t lowerBound = 0, uint8_t upperBound = 127);
void writeValuesToDac(DacValues *dacValues);

void setup()
{
    // Set dac handler write function
    DacHandler::writeValuesToDac = writeValuesToDac;
    dacHandler.setHandler(DacEventHandlerFactory::createEventHandler(&preset0.dacConfigA, false));

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
    dac.fastWrite(
        remapMidiNote(dacValues->values[0], LOW_MIDI_NOTE),
        remapMidiValue(dacValues->values[1], 0, 1 << 7),
        remapMidiNote(dacValues->values[2], LOW_MIDI_NOTE),
        remapMidiValue(dacValues->values[3], 0, 1 << 7)
    );
}

#else
int main() { return 0; }
#endif

#ifdef ARDUINO
#include <Arduino.h>
#include <Adafruit_MCP4728.h>

#include "DefaultPresets.h"
#include "DacEventHandlerFactory.h"
#include "DacHandler.h"
#include "DacPitchCalibrationLookUpTable.h"
#include "MidiParser.h"
#include "MidiSerialCommunication.h"
#include "Preset.h"

#define MIDI_IN_PIN (1)
#define LOW_MIDI_NOTE (36)
#define DAC_A_ADDRESS (0x60)

ReceiveOnlySoftwareSerial serial(MIDI_IN_PIN);
MidiParser midi;
MidiSerialCommunication midiSerial(&serial, &midi);
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
    dacHandler.setHandler(DacEventHandlerFactory::createEventHandler(&preset0.dacConfigA, true));

    dac.begin(DAC_A_ADDRESS);
    midiSerial.begin();
}

void loop()
{
    if (midiSerial.recv(&midiEvent))
    {
        dacHandler.handleEvent(&midiEvent);
    }
}

uint16_t remapMidiNote(uint8_t midiNote, uint8_t lowerMidiNote)
{
    uint16_t remappedNote = pgm_read_word_near(DacPitchCalibrationLookUpTable + min(midiNote - lowerMidiNote, RANGE - 1));
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

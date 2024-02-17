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
#include "ShiftRegisterHandler.h"
#include "SwitchHandler.h"

#define BAUD_RATE (31250U)

#define MIDI_IN_PIN (1U)
#define SWITCH_A_PIN (3U)
#define SWITCH_B_PIN (4U)
#define LOW_MIDI_NOTE (36U)
#define DAC_A_ADDRESS (0x60U)
#define DAC_B_ADDRESS (0x61U)
#define SLAVE_ADDRESS (0x62U)

#define DAC_COUNT (2U)

ReceiveOnlySoftwareSerial serial(MIDI_IN_PIN);
MidiParser midiParser;
MidiEvent midiEvent;

Adafruit_MCP4728 DACs[DAC_COUNT];
DacHandler dacHandlers[DAC_COUNT];

ShiftRegisterHandler shiftRegisterHandler;

uint8_t switchesState = 0, previousSwitchesState = 0;

static inline uint16_t remapMidiNote(uint8_t midiNote, uint8_t lowerMidiNote);
static inline uint16_t remapMidiValue(uint8_t midiNote, uint8_t lowerBound = 0, uint8_t upperBound = 127);
static void writeValuesToFirstDac(DacValues *dacValues);
static void writeValuesToSecondDac(DacValues *dacValues);
static void writeValuesToShiftRegister(uint8_t values);

void setup()
{
    SwitchHandler::initSwitches<SWITCH_A_PIN, SWITCH_B_PIN>();
    SwitchHandler::getSwitchesState<SWITCH_A_PIN, SWITCH_B_PIN>(&switchesState);
    previousSwitchesState = switchesState;

    // Set dac handler write function
    dacHandlers[0] = DacHandler(writeValuesToFirstDac);
    dacHandlers[0].setHandler(DacEventHandlerFactory::createEventHandler(&preset0.dacConfigA, switchesState & 1));

    dacHandlers[1] = DacHandler(writeValuesToSecondDac);
    dacHandlers[1].setHandler(DacEventHandlerFactory::createEventHandler(&preset0.dacConfigB, switchesState & 2));

    shiftRegisterHandler.updateHandlersFromFirstDacConfig(&preset0.dacConfigA, switchesState & 1);
    shiftRegisterHandler.updateHandlersFromSecondDacConfig(&preset0.dacConfigB, switchesState & 2);
    shiftRegisterHandler.writeValuesToShiftRegister = writeValuesToShiftRegister;

    DACs[0] = Adafruit_MCP4728();
    DACs[0].begin(DAC_A_ADDRESS);
    DACs[1] = Adafruit_MCP4728();
    DACs[1].begin(DAC_B_ADDRESS);
    MidiSerialCommunication::begin<BAUD_RATE>(&serial);
}

void loop()
{
    SwitchHandler::getSwitchesState<SWITCH_A_PIN, SWITCH_B_PIN>(&switchesState);
    if (switchesState != previousSwitchesState)
    {
        dacHandlers[0].setHandler(DacEventHandlerFactory::createEventHandler(&preset0.dacConfigA, switchesState & 1));
        dacHandlers[1].setHandler(DacEventHandlerFactory::createEventHandler(&preset0.dacConfigB, switchesState & 2));
        shiftRegisterHandler.updateHandlersFromFirstDacConfig(&preset0.dacConfigA, switchesState & 1);
        shiftRegisterHandler.updateHandlersFromSecondDacConfig(&preset0.dacConfigB, switchesState & 2);
        previousSwitchesState = switchesState;
    }

    if (MidiSerialCommunication::recv(&midiEvent, &serial, &midiParser))
    {
        dacHandlers[0].handleEvent(&midiEvent);
        dacHandlers[1].handleEvent(&midiEvent);
        shiftRegisterHandler.processEvent(&midiEvent);
    }
}

static inline uint16_t remapMidiNote(uint8_t midiNote, uint8_t lowerMidiNote)
{
    return pgm_read_word_near(DacPitchCalibrationLookUpTable + min(midiNote - lowerMidiNote, RANGE - 1));
}

static inline uint16_t remapMidiValue(uint8_t midiNote, uint8_t lowerBound, uint8_t upperBound)
{
    return map(midiNote, lowerBound, upperBound, 0, 4095);
}

static void writeValuesToFirstDac(DacValues *dacValues)
{
    DACs[0].fastWrite(
        remapMidiNote(dacValues->values[0], LOW_MIDI_NOTE),
        remapMidiValue(dacValues->values[1], 0, 1 << 7),
        remapMidiNote(dacValues->values[2], LOW_MIDI_NOTE),
        remapMidiValue(dacValues->values[3], 0, 1 << 7)
    );
}

static void writeValuesToSecondDac(DacValues *dacValues)
{
    DACs[1].fastWrite(
        remapMidiNote(dacValues->values[0], LOW_MIDI_NOTE),
        remapMidiValue(dacValues->values[1], 0, 1 << 7),
        remapMidiNote(dacValues->values[2], LOW_MIDI_NOTE),
        remapMidiValue(dacValues->values[3], 0, 1 << 7)
    );
}

void writeValuesToShiftRegister(uint8_t values)
{
    // TODO Send message to slave ATTiny85
}

#else
int main() { return 0; }
#endif

#ifdef MASTER
#include <Arduino.h>
#include <Adafruit_MCP4728.h>

#include "DefaultPresets.h"
#include "DacEventHandlerFactory.h"
#include "DacHandler.h"
#include "MidiParser.h"
#include "MidiSerialCommunication.h"
#include "Preset.h"
#include "ShiftRegisterHandler.h"
#include "SwitchHandler.h"
#include "CommunicationHandler.h"

#define BAUD_RATE (31250U)
#define MIDI_MAX_VALUE (127U)

#define MIDI_IN_PIN (1U)
#define SWITCH_A_PIN (3U)
#define SWITCH_B_PIN (4U)
#define LOW_MIDI_NOTE (36U)
#define DAC_A_ADDRESS (0x60U)
#define DAC_B_ADDRESS (0x61U)

#define DAC_COUNT (2U)

ReceiveOnlySoftwareSerial serial(MIDI_IN_PIN);
MidiParser midiParser;
MidiEvent midiEvent;

Adafruit_MCP4728 DACs[DAC_COUNT];
DacHandler dacHandlers[DAC_COUNT];

ShiftRegisterHandler shiftRegisterHandler;

uint8_t switchesState = 0, previousSwitchesState = 0;

static void writeValuesToShiftRegister(uint8_t values);

template<uint8_t dacIndex>
static void writeValuesToDac(DacValues *dacValues)
{
    DACs[dacIndex].fastWrite(dacValues->values[0], dacValues->values[1], dacValues->values[2], dacValues->values[3]);
};

void setup()
{
    SwitchHandler::initSwitches<SWITCH_A_PIN, SWITCH_B_PIN>();
    SwitchHandler::getSwitchesState<SWITCH_A_PIN, SWITCH_B_PIN>(&switchesState);
    previousSwitchesState = switchesState;

    DacEventHandlerFactory::Factory::noteValueMapper = &DacEventHandlerFactory::dacNoteValueMapper<LOW_MIDI_NOTE, RANGE, MIDI_MAX_VALUE>;
    DacEventHandlerFactory::Factory::ccValueMapper = &DacEventHandlerFactory::dacCCValueMapper<MIDI_MAX_VALUE>;

    // Set dac handler write function
    const uint8_t firstDacIndex = 0;
    DacHandler::WriteValuesToDac writeValuesToFirstDac = &writeValuesToDac<firstDacIndex>;
    dacHandlers[0] = DacHandler(writeValuesToFirstDac);
    dacHandlers[0].setHandler(DacEventHandlerFactory::Factory::createEventHandler(&preset0.dacConfigA, switchesState & 1));

    const uint8_t secondDacIndex = 1;
    DacHandler::WriteValuesToDac writeValuesToSecondDac = &writeValuesToDac<secondDacIndex>;
    dacHandlers[1] = DacHandler(writeValuesToSecondDac);
    dacHandlers[1].setHandler(DacEventHandlerFactory::Factory::createEventHandler(&preset0.dacConfigB, switchesState & 2));

    shiftRegisterHandler.updateHandlersFromFirstDacConfig(&preset0.dacConfigA, switchesState & 1);
    shiftRegisterHandler.updateHandlersFromSecondDacConfig(&preset0.dacConfigB, switchesState & 2);
    shiftRegisterHandler.writeValuesToShiftRegister = writeValuesToShiftRegister;

    DACs[0] = Adafruit_MCP4728();
    DACs[0].begin(DAC_A_ADDRESS);
    DACs[1] = Adafruit_MCP4728();
    DACs[1].begin(DAC_B_ADDRESS);

    MidiSerialCommunication::begin<BAUD_RATE>(&serial);

    CommunicationHandler::init();
    CommunicationHandler::sendData<SLAVE_ADDRESS>(0);
}

void loop()
{
    SwitchHandler::getSwitchesState<SWITCH_A_PIN, SWITCH_B_PIN>(&switchesState);
    if (switchesState != previousSwitchesState)
    {
        dacHandlers[0].setHandler(DacEventHandlerFactory::Factory::createEventHandler(&preset0.dacConfigA, switchesState & 1));
        dacHandlers[1].setHandler(DacEventHandlerFactory::Factory::createEventHandler(&preset0.dacConfigB, switchesState & 2));
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

void writeValuesToShiftRegister(uint8_t values)
{
    CommunicationHandler::sendData<SLAVE_ADDRESS>(values);
}
#endif

#include <unity.h>
#include "DacHandler.h"
#include "DacEventHandlerFactory.h"
#include "MidiEvent.h"
#include "Preset.h"

const uint8_t midiMinValue = 0;
const uint8_t midiMaxValue = 127;
const uint8_t lowerMidiNote = 0;
const uint8_t range = DacPitchCalibration::RANGE;

uint8_t values[4];
void setUp() {
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
}

void tearDown() {}

const DacPresetConfig dacPresetConfig = {
    0x01, // Note channels (1 and 2)
    0x00, // CC number 1 (CC1)
    0x01, // CC number 2 (CC2)
    0x02, // CC number 3 (CC3)
    0x03, // CC number 4 (CC4)
    0x00, // CC channels 1 (0)
    0x00, // CC channels 2 (0)
};

void mock_writeValuesToDac(DacValues *dacValues) {
    values[0] = dacValues->values[0];
    values[1] = dacValues->values[1];
    values[2] = dacValues->values[2];
    values[3] = dacValues->values[3];
}

void test_DacHandler_handleEvent_nullHandlerPointer_DontHandle() {
    DacHandler dacHandler;
    const MidiEvent event = {MidiEventType::NOTE_ON, 0, 1, 1};
    TEST_ASSERT_FALSE(dacHandler.handleEvent(&event));
}

void test_DacHandler_handleEvent_nullMidiEventPointer_DontHandle() {
    DacHandler dacHandler;
    DacEventHandlerFactory<midiMinValue, midiMaxValue, lowerMidiNote, range> factory;
    DacEventHandler *eventHandler = factory.createEventHandler(&dacPresetConfig, true);
    dacHandler.setHandler(eventHandler);
    TEST_ASSERT_FALSE(dacHandler.handleEvent(nullptr));
}

void test_DacHandler_MidiNoteOnEvent_handleEvent() {
    DacHandler dacHandler(mock_writeValuesToDac);
    DacEventHandlerFactory<midiMinValue, midiMaxValue, lowerMidiNote, range> factory;
    DacEventHandler *eventHandler = factory.createEventHandler(&dacPresetConfig, true);
    dacHandler.setHandler(eventHandler);

    const MidiEvent event = {MidiEventType::NOTE_ON, 0, 1, 1};
    TEST_ASSERT_TRUE(dacHandler.handleEvent(&event));

    uint16_t expectedValue = factory.valueRemapper.remapNote(1);
    TEST_ASSERT_EQUAL(expectedValue, values[0]);
    expectedValue = factory.valueRemapper.remapVelocity(1);
    TEST_ASSERT_EQUAL(expectedValue, values[1]);
    expectedValue = factory.valueRemapper.remapNote(0);
    TEST_ASSERT_EQUAL(expectedValue, values[2]);
    expectedValue = factory.valueRemapper.remapVelocity(0);
    TEST_ASSERT_EQUAL(expectedValue, values[3]);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_DacHandler_handleEvent_nullHandlerPointer_DontHandle);
    RUN_TEST(test_DacHandler_handleEvent_nullMidiEventPointer_DontHandle);
    RUN_TEST(test_DacHandler_MidiNoteOnEvent_handleEvent);
    UNITY_END();
}

#include <unity.h>
#include "DacEventHandlerFactory.h"
#include "MidiEvent.h"

const uint8_t midiMinValue = 0;
const uint8_t midiMaxValue = 127;
const uint8_t lowerMidiNote = 0;
const uint8_t range = DacPitchCalibration::RANGE;

const DacPresetConfig dacPresetConfig = {
    0x01, // Note channels (1 and 2)
    0x00, // CC number 1 (CC1)
    0x01, // CC number 2 (CC2)
    0x02, // CC number 3 (CC3)
    0x03, // CC number 4 (CC4)
    0x00, // CC channels 1 (0)
    0x00, // CC channels 2 (0)
};

void setUp() {}

void tearDown() {}

void test_DacEventHandlerFactory_createNoteEventHandler() {
    DacPresetConfig dacConfig;
    DacEventHandlerFactory<midiMinValue, midiMaxValue, lowerMidiNote, range> factory;
    DacEventHandler *noteEventHandler = factory.createEventHandler(&dacPresetConfig, true);
    TEST_ASSERT_NOT_NULL(noteEventHandler);

    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 2};
    DacValues dacValues;
    TEST_ASSERT_TRUE(noteEventHandler->handleEvent(&noteOnEvent, &dacValues));
    uint8_t expectedValue = factory.valueRemapper.remapNote(1);
    TEST_ASSERT_EQUAL(expectedValue, dacValues.values[0]);
    expectedValue = factory.valueRemapper.remapVelocity(2);
    TEST_ASSERT_EQUAL(expectedValue, dacValues.values[1]);
}

void test_DacEventHandlerFactory_createCCEventHandler() {
    DacPresetConfig dacConfig;
    dacConfig.CCChannels1 = 0x00;
    dacConfig.CCNumber1 = 1;
    DacEventHandlerFactory<midiMinValue, midiMaxValue, lowerMidiNote, range> factory;
    DacEventHandler *ccEventHandler = factory.createEventHandler(&dacConfig, false);
    TEST_ASSERT_NOT_NULL(ccEventHandler);

    const MidiEvent ccEvent = {MidiEventType::CC, 0, 1, 1};
    DacValues dacValues;
    TEST_ASSERT_TRUE(ccEventHandler->handleEvent(&ccEvent, &dacValues));
    const uint8_t expectedValue = factory.valueRemapper.remapCC(1);
    TEST_ASSERT_EQUAL(expectedValue, dacValues.values[0]);
}

// TODO test various mapping functions

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_DacEventHandlerFactory_createNoteEventHandler);
    RUN_TEST(test_DacEventHandlerFactory_createCCEventHandler);
    UNITY_END();
}

#include <unity.h>
#include "DacEventHandlerContainer.h"
#include "Utils/DacPitchCalibrationLookUpTable.h"
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
    DacEventHandlerContainer container;
    container.configureHandlers(&dacPresetConfig, true);

    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 2};
    DacValues dacValues;
    TEST_ASSERT_TRUE(container.handleEvent(&noteOnEvent, &dacValues));
    uint8_t expectedValue = ValueRemapper::remapNote(1);
    TEST_ASSERT_EQUAL(expectedValue, dacValues.values[0]);
    expectedValue = ValueRemapper::remapMidiValue(2);
    TEST_ASSERT_EQUAL(expectedValue, dacValues.values[1]);
}

void test_DacEventHandlerFactory_createCCEventHandler() {
    DacEventHandlerContainer container;
    container.configureHandlers(&dacPresetConfig, false);

    const MidiEvent ccEvent = {MidiEventType::CC, 0, 0, 1};
    DacValues dacValues;
    TEST_ASSERT_TRUE(container.handleEvent(&ccEvent, &dacValues));
    const uint8_t expectedValue = ValueRemapper::remapMidiValue(1);
    TEST_ASSERT_EQUAL(expectedValue, dacValues.values[0]);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_DacEventHandlerFactory_createNoteEventHandler);
    RUN_TEST(test_DacEventHandlerFactory_createCCEventHandler);
    UNITY_END();
}

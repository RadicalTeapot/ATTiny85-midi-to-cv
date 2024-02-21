#include <unity.h>
#include "DacHandler.h"
#include "DacEventHandlerFactory.h"
#include "MidiEvent.h"
#include "Preset.h"

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
    DacPresetConfig dacConfig;
    DacEventHandlerFactory::Factory factory;
    DacEventHandler *eventHandler = factory.createEventHandler(&dacConfig, true);
    dacHandler.setHandler(eventHandler);
    TEST_ASSERT_FALSE(dacHandler.handleEvent(nullptr));
}

void test_DacHandler_MidiNoteOnEvent_handleEvent() {
    DacEventHandlerFactory::Factory factory;
    DacHandler dacHandler(mock_writeValuesToDac);
    DacEventHandler *eventHandler = factory.createEventHandler(&dacPresetConfig, true);
    dacHandler.setHandler(eventHandler);

    const MidiEvent event = {MidiEventType::NOTE_ON, 0, 1, 1};
    TEST_ASSERT_TRUE(dacHandler.handleEvent(&event));
    TEST_ASSERT_EQUAL(1, values[0]);
    TEST_ASSERT_EQUAL(1, values[1]);
    TEST_ASSERT_EQUAL(0, values[2]);
    TEST_ASSERT_EQUAL(0, values[3]);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_DacHandler_handleEvent_nullHandlerPointer_DontHandle);
    RUN_TEST(test_DacHandler_handleEvent_nullMidiEventPointer_DontHandle);
    RUN_TEST(test_DacHandler_MidiNoteOnEvent_handleEvent);
    UNITY_END();
}

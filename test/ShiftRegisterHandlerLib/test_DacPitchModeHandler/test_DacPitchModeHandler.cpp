#include <unity.h>
#include "EventHandlers/DacPitchModeHandler.h"

void setUp() {}

void tearDown() {}

void test_DacPitchModeHandler_ValidEvents_ValidOutput()
{
    const uint8_t channels = 0x01;
    DacPitchModeHandler<0> handler(channels);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    uint8_t values = 0B00000000;
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOnEvent, &values));
    TEST_ASSERT_EQUAL(0B00000001, values);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOffEvent, &values));
    TEST_ASSERT_EQUAL(0B00000000, values);

    noteOnEvent = {MidiEventType::NOTE_ON, 1, 1, 1};
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOnEvent, &values));
    TEST_ASSERT_EQUAL(0B00000010, values);
    noteOffEvent = {MidiEventType::NOTE_OFF, 1, 1, 1};
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOffEvent, &values));
    TEST_ASSERT_EQUAL(0B00000000, values);
}

// TODO Write more tests

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_DacPitchModeHandler_ValidEvents_ValidOutput);
    UNITY_END();
}

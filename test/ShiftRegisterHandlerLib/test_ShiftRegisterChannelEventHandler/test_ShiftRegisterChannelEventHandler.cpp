#include <unity.h>
#include "EventHandlers/ShiftRegisterChannelEventHandler.h"

void setUp() {}

void tearDown() {}

void test_DacPitchModeHandler_ValidEvents_ValidOutput()
{
    ShiftRegisterChannelEventHandler handler(0);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    TEST_ASSERT_TRUE(handler.processEvent(&noteOnEvent));
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&noteOffEvent));
}

// TODO Write more tests

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_DacPitchModeHandler_ValidEvents_ValidOutput);
    UNITY_END();
}

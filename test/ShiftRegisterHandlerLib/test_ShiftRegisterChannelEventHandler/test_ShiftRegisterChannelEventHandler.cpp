#include <unity.h>
#include "EventHandlers/ShiftRegisterChannelEventHandler.h"

void setUp() {}

void tearDown() {}

void test_DacPitchModeHandler_ValidEvents_ValidOutput()
{
    ShiftRegisterChannelEventHandler<0> handler;
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOnEvent));
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_FALSE(handler.handleEvent(&noteOffEvent));
}

// TODO Write more tests

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_DacPitchModeHandler_ValidEvents_ValidOutput);
    UNITY_END();
}

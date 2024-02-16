#include <unity.h>
#include "ShiftRegisterEventHandlerContainer.h"

void setUp() {}

void tearDown() {}

void test_ShiftRegisterEventHandlerContainer_defaultState()
{
    ShiftRegisterEventHandlerContainer container;
    uint8_t notes[8] = DEFAULT_NOTES;
    MidiEvent midiEvent = {MidiEventType::NOTE_ON, DEFAULT_CHANNEL, 0, 0};
    uint8_t expectedResult = 0, result = 0;
    for (int i = 0; i < 8; i++)
    {
        midiEvent.firstByte = notes[i];
        container.processEvent(&midiEvent, &result);
        expectedResult |= 1 << i;
        TEST_ASSERT_EQUAL(expectedResult, result);
    }
    midiEvent.type = MidiEventType::NOTE_OFF;
    for (int i = 0; i < 8; i++)
    {
        midiEvent.firstByte = notes[i];
        container.processEvent(&midiEvent, &result);
        expectedResult &= ~(1 << i);
        TEST_ASSERT_EQUAL(expectedResult, result);
    }
}

// TODO: Add more tests

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_defaultState);
    return UNITY_END();
}

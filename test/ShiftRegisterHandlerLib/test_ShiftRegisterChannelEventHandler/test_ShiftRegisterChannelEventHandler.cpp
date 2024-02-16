#include <unity.h>
#include "EventHandlers/ShiftRegisterChannelEventHandler.h"

void setUp() {}

void tearDown() {}

void test_ShiftRegisterChannelEventHandler_ValidNoteOnEvent_ReturnsTrue()
{
    ShiftRegisterChannelEventHandler handler(0);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    const uint8_t state = 0;
    TEST_ASSERT_TRUE(handler.processEvent(&noteOnEvent, &state));
}

void test_ShiftRegisterChannelEventHandler_ValidNoteOffEvent_ReturnsFalse()
{
    ShiftRegisterChannelEventHandler handler(0);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    const uint8_t state = 0;
    TEST_ASSERT_FALSE(handler.processEvent(&noteOffEvent, &state));
}

void test_ShiftRegisterChannelEventHandler_InvalidMidiEvent_ReturnsState()
{
    ShiftRegisterChannelEventHandler handler(0);

    // test invalid channel
    uint8_t state = 0;
    MidiEvent event = {MidiEventType::NOTE_ON, 1, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&event, &state));
    state = 1;
    TEST_ASSERT_TRUE(handler.processEvent(&event, &state));

    // test invalid event type
    state = 0;
    event = {MidiEventType::CC, 0, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&event, &state));
    state = 1;
    TEST_ASSERT_TRUE(handler.processEvent(&event, &state));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_ShiftRegisterChannelEventHandler_ValidNoteOnEvent_ReturnsTrue);
    RUN_TEST(test_ShiftRegisterChannelEventHandler_ValidNoteOffEvent_ReturnsFalse);
    RUN_TEST(test_ShiftRegisterChannelEventHandler_InvalidMidiEvent_ReturnsState);
    UNITY_END();
}

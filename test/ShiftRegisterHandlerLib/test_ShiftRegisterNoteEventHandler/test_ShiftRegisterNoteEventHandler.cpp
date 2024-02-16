#include <unity.h>
#include "EventHandlers/ShiftRegisterNoteEventHandler.h"

void setUp() {}

void tearDown() {}

void test_ShiftRegisterNoteHandler_ValidMidiNoteOnEvent_ValidOutput()
{
    ShiftRegisterNoteEventHandler<0> handler(0);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 0, 1};
    const uint8_t state = 0;
    TEST_ASSERT_TRUE(handler.processEvent(&noteOnEvent, &state));
}

void test_ShiftRegisterNoteHandler_ValidMidiNoteOffEvent_ValidOutput()
{
    ShiftRegisterNoteEventHandler<0> handler(0);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 0, 1};
    const uint8_t state = 1;
    TEST_ASSERT_FALSE(handler.processEvent(&noteOffEvent, &state));
}

void test_ShiftRegisterNoteHandler_InvalidMidiEvent_ReturnsState()
{
    ShiftRegisterNoteEventHandler<0> handler(0);

    // test invalid channel
    uint8_t state = 0;
    MidiEvent event = {MidiEventType::NOTE_ON, 1, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&event, &state));
    state = 1;
    TEST_ASSERT_TRUE(handler.processEvent(&event, &state));

    // test invalid note
    state = 0;
    event = {MidiEventType::NOTE_ON, 0, 2, 1};
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
    RUN_TEST(test_ShiftRegisterNoteHandler_ValidMidiNoteOnEvent_ValidOutput);
    RUN_TEST(test_ShiftRegisterNoteHandler_ValidMidiNoteOffEvent_ValidOutput);
    RUN_TEST(test_ShiftRegisterNoteHandler_InvalidMidiEvent_ReturnsState);
    UNITY_END();
}

#include <unity.h>
#include "EventHandlers/ShiftRegisterNoteEventHandler.h"

void setUp() {}

void tearDown() {}

void test_StaticNotesHandler_ValidMidiNoteOnEvent_ValidOutput()
{
    ShiftRegisterNoteEventHandler<0> handler(0);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 0, 1};
    TEST_ASSERT_TRUE(handler.processEvent(&noteOnEvent));
}

void test_StaticNotesHandler_ValidMidiNoteOffEvent_ValidOutput()
{
    ShiftRegisterNoteEventHandler<0> handler(0);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 0, 1};
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 0, 1};
    handler.processEvent(&noteOnEvent);
    TEST_ASSERT_FALSE(handler.processEvent(&noteOffEvent));
}

void test_StaticNotesHandler_InvalidMidiEvent_ReturnsFalse()
{
    ShiftRegisterNoteEventHandler<0> handler(0);

    // test invalid channel
    MidiEvent event = {MidiEventType::NOTE_ON, 1, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&event));

    // test invalid note
    event = {MidiEventType::NOTE_ON, 0, 2, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&event));

    // test invalid event type
    event = {MidiEventType::CC, 0, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&event));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_StaticNotesHandler_ValidMidiNoteOnEvent_ValidOutput);
    RUN_TEST(test_StaticNotesHandler_ValidMidiNoteOffEvent_ValidOutput);
    RUN_TEST(test_StaticNotesHandler_InvalidMidiEvent_ReturnsFalse);
    UNITY_END();
}

#include <unity.h>
#include "EventHandlers/StaticNotesHandler.h"

void setUp() {}

void tearDown() {}

void test_StaticNotesHandler_ValidMidiNoteOnEvent_ValidOutput()
{
    const uint8_t notes[2] = {0, 1};
    StaticNotesHandler<2, 0> handler(notes);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, notes[0], 1};
    uint8_t values = 0B00000000;;
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOnEvent, &values));
    TEST_ASSERT_EQUAL(0B00000001, values);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, notes[1], 1};
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOnEvent, &values));
    TEST_ASSERT_EQUAL(0B00000011, values);
}

void test_StaticNotesHandler_ValidMidiNoteOffEvent_ValidOutput()
{
    const uint8_t notes[2] = {0, 1};
    StaticNotesHandler<2, 0> handler(notes);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, notes[0], 1};
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, notes[0], 1};
    uint8_t values = 0B00000000;;
    handler.handleEvent(&noteOnEvent, &values);
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOffEvent, &values));
    TEST_ASSERT_EQUAL(0B00000000, values);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, notes[1], 1};
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, notes[1], 1};
    handler.handleEvent(&noteOnEvent, &values);
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOffEvent, &values));
    TEST_ASSERT_EQUAL(0B00000000, values);
}

void test_StaticNotesHandler_InvalidMidiEvent_ReturnsFalse()
{
    const uint8_t notes[2] = {0, 1};
    StaticNotesHandler<2, 0> handler(notes);

    // test invalid channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 1, 1, 1};
    uint8_t values = 0B00000000;;
    TEST_ASSERT_FALSE(handler.handleEvent(&noteOnEvent, &values));

    // test invalid note
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    TEST_ASSERT_FALSE(handler.handleEvent(&noteOnEvent, &values));

    // test invalid event type
    noteOnEvent = {MidiEventType::CC, 0, 1, 1};
    TEST_ASSERT_FALSE(handler.handleEvent(&noteOnEvent, &values));
}

void test_StaticNotesHandler_ValidMidiEvent_Offset_ValidOutput()
{
    const uint8_t notes[2] = {0, 1};
    StaticNotesHandler<2, 2> handler(notes);
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, notes[0], 1};
    uint8_t values = 0B00000000;;
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOnEvent, &values));
    TEST_ASSERT_EQUAL(0B00000100, values);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, notes[1], 1};
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOnEvent, &values));
    TEST_ASSERT_EQUAL(0B00001100, values);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, notes[0], 1};
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOffEvent, &values));
    TEST_ASSERT_EQUAL(0B00001000, values);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, notes[1], 1};
    TEST_ASSERT_TRUE(handler.handleEvent(&noteOffEvent, &values));
    TEST_ASSERT_EQUAL(0B00000000, values);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_StaticNotesHandler_ValidMidiNoteOnEvent_ValidOutput);
    RUN_TEST(test_StaticNotesHandler_ValidMidiNoteOffEvent_ValidOutput);
    RUN_TEST(test_StaticNotesHandler_InvalidMidiEvent_ReturnsFalse);
    RUN_TEST(test_StaticNotesHandler_ValidMidiEvent_Offset_ValidOutput);
    UNITY_END();
}

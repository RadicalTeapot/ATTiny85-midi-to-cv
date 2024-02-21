#include <unity.h>
#include "eventHandlers/NoteEventHandler.h"
#include "MidiEvent.h"

void setUp() {}

void tearDown() {}

void mockValueMapper(const uint8_t values[4], DacValues *dacValues)
{
    dacValues->values[0] = values[0];
    dacValues->values[1] = values[1];
    dacValues->values[2] = values[2];
    dacValues->values[3] = values[3];
}

void test_NoteEventHandler_midiCCEvent_dontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};

    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&ccEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEvent_Handle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    TEST_ASSERT_TRUE(noteEventHandler->handleEvent(&noteOnEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOffEventWithMatchingNoteOnEvent_Handle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};

    // test simple matching
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_TRUE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));

    // test matching of latest note on
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    TEST_ASSERT_TRUE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOffEventWithNotMatchingNoteOnEvent_DontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;

    // test no note on event
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));

    // test note off event not matching any note on
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));

    // test note off event discarding matched note on
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));

    // test note off event matching only latest note on
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 3, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnOrOff_nullDacEventPointer_DontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    const MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};

    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOnEvent, nullptr));
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, nullptr));
}

void test_NoteEventHandler_nullMidiEventPointer_DontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;

    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(nullptr, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEventNotMatchingChannelAndNumber_DontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 2, 2, 1};
    const MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 2, 2, 1};

    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOnEvent, &dacValues));
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;

    // test match first midi channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(2, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(0, dacValues.values[3]); // velocity

    // test match second midi channel
    noteOnEvent = {MidiEventType::NOTE_ON, 1, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(2, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(2, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[3]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;

    // test match first midi channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(0, dacValues.values[3]); // velocity

    // test match second midi channel
    noteOnEvent = {MidiEventType::NOTE_ON, 1, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 1, 2, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[3]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_legatoInOrder_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;

    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 3, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(2, dacValues.values[0]); // note
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
}


void test_NoteEventHandler_midiNoteOffEvent_legatoOutOfOrder_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;

    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 3, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(3, dacValues.values[0]); // note
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
}

void test_NoteEventHandler_midiNoteOffEvent_successiveNotes_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(mockValueMapper, 0, 1);
    DacValues dacValues;

    // Same notes
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[0]); // note

    // Different notes
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 3, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 4, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 4, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[0]); // note
}

void doubleValueMapper(const uint8_t values[4], DacValues *dacValues)
{
    dacValues->values[0] = values[0] * 2;
    dacValues->values[1] = values[1] * 2;
    dacValues->values[2] = values[2] * 2;
    dacValues->values[3] = values[3] * 2;
}

void test_NoteEventHandler_midiNoteOnEvent_DoubleValueMapper_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(doubleValueMapper, 0, 1);
    DacValues dacValues;

    // test match first midi channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(4, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(2, dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(0, dacValues.values[3]); // velocity

    // test match second midi channel
    noteOnEvent = {MidiEventType::NOTE_ON, 1, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(4, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(2, dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(4, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(2, dacValues.values[3]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_DoubleValueMapper_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(doubleValueMapper, 0, 1);
    DacValues dacValues;

    // test match first midi channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(2, dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(0, dacValues.values[3]); // velocity

    // test match second midi channel
    noteOnEvent = {MidiEventType::NOTE_ON, 1, 2, 1};
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 1, 2, 1};
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(2, dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(2, dacValues.values[3]); // velocity
}
int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_NoteEventHandler_midiCCEvent_dontHandle);
    RUN_TEST(test_NoteEventHandler_midiNoteOnEvent_Handle);
    RUN_TEST(test_NoteEventHandler_midiNoteOffEventWithMatchingNoteOnEvent_Handle);
    RUN_TEST(test_NoteEventHandler_midiNoteOffEventWithNotMatchingNoteOnEvent_DontHandle);
    RUN_TEST(test_NoteEventHandler_midiNoteOnOrOff_nullDacEventPointer_DontHandle);
    RUN_TEST(test_NoteEventHandler_nullMidiEventPointer_DontHandle);
    RUN_TEST(test_NoteEventHandler_midiNoteOnEventNotMatchingChannelAndNumber_DontHandle);
    RUN_TEST(test_NoteEventHandler_midiNoteOnEvent_MatchingChannelAndNumberAndValue_ValidDacEvent);
    RUN_TEST(test_NoteEventHandler_midiNoteOffEvent_MatchingChannelAndNumberAndValue_ValidDacEvent);
    RUN_TEST(test_NoteEventHandler_midiNoteOffEvent_legatoInOrder_ValidDacEvent);
    RUN_TEST(test_NoteEventHandler_midiNoteOffEvent_legatoOutOfOrder_ValidDacEvent);
    RUN_TEST(test_NoteEventHandler_midiNoteOffEvent_successiveNotes_ValidDacEvent);
    RUN_TEST(test_NoteEventHandler_midiNoteOnEvent_DoubleValueMapper_ValidDacEvent);
    RUN_TEST(test_NoteEventHandler_midiNoteOffEvent_DoubleValueMapper_ValidDacEvent);
    UNITY_END();
}

#include <unity.h>
#include "eventHandlers/NoteEventHandler.h"

void setUp() {}

void tearDown() {}

void test_NoteEventHandler_midiCCEvent_dontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;
    MIDICCEvent ccEvent = MIDICCEvent(1, 1, 1);

    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&ccEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEvent_Handle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;
    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 1, 1);
    TEST_ASSERT_TRUE(noteEventHandler->handleEvent(&noteOnEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOffEventWithMatchingNoteOnEvent_Handle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;
    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 1, 1);
    MIDINoteOffEvent noteOffEvent = MIDINoteOffEvent(0, 1, 1);

    // test simple matching
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_TRUE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));

    // test matching of latest note on
    noteOnEvent = MIDINoteOnEvent(0, 1, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = MIDINoteOffEvent(0, 2, 1);
    TEST_ASSERT_TRUE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOffEventWithNotMatchingNoteOnEvent_DontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;

    // test no note on event
    MIDINoteOffEvent noteOffEvent = MIDINoteOffEvent(0, 1, 1);
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));

    // test note off event not matching any note on
    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = MIDINoteOffEvent(0, 1, 1);
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));

    // test note off event discarding matched note on
    noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = MIDINoteOffEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));

    // test note off event matching only latest note on
    noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = MIDINoteOnEvent(0, 3, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = MIDINoteOffEvent(0, 2, 1);
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnOrOff_nullDacEventPointer_DontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    MIDINoteOffEvent noteOffEvent = MIDINoteOffEvent(0, 2, 1);

    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOnEvent, nullptr));
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, nullptr));
}

void test_NoteEventHandler_nullMidiEventPointer_DontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;

    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(nullptr, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEventNotMatchingChannelAndNumber_DontHandle()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;
    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(2, 2, 1);
    MIDINoteOffEvent noteOffEvent = MIDINoteOffEvent(2, 2, 1);

    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOnEvent, &dacValues));
    TEST_ASSERT_FALSE(noteEventHandler->handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;

    // test match first midi channel
    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(2, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[1]); // velocity

    // test match second midi channel
    noteOnEvent = MIDINoteOnEvent(1, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(2, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[3]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;

    // test match first midi channel
    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    MIDINoteOffEvent noteOffEvent = MIDINoteOffEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[1]); // velocity

    // test match second midi channel
    noteOnEvent = MIDINoteOnEvent(1, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = MIDINoteOffEvent(1, 2, 1);
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(1, dacValues.values[3]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_legatoInOrder_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;

    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = MIDINoteOnEvent(0, 3, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    MIDINoteOffEvent noteOffEvent = MIDINoteOffEvent(0, 3, 1);
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(2, dacValues.values[0]); // note
    noteOffEvent = MIDINoteOffEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
}


void test_NoteEventHandler_midiNoteOffEvent_legatoOutOfOrder_ValidDacEvent()
{
    NoteEventHandler *noteEventHandler = new NoteEventHandler(0, 1);
    DacValues dacValues;

    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = MIDINoteOnEvent(0, 3, 1);
    noteEventHandler->handleEvent(&noteOnEvent, &dacValues);
    MIDINoteOffEvent noteOffEvent = MIDINoteOffEvent(0, 2, 1);
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(3, dacValues.values[0]); // note
    noteOffEvent = MIDINoteOffEvent(0, 3, 1);
    noteEventHandler->handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(0, dacValues.values[2]); // note
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
    UNITY_END();
}

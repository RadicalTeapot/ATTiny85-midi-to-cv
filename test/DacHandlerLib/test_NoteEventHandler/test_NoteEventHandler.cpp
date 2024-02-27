#include <unity.h>
#include "eventHandlers/NoteEventHandler.h"
#include "MidiEvent.h"

void setUp() {}

void tearDown() {}

void test_NoteEventHandler_midiCCEvent_dontHandle()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};
    const MidiEventLib::Event ccEvent = {MidiEventLib::EventType::CC, 1, 1, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&ccEvent, &values[0]));
}

void test_NoteEventHandler_midiNoteOnEvent_Handle()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};
    const MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 1, 1};
    TEST_ASSERT_TRUE(noteEventHandler.handleEvent(&noteOnEvent, &values[0]));
}

void test_NoteEventHandler_midiNoteOffEventWithMatchingNoteOnEvent_Handle()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};
    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 1, 1};
    MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 1, 1};

    // test simple matching
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    TEST_ASSERT_TRUE(noteEventHandler.handleEvent(&noteOffEvent, &values[0]));

    // test matching of latest note on
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 1, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};
    TEST_ASSERT_TRUE(noteEventHandler.handleEvent(&noteOffEvent, &values[0]));
}

void test_NoteEventHandler_midiNoteOffEventWithNotMatchingNoteOnEvent_DontHandle()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};

    // test no note on event
    MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &values[0]));

    // test note off event not matching any note on
    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &values[0]));

    // test note off event discarding matched note on
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &values[0]));

    // test note off event matching only latest note on
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &values[0]));
}

void test_NoteEventHandler_midiNoteOnOrOff_nullDacEventPointer_DontHandle()
{
    NoteEventHandler noteEventHandler;
    const MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    const MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};

    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOnEvent, nullptr));
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, nullptr));
}

void test_NoteEventHandler_nullMidiEventPointer_DontHandle()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(nullptr, &values[0]));
}

void test_NoteEventHandler_midiNoteOnEventNotMatchingChannelAndNumber_DontHandle()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};
    const MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 2, 2, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOnEvent, &values[0]));
    const MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 2, 2, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &values[0]));
}

void test_NoteEventHandler_midiNoteOnEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};

    // test match first midi channel
    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    TEST_ASSERT_EQUAL(ValueRemapper::remapNote(2), values[0]); // note
    TEST_ASSERT_EQUAL(ValueRemapper::remapMidiValue(1), values[1]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};

    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_EQUAL(ValueRemapper::remapNote(0), values[0]); // note
    TEST_ASSERT_EQUAL(ValueRemapper::remapMidiValue(1), values[1]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_legatoInOrder_ValidDacEvent()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};

    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_EQUAL(ValueRemapper::remapNote(2), values[0]); // note
    noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_EQUAL(ValueRemapper::remapNote(0), values[0]); // note
}


void test_NoteEventHandler_midiNoteOffEvent_legatoOutOfOrder_ValidDacEvent()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};

    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_EQUAL(ValueRemapper::remapNote(3), values[0]); // note
    noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_EQUAL(ValueRemapper::remapNote(0), values[0]); // note
}

void test_NoteEventHandler_midiNoteOffEvent_successiveNotes_ValidDacEvent()
{
    NoteEventHandler noteEventHandler;
    uint16_t values[2] = {0, 0};

    // Same notes
    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_EQUAL(ValueRemapper::remapNote(0), values[0]); // note

    // Different notes
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 4, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 4, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_EQUAL(ValueRemapper::remapNote(0), values[0]); // note
}

uint16_t mockDoubleValueRemapper(uint8_t value) { return value * 2; }

void test_NoteEventHandler_midiNoteOnEvent_DoubleValueMapper_ValidDacEvent()
{
    NoteEventHandler noteEventHandler(0, &mockDoubleValueRemapper, &mockDoubleValueRemapper);
    uint16_t values[2] = {0, 0};

    // test match first midi channel
    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    TEST_ASSERT_EQUAL(mockDoubleValueRemapper(2), values[0]); // note
    TEST_ASSERT_EQUAL(mockDoubleValueRemapper(1), values[1]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_DoubleValueMapper_ValidDacEvent()
{
    NoteEventHandler noteEventHandler(0, &mockDoubleValueRemapper, &mockDoubleValueRemapper);
    uint16_t values[2] = {0, 0};

    // test match first midi channel
    MidiEventLib::Event noteOnEvent = {MidiEventLib::EventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &values[0]);
    MidiEventLib::Event noteOffEvent = {MidiEventLib::EventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &values[0]);
    TEST_ASSERT_EQUAL(mockDoubleValueRemapper(0), values[0]); // note
    TEST_ASSERT_EQUAL(mockDoubleValueRemapper(1), values[1]); // velocity
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

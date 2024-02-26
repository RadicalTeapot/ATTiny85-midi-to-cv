#include <unity.h>
#include "eventHandlers/NoteEventHandler.h"
#include "MidiEvent.h"

void setUp() {}

void tearDown() {}

class MockValueRemapper: public IValueRemapper
{
public:
    inline uint16_t remapCC(uint8_t value) const override { return value; }
    inline uint16_t remapNote(uint8_t value) const override { return value; }
    inline uint16_t remapVelocity(uint8_t value) const override { return value; }
};

void test_NoteEventHandler_midiCCEvent_dontHandle()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};

    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&ccEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEvent_Handle()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    TEST_ASSERT_TRUE(noteEventHandler.handleEvent(&noteOnEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOffEventWithMatchingNoteOnEvent_Handle()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};

    // test simple matching
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_TRUE(noteEventHandler.handleEvent(&noteOffEvent, &dacValues));

    // test matching of latest note on
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    TEST_ASSERT_TRUE(noteEventHandler.handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOffEventWithNotMatchingNoteOnEvent_DontHandle()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    // test no note on event
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &dacValues));

    // test note off event not matching any note on
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &dacValues));

    // test note off event discarding matched note on
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &dacValues));

    // test note off event matching only latest note on
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnOrOff_nullDacEventPointer_DontHandle()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    const MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};

    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOnEvent, nullptr));
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, nullptr));
}

void test_NoteEventHandler_nullMidiEventPointer_DontHandle()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(nullptr, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEventNotMatchingChannelAndNumber_DontHandle()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 2, 2, 1};
    const MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 2, 2, 1};

    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOnEvent, &dacValues));
    TEST_ASSERT_FALSE(noteEventHandler.handleEvent(&noteOffEvent, &dacValues));
}

void test_NoteEventHandler_midiNoteOnEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    // test match first midi channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(2), dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(0), dacValues.values[3]); // velocity

    // test match second midi channel
    noteOnEvent = {MidiEventType::NOTE_ON, 1, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(2), dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(2), dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[3]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    // test match first midi channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(0), dacValues.values[3]); // velocity

    // test match second midi channel
    noteOnEvent = {MidiEventType::NOTE_ON, 1, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 1, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[3]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_legatoInOrder_ValidDacEvent()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(2), dacValues.values[0]); // note
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[2]); // note
}


void test_NoteEventHandler_midiNoteOffEvent_legatoOutOfOrder_ValidDacEvent()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(3), dacValues.values[0]); // note
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[2]); // note
}

void test_NoteEventHandler_midiNoteOffEvent_successiveNotes_ValidDacEvent()
{
    MockValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    // Same notes
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[0]); // note

    // Different notes
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 3, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    noteOnEvent = {MidiEventType::NOTE_ON, 0, 4, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 0, 4, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[0]); // note
}

class MockDoubleValueRemapper: public IValueRemapper
{
public:
    inline uint16_t remapCC(uint8_t value) const override { return value * 2; }
    inline uint16_t remapNote(uint8_t value) const override { return value * 2; }
    inline uint16_t remapVelocity(uint8_t value) const override { return value * 2; }
};

void test_NoteEventHandler_midiNoteOnEvent_DoubleValueMapper_ValidDacEvent()
{
    MockDoubleValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    // test match first midi channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(2), dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(0), dacValues.values[3]); // velocity

    // test match second midi channel
    noteOnEvent = {MidiEventType::NOTE_ON, 1, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(2), dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(2), dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[3]); // velocity
}

void test_NoteEventHandler_midiNoteOffEvent_DoubleValueMapper_ValidDacEvent()
{
    MockDoubleValueRemapper valueRemapper;
    NoteEventHandler noteEventHandler = NoteEventHandler(0, 1, &valueRemapper);
    DacValues dacValues;

    // test match first midi channel
    MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(0), dacValues.values[3]); // velocity

    // test match second midi channel
    noteOnEvent = {MidiEventType::NOTE_ON, 1, 2, 1};
    noteEventHandler.handleEvent(&noteOnEvent, &dacValues);
    noteOffEvent = {MidiEventType::NOTE_OFF, 1, 2, 1};
    noteEventHandler.handleEvent(&noteOffEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[0]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[1]); // velocity
    TEST_ASSERT_EQUAL(valueRemapper.remapNote(0), dacValues.values[2]); // note
    TEST_ASSERT_EQUAL(valueRemapper.remapVelocity(1), dacValues.values[3]); // velocity
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

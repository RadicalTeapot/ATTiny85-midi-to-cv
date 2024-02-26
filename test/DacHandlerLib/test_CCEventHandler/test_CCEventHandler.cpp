#include <unity.h>
#include "eventHandlers/CCEventHandler.h"
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

void test_CCEventHandler_midiNoteOnEvent_dontHandle()
{
    MockValueRemapper valueRemapper;
    CCEventHandler ccEventHandler = CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4, &valueRemapper);
    DacValues dacValues;
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 1, 1, 1};

    TEST_ASSERT_EQUAL(0, ccEventHandler.handleEvent(&noteOnEvent, &dacValues));
}

void test_CCEventHandler_midiNoteOffEvent_dontHandle()
{
    MockValueRemapper valueRemapper;
    CCEventHandler ccEventHandler = CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4, &valueRemapper);
    DacValues dacValues;
    const MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 1, 1, 1};

    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&noteOffEvent, &dacValues));
}

void test_CCEventHandler_midiCCEvent_MatchingChannelAndNumber_Handle()
{
    MockValueRemapper valueRemapper;
    CCEventHandler ccEventHandler = CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4, &valueRemapper);
    DacValues dacValues;
    MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};
    TEST_ASSERT_TRUE(ccEventHandler.handleEvent(&ccEvent, &dacValues));
    TEST_ASSERT_EQUAL(valueRemapper.remapCC(1), dacValues.values[0]);
    ccEvent = {MidiEventType::CC, 2, 2, 1};
    TEST_ASSERT_TRUE(ccEventHandler.handleEvent(&ccEvent, &dacValues));
    TEST_ASSERT_EQUAL(valueRemapper.remapCC(1), dacValues.values[1]);
    ccEvent = {MidiEventType::CC, 3, 3, 1};
    TEST_ASSERT_TRUE(ccEventHandler.handleEvent(&ccEvent, &dacValues));
    TEST_ASSERT_EQUAL(valueRemapper.remapCC(1), dacValues.values[2]);
}

void test_CCEventHandler_NullMidiEventPointer_DontHandle()
{
    MockValueRemapper valueRemapper;
    CCEventHandler ccEventHandler = CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4, &valueRemapper);
    DacValues dacValues;
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(nullptr, &dacValues));
}

void test_CCEventHandler_midiCCEvent_nullDacEventPointer_DontHandle()
{
    MockValueRemapper valueRemapper;
    CCEventHandler ccEventHandler = CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4, &valueRemapper);
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&ccEvent, nullptr));
}

void test_CCEventHandler_midiCCEvent_NotMatchingChannelAndNumber_DontHandle()
{
    MockValueRemapper valueRemapper;
    CCEventHandler ccEventHandler = CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4, &valueRemapper);
    DacValues dacValues;
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 2, 1};

    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&ccEvent, &dacValues));
}

void test_CCEventHandler_midiCCEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    MockValueRemapper valueRemapper;
    CCEventHandler ccEventHandler = CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4, &valueRemapper);
    DacValues dacValues;
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};

    ccEventHandler.handleEvent(&ccEvent, &dacValues);
    TEST_ASSERT_EQUAL(valueRemapper.remapCC(1), dacValues.values[0]); // CC Value
}

class MockDoubleValueRemapper: public IValueRemapper
{
public:
    inline uint16_t remapCC(uint8_t value) const override { return value * 2; }
    inline uint16_t remapNote(uint8_t value) const override { return value * 2; }
    inline uint16_t remapVelocity(uint8_t value) const override { return value * 2; }
};

void test_CCEventHandler_midiCCEvent_DoubleValueMapper_ValidDacEvent()
{
    MockDoubleValueRemapper doubleValueRemapper;
    CCEventHandler ccEventHandler = CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4, &doubleValueRemapper);
    DacValues dacValues;
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};

    ccEventHandler.handleEvent(&ccEvent, &dacValues);
    TEST_ASSERT_EQUAL(doubleValueRemapper.remapCC(1), dacValues.values[0]);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_CCEventHandler_midiNoteOnEvent_dontHandle);
    RUN_TEST(test_CCEventHandler_midiNoteOffEvent_dontHandle);
    RUN_TEST(test_CCEventHandler_midiCCEvent_MatchingChannelAndNumber_Handle);
    RUN_TEST(test_CCEventHandler_midiCCEvent_nullDacEventPointer_DontHandle);
    RUN_TEST(test_CCEventHandler_NullMidiEventPointer_DontHandle);
    RUN_TEST(test_CCEventHandler_midiCCEvent_NotMatchingChannelAndNumber_DontHandle);
    RUN_TEST(test_CCEventHandler_midiCCEvent_MatchingChannelAndNumberAndValue_ValidDacEvent);
    RUN_TEST(test_CCEventHandler_midiCCEvent_DoubleValueMapper_ValidDacEvent);
    UNITY_END();
}

#include <unity.h>
#include "eventHandlers/CCEventHandler.h"
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

void test_CCEventHandler_midiNoteOnEvent_dontHandle()
{
    CCEventHandler ccEventHandler = CCEventHandler(mockValueMapper, 1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 1, 1, 1};

    TEST_ASSERT_EQUAL(0, ccEventHandler.handleEvent(&noteOnEvent, &dacValues));
}

void test_CCEventHandler_midiNoteOffEvent_dontHandle()
{
    CCEventHandler ccEventHandler = CCEventHandler(mockValueMapper, 1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    const MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 1, 1, 1};

    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&noteOffEvent, &dacValues));
}

void test_CCEventHandler_midiCCEvent_MatchingChannelAndNumber_Handle()
{
    CCEventHandler ccEventHandler = CCEventHandler(mockValueMapper, 1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};
    TEST_ASSERT_TRUE(ccEventHandler.handleEvent(&ccEvent, &dacValues));
    TEST_ASSERT_EQUAL(1, dacValues.values[0]);
    ccEvent = {MidiEventType::CC, 2, 2, 1};
    TEST_ASSERT_TRUE(ccEventHandler.handleEvent(&ccEvent, &dacValues));
    TEST_ASSERT_EQUAL(1, dacValues.values[1]);
    ccEvent = {MidiEventType::CC, 3, 3, 1};
    TEST_ASSERT_TRUE(ccEventHandler.handleEvent(&ccEvent, &dacValues));
    TEST_ASSERT_EQUAL(1, dacValues.values[2]);
}

void test_CCEventHandler_NullMidiEventPointer_DontHandle()
{
    CCEventHandler ccEventHandler = CCEventHandler(mockValueMapper, 1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(nullptr, &dacValues));
}

void test_CCEventHandler_midiCCEvent_nullDacEventPointer_DontHandle()
{
    CCEventHandler ccEventHandler = CCEventHandler(mockValueMapper, 1, 1, 2, 2, 3, 3, 4, 4);
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&ccEvent, nullptr));
}

void test_CCEventHandler_midiCCEvent_NotMatchingChannelAndNumber_DontHandle()
{
    CCEventHandler ccEventHandler = CCEventHandler(mockValueMapper, 1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 2, 1};

    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&ccEvent, &dacValues));
}

void test_CCEventHandler_midiCCEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    CCEventHandler ccEventHandler = CCEventHandler(mockValueMapper, 1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};

    ccEventHandler.handleEvent(&ccEvent, &dacValues);
    TEST_ASSERT_EQUAL(1, dacValues.values[0]); // CC Value
}

void doubleValueMapper(const uint8_t values[4], DacValues *dacValues)
{
    dacValues->values[0] = values[0] * 2;
    dacValues->values[1] = values[1] * 2;
    dacValues->values[2] = values[2] * 2;
    dacValues->values[3] = values[3] * 2;
}

void test_CCEventHandler_midiCCEvent_DoubleValueMapper_ValidDacEvent()
{
    CCEventHandler ccEventHandler = CCEventHandler(doubleValueMapper, 1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    const MidiEvent ccEvent = {MidiEventType::CC, 1, 1, 1};

    ccEventHandler.handleEvent(&ccEvent, &dacValues);
    TEST_ASSERT_EQUAL(2, dacValues.values[0]);
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

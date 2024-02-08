#include <unity.h>
#include "eventHandlers/CCEventHandler.h"

void setUp() {}

void tearDown() {}

void test_CCEventHandler_midiNoteOnEvent_dontHandle()
{
    CCEventHandler *ccEventHandler = new CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    MIDINoteOnEvent noteOnEvent = MIDINoteOnEvent(1, 1, 1);

    TEST_ASSERT_EQUAL(0, ccEventHandler->handleEvent(&noteOnEvent, &dacValues));
}

void test_CCEventHandler_midiNoteOffEvent_dontHandle()
{
    CCEventHandler *ccEventHandler = new CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    MIDINoteOffEvent noteOffEvent = MIDINoteOffEvent(1, 1, 1);

    TEST_ASSERT_FALSE(ccEventHandler->handleEvent(&noteOffEvent, &dacValues));
}

void test_CCEventHandler_midiCCEvent_MatchingChannelAndNumber_Handle()
{
    CCEventHandler *ccEventHandler = new CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    MIDICCEvent ccEvent = MIDICCEvent(1, 1, 1);
    TEST_ASSERT_TRUE(ccEventHandler->handleEvent(&ccEvent, &dacValues));
    ccEvent = MIDICCEvent(2, 2, 1);
    TEST_ASSERT_TRUE(ccEventHandler->handleEvent(&ccEvent, &dacValues));
    ccEvent = MIDICCEvent(3, 3, 1);
    TEST_ASSERT_TRUE(ccEventHandler->handleEvent(&ccEvent, &dacValues));
}

void test_CCEventHandler_NullMidiEventPointer_DontHandle()
{
    CCEventHandler *ccEventHandler = new CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    TEST_ASSERT_FALSE(ccEventHandler->handleEvent(nullptr, &dacValues));
}

void test_CCEventHandler_midiCCEvent_nullDacEventPointer_DontHandle()
{
    CCEventHandler *ccEventHandler = new CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4);
    MIDICCEvent ccEvent = MIDICCEvent(1, 1, 1);
    TEST_ASSERT_FALSE(ccEventHandler->handleEvent(&ccEvent, nullptr));
}

void test_CCEventHandler_midiCCEvent_NotMatchingChannelAndNumber_DontHandle()
{
    CCEventHandler *ccEventHandler = new CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    MIDICCEvent ccEvent = MIDICCEvent(1, 2, 1);

    TEST_ASSERT_FALSE(ccEventHandler->handleEvent(&ccEvent, &dacValues));
}

void test_CCEventHandler_midiCCEvent_MatchingChannelAndNumberAndValue_ValidDacEvent()
{
    CCEventHandler *ccEventHandler = new CCEventHandler(1, 1, 2, 2, 3, 3, 4, 4);
    DacValues dacValues;
    MIDICCEvent ccEvent = MIDICCEvent(1, 1, 1);

    ccEventHandler->handleEvent(&ccEvent, &dacValues);
    TEST_ASSERT_EQUAL(1, dacValues.values[0]); // CC Value
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
    UNITY_END();
}

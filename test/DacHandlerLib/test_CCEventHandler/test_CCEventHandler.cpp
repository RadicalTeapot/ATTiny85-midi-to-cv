#include <unity.h>
#include "eventHandlers/CCEventHandler.h"
#include "MidiEvent.h"

void setUp() {}

void tearDown() {}

void test_CCEventHandler_midiNoteOnEvent_dontHandle()
{
    CCEventHandler ccEventHandler;
    const MidiEvent noteOnEvent = {MidiEventType::NOTE_ON, 0, 1, 1};
    uint16_t value = 0;
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&noteOnEvent, &value));
}

void test_CCEventHandler_midiNoteOffEvent_dontHandle()
{
    CCEventHandler ccEventHandler;
    const MidiEvent noteOffEvent = {MidiEventType::NOTE_OFF, 0, 1, 1};
    uint16_t value = 0;
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&noteOffEvent, &value));
}

void test_CCEventHandler_midiCCEvent_MatchingChannelAndNumber_Handle()
{
    CCEventHandler ccEventHandler;

    uint16_t value = 0;
    MidiEvent ccEvent = {MidiEventType::CC, 0, 0, 1};
    TEST_ASSERT_TRUE(ccEventHandler.handleEvent(&ccEvent, &value));
    TEST_ASSERT_EQUAL(ValueRemapper::remapMidiValue(1), value);
}

void test_CCEventHandler_NullMidiEventPointer_DontHandle()
{
    CCEventHandler ccEventHandler;
    uint16_t value = 0;
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(nullptr, &value));
}

void test_CCEventHandler_midiCCEvent_nullDacEventPointer_DontHandle()
{
    CCEventHandler ccEventHandler;
    const MidiEvent ccEvent = {MidiEventType::CC, 0, 0, 1};
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&ccEvent, nullptr));
}

void test_CCEventHandler_midiCCEvent_NotMatchingChannelAndNumber_DontHandle()
{
    CCEventHandler ccEventHandler(0, 0);
    uint16_t value = 0;
    MidiEvent ccEvent = {MidiEventType::CC, 0, 1, 1};
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&ccEvent, &value));
    ccEvent = {MidiEventType::CC, 1, 0, 1};
    TEST_ASSERT_FALSE(ccEventHandler.handleEvent(&ccEvent, &value));
}

uint16_t mockDoubleRemapCC(uint8_t value) { return value * 2; }

void test_CCEventHandler_midiCCEvent_DoubleValueMapper_ValidDacEvent()
{
    CCEventHandler ccEventHandler = CCEventHandler(0, 0, &mockDoubleRemapCC);
    uint16_t value = 0;
    const MidiEvent ccEvent = {MidiEventType::CC, 0, 0, 1};

    ccEventHandler.handleEvent(&ccEvent, &value);
    TEST_ASSERT_EQUAL(mockDoubleRemapCC(1), value);
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
    RUN_TEST(test_CCEventHandler_midiCCEvent_DoubleValueMapper_ValidDacEvent);
    UNITY_END();
}

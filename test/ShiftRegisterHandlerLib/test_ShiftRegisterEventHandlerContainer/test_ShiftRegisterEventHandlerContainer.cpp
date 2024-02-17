#include <unity.h>
#include "ShiftRegisterEventHandlerContainer.h"
#include "DefaultPresets.h"

void setUp() {}

void tearDown() {}

void test_ShiftRegisterEventHandlerContainer_defaultState()
{
    ShiftRegisterEventHandlerContainer container;
    uint8_t notes[8] = DEFAULT_NOTES;
    MidiEvent midiEvent = {MidiEventType::NOTE_ON, DEFAULT_CHANNEL, 0, 0};
    uint8_t expectedResult = 0, result = 0;
    for (int i = 0; i < 8; i++)
    {
        midiEvent.firstByte = notes[i];
        container.processEvent(&midiEvent, &result);
        expectedResult |= 1 << i;
        TEST_ASSERT_EQUAL(expectedResult, result);
    }
    midiEvent.type = MidiEventType::NOTE_OFF;
    for (int i = 0; i < 8; i++)
    {
        midiEvent.firstByte = notes[i];
        container.processEvent(&midiEvent, &result);
        expectedResult &= ~(1 << i);
        TEST_ASSERT_EQUAL(expectedResult, result);
    }
}

void test_ShiftRegisterEventHandlerContainer_InvalidMidiEvents_ReturnsState()
{
    ShiftRegisterEventHandlerContainer container;
    uint8_t notes[8] = DEFAULT_NOTES;

    // test invalid channel
    uint8_t result = 0;
    MidiEvent event = {MidiEventType::NOTE_ON, 1, notes[0], 1};
    container.processEvent(&event, &result);
    TEST_ASSERT_EQUAL(0, &result);

    // test invalid note
    result = 0;
    event = {MidiEventType::NOTE_ON, DEFAULT_CHANNEL, 0, 1};
    container.processEvent(&event, &result);
    TEST_ASSERT_EQUAL(0, &result);

    // test invalid event type
    result = 0;
    event = {MidiEventType::CC, 0, 1, 1};
    container.processEvent(&event, &result);
    TEST_ASSERT_EQUAL(0, &result);
}

void test_ShiftRegisterEventHandlerContainer_setFirstHandlersPairFromDacConfig()
{
    ShiftRegisterEventHandlerContainer container;
    container.setFirstHandlersPairFromDacConfig(&preset0.dacConfigA, true);

    MidiEvent midiEvent = {MidiEventType::NOTE_ON, (uint8_t)(preset0.dacConfigA.NoteChannels >> 4), 0, 0};
    uint8_t result = 0;
    container.processEvent(&midiEvent, &result);
    TEST_ASSERT_EQUAL(0B00000001, result);
    midiEvent.type = MidiEventType::NOTE_OFF;
    container.processEvent(&midiEvent, &result);
    TEST_ASSERT_EQUAL(0B00000000, result);

    midiEvent.channel = preset0.dacConfigA.NoteChannels & 0x0F;
    result = 0;
    container.processEvent(&midiEvent, &result);
    TEST_ASSERT_EQUAL(0B00000010, result);
    midiEvent.type = MidiEventType::NOTE_OFF;
    container.processEvent(&midiEvent, &result);
    TEST_ASSERT_EQUAL(0B00000000, result);
}

void test_ShiftRegisterEventHandlerContainer_setSecondHandlersPairFromDacConfig()
{
    ShiftRegisterEventHandlerContainer container;
    container.setSecondHandlersPairFromDacConfig(&preset0.dacConfigB, true);
    uint8_t result = 0;

    MidiEvent midiEvent = {MidiEventType::NOTE_ON, (uint8_t)(preset0.dacConfigB.NoteChannels >> 4), 0, 0};
    container.processEvent(&midiEvent, &result);
    TEST_ASSERT_EQUAL(0B00000100, result);
    midiEvent.type = MidiEventType::NOTE_OFF;
    container.processEvent(&midiEvent, &result);
    TEST_ASSERT_EQUAL(0B00000000, result);

    result = 0;
    midiEvent.channel = preset0.dacConfigB.NoteChannels & 0x0F;
    container.processEvent(&midiEvent, &result);
    TEST_ASSERT_EQUAL(0B00001000, result);
    midiEvent.type = MidiEventType::NOTE_OFF;
    container.processEvent(&midiEvent, &result);
}

void test_ShiftRegisterEventHandlerContainer_setHandlersFromDacConfig_InvalidMidiEvent_ReturnsState()
{
    ShiftRegisterEventHandlerContainer container;
    container.setFirstHandlersPairFromDacConfig(&preset0.dacConfigA, true);
    container.setSecondHandlersPairFromDacConfig(&preset0.dacConfigB, true);

    // test invalid channel
    uint8_t result = 0;
    MidiEvent event = {MidiEventType::NOTE_ON, 4, 0, 1};
    container.processEvent(&event, &result);
    TEST_ASSERT_EQUAL(0, &result);

    // test invalid event type
    result = 0;
    event = {MidiEventType::CC, 0, 1, 1};
    container.processEvent(&event, &result);
    TEST_ASSERT_EQUAL(0, &result);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_defaultState);
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_InvalidMidiEvents_ReturnsState);
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_setFirstHandlersPairFromDacConfig);
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_setSecondHandlersPairFromDacConfig);
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_setHandlersFromDacConfig_InvalidMidiEvent_ReturnsState);
    return UNITY_END();
}

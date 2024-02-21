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
        result = container.processEvent(&midiEvent, result);
        expectedResult |= 1 << i;
        TEST_ASSERT_EQUAL(expectedResult, result);
    }
    midiEvent.type = MidiEventType::NOTE_OFF;
    for (int i = 0; i < 8; i++)
    {
        midiEvent.firstByte = notes[i];
        result = container.processEvent(&midiEvent, result);
        expectedResult &= ~(1 << i);
        TEST_ASSERT_EQUAL(expectedResult, result);
    }
}

void test_ShiftRegisterEventHandlerContainer_InvalidMidiEvents_ReturnsState()
{
    ShiftRegisterEventHandlerContainer container;
    uint8_t notes[8] = DEFAULT_NOTES;

    // test invalid channel
    MidiEvent event = {MidiEventType::NOTE_ON, 1, notes[0], 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));

    // test invalid note
    event = {MidiEventType::NOTE_ON, DEFAULT_CHANNEL, 0, 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));

    // test invalid event type
    event = {MidiEventType::CC, 0, 1, 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));
}

void test_ShiftRegisterEventHandlerContainer_setFirstHandlersPairFromDacConfig()
{
    ShiftRegisterEventHandlerContainer container;
    container.setFirstHandlersPairFromDacConfig(&preset0.dacConfigA, true);

    MidiEvent midiEvent = {MidiEventType::NOTE_ON, preset0.dacConfigA.NoteChannels >> 4U, 0, 0};
    TEST_ASSERT_EQUAL(0B00000001, container.processEvent(&midiEvent, 0));
    midiEvent.type = MidiEventType::NOTE_OFF;
    TEST_ASSERT_EQUAL(0B00000000, container.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventType::NOTE_ON, preset0.dacConfigA.NoteChannels & 0x0F, 0, 0};
    TEST_ASSERT_EQUAL(0B00000010, container.processEvent(&midiEvent, 0));
    midiEvent.type = MidiEventType::NOTE_OFF;
    TEST_ASSERT_EQUAL(0B00000000, container.processEvent(&midiEvent, 0));
}

void test_ShiftRegisterEventHandlerContainer_setSecondHandlersPairFromDacConfig()
{
    ShiftRegisterEventHandlerContainer container;
    container.setSecondHandlersPairFromDacConfig(&preset0.dacConfigB, true);

    MidiEvent midiEvent = {MidiEventType::NOTE_ON, (uint8_t)(preset0.dacConfigB.NoteChannels >> 4), 0, 0};
    TEST_ASSERT_EQUAL(0B00000100, container.processEvent(&midiEvent, 0));
    midiEvent.type = MidiEventType::NOTE_OFF;
    TEST_ASSERT_EQUAL(0B00000000, container.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventType::NOTE_ON, preset0.dacConfigB.NoteChannels & 0x0F, 0, 0};
    TEST_ASSERT_EQUAL(0B00001000, container.processEvent(&midiEvent, 0));
    midiEvent.type = MidiEventType::NOTE_OFF;
    TEST_ASSERT_EQUAL(0B00000000, container.processEvent(&midiEvent, 0));
}

void test_ShiftRegisterEventHandlerContainer_setHandlersFromDacConfig_InvalidMidiEvent_ReturnsState()
{
    ShiftRegisterEventHandlerContainer container;
    container.setFirstHandlersPairFromDacConfig(&preset0.dacConfigA, true);
    container.setSecondHandlersPairFromDacConfig(&preset0.dacConfigB, true);

    // test invalid channel
    MidiEvent event = {MidiEventType::NOTE_ON, 4, 0, 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));

    // test invalid event type
    event = {MidiEventType::CC, 0, 1, 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));
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

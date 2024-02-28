#include <unity.h>
#include "ShiftRegisterEventHandlerContainer.h"
#include "DefaultPresets.h"

void setUp() {}

void tearDown() {}

void test_ShiftRegisterEventHandlerContainer_DefaultState()
{
    ShiftRegisterEventHandlerContainer container;
    MidiEventLib::Event midiEvent = {MidiEventLib::EventType::NOTE_ON, ShiftRegisterEventHandlerConstants::NOTE_CHANNEL, 0, 0};
    uint8_t expectedResult = 0, result = 0;
    for (int i = 0; i < 8; i++)
    {
        midiEvent.firstByte = ShiftRegisterEventHandlerContainerConstants::DEFAULT_NOTES[i];
        result = container.processEvent(&midiEvent, result);
        expectedResult |= 1 << i;
        TEST_ASSERT_EQUAL(expectedResult, result);
    }
    midiEvent.type = MidiEventLib::EventType::NOTE_OFF;
    for (int i = 0; i < 8; i++)
    {
        midiEvent.firstByte = ShiftRegisterEventHandlerContainerConstants::DEFAULT_NOTES[i];
        result = container.processEvent(&midiEvent, result);
        expectedResult &= ~(1 << i);
        TEST_ASSERT_EQUAL(expectedResult, result);
    }
}

void test_ShiftRegisterEventHandlerContainer_InvalidMidiEvents_ReturnsState()
{
    ShiftRegisterEventHandlerContainer container;

    // test invalid channel
    MidiEventLib::Event event = {MidiEventLib::EventType::NOTE_ON, 1, ShiftRegisterEventHandlerContainerConstants::DEFAULT_NOTES[0], 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));

    // test invalid note
    event = {MidiEventLib::EventType::NOTE_ON, ShiftRegisterEventHandlerConstants::NOTE_CHANNEL, 0, 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));

    // test invalid event type
    event = {MidiEventLib::EventType::CC, 0, 1, 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));
}

void test_ShiftRegisterEventHandlerContainer_ChannelMode_SetFirstHandlersPairFromDacConfig_ValidData()
{
    ShiftRegisterEventHandlerContainer container;
    container.setFirstHandlersPairFromDacConfig(&preset0.dacConfigA, true);

    MidiEventLib::Event midiEvent = {MidiEventLib::EventType::NOTE_ON, (uint8_t)(preset0.dacConfigA.NoteChannels >> 4U), 0, 0};
    TEST_ASSERT_EQUAL(0B00000001, container.processEvent(&midiEvent, 0));
    midiEvent.type = MidiEventLib::EventType::NOTE_OFF;
    TEST_ASSERT_EQUAL(0B00000000, container.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventLib::EventType::NOTE_ON, (uint8_t)(preset0.dacConfigA.NoteChannels & 0x0F), 0, 0};
    TEST_ASSERT_EQUAL(0B00000010, container.processEvent(&midiEvent, 0));
    midiEvent.type = MidiEventLib::EventType::NOTE_OFF;
    TEST_ASSERT_EQUAL(0B00000000, container.processEvent(&midiEvent, 0));
}


void test_ShiftRegisterEventHandlerContainer_CCMode_SetFirstHandlersPairFromDacConfig_ValidData()
    {
        ShiftRegisterEventHandlerContainer container;
        container.setFirstHandlersPairFromDacConfig(&preset0.dacConfigA, false);

        MidiEventLib::Event midiEvent = {MidiEventLib::EventType::CC, (uint8_t)(preset0.dacConfigA.CCChannels1 >> 4U), preset0.dacConfigA.CCNumber1, 1};
        TEST_ASSERT_EQUAL(0B00000001, container.processEvent(&midiEvent, 0));

    }

void test_ShiftRegisterEventHandlerContainer_ChannelMode_SetSecondHandlersPairFromDacConfig_ValidData()
{
    ShiftRegisterEventHandlerContainer container;
    container.setSecondHandlersPairFromDacConfig(&preset0.dacConfigB, true);

    MidiEventLib::Event midiEvent = {MidiEventLib::EventType::NOTE_ON, (uint8_t)(preset0.dacConfigB.NoteChannels >> 4), 0, 0};
    TEST_ASSERT_EQUAL(0B00000100, container.processEvent(&midiEvent, 0));
    midiEvent.type = MidiEventLib::EventType::NOTE_OFF;
    TEST_ASSERT_EQUAL(0B00000000, container.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventLib::EventType::NOTE_ON, (uint8_t)(preset0.dacConfigB.NoteChannels & 0x0F), 0, 0};
    TEST_ASSERT_EQUAL(0B00001000, container.processEvent(&midiEvent, 0));
    midiEvent.type = MidiEventLib::EventType::NOTE_OFF;
    TEST_ASSERT_EQUAL(0B00000000, container.processEvent(&midiEvent, 0));
}

void test_ShiftRegisterEventHandlerContainer_SetHandlersFromDacConfig_InvalidMidiEvent_ReturnsState()

{
    ShiftRegisterEventHandlerContainer container;
    container.setFirstHandlersPairFromDacConfig(&preset0.dacConfigA, true);
    container.setSecondHandlersPairFromDacConfig(&preset0.dacConfigB, true);

    // test invalid channel
    MidiEventLib::Event event = {MidiEventLib::EventType::NOTE_ON, 4, 0, 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));

    // test invalid event type
    event = {MidiEventLib::EventType::CC, 0, 1, 1};
    TEST_ASSERT_EQUAL(0, container.processEvent(&event, 0));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_DefaultState);
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_InvalidMidiEvents_ReturnsState);
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_ChannelMode_SetFirstHandlersPairFromDacConfig_ValidData);
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_ChannelMode_SetSecondHandlersPairFromDacConfig_ValidData);
    RUN_TEST(test_ShiftRegisterEventHandlerContainer_SetHandlersFromDacConfig_InvalidMidiEvent_ReturnsState);
    return UNITY_END();
}

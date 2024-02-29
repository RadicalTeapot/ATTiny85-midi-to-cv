#include <unity.h>
#include "ShiftRegisterEventHandler.h"
#include "MidiEvent.h"

void setUp() {}

void tearDown() {}

void test_ShiftRegisterEventHandler_default()
{
    const uint8_t midiChannel = 0;
    ShiftRegisterEventHandler<midiChannel> handler;
    MidiEventLib::Event midiEvent = {MidiEventLib::EventType::NOTE_ON, midiChannel, 1, 1};
    TEST_ASSERT_TRUE(handler.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventLib::EventType::NOTE_OFF, midiChannel, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&midiEvent, 0));
}

void test_ShiftRegisterEventHandler_otherMidiChannel()
{
    const uint8_t midiChannel = 1;
    ShiftRegisterEventHandler<midiChannel> handler;
    MidiEventLib::Event midiEvent = {MidiEventLib::EventType::NOTE_ON, midiChannel, 1, 1};
    TEST_ASSERT_TRUE(handler.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventLib::EventType::NOTE_OFF, midiChannel, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&midiEvent, 0));
}

void test_ShiftRegisterEventHandler_channelMode()
{
    const uint8_t midiChannel = 0;
    const uint8_t otherMidiChannel = 1;
    ShiftRegisterEventHandler<midiChannel> handler;
    handler.configure(midiChannel, true);

    MidiEventLib::Event midiEvent = {MidiEventLib::EventType::NOTE_ON, midiChannel, 1, 1};
    TEST_ASSERT_TRUE(handler.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventLib::EventType::NOTE_ON, otherMidiChannel, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&midiEvent, 0));

    handler.configure(otherMidiChannel, true);

    midiEvent = {MidiEventLib::EventType::NOTE_ON, midiChannel, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventLib::EventType::NOTE_ON, otherMidiChannel, 1, 1};
    TEST_ASSERT_TRUE(handler.processEvent(&midiEvent, 0));
}

void test_ShiftRegisterEventHandler_noteMode()
{
    const uint8_t midiChannel = 0;
    const uint8_t note = 1;
    const uint8_t otherNote = 2;
    ShiftRegisterEventHandler<midiChannel> handler;
    handler.configure(note, false);

    MidiEventLib::Event midiEvent = {MidiEventLib::EventType::NOTE_ON, midiChannel, note, 1};
    TEST_ASSERT_TRUE(handler.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventLib::EventType::NOTE_ON, midiChannel, otherNote, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&midiEvent, 0));

    handler.configure(otherNote, false);

    midiEvent = {MidiEventLib::EventType::NOTE_ON, midiChannel, note, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&midiEvent, 0));

    midiEvent = {MidiEventLib::EventType::NOTE_ON, midiChannel, otherNote, 1};
    TEST_ASSERT_TRUE(handler.processEvent(&midiEvent, 0));
}

void test_ShiftRegisterEventHandler_unhandled_returnsPreviousState()
{
    const uint8_t midiChannel = 0;
    ShiftRegisterEventHandler<midiChannel> handler;
    MidiEventLib::Event midiEvent = {MidiEventLib::EventType::CC, midiChannel, 1, 1};
    TEST_ASSERT_FALSE(handler.processEvent(&midiEvent, 0));
    TEST_ASSERT_TRUE(handler.processEvent(&midiEvent, 1));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_ShiftRegisterEventHandler_default);
    RUN_TEST(test_ShiftRegisterEventHandler_otherMidiChannel);
    RUN_TEST(test_ShiftRegisterEventHandler_channelMode);
    RUN_TEST(test_ShiftRegisterEventHandler_noteMode);
    RUN_TEST(test_ShiftRegisterEventHandler_unhandled_returnsPreviousState);
    UNITY_END();
}

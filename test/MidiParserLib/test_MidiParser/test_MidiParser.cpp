#include <unity.h>
#include "MidiParser.h"

void setUp() {}

void tearDown() {}

void test_parseMidiCommandOnly_returnsFalse() {
    MidiEventLib::Parser midiParser;
    uint8_t midiByte = 0x80;
    MidiEventLib::Event midiEvent;
    TEST_ASSERT_FALSE(midiParser.parse(midiByte, &midiEvent));
}

void test_parseMalformedMidiCommand_returnsFalse() {
    MidiEventLib::Parser midiParser;
    MidiEventLib::Event midiEvent;
    TEST_ASSERT_FALSE(midiParser.parse(0x00, &midiEvent)); // Data rather than command
    TEST_ASSERT_FALSE(midiParser.parse(0x00, &midiEvent));
    TEST_ASSERT_FALSE(midiParser.parse(0x00, &midiEvent));
}

void test_parseNoteOnCommand_validData() {
    MidiEventLib::Parser midiParser;
    MidiEventLib::Event midiEvent;
    TEST_ASSERT_FALSE(midiParser.parse(MidiEventLib::EventType::NOTE_ON | 0x01, &midiEvent));
    TEST_ASSERT_FALSE(midiParser.parse(0x02, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x03, &midiEvent));

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::NOTE_ON, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x02, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x03, midiEvent.secondByte);
}

void test_parseNoteOffCommand_validData() {
    MidiEventLib::Parser midiParser;
    MidiEventLib::Event midiEvent;
    TEST_ASSERT_FALSE(midiParser.parse(MidiEventLib::EventType::NOTE_OFF | 0x01, &midiEvent));
    TEST_ASSERT_FALSE(midiParser.parse(0x02, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x03, &midiEvent));

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::NOTE_OFF, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x02, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x03, midiEvent.secondByte);
}

void test_parseNoteOnOffCommand_validData() {
    MidiEventLib::Parser midiParser;
    MidiEventLib::Event midiEvent;
    TEST_ASSERT_FALSE(midiParser.parse(MidiEventLib::EventType::NOTE_ON | 0x01, &midiEvent));
    TEST_ASSERT_FALSE(midiParser.parse(0x02, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x00, &midiEvent)); // Velocity = 0 -> Note Off

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::NOTE_OFF, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x02, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x00, midiEvent.secondByte);
}

void test_parseContinuousNoteCommands_validData() {
    MidiEventLib::Parser midiParser;
    MidiEventLib::Event midiEvent;
    TEST_ASSERT_FALSE(midiParser.parse(MidiEventLib::EventType::NOTE_ON | 0x01, &midiEvent));
    TEST_ASSERT_FALSE(midiParser.parse(0x02, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x03, &midiEvent));

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::NOTE_ON, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x02, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x03, midiEvent.secondByte);

    TEST_ASSERT_FALSE(midiParser.parse(0x10, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x11, &midiEvent));

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::NOTE_ON, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x10, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x11, midiEvent.secondByte);

    TEST_ASSERT_FALSE(midiParser.parse(0x20, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x00, &midiEvent)); // Velocity = 0 -> Note Off

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::NOTE_OFF, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x20, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x00, midiEvent.secondByte);
}

void test_parseCCCommand_validData() {
    MidiEventLib::Parser midiParser;
    MidiEventLib::Event midiEvent;
    TEST_ASSERT_FALSE(midiParser.parse(MidiEventLib::EventType::CC | 0x01, &midiEvent));
    TEST_ASSERT_FALSE(midiParser.parse(0x02, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x03, &midiEvent));

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::CC, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x02, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x03, midiEvent.secondByte);
}

void test_parseContinuousCCCommands_validData() {
    MidiEventLib::Parser midiParser;
    MidiEventLib::Event midiEvent;
    TEST_ASSERT_FALSE(midiParser.parse(MidiEventLib::EventType::CC | 0x01, &midiEvent));
    TEST_ASSERT_FALSE(midiParser.parse(0x02, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x03, &midiEvent));

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::CC, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x02, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x03, midiEvent.secondByte);

    TEST_ASSERT_FALSE(midiParser.parse(0x10, &midiEvent));
    TEST_ASSERT_TRUE(midiParser.parse(0x11, &midiEvent));

    TEST_ASSERT_EQUAL(MidiEventLib::EventType::CC, midiEvent.type);
    TEST_ASSERT_EQUAL(0x01, midiEvent.channel);
    TEST_ASSERT_EQUAL(0x10, midiEvent.firstByte);
    TEST_ASSERT_EQUAL(0x11, midiEvent.secondByte);
}

void test_parseMidiData() {
    MidiEventLib::Parser midiParser;
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_parseMidiCommandOnly_returnsFalse);
    RUN_TEST(test_parseMalformedMidiCommand_returnsFalse);
    RUN_TEST(test_parseNoteOnCommand_validData);
    RUN_TEST(test_parseNoteOffCommand_validData);
    RUN_TEST(test_parseNoteOnOffCommand_validData);
    RUN_TEST(test_parseContinuousNoteCommands_validData);
    RUN_TEST(test_parseCCCommand_validData);
    RUN_TEST(test_parseContinuousCCCommands_validData);
    return UNITY_END();
}

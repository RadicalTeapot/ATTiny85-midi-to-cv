#include <unity.h>
#include "Utils/ValueRemapper.h"

void setUp() {}

void tearDown() {}

void test_ValueRemapper_midiValue_outOfRange_clamped()
{
    uint8_t input = 0;
    uint16_t output = ValueRemapper::remapMidiValue<1, 2, 1, 2>(input);
    TEST_ASSERT_EQUAL(1, output);

    input = 3;
    output = ValueRemapper::remapMidiValue<1, 2, 1, 2>(input);
    TEST_ASSERT_EQUAL(2, output);
}

void test_ValueRemapper_midiValue_inRange_mapped()
{
    uint8_t input = 1;
    uint16_t output = ValueRemapper::remapMidiValue<0, 10, 0, 10>(input);
    TEST_ASSERT_EQUAL(1, output);

    input = 1;
    output = ValueRemapper::remapMidiValue<0, 1, 0, 10>(input);
    TEST_ASSERT_EQUAL(10, output);

    input = 1;
    output = ValueRemapper::remapMidiValue<0, 2, 0, 10>(input);
    TEST_ASSERT_EQUAL(5, output);
}

void test_ValueRemapper_midiValue_defaultValues()
{
    const uint8_t midiMaxValue = 127;
    const uint16_t dacMaxValue = 4095;
    uint8_t input = 0;
    uint16_t output = ValueRemapper::remapMidiValue(input);
    TEST_ASSERT_EQUAL(0, output);

    output = ValueRemapper::remapMidiValue(midiMaxValue);
    TEST_ASSERT_EQUAL(dacMaxValue, output);
}

void test_ValueRemapper_noteValue_outOfRange_clamped()
{
    uint8_t input = DacPitchCalibration::RANGE + 1;
    uint16_t output = ValueRemapper::remapNote<0, DacPitchCalibration::RANGE>(input);
    TEST_ASSERT_EQUAL(DacPitchCalibration::LookUpTable[DacPitchCalibration::RANGE - 1], output);

    input = 0;
    output = ValueRemapper::remapNote<10, DacPitchCalibration::RANGE>(input);
    TEST_ASSERT_EQUAL(DacPitchCalibration::LookUpTable[0], output);
}

void test_ValueRemapper_noteValue_inRange_mapped()
{
    uint8_t input = 1;
    uint16_t output = ValueRemapper::remapNote<0, 10>(input);
    TEST_ASSERT_EQUAL(DacPitchCalibration::LookUpTable[1], output);

    input = 9;
    output = ValueRemapper::remapNote<0, 10>(input);
    TEST_ASSERT_EQUAL(DacPitchCalibration::LookUpTable[9], output);

    input = 1;
    output = ValueRemapper::remapNote<1, 10>(input);
    TEST_ASSERT_EQUAL(DacPitchCalibration::LookUpTable[0], output);
}

void test_ValueRemapper_noteValue_defaultValues()
{
    uint8_t input = DacPitchCalibration::LOW_MIDI_NOTE;
    uint16_t output = ValueRemapper::remapNote(input);
    TEST_ASSERT_EQUAL(DacPitchCalibration::LookUpTable[0], output);

    input = DacPitchCalibration::LOW_MIDI_NOTE + DacPitchCalibration::RANGE - 1;
    output = ValueRemapper::remapNote(input);
    TEST_ASSERT_EQUAL(DacPitchCalibration::LookUpTable[DacPitchCalibration::RANGE - 1], output);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_ValueRemapper_midiValue_outOfRange_clamped);
    RUN_TEST(test_ValueRemapper_midiValue_inRange_mapped);
    RUN_TEST(test_ValueRemapper_midiValue_defaultValues);
    RUN_TEST(test_ValueRemapper_noteValue_outOfRange_clamped);
    RUN_TEST(test_ValueRemapper_noteValue_inRange_mapped);
    RUN_TEST(test_ValueRemapper_noteValue_defaultValues);
    UNITY_END();
}

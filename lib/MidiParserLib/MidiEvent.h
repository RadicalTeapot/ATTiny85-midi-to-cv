#ifndef MidiEvent_h
#define MidiEvent_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#define MIDI_COMMAND_MASK (0x80)
#define MIDI_CHANNEL_COMMAND_MASK (0x0F)
#define MIDI_CHANNEL_SYSTEM_COMMAND_MASK (0xF0)
#define MIDI_ON_MASK (0x90)
#define MIDI_OFF_MASK (0x80)
#define MIDI_CONTROL_CHANGE_MASK (0xB0)
#define MIDI_PROGRAM_CHANGE_MASK (0xC0)

enum MidiEventType
{
    NOTE_ON = MIDI_ON_MASK,
    NOTE_OFF = MIDI_OFF_MASK,
    PROGRAM_CHANGE = MIDI_PROGRAM_CHANGE_MASK,
    CC = MIDI_CONTROL_CHANGE_MASK,
    SYS_EX = 0xF0,
    INVALID = 0xFF,
};

struct MidiEvent
{
    MidiEventType type = INVALID;
    uint8_t channel = 0;
    uint8_t firstByte = 0;
    uint8_t secondByte = 0;

    static void setCCEventData(MidiEvent *event, uint8_t channel, uint8_t number, uint8_t value) {
        event->type = CC;
        event->channel = channel;
        event->firstByte = number;
        event->secondByte = value;
    }

    static void SetNoteOnEventData(MidiEvent *event, uint8_t channel, uint8_t note, uint8_t velocity) {
        event->type = NOTE_ON;
        event->channel = channel;
        event->firstByte = note;
        event->secondByte = velocity;
    }

    static void setNoteOffEventData(MidiEvent *event, uint8_t channel, uint8_t note, uint8_t velocity) {
        event->type = NOTE_OFF;
        event->channel = channel;
        event->firstByte = note;
        event->secondByte = velocity;
    }
};

struct MidiNoteOnEvent {
    MidiEventType type = MidiEventType::NOTE_ON;
    uint8_t channel = 0;
    uint8_t note = 0;
    uint8_t velocity = 0;
};

struct MidiNoteOffEvent {
    MidiEventType type = MidiEventType::NOTE_OFF;
    uint8_t channel = 0;
    uint8_t note = 0;
    uint8_t velocity = 0;
};

struct MidiCCEvent {
    MidiEventType type = MidiEventType::CC;
    uint8_t channel = 0;
    uint8_t number = 0;
    uint8_t value = 0;
};

#endif // MidiEvent_h

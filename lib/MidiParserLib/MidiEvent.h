#ifndef MIDIEvent_h
#define MIDIEvent_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

#define MIDI_COMMAND_MASK (0x80)
#define MIDI_CHANNEL_SYSTEM_COMMAND_MASK (0xF0)
#define MIDI_ON_MASK (0x90)
#define MIDI_OFF_MASK (0x80)
#define MIDI_CONTROL_CHANGE_MASK (0xB0)
#define MIDI_PROGRAM_CHANGE_MASK (0xC0)

enum MIDIEventType
{
    NOTE_ON = MIDI_ON_MASK,
    NOTE_OFF = MIDI_OFF_MASK,
    PROGRAM_CHANGE = MIDI_PROGRAM_CHANGE_MASK,
    CC = MIDI_CONTROL_CHANGE_MASK,
    SYS_EX = 0xF0,
    INVALID = 0xFF,
};

struct MIDIEvent
{
    MIDIEventType type = INVALID;
};

struct MIDICCEvent: MIDIEvent
{
    uint8_t channel;
    uint8_t number;
    uint8_t value;

    MIDICCEvent(uint8_t channel, uint8_t number, uint8_t value)
    {
        type = CC;
        this->channel = channel;
        this->number = number;
        this->value = value;
    }
};

struct MIDINoteEvent: MIDIEvent
{
    uint8_t channel;
    uint8_t note;
    uint8_t velocity;
};

struct MIDINoteOnEvent: MIDINoteEvent
{
    MIDINoteOnEvent(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        type = NOTE_ON;
        this->channel = channel;
        this->note = note;
        this->velocity = velocity;
    }
};

struct MIDINoteOffEvent: MIDINoteEvent
{
    MIDINoteOffEvent(uint8_t channel, uint8_t note, uint8_t velocity)
    {
        type = NOTE_OFF;
        this->channel = channel;
        this->note = note;
        this->velocity = velocity;
    }
};

#endif // MIDIEvent_h

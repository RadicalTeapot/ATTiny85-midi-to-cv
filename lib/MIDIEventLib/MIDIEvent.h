#ifndef MIDIEvent_h
#define MIDIEvent_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

enum MIDIEventType
{
    NOTE_ON = 0x90,
    NOTE_OFF = 0x80,
    PROGRAM_CHANGE = 0xC0,
    CC = 0xB0,
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

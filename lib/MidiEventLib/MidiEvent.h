#ifndef MidiEvent_h
#define MidiEvent_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

// Local constants
namespace
{
    const uint8_t COMMAND_MASK = 0xF0;
    const uint8_t CHANNEL_COMMAND_MASK = 0x0F;
    const uint8_t CHANNEL_SYSTEM_COMMAND = 0xF0;
    const uint8_t NOTE_OFF_COMMAND = 0x80;
    const uint8_t NOTE_ON_COMMAND = 0x90;
    const uint8_t CONTROL_CHANGE_COMMAND = 0xB0;
    const uint8_t PROGRAM_CHANGE_COMMAND = 0xC0;
}

namespace MidiEventLib
{
    enum class EventType: uint8_t
    {
        NOTE_ON = NOTE_ON_COMMAND,
        NOTE_OFF = NOTE_OFF_COMMAND,
        PROGRAM_CHANGE = PROGRAM_CHANGE_COMMAND,
        CC = CONTROL_CHANGE_COMMAND,
        SYS_EX = CHANNEL_SYSTEM_COMMAND,
        INVALID = 0xFF,
    };

    struct Event
    {
        EventType type = EventType::INVALID;
        uint8_t channel = 0;
        uint8_t firstByte = 0;
        uint8_t secondByte = 0;

        static void setCCEventData(Event *event, uint8_t channel, uint8_t number, uint8_t value)
        {
            event->type = EventType::CC;
            event->channel = channel;
            event->firstByte = number;
            event->secondByte = value;
        }

        static void setNoteOnEventData(Event *event, uint8_t channel, uint8_t note, uint8_t velocity)
        {
            event->type = EventType::NOTE_ON;
            event->channel = channel;
            event->firstByte = note;
            event->secondByte = velocity;
        }

        static void setNoteOffEventData(Event *event, uint8_t channel, uint8_t note, uint8_t velocity)
        {
            event->type = EventType::NOTE_OFF;
            event->channel = channel;
            event->firstByte = note;
            event->secondByte = velocity;
        }
    };

    struct NoteOnEvent
    {
        EventType type = EventType::NOTE_ON;
        uint8_t channel = 0;
        uint8_t note = 0;
        uint8_t velocity = 0;
    };

    struct NoteOffEvent
    {
        EventType type = EventType::NOTE_OFF;
        uint8_t channel = 0;
        uint8_t note = 0;
        uint8_t velocity = 0;
    };

    struct CCEvent
    {
        EventType type = EventType::CC;
        uint8_t channel = 0;
        uint8_t number = 0;
        uint8_t value = 0;
    };
}

#endif // MidiEvent_h

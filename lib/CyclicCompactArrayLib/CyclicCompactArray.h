#ifndef CyclicCompactArray_h
#define CyclicCompactArray_h

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

template <uint8_t maxSize>
class CyclicCompactArray
{
public:
    CyclicCompactArray() : _tail(0) {
        for (uint8_t i = 0; i < maxSize; ++i)
        {
            _data[i] = 0;
        }
    }

    void append(int value)
    {
        _data[_tail] = value;
        _tail = (_tail + 1) % maxSize;
    }

    bool remove(int value)
    {
        uint8_t index = _tail;
        for (uint8_t i = 0; i < maxSize; ++i)
        {
            if (_data[index] == value)
            {
                for (uint8_t j = 0; j < maxSize - i - 1; ++j)
                {
                    uint8_t currentIndex = (index + j) % maxSize;
                    uint8_t nextIndex = (index + j + 1) % maxSize;
                    _data[currentIndex] = _data[nextIndex];
                }
                _tail = (_tail + maxSize - 1) % maxSize;
                _data[_tail] = 0;
                return true;
            }
            index = (index + 1) % maxSize;
        }
        return false;
    }

    int getTail()
    {
        return _data[(_tail + maxSize - 1) % maxSize];
    }

private:
    uint8_t _data[maxSize];
    uint8_t _tail;
};

#endif // CyclicCompactArray_h

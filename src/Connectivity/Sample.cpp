#include "Sample.h"

Sample::Sample(uint8_t _hour, uint8_t _minutes, uint16_t _depth)
{
    date.time.hour = _hour;
    date.time.minutes = _minutes;
    type = unique;
    frequency = 0;
    depth = _depth;
}

Sample::Sample(uint8_t _hour, uint8_t _minutes, uint16_t _depth, uint8_t _frequency)
{
    date.time.hour = _hour;
    date.time.minutes = _minutes;
    type = repeated;
    frequency = _frequency;
    depth = _depth;
}




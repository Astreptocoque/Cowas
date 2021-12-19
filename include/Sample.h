#ifndef SAMPLE_H
#define SAMPLE_H

#include <Arduino.h>
#include "Settings.h"

enum Sampling_type
{
    unique,
    repeated
};

class Sample
{

private:
    struct Date date;
    uint16_t depth;
    uint8_t frequency;
    Sampling_type type;

public:
    Sample(uint8_t _hour, uint8_t _minutes, uint16_t _depth);
    Sample(uint8_t _hour, uint8_t _minutes, uint16_t _depth, uint8_t _frequency);
};

#endif
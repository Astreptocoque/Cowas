#include "Samples.h"



void Samples::add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _depth){
    Sample sample;
    sample.date.time.hour = _hour;
    sample.date.time.minutes = _minutes;
    sample.type = Sampling_type::unique;
    sample.frequency = 0;
    sample.depth = 0;
}

void Samples::add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _frequency, uint8_t _depth){
    Sample sample;
    sample.date.time.hour = _hour;
    sample.date.time.minutes = _minutes;
    sample.type = Sampling_type::repeated;
    sample.frequency = _frequency;
    sample.depth = _depth;
}

Samples sample;
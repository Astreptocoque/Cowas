#include "Samples.h"



void Samples::add_sample(uint8_t _hour, uint8_t _minutes){
    Sample sample;
    sample.time.hour = _hour;
    sample.time.minutes = _minutes;
    sample.type = Sampling_type::unique;
    sample.frequency = 0;
}

void Samples::add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _frequency){
    Sample sample;
    sample.time.hour = _hour;
    sample.time.minutes = _minutes;
    sample.type = Sampling_type::repeated;
    sample.frequency = _frequency;
}

Samples sample;
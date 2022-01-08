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
    struct Date date;       // store hour of sampling
    uint16_t depth;         // depth of the sample in cm
    uint8_t from_day;       // when creating sample, sample in X days
    uint8_t frequency;      // frequency of sampling if sampling is regular
    Sampling_type type;     // if sample is regular or unique
   
public:
    Sample(uint8_t _hour, uint8_t _minutes, uint8_t from_day, uint16_t _depth);
    Sample(uint8_t _hour, uint8_t _minutes, uint8_t from_day, uint16_t _depth, uint8_t _frequency);
    // ~Sample();
    uint8_t get_day();
    uint8_t get_hour();
    uint8_t get_minutes();
    uint16_t Sample::get_depth();
    uint8_t Sample::get_frequency();
    void set_frequency(uint8_t _frequency);

};

uint8_t add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _from_day, uint16_t _depth);
uint8_t add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _from_day, uint16_t _depth, uint8_t _frequency);
void display_samples();

#endif
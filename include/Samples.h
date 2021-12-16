#ifndef SAMPLE_H
#define SAMPLE_H

#include <Arduino.h>
#include "Settings.h"


#define MAX_SAMPLE = 14

enum Sampling_type{unique,repeated};

/**
 * @brief Create a sample that has a time
 * @param hour time for sampling
 * @param minutes time for sampling
 * @param type  unique or repeated sampling
 * @param frequency if type = reapeated, the frequency in days to be repeated
 * 
 */

struct Sample
{
    Date date;
    Sampling_type type = repeated;
    uint8_t frequency = 1;
    uint8_t depth;
};

class Samples
{

private:
    uint8_t current_sample = 0;
public:
    void add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _depth);
    void add_sample(uint8_t _hour, uint8_t _minutes, uint8_t _frequency, uint8_t _depth);
};

#endif
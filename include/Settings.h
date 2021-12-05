#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

#define MAX_SAMPLE = 14

struct Time{
    uint8_t hour;
    uint8_t minutes;
};

struct Date{
    int year;
    uint8_t day;
    uint8_t month;
    struct Time time;
};



#endif
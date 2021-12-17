#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Serial_output.h"

#define MAX_SAMPLE = 14             // max samples before refilling

const int HEIGHT_FROM_WATER = 34;   //in centimeters, ref to spool endstop
const int SPEED_UP = 80;
const int SPEED_DOWN = 50;

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



void update_time();
void set_time_manual();

/* Date current_date; */

#endif
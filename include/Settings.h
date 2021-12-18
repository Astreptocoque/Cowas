#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Serial_output.h"

#define MAX_SAMPLE = 14             // max samples before refilling

// SPOOL CONSTANTS
const int HEIGHT_FROM_WATER = 52;       // in centimeters, ref to spool endstop
const uint8_t DISTANCE_FROM_STOP = 5;   // security distance to come back to origin
const uint8_t SPEED_UP = 90;            // speed when moving up (can be fast, but take water resistance into account)
const uint8_t SPEED_DOWN = 30;          // speed when moving down (slower than up, weight has to sink)
// SYSTEM CONSTANTS
const int UPDATE_TIME = 5;              // refresh frequency for action i.e. sampling

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
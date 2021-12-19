#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Serial_output.h"

#define MAX_SAMPLE 14             // max samples before refilling

// SPOOL CONSTANTS
const int HEIGHT_FROM_WATER = 46;       // in centimeters, ref to spool endstop
const uint8_t DISTANCE_FROM_STOP = 5;   // security distance to come back to origin
const uint8_t SPEED_UP = 80;            // speed when moving up (can be fast, but take water resistance into account)
const uint8_t SPEED_DOWN = 30;          // speed when moving down (slower than up, weight has to sink)
const int TUBE_LENGTH = 4900;           // length of tube
// SYSTEM CONSTANTS
const int UPDATE_TIME = 5;              // refresh frequency for action i.e. sampling
const float EMPTY_WATER_THRESHOLD = 0.2;
const uint32_t DEBOUCHE_CHIOTTE = 10*1000;
const float VACUUM_TO_ACHIEVE = 0.13;   // vacuum to achieve
const float VACUUM_MINIMUM = 0.17;      // vacuum before restarting vacuum pump
const uint32_t DRYING_TIME = 30*1000;    // time for pumping hysteris and heating

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
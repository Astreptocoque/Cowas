#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Serial_output.h"
#include "TimeLib.h"

#define MAX_SAMPLE 2             // max samples before refilling
#define NUMBER_SAMPLES 2

// SPOOL VARIABLES
const int HEIGHT_FROM_WATER = 115;          // in centimeters, ref to spool endstop
const uint8_t DISTANCE_FROM_STOP = 5;       // slow down at this distance from origin
const uint8_t SPEED_UP = 100;               // speed when moving up - experimental tested
const uint8_t SPEED_DOWN = 100;             // speed when moving down - experimental tested
const int TUBE_LENGTH = 49000;              // length of tube

// WATER PUMP VARIABLES
const uint8_t POWER_PUMP = 100;             // speed when pumping from water
const uint8_t POWER_FLUSH = 100;            // speed when pumping from container
const uint8_t POWER_STX = 50;
const uint32_t PURGE_TIME = 330000;         // time after which container should be empty

// VACUUM PUMP VARIABLES
const float VACUUM_TO_ACHIEVE = 0.13;           // vacuum to achieve
const float VACUUM_MINIMUM = 0.17;              // vacuum before restarting vacuum pump
const uint32_t DRYING_TIME = 30*1000;           // time for pumping hysteris and heating

// System variables
const int UPDATE_TIME = 5;                      // refresh frequency for action i.e. sampling
const float EMPTY_WATER_THRESHOLD = 0.2;
const uint32_t DEBOUCHE_CHIOTTE = 10*1000;      // milliseconds
const uint32_t PREPARATION_TIME = 60*30;         // seconds. system needs 30 minutes preparation before sampling
const uint8_t PURGE_NUMBER = 2;                 // number of water container purge before sampling
const uint32_t SYNC_TIME = 32400;               // every 9 hours

struct Time{
    uint8_t hour;
    uint8_t minutes;
};

struct Date{
    int year;
    uint8_t day;
    uint8_t month;
    time_t epoch;
    struct Time time;
};

#endif
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "TimeLib.h"

// ===> PINS DECLARATION AT THE BOTTOM


// global const = static = each file has its own decleration

// =============== VARIABLES ===============
// spool variables
const int HEIGHT_FROM_WATER = 10;          // in centimeters, ref to spool endstop
const uint8_t DISTANCE_FROM_STOP = 5;       // slow down at this distance from origin
const uint8_t SPEED_UP = 100;               // speed when moving up - experimental tested
const uint8_t SPEED_DOWN = 100;             // speed when moving down - experimental tested
const int TUBE_LENGTH = 49000;              // length of tube

// water pump variables
const uint8_t POWER_PUMP = 100;             // speed when pumping from water
const uint8_t POWER_FLUSH = 100;            // speed when pumping from container
const uint8_t POWER_STX = 35;
const uint32_t PURGE_TIME = 60*1000*6;         // time after which container should be empty

// vacuum pump variables
const float VACUUM_TO_ACHIEVE = 0.13;           // vacuum to achieve
const float VACUUM_MINIMUM = 0.20;              // vacuum before restarting vacuum pump
const uint32_t DRYING_TIME = 5000; //30*1000;           // time for pumping hysteris and heating

// system variables
const int UPDATE_TIME = 1000;                      // milliseconds. refresh frequency for action i.e. sampling
const float EMPTY_WATER_PRESSURE_PURGE_THRESHOLD = 0.04;        // bar. threshold of pressure considered as empty (no water)
const float EMPTY_WATER_PRESSURE_STX_THRESHOLD = 1;           //bar
const uint32_t EMPTY_WATER_SECURITY_TIME = 10*1000;      // milliseconds. time to ensure a correct flush of the conainter. milliseconds
const uint32_t PREPARATION_TIME = 60*30;         // seconds. system needs 30 minutes preparation before sampling
const uint8_t PURGE_NUMBER = 2;                 // number of water container purge before sampling
const uint32_t SYNC_TIME = 32400;               // every 9 hours
const uint8_t MAX_FILTER_NUMBER = 2;             // max samples allowed in the system. For actuactor init purpose
extern uint8_t FILTER_IN_SYSTEM;           // max samples currently inserted in the system
const bool ENABLE_TIME_LOG = false;

// ============= TIME MANAGEMENT ==============
struct Time{
    uint8_t hour;
    uint8_t minute;
};

struct Date{
    int year;
    uint8_t day;
    uint8_t month;
    time_t epoch;
    struct Time time;
};

// ========== SYSTEM STATES =============
// system states are here for the human interface. The goal is to have the system updating its
// state when necessary and communicating it to the wifi card. When connecting to the server,
// the wifi card will send the state and by this mean enable or disable control functions.
enum System_state{
    state_starting,         // indicate not in normal running mode
    state_idle,             // do nothing, wait for next sterivex. Available for other tasks
    state_sampling,         // currently sampling
    state_refill,           // need to refill sterivex
    state_communicating,    // when communicating with wifi card
    state_error,            // system not working for a reason
};

System_state get_system_state();
void set_system_state(System_state state);
String format_date_logging(time_t t);
String format_date_friendly(time_t t);

// ============ PIN DEFINITIONS ==================
const uint8_t STATUS_LED_PIN = 22;
const uint8_t GREEN_LED_PIN = 23;
const uint8_t PRESSURE1_PIN = 3;
const uint8_t PRESSURE2_PIN = 4;
const uint8_t VALVE_1_PIN = 44;
const uint8_t VALVE_23_PIN = 46;
const uint8_t VALVE_PURGE = 30;         // heater 1 for now (miss a cable)
const uint8_t PUMP_PIN = 6;
const uint8_t PUMP_VACUUM = 34;
const uint8_t ENCODER_A_PIN = 31;
const uint8_t ENCODER_B_PIN = 33;
const uint8_t ENCODER_Z_PIN = 35;
const uint8_t BUTTON_START_PIN = 24;
const uint8_t BUTTON_CONTAINER_PIN = 27;
const uint8_t BUTTON_SPOOL_UP = 28;
const uint8_t BUTTON_SPOOL_DOWN = 29;
const uint8_t BUTTON_LEFT_PIN = 25;
const uint8_t BUTTON_RIGHT_PIN = 26;
const uint8_t POTENTIOMETER_PIN = A0;
const uint8_t SD_CARD_SS_PIN = 5;
const uint8_t ESP8266_COMM_PIN = 12; // communication signal pin with wifi card
const uint8_t VALVE_STX_IN_PIN[MAX_FILTER_NUMBER] = {36, 38};
const uint8_t VALVE_STX_OUT_PIN[MAX_FILTER_NUMBER] = {32, 40};
const uint8_t MOTOR_INA1_PIN = 53;
const uint8_t MOTOR_INB1_PIN = 51;
const uint8_t MOTOR_PWM1_PIN = 7;
const uint8_t MOTOR_EN1DIAG1_PIN = 49;
const uint8_t MOTOR_CS1_PIN = A11;
const uint8_t MOTOR_INA2_PIN = 52;
const uint8_t MOTOR_INB2_PIN = 50;
const uint8_t MOTOR_PW2_PIN = 48;
const uint8_t MOTOR_EN2DIAG2_PIN = 47;
const uint8_t MOTOR_CS2_PIN = A10;
const uint8_t HEATER_PIN[MAX_FILTER_NUMBER] = {30, 42};


#endif
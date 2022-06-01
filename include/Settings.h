#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "TimeLib.h"

// ===> PINS DECLARATION AT THE BOTTOM


// global const = static = each file has its own declaration

// =============== COWAS VARIABLES ===============
// spool variables
const uint8_t HEIGHT_FROM_WATER = 50;           // cm. between water level and spool endstop
const uint8_t DISTANCE_FROM_STOP = 5;       // cm. distance from spool endstop at which speed is decreased
const uint8_t SPEED_UP = 100;               // over 100. Speed when moving up - experimentaly tested
const uint8_t SPEED_DOWN = 100;             // over 100. Speed when moving down - experimentaly tested
const uint16_t TUBE_LENGTH = 4900;              // cm. length of tube (not sure of this number)

// water pump variables
const uint8_t POWER_PUMP = 50;                              // over 100. Power when pumping from water. Experimentaly tested to not go over 500mA
const uint8_t POWER_FLUSH = 100;                            // over 100. Power when pumping from container
const uint8_t POWER_STX = 45;                               // over 100. Experimental. Start power for sterivex but code adapts it.
const uint32_t EMPTY_CONTAINER_TIME_PURGE = 60*1000*8;      // ms. Experimental. Time after which container should be empty
const uint32_t EMPTY_TUBE_TIME = 60*1000*1;                 // ms. Experimental. Time after which Tubes should be empty
const uint32_t EMPTY_CONTAINER_TIME_FILTER = 60*1000*18;    // ms. Experimental.
const uint32_t FILL_TUBES_WITH_WATER_TIME = 5*1000;         // ms. Experimental. Time to fill tubes for purge and sampling before sensor take over
const uint32_t FILL_CONTAINER_TIME = 60*1000*11;            // ms. Experimental.


// vacuum pump variables
const float VACUUM_TO_ACHIEVE = 0.13;               // bar from atmsophere. Vacuum to achieve
const float VACUUM_MINIMUM = 0.20;                  // bar from atmosphere. Vacuum before restarting vacuum pump
const uint32_t DRYING_TIME = 2*60*1000-10*1000;     // ms. Time for pumping hysteris and heating

// system variables
const int UPDATE_TIME = 1000;                                   // ms. Refresh frequency for main program
const float EMPTY_WATER_PRESSURE_PURGE_THRESHOLD = 0.06f;       // bar from atmosphere. Threshold of pressure in tube considered as empty when purging
const float EMPTY_WATER_PRESSURE_STX_THRESHOLD = 1.7f;          // bar from atmosphere. Threshold of pressure in tube considered as empty when filtering
const uint32_t EMPTY_WATER_SECURITY_TIME = 5*1000;              // ms. Time to ensure a correct flush of the container when purging
const uint32_t EMPTY_WATER_STX_SECURITY_TIME = 10*1000;         // ms. Time to ensure a correct flush of the conainter when filtering
const uint32_t PREPARATION_TIME = 60*30;                        // ms. system needs 30 minutes preparation before sampling
const uint8_t PURGE_NUMBER = 1;                                 // number of water container purge before sampling
const uint32_t SYNC_TIME = 32400;                               // ms. Time before refetching wifi time. Not implemented
const uint8_t MAX_FILTER_NUMBER = 2;                            // max filters possible in the system
extern uint8_t FILTER_IN_SYSTEM;                                // max filters currently inserted in the system
extern bool ENABLE_OUTPUT;                                      // Enable or disable output printing, even if trying
const bool DEBUG_MODE_PRINT = false;

// ============= TIME MANAGEMENT ==============
struct Time{
    uint8_t hour;
    uint8_t minute;
};

struct Date{
    uint16_t year;
    uint8_t day;
    uint8_t month;
    time_t epoch;
    struct Time time;
};

String format_date_logging(time_t t);
String format_date_friendly(time_t t);
time_t timeToEpoch(uint8_t _hour, uint8_t _minute, uint8_t _day, uint8_t _month, uint16_t _year);

// ============ SYSTEM STATES ================
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
void enable_output(bool enable);

// ============ VERBOSE DEFINITIONS ==================
// To print or not the infos of a subsystem in the terminal
const bool ENABLE_TIME_LOG = false;                              // If true, print the time before each printed output
const bool VERBOSE_INIT = false;
const bool VERBOSE_VALVES = false;
const bool VERBOSE_MOTOR = false;
const bool VERBOSE_PURGE = false;
const bool VERBOSE_PURGE_PRESSURE = false;
const bool VERBOSE_SAMPLE = false;
const bool VERBOSE_SAMPLE_PRESSURE = true;
const bool VERBOSE_PUMP = false;
const bool VERBOSE_REWIND = false;
const bool VERBOSE_FILL_CONTAINER = false;
const bool VERBOSE_DIVE = false;
const bool TIMER = true;
const bool PRESSURE_SENSOR_ERROR = false;


// ============ PIN DEFINITIONS ==================
// To update with pinout table sheet
const uint8_t STATUS_LED_PIN = 22;
const uint8_t GREEN_LED_PIN = 23;
const uint8_t PRESSURE1_PIN = 3;
const uint8_t PRESSURE2_PIN = 4;
const uint8_t VALVE_1_PIN = 44;
const uint8_t VALVE_23_PIN = 46;
const uint8_t VALVE_PURGE = 30; // heater 1 but valve purge for now (miss a cable)
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
const uint8_t HEATER_PIN[MAX_FILTER_NUMBER] = {30}; //{30, 42};

const uint8_t PUMP_TEST_INPUT = 9;
const uint8_t PUMP_TEST_OUTPUT = 10;

#endif
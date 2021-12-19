#include <Arduino.h>
#include "Button.h"
#include "Trustability_ABP_Gage.h"
#include "Valve_3_2.h"
#include "Valve_2_2.h"
#include "Pump.h"
#include "Motor.h"
#include "Encoder.h"
#include "Potentiometer.h"
#include "Led.h"
#include <SPI.h>
#include "Serial_output.h"
#include "sample.h"
#include "Settings.h"
#include "Critical_error.h"
#include "Timer.h"
#include "Serial_device.h"
#include "Main_functions.h"

extern Serial_output output;
extern Serial_device serial;
extern Led blue_led;
extern Led green_led;
extern Trustability_ABP_Gage pressure1;
extern Trustability_ABP_Gage pressure2;
extern Valve_2_2 valve_1;
extern Valve_3_2 valve_23;
extern Valve_2_2 valve_stx_1_in;
extern Valve_2_2 valve_stx_2_in;
extern Valve_3_2 valve_stx_1_out;
extern Valve_3_2 valve_stx_2_out;
extern Pump pump;
extern Pump pump_vacuum;
extern Motor spool;
extern Encoder encoder;
extern Button button_start;     // normally open
extern Button button_container; // normally ???
extern Button button_spool_up;     // normally closed
extern Button button_spool_down;
extern Button button_left;      // normally open
extern Button button_right;     // normally open
extern Potentiometer potentiometer;
extern struct Timer timer_control_pressure1;

void step_dive()
{
}

void step_purge()
{
}

void step_fill_container()
{
}

void step_sampling()
{
}

void step_flush_water()
{
}

void step_rewind()
{
}
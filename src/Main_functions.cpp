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

#define DELAY_ACTIONS 500

extern Serial_output output;
extern Serial_device serial;
extern Led blue_led;
extern Led green_led;
extern Trustability_ABP_Gage pressure1;
extern Trustability_ABP_Gage pressure2;
extern Valve_2_2 valve_1;
extern Valve_3_2 valve_23;
extern Valve_2_2 valve_purge;
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

void step_dive(int _depth)
{
    valve_1.set_open_way();
    delay(DELAY_ACTIONS);
    spool.set_speed(SPEED_DOWN, down);
    spool.start(_depth);
    valve_1.set_close_way();
}

void step_fill_container()
{
    valve_23.set_I_way();
    delay(DELAY_ACTIONS);
    pump.set_power(100);
    pump.start();
    while(button_container.getState() == 1);
    pump.stop();
    valve_23.set_L_way();
}

void step_purge()
{
    // add loop for sterivex array
    valve_stx_1_in.set_close_way();
    valve_stx_2_in.set_close_way();
    valve_23.set_L_way();
    valve_purge.set_open_way();
    delay(DELAY_ACTIONS);

    pump.set_power(100);

    // monitor pressure in parallel
    // timerStart(timer_control_pressure1);
    pump.start();
    uint32_t time1 = millis();
    // while(pressure1.getPressure() > EMPTY_WATER_THRESHOLD && millis()-time1 < 40000);
    while(millis()-time1 < 20000000);
    pump.start(DEBOUCHE_CHIOTTE); // auto-stop
    // timerStop(timer_control_pressure1);

    valve_purge.set_close_way();


}

void step_sampling(int num_sterivex)
{
    valve_23.set_L_way();
    valve_purge.set_close_way();
    // TODO add loop to close all sterivex and open the right one
    if(num_sterivex == 1){
        valve_stx_1_in.set_open_way();
        valve_stx_2_in.set_close_way();
        valve_stx_1_out.set_I_way();
        valve_stx_2_out.set_L_way();
    }else if(num_sterivex == 2){
        valve_stx_1_in.set_close_way();
        valve_stx_2_in.set_open_way();
        valve_stx_1_out.set_L_way();
        valve_stx_2_out.set_I_way();
    }
    delay(DELAY_ACTIONS);

    pump.set_power(50);

    // monitor pressure in parallel
    timerStart(timer_control_pressure1);
    pump.start();
    while(pressure1.getPressure() > EMPTY_WATER_THRESHOLD);
    pump.start(DEBOUCHE_CHIOTTE); // auto-stop
    timerStop(timer_control_pressure1);

     // TODO add loop to close specific sterivex
    valve_stx_1_in.set_close_way();
    valve_stx_2_in.set_close_way();
    valve_stx_1_out.set_L_way();
    valve_stx_2_out.set_L_way();
}

void step_rewind()
{
    valve_1.set_open_way();
    valve_23.set_L_way();
    delay(DELAY_ACTIONS);

    spool.set_speed(SPEED_UP, up);
    spool.start(-1);
}

void step_dry(int num_sterivex)
{
    // TODO loop to close all one-way sterivex valves
    valve_stx_1_in.set_close_way();
    valve_stx_2_in.set_close_way();

    if(num_sterivex == 1){
        valve_stx_1_out.set_L_way();
        valve_stx_2_out.set_I_way();
    }else if(num_sterivex == 2){
        valve_stx_1_out.set_I_way();
        valve_stx_2_out.set_L_way();
    }
    delay(DELAY_ACTIONS);
    
    // TODO : function interrupt for pressure2
    uint32_t currentTime = millis();
    while(millis() - currentTime < DRYING_TIME){
        if(pressure2.getPressure() > VACUUM_MINIMUM){
            pump_vacuum.start();
            while(pressure2.getPressure() > VACUUM_TO_ACHIEVE);
            pump_vacuum.stop();
        }
        delay(50);
    }

    // TODO set L way for valve out of considered sterivex
    valve_stx_1_out.set_L_way();
    valve_stx_2_out.set_L_way();

}
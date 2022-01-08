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
#include "Step_functions.h"

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
extern Valve_2_2 * valve_stx_in;
extern Valve_3_2 * valve_stx_out;
extern Pump pump;
extern Pump pump_vacuum;
extern Motor spool;
extern Encoder encoder;
extern Button button_start;    
extern Button button_container;
extern Button button_spool_up;     
extern Button button_spool_down;
extern Button button_left;      
extern Button button_right;
extern Potentiometer potentiometer;
extern struct Timer timer_control_pressure1;

// ============= DESCRIPTION ==============
// Code for all steps of execution of COWAS with high level functions.
// Each step should be independant, that is be responible to have all valves 
// and actuators being correctely set.
// See fluidic diagram for details.

/**
 * @brief Unroll the spool at the correct depth
 * 
 * @param _depth absolute depth at which the sampling is going to be done.
 */
void step_dive(int _depth)
{
    // let air escape the system while diving
    valve_1.set_open_way();
    delay(DELAY_ACTIONS);
    spool.set_speed(SPEED_DOWN, down);      
    spool.start(_depth);
    valve_1.set_close_way();
}

/**
 * @brief Fill with water the container. Step_dive first.
 * 
 */
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

/**
 * @brief Empty the container through purge channel. Step_fill_container first.
 * 
 */
void step_purge()
{
    // make sure all sterivex valves are closed
    for(uint8_t i = 0; i < MAX_SAMPLE; i++) valve_stx_in[i].set_close_way();

    valve_23.set_L_way();
    valve_purge.set_open_way();
    delay(DELAY_ACTIONS);

    pump.set_power(100);

    // monitor pressure in parallel
    timerStart(timer_control_pressure1);
    
    pump.start();
    uint32_t time1 = millis();
    while(pressure1.getPressure() > EMPTY_WATER_THRESHOLD && millis()-time1 < PURGE_TIME);
    // pump a little bit more to flush all water
    pump.start(DEBOUCHE_CHIOTTE);
    
    timerStop(timer_control_pressure1);

    valve_purge.set_close_way();
}

/**
 * @brief Empty water from container into choosen sterivex. Step_fill_container first.
 * 
 * @param num_sterivex The sterivex in which the sampling is made
 */
void step_sampling(uint8_t num_sterivex)
{
    valve_23.set_L_way();
    valve_purge.set_close_way();
    for(uint8_t i = 0; i < MAX_SAMPLE; i++){
        if(i == num_sterivex){
            valve_stx_in[i].set_open_way();
            valve_stx_out[i].set_I_way();
        }else{
            valve_stx_in[i].set_close_way();
            valve_stx_out[i].set_L_way();
        }
    }
    delay(DELAY_ACTIONS);

    pump.set_power(POWER_STX);

    // monitor pressure in parallel
    timerStart(timer_control_pressure1);
    
    pump.start();
    uint32_t time1 = millis();
    while(pressure1.getPressure() > EMPTY_WATER_THRESHOLD && millis()-time1 < PURGE_TIME);
    // pump a little bit more to flush all water    
    pump.start(DEBOUCHE_CHIOTTE); 
    
    timerStop(timer_control_pressure1);

    valve_stx_in[num_sterivex].set_close_way();
    valve_stx_out[num_sterivex].set_L_way();
}

/**
 * @brief Roll the spool back. Step_dive first.
 * 
 */
void step_rewind()
{
    // let air enter the system
    valve_1.set_open_way();
    valve_23.set_L_way();
    delay(DELAY_ACTIONS);

    spool.set_speed(SPEED_UP, up);
    // go to origin
    spool.start(-1);
}

/**
 * @brief Dry a sterivex. Step_sample first.
 * 
 * @param num_sterivex the sterivex to dry
 */
void step_dry(uint8_t num_sterivex)
{
    for(uint8_t i = 0; i < MAX_SAMPLE; i++){
        valve_stx_in[i].set_close_way();

        if(i == num_sterivex)
            valve_stx_out[i].set_L_way();
        else
            valve_stx_out[i].set_I_way();
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

    for(uint8_t i = 0; i < MAX_SAMPLE; i++){
        valve_stx_out[i].set_L_way();
    }
}
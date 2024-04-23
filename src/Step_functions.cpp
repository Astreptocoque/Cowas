/**
 * @file Step_functions.cpp
 * @author Timothée Hirt & Christophe Deloose & Paco Mermoud
 * @brief All detailed steps of a sampling process. Each function shall be independant
 *        from the other and make sure the valves are correctely set
 *        See fluidic diagram for all details and understand the steps
 * @version 0.1
 * @date 2022-01-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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
#include "C_output.h"
#include "Settings.h"
#include "Critical_error.h"
#include "Timer.h"
#include "Serial_device.h"
#include "Step_functions.h"
#include "Manifold.h"

// Delay when actuating valves
#define DELAY_ACTIONS 1000

extern C_output output;
extern Serial_device serial;
extern Led blue_led;
extern Led green_led;
extern Trustability_ABP_Gage pressure1;
extern Trustability_ABP_Gage pressure2;
extern Valve_2_2 valve_1;
extern Valve_3_2 valve_23;
extern Valve_2_2 valve_manifold;
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
extern Manifold manifold;

/**
 * @brief Unroll the spool at the correct depth
 * 
 * @param _depth Absolute depth at which to go.
 */
void step_dive(int _depth)
{
    if(VERBOSE_DIVE){output.println("Step dive started");}

    valve_23.set_L_way();
    valve_1.set_open_way();  // let air escape the system while diving
    delay(DELAY_ACTIONS);

    uint32_t time1 = millis();

    spool.set_speed(SPEED_DOWN, down);
    spool.start(_depth);

    if(VERBOSE_DIVE){output.println("Time to dive : " + String(millis() - time1) + " ms");}

    valve_1.set_close_way();

    if(VERBOSE_DIVE){output.println("Step dive ended");}
}

/**
 * @brief Fill with water the container. Step_dive first.
 * 
 */
void step_fill_container()
{
    if(VERBOSE_FILL_CONTAINER){output.println("Step fill container started");}


    valve_1.set_close_way();
    valve_23.set_I_way();
    delay(DELAY_ACTIONS);

    uint32_t time1 = millis();
    bool run = true;

    pump.set_power(POWER_PUMP);
    pump.start();

    // two possibilities to stop filling : switch or time
    do
    {
        if (button_container.getState() == 0)
        {
            run = false;
            if(VERBOSE_FILL_CONTAINER){output.println("Fill container stopped by button");}
        }
        // TODO : better function. cannot be constant time because time depends of how deep we sample
        // --> water need to go through the spool tube before entering the system
        else if (millis() - time1 > FILL_CONTAINER_TIME)
        {
            run = false;
            if(VERBOSE_FILL_CONTAINER){output.println("Fill container stopped by security timer");}
            // TODO : raise a system warning to user
        }
    } while (run); // conditions are ouside loop to print what condition is responible for stopping

    pump.stop();

    if(VERBOSE_FILL_CONTAINER || TIMER){output.println("Time to fill container : " + String(millis() - time1) + " ms");}

    delay(DELAY_ACTIONS);
    valve_23.set_L_way();

    if(VERBOSE_FILL_CONTAINER){output.println("Step fill container ended");}
}

/**
 * @brief Empty the container through purge channel. Step_fill_container first.
 * 
 */
void step_purge()
{

    if(VERBOSE_PURGE){output.println("Step purge container started");}
    

    // set the valves
    rotateMotor(PURGE_SLOT); // purge slot
    valve_manifold.set_open_way();
    valve_23.set_L_way();
    delay(DELAY_ACTIONS);

    pump.set_power(POWER_FLUSH);

    // first action is to fill all tubes with water
    // otherwise the sensor will detect end of emptying
    pump.start(FILL_TUBES_WITH_WATER_TIME);

    // then start with sensor reading
    pump.start();
    uint32_t time1 = millis();
    uint8_t validation_tick = 0;
    float pressure;

    bool run = true;

    // loop stops after 2.5 seconds under threshold pressure or when time max is reach
    do
    {
        delay(10); // don't read pressure to fast

        pressure = pressure1.getPressure();
        if(VERBOSE_PURGE_PRESSURE){output.println("Pressure equal : " + String(pressure));}
        

        // adapt pump power to pressure to not go over limit of 3 bar
        // simple incremental controler. Not the best one but does the job
        if (pressure > 3)
            pump.set_power(pump.get_power() - 2);
        else if (pressure < 2.6f)
            pump.set_power(pump.get_power() + 2);

        // if pressure is low enough, engage stopping processure
        if (pressure1.getPressure() < EMPTY_WATER_PRESSURE_PURGE_THRESHOLD)
            validation_tick++;
        else
            validation_tick = 0;

        if (validation_tick >= 250)
        {
            run = false;
            if(VERBOSE_PURGE){output.println("Purge stopped by low pressure");}
        }
        else if (millis() - time1 > EMPTY_CONTAINER_TIME_PURGE)
        {
            run = false;
            if(VERBOSE_PURGE){output.println("Purge stopped by security timer");}
            // TODO : raise a system warning to user
        }

    } while (run); // conditions outside while loop to allow printing which condition is responsible for stop

    // pump a little bit more to flush all water
    pump.start(EMPTY_WATER_SECURITY_TIME);

    if(VERBOSE_PURGE || TIMER){output.println("Time to purge container : " + String(millis() - time1) + " ms");}

    delay(DELAY_ACTIONS);
    valve_manifold.set_close_way();

    if(VERBOSE_PURGE){output.println("Step purge container ended");}
}

/**
 * @brief Empty water from container into choosen filter. Step_fill_container first.
 * 
 * @param num_filter The filter in which the sampling is made
 */
void step_sampling(int slot_manifold)
{
    if(VERBOSE_SAMPLE){output.println("Step sample through filter started");}

    // set the valves
    rotateMotor(slot_manifold);
    valve_23.set_L_way();
    valve_manifold.set_open_way();
    delay(DELAY_ACTIONS);

    pump.set_power(POWER_STX);

    // start by filling all tubes with water to have enough pressure (sensor)
    pump.start(FILL_TUBES_WITH_WATER_TIME);

    pump.start();
    uint32_t time1 = millis();
    uint32_t validation_tick = 0;
    float pressure;
    bool run = true;
    float MIN_POWER_MOTOR=20;
    float MAX_POWER_MOTOR=100;
    int compteur=0;

    // P-Controler parameters
    float POUT_TARGET = 3;
    float error = 0;
    float gain = 0;
    float offset = 0;
    float Kp = 90;

    // loop stops after 2.5 seconds under pressure threshold or when time max is reached
    do
    {
        delay(10); // don't read pressure to fast
        pressure = pressure1.getPressure();
        
        if (compteur==10 && VERBOSE_SAMPLE_PRESSURE)
        {
            output.println("Pressure = " + String(pressure)+ ",   pump power = " +String(pump.get_power()));
            compteur=0;
        }

        // adapt pump power to pressure to not go over limit of 3 bar
        error=POUT_TARGET-pressure;
        gain=error*Kp + offset;

        if (gain > MAX_POWER_MOTOR)
        {
            gain=100;
        }
        else if (gain < MIN_POWER_MOTOR)
        {
            gain=0;
        }

        pump.set_power(gain);

        // if pressure low enough, engage stopping processure
        if (pressure < EMPTY_WATER_PRESSURE_STX_THRESHOLD){
            validation_tick++;}
        else
            validation_tick = 0;

        if (validation_tick >= 2000)
        {
            run = false;
            if(VERBOSE_SAMPLE){output.println("Sampling stopped by low pressure");}
        }
        else if (millis() - time1 > EMPTY_CONTAINER_TIME_FILTER && !TIMER)
        {
            run = false;
            if(VERBOSE_SAMPLE){output.println("Sampling stopped by security timer");}
            // TODO : raise a system warning to user
        }
        compteur++;
    } while (run); // conditions outside while loop to allow printing which condition is responsible for stop

    // pump a little bit more to flush all water
    pump.start(EMPTY_WATER_STX_SECURITY_TIME);

    if(VERBOSE_SAMPLE || TIMER){output.println("Time to sample water : " + String(millis() - time1) + " ms");}

    delay(DELAY_ACTIONS);
    valve_manifold.set_close_way();

    // Do a purge to facilitate emptying the tubes from remaining water (hard trough filter)
    step_purge();

    // after purge, all tube are mainly empty. Push air through sterivex again to really flush last water in filter tubing
    purge_sterivex(slot_manifold);

    if(VERBOSE_SAMPLE){output.println("Step sample through filter ended");}
}

/**
 * @brief Purge Sterivex
 * 
 */
void purge_sterivex(int slot_manifold)
{
    rotateMotor(slot_manifold);
    valve_23.set_L_way();
    valve_manifold.set_open_way();

    delay(DELAY_ACTIONS);

    pump.set_power(100);
    pump.start(EMPTY_WATER_SECURITY_TIME * 4);
    delay(DELAY_ACTIONS);

    valve_manifold.set_close_way();

}

/**
 * @brief Roll the spool back. Step_dive first.
 * 
 */
void step_rewind()
{
    if(VERBOSE_REWIND){output.println("Step rewind started");}

    valve_1.set_open_way(); // let air enter the system
    valve_23.set_L_way();
    delay(DELAY_ACTIONS);

    uint32_t time1 = millis();

    spool.set_speed(SPEED_UP, up);
    // go to origin
    spool.start(-1);

    if(VERBOSE_REWIND){output.println("Time to rewind : " + String(millis() - time1) + " ms");}

    delay(DELAY_ACTIONS);
    valve_1.set_close_way();

    if(VERBOSE_REWIND){output.println("Step rewind ended");}
}

/**
 * @brief Dry a sterivex. Step_sample first.
 *        FUNCTION NOT CORRECTLY COMPLETED.
 *        TO ADAPT WITH SYSTEM.
 * 
 * @param num_filter the sterivex to dry
 */
void step_dry(uint8_t num_filter)
{
    output.println("Step drying started");

    // 10 secondes toutes les 2 minutes 3x
    // temporaire

    // set the valves
    // TODO add valves

    delay(DELAY_ACTIONS);

    // TODO : function interrupt for pressure2
    uint32_t time1 = millis();

    for (uint8_t i = 0; i < 3; i++)
    {
        pump_vacuum.set_power(100);
        pump_vacuum.start(10000);
        while (millis() - time1 < DRYING_TIME)
        {
            // if(pressure2.getPressure() > VACUUM_MINIMUM){
            //     pump_vacuum.start();
            //     while(pressure2.getPressure() > VACUUM_TO_ACHIEVE && millis() - time1 < DRYING_TIME);
            //     pump_vacuum.stop();
            // }
            delay(50);
        }
    }

    // TODO : close valves

    output.println("Step drying ended");
}

void purge_Pipes(){
    step_rewind();
        if(VERBOSE_FILL_CONTAINER){output.println("Step fill container started");}

    valve_1.set_close_way();
    valve_23.set_I_way();
    delay(DELAY_ACTIONS);

    uint32_t time1 = millis();
    bool run = true;

    pump.set_power(POWER_PUMP);
    pump.start();

    // two possibilities to stop filling : switch or time
    do
    {
        if (millis() - time1 > EMPTY_TUBE_TIME)
        {
            run = false;
        }
    } while (run); // conditions are ouside loop to print what condition is responible for stopping

    pump.stop();
    delay(DELAY_ACTIONS);
    valve_23.set_L_way();
}

void sample_process(int depth){
    // Verify if available filter
    bool filter_available= false;
    int manifold_slot=0;
    for(int i=1; i < NB_SLOT; i++){
        if(manifold.get_state(i) == available){
            manifold.change_state(i, unaivailable);
            filter_available= true;
            manifold_slot=i;
            break;
        }
    }

    if(filter_available == false){
        output.println("No filter available");
        return;
    }


    uint32_t time_sampling = millis();

    step_rewind();
    set_system_state(state_sampling);
    if(VERBOSE_SAMPLE){output.println("It's sampling time !");}
    if(VERBOSE_SAMPLE){output.println("Sample started at depth " + String(depth) + "cm in filter ");}
    // Sampling steps
    step_dive(depth);
    for(uint8_t i = 0; i < PURGE_NUMBER; i++){
        step_fill_container();
        step_purge();
    }
    step_fill_container();
    step_rewind();
    step_sampling(manifold_slot); // sample place is a human number, start at 1
    // step_dry(get_next_sample_place());   // not completely done yet

    // ! TODO: add DNA shield here

    if(VERBOSE_SAMPLE || TIMER){output.println("Time for complete sample : " + String(millis()-time_sampling) + " ms");}

}
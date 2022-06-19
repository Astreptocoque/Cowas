/**
 * @file Tests.cpp
 * @author Timothée Hirt
 * @brief Try things, be free. No guarantie that those functions work.
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
#include "C_output.h"
#include "sample.h"
#include "Settings.h"
#include "Timer.h"
#include "Serial_device.h"
#include "Tests.h"
#include "Step_functions.h"

extern C_output output;
extern Serial_device serial;
extern Led status_led;
extern Led green_led;
extern Trustability_ABP_Gage pressure1;
extern Trustability_ABP_Gage pressure2;
extern Valve_2_2 valve_1;
extern Valve_3_2 valve_23;
extern Valve_2_2 valve_purge;
extern Valve_2_2 valve_stx_in[MAX_FILTER_NUMBER];
extern Valve_3_2 valve_stx_out[MAX_FILTER_NUMBER];
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
extern struct Timer timer_control_pressure2;

void tests()
{

    if (serial.available())
    {
        output.println(serial.receive());
    }
    // output.println(pressure1.getPressure());
    // output.println(pressure1.getTemperature());
    // delay(500);
}

void test_hardware_general()
{

    output.println("Valve 1");
    button_left.waitPressedAndReleased();
    valve_1.switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_1.switch_way();

    output.println("Valve 23");
    button_left.waitPressedAndReleased();
    valve_23.switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_23.switch_way();
    
    output.println("Valve purge");
    button_left.waitPressedAndReleased();
    valve_purge.switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_purge.switch_way();

    output.println("Valve stx 1 in");
    button_left.waitPressedAndReleased();
    valve_stx_in[0].switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_stx_in[0].switch_way();

    output.println("Valve stx 2 in");
    button_left.waitPressedAndReleased();
    valve_stx_in[1].switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_stx_in[1].switch_way();

    output.println("Valve stx 1 out");
    button_left.waitPressedAndReleased();
    valve_stx_out[0].switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_stx_out[0].switch_way();

    output.println("Valve stx 2 out");
    button_left.waitPressedAndReleased();
    valve_stx_out[1].switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_stx_out[1].switch_way();

    output.println("Pump vaccum");
    button_left.waitPressedAndReleased();
    pump_vacuum.start();
    delay(500);
    button_left.waitPressedAndReleased();
    pump_vacuum.stop();
}

void test_serial_device()
{
    if (Serial1.available())
    {
        output.println(Serial1.read());
        output.println("");
    }
}

void test_pressure()
{

    int pot_last_value = potentiometer.get_value(0, 100);
    int pot_value = 0;
    int speedy = 60;
    while (!button_start.isPressed())
    {
        pot_value = potentiometer.get_value(0, 100);
        if (pot_value <= pot_last_value - 4 || pot_value >= pot_last_value + 4)
        {
            speedy = pot_value;
            pot_last_value = pot_value;
            output.println("speed " + String(speedy));
        }
        if (button_left.isPressed())
        {
            button_left.waitPressedAndReleased();
            pump.set_power(speedy);
            pump.start();
        }
        if (button_right.isPressed())
        {
            pump.stop();
            button_right.waitPressedAndReleased();
        }

        output.println("P1 " + String(pressure1.getPressure()) + "  |   P2 " + String(pressure2.getPressure()));
        delay(200);
    }
}

void test_1_depth_20m()
{
    output.println("TEST 1 - ALTIMETRE 20");
    uint32_t lastTime = 0;



    spool.start_origin();
    status_led.on();
    output.println("=========== go 30cm");
    button_start.waitPressedAndReleased();
    status_led.off();
    lastTime = millis();
    spool.start(30);
    output.println(millis()-lastTime);

    status_led.on();
    output.println("=========== go 1m");
    button_start.waitPressedAndReleased();
    status_led.off();
    lastTime = millis();
    spool.start(100);
    output.println(millis()-lastTime);

    status_led.on();
    output.println("=========== go 2m");
    button_start.waitPressedAndReleased();
    status_led.off();
    lastTime = millis();
    spool.start(200);
    output.println(millis()-lastTime);

    int i = 1;
    while (i < 5)
    {
        status_led.on();
        output.println("=========== go " + String(i*5) + "m");
        button_start.waitPressedAndReleased();
        status_led.off();
        lastTime = millis();
        spool.start(i*5*100);
        output.println(millis()-lastTime);
        i++;
    }

    i = 4;
    while (i > 0)
    {
        status_led.on();
        output.println("=========== go " + String(i*5) + "m");
        button_start.waitPressedAndReleased();
        status_led.off();
        lastTime = millis();
        spool.start(i*5*100);
        output.println(millis()-lastTime);
        i--;
    }

    status_led.on();
    output.println("=========== go origin");
    button_start.waitPressedAndReleased();
    status_led.off();
    lastTime = millis();
    spool.start(-1);
    output.println(millis()-lastTime);

    green_led.on();
    output.println("END OF TEST 1 20 meters");
    button_start.waitPressedAndReleased();
    green_led.off();
}

void test_1_depth_40m()
{
    output.println("TEST 1 - ALTIMETRE 40 indirect");
    uint32_t lastTime = 0;

    spool.start_origin();
    status_led.on();
    output.println("=========== go 20m");
    button_start.waitPressedAndReleased();
    status_led.off();
    lastTime = millis();
    spool.start(2000);
    output.println(millis()-lastTime);

    int i = 5;
    while (i < 10)
    {
        status_led.on();
        output.println("=========== go " + String(i*5) + "m");
        button_start.waitPressedAndReleased();
        status_led.off();
        lastTime = millis();
        spool.start(i*5*100);
        output.println(millis()-lastTime);
        i++;
    }

    i = 9;
    while (i > 3)
    {
        status_led.on();
        output.println("=========== go " + String(i*5) + "m");
        button_start.waitPressedAndReleased();
        status_led.off();
        lastTime = millis();
        spool.start(i*5*100);
        output.println(millis()-lastTime);
        i--;
    }

    status_led.on();
    output.println("=========== go origin");
    button_start.waitPressedAndReleased();
    status_led.off();
    lastTime = millis();
    spool.start(-1);
    output.println(millis()-lastTime);

    green_led.on();
    output.println("END OF TEST 1 40 meters");
    button_start.waitPressedAndReleased();
    green_led.off();
}

void test_1_depth_40m_direct()
{
    output.println("TEST 1 - ALTIMETRE 19m direct");
    uint32_t lastTime = 0;

    spool.start_origin();
    status_led.on();
    output.println("=========== go 19m");
    button_start.waitPressedAndReleased();
    status_led.off();
    lastTime = millis();
    spool.start(19000);
    output.println(millis()-lastTime);

    status_led.on();
    output.println("=========== go origin");
    button_start.waitPressedAndReleased();
    status_led.off();
    lastTime = millis();
    spool.start(-1);
    output.println(millis()-lastTime);

    green_led.on();
    output.println("END OF TEST 1 19 meters direct");
    button_start.waitPressedAndReleased();
    green_led.off();
}

void test_2_remplissage_container_1m(){

    output.println("TEST 2 - container 1m");

    spool.start(100);
    valve_23.set_I_way();
    delay(200);
    pump.set_power(255);
    timerStart(timer_control_pressure2);
    pump.start();
    while(button_container.getState() == 1);
    pump.stop();
    timerStop(timer_control_pressure2);
    valve_23.set_L_way();

    step_purge();
    step_rewind();

}

void test_2_remplissage_container_40m(){

    output.println("TEST 2 - container 40m");

    spool.start(40000);
    valve_23.set_I_way();
    delay(200);
    pump.set_power(100);
    timerStart(timer_control_pressure2);
    pump.start();
    while(button_container.getState() == 1);
    pump.stop();
    timerStop(timer_control_pressure2);
    valve_23.set_L_way();

    step_purge();
    step_rewind();
}

void test_3_sterivex_1(){

    output.println("TEST 3 - sterivex 1");

    spool.start(1000);
    for(int i = 0; i<3; i++){
        output.println("Fill container number " + String(i+1));
        step_fill_container();
        output.println("Purge number " + String(i+1));
        step_purge();
    }

    output.println("Fill container number 4");
    step_fill_container();

    // sampling sterivex 1
    uint32_t lastTime = millis();
    step_sampling(1);
    output.println("Temps " + String(millis()-lastTime));

    output.println("Control manuel pompe");
    status_led.on();
    button_start.waitPressedAndReleased();
    status_led.off();

    // mnaual contorl
    int pot_last_value = potentiometer.get_value(0, 100);
    int pot_value = 0;
    int speedy = 60;
    while (!button_start.isPressed())
    {
        pot_value = potentiometer.get_value(0, 100);
        if (pot_value <= pot_last_value - 4 || pot_value >= pot_last_value + 4)
        {
            speedy = pot_value;
            pot_last_value = pot_value;
            output.println("speed " + String(speedy));
        }
        if (button_left.isPressed())
        {
            button_left.waitPressedAndReleased();
            pump.set_power(speedy);
            pump.start();
        }
        if (button_right.isPressed())
        {
            button_right.waitPressedAndReleased();
            pump.set_power(speedy);
            pump.stop();           
        }
        delay(10);
    }
    button_start.waitPressedAndReleased();
    output.println("purge");
    green_led.on();
    button_start.waitPressedAndReleased();
    green_led.off();
    
    step_purge();
    output.println("rewind");
    button_start.waitPressedAndReleased();
    step_rewind();

}

void test_3_sterivex_2(){


    output.println("TEST 3 - sterivex 2");

    spool.start(1000);
    output.println("Fill container number 5");
    step_fill_container();

    // sampling sterivex 2
    uint32_t lastTime = millis();
    step_sampling(2);
    output.println("Temps " + String(millis()-lastTime));

    output.println("Control manuel pompe");
    status_led.on();
    button_start.waitPressedAndReleased();
    status_led.off();

    // mnaual contorl
    int pot_last_value = potentiometer.get_value(0, 100);
    int pot_value = 0;
    int speedy = 60;
    while (!button_start.isPressed())
    {
        pot_value = potentiometer.get_value(0, 100);
        if (pot_value <= pot_last_value - 4 || pot_value >= pot_last_value + 4)
        {
            speedy = pot_value;
            pot_last_value = pot_value;
            output.println("speed " + String(speedy));
        }
        if (button_left.isPressed())
        {
            button_left.waitPressedAndReleased();
            pump.set_power(speedy);
            pump.start();
        }
        if (button_right.isPressed())
        {
            button_right.waitPressedAndReleased();
            pump.set_power(speedy);
            pump.stop();           
        }
        delay(10);
    }
    button_start.waitPressedAndReleased();
    output.println("purge");
    green_led.on();
    button_start.waitPressedAndReleased();
    green_led.off();
    
    step_purge();
    output.println("rewind");
    button_start.waitPressedAndReleased();
    step_rewind();

}

void test_sampling(uint8_t num_sterivex)
{
    // set the valves
    valve_23.set_L_way();
    valve_purge.set_close_way();
    for(uint8_t i = 0; i < MAX_FILTER_NUMBER; i++){
        if(i == num_sterivex){
            valve_stx_in[i].set_open_way();
            valve_stx_out[i].set_I_way();
        }else{
            valve_stx_in[i].set_close_way();
            valve_stx_out[i].set_L_way();
        }
    }
    delay(1000);

    output.println("sample every 10 ms");

    pump.set_power(POWER_STX);

    // start by filling all tubes with water
    pump.start(10000);
    
    pump.start();
    uint32_t time1 = millis();
    uint8_t validation_tick = 0;
    float pressure;
    bool run = true;

    // loop stops after 2.5 seconds under threshold pressure or when time max is reach
    do{
        delay(10); // don't read pressure to fast

        pressure = pressure1.getPressure();

        // adapt pump power to pressure
        if(pressure > 3){
            pump.set_power(pump.get_power()-2);
        }else if(pressure < 2.6f){
            pump.set_power(pump.get_power()+2);
        }

        // if pressure low enough, engage stopping processure
        if(pressure < EMPTY_WATER_PRESSURE_STX_THRESHOLD){
            validation_tick++;
        }else{
            validation_tick = 0;
        }

        output.println(pump.get_power());
        output.println(pressure);
        
        
        // conditions outside while loop to allow printing which condition is responsible for stop
        if(validation_tick >= 250){
            run = false;
            output.println("Purge stopped by low pressure");
        }else if(millis()-time1 > EMPTY_CONTAINER_TIME_FILTER){
            run = false;
            output.println("Purge stopped by security timer");
            // TODO : raise a system warning to user
        }
       
    }while(run);

    output.println("temps à l'arret");
    output.println(millis()-time1);

    // pump a little bit more to flush all water    
    pump.start(EMPTY_WATER_SECURITY_TIME); 

    pump.stop();
    delay(1000);
    valve_stx_in[num_sterivex].set_close_way();
    valve_stx_out[num_sterivex].set_L_way();

    // end with a purge
    step_purge();

    // now that tubes are empty, ensure to have the tube before filter emmpty
    // set the valves
    valve_23.set_L_way();
    valve_purge.set_close_way();
    for(uint8_t i = 0; i < MAX_FILTER_NUMBER; i++){
        if(i == num_sterivex){
            valve_stx_in[i].set_open_way();
            valve_stx_out[i].set_I_way();
        }else{
            valve_stx_in[i].set_close_way();
            valve_stx_out[i].set_L_way();
        }
    }
    delay(1000);
    pump.set_power(100);
    pump.start(EMPTY_WATER_SECURITY_TIME*2); 
    delay(1000);
    valve_stx_in[num_sterivex].set_close_way();
    valve_stx_out[num_sterivex].set_L_way();
}

void test_purge()
{
    // set the valves
    for(uint8_t i = 0; i < MAX_FILTER_NUMBER; i++) valve_stx_in[i].set_close_way();
    valve_23.set_L_way();
    valve_purge.set_open_way();
    delay(1000);

    pump.set_power(100);

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
    do{
        delay(10); // don't read pressure to fast

        pressure = pressure1.getPressure();

        // adapt pump power to pressure to not go over limit
        if(pressure > 3){
            pump.set_power(pump.get_power()-2);
        }else if(pressure < 2.6f){
            pump.set_power(pump.get_power()+2);
        }

        output.println(pump.get_power());
        output.println(pressure);
        
       
        // if pressure low enough, engage stopping processure
        if(pressure1.getPressure() < EMPTY_WATER_PRESSURE_PURGE_THRESHOLD){
            validation_tick++;   
        }else{
            validation_tick = 0;
        }

        // conditions outside while loop to allow printing which condition is responsible for stop
        if(validation_tick >= 250){
            run = false;
            output.println("Purge stopped by low pressure");
        }else if(millis()-time1 > EMPTY_CONTAINER_TIME_PURGE){
            run = false;
            output.println("Purge stopped by security timer");
            // TODO : raise a system warning to user
        }
        
    }while(run);
    
    output.println("temps à l'arret");
    output.println(millis()-time1);

    // pump a little bit more to flush all water
    pump.start(EMPTY_WATER_SECURITY_TIME);
    pump.stop();
    delay(1000);
    valve_purge.set_close_way();
}



void test_characteristic_new_pump()
{

    output.println("========== Adjust height ===============");
    output.println("Press start button to start pumping =========================");
    output.println("Press left button to move spool up ==========================");
    output.println("Press right button to move spool down =======================");
    output.println("Press reset button on due button to come back here ==========");
    output.flush();

    uint8_t speedy = 60;
    while (!button_start.isPressed())
    {
        if (button_left.isPressed())
        {
            spool.set_speed(speedy, up);
            spool.start();
            while (button_left.isPressed())
                delay(5);
            spool.stop();
        }
        if (button_right.isPressed())
        {
            spool.set_speed(speedy, down);
            spool.start();
            while (button_right.isPressed())
                delay(5);
            spool.stop();
        }
        delay(10);
    }

    valve_1.set_close_way();
    valve_23.set_I_way();
    delay(1000);

    uint32_t time1 = millis();
    bool run = true;

    pump.set_power(POWER_PUMP);
    pump.start();
    int compteur=0;
    float pressure=0;
    // two possibilities to stop filling : switch or time
    do
    {
        delay(10); // don't read pressure to fast
        pressure = pressure1.getPressure();
        if (compteur==100)
        {
            output.println(String(pressure));
            compteur=0;
        }

        if (button_start.isPressed())
        {
            run = false;
            output.println("Fill container stopped by button");
        }

        else if (millis() - time1 > FILL_CONTAINER_TIME)
        {
            run = false;
            output.println("Fill container stopped by security timer");
        }
        compteur++;
    } while (run); // conditions are ouside loop to print what condition is responible for stopping

    pump.stop();
    output.println("Time to fill container : " + String(millis() - time1) + " ms");

    delay(1000);

    // set the valves
    uint8_t num_filter = 1;
    valve_23.set_L_way();
    valve_purge.set_close_way();
    for (uint8_t i = 0; i < MAX_FILTER_NUMBER; i++)
    {
        if (i == num_filter)
        {
            valve_stx_in[i].set_open_way();
            valve_stx_out[i].set_I_way();
        }
        else
        {
            valve_stx_in[i].set_close_way();
            valve_stx_out[i].set_L_way();
        }
    }

    delay(1000); // delay to make sure the valves are sets
    const uint8_t power_stx = 50;    
    pump.set_power(power_stx);

    output.println("Prepare water to fill up the tubes and press start");
    output.flush();
     while (!button_start.isPressed())
    {
        delay(1000);
    }
    
    // start by filling all tubes with water to have enough pressure (sensor)
    pump.start(FILL_TUBES_WITH_WATER_TIME);

    output.println("System ready");
    output.println("Fill 1 liter of water and press start to start pumping");
    output.flush();
     while (!button_start.isPressed())
    {
        delay(1000);
    }
    pump.start(FILL_TUBES_WITH_WATER_TIME); 
    pump.start();
    time1 = millis();
    uint8_t validation_tick = 0;
    run = true;
    compteur=0;
    float Pout_max=3.0f;
    float Pout_min =2.7f;
    float empty_water_pressure_stx_threshold = 0.1f;

    // loop stops after 2.5 seconds under pressure threshold or when time max is reached
    do
    {
        
        delay(10); // don't read pressure to fast
        pressure = pressure1.getPressure();
        
        if (compteur==50)
        {
            output.println(String(pressure)+String(pump.get_power()));
            compteur=0;
        }

        // adapt pump power to pressure to not go over limit of 3 bar
        // simple incremental controler. Not the best one but does the job
        if (pressure > Pout_max)
            pump.set_power(pump.get_power() - 2);
        else if (pressure < Pout_min)
            pump.set_power(pump.get_power() + 2);

        // if pressure low enough, engage stopping processure
        if (pressure < empty_water_pressure_stx_threshold)
            validation_tick++;
        else
            validation_tick = 0;

        if (validation_tick >= 250)
        {
            run = false;
            output.println("Purge stopped by low pressure");
        }

        compteur++;
    } while (run); // conditions outside while loop to allow printing which condition is responsible for stop
    
    // pump a little bit more to flush all water
    //pump.start(EMPTY_WATER_STX_SECURITY_TIME);

    output.println("The system stop after:");
    output.println(String(millis()-time1-250));
    //output.flush();

    
}

void test_control_loop_Kp()
{
    int compteur=0;
    float pressure=0;
    uint32_t time1 = millis();
    bool run = true;
    // set the valves
    uint8_t num_filter = 1;
    valve_23.set_L_way();
    valve_purge.set_close_way();
    for (uint8_t i = 0; i < MAX_FILTER_NUMBER; i++)
    {
        if (i == num_filter)
        {
            valve_stx_in[i].set_open_way();
            valve_stx_out[i].set_I_way();
        }
        else
        {
            valve_stx_in[i].set_close_way();
            valve_stx_out[i].set_L_way();
        }
    }

    delay(1000); // delay to make sure the valves are sets
    const uint8_t power_stx = 50;    
    pump.set_power(power_stx);

    output.println("Prepare water to fill up the tubes and press start");
    output.flush();
     while (!button_start.isPressed())
    {
        delay(1000);
    }
    
    // start by filling all tubes with water to have enough pressure (sensor)
    pump.start(FILL_TUBES_WITH_WATER_TIME);

    output.println("System ready");
    output.println("Fill 1 liter of water and press start to start pumping");
    output.flush();
     while (!button_start.isPressed())
    {
        delay(1000);
    }

    pump.start(FILL_TUBES_WITH_WATER_TIME); 
    pump.start();
    time1 = millis();
    uint8_t validation_tick = 0;
    run = true;
    compteur=0;
    float Pout_target=3;
    float empty_water_pressure_stx_threshold = 0.1f;
    float error = 0;
    float gain = 0;
    float offset = 0;
    float Kp = 90;

    // loop stops after 2.5 seconds under pressure threshold or when time max is reached
    do
    {
        
        delay(10); // don't read pressure to fast
        pressure = pressure1.getPressure();
        
        if (compteur==50)
        {
            output.println("Pressure = " + String(pressure)+ ",   pump power = " +String(pump.get_power()));
            compteur=0;
        }

        // adapt pump power to pressure to not go over limit of 3 bar
        error=Pout_target-pressure;
        gain=error*Kp + offset;

        if (gain > 100)
        {
            gain=100;
        }
        else if (gain < 40)
        {
            gain=0;
        }
        pump.set_power(gain);

        // if pressure low enough, engage stopping processure
        if (pressure < empty_water_pressure_stx_threshold)
            validation_tick++;
        else
            validation_tick = 0;

        if (validation_tick >= 250)
        {
            run = false;
            output.println("Purge stopped by low pressure");
        }

        compteur++;
    } while (run); // conditions outside while loop to allow printing which condition is responsible for stop
    
    // pump a little bit more to flush all water
    pump.start(EMPTY_WATER_STX_SECURITY_TIME);

    output.println("The system stop after:");
    output.println(String(millis()-time1-250));
    //output.flush();

}

void test_flux_pompe()
{
    // set the valves
    uint8_t num_filter = 1;
    valve_23.set_L_way();
    valve_purge.set_close_way();
    for (uint8_t i = 0; i < MAX_FILTER_NUMBER; i++)
    {
        if (i == num_filter)
        {
            valve_stx_in[i].set_open_way();
            valve_stx_out[i].set_I_way();
        }
        else
        {
            valve_stx_in[i].set_close_way();
            valve_stx_out[i].set_L_way();
        }
    }
    delay(1000); // delay to make sure the valves are sets

    const uint8_t power_stx = 100;  
    int compteur=0;
    float pressure=0;  

    pump.set_power(power_stx);
    pump.start();
    output.println("Press button to stop");
     while (!button_start.isPressed())
    {
        delay(10); // don't read pressure to fast
        pressure = pressure1.getPressure();

        if (compteur==50)
        {
            output.println("Pressure = " + String(pressure)+ ",   pump power = " +String(pump.get_power()));
            compteur=0;
        }
        compteur++;
    }
    pump.stop();
}

void test_vanes()
{
    output.println("Valve 1 checkout start");
      while (!button_start.isPressed())
    {
        delay(1000);
    }
    uint8_t num_filter = 0;
    valve_23.set_L_way();
    valve_purge.set_close_way();
    for (uint8_t i = 0; i < MAX_FILTER_NUMBER; i++)
    {
        if (i == num_filter)
        {
            valve_stx_in[i].set_open_way();
            valve_stx_out[i].set_I_way();
        }
        else
        {
            valve_stx_in[i].set_close_way();
            valve_stx_out[i].set_L_way();
        }
    }

    delay(1000); // delay to make sure the valves are sets

    output.println("Valve 2 checkout start");
      while (!button_start.isPressed())
    {
        delay(1000);
    }
    num_filter = 1;
    valve_23.set_L_way();
    valve_purge.set_close_way();
    for (uint8_t i = 0; i < MAX_FILTER_NUMBER; i++)
    {
        if (i == num_filter)
        {
            valve_stx_in[i].set_open_way();
            valve_stx_out[i].set_I_way();
        }
        else
        {
            valve_stx_in[i].set_close_way();
            valve_stx_out[i].set_L_way();
        }
    }
    delay(1000); // delay to make sure the valves are sets

     output.println("Valve I checkout start");
      while (!button_start.isPressed())
    {
        delay(1000);
    }
    num_filter = 1;
    valve_23.set_I_way();

    output.println("STOP checks");
      while (!button_start.isPressed())
    {
        delay(1000);
    }

}


void test_demonstration()
{

     static bool one_time_code_idle_mode = true;
     static bool auto_demo = true;

    // ========= ONE TIME CODE IN IDLE ============
    if (one_time_code_idle_mode)
    {
        // output.println("Next sample scheduled :");
        display_sample(0);
        one_time_code_idle_mode = false;
        // TODO : send this information to server
    }

    // ======== FILTER REFILLING COMMAND ==========
    // Manual command. Potentiometer on the left and button left pressed
    // TODO : refill validation through server
    if ((potentiometer.get_value() < 50 && button_left.isPressed()) || auto_demo){
        // choose number of filter inserted. For now always 2.
        reload_filters(2);
        // output.println("Filter refilled");
    }

    // ============== SAMPLING STEPS ==============
    if (now() > get_next_sample_time() - PREPARATION_TIME || button_start.isPressed() || auto_demo)
    {
        // if there is still new filters in the system
        if (is_filter_available())
        {
            uint32_t time_sampling = millis();


            set_system_state(state_sampling);

            // get first sample in list
            Sample sample = get_sample(0);

            output.println("Start sampling");
            output.println("Sample started at depth " + String(sample.get_depth()) + "cm in filter " + String(get_next_filter_place()));

            // Sampling steps
            step_dive(sample.get_depth());
            for(uint8_t i = 0; i < PURGE_NUMBER; i++){
                step_fill_container();
                step_purge();
            }

            step_fill_container();

            step_rewind();

            step_sampling(get_next_filter_place()); // sample place is a human number, start at 1
            // step_dry(get_next_sample_place());   // not completely done yet
            
            // changes variables accordingly and log done sample
            validate_sample();

            output.println("Time for complete sample : " + String(millis()-time_sampling) + " ms");

            // Check new filter availability
            if(is_filter_available() == false){
                set_system_state(state_refill);
                output.println("Out of filter stock");
            }else{
                set_system_state(state_idle);
            }
        }
        else
        {
            output.println("ERROR - sample not done - no filter available");
            validate_sample();
        }

        one_time_code_idle_mode = true;
    }
    delay(UPDATE_TIME);
}

void test_pressure_sensor(){
    float pressure;
    bool run = true;
    int compteur=0;
     do
    {
        delay(10); // don't read pressure to fast
        pressure = pressure1.getPressure();
        
        if (compteur==10)
        {
            output.println("Pressure = " + String(pressure));
            compteur=0;
        }

    compteur++;
    } while (run); // conditions outside while loop to allow printing which condition is responsible for stop

}
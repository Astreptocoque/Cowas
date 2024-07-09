/**
 * @file MAIN.CPP
 * @author Timothée Hirt & Paco Mermoud
 * @brief A GenoRobotics project - CoWaS (Continous Water Sampling). 
 *        A project for ARDUINO DUE
 * @version 2.0
 * @date 2022-01-28
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
#include "Micro_pump.h"
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
#include "Tests.h"
#include "Step_functions.h"
#include "GPIO.h"
#include "TimeLib.h"
#include "Manifold.h"
#include "maintenance.h"


// ============ MAIN FUNCTION DECLARATION =======
void system_checkup();
void before_start_program();
void main_program();
void button_control();

void cross_cont_exp();
void exp_explore_2905();

// ============ EXECUTION MODE ===================
// comment this line if no system check at startup
#define  SYSTEM_CHECKUP
// ============ PIN DEFINITIONS ==================
// See in Settings.h

// states to manually control CoWaS with buttons
bool ctrl_button;       // if possible to control CoWaS with buttons
enum ctrl_state {ctrl_pump, ctrl_motor_spool, ctrl_v23, ctrl_v1, ctrl_vman, ctrl_man_slot, ctrl_micro_pump};
uint8_t ctrl_state_len = 7; // to update when modifying lenght of control states
ctrl_state control_state;

// ============= REAL HARDWARE =================
// ====> do not forget to add the object.begin() in setup()
C_output output;                       // custom print function to handle multiple outputs
Led status_led;                             // general purpose LED
Led green_led;                              // general purpose LED, used here for start signals
Trustability_ABP_Gage pressure1;            // see schematics
Valve_2_2 valve_1;                          // see schematics
Valve_3_2 valve_23;                         // see schematics, voltage off: L_way
Valve_2_2 valve_manifold;                   // see schematics
Pump pump;                                  // see schematics
Pump pump_vacuum;                           // see schematics
Micro_Pump micro_pump;                      // For DNA shield
Micro_Pump test_5V_micro_pump;
Motor spool;                                // see schematics
Motor manifold_motor;                       // see schematics
Encoder encoder;                            // see schematics
Button button_start;                        // User button. Normally open
Button button_left;                         // User button. Normally open
Button button_right;                        // User button. Normally open
Button button_container;                    // Control button. Normally closed
Button button_spool_up;                     // Control button. Normally closed
Button button_spool_down;                   // Control button. Normally closed
Potentiometer potentiometer;                // User rotary knob
struct Timer timer_control_pressure1;       // Timer for interrupts with pressure sensor 1
Manifold manifold;                          // Manifold


int manifold_slot; // Armand presentation


void setup()
{

    // ========== SYSTEM INITIALIZATION ============
    SPI.begin();
    output.begin(terminal); // choose output of logs
    timer_control_pressure1 = {TC1, 0, TC3_IRQn, 4};    // timer of 1 second (4)

    // ========== HARDWARE INITIALIZATION ==========
    status_led.begin(STATUS_LED_PIN, "status");
    green_led.begin(GREEN_LED_PIN, "green");
    pressure1.begin(PRESSURE1_PIN, 3, "P1");
    valve_1.begin(VALVE_1_PIN, "V1");
    valve_23.begin(VALVE_23_PIN, "V_23");
    valve_manifold.begin(VALVE_MANIFOLD, "V_manifold");
    pump.begin(PUMP_PIN, true, "P1");
    micro_pump.begin(ON_OFF_33V, "DNA Shield pump");
    test_5V_micro_pump.begin(ON_OFF_5V, "Test ON/OFF 5V pump");
    spool.begin();
    manifold_motor.begin("MANIFOLD");
    encoder.begin(ENCODER_A_PIN, ENCODER_B_PIN, ENCODER_Z_PIN, 720, 10);
    button_left.begin(BUTTON_LEFT_PIN, "B_left");
    button_right.begin(BUTTON_RIGHT_PIN, "B_right");
    button_start.begin(BUTTON_START_PIN, "B_start");
    button_container.begin(BUTTON_CONTAINER_PIN, "B_container");
    button_spool_up.begin(BUTTON_SPOOL_UP, "B_spool_UP");
    attachInterrupt(digitalPinToInterrupt(BUTTON_SPOOL_UP), ISR_emergency_stop_up, FALLING);
    button_spool_down.begin(BUTTON_SPOOL_DOWN, "B_spool_down");
    // doing problem right now, to investigate
    // attachInterrupt(digitalPinToInterrupt(BUTTON_SPOOL_DOWN), ISR_emergency_stop_down, FALLING);
    spool.endstop_up = false;
    spool.endstop_down = false;
    potentiometer.begin(POTENTIOMETER_PIN);
    manifold.begin();
    manifold.change_state(PURGE_SLOT, unaivailable); // The purge has no filter


    output.println("system initalized\n");

    // ======== PRE-STARTING EXECUTION =========
    output.println("========== Press start button to play program ==========================");
    output.println("========== Press left button to move spool up ==========================");
    output.println("========== Press right button to move spool down =======================");
    output.println("========== Press reset button on due button to come back here ==========");
    output.flush();

    green_led.on();
    status_led.on();
#if (DEBUG_MODE_PRINT)
    before_start_program();
    button_start.waitPressedAndReleased();
#endif
    green_led.off();
    status_led.off();

#ifdef SYSTEM_CHECKUP
    #if(DEBUG_MODE_PRINT)
        system_checkup();
        output.println("System checked\n");
    #endif
#endif

    output.println("Programm started\n");
    // System states are command for the server to allow or not some manipulations
    set_system_state(state_idle);

    // Communication with rapsberry
    Serial.begin(9600);

    // For Armand Presentation
    manifold_slot=0;

    ctrl_button = true;

    // ! calling testing function
    // purge_pipes_manifold();
    
    // test_all_components();
    // cross_cont_exp();


    // filling the DNA shield pipe
    // fill_DNA_shield_tube();
    // exp_explore_2905();
}


void loop()
{
    
    // button_control();
    main_program();

}

void main_program()
{
    if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        int depth=0;
        String cmp_temp="sample";
        if (data.startsWith(cmp_temp)){
            if (data.length()==15){
                depth=data.substring(14,15).toInt();
            }
            else if (data.length()==16){
                depth=data.substring(14,16).toInt();
            }
            else{
                Serial.println("Error depth");
            }
            data="sampleFunction";
        }

        if(data == "reloadManifoldFunction"){
            manifold.reload();
        }
        else if(data == "purgeSterivexFunction"){
            purge_sterivex(PURGE_SLOT);
        }
        else if(data == "purgeContainerFunction"){
            step_purge();
        }
        else if(data == "purgePipesFunction"){
            purge_Pipes();
        }
        else if(data == "fillContainerFunction"){
            step_fill_container();
        }
        else if(data == "sampleFunction"){
            depth*=100; // convert m -> cm
            sample_process(depth); 
        }
        else if(data == "rollingFunctionUp"){
            uint8_t speedy = 60;
            spool.set_speed(speedy, up);
            spool.start();
        }
        else if(data == "rollingFunctionDown"){
            uint8_t speedy = 60;
            spool.set_speed(speedy, down);
            spool.start();
        }
        else if(data == "stopRolling"){
            spool.stop();
        }
        else if(data == "startDNA"){
            micro_pump.start();
        }
        else if(data == "stopDNA"){
            micro_pump.stop();
        }

        Serial.print(" - function accomplished - ");
        Serial.println(data);
        Serial.flush();
    }
}

void before_start_program()
{
    // Enable to move spool manualy with left and right buttons
    // Tune speed with potentiometer

    uint8_t pot_last_value = potentiometer.get_value(0, 100);
    uint8_t pot_value = 0;
    uint8_t speedy = 60;
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
}

#ifdef SYSTEM_CHECKUP
void system_checkup()
{
    // check if sensor are operationnal
    bool error = false;

    // check spool switch 1
    spool.start(20, down);
    delay(200);
    spool.stop();
    if (button_spool_up.getState() == 1){
        output.println("CHECK | Button spool working");
        spool.start_origin();
    }else
    {
        output.println("ERROR | Button spool not working");
        error = true;
    }

    // check container switch
    if (button_container.getState() == 1)
        output.println("CHECK | Button container working");
    else{
        output.println("ERROR | Button container not working");
        error = true;
    }

    // check spool down switch
    if (button_spool_down.getState() == 1)
        output.println("CHECK | Button spool down working");
    else{
        output.println("ERROR | Button spool down not working");
        error = true;
    }

    //check temperature
    // flush first time reading otherwise error (no idea why)
    pressure1.getTemperature();
    delay(10);
    if (pressure1.getTemperature() > 0)
    {
        output.println("CHECK | Temperature okay (" + String(pressure1.getTemperature()) + ")");
    }
    else
    {
        output.println("ERROR | Temperature too low (" + String(pressure1.getTemperature()) + ")");
        error = true;
    }

    if (error)
    {
        output.println("FATAL ERROR AT STARTUP ");
        set_system_state(state_error);
        while (true)
            delay(500);
    }
}


/*
    For valves: left close, right open
    Valves 23: left I way, right L way
    Motor spool: left up, right down, push and hold
    manifold: left, decrement slot and right increment
    Pump: right start, left stop
*/
void button_control(){
    static uint8_t current_slot = 0;
    if (!ctrl_button){
        return;
    }

    switch (control_state){
        case ctrl_pump:{
            if (button_right.isPressed()) {
                pump.set_power(80);
                pump.start();
                button_right.waitPressedAndReleased();
            }
            if (button_left.isPressed()){
                pump.stop();
                button_left.waitPressedAndReleased();
            } 
            break;
        }  
        case ctrl_v23:{
            if (button_left.isPressed()){
                valve_23.set_L_way();
                button_left.waitPressedAndReleased();
            }
            if (button_right.isPressed()){
                valve_23.set_I_way();
                button_right.waitPressedAndReleased();
            }
            break;
        }
        case ctrl_v1: {
            if (button_left.isPressed()){
                valve_1.set_close_way();
                button_left.waitPressedAndReleased();
            }
            if (button_right.isPressed()){
                valve_1.set_open_way();
                button_right.waitPressedAndReleased();
            }
            break;
        }
        case ctrl_vman: {
            if (button_left.isPressed()){
                valve_manifold.set_close_way();
                button_left.waitPressedAndReleased();
            }
            if (button_right.isPressed()){
                valve_manifold.set_open_way();
                button_right.waitPressedAndReleased();
            }
            break;
        }
        case ctrl_man_slot: {
            if (button_right.isPressed()){
                current_slot++;
                current_slot = current_slot % 15;
                Serial.println(current_slot);
                button_right.waitPressedAndReleased();
                rotateMotor(current_slot);
            }
            if (button_left.isPressed()){
                current_slot--;
                if (current_slot > 14) current_slot = 14;
                Serial.println(current_slot);
                button_left.waitPressedAndReleased();
                rotateMotor(current_slot);
            }
            current_slot = current_slot % 15;
            break;
        }
        case ctrl_motor_spool: {
            if (button_right.isPressed()){
                spool.set_speed(50, down);
                spool.start();
                button_right.waitPressedAndReleased();
                spool.stop();
            }
            if (button_left.isPressed()){
                spool.set_speed(100, up);
                spool.start();
                button_left.waitPressedAndReleased();
                spool.stop();
            }
            break;
        }
        case ctrl_micro_pump: {
            if (button_right.isPressed()) {
                micro_pump.start();
                button_right.waitPressedAndReleased();
            }
            if (button_left.isPressed()){
                micro_pump.stop();
                button_left.waitPressedAndReleased();
            } 
            break;
        }
        default: break;
    }

    // check if going onto next mode
    if (button_start.isPressed()){
        control_state = static_cast<ctrl_state>((static_cast<uint8_t>(control_state) + 1) % ctrl_state_len);
        button_start.waitPressedAndReleased();
        Serial.print("New control state : ");
        Serial.println(control_state);
    }
}

void cross_cont_exp(){
    output.println("Cross contamination test started");
    output.println("");
    output.println("Reuse the purged water in recipient, to filter and reuse later");
    output.println("When pipe is destilled water, press start");

    // using 4 liter of distilled water

    button_start.waitPressedAndReleased();

    // sample 1, distilled water
    // !!!!!!!! -----------------
    step_fill_container();
    step_purge(false);
    // !! ------------


    output.println("REUSE filtered water in 5L container");
    output.println("When pipe is in NEW destilled water, press start to start SAMPLE N°1");

    button_start.waitPressedAndReleased();

    step_fill_container();
    step_sampling(1, false);

    // sample 2, no need to rinse as sample before was with destilled water
    output.println("Put filtered water in seperate recipient to discard colony later"); // !
    output.println("When pipe is in bacterial colony, press start for SAMPLING N°2");

    button_start.waitPressedAndReleased();
    
    step_fill_container();
    step_sampling(2, false);

    output.println("");
    output.println("Discard the water with the bacterial colony");
    

    output.println("Reuse water in a recipient");
    output.println("When pipe is in distilled water container 5L (2L, filtered from sample 1), press start");
    button_start.waitPressedAndReleased();

    // rinsing bacterial colony, before sample 3

    step_fill_container();
    delay(3000);
    step_purge(false);

    output.println("Filter the 4L of water (from last purge and first purge) through a serivex into the 5L container");
    output.println("When filtered, put pipe into the 5L container (containing 4L of distilled water)");
    output.println("Press start button when ready");

    button_start.waitPressedAndReleased();

    for(uint8_t i = 0; i < 2; i++){
        step_fill_container();
        delay(3000);
        step_purge(false);
    }



    output.println("Purge finished");
    output.println("When pipe is in NEW (the last 2L) distilled water, press start for SAMPLE N°3");
    button_start.waitPressedAndReleased();

    step_fill_container();
    step_sampling(3, false);
}


void exp_explore_2905(){
    output.println("Start of experiment - L'EXPLORE");
    output.println("--------------------------------------------");

    // output.println("Start of sample 1 in slot 1");
    // output.println("Depth : 39m");
    // output.println("Press start when ready");
    // button_start.waitPressedAndReleased();
    // sample_process(39*100, 1);

    output.println("Start of sample 2 in slot 4");
    output.println("Depth : 5m");
    output.println("Press start when ready");
    button_start.waitPressedAndReleased();
    sample_process(5*100, 4);

    output.println("Start of sample 3 in slot 8");
    output.println("Depth : 5m");
    output.println("Press start when ready");
    button_start.waitPressedAndReleased();
    sample_process(5*100, 8);

    output.println("Start of sample 4 in slot 11");
    output.println("Depth : 5m");
    output.println("Press start when ready");
    button_start.waitPressedAndReleased();
    sample_process(5*100, 11);

    output.println("Start of sample 5 in slot 14");
    output.println("Depth : 3m");
    output.println("Press start when ready");
    button_start.waitPressedAndReleased();
    sample_process(3*100, 14);

    output.println("Well done, get a drink and enjoy the succesfull experiene");
    output.println("------------------------------------------------------------");
}

#endif

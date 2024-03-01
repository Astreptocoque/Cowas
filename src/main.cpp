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


// ============ MAIN FUNCTION DECLARATION =======
void system_checkup();
void before_start_program();
void main_program();

// ============ EXECUTION MODE ===================
// comment this line if no system check at startup
#define  SYSTEM_CHECKUP
#define  SYSTEM_CHECKUP
// ============ PIN DEFINITIONS ==================
// See in Settings.h

// ============= REAL HARDWARE =================
// ====> do not forget to add the object.begin() in setup()
C_output output;                       // custom print function to handle multiple outputs
Led status_led;                             // general purpose LED
Led green_led;                              // general purpose LED, used here for start signals
Trustability_ABP_Gage pressure1;            // see schematics
Valve_2_2 valve_1;                          // see schematics
Valve_3_2 valve_23;                         // see schematics
Valve_2_2 valve_manifold;                   // see schematics
Pump pump;                                  // see schematics
Pump pump_vacuum;                           // see schematics
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

    // ! calling testing function
    test_all_components();
}


void loop()
{

    add_button_manifold_demo();
    main_program();

    // TESTS 1
    // test_1_depth_20m();
    // test_1_depth_40m();
    // test_1_depth_40m_direct();

    // TESTS 2
    // test_2_remplissage_container_1m();
    // test_2_remplissage_container_40m();

    // TESTS 3
    // test_3_sterivex_1();

    // TESTS SYSTEM
    // test_pressure_sensor();
    // test_manifold();

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
#endif

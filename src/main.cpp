/**
 * @file MAIN.CPP
 * @author Timothée Hirt
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
#include "Sample.h"
#include "Settings.h"
#include "Critical_error.h"
#include "Timer.h"
#include "Serial_device.h"
#include "Tests.h"
#include "Step_functions.h"
#include "GPIO.h"
#include "TimeLib.h"

// ============ MAIN FUNCTION DECLARATION =======
void system_checkup();
void before_start_program();
void main_program();

// ============ EXECUTION MODE ===================
// comment this line if no system check at startup
#define  SYSTEM_CHECKUP

// ============ PIN DEFINITIONS ==================
// See in Settings.h

// ============= REAL HARDWARE =================
// ====> do not forget to add the object.begin() in setup()
C_output output;                       // custom print function to handle multiple outputs
Serial_device esp8266;                      // enable communication with wifi card
Led status_led;                             // general purpose LED
Led green_led;                              // general purpose LED, used here for start signals
Trustability_ABP_Gage pressure1;            // see schematics
Trustability_ABP_Gage pressure2;            // see schematics
Valve_2_2 valve_1;                          // see schematics
Valve_3_2 valve_23;                         // see schematics
Valve_2_2 valve_purge;                      // see schematics
Valve_2_2 valve_stx_in[MAX_FILTER_NUMBER];  // see schematics
Valve_3_2 valve_stx_out[MAX_FILTER_NUMBER]; // see schematics
Pump pump;                                  // see schematics
Pump pump_vacuum;                           // see schematics
Motor spool;                                // see schematics
Encoder encoder;                            // see schematics
Button button_start;                        // User button. Normally open
Button button_left;                         // User button. Normally open
Button button_right;                        // User button. Normally open
Button button_container;                    // Control button. Normally closed
Button button_spool_up;                     // Control button. Normally closed
Button button_spool_down;                   // Control button. Normally closed
Potentiometer potentiometer;                // User rotary knob
struct Timer timer_control_pressure1;       // Timer for interrupts with pressure sensor 1
struct Timer timer_control_pressure2;       // Timer for interrupts with pressure sensor 2
GPIO wifi_message;                          // Input for message line report from wifi card

void setup()
{

    // ========== SYSTEM INITIALIZATION ============
    SPI.begin();
    output.begin(terminal); // choose output of logs
    esp8266.begin();        // connect to wifi card

    timer_control_pressure1 = {TC1, 0, TC3_IRQn, 4};    // timer of 1 second (4)
    timer_control_pressure2 = {TC1, 1, TC4_IRQn, 4};    // timer of 1 second (4)

    // ========== HARDWARE INITIALIZATION ==========
    status_led.begin(STATUS_LED_PIN, "status");
    green_led.begin(GREEN_LED_PIN, "green");
    pressure1.begin(PRESSURE1_PIN, 3, "P1");
    pressure2.begin(PRESSURE2_PIN, 3, "P2");
    valve_1.begin(VALVE_1_PIN, "V1");
    valve_23.begin(VALVE_23_PIN, "V_23");
    valve_purge.begin(VALVE_PURGE, "V_purge");
    for (uint8_t i = 0; i < MAX_FILTER_NUMBER; i++)
    {
        valve_stx_in[i].begin(VALVE_STX_IN_PIN[i], "Vstx_in_" + String(i));
        valve_stx_out[i].begin(VALVE_STX_OUT_PIN[i], "Vstx_out_" + String(i));
    }
    pump.begin(PUMP_PIN, true, "P1");
    pump_vacuum.begin(PUMP_VACUUM, false, "Pvac");
    spool.begin();
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
    wifi_message.begin(ESP8266_COMM_PIN, INPUT);

    // ========= DATE FROM WIFI CARD ============
    output.println("Get date");
    struct Date current_date;
    esp8266.start_communication();
    current_date = esp8266.receive_time();
    setTime(current_date.epoch);
    // manual time settup
    setTime(timeToEpoch(16, 00, 22, 01, 2022));
    output.println("It is " + format_date_friendly(now()));

    //  Test samples intialized
    add_sample(16, 35, 25, 01, 2022, 5, 2);
    add_sample(8, 00, 26, 01, 2022, 600, 1);

    display_samples();


    output.println("system initalized\n");

    // ======== PRE-STARTING EXECUTION =========
    output.println("========== Press start button to play program ==========================");
    output.println("========== Press left button to move spool up ==========================");
    output.println("========== Press right button to move spool down =======================");
    output.println("========== Press reset button on due button to come back here ==========");
    output.flush();

    green_led.on();
    status_led.on();
    before_start_program();
    button_start.waitPressedAndReleased();
    green_led.off();
    status_led.off();

#ifdef SYSTEM_CHECKUP
    system_checkup();
    output.println("System checked\n");
#endif

    output.println("Programm started\n");
    // System states are command for the server to allow or not some manipulations
    set_system_state(state_idle);
}

void loop()
{
    main_program();
    
    // test_hardware_general();

    // TESTS 1
    // test_1_depth_20m();
    // test_1_depth_40m();
    // test_1_depth_40m_direct();

    // TESTS 2
    // test_2_remplissage_container_1m();
    // test_2_remplissage_container_40m();

    // TESTS 3
    // test_3_sterivex_1();

}

void main_program()
{
    static bool one_time_code_idle_mode = true;

    // =============== IDLE CODE ==================    
    // TODO : Wi-Fi and communication stuffs


    // ========= ONE TIME CODE IN IDLE ============
    if (one_time_code_idle_mode)
    {
        output.println("Next sample scheduled :");
        display_sample(0);
        one_time_code_idle_mode = false;
        // TODO : send this information to server
    }

    // ======== FILTER REFILLING COMMAND ==========
    // Manual command. Potentiometer on the left and button left pressed
    // TODO : refill validation through server
    if (potentiometer.get_value() < 50 && button_left.isPressed()){
        // choose number of filter inserted. For now always 2.
        reload_filters(2);
        output.println("Filter refilled");
    }

    // ============== SAMPLING STEPS ==============
    if (now() > get_next_sample_time() - PREPARATION_TIME)
    {
        // if there is still new filters in the system
        if (is_filter_available())
        {
            uint32_t time_sampling = millis();

            set_system_state(state_sampling);

            // get first sample in list
            Sample sample = get_sample(0);

            output.println("It's sampling time !");
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

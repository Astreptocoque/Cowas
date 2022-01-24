/**
 * @file MAIN.CPP
 * @author Timothée Hirt & Christophe Deloose & Elio Sanchez
 * @brief A GenoRobotics project - CoWaS (Continous Water Sampling). 
 *        ARDUINO DUE !!!
 * @version 2.0
 * @date 2021-10-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// #include <list>                     // include before arduino.h, otherwise error
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
#define  SYSTEM_CHECKUP

// ============ PIN DEFINITIONS ==================
// See in settings

// ============= REAL HARDWARE =================
// ====> do not forget to add the object.begin() in setup()
Serial_output output;                       // custom print function to handle multiple outputs
Serial_device esp8266;                      // enable communication with wifi card
Led status_led;                             // general purpose LED
Led green_led;                              // general purpose LED, used for start signals
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
Button button_container;                    // Control button. Normally closed
Button button_spool_up;                     // Control button. Normally closed
Button button_spool_down;                   // Control button. Normally closed
Button button_left;                         // User button. Normally open
Button button_right;                        // User button. Normally open
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

    timer_control_pressure1 = {TC1, 0, TC3_IRQn, 4};
    timer_control_pressure2 = {TC1, 1, TC4_IRQn, 4};

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
    attachInterrupt(digitalPinToInterrupt(BUTTON_SPOOL_DOWN), ISR_emergency_stop_down, FALLING);
    spool.endstop_up = false;
    spool.endstop_down = false;
    potentiometer.begin(POTENTIOMETER_PIN);
    wifi_message.begin(ESP8266_COMM_PIN, INPUT);

    output.println("system initalized\n");

    // ======== PRE-STARTING EXECUTION =========
    output.println("========== Press start button to play program ==========================");
    output.println("========== Press left button to move spool up ==========================");
    output.println("========== Press right button to move spool down =======================");
    output.println("========== Press reset button on due button to come back here ==========");
    output.flush();

    output.println("Get date");
    struct Date current_date;
    esp8266.start_communication();
    current_date = esp8266.receive_time();
    setTime(current_date.epoch);
    // setSyncInterval(SYNC_TIME);
    // setSyncProvider(esp8266.receive_time());

    // manual time settup
    // setTime(timeToEpoch(16, 00, 22, 01, 2022));
    output.println("It is " + format_date_friendly(now()));
    output.flush();

    //add to test samples
    add_sample(23, 30, 23, 01, 2022, 10, 2);
    add_sample(17, 05, 24, 01, 2022, 600, 1);
    // add_sample(16, 00, 22, 01, 2022, 20, 0);
    // add_sample(19, 30, 12, 1, 2022, 10, 4);
    // add_sample(15, 30, 13, 1, 2022, 40, 4);
    // add_sample(18, 03, 10, 1, 2022, 40, 5);
    // add_sample(18, 30, 12, 1, 2022, 40, 0);
    display_samples();

    green_led.on();
    status_led.on();
    output.println("Before start program");
    before_start_program();
    button_start.waitPressedAndReleased();
    green_led.off();
    status_led.off();

#ifdef SYSTEM_CHECKUP
    system_checkup();
    output.println("System checked\n");
#endif
    output.println("Programm started\n");
    set_system_state(state_idle);
}

void loop()
{

    output.println(pressure1.getPressure());
    output.println(pressure1.getTemperature());
    delay(500);

    // test_purge();
    // test_sampling(0);
    // step_purge();
    // step_sampling(0);
    // button_start.waitPressedAndReleased();
    // main_program();
    // step_dive(30);
    // step_fill_container();
    // step_purge();
    // step_sampling(1);
    // // button_start.waitPressedAndReleased();
    // test_hardware_general();
    // output.println("fini");
    // button_left.waitPressedAndReleased();

    // TESTS 1
    // test_1_depth_20m();
    // test_1_depth_40m();
    // test_1_depth_40m_direct();

    // TESTS 2
    // test_2_remplissage_container_1m();
    // test_2_remplissage_container_40m();

    // TESTS 3
    // test_3_sterivex_1();
    // test_pressure();
    //   main_program();
    //   test_depth();
    //   tests();
}

void main_program()
{
    static bool next_sample_information = true;
    // DO wifi and communication stuffs

    // one time print of the next sample informatin for logging
    if (next_sample_information)
    {
        output.println("Next sample scheduled :");
        display_sample(0);
        output.println("");
        next_sample_information = false;
    }

    // check if filter are refilled. Potentiometer on the left and button left pressed
    // if (potentiometer.get_value() < 50 && button_left.isPressed()){
    //     // choose number of filter inserted. For now always 2.
    //     reload_filters(2);
    // }

    // when time occurs for a sample to be done
    if (now() > get_next_sample_time() - PREPARATION_TIME)
    {
        // if a filter is available to be used
        if (is_filter_available())
        {
            // for human interface
            set_system_state(state_sampling);

            output.println("It's sampling time !");

            // get first sample in list
            Sample sample = get_sample(0);
            // COWAS sampling
            step_dive(sample.get_depth());
            // for(uint8_t i = 0; i < PURGE_NUMBER; i++){
            //     step_fill_container();
            //     step_purge();
            // }
            // step_fill_container();
            step_rewind();
            // step_sampling(get_next_sample_place()-1); // sample place is a human number, start at 1
            step_sampling(0); // sample place is a human number, start at 1
            // display_sample(get_next_sample_place()-1);
            
            // step_dry(get_next_sample_place()-1);
            validate_sample();
            
            if(is_filter_available() == false){
                set_system_state(state_refill);
                output.println("No more filter in stock");
            }else{
                set_system_state(state_idle);
                next_sample_information = true;
            }
        }
        else
        {
            output.println("ERROR - sample not done - no filter available");
            validate_sample();
            next_sample_information = true;
        }
    }
    delay(UPDATE_TIME);
}

void before_start_program()
{
    // move the motor before starting

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
    if (button_spool_up.getState() == 1)
        output.println("CHECK | Button spool working");
    else
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

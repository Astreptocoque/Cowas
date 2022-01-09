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
// #include "Samples.h"

// ============ MAIN FUNCTION DECLARATION =======
void system_checkup();
void before_start_program();
void main_program();

// ============ EXECUTION MODE ===================
// #define  SYSTEM_CHECKUP

// ============ PIN DEFINITIONS ==================
// ====> define here the pins
const uint8_t  STATUS_LED_PIN = 22;
const uint8_t  GREEN_LED_PIN = 23;
const uint8_t  PRESSURE1_PIN = 3;
const uint8_t  PRESSURE2_PIN = 5;
const uint8_t  VALVE_1_PIN = 44;
const uint8_t  VALVE_23_PIN = 46;
const uint8_t  VALVE_PURGE = 45;
const uint8_t  PUMP_PIN = 6;
const uint8_t  PUMP_VACUUM = 34;
const uint8_t  ENCODER_A_PIN = 31;
const uint8_t  ENCODER_B_PIN = 33;
const uint8_t  ENCODER_Z_PIN = 35;
const uint8_t  BUTTON_START_PIN = 24;
const uint8_t  BUTTON_CONTAINER_PIN = 27;
const uint8_t  BUTTON_SPOOL_UP = 28;
const uint8_t  BUTTON_SPOOL_DOWN = 29;
const uint8_t  BUTTON_LEFT_PIN = 25;
const uint8_t  BUTTON_RIGHT_PIN = 26;
const uint8_t  POTENTIOMETER_PIN = A0;
const uint8_t  SD_CARD_PIN = 4; // need to change in Serial_output.h
const uint8_t  ESP8266_COMM_PIN = 12; // communication signal pin with wifi card
const uint8_t VALVE_STX_IN_PIN[NUMBER_SAMPLES] = {36, 38};
const uint8_t VALVE_STX_OUT_PIN[NUMBER_SAMPLES] = {32, 40};

// ============= REAL HARDWARE =================
// ====> do not forget to add the object.begin() in setup()
Serial_output output;                   // custom print function to handle multiple outputs
Serial_device esp8266;                  // enable communication with wifi card
Led status_led;                         // general purpose LED
Led green_led;                          // general purpose LED, used for start signals
Trustability_ABP_Gage pressure1;        // see schematics
Trustability_ABP_Gage pressure2;        // see schematics
Valve_2_2 valve_1;                      // see schematics
Valve_3_2 valve_23;                     // see schematics
Valve_2_2 valve_purge;                  // see schematics
Valve_2_2 valve_stx_in[MAX_SAMPLE];     // see schematics
Valve_3_2 valve_stx_out[MAX_SAMPLE];    // see schematics
Pump pump;                              // see schematics
Pump pump_vacuum;                       // see schematics
Motor spool;                            // see schematics
Encoder encoder;                        // see schematics
Button button_start;                    // User button. Normally open
Button button_container;                // Control button. Normally closed
Button button_spool_up;                 // Control button. Normally closed
Button button_spool_down;               // Control button. Normally closed
Button button_left;                     // User button. Normally open
Button button_right;                    // User button. Normally open
Potentiometer potentiometer;            // User rotary knob
struct Timer timer_control_pressure1;   // Timer for interrupts with pressure sensor 1
struct Timer timer_control_pressure2;   // Timer for interrupts with pressure sensor 2
GPIO wifi_message;                      // Input for message line report from wifi card


void setup()
{

    // ========== SYSTEM INITIALIZATION ============
    SPI.begin();
    output.begin(terminal);     // choose output of logs
    esp8266.begin();            // connect to wifi card

    timer_control_pressure1 = {TC1, 0, TC3_IRQn, 4};
    timer_control_pressure2 = {TC1, 1, TC4_IRQn, 4};

    // ========== HARDWARE INITIALIZATION ==========
    status_led.begin(STATUS_LED_PIN, "blue");
    green_led.begin(GREEN_LED_PIN, "green");
    pressure1.begin(PRESSURE1_PIN, 3, "P1");
    pressure2.begin(PRESSURE2_PIN, 3, "P2");
    valve_1.begin(VALVE_1_PIN, "V1");
    valve_23.begin(VALVE_23_PIN, "V_23");
    valve_purge.begin(VALVE_PURGE, "V_purge");
    for(uint8_t i = 0; i < NUMBER_SAMPLES; i++){
        valve_stx_in[i].begin(VALVE_STX_IN_PIN[i], "Vstx_in_" + String(i));
        valve_stx_out[i].begin(VALVE_STX_OUT_PIN[i], "Vstx_out_" + String(i));
    }
    pump.begin(PUMP_PIN, true, "P1");
    pump_vacuum.begin(PUMP_VACUUM, false, "Pvac");
    spool.begin();
    encoder.begin(ENCODER_A_PIN, ENCODER_B_PIN, ENCODER_Z_PIN, 720, 10);
    // attachInterrupt(digitalPinToInterrupt(ENCODER_Z_PIN), ISR_encoder_z_signal, RISING);
    button_left.begin(BUTTON_LEFT_PIN, "B_left");
    button_right.begin(BUTTON_RIGHT_PIN, "B_right");
    button_start.begin(BUTTON_START_PIN, "B_start");
    button_container.begin(BUTTON_CONTAINER_PIN, "B_container");
    button_spool_up.begin(BUTTON_SPOOL_UP, "B_spool_UP");
    // attachInterrupt(digitalPinToInterrupt(BUTTON_SPOOL_UP), ISR_emergency_stop_up, FALLING);
    button_spool_down.begin(BUTTON_SPOOL_DOWN, "B_spool_down");
    // attachInterrupt(digitalPinToInterrupt(BUTTON_SPOOL_DOWN), ISR_emergency_stop_down, FALLING);
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


    output.println("Get date");
    struct Date current_date;
    esp8266.start_communication();
    current_date = esp8266.receive_time();
    // esp8266.validate();
    setTime(current_date.epoch);
    // setSyncInterval(SYNC_TIME);
    // setSyncProvider(esp8266.receive_time());
    time_t t = now();
    output.println("It is " + String(hour(t)) + "h" + String(minute(t)) + "m, on the " + String(day(t)) + "." + String(month(t)) + "." + String(year(t)));

    // add to test samples
    add_sample(18, 30, 12, 1, 2022, 40);
    add_sample(19, 30, 13, 1, 2022, 20);
    add_sample(19, 30, 12, 1, 2022, 10, 4);
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
    // Sample sample(13, 15, 30);
    // Serial1.begin(115200);
}

void loop()
{


    // step_fill_container();
    step_fill_container();
    step_purge();
    button_start.waitPressedAndReleased();
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
    // test_pressure();
    //   main_program();
    //   test_depth();
    //   tests();
}


void main_program()
{

    // DO wifi and communication stuffs

    // when time occurs for a sample to be done
    if(now() > get_next_sample_time() - PREPARATION_TIME){

        Sample sample = get_next_sample();

        // COWAS sampling
        

        step_dive(sample.get_depth());
        for(uint8_t i = 0; i < PURGE_NUMBER; i++){
            step_fill_container();
            step_purge();
        }
        step_fill_container();
        step_sampling(get_next_sample_place());
        step_rewind();
        step_dry(get_next_sample_place());
        validate_sample();

    }
    
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

    /*     // check container switch
    if (button_container.getState() == 1)
        output.println("CHECK | Button container working");
    else{
        output.println("ERROR | Button container not working");
        error = true;
    } */

    //check temperature
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
        while (true)
            delay(500);
    }
}
#endif


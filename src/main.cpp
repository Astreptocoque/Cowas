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
#include "samples.h"
#include "Settings.h"
#include "Critical_error.h"
#include "Timer.h"

// ============ MAIN FUNCTION DECLARATION =======
void before_start();
void before_start_program();
void depths_test();
void tests();
void main_program();
void step_dive();
void step_purge();
void step_fill_container();
void step_sampling();
void step_flush_water();
void step_rewind();
// ============ EXECUTION MODE ===================
#define REAL_HARDWARE
//  #define VIRTUAL_HARDWARE
#define SYSTEM_CHECKUP

// ============ PIN DEFINITIONS ==================
// ====> define here the pins

#define BLUE_LED_PIN 22
#define GREEN_LED_PIN 23
#define PRESSURE1_PIN 3
// #define PRESSURE2_PIN
#define VALVE1_PIN 30
#define VALVE2_PIN 32
#define VALVE3_PIN 34
#define VALVE_STERIVEX1_PIN 36
#define VALVE_STERIVEX2_PIN 38
#define PUMP_PIN 8
#define ENCODER_A_PIN 31
#define ENCODER_B_PIN 33
#define ENCODER_Z_PIN 35
#define BUTTON_START_PIN 24
#define BUTTON_CONTAINER_PIN 27
#define BUTTON_SPOOL_PIN 28
#define BUTTON_LEFT_PIN 25
#define BUTTON_RIGHT_PIN 26
#define POTENTIOMETER_PIN A0

// ==============================================================================
// ====> NO CONSTRUCTOR EXPLICITELY DEFINED
// ====> because nothing works before setup() is called, so initializing code before it
// ====> in constructor won't work.
// ====> Workaround : use a begin() function as all arduino libraries do.

// ============= VIRTUAL TESTING =================
// ====> write here the hardware to test the code virtualy in the terminal output
// ====> declare hardware with respective interface class
#ifdef VIRTUAL_HARDWARE
Serial_output output;
Led_interface blue_led;
Led_interface green_led;
Pressure_interface pressure1;
Pressure_interface pressure2;
Valve_3_2_interface valve1;
Valve_3_2_interface valve2;
Valve_3_2_interface valve3;
Valve_2_2_interface valve_sterivex1;
Valve_2_2_interface valve_sterivex2;
Pump_interface pump;
Motor_interface spool;
Encoder_interface encoder;
Button_interface button_start;
Button_interface button_left;
Button_interface button_right;
Button_interface button_container;
Button_interface button_spool;
Potentiometer_interface potentiometer;
#endif

// ============= REAL HARDWARE =================
// ====> write here the hardware to test with the real machine
// ====> declare hardware with respective hardware class
// ====> do not forget to add the object.begin(PIN) in setup()
#ifdef REAL_HARDWARE
Serial_output output;
Led blue_led;
Led green_led;
Trustability_ABP_Gage pressure1;
// Trustability_ABP_Gage pressure2;
Valve_3_2 valve1;
Valve_3_2 valve2;
Valve_3_2 valve3;
Valve_2_2 valve_sterivex1;
Valve_2_2 valve_sterivex2;
Pump pump;
Motor spool;
Encoder encoder;
Button button_start;     // normally open
Button button_container; // normally ???
Button button_spool;     // normally closed
Button button_left;      // normally open
Button button_right;     // normally open
Potentiometer potentiometer;
struct Timer timer_control_pressure1;

#endif

void setup()
{

    // ========== SYSTEM INITIALIZATION ============
    output.begin(terminal);
    SPI.begin();
    timer_control_pressure1 = {TC1, 0, TC3_IRQn, 4};

    // ========== HARDWARE INITIALIZATION ==========
    blue_led.begin(BLUE_LED_PIN, "blue");
    green_led.begin(GREEN_LED_PIN, "green");
    pressure1.begin(PRESSURE1_PIN, 3, "P1");
    // pressure2.begin(PRESSURE2_PIN);
    valve1.begin(VALVE1_PIN, "V1");
    valve2.begin(VALVE2_PIN, "V2");
    valve3.begin(VALVE3_PIN, "V3");
    valve_sterivex1.begin(VALVE_STERIVEX1_PIN, "VS1");
    valve_sterivex2.begin(VALVE_STERIVEX2_PIN, "VS2");
    pump.begin(PUMP_PIN, "P1");
    spool.begin();
    encoder.begin(ENCODER_A_PIN, ENCODER_B_PIN, ENCODER_Z_PIN, 720, 10);
    // attachInterrupt(digitalPinToInterrupt(ENCODER_Z_PIN), ISR_encoder_z_signal, RISING);
    button_left.begin(BUTTON_LEFT_PIN, "B_left");
    button_right.begin(BUTTON_RIGHT_PIN, "B_right");
    button_start.begin(BUTTON_START_PIN, "B_start");
    button_container.begin(BUTTON_CONTAINER_PIN, "B_container");
    button_spool.begin(BUTTON_SPOOL_PIN, "B_spool");
    attachInterrupt(digitalPinToInterrupt(BUTTON_SPOOL_PIN), ISR_emergency_stop, FALLING);
    spool.endstop = false;
    potentiometer.begin(POTENTIOMETER_PIN);

    output.println("system initalized\n");

    // ======== PRE-STARTING EXECUTION =========
    output.println("========== Press start button to play program ==========================");
    output.println("========== Press left button to move spool up ==========================");
    output.println("========== Press right button to move spool down =======================");
    output.println("========== Press reset button on due button to come back here ==========");

    green_led.on();
    before_start_program();
    button_start.waitPressedAndReleased();
    green_led.off();

    before_start();
    output.println("System checked\n");


    output.println("Programm started\n");
    
}

void loop()
{
//   main_program();
//   depths_test();
  tests();
}

void tests(){
    
    timerStart(timer_control_pressure1);
    delay(5000);
    timerStop(timer_control_pressure1);
    delay(5000);
    
}

void depths_test()
{
     
    spool.start_origin();
    button_start.waitPressedAndReleased();
    output.println("=========== go 10 cm");
    spool.start(10);
    delay(1000);
    button_start.waitPressedAndReleased();
    output.println("=========== go 30 cm");
    spool.start(30);
    delay(1000);
    button_start.waitPressedAndReleased();
    output.println("=========== go 40 cm");
    spool.start(40);
    delay(1000);
    button_start.waitPressedAndReleased();
    output.println("=========== go 30 cm");
    spool.start(30);
    delay(1000);
    button_start.waitPressedAndReleased();
    output.println("=========== go origin cm");
    spool.start(0);
    button_start.waitPressedAndReleased();
    output.println("=========== go origin cm");
    spool.start(-1);

    output.println("####### end of trajectory #######");

    while (!button_right.isPressed())
    {
        if (button_left.isPressed())
        {
            output.println("Pulses A : " + String(encoder.get_pulses_A()));
            output.println("Pulses B : " + String(encoder.get_pulses_B()));
            output.println("Pulses Z : " + String(encoder.get_pulses_Z()));
            output.println("Distance : " + String(encoder.get_distance()));
            output.println("Direction : " + String(encoder.get_direction() == e_up ? "up" : "down"));
            button_left.waitPressedAndReleased();
        }
    }
    button_right.waitPressedAndReleased();

    green_led.on();
    button_start.waitPressedAndReleased();
    green_led.off();
}

void main_program()
{
    spool.start(30);
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
void before_start()
{
    // check if sensor are operationnal
    bool error = false;
    
    // check spool switch 1
    spool.start(20, down);
    delay(200);
    spool.stop();
    if (button_spool.getState() == 1)
        output.println("CHECK | Button spool working");
    else{
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
    if(pressure1.getTemperature() > 0){
        output.println("CHECK | Temperature okay (" + String(pressure1.getTemperature()) + ")");
    }else{
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

void step_dive(){

}

void step_purge(){

}

void step_fill_container(){

}

void step_sampling(){

}

void step_flush_water(){

}

void step_rewind(){

}
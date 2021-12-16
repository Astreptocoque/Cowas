/**
 * @file MAIN.CPP
 * @author Timothée Hirt & Christophe Deloose & Elio Sanchez
 * @brief A GenoRobotics project - CoWaS (Continous Water Sampling). Simple version with direct control
 *        of the pump and display
 *        ARDUINO DUE !!!
 * @version 2.0
 * @date 2021-10-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// #include <StaticThreadController.h>
// #include <Thread.h>
// #include <ThreadController.h>

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

#include "Communication/Communication.h"
#include <SPI.h>
#include "Serial_output.h"
#include "samples.h"
#include "Settings.h"

// ============ MAIN FUNCTION DECLARATION =======
void before_start();

// ============ EXECUTION MODE ===================
#define REAL_HARDWARE
// #define VIRTUAL_HARDWARE

// ============ PIN DEFINITIONS ==================
// ====> define here the pins

#define BLUE_LED_PIN 22
#define GREEN_LED_PIN 23
#define PRESSURE1_PIN 41
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
// ====> = !! Warning !! == NO CONSTRUCTOR EXPLICITELY DEFINED== !! Warning !! =
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
Button button_start;
Button button_container;
Button button_spool;
Button button_left;
Button button_right;
Potentiometer potentiometer;
#endif

// Treads
// Thread pressureThread = Thread();
// ThreadController controller = ThreadController();

// void callback(){
//     pressure1.readPressure();
// }
int counter = 0;
int aState;
int aLastState;
int bState;

void setup()
{

    // ========== SYSTEM INITIALIZATION ============
    output.begin(terminal);
    SPI.begin();

    // ========== HARDWARE INITIALIZATION ==========
    blue_led.begin(BLUE_LED_PIN, "L1");
    green_led.begin(GREEN_LED_PIN, "L2");
    pressure1.begin(PRESSURE1_PIN, 3, "P1");
    // pressure2.begin(PRESSURE2_PIN);
    valve1.begin(VALVE1_PIN, "V1");
    valve2.begin(VALVE2_PIN, "V2");
    valve3.begin(VALVE3_PIN, "V3");
    valve_sterivex1.begin(VALVE_STERIVEX1_PIN, "VS1");
    valve_sterivex2.begin(VALVE_STERIVEX2_PIN, "VS2");
    pump.begin(PUMP_PIN, "P1");
    spool.begin();
    delay(500);
    encoder.begin(ENCODER_A_PIN, ENCODER_B_PIN, ENCODER_Z_PIN, 720, 10);
    attachInterrupt(digitalPinToInterrupt(ENCODER_Z_PIN), ISR_encoder_z_signal, RISING);
    button_left.begin(BUTTON_LEFT_PIN, "B_left");
    button_right.begin(BUTTON_RIGHT_PIN, "B_right");
    button_start.begin(BUTTON_START_PIN, "B_start");
    button_container.begin(BUTTON_CONTAINER_PIN, "B_container");
    button_spool.begin(BUTTON_SPOOL_PIN, "B_spool");
    attachInterrupt(digitalPinToInterrupt(BUTTON_SPOOL_PIN), ISR_emergency_stop, FALLING);
    potentiometer.begin(POTENTIOMETER_PIN);

    pinMode(LED_BUILTIN, OUTPUT);
    // output.println("SPI initialized");
    // digitalWrite(LED_BUILTIN, HIGH);
    // communication with esp8266
    // output.println("ESP8266 initalized");
    // wifi connectivity
    // output.println("Wifi connected");
    // update current time
    // output.println("Time updated");

    // pressureThread settings
    // pressureThread.setInterval(1000);
    // pressureThread.onRun(callback);
    // controller.add(&pressureThread);
    output.println("system initalized");

    // ======== PRE-STARTING EXECUTION =========
    output.println("========== Press start button to play program ==========");
    output.println("========== Press left button to move spool up ==========");
    output.println("========== Press right button to move spool down ==========");
    output.println("========== Press reset button on due button to come back here ==========");

    green_led.on();
    before_start();
    button_start.waitPressedAndReleased();
    green_led.off();
    output.println("Programm started\n");
}

void loop()
{

    // update_time();

    spool.set_speed(400, up);
    spool.start();

    while (encoder.get_distance() > -4300)
    {
        encoder.step_counter();
        if (button_left.isPressed())
        {
            output.println("Pulses A : " + String(encoder.get_pulses_A()));
            output.println("Pulses Z : " + String(encoder.get_pulses_A()));
            output.println("Distance : " + String(encoder.get_distance()));
            output.println("Direction : " + String(encoder.get_direction() == e_up ? "up" : "down"));
            button_left.waitPressedAndReleased();
        }
        if(button_right.isPressed()){
            spool.stop();
            button_right.waitPressedAndReleased();
            spool.start();
        }
    }
    spool.stop();

    while (!button_right.isPressed())
    {
        if (button_left.isPressed())
        {
            output.println("Pulses A : " + String(encoder.get_pulses_A()));
            output.println("Pulses Z : " + String(encoder.get_pulses_A()));
            output.println("Distance : " + String(encoder.get_distance()));
            output.println("Direction : " + String(encoder.get_direction() == e_up ? "up" : "down"));
            button_left.waitPressedAndReleased();
        }
    }
    button_right.waitPressedAndReleased();

    green_led.on();
    button_start.waitPressedAndReleased();
    green_led.off();
    // if(button_right.isPressed()){
    //     encoder.reset();
    //     button_right.waitPressedAndReleased();
    // }

    // button.waitPressedAndReleased();
    // spool.set_speed(150, up);
    // spool.start();
    // valve1.set_I_way();

    // // uint32_t temps = millis();
    // // while (millis() - temps < timem * 1000)
    // // {
    // while (counter > -300)
    // {
    //     step_counter();
    // }
    // spool.stop();
    // delay(1000);
    // button.waitPressedAndReleased();
    // spool.set_speed(50, down);
    // spool.start();

    // // unsigned int start_millis = millis();
    // // while (millis() - start_millis < timem * 1000)
    // while (counter < 0)
    // {
    //     step_counter();
    // }
    // spool.stop();
    // delay(1000);
}

// void step_counter()
// {
//     aState = digitalRead(ENCODER_A_PIN);
//     if (aState != aLastState)
//     {
//         if (digitalRead(ENCODER_B_PIN) != aState)
//         {
//             counter++;
//         }
//         else
//         {
//             counter--;
//         }
//     }
//     aLastState = aState;
//     if (button_left.isPressed())
//     {
//         output.println(counter);
//         button_left.waitPressedAndReleased();
//     }
//     if (button_right.isPressed()){
//         counter = 0;
//         output.println(counter);
//         button_right.waitPressedAndReleased();
//     }
// }

void before_start()
{
    // move the motor before starting
    while (!button_start.isPressed())
    {
        if (button_left.isPressed())
        {
            spool.set_speed(200, up);
            spool.start();
            while (button_left.isPressed())
                delay(5);
            spool.stop();
        }
        if (button_right.isPressed())
        {
            spool.set_speed(200, down);
            spool.start();
            while (button_right.isPressed())
                delay(5);
            spool.stop();
        }
        delay(10);
    }
}
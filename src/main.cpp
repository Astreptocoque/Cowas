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

#include "Led.h"

#include "Communication/Communication.h"
#include <SPI.h>
#include "Serial_output.h"
#include "samples.h"
#include "Settings.h"

void step_counter();
// ============ EXECUTION MODE ===================
#define REAL_HARDWARE
// #define VIRTUAL_HARDWARE
// ============ PIN DEFINITIONS ==================
// ====> define here the pins
#define BUTTON_PIN 5
#define BUTTON2_PIN 9
#define START_BUTTON_PIN 26
#define BLUE_LED_PIN 22
#define GREEN_LED_PIN 24
#define PRESSURE1_PIN 3
#define PRESSURE2_PIN 2
#define VALVE1_PIN 3
#define VALVE2_PIN 25
#define VALVE3_PIN 23
// #define VALVE_STERIVEX1_PIN 10
#define VALVE_STERIVEX2_PIN 7
#define PUMP_PIN 8
#define ENCODER_A_PIN 13
#define ENCODER_B_PIN 12
#define ENCODER_Z_PIN 11
// // #define SPOOL_PIN 9

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
Pressure_interface pressure1;
Pressure_interface pressure2;
Valve_3_2_interface valve1;
Valve_3_2_interface valve2;
Valve_3_2_interface valve3;
Valve_2_2_interface valve_sterivex1;
Valve_2_2_interface valve_sterivex2;
Pump_interface pump;
Motor_interface spool;
Led_interface blue_led;
Led_interface green_led;
Button_interface start_button;
Button_interface button;
#endif

// ============= REAL HARDWARE =================
// ====> write here the hardware to test with the real machine
// ====> declare hardware with respective hardware class
// ====> do not forget to add the object.begin(PIN) in setup()
#ifdef REAL_HARDWARE
Serial_output output;
Trustability_ABP_Gage pressure1;
Trustability_ABP_Gage pressure2;
Valve_3_2 valve1;
Valve_3_2 valve2;
Valve_3_2 valve3;
Valve_2_2 valve_sterivex1;
Valve_2_2 valve_sterivex2;
Pump pump;
Motor spool;
Button button;
Button button2;
Led blue_led;
Led green_led;
Button start_button;
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
    // pressure1.begin(PRESSURE1_PIN, "P1", 3);
    // pressure2.begin(PRESSURE2_PIN);
    // valve1.begin(VALVE1_PIN);
    // valve2.begin(VALVE2_PIN, "V2");
    // valve3.begin(VALVE3_PIN, "V3");
    // valve_sterivex1.begin(VALVE_STERIVEX1_PIN);
    button.begin(BUTTON_PIN);
    button2.begin(BUTTON2_PIN);
    blue_led.begin(22);
    green_led.begin(24);
    start_button.begin(26);
    spool.begin();
    // valve_sterivex1.begin(VALVE_STERIVEX1_PIN);
    // valve_sterivex2.begin(VALVE_STERIVEX2_PIN);
    // pump.begin(PUMP_PIN, "P1");
    // spool.begin(SPOOL_PIN);

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

    pinMode(ENCODER_A_PIN, INPUT);
    pinMode(ENCODER_B_PIN, INPUT);
    pinMode(ENCODER_Z_PIN, INPUT);
    aLastState = digitalRead(ENCODER_A_PIN);

    // DO NOT TOUCH
    output.println("========== Press start button to play program ==========");
    output.println("========== Press left button to move spool up ==========");
    output.println("========== Press right button to move spool down ==========");
    output.println("========== Press reset button on due button to come back here ==========");

    green_led.on();
    // move the motor before starting
    while (!start_button.isPressed())
    {
        if (button.isPressed())
        {
            spool.set_speed(150, up);
            spool.start();
            while(button.isPressed())
                step_counter();
            spool.stop();
        }
        if (button2.isPressed())
        {
            spool.set_speed(50, down);
            spool.start();
            while(button2.isPressed())
                step_counter();
            spool.stop();
        }
        delay(10);
    }
    start_button.waitPressedAndReleased();
    green_led.off();
    output.println("Programm started\n");
}

void loop()
{
    //     // controller.run();
    //     pressure1.readPressure();
    //     output.print("Temperature : ");
    //  //   Serial.println(pressure1.getPressure());
    //     output.println(pressure1.getTemperature());
    //     output.print("Pression : ");
    //     output.println(pressure1.getPressure());
    //     delay(1000);

    // update_time();

    // output.println("enter number");
    // unsigned int timem = output.waitInt();
    //     spool.set_speed(150, up);
    // spool.start();
    // delay(1000);
    // spool.stop();

    button.waitPressedAndReleased();
    spool.set_speed(150, up);
    spool.start();
    valve1.set_I_way();
    
    // uint32_t temps = millis();
    // while (millis() - temps < timem * 1000)
    // {
    while (counter > -300)
    {
        step_counter();
    }
    spool.stop();
    delay(1000);
    button.waitPressedAndReleased();
    spool.set_speed(50, down);
    spool.start();

    // unsigned int start_millis = millis();
    // while (millis() - start_millis < timem * 1000)
    while (counter < 0)
    {
        step_counter();
    }
    spool.stop();
    delay(1000);

    // spool.set_speed(250, up);
    // spool.start();
    // delay(500);
    // spool.stop();

    // spool_spool.set_speed(350,up);
    // spool.start();
    // delay(2000);
    // spool.stop();
    // delay(1000);
    // spool.set_speed(400, down);
    // spool.start();
    // delay(2000);

    // button.waitPressedAndReleased();
    // pump.start();
    // valve3.set_I_way();
    // button.waitPressedAndReleased();
    // pump.stop();
    // valve2.switch_way();
    // valve3.set_L_way();

    // // PURGE
    // output.println("==== PURGE ====");
    // output.println("...");
    // output.println("=== SAMPLING ===");
    // // SAMPLING
    // int time = 1500;
    // valve1.L_way();
    // delay(time);
    // spool.start(40);
    // delay(time);
    // valve1.I_way();
    // delay(time);
    // valve2.I_way();
    // delay(time);
    // valve3.L_way();
    // delay(time);
    // output.println("=== FILL 2L CONTAINER ===");
    // pump.start(); // DEFINE WHEN TO STOP
    // delay(time);
    // pump.stop();
    // delay(time);
    // valve2.L_way();
    // delay(time);
    // valve3.I_way();
    // delay(time);

    // // CHOOSE STERIVEX
    // output.println("=== CHOOSE STERIVEX ===");

    // switch(1){
    //     case 1:
    //         valve_sterivex1.open();
    //     break;
    //     case 2:
    //         valve_sterivex2.open();
    //     break;
    // }
    // output.println("=== FLUSH ===");

    // delay(time);
    // pump.start();
    // delay(time);
    // pump.stop();
    // delay(time);

    // // CHOOSE STERIVEX
    // switch(1){
    //     case 1:
    //         valve_sterivex1.close();
    //     break;
    //     case 2:
    //         valve_sterivex2.close();
    //     break;
    // }
    // delay(time);
}

void step_counter()
{
    aState = digitalRead(ENCODER_A_PIN);
    if (aState != aLastState)
    {
        if (digitalRead(ENCODER_B_PIN) != aState)
        {
            counter++;
        }
        else
        {
            counter--;
        }
        output.println(counter);
    }
    aLastState = aState;
    // if (button.isPressed())
    // {
    //     counter = 0;
    //     output.println(counter);
    //     button.waitPressedAndReleased();
    // }
}
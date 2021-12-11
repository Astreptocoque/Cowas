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

// ============ EXECUTION MODE ===================
#define REAL_HARDWARE

// ============ PIN DEFINITIONS ==================
// ====> define here the pins
#define BUTTON_PIN 5
// #define PRESSURE1_PIN 3
// #define PRESSURE2_PIN 2
// #define VALVE1_PIN 3
// #define VALVE2_PIN 11
// #define VALVE3_PIN 10
#define VALVE_STERIVEX1_PIN 10
// #define VALVE_STERIVEX2_PIN 7
// #define PUMP_PIN 8
// #define MOTOR_SPOOL_PIN 9

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
Motor_interface motor_spool;
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
Motor motor_spool;
Button button;
#endif


// Treads
// Thread pressureThread = Thread();
// ThreadController controller = ThreadController();

// void callback(){
//     pressure1.readPressure();
// }

void setup()
{   

    // ========== SYSTEM INITIALIZATION ============
    output.begin(1);
    SPI.begin();

    // ========== HARDWARE INITIALIZATION ==========
    // pressure1.begin(PRESSURE1_PIN);
    // pressure2.begin(PRESSURE2_PIN);
    // valve1.begin(VALVE1_PIN);
    // valve2.begin(VALVE2_PIN);
    // valve3.begin(VALVE3_PIN);
    // valve_sterivex1.begin(VALVE_STERIVEX1_PIN);
    button.begin(BUTTON_PIN);
    // valve_sterivex1.begin(VALVE_STERIVEX1_PIN);
    // valve_sterivex2.begin(VALVE_STERIVEX2_PIN);
    // pump.begin(PUMP_PIN);
    // motor_spool.begin(MOTOR_SPOOL_PIN);

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

    button.waitPressedAndReleased();
    valve_sterivex1.set_open_way();
    button.waitPressedAndReleased();
    valve_sterivex1.set_close_way();
    // button.waitPressedAndReleased();
    // // valve2.set_L_way();
    // // valve3.set_L_way();
    // output.println("Valve 3 state = " + String(valve3.get_state()));
    // output.println("Valve 2 state = " + String(valve2.get_state()));
    // if time to sample
    // define cowas as busy

    // // PURGE
    // output.println("==== PURGE ====");
    // output.println("...");
    // output.println("=== SAMPLING ===");
    // // SAMPLING
    // int time = 1500;
    // valve1.L_way();
    // delay(time);
    // motor_spool.start(40);
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
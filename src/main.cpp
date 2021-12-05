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

#include "Led.h"
#include "Communication/Communication.h"
#include <SPI.h>
#include "Serial_output.h"
#include "samples.h"

// ============ PIN DEFINITIONS ==================
#define BUTTON_PIN 5
#define PRESSURE1_PIN 1
#define PRESSURE2_PIN 2
#define VALVE1_PIN 3
#define VALVE2_PIN 4
#define VALVE3_PIN 5
#define VALVE_STERIVEX1_PIN 6
#define VALVE_STERIVEX2_PIN 7
#define PUMP_PIN 8

// ============= VIRTUAL TESTING =================
Serial_output output(1);
Pressure_interface pressure1(PRESSURE1_PIN);
Pressure_interface pressure2(PRESSURE2_PIN);
Valve_3_2_interface valve1(VALVE1_PIN);
Valve_3_2_interface valve2(VALVE2_PIN);
Valve_3_2_interface valve3(VALVE3_PIN);
Valve_2_2_interface valve_sterivex1(VALVE_STERIVEX1_PIN);
Valve_2_2_interface valve_sterivex2(VALVE_STERIVEX2_PIN);
Pump_interface pump(PUMP_PIN);

// ============= REAL HARDWARE =================
// Serial_output output(1);
// Trustability_ABP_Gage pressure1(PRESSURE1_PIN);
// Trustability_ABP_Gage pressure2(PRESSURE2_PIN);
// Valve_3_2 valve1(VALVE1_PIN);
// Valve_3_2 valve2(VALVE2_PIN);
// Valve_3_2 valve3(VALVE3_PIN);
// Valve_2_2 valve_sterivex1(VALVE_STERIVEX1);
// Valve_2_2 valve_sterivex2(VALVE_STERIVEX2);
// Pump pump(PUMP_PIN);

// Treads
// Thread pressureThread = Thread();
// ThreadController controller = ThreadController();

// void callback(){
//     pressure1.readPressure();
// }

void setup()
{
    SPI.begin();
    output.println("SPI initialized");
    // communication with esp8266
    output.println("ESP8266 initalized");
    // wifi connectivity
    output.println("Wifi connected");
    
    // pressureThread settings
    // pressureThread.setInterval(1000);
    // pressureThread.onRun(callback);
    // controller.add(&pressureThread);
}


void loop()
{
    // controller.run();
    pressure1.readPressure();
    output.print("Temperature : ");
 //   Serial.println(pressure1.getPressure());
    output.println(pressure1.getTemperature());
    output.print("Pression : ");
    output.println(pressure1.getPressure());
    delay(1000);




}
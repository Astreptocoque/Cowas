/**
 * @file CoWas_simple_excel.ino
 * @author Timothée Hirt & Christophe Deloose & Elio Sanchez
 * @brief A GenoRobotics project - CoWaS (Continous Water Sampling). Simple version with direct control
 *        of the pump and display
 *        ARDUINO DUE !!!
 * @version 1.0
 * @date 2021-10-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// #include <StaticThreadController.h>
// #include <Thread.h>
// #include <ThreadController.h>

#include <Arduino.h>
#include "Hardware/Button.h"
#include "Hardware/Pressure_sensor.h"
#include "Hardware/Led.h"
#include "Communication/Communication.h"
#include <SPI.h>

// pins definition
#define BUTTON_PIN 5

Pressure_sensor pressure1(3);

// Treads
// Thread pressureThread = Thread();
// ThreadController controller = ThreadController();

// void callback(){
//     pressure1.readPressure();
// }

void setup()
{
    SPI.begin();
    Serial.begin(9600);

    // pressureThread settings
    // pressureThread.setInterval(1000);
    // pressureThread.onRun(callback);
    // controller.add(&pressureThread);
    
}

void loop()
{
    // controller.run();
    pressure1.readPressure();
    Serial.print("Temperature : ");
 //   Serial.println(pressure1.getPressure());
    Serial.println(pressure1.getTemperature());
    Serial.print("Pression : ");
    Serial.println(pressure1.getPressure());
    delay(1000);
}
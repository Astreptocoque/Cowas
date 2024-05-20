/**
 * @file maintenance.cpp
 * @author Felix Schmeding
 * @brief Functions for maintenance of the CoWaS, read instructions for each function carefully
 * @version 0.1
 * @date 2024-05-20
 */
#include <Arduino.h>
#include "Button.h"
#include "Trustability_ABP_Gage.h"
#include "Valve_3_2.h"
#include "Valve_2_2.h"
#include "Pump.h"
#include "Micro_pump.h"
#include "Motor.h"
#include "Encoder.h"
#include "Led.h"
#include "C_output.h"
#include "Settings.h"
#include "Timer.h"
#include "Serial_device.h"
#include "maintenance.h"
#include "Step_functions.h"
#include "Manifold.h"

extern int manifold_slot;
extern C_output output;
extern Serial_device serial;
extern Led status_led;
extern Led green_led;
extern Trustability_ABP_Gage pressure1;
extern Valve_2_2 valve_1;
extern Valve_3_2 valve_23;
extern Valve_2_2 valve_manifold;
extern Pump pump;

extern Micro_Pump micro_pump;
extern Micro_Pump test_5V_micro_pump;

extern Motor spool;
extern Motor manifold_motor;
extern Encoder encoder;
extern Button button_start;    
extern Button button_container;
extern Button button_spool_up;     
extern Button button_spool_down;
extern Button button_left;      
extern Button button_right;

void run_pump(uint32_t max_time);


void purge_pipes_manifold(){
    Serial.println("Ready to start purge of manifold pipes, please press start button");

    button_start.waitPressedAndReleased();

    // fill some ethanol in the container
    valve_1.set_close_way();
    valve_23.set_I_way();
    // pump for 1 minute or button start is pressed
    pump.set_power(POWER_PUMP);
    
    run_pump(MAINT_FILL_TIME);
    delay(2000);

    for (uint8_t slot = 0; slot < 15; slot++){
        Serial.print("Filling and purging slot ");
        Serial.println(slot);

        valve_1.set_close_way();
        valve_23.set_I_way();
        run_pump(MAINT_FILL_TIME);

        rotateMotor(slot);
        delay(500);

        valve_23.set_L_way();
        valve_manifold.set_open_way();
        run_pump(MAINT_PURGE_TIME);
        valve_manifold.set_close_way();

        if (slot == 8){
            rotateMotor(4);
            rotateMotor(0);
        }

        delay(500);
    }

    valve_1.set_close_way();
    // going back in a circle to not tangle the pipes

}



void run_pump(uint32_t max_time){
    uint32_t start_time = millis();
    pump.start();

    do {
        delay(20);
        // Serial.println(millis() - start_time);
    } while (button_start.isReleased() && (millis() - start_time) < max_time);

    if (button_start.isPressed()) 
        button_start.waitPressedAndReleased();

    pump.stop();
}
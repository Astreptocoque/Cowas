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
#include "sample.h"
#include "Settings.h"
#include "Critical_error.h"
#include "Timer.h"
#include "Serial_device.h"
#include "Tests.h"

extern Serial_output output;
extern Serial_device serial;
extern Led blue_led;
extern Led green_led;
extern Trustability_ABP_Gage pressure1;
extern Trustability_ABP_Gage pressure2;
extern Valve_2_2 valve_1;
extern Valve_3_2 valve_23;
extern Valve_2_2 valve_purge;
extern Valve_2_2 valve_stx_1_in;
extern Valve_2_2 valve_stx_2_in;
extern Valve_3_2 valve_stx_1_out;
extern Valve_3_2 valve_stx_2_out;
extern Pump pump;
extern Pump pump_vacuum;
extern Motor spool;
extern Encoder encoder;
extern Button button_start;     // normally open
extern Button button_container; // normally ???
extern Button button_spool_up;     // normally closed
extern Button button_spool_down;
extern Button button_left;      // normally open
extern Button button_right;     // normally open
extern Potentiometer potentiometer;
extern struct Timer timer_control_pressure1;

void tests()
{

    if (serial.available())
    {
        output.println(serial.receive());
    }

    // output.println(pressure1.getPressure());
    // output.println(pressure1.getTemperature());
    // delay(500);
}

void test_hardware_general(){

    output.println("Valve stx 1 in");
    button_left.waitPressedAndReleased();
    valve_stx_1_in.switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_stx_1_in.switch_way();

    output.println("Valve stx 2 in");
    button_left.waitPressedAndReleased();
    valve_stx_2_in.switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_stx_2_in.switch_way();

    output.println("Valve stx 1 out");
    button_left.waitPressedAndReleased();
    valve_stx_1_out.switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_stx_1_out.switch_way();

    output.println("Valve stx 2 out");
    button_left.waitPressedAndReleased();
    valve_stx_2_out.switch_way();
    delay(500);
    button_left.waitPressedAndReleased();
    valve_stx_2_out.switch_way();

    output.println("Pump vaccum");
    button_left.waitPressedAndReleased();
    pump_vacuum.start();
    delay(500);
    button_left.waitPressedAndReleased();
    pump_vacuum.stop();
    // output.println("Valve stx 1 in");
    // button_left.waitPressedAndReleased();
    // valve_stx_1_in.set_open_way();
    // delay(500);
    // button_left.waitPressedAndReleased();
    // valve_stx_1_in.set_close_way();

    // output.println("Valve stx 2 in");
    // button_left.waitPressedAndReleased();
    // valve_stx_2_in.set_open_way();
    // delay(500);
    // button_left.waitPressedAndReleased();
    // valve_stx_2_in.set_close_way();

    // output.println("Valve stx 1 out");
    // button_left.waitPressedAndReleased();
    // valve_stx_1_out.set_I_way();
    // delay(500);
    // button_left.waitPressedAndReleased();
    // valve_stx_1_out.set_L_way();

    // output.println("Valve stx 2 out");
    // button_left.waitPressedAndReleased();
    // valve_stx_2_out.set_I_way();
    // delay(500);
    // button_left.waitPressedAndReleased();
    // valve_stx_2_out.set_L_way();

    // output.println("Pump vaccum");
    // button_left.waitPressedAndReleased();
    // pump_vacuum.start();
    // delay(500);
    // button_left.waitPressedAndReleased();
    // pump_vacuum.stop();
    
}

void test_serial_device(){
    if (Serial1.available())
    {
        output.println(Serial1.read());
        output.println("");
    }
}

void test_pressure(){

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
            button_left.waitPressedAndReleased();
            pump.set_power(speedy);
            pump.start();
            
        }
        if (button_right.isPressed())
        {
           pump.stop();
            button_right.waitPressedAndReleased();

        }

        output.println("P1 " + String(pressure1.getPressure()) + "  |   P2 " + String(pressure2.getPressure()));
        delay(200);
    }
}

void test_depth()
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

    green_led.on();
    button_start.waitPressedAndReleased();
    green_led.off();
}
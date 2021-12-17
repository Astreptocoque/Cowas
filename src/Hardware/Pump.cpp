#include <Arduino.h>
#include "Pump.h"

/**
 * @brief Constructor for a diaphragm pump
 * @param control_pin Output connection on the board
 *
 */
void Pump::begin(byte _control_pin)
{
    control_pin = _control_pin;
    pinMode(control_pin, INPUT);
    set_power(190); // after 190, flow doesn't increase
    stop();
}

void Pump::begin(byte _control_pin, String _ID)
{
    ID = _ID;
    control_pin = _control_pin;
    pinMode(control_pin, OUTPUT);
    set_power(190);
    stop();
}

void Pump::set_flow(int _flow){
    output.println("function -set_flow- not implemented");
}

void Pump::set_power(int _power){
    power = _power;
}

void Pump::start(){
    analogWrite(control_pin, power);
    Pump_interface::start();
}

void Pump::start(int time_ms){
    start();
    int current_time = millis();
    while(millis() - current_time < time_ms){
        delay(5);
    }
    stop();
}

void Pump::stop(){
    analogWrite(control_pin, 0);
    Pump_interface::stop();
    
}
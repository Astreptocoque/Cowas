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
}

void Pump::set_flow(int _flow){

}

void Pump::set_power(int _power){

}

void Pump::start(){

}

void Pump::stop(){
    
}
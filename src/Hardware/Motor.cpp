#include <Arduino.h>
#include "Motor.h"

/**
 * @brief Constructor for a diaphragm Motor
 * @param control_pin Output connection on the board
 *
 */
Motor::Motor(byte _control_pin)
{
    control_pin = _control_pin;
    pinMode(control_pin, INPUT);
}

void Motor::set_power(int _power){

}

void Motor::start(){

}

void Motor::start(uint8_t _depth){

}

void Motor::stop(){
    
}
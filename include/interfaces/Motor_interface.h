#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Motor_interface
{

protected:
    byte control_pin;
    int power; // refers to power

public:
    virtual void begin(byte _control_pin){
        control_pin = _control_pin;
        output.println("Motor created on pin " + String(control_pin));
    }
    virtual void set_power(int _power){
        output.println("Power set on " + String(_power) + "% 24VDC");
    }
    virtual void start(){
        output.println("Motor started");
    }
    virtual void start(uint8_t _depth){
        output.println("Motor started and run for " + String(_depth) + " meters");
    }
    virtual void stop(){
        output.println("Motor stopped");
    }
    
};

#endif
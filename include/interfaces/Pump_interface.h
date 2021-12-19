#ifndef PUMP_INTERFACE_H
#define PUMP_INTERFACE_H

#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Pump_interface
{

protected:
    byte control_pin;
    int power; // refers to power
    bool pwm; // if pump is controlled by pwm or only HIGH/LOW
    String ID = "no_ID";

public:
    virtual void begin(byte _control_pin, bool _pwm){
        output.println("Pump " + ID + " initiated");
    }
    virtual void begin(byte _control_pin, bool _PWM, String _ID){
        ID = _ID;
        output.println("Pump " + ID + " initiated");
    }
    virtual void set_flow(int _flow){
        power = _flow; //function needed)
        output.println("Flow pump " + ID + " set on " + String(_flow) + " L/min");
    }
    virtual void set_power(int _power){
        power = _power;
        output.println("Power pump " + ID + " set on " + String(_power) + "% 24VDC");
    }
    virtual void start(){
        output.println("Pump " + ID + " started with power " + power);
    }
    virtual void stop(){
        output.println("Pump " + ID + " stopped");
    }
    
};

#endif
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
    String name = "default";

public:
    virtual void begin(byte _control_pin){
        control_pin = _control_pin;
        output.println("Pump created on pin " + String(control_pin));
    }
    virtual void begin(byte _control_pin, String _name){
        name = _name;
        control_pin = _control_pin;
        output.println("Pump " + name + " created on pin " + String(control_pin));
    }
    virtual void set_flow(int _flow){
        power = _flow; //function needed)
        output.println("Flow pump " + name + " set on " + String(_flow) + " L/min");
    }
    virtual void set_power(int _power){
        power = _power;
        output.println("Power pump " + name + " set on " + String(_power) + "% 24VDC");
    }
    virtual void start(){
        output.println("Pump " + name + " started with power " + power);
    }
    virtual void stop(){
        output.println("Pump " + name + " stopped");
    }
    
};

#endif
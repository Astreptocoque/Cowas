#ifndef PUMP_INTERFACE_H
#define PUMP_INTERFACE_H

#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Pump_interface
{

protected:
    byte control_pin;
    int flow; // refers to power

public:
    virtual void begin(byte _control_pin){
        control_pin = _control_pin;
        output.println("Pump created on pin " + String(control_pin));
    }
    virtual void set_flow(int _flow){
        flow = _flow;
        output.println("Flow set on " + String(flow) + " L/min");
    }
    virtual void set_power(int _power){
        output.println("Power set on " + String(_power) + "% 24VDC");
    }
    virtual void start(){
        output.println("Pump started");
    }
    virtual void stop(){
        output.println("Pump stopped");
    }
    
};

#endif
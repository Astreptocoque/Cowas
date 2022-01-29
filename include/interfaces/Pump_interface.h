#ifndef PUMP_INTERFACE_H
#define PUMP_INTERFACE_H

#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Pump_interface
{

protected:
    byte control_pin;
    uint8_t power; // refers to power
    uint8_t power_percent;
    bool pwm; // if pump is controlled by pwm or only HIGH/LOW
    String ID = "no_ID";
    bool running = false;

public:
    virtual void begin(byte _control_pin, bool _pwm){
        output.println("Pump " + ID + " initiated");
    }
    virtual void begin(byte _control_pin, bool _PWM, String _ID){
        ID = _ID;
        output.println("Pump " + ID + " initiated");
    }
    virtual void set_flow(int _flow){
        power = _flow;
        output.println("Flow pump " + ID + " set on " + String(_flow) + " L/min");
    }
    virtual void set_power(int8_t _power){
        power = _power;
        output.println("Power pump " + ID + " set on " + String(_power) + "% 24VDC");
    }
    uint8_t get_power(){
        output.println("Power pump " + ID + " asked");
        return 1;
    }
    virtual void start(){
        output.println("Pump " + ID + " started with power " + power_percent);
    }
    virtual void start(uint32_t _time_ms){
        output.println("Pump " + ID + " started with power " + power_percent + " for " + String(_time_ms) + " ms");
    }
    virtual void stop(){
        output.println("Pump " + ID + " stopped");
    }
    
};

#endif
#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

enum motor_direction{down, up};

class Motor_interface
{

protected:
    byte control_pin;
    int speed; // refers to power
    motor_direction direction;

public:
    virtual void begin(){
    }
    virtual void set_speed(int _speed, int _direction){
        output.println("Power set on " + String(_speed) + "% 24VDC");
    }
    virtual void start(){
        output.println("Motor started with speed " + String(speed) + " in direction " + (direction==up?"up":"down"));
    }
    virtual void start(uint8_t _depth){
        output.println("Motor started and run for " + String(_depth) + " meters");
    }
    virtual void stop(){
        output.println("Motor stopped");
    }
    
};

#endif
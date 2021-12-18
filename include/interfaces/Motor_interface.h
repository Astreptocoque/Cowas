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
    String ID = "no_ID";

public:
    motor_direction direction;

    virtual void begin(){
        output.println("Motor " + ID + " initiated");
    }
    virtual void set_speed(int _speed, int _direction){
        output.println("Power set on " + String(_speed) + "%");
    }
    virtual void start(){
        output.println("Motor started with speed " + String(speed) + " in direction " + (direction==up?"up":"down"));
    }
    virtual void start(int _speed, motor_direction _direction){
        output.println("Motor started with speed " + String(speed) + " in direction " + (direction==up?"up":"down"));
    }
    virtual void start(int _depth){
        output.println("Motor started to go at " + String(_depth)) ;
    }
    virtual void start_origin(){
        output.println("Motor started to go at origin");
    }
    virtual void stop(){
        output.println("Motor stopped");
    }    
};

#endif
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "interfaces/Motor_interface.h"

const int DISTANCE_FROM_STOP = 15;

class Motor : public Motor_interface
{

protected:
    int depth_current;
    int depth_goal;

public:
    bool endstop; // public for interrupt
    void begin();
    void set_speed(uint8_t _speed, motor_direction _direction);
    void start();
    void start(uint8_t _speed, motor_direction _direction);
    void start(int _depth);
    void start_origin();
    void stop();
    void stopIfFault();
};

void ISR_emergency_stop();

#endif
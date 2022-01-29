#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "interfaces/Motor_interface.h"

class Motor : public Motor_interface
{

private:
    int depth_current;
    int depth_goal;

public:
    bool endstop_down = false; // public for interrupt
    bool endstop_up = false;
    void begin();
    void set_speed(uint8_t _speed, motor_direction _direction);
    void start();
    void start(uint8_t _speed, motor_direction _direction);
    void start(int _depth);
    void start_origin();
    void stop();
    void stopIfFault();
};

void ISR_emergency_stop_down();
void ISR_emergency_stop_up();

#endif
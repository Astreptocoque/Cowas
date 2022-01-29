#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

enum motor_direction{down, up};

class Motor
{

private:
    byte control_pin;
    int speed; // Refers to power
    String ID = "no_ID";
    int depth_current;
    int depth_goal;

public:
    motor_direction direction;
    
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
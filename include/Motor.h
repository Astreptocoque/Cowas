#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "interfaces/Motor_interface.h"


class Motor : public Motor_interface
{

private:

public:
    void begin();
    void set_speed(int _speed, motor_direction _direction);
    void start();
    void start(uint8_t _depth);
    void stop();
    void stopIfFault();
};

void ISR_emergency_stop();

#endif
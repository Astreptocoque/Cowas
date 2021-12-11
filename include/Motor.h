#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "interfaces/Motor_interface.h"

class Motor : public Motor_interface
{

private:

public:
    void begin(byte _control_pin);
    void set_power(int _power);
    void start();
    void start(uint8_t _depth);
    void stop();
};

#endif
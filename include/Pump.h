#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>
#include "interfaces/Pump_interface.h"

class Pump : public Pump_interface
{

private:

public:
    void begin(byte _control_pin, bool _pwm);
    void begin(byte _control_pin, bool _pwm, String _name);
    void set_flow(int _flow);
    void set_power(int8_t _power);
    int get_power();
    void start();
    void start(uint32_t time_ms);
    void stop();
};

#endif
#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>
#include "interfaces/Pump_interface.h"

class Pump : public Pump_interface
{

private:

public:
    void begin(byte _control_pin);
    void begin(byte _control_pin, String _name);
    void set_flow(int _flow);
    void set_power(int _power);
    void start();
    void start(int time_ms);
    void stop();
};

#endif
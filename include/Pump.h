#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>
#include "Pump_interface.h"

class Pump : public Pump_interface
{

private:

public:
    Pump(byte _control_pin);
    void set_flow(int _flow);
    void set_power(int _power);
    void start();
    void stop();
};

#endif
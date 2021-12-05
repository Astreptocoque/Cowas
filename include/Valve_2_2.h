#ifndef VALVE_2_2_H
#define VALVE_2_2_H

#include <Arduino.h>
#include "Valve_2_2_interface.h"

class Valve_2_2 : public Valve_2_2_interface
{

private:

public:
    Valve_2_2(byte _pin_control);
    void open_way();
    void close_way();
    void switch_way();
    bool get_state();
};

#endif
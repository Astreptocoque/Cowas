#ifndef VALVE_3_2_H
#define VALVE_3_2_H

#include <Arduino.h>
#include "Valve_3_2_interface.h"

class Valve_3_2 : public Valve_3_2_interface
{

private:

public:
    Valve_3_2(byte _pin_control);
    void L_way();
    void I_way();
    void switch_way();
    bool get_state();
};

#endif
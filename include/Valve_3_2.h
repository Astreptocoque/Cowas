#ifndef VALVE_3_2_H
#define VALVE_3_2_H

#include <Arduino.h>
#include "interfaces/Valve_3_2_interface.h"

class Valve_3_2 : public Valve_3_2_interface
{

private:

public:
    void begin(byte _pin_control);
    void set_L_way();
    void set_I_way();
    void switch_way();
    valve_3_2_state get_state();
};

#endif
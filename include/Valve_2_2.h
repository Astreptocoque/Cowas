#ifndef VALVE_2_2_H
#define VALVE_2_2_H

#include <Arduino.h>
#include "interfaces/Valve_2_2_interface.h"

class Valve_2_2 : public Valve_2_2_interface
{

private:

public:
    void begin(byte _pin_control, String _name);
    void begin(byte _pin_control);
    void set_open_way();
    void set_close_way();
    void switch_way();
    valve_2_2_state get_state();
};

#endif
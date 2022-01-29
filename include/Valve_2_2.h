#ifndef VALVE_2_2_H
#define VALVE_2_2_H

#include <Arduino.h>

enum valve_2_2_state
{
    open_way,
    close_way
};

class Valve_2_2
{

private:
    int pin_control;
    valve_2_2_state state; // 1 open, 0 closed
    String ID = "no_ID";

public:
    void begin(byte _pin_control, String _name);
    void begin(byte _pin_control);
    void set_open_way();
    void set_close_way();
    void switch_way();
    valve_2_2_state get_state();
};

#endif
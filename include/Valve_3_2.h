#ifndef VALVE_3_2_H
#define VALVE_3_2_H

#include <Arduino.h>

enum valve_3_2_state
{
    L_way,
    I_way
};

class Valve_3_2
{

private:
    int pin_control;
    valve_3_2_state state; // 1 I way, 0 L way
    String ID = "no_ID";
    valve_3_2_state off_state;

public:
    void begin(byte _pin_control, String _name, valve_3_2_state off_state_ = L_way);
    void begin(byte _pin_control, valve_3_2_state off_state_ = L_way);
    void set_L_way();
    void set_I_way();
    void set_off();
    void switch_way();
    valve_3_2_state get_state();
};

#endif
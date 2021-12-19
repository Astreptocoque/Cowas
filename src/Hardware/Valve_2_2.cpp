#include "Valve_2_2.h"

/**
 * @brief Constructor for a valve 2 port 2 ways
 * @param pin_control Output connection on the board
 *
 */
void Valve_2_2::begin(byte _pin_control, String _ID)
{
    ID = _ID;
    pin_control = _pin_control;
    pinMode(pin_control, OUTPUT);
    set_close_way();
}

void Valve_2_2::begin(byte _pin_control)
{
    pin_control = _pin_control;
    pinMode(pin_control, OUTPUT);
    set_close_way();
}

void Valve_2_2::set_open_way(){
    digitalWrite(pin_control, HIGH);
    state = open_way;
    Valve_2_2_interface::set_open_way();
}
void Valve_2_2::set_close_way(){
    digitalWrite(pin_control, LOW);
    state = close_way;
    Valve_2_2_interface::set_close_way();
}
void Valve_2_2::switch_way()
{
    if(state == open_way) 
        set_close_way();
    else
        set_open_way();
}
valve_2_2_state Valve_2_2::get_state(){
    return state;
}

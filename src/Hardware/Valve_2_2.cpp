#include "Valve_2_2.h"

/**
 * @brief Constructor for a valve 2 port 2 ways
 * @param pin_control Output connection on the board
 *
 */
void Valve_2_2::begin(byte _pin_control)
{
    pin_control = _pin_control;
    pinMode(pin_control, OUTPUT);
    set_close_way();
}

void Valve_2_2::set_open_way(){
    digitalWrite(pin_control, HIGH);
    state = open_way;
}
void Valve_2_2::set_close_way(){
    digitalWrite(pin_control, LOW);
    state = close_way;
}
void Valve_2_2::switch_way()
{
    if(state == open_way) state = close_way;
    else state = open_way;
    digitalWrite(pin_control, state);
}
valve_2_2_state Valve_2_2::get_state(){
    return state;
}

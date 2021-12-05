#include "Valve_2_2.h"

/**
 * @brief Constructor for a valve 2 port 2 ways
 * @param pin_control Output connection on the board
 *
 */
Valve_2_2::Valve_2_2(byte _pin_control)
{
    pin_control = _pin_control;
    pinMode(pin_control, OUTPUT);
}

void Valve_2_2::open_way(){
    digitalWrite(pin_control, HIGH);
}
void Valve_2_2::close_way(){
    digitalWrite(pin_control, LOW);
}
void Valve_2_2::switch_way()
{
    state = get_state()?0:1;
    digitalWrite(pin_control, state);
}
bool Valve_2_2::get_state(){
    return state;
}

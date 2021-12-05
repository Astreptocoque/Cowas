#include "Valve_3_2.h"

/**
 * @brief Constructor for a valve 2 port 3 ways
 * @param pin_control Output connection on the board
 *
 */
Valve_3_2::Valve_3_2(byte _pin_control)
{
    pin_control = _pin_control;
    pinMode(pin_control, OUTPUT);
}

void Valve_3_2::L_way(){
    digitalWrite(pin_control, LOW);
}
void Valve_3_2::I_way(){
    digitalWrite(pin_control, HIGH);
}
void Valve_3_2::switch_way()
{
    state = get_state()?0:1;
    digitalWrite(pin_control, state);
}
bool Valve_3_2::get_state(){
    return state;
}

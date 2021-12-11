#include "Valve_3_2.h"



/**
 * @brief Constructor for a valve 2 port 3 ways
 * @param pin_control Output connection on the board
 *
 */
void Valve_3_2::begin(byte _pin_control, String _name)
{
    name = _name;
    pin_control = _pin_control;
    pinMode(pin_control, OUTPUT);
    set_L_way();
}

void Valve_3_2::begin(byte _pin_control)
{
    pin_control = _pin_control;
    pinMode(pin_control, OUTPUT);
    set_L_way();
}

void Valve_3_2::set_L_way(){
    digitalWrite(pin_control, LOW);
    state = L_way;
    Valve_3_2_interface::set_L_way();

}
void Valve_3_2::set_I_way(){
    digitalWrite(pin_control, HIGH);
    state = I_way;
    Valve_3_2_interface::set_I_way();
}
void Valve_3_2::switch_way()
{
    if(state == L_way) state = I_way; else state = L_way;
    digitalWrite(pin_control, state);
    Valve_3_2_interface::switch_way();
}
valve_3_2_state Valve_3_2::get_state(){
    return state;
}

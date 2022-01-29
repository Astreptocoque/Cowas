/**
 * @file Valve_3_2.cpp
 * @author Timothée Hirt
 * @brief Class for valve 3/2 from SMC
 * @version 0.1
 * @date 2022-01-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Valve_3_2.h"
#include "C_output.h"

extern C_output output;

void Valve_3_2::begin(byte _pin_control)
{
    pin_control = _pin_control;
    pinMode(pin_control, OUTPUT);
    set_L_way(); // normally closed on L way
    output.println("Valve " + ID + " initiated");
}

/**
 * @brief Constructor for a valve 3 port 2 ways
 * @param pin_control Output connection on the board
 *
 */
void Valve_3_2::begin(byte _pin_control, String _ID)
{
    ID = _ID;
    begin(_pin_control);
}

void Valve_3_2::set_L_way()
{
    digitalWrite(pin_control, LOW);
    state = L_way;
    output.println("Valve " + ID + " on L way");
}

void Valve_3_2::set_I_way()
{
    digitalWrite(pin_control, HIGH);
    state = I_way;
    output.println("Valve " + ID + " on I way");
}

void Valve_3_2::switch_way()
{
    if (state == L_way)
        set_I_way();
    else
        set_L_way();
}

valve_3_2_state Valve_3_2::get_state()
{
    return state;
}

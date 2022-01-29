/**
 * @file GPIO.cpp
 * @author Timothée Hirt
 * @brief Class for basic GPIO, such as direct line or LED
 * @version 0.1
 * @date 2022-01-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>
#include "GPIO.h"
#include "Serial_output.h"

extern Serial_output output;

void GPIO::begin(byte _pin, byte _mode)
{
    pin = _pin;
    mode = _mode;
    pinMode(pin, (_mode == input_mode ? INPUT : OUTPUT));
    if (mode == output_mode)
        off();
    output.println("GPIO " + ID + " initiated");
}

void GPIO::begin(byte _pin, byte _mode, String _ID)
{
    ID = _ID;
    begin(_pin, _mode);
}

void GPIO::on()
{
    if (mode == output_mode)
    {
        digitalWrite(pin, HIGH);
        state = 1;
    }
}

void GPIO::off()
{
    if (mode == output_mode)
    {
        digitalWrite(pin, LOW);
        state = 0;
    }
}

void GPIO::switch_state()
{
    if (mode == output_mode)
    {
        state = !state;
        digitalWrite(pin, state);
    }
}

bool GPIO::read()
{
    if (mode == input_mode)
        state = digitalRead(pin);

    return state;
}
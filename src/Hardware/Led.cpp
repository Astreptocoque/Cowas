#include <Arduino.h>
#include "Led.h"
#include "Serial_output.h"

extern Serial_output output;

void Led::begin(byte _control_pin)
{
    control_pin = _control_pin;
    pinMode(control_pin, OUTPUT);
    off();
    output.println("LED " + ID + " initiated");
}

void Led::begin(byte _control_pin, String _ID)
{
    ID = _ID;
    begin(_control_pin);
}

void Led::on()
{
    digitalWrite(control_pin, HIGH);
    state = 1;
}

void Led::off()
{
    digitalWrite(control_pin, LOW);
    state = 0;
}

void Led::switch_state()
{
    state = !state;
    digitalWrite(control_pin, state);
}
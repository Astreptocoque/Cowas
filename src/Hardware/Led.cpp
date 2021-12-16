#include <Arduino.h>
#include "Led.h"

void Led::begin(byte _control_pin)
{
    control_pin = _control_pin;
    pinMode(control_pin, OUTPUT);
    off();
}

void Led::begin(byte _control_pin, String _ID)
{
    ID = _ID;
    control_pin = _control_pin;
    pinMode(control_pin, OUTPUT);
    off();
}

void Led::on()
{
    digitalWrite(control_pin, HIGH);
}

void Led::off()
{
    digitalWrite(control_pin, LOW);
}
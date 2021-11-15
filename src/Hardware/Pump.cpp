#include "Pump.h"

/**
 * @brief Constructor for a pressure sensor
 * @param pin Input connection on the board
 *
 */
Pump::Pump(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Pump::init()
{
    pinMode(pin, INPUT);
    update();
}

void Pump::update()
{
    // You can handle the debounce of the button directly
    // in the class, so you don't have to think about it
    // elsewhere in your code
    byte newReading = digitalRead(pin);

    if (newReading != lastReading)
    {
        lastDebounceTime = millis();
    }
    if (millis() - lastDebounceTime > debounceDelay)
    {
        // Update the 'state' attribute only if debounce is checked
        state = newReading;
    }
    lastReading = newReading;
}
byte Pump::getState()
{
    update();
    return state;
}
bool Pump::isPressed()
{
    return (getState() == HIGH);
}

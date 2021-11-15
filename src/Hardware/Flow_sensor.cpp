#include "Flow_sensor.h"

/**
 * @brief Constructor for a pressure sensor
 * @param pin Input connection on the board
 *
 */
Flow_sensor::Flow_sensor(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Flow_sensor::init()
{
    pinMode(pin, INPUT);
    update();
}

void Flow_sensor::update()
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
byte Flow_sensor::getState()
{
    update();
    return state;
}
bool Flow_sensor::isPressed()
{
    return (getState() == HIGH);
}

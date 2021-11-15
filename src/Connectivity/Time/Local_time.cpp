#include "Local_time.h"

/**
 * @brief Constructor for a Local_time
 * @param pin Input connection on the board
 *
 */
Local_time::Local_time(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Local_time::init()
{
    pinMode(pin, INPUT);
    update();
}

void Local_time::update()
{
    // You can handle the debounce of the Local_time directly
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
byte Local_time::getState()
{
    update();
    return state;
}
bool Local_time::isPressed()
{
    return (getState() == HIGH);
}

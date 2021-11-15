#include "Internet_time.h"

/**
 * @brief Constructor for a Internet_time
 * @param pin Input connection on the board
 *
 */
Internet_time::Internet_time(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Internet_time::init()
{
    pinMode(pin, INPUT);
    update();
}

void Internet_time::update()
{
    // You can handle the debounce of the Internet_time directly
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
byte Internet_time::getState()
{
    update();
    return state;
}
bool Internet_time::isPressed()
{
    return (getState() == HIGH);
}

#include "Time_cowas.h"

/**
 * @brief Constructor for a Time_cowas
 * @param pin Input connection on the board
 *
 */
Time_cowas::Time_cowas(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Time_cowas::init()
{
    pinMode(pin, INPUT);
    update();
}

void Time_cowas::update()
{
    // You can handle the debounce of the Time_cowas directly
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
byte Time_cowas::getState()
{
    update();
    return state;
}
bool Time_cowas::isPressed()
{
    return (getState() == HIGH);
}

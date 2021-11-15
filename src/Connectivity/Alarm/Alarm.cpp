#include "Alarm.h"

/**
 * @brief Constructor for a Alarm
 * @param pin Input connection on the board
 *
 */
Alarm::Alarm(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Alarm::init()
{
    pinMode(pin, INPUT);
    update();
}

void Alarm::update()
{
    // You can handle the debounce of the Alarm directly
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
byte Alarm::getState()
{
    update();
    return state;
}
bool Alarm::isPressed()
{
    return (getState() == HIGH);
}

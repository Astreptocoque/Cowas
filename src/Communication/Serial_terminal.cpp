#include "Serial_terminal.h"

/**
 * @brief Constructor for a Serial_terminal
 * @param pin Input connection on the board
 *
 */
Serial_terminal::Serial_terminal(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Serial_terminal::init()
{
    pinMode(pin, INPUT);
    update();
}

void Serial_terminal::update()
{
    // You can handle the debounce of the Serial_terminal directly
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
byte Serial_terminal::getState()
{
    update();
    return state;
}
bool Serial_terminal::isPressed()
{
    return (getState() == HIGH);
}

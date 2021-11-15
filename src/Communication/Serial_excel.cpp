#include "Serial_excel.h"

/**
 * @brief Constructor for a Serial_excel
 * @param pin Input connection on the board
 *
 */
Serial_excel::Serial_excel(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Serial_excel::init()
{
    pinMode(pin, INPUT);
    update();
}

void Serial_excel::update()
{
    // You can handle the debounce of the Serial_excel directly
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
byte Serial_excel::getState()
{
    update();
    return state;
}
bool Serial_excel::isPressed()
{
    return (getState() == HIGH);
}

#include "Button.h"

/**
 * @brief Constructor for a button
 * @param _input_pin Input connection on the board
 *
 */
void Button::begin(byte _input_pin)
{
    input_pin = _input_pin;
    pinMode(input_pin, INPUT);
    update();
}
void Button::begin(byte _input_pin, String _ID)
{
    ID = _ID;
    begin(_input_pin);
}

void Button::update()
{
    // You can handle the debounce of the button directly
    // in the class, so you don't have to think about it
    // elsewhere in your code
    // unsigned long timeCurrent = millis();
    bool looping = true;
    while (looping)
    {
        byte newReading = digitalRead(input_pin);

        if (newReading != lastReading)
        {
            lastDebounceTime = millis();
        }
        if (millis() - lastDebounceTime > debounceDelay)
        {
            // Update the 'state' attribute only if debounce is checked
            state = newReading;
            looping = false;
        }
        lastReading = newReading;
    }
}

byte Button::getState()
{
    update();
    return state;
}

bool Button::isPressed()
{
    return (getState() == HIGH);
}

bool Button::isReleased()
{
    return (getState() == LOW);
}

void Button::waitPressedAndReleased()
{
    while (!isPressed())
    {
        delay(5);
    }
    while (isPressed())
    {
        delay(5);
    }
}

void Button::waitReleasedAndPressed()
{
    while (isPressed())
    {
        delay(5);
    }
    while (!isPressed())
    {
        delay(5);
    }
}
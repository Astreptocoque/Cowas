#include "Wifi_ESP8266.h"

/**
 * @brief Constructor for a Wifi_ESP8266
 * @param pin Input connection on the board
 *
 */
Wifi_ESP8266::Wifi_ESP8266(byte pin)
{
    this->pin = pin;
    lastReading = LOW;
    init();
}

void Wifi_ESP8266::init()
{
    pinMode(pin, INPUT);
    update();
}

void Wifi_ESP8266::update()
{
    // You can handle the debounce of the Wifi_ESP8266 directly
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
byte Wifi_ESP8266::getState()
{
    update();
    return state;
}
bool Wifi_ESP8266::isPressed()
{
    return (getState() == HIGH);
}

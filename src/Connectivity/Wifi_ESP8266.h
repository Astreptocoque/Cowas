#ifndef WIFI_ESP8266_H
#define WIFI_ESP8266_H

#include <Arduino.h>

class Wifi_ESP8266
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Wifi_ESP8266(byte pin);
    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif
#ifndef BUTTON_INTERFACE_H
#define BUTTON_INTERFACE_H

#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Button_interface
{
protected:
    byte input_pin;
    int state;
    String ID = "no_ID";

public:
    void begin(byte _input_pin)
    {
        output.println("Button " + ID + " initiated");
    }

    void begin(byte _input_pin, String _ID)
    {
        ID = _ID;
        output.println("Button " + ID + " initiated");

    }

    void update()
    {
        output.println("Button " + ID + " state updated");
    }

    byte getState()
    {
        output.println("Get button " + ID + " value");
        return 1;
    }

    bool isPressed()
    {
        output.println("Asked if button " + ID + " is pressed");
        return 1;
    }

    void waitPressedAndReleased()
    {
        output.println("Wait for button " + ID + " for pressed and released"); 
    }
};

#endif
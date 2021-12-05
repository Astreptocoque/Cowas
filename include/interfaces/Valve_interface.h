#ifndef Valve_interface_h
#define Valve_interface_h

#include "Serial_output.h"

extern Serial_output output;

class Valve_interface
{

protected:
    int pin_control;
    bool state; // 1 open, 0 closed

public:
    Valve_interface()
    {
        output.println("Valve initialized without port");
    }

    Valve_interface(byte _pin_control)
    {
        output.println("Valve created on port " + _pin_control);
    }

    virtual void switch_way()
    {
        output.println("Valve on way" + String((get_state()?1:0)));
        state = get_state() ? 0:1;
    }

    virtual bool get_state()
    {
        output.println("Get valve state");
        return state;
    }
};

#endif

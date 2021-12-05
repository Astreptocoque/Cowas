#ifndef VALVE_2_2_INTERFACE_H
#define VALVE_2_2_INTERFACE_H

#include "Serial_output.h"
#include "Valve_interface.h"

extern Serial_output output;

class Valve_2_2_interface : public Valve_interface
{

protected:
    int pin_slave_select;
    bool state; // 1 open, 0 closed

public:
    Valve_2_2_interface()
    {
        output.println("Valve initialized without port");
    }

    Valve_2_2_interface(byte _pin_control)
    {
        output.println("Valve created on port " + _pin_control);
        state = 0;
        output.println("Valve init closed state");
    }

    virtual void switch_way()
    {
        output.println("Valve " + String((get_state()?"open":"close")));
        state = get_state() ? 0:1;
    }

    virtual void open()
    {
        state = 1;
        output.println("Valve opened");
    }
    virtual void close()
    {
        state = 0;
        output.println("Valve closed");
    }

    virtual bool get_state()
    {
        output.println("Get valve state : " + String(state));
        return state;
    }
};

#endif

#ifndef VALVE_3_2_INTERFACE_H
#define VALVE_3_2_INTERFACE_H

#include "Serial_output.h"
#include "Valve_interface.h"

extern Serial_output output;

class Valve_3_2_interface : public Valve_interface
{

protected:
    int pin_slave_select;
    bool state; // 1 open, 0 closed

public:
    Valve_3_2_interface()
    {
        output.println("Valve initialized without port");
    }

    Valve_3_2_interface(byte _pin_control)
    {
        output.println("Valve created on port " + _pin_control);
        state = 0;
        output.println("Valve init I way");
    }

    virtual void switch_way()
    {
        output.println("Valve on " + String((get_state()?"I":"L")) + " way");
        state = get_state() ? 0:1;
    }

    virtual void L_way()
    {
        state = 0;
        output.println("Valve on L way - TO CHECK");
    }
    virtual void I_way()
    {
        state = 1;
        output.println("Valve on I way - TO CHECK");
    }

    virtual bool get_state()
    {
        output.println("Get valve state");
        return state;
    }
};

#endif

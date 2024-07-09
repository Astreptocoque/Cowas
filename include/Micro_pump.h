#ifndef MICRO_PUMP_H
#define MICRO_PUMP_H

#include <Arduino.h>

class Micro_Pump
{
private:
    byte control_pin;
    String ID = "no_ID";
    bool running = false;

public:
    void begin(byte _control_pin);
    void begin(byte _control_pin, String _name);
    bool get_state();
    void start();
    void start(uint32_t _time_ms);
    void stop();
};

#endif
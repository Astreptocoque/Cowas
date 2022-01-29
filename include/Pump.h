#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump
{

private:
    byte control_pin;
    uint8_t power; // refers to power
    uint8_t power_percent;
    bool pwm; // if pump is controlled by pwm or only HIGH/LOW
    String ID = "no_ID";
    bool running = false;

public:
    void begin(byte _control_pin, bool _pwm);
    void begin(byte _control_pin, bool _pwm, String _name);
    void set_flow(int _flow);
    void set_power(int8_t _power);
    uint8_t get_power();
    void start();
    void start(uint32_t _time_ms);
    void stop();
};

#endif
#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>
#include "MiniPID.h"
#include "Flow_sensor.h"
#include "Trustability_ABP_Gage.h"

extern MiniPID pump_pid;   // use this PID for pump, it is tuned for pressure control

class Pump
{
private:
    byte control_pin;
    byte enable_pin;
    uint8_t power; // refers to power
    uint8_t power_percent;
    bool pwm; // if pump is controlled by pwm or only HIGH/LOW
    String ID = "no_ID";
    bool running = false;

public:
    void begin(byte _control_pin, bool _pwm, byte _enable_pin = -1);
    void begin(byte _control_pin, bool _pwm, String _name, byte _enable_pin = -1);
    void set_flow(int _flow);
    void set_power(int8_t _power);
    uint8_t get_power();
    void start();
    void start(uint32_t _time_ms);
    void stop();
};

// --------------------- Classes to control the pump ------------------------------

// base class that makes sure the pressure doesn't get to high
class CtrlPump
{
public:
    void begin(Pump *pump, MiniPID *pid, Trustability_ABP_Gage *pressure_sens, float target_pressure = 2.5, bool print = false);
    void run();
    void set_max_pressure(float max_pressure){max_pressure_ = max_pressure;}
    void set_max_runtime(uint32_t max_runtime){max_runtime_ = max_runtime;}
    void set_end_cond_time(uint32_t time){time_end_valid_ = time;}
    void virtual reset(){;}
protected:
    virtual bool check_end_cond(){ return false;}
    Pump* pump_;
    MiniPID* pid_;
    Trustability_ABP_Gage* pres_sens_;
    
    uint32_t start_time_;
    uint32_t max_runtime_;   // security timeout in case condition is never mets
    float target_pressure_;
    float max_pressure_;     // set to global variable

    uint32_t time_first_cond_met_;
    uint32_t time_end_valid_;        // time end condition must be valid
    bool end_cond_met_;  // if end condition already met at last iteration
    void reset_end_cond();
    bool update_end_cond(bool new_end_cond);     // true if should stop

    bool print_;
};


class CtrlPumpNoWater : public CtrlPump
{
public:
    // constructor or begin
    void begin(Pump *pump, MiniPID *pid, Trustability_ABP_Gage *pressure_sens, float target_pressure = 2.5, bool print = false);
    void set_pressure_thresh(float pres_tresh){pressure_thresh_ = pres_tresh;}   // if not the default
    void reset(){filtered_pressure_ = 0;}
protected:
    bool check_end_cond();
    float pressure_thresh_;  // below it is considered to be air
    float filtered_pressure_;
    //Trustability_ABP_Gage* pressure_sens;       // already in parent class
};

class CtrlPumpFlow : public CtrlPump
{
public:
    // constructor or begin
    void begin(Pump *pump, MiniPID *pid, Trustability_ABP_Gage *pressure_sens, 
               Flow_sensor *flow_sensor, float mL_thresh, float target_pressure = 2.5, bool print = false);
    void reset();
protected:
    bool check_end_cond();
    float milliL_tresh_;
    Flow_sensor* flow_sensor_;
};



#endif  // PUMP_H
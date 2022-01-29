#include <Arduino.h>
#include "Pump.h"
#include "Trustability_ABP_Gage.h"
#include "Led.h"

extern Trustability_ABP_Gage pressure1;
extern Trustability_ABP_Gage pressure2;
extern Led status_led;
extern Pump pump;


/**
 * @brief Constructor for a diaphragm pump
 * @param _control_pin Output connection on the board
 * @param _pwm True if the control is PWM (analog pin). False if control is only HIGH/LOW (digital pin)
 *
 */
void Pump::begin(byte _control_pin, bool _pwm)
{
    control_pin = _control_pin;
    pwm = _pwm;
    pinMode(control_pin, OUTPUT);
    set_power(50);
    stop();
}

void Pump::begin(byte _control_pin, bool _pwm, String _ID)
{
    ID = _ID;
    begin(_control_pin, _pwm);
}

/**
 * @brief Not implemented. Possiblity to control flow if power is mapped correspondingly.
 * 
 * @param _flow ???
 */
void Pump::set_flow(int _flow){
    output.println("function -set_flow- not implemented");
}

/**
 * @brief Set pump power. Warning, on diaphragm pump, flow is not linear at all.
 * 
 * @param _power From 0 to 100
 */
void Pump::set_power(int8_t _power){
    // data validation
    if(_power > 100)
        _power = 100;
    if(_power < 0)
        _power = 0;

    power = map(_power, 0, 100, 0, 255);
    power_percent = _power;
    // if pump already ON, then update the power live
    if(running)
        pump.start();
}

/**
 * @brief Get current power set
 * 
 * @return int power between 0 to 100.
 */
uint8_t Pump::get_power(){
    return power_percent;
}

/**
 * @brief Start the pump with current power setting until stop command.
 * 
 */
void Pump::start(){
    if(pwm)
        analogWrite(control_pin, power);
    else
        digitalWrite(control_pin, HIGH);
   
    // display starting info only if previous state was OFF
    if(!running){
        Pump_interface::start();
        running = true;
    }
}

/**
 * @brief Start the pump with current power setting for a certain time, then stop
 * 
 * @param time_ms The time is millisecond to run the pump.
 */
void Pump::start(uint32_t _time_ms){
    start();
    uint32_t current_time = millis();
    while(millis() - current_time < _time_ms){
        delay(5);
    }
    stop();
}

/**
 * @brief Stop the pump.
 * 
 */
void Pump::stop(){
    if(pwm)
        analogWrite(control_pin, 0);
    else
        digitalWrite(control_pin, 0);
    running = false;
    Pump_interface::stop();   
}

/**
 * @brief Timer interrupt. Was implemented but now not used anymore.
 * 
 */
void TC3_Handler(){
    // You must do TC_GetStatus to "accept" interrupt
    // As parameters use the first two parameters used in startTimer (TC1, 0 in this case)
    TC_GetStatus(TC1, 0);

    // disable all text output
    ENABLE_OUTPUT = false;
   
    // TODO : Raise flag

    status_led.switch_state();
    
    //enable all text output back
    ENABLE_OUTPUT = true;
}

/**
 * @brief Timer interrupt. Was implemented but now not used anymore.
 * 
 */
void TC4_Handler(){
    // You must do TC_GetStatus to "accept" interrupt
    // As parameters use the first two parameters used in startTimer (TC1, 0 in this case)
    TC_GetStatus(TC1, 1);

    // disable all text output
    ENABLE_OUTPUT = false;
   
    // TODO : raise flag

    status_led.switch_state();

    //enable all text output back
    ENABLE_OUTPUT = true;
}
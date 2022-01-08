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
 * @param control_pin Output connection on the board
 *
 */
void Pump::begin(byte _control_pin, bool _pwm)
{
    control_pin = _control_pin;
    pwm = _pwm;
    pinMode(control_pin, INPUT);
    set_power(190); // after 190, flow doesn't increase
    stop();
}

void Pump::begin(byte _control_pin, bool _pwm, String _ID)
{
    ID = _ID;
    control_pin = _control_pin;
    pwm = _pwm;
    pinMode(control_pin, OUTPUT);
    set_power(190);
    stop();
}

void Pump::set_flow(int _flow){
    output.println("function -set_flow- not implemented");
}

void Pump::set_power(int _power){
    power = map(_power, 0, 100, 0, 255);
}

int Pump::get_power(){
    return power;
}

void Pump::start(){
    if(pwm)
        analogWrite(control_pin, power);
    else
        digitalWrite(control_pin, HIGH);
    Pump_interface::start();
}

void Pump::start(uint32_t time_ms){
    start();
    uint32_t current_time = millis();
    while(millis() - current_time < time_ms){
        delay(5);
    }
    stop();
}

void Pump::stop(){
    if(pwm)
        analogWrite(control_pin, 0);
    else
        digitalWrite(control_pin, 0);
    Pump_interface::stop();   
}

/**
 * @brief Pressure checking, every 1 seconds
 *        System function, place wherever
 *        No need to call
 * 
 */
void TC3_Handler(){
    TC_GetStatus(TC1, 0);
    // print if too high is included in .getPressure()
    float pressure = pressure1.getPressure();
    // output.println(pressure);
    if(pressure > pressure1.getMaxPressure()){
        // simple decremental function to reduce pressure
        pump.set_power(pump.get_power() - 5);
    }
    // pressure = pressure2.getPressure();
    // if(pressure > pressure2.getMaxPressure()){
    //     output.println("Pressure to high on sensor " + pressure1.getID() + " : " + pressure + "bar");
    // }

    status_led.switch_state();
}

void TC4_Handler(){
    TC_GetStatus(TC1, 0);
    // print if too high is included in .getPressure()
    float pressure = pressure2.getPressure();
    // output.println(pressure);
    if(pressure > pressure2.getMaxPressure()){
        // simple decremental function to reduce pressure
        pump.set_power(pump.get_power() - 5);
    }
    // pressure = pressure2.getPressure();
    // if(pressure > pressure2.getMaxPressure()){
    //     output.println("Pressure to high on sensor " + pressure1.getID() + " : " + pressure + "bar");
    // }

    status_led.switch_state();
}
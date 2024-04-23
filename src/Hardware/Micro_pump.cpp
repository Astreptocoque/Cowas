#include <Arduino.h>
#include "Micro_pump.h"
#include "Led.h"
#include "C_output.h"

extern C_output output;
extern Micro_Pump micro_pump;

/**
 * @brief Constructor for a micro pump, just ON/OFF
 * @param _control_pin Output connection on the board
 *
 */
void Micro_Pump::begin(byte _control_pin)
{
    control_pin = _control_pin;
    pinMode(control_pin, OUTPUT);
    stop();
    output.println("Micro Pump " + ID + " initiated");
}

/**
 * @brief Constructor for a micro pump, just ON/OFF
 * @param _control_pin Output connection on the board
 * @param _ID Name of hardware component used for outputs
 *
 */
void Micro_Pump::begin(byte _control_pin, String _ID)
{
    ID = _ID;
    begin(_control_pin);
}

/**
 * @brief Start the pump with current power setting until stop command.
 *
 */
void Micro_Pump::start()
{
    digitalWrite(control_pin, HIGH);

    // display starting info only if previous state was OFF
    if (!running)
    {
        if(VERBOSE_PUMP){output.println("Micro Pump " + ID + " started ");}
        running = true;
    }
}

/**
 * @brief Start the pump for a certain time, then stop
 *
 * @param time_ms The time is millisecond to run the pump.
 */
void Micro_Pump::start(uint32_t _time_ms)
{
    start();
    uint32_t current_time = millis();
    while (millis() - current_time < _time_ms)
    {
        delay(5);
    }
    stop();
}

/**
 * @brief Stop the pump.
 *
 */
void Micro_Pump::stop()
{
    digitalWrite(control_pin, 0);
    running = false;
    if(VERBOSE_PUMP){output.println("Micro Pump " + ID + " stopped");}
}



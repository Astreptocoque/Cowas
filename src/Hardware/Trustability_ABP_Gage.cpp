#include "Trustability_ABP_Gage.h"
#include <SPI.h>
#include <Arduino.h>


#define BAR_FACTORa 7.63e-4
#define BAR_FACTORb 1.25
#define TEMP_FACTOREa 9.77e-2
#define TEMP_FACTOREb 50

/**
 * @brief Constructor for a pressure sensor
 * @param pin Input connection on the board
 *
 */
void Trustability_ABP_Gage::begin(byte _pin_slave_select, float _max_pressure)
{
    pin_slave_select = _pin_slave_select;
    pinMode(pin_slave_select, OUTPUT);
    max_pressure = _max_pressure;
    SPIPressure = new SPISettings(800000, MSBFIRST, SPI_MODE0);
}

void Trustability_ABP_Gage::begin(byte _pin_slave_select, float _max_pressure, String _ID)
{
    ID = _ID;
    begin(_pin_slave_select, _max_pressure);
}

/**
 * @brief Pressure with reference 0 at atmospheric pressure
 * 
 */
void Trustability_ABP_Gage::read()
{
    // SPISettings SPIPressure(800000, MSBFIRST, SPI_MODE0);

    // SPI communication API
    SPI.beginTransaction(*SPIPressure);
    digitalWrite(pin_slave_select, LOW);
    byte byte1 = SPI.transfer(0); // control & pressure 14 bits
    byte byte2 = SPI.transfer(0); // pressure 14 bits
    byte byte3 = SPI.transfer(0); // temperature 8 bits
    digitalWrite(pin_slave_select, HIGH);
    SPI.endTransaction();

    // filter non valid data of pressure sensor (see datasheet)
    // two first bits should be 00
    if (!(byte1 & B11000000))
    {
        // output of pressure sensor in bits
        // concatenate data byte into one 16 byte
        float output_pressure = (byte1 << 8) + byte2;
        float output_temp = (byte3 << 3) + B000;

        pressure = output_pressure * BAR_FACTORa - BAR_FACTORb;
        temperature = output_temp * TEMP_FACTOREa - TEMP_FACTOREb;
        if(pressure > max_pressure){
            output.println("ERROR | Pressure to high on sensor " + ID + " (" + String(pressure) + " bar)");
        }
    }
    else
    {
        output.println("error pressure reading");
    }
}

float Trustability_ABP_Gage::getPressure()
{
    read();
    return pressure;
}

float Trustability_ABP_Gage::getTemperature()
{
    read();
    return temperature;
}

float Trustability_ABP_Gage::getMaxPressure()
{
    return max_pressure;
}

String Trustability_ABP_Gage::getID()
{
    return ID;
}

void ISR_pressure_checking(){

}


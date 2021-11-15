#include "Pressure_sensor.h"
#include <SPI.h>

#define BAR_FACTORa 4e-4 
#define BAR_FACTORb 1.292

/**
 * @brief Constructor for a pressure sensor
 * @param pin Input connection on the board
 *
 */
Pressure_sensor::Pressure_sensor(byte _pin_slave_select)
{
    pin_slave_select = _pin_slave_select;
    pinMode(pin_slave_select, OUTPUT);
    // SPIPressure = new SPISettings(800000, MSBFIRST, SPI_MODE1);
}


void Pressure_sensor::readPressure()
{
    SPISettings SPIPressure(800000, MSBFIRST, SPI_MODE1);
    
    // SPI communication API
    SPI.beginTransaction(SPIPressure);
    digitalWrite(pin_slave_select, LOW);
    byte byte1 = SPI.transfer(0);
    byte byte2 = SPI.transfer(0);
    byte byte3 = SPI.transfer(0);
    digitalWrite(pin_slave_select, HIGH);
    SPI.endTransaction();

     // filter non valid data of pressure sensor (see datasheet)
    // two first bits should be 00
    if (!(byte1 & B11000000))
    {
        // output of pressure sensor in bits
        // concatenate data byte into one 16 byte
        float output_pressure = (byte1 << 8) + byte2; // 
        float output_temp = (byte3 << 3) + B000; // transform binary to decimal
       // Serial.println((byte3), BIN);
       // Serial.println(output_temp, BIN);

        pressure = ((output_pressure * BAR_FACTORa - BAR_FACTORb));
        temperature = output_temp*200/2047-50;

    }else{
        Serial.print("error pressure reading");
    }

}

float Pressure_sensor::getPressure()
{
    return pressure;
}

float Pressure_sensor::getTemperature(){
    return temperature;
}

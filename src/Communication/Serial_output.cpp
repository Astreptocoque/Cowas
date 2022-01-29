#include <Arduino.h>
#include "Serial_output.h"
#include <SD.h>
#include "Settings.h"
#include <TimeLib.h>
#include <SPI.h>

/**
 * @brief Choose output of arduino
 * @param _serial_type Terminal, SDCard or both
 *
 */
void Serial_output::begin(serial_type_enum _serial_type)
{

    serial_type = _serial_type;
    bool validated = false;
    if (serial_type == terminal || serial_type == terminalANDsdCard)
    {
        Serial.begin(9600);
        delay(100);
        Serial.println("Serial initialized");
    }
    if (serial_type == sdCard || serial_type == terminalANDsdCard)
    {
        validated = SD.begin(SD_CARD_SS_PIN);
        delay(100);
        if (validated)
            Serial.println("SD card initialized");
        else
            Serial.println("SD card failed initalization");
    }
}

/**
 * @brief Read a single number between 0 and 9
 * 
 * @return int 
 */
int Serial_output::read()
{
    int data;
    switch (serial_type)
    {
    case terminal:
        data = Serial.read();
        if (data >= 48 && data <= 57)
            data = data - 48;
        else
            data = 0;
        break;
    case sdCard:
        data = 0;
        break;
    case terminalANDsdCard:
        break;
    default:
        data = 0;
    }
    return data;
}

/**
 * @brief True if data is in the serial buffer.
 * 
 * @return int 
 */
int Serial_output::available()
{
    int data;
    switch (serial_type)
    {
    case terminal:
        data = Serial.available();
        break;
    case sdCard:
        break;
    case terminalANDsdCard:
        data = Serial.available();
        break;
    default:
        data = 0;
    }
    return data;
}

serial_type_enum Serial_output::get_serial_type()
{
    return serial_type;
}

int Serial_output::waitInt()
{
    int data = 0;
    if (serial_type == terminal)
    {
        while (Serial.available() == 0)
        {
            delay(10);
        }
        data = read();
    }
    return data;
}

/**
 * @brief Wait until all serial buffer is sent
 *
 */
void Serial_output::flush()
{
    if (serial_type == terminal || serial_type == terminalANDsdCard)
    {
        Serial.flush();
    }
}
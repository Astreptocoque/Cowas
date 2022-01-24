#ifndef SERIAL_OUTPUT_H
#define SERIAL_OUTPUT_H

using namespace std;

#include <Arduino.h>
#include <SD.h>
#include "Settings.h"
#include <TimeLib.h>
#include <SPI.h>

enum serial_type_enum
{
    terminal,
    sdCard,
    terminalANDsdCard
};

class Serial_output
{

private:
    serial_type_enum serial_type;
    File dataFile;

public:
    /**
 * @brief Choose output of arduino
 * @param _serial_type Terminal, SDCard or both
 *
 */
    void begin(serial_type_enum _serial_type)
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

    // enable to create multiple declaration from the code types used
    // templates prohibit to create a declaration and implementation files.
    // compiler takes it into account
    template <typename T>
    void print(T output)
    {
        if(ENABLE_OUTPUT){
            if (serial_type == terminal || serial_type == terminalANDsdCard)
                Serial.print(output);

            if (serial_type == sdCard || serial_type == terminalANDsdCard)
            {
                dataFile = SD.open("datalog.txt", FILE_WRITE);
                if (dataFile)
                {
                    dataFile.print(String(output));
                    dataFile.close();
                }
            }
        }
    }

    template <typename T>
    void println(T output)
    {
        if(ENABLE_OUTPUT){
            // add time before printing in format yyyy-mm-dd hh:mm:ss
            String data;
            if (ENABLE_TIME_LOG)
                data = format_date_logging(now()) + "   " + String(output);
            else
                data = output;


            if (serial_type == terminal || serial_type == terminalANDsdCard)
                Serial.println(data);

            if (serial_type == sdCard || serial_type == terminalANDsdCard)
            {
                
                dataFile = SD.open("datalog.txt", FILE_WRITE);
                if (dataFile)
                {
                    dataFile.println(String(data));
                    dataFile.close();
                }
            }
        }
    }

    int read()
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

    int available()
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
            break;
        default:
            data = 0;
        }
        return data;
    }

    serial_type_enum get_serial_type()
    {
        return serial_type;
    }

    int waitInt()
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
    void flush()
    {
        if (serial_type == terminal || serial_type == terminalANDsdCard)
        {
            Serial.flush();
        }
    }
};



#endif
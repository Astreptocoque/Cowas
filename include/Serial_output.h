#ifndef SERIAL_OUTPUT_H
#define SERIAL_OUTPUT_H

using namespace std;

#include <Arduino.h>
#include <SD.h>
#define PIN_SS_SD_CARD 4

enum serial_type_enum
{
    terminal,
    sdCard,
    both
};

class Serial_output
{

private:
    serial_type_enum serial_type;
    File dataFile;

public:
    // Serial_output(uint8_t _serial_type);
    // template<typename T>
    // void print(T output, bool line_break);
    // template<typename T>
    // void Serial_output::print(T output);
    // template<typename T>
    // void Serial_output::println(T output);

    /**
 * @brief Choose output of arduino
 * @param _serial_type 1 is serial monitor, 2 is sd card
 *
 */
    void begin(serial_type_enum _serial_type)
    {
                    
        int i = 0;
        serial_type = _serial_type;
        bool validated = false;
        switch (serial_type)
        {
        case terminal: // serial monitor
            Serial.begin(9600);
            delay(100);
            Serial.println("Serial initialized");
            break;
        case sdCard:
            validated = SD.begin(PIN_SS_SD_CARD);
            delay(100);
            if (validated)
            {
                Serial.println("SD card initialized");
            }
            else
            {
                Serial.println("SD card failed initalization");
            }
            break;
        case both:
            Serial.begin(9600);
            delay(100);
            Serial.println("Serial initialized");
            while (i < 3 || validated)
            {
                validated = SD.begin(PIN_SS_SD_CARD);
                delay(100);
                if (validated)
                {
                    Serial.println("SD card initialized");
                }
                else
                {
                    Serial.println("SD card failed initalization");
                    Serial.println("Try " + String(i+1));
                    i++;
                }
            }
            break;
        default:
            Serial.begin(9600);
            Serial.println("Error initalizing serial");
            break;
        }
    }

    template <typename T>
    void print(T output)
    {
        switch (serial_type)
        {
        case terminal:
            Serial.print(output);
            break;
        case sdCard:
            dataFile = SD.open("datalog.txt", FILE_WRITE);
            if (dataFile)
            {
                dataFile.print(String(output));
                dataFile.close();
            }
            break;
        case both:
            dataFile = SD.open("datalog.txt", FILE_WRITE);
            if (dataFile)
            {
                dataFile.print(String(output));
                dataFile.close();
            }
            Serial.print(output);
            break;
        default:
            break;
        }
    }

    template <typename T>
    void println(T output)
    {
        switch (serial_type)
        {
        case terminal:
            Serial.println(output);
            break;
        case sdCard:
            dataFile = SD.open("datalog.txt", FILE_WRITE);
            if (dataFile)
            {
                dataFile.println(String(output));
                dataFile.close();
            }
            break;
        case both:
            dataFile = SD.open("datalog.txt", FILE_WRITE);
            if (dataFile)
            {
                dataFile.println(String(output));
                dataFile.close();
            }
            Serial.println(output);
            break;
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
        case both:
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
        case both:
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

    void flush()
    {
        if (serial_type == terminal)
        {
            Serial.flush();
        }
    }
};

#endif
#ifndef C_OUTPUT_H
#define C_OUTPUT_H

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

class C_output
{

private:
    serial_type_enum serial_type;
    File dataFile;

public:
    void begin(serial_type_enum _serial_type);

    template <typename T>
    void print(T output);

    template <typename T>
    void println(T output);

    int read();
    int available();
    int waitInt();
    void flush();
    serial_type_enum get_serial_type();
};

/**
 * @brief enable to create multiple declaration from the code types used.
 *        Templates prohibit to create a declaration and implementation files.
 *        Therefor function must be implemented here in .h.
 *        Compiler takes it into account
 * 
 * @param output The <typename T> data to print
 * @return template <typename T> 
 */
template <typename T>
void C_output::print(T output)
{
    if (ENABLE_OUTPUT)
    {
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

/**
 * @brief enable to create multiple declaration from the code types used.
 *        Templates prohibit to create a declaration and implementation files.
 *        Therefor function must be implemented here in .h.
 *        Compiler takes it into account
 * 
 * @param output The <typename T> data to print
 * @return template <typename T> 
 */
template <typename T>
void C_output::println(T output)
{
    if (ENABLE_OUTPUT)
    {
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

#endif
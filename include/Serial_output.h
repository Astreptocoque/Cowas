#ifndef SERIAL_OUTPUT_H
#define SERIAL_OUTPUT_H

using namespace std;

#include <Arduino.h>

class Serial_output
{

private:
    int8_t serial_type;

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

void begin(uint8_t _serial_type){
    serial_type = _serial_type;
    digitalWrite(LED_BUILTIN, HIGH);
    switch(serial_type){
        case 1: // serial monitor
            Serial.begin(9600);
            delay(100);
            Serial.println("Serial initialized");
            break;
        default:
            Serial.begin(9600);
            Serial.println("Error initalizing serial");
    }
}

template<typename T>
void print(T output, bool line_break)
{
    switch(serial_type){
        case 1:
            if(line_break){
                Serial.println(output);
            }else{
                Serial.print(output);
            }
            break;
    }
}

template<typename T>
void print(T output)
{
    switch(serial_type){
        case 1:
            Serial.print(output);
            break;
    }
}

template<typename T>
void println(T output)
{
    switch(serial_type){
        case 1:
            Serial.println(output);
            break;
    }
}




};

#endif
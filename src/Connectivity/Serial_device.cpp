#include "Serial_device.h"
#include "Serial_output.h"
#include "Settings.h"

extern Serial_output output;

void Serial_device::begin(){
    Serial1.begin(115200);
    delay(100);
    output.println("Serial with device initialised");
}

void Serial_device::start_communication(){
    Serial1.print("1");
    while(Serial1.available() == 0);
    int data = Serial.parseInt();
    output.println(data);
    output.println(Serial.read());
    if(data != 2){
        output.println("Serial with esp8266 failed");
    }
    Serial1.print("3");
}

struct Date Serial_device::receive_time(){

    while(true){
        if(Serial1.available() > 0){
            Serial.println(Serial1.parseInt());
        }
    }

    struct Date date;
    while(Serial1.available() == 0);
    date.time.hour = Serial1.parseInt();
    while(Serial1.available() == 0);
    date.time.minutes = Serial1.parseInt();
    while(Serial1.available() == 0);
    date.day = Serial1.parseInt(); 
    return date;
}

void Serial_device::validate(){
    Serial1.print("1");
    while(Serial.available() == 0);
    if(Serial.parseInt() != 1){
        output.println("Failed serial1 validation");
    }
}

template <typename T>
void Serial_device::send(T output){
    Serial1.println(output);
}

String Serial_device::receive(){
    return Serial1.readString();
}

int Serial_device::available(){
    return Serial1.available();
}

void Serial_device::flush(){
    Serial1.flush();
}

long Serial_device::parseNumber(){
    return Serial1.parseInt();
}
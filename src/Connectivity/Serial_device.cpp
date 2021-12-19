#include "Serial_device.h"
#include "Serial_output.h"

extern Serial_output output;

void Serial_device::begin(){
    Serial1.begin(115200);
    delay(100);
    output.println("Serial with device initialised");
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
#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <Arduino.h>

class Serial_device{
    private:

    public:
        void begin();
        template<typename T>
        void send(T output);
        // template<typename T>
        String receive();
        int available();
        void flush();
        long parseNumber();

};


#endif
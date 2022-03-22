#pragma once

#include <Arduino.h>

extern "C"
{
  // callback functions
  typedef void (*MotAxisEvent) (int, uint8_t, const char *);
};


class MFMotAxis
{
public:
    MFMotAxis(uint8_t pin = 1, const char * name = "Analog Input", uint8_t sensitivity = 2);
    static void attachHandler(MotAxisEvent handler);    
    void update();
    void readBuffer();
    int16_t getActualValue();
    
private:
    static MotAxisEvent   _handler; 

};
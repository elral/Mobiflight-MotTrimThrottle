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
    MFMotAxis(uint8_t analogPin);
    void detach();
    static void attachHandler(MotAxisEvent handler);    
    void update();
    void setSetpoint(uint16_t setpoint);
    int16_t getSetpoint();
    
private:
    static MotAxisEvent   _handler;
    bool _initialized = false;
    uint16_t _setpoint;
    uint8_t _analogPin;

};
#pragma once

#include <Arduino.h>

#define OUTOFSYNC_RANGE 6             // if delta is below, then it's synced
#define OUTOFSYNC_TIME 100            // min. time for out of sync detection, in ms

extern "C"
{
  // callback functions
  typedef void (*MotAxisEvent) (int, uint8_t, const char *);
};


class MFMotAxis
{
public:
    MFMotAxis(uint8_t analogPin, uint8_t syncButton, uint8_t stepper);
    void detach();
    static void attachHandler(MotAxisEvent handler);    
    void update();
    void setSetpoint(uint16_t setpoint);
    int16_t getSetpoint();
    
private:
    static MotAxisEvent   _handler;
    bool _initialized = false;
    int16_t _setPoint;
    uint8_t _analogPin;
    uint8_t _syncButton;
    uint8_t _stepper;
    int16_t _actualValue;
    int16_t _deltaSteps;
    int16_t _oldsetPoint;
    bool _inMove;
    uint32_t _time2move;
    bool _synchronizedTrim;
    uint32_t _lastSyncTrim;
};
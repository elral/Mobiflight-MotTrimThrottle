#pragma once

#include <Arduino.h>

#define OUTOFSYNC_RANGE 6             // if delta is below this value, then it's synced
#define OUTOFSYNC_TIME 100            // min. time for out of sync detection, in ms

extern "C"
{
  // callback functions
  typedef void (*MotAxisEvent) (int, /*uint8_t,*/ const char *);
};

enum
{
  btnOnPress,
  btnOnRelease,
};

class MFMotAxis
{
public:
    //MFMotAxis(uint8_t analogPin, uint8_t syncButton, uint8_t stepper, uint8_t startPosition, uint16_t movingTime, uint16_t maxSteps, uint8_t enablePin, uint16_t maxSpeed, uint16_t maxAccel);
    MFMotAxis(uint8_t analogPin, const char *syncName, uint8_t stepper, uint8_t startPosition, uint16_t movingTime, uint16_t maxSteps, uint8_t enablePin, uint16_t maxSpeed, uint16_t maxAccel);
    void startPosition();
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
    //uint8_t _syncButton;
    const char *_syncName;
    uint8_t _stepper;
    int16_t _actualValue;
    int16_t _deltaSteps;
    int16_t _oldsetPoint;
    bool _inMove;
    uint32_t _time2move;
    bool _synchronized;
    uint32_t _lastSync;
    uint16_t _movingTime;
    uint16_t _maxSteps;
    uint8_t _enablePin;
    uint16_t _maxSpeed;
    uint16_t _maxAccel;
};
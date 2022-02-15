
#pragma once

#include <Arduino.h>
class MFSetpointStepper
{
public:
    MFSetpointStepper();
    void setSetpoint(char *string);
    void attach();
    void detach();
    int16_t getSetpoint();

private:
    bool        _initialized;
    int16_t     _setpoint;
};

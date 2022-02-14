
#pragma once

#include <Arduino.h>
class MFSegments
{
public:
    MFSegments();
    void display(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    void attach();
    void detach();
    int16_t getSetpoint();

private:
    bool        _initialized;
    int16_t     _setpoint;
};

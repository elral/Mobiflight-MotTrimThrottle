// MFSegments.h
//
/// \mainpage MF Button module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#pragma once

#include <Arduino.h>
class MFSegments
{
public:
    MFSegments();
    void display(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    void attach();
    void detach();
    uint16_t getSetpoint();

private:
    bool        _initialized;
    uint16_t    _setpoint;
};

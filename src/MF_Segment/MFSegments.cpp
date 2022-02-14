// MFSegments.cpp
//
// Copyright (C) 2013-2021
#include <Arduino.h>
#include "MFSegments.h"

MFSegments::MFSegments()
{
  _initialized = false;
}

void MFSegments::display(byte module, char *string, byte points, byte mask, bool convertPoints)
{
  if (!_initialized)
    return;
  _setpoint = atoi(string);         // range is -100 ... 0 ... +100
  if (_setpoint < -100)
    _setpoint = -100;
  if (_setpoint > 100)
    _setpoint = 100;
  _setpoint *= 5;                   // range is -500 ... 500
}

int16_t MFSegments::getSetpoint()
{
  if (!_initialized)
    return;
  return _setpoint;
}

void MFSegments::attach()
{
  _initialized = true;
}

void MFSegments::detach()
{
  _initialized = false;
}


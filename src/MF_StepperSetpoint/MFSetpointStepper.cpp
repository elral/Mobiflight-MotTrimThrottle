// MFSegments.cpp
//
// Copyright (C) 2013-2021
#include <Arduino.h>
#include "MFSetpointStepper.h"

MFSetpointStepper::MFSetpointStepper()
{
  _initialized = false;
}

void MFSetpointStepper::setSetpoint(char *string)
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

int16_t MFSetpointStepper::getSetpoint()
{
  if (!_initialized)
    return;
  return _setpoint;
}

void MFSetpointStepper::attach()
{
  _initialized = true;
}

void MFSetpointStepper::detach()
{
  _initialized = false;
}


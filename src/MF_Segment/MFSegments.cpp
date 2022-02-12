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
  _setpoint = atoi(string);
Serial.println(_setpoint);
}

uint16_t MFSegments::getSetpoint()
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


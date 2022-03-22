#include "MFMotAxis.h"

MotAxisEvent MFMotAxis::_handler = NULL;

MFMotAxis::MFMotAxis(uint8_t analogPin)
{
    _initialized = true;
    _setpoint = 0;
    _analogPin = analogPin;
}

void detach()
{

}

void MFMotAxis::update()
{

}

void MFMotAxis::setSetpoint(uint16_t setpoint)
{
    _setpoint = setpoint;
}

int16_t MFMotAxis::getSetpoint()
{
    return _setpoint;
}


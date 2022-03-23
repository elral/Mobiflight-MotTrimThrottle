#include "MFMotAxis.h"
#include "Analog.h"
#include "Stepper.h"
#include "Button.h"

MotAxisEvent MFMotAxis::_handler = NULL;

MFMotAxis::MFMotAxis(uint8_t analogPin, uint8_t syncButton, uint8_t stepper)
{
    _initialized = true;
    _setPoint = 0;                                      // range is -500 ... 500, from UI setpoint must be in +/-0.1%
    _synchronizedTrim = false;
    _lastSyncTrim = 0;
    _analogPin = analogPin;
    _syncButton = syncButton;
    _stepper = stepper;
// add moving axis to mid position.
// mhmhm, for throttle it must be the min position....
}

void detach()
{

}

void MFMotAxis::update()
{

    _actualValue = Analog::getActualValue(_analogPin);   // range is -512 ... 511 for 270°
    _deltaSteps = _setPoint - _actualValue;              // Stepper: 800 steps for 360° -> 600 steps for 270° -> with gear 1:2 = 900 steps
    Stepper::SetRelative(_stepper, _deltaSteps / 2);     // Accellib has it's own PID controller, so handles acceleration and max. speed by itself

    if (_oldsetPoint != _setPoint)                        // stepper must move
    {
        _oldsetPoint = _setPoint;
        _inMove = true;
        //time2move = millis() + ((uint32_t)abs(deltaSteps) * 1000 * 4) / 900;  // 4 sec. from min to max (900 steps) in msec.
        uint16_t accel = 1 + ((1000 - abs(_deltaSteps)) / 250);      // consider longer time for small steps due to acceleration
        //time2move = millis() + ((uint32_t)abs(deltaSteps) * 10 * 4 * accel) / 9;
        _time2move = millis() + ((uint32_t)abs(_deltaSteps) * 10 * 3 * accel) / 9;
Serial.print("Start Moving for corrected:"); Serial.println(_time2move - millis());
Serial.print("Delta Steps: "); Serial.println(abs(_deltaSteps));
    }

    if (_time2move < millis() && _inMove)
    {
        _inMove = false;
Serial.println("Stop moving");
    }

    if (abs(_deltaSteps) < OUTOFSYNC_RANGE)              // if actual value is near setpoint
    {           // do I have to check for AutoTrim mode??? What happens if manual mode selected and actual value is setpoint -> synchronized = true,
                // next movement could be out of range, button press will be initiated
        _synchronizedTrim = true;                        // we are synchronized
        _lastSyncTrim = millis();                        // save the time of last synchronization for detecting out of sync for more than specified time
    } else if (millis() - _lastSyncTrim >= OUTOFSYNC_TIME && _synchronizedTrim == true && !_inMove)
    {
        _synchronizedTrim = false;
        Button::press(_syncButton);                               // simulate button press, is button release required for the connector?
    }
}

void MFMotAxis::setSetpoint(uint16_t setpoint)
{
    _setPoint = setpoint;
}

int16_t MFMotAxis::getSetpoint()
{
    return _setPoint;
}


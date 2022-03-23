#include "MFMotAxis.h"
#include "Analog.h"
#include "Stepper.h"
#include "Button.h"

MotAxisEvent MFMotAxis::_handler = NULL;

MFMotAxis::MFMotAxis(uint8_t analogPin, uint8_t syncButton, uint8_t stepper, uint8_t startPosition, uint16_t movingTime, uint16_t maxSteps, uint8_t enablePin, uint16_t maxSpeed, uint16_t maxAccel)
{
    _initialized = true;
    _setPoint = map(startPosition, 0, 100, -512, 511);              // define center position, comes in 0...100%, must be -512...511
    _synchronized = true;                                           // on startup we will move to start position
    _lastSync = 0;                                                  // for calculation of out of sync, this is time dependent
    _analogPin = analogPin;                                         // where to get the actual value from
    _syncButton = syncButton;                                       // button on which out of sync is reported
    _stepper = stepper;                                             // which stepper has to be moved
    _movingTime = movingTime;                                       // time for complete stroke in 1ms (0s to 25.5s)
    _maxSteps = maxSteps;                                           // number of steps for the complete stroke
    _enablePin = enablePin;                                         // output to en-/dis-able the stepper
    _maxSpeed = maxSpeed;
    _maxAccel = maxAccel;
}

void MFMotAxis::startPosition()                                     // this must be called after reading the config as it can not be ensured that all device are initialized when the constructor is called
{
    Stepper::setMaxSpeed(_stepper, _maxSpeed);
    Stepper::setAcceleration(_stepper, _maxAccel);
    digitalWrite(_enablePin, 0);                                    // enable stepper for moving to center position
    uint32_t startCentering = millis();
    do {
        Analog::readAverage();                                      // read analog and calculate floating average
        _actualValue = Analog::getActualValue(_analogPin);          // range is -512 ... 511 for 270°
        _deltaSteps = _setPoint - _actualValue;                     // Stepper: 800 steps for 360° -> 600 steps for 270°
        Stepper::SetRelative(_stepper, _deltaSteps / 2);            // Accellib has it's own PID controller, so handles acceleration and max. speed by itself
        Stepper::update();                                          // ensure stepper is moving
        if (millis() - startCentering > _movingTime)                // do not move to center position forever, must be within max. moving time
        {
            _synchronized = false;                                  // in this case we are not synchronized
            break;                                                  // centering must be within 3 sec in case one analog in is not connected
        }   
    } while (abs(_deltaSteps) > 5);                                 // on startup center Axis
    digitalWrite(_enablePin, 1);                                    // disable stepper on startup
}

void MFMotAxis::detach()
{
    _initialized = false;
}

void MFMotAxis::update()
{
    _actualValue = Analog::getActualValue(_analogPin);              // range is -512 ... 511 for 270°
    _deltaSteps = _setPoint - _actualValue;                         // Stepper: 800 steps for 360° -> 600 steps for 270° -> with gear 1:2 = 900 steps
    Stepper::SetRelative(_stepper, _deltaSteps / 2);                // Accellib has it's own PID controller, so handles acceleration and max. speed by itself

    if (_oldsetPoint != _setPoint)                                  // stepper must move
    {
        _oldsetPoint = _setPoint;
        _inMove = true;
        uint16_t accel = 1 + ((1000 - abs(_deltaSteps)) / 250);      // consider longer time for small steps due to acceleration
        _time2move = millis() + ((uint32_t)abs(_deltaSteps) * _movingTime * accel) / 900;  // 4 sec. from min to max (900 steps) in msec.
Serial.print("Start Moving for corrected:"); Serial.println(_time2move - millis());
Serial.print("Delta Steps: "); Serial.println(abs(_deltaSteps));
    }

    if (_time2move < millis() && _inMove)
    {
        _inMove = false;
Serial.println("Stop moving");
    }

    if (abs(_deltaSteps) < OUTOFSYNC_RANGE)                         // if actual value is near setpoint
    {           // do I have to check for AutoTrim mode??? What happens if manual mode selected and actual value is setpoint -> synchronized = true,
                // next movement could be out of range, button press will be initiated
        _synchronized = true;                                       // we are synchronized
        _lastSync = millis();                                       // save the time of last synchronization for detecting out of sync for more than specified time
    } else if (millis() - _lastSync >= OUTOFSYNC_TIME && _synchronized == true && !_inMove)
    {
        _synchronized = false;
        Button::press(_syncButton);                                 // simulate button press, is button release required for the connector?
    }
}

void MFMotAxis::setSetpoint(uint16_t setpoint)
{
    _setPoint = setpoint;                                           // range is -500 ... 500, from UI setpoint must be in +/-0.1%
}

int16_t MFMotAxis::getSetpoint()
{
    return _setPoint;
}


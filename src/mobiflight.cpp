#include <Arduino.h>
#include "mobiflight.h"
#include "CommandMessenger.h"
#include "MFBoards.h"
#include "config.h"
#include "Button.h"
#include "Analog.h"
#include "Output.h"
#include "SetpointStepper.h"
#include "Stepper.h"


#define MF_BUTTON_DEBOUNCE_MS         10    // time between updating the buttons
#define MF_ANALOGAVERAGE_DELAY_MS     10    // time between updating the analog average calculation
#define MF_ANALOGREAD_DELAY_MS        50    // time between sending analog values

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds
uint32_t lastButtonUpdate= 0;
uint32_t lastAnalogAverage = 0;
uint32_t lastAnalogRead = 0;
int16_t setPoint = 0;
int16_t actualValue = 0;
int16_t deltaSteps = 0;

// ************************************************************
// Power saving
// ************************************************************
void SetPowerSavingMode(bool state)
{
  // disable the lights ;)
  powerSavingMode = state;
  Output::PowerSave(state);     // why is this uncommeted in the main branch??
// Maybe there is one for the stepper needed....
#ifdef DEBUG2CMDMESSENGER
  if (state)
    cmdMessenger.sendCmd(kStatus, F("On"));
  else
    cmdMessenger.sendCmd(kStatus, F("Off"));
#endif
}

void updatePowerSaving()
{
  if (!powerSavingMode && ((millis() - getLastCommandMillis()) > (POWER_SAVING_TIME * 1000)))
  {
    // enable power saving
    SetPowerSavingMode(true);
  }
  else if (powerSavingMode && ((millis() - getLastCommandMillis()) < (POWER_SAVING_TIME * 1000)))
  {
    // disable power saving
    SetPowerSavingMode(false);
  }
}

// ************************************************************
// Reset Board
// ************************************************************
void ResetBoard()
{
  generateSerial(false);
  setLastCommandMillis();
  loadConfig();
  _restoreName();
}

enum {
  TrimWheel,
  Throttle
};

// ************************************************************
// Setup
// ************************************************************
void setup()
{
  Serial.begin(115200);
  attachCommandCallbacks();
  cmdMessenger.printLfCr();
  ResetBoard();

  Stepper::Add(2, 3, 2, 3, 0);                              // add and initialize Stepper TrimWheel
  Stepper::Add(5, 6, 5, 6, 0);                              // add and initialize Stepper Throttle

  digitalWrite(4,0);                                        // enable stepper for moving to center position
  digitalWrite(7,0);                                        // enable stepper for moving to center position
  setPoint = 0;                                             // define center position
  uint32_t startCentering = millis();
  int16_t deltaTrim = 0;
  int16_t deltaThrottle = 0;
  do 
  {
    Analog::readAverage();                                  // read analog and calculate floating average

    actualValue = Analog::getActualValue(TrimWheel);        // range is -512 ... 511 for 270°
    deltaTrim = setPoint - actualValue;                     // Stepper: 800 steps for 360° -> 600 steps for 270°
    Stepper::SetRelative(TrimWheel, deltaTrim/2);           // Accellib has it's own PID controller, so handles acceleration and max. speed by itself
    
    actualValue = Analog::getActualValue(Throttle);         // range is -512 ... 511 for 270°
    deltaThrottle = setPoint - actualValue;                 // Stepper: 800 steps for 360° -> 600 steps for 270°
    Stepper::SetRelative(Throttle, deltaThrottle/2);        // Accellib has it's own PID controller, so handles acceleration and max. speed by itself

    Stepper::update();                                      // ensure stepper is moving
  } while ((abs(deltaTrim) > 5 && abs(deltaThrottle) > 5) || millis()-startCentering < 3000); // on startup center TrimWheel and Throttle, must be within 3 sec in case one analog in is not connected

  digitalWrite(4,1);                                        // disable stepper on startup
  digitalWrite(7,1);                                        // disable stepper on startup

// Time Gap between Inputs, do not read at the same loop
  lastButtonUpdate = millis() + 0;
  lastAnalogAverage = millis() + 4;
  lastAnalogRead = millis() + 4;
}

// ************************************************************
// Loop function
// ************************************************************
void loop()
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
  updatePowerSaving();

  if (getStatusConfig())
  {
    if (millis() - lastButtonUpdate >= MF_BUTTON_DEBOUNCE_MS)
    {
      lastButtonUpdate = millis();
      Button::read();
    }
    if (millis() - lastAnalogRead >= MF_ANALOGREAD_DELAY_MS)
    {
      lastAnalogRead = millis();
      Analog::read();
    }
    if (millis() - lastAnalogAverage >= MF_ANALOGAVERAGE_DELAY_MS)
    {
      lastAnalogAverage = millis();
      Analog::readAverage();
    }

    Stepper::update();
    
    setPoint = SetpointStepper::GetSetpoint(TrimWheel);     // range is -500 ... 500, from UI setpoint must be in +/-0.1%
    actualValue = Analog::getActualValue(TrimWheel);        // range is -512 ... 511 for 270°
    deltaSteps = setPoint - actualValue;                    // Stepper: 800 steps for 360° -> 600 steps for 270°
    Stepper::SetRelative(TrimWheel, deltaSteps/2);          // Accellib has it's own PID controller, so handles acceleration and max. speed by itself

    setPoint = SetpointStepper::GetSetpoint(Throttle);      // range is -500 ... 500, from UI setpoint must be in +/-0.1%
    actualValue = Analog::getActualValue(Throttle);         // range is -512 ... 511 for 270°
    deltaSteps = setPoint - actualValue;                    // Stepper: 800 steps for 360° -> 600 steps for 270°
    Stepper::SetRelative(Throttle, deltaSteps/2);           // Accellib has it's own PID controller, so handles acceleration and max. speed by itself

  }
}


// 1,0,0,12345678,255;  Modul 0 = setpoint TrimWheel -> LedSegment::GetSetpoint(0)
// 1,1,0,12345678,255;  Modul 1 = setpoint Throttle  -> LedSegment::GetSetpoint(1)
// 3,0,800;             Stepper 0 relative  Stepper::OnSetRelative() gets still valie from CMDMessenger!! additional function required to keep receiving from UI??
// 3,1,-800;            Stepper 1 relative
// 2,4,255;             Disable Stepper 0
// 2,4,0;               Enable Stepper 0

#include <Arduino.h>
#include "mobiflight.h"
#include "CommandMessenger.h"
#include "MFBoards.h"
#include "config.h"
#include "Button.h"
#include "Analog.h"
#include "Output.h"
#include "LedSegment.h"
#include "Stepper.h"


#define MF_BUTTON_DEBOUNCE_MS         10    // time between updating the buttons
#define MF_ANALOGAVERAGE_DELAY_MS     10    // time between updating the analog average calculation
#define MF_ANALOGREAD_DELAY_MS        50    // time between sending analog values

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds
uint32_t lastButtonUpdate= 0;
uint32_t lastEncoderUpdate = 0;
uint32_t lastAnalogAverage = 0;
uint32_t lastAnalogRead = 0;




// ************************************************************
// Power saving
// ************************************************************
void SetPowerSavingMode(bool state)
{
  // disable the lights ;)
  powerSavingMode = state;
//  Output::PowerSave(state);     // why is this uncommeted in the main branch??
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

// ************************************************************
// Setup
// ************************************************************
void setup()
{
  Serial.begin(115200);
  attachCommandCallbacks();
  cmdMessenger.printLfCr();
  ResetBoard();

  Stepper::Add(2, 3, 2, 3, 0);            // Stepper TrimWheel
  Stepper::Add(5, 6, 5, 6, 0);            // Stepper Throttle

// Time Gap between Inputs, do not read at the same loop
  lastButtonUpdate = millis() + 0;
  lastEncoderUpdate = millis();           // encoders will be updated every 1ms
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

  // if config has been reset and still is not activated
  // do not perform updates
  // to prevent mangling input for config (shared buffers)
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
    
// 1,0,0,12345678,255;  Modul 0 = setpoint TrimWheel -> LedSegment::GetSetpoint(0)
// 1,1,0,12345678,255;  Modul 1 = setpoint Throttle  -> LedSegment::GetSetpoint(1)
// 3,0,800;             Stepper 0 relative  Stepper::OnSetRelative() gets still valie from CMDMessenger!! additional function required to keep receiving from UI??
// 3,1,-800;            Stepper 1 relative

  }
}
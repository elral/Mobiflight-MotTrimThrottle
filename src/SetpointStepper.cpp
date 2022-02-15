#include <Arduino.h>
#include "allocateMem.h"
#include "CommandMessenger.h"
#include "SetpointStepper.h"
#include "MFBoards.h"

namespace SetpointStepper
{

uint8_t stepperSetpointRegistered = 0;
int16_t Setpoint[MAX_STEPPER_SETPOINT];

void Add()
{
  if (stepperSetpointRegistered == MAX_STEPPER_SETPOINT)
    return;
  Setpoint[stepperSetpointRegistered] = 0;
  stepperSetpointRegistered++;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Added Stepper Setpoint"));
#endif
}

void Clear()
{
  stepperSetpointRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Cleared Stepper Setpoint"));
#endif
}

void OnSetModule()
{
  int stepper = cmdMessenger.readInt16Arg();
  int newValue = cmdMessenger.readInt16Arg();     // range must be in +/- 0.1% -> -1000 ... 0 ... +1000
  if (stepper >= stepperSetpointRegistered)
    return;
  if (newValue < -1000)
    newValue = -1000;
  if (newValue > 1000)
    newValue = 1000;
  Setpoint[stepper] = newValue / 2;              // divide by 2 to get -500 ... +500 like the analog values
  setLastCommandMillis();
}

int16_t GetSetpoint(uint8_t stepper)
{
  if (stepper >= stepperSetpointRegistered)
    return 0;
  return Setpoint[stepper];
}

}   // end of namespace LedSegment

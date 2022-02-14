#include <Arduino.h>
#include "MFSetpointStepper.h"
#include "allocateMem.h"
#include "CommandMessenger.h"
#include "SetpointStepper.h"
#include "MFBoards.h"

namespace SetpointStepper
{
MFSetpointStepper *stepperSetpoint[MAX_STEPPER_SETPOINT];
uint8_t stepperSetpointRegistered = 0;

void Add(int dataPin, int csPin, int clkPin, int numDevices, int brightness)
{
  if (stepperSetpointRegistered == MAX_STEPPER_SETPOINT)
    return;

  if (!FitInMemory(sizeof(MFSetpointStepper)))
	{
		// Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("Stepper Setpoint does not fit in Memory!"));
		return;
	}
  stepperSetpoint[stepperSetpointRegistered] = new (allocateMemory(sizeof(MFSetpointStepper))) MFSetpointStepper;
  stepperSetpoint[stepperSetpointRegistered]->attach();
  stepperSetpointRegistered++;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Added Stepper Setpoint"));
#endif
}

void Clear()
{
  for (int i = 0; i != stepperSetpointRegistered; i++)
  {
    stepperSetpoint[i]->detach();
  }
  stepperSetpointRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Cleared Stepper Setpoint"));
#endif
}

void OnInitModule()
{
/*
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg();
*/
  setLastCommandMillis();
}

void OnSetModule()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  char *value = cmdMessenger.readStringArg();
  uint8_t points = (uint8_t)cmdMessenger.readInt16Arg();
  uint8_t mask = (uint8_t)cmdMessenger.readInt16Arg();
  stepperSetpoint[module]->setSetpoint(subModule, value, points, mask);
  setLastCommandMillis();
}

void OnSetModuleBrightness()
{
/*
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg();
*/
  setLastCommandMillis();
}

int16_t GetSetpoint(uint8_t _module)
{
  return stepperSetpoint[_module]->getSetpoint();
}

}   // end of namespace LedSegment

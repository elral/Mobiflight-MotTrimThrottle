#include <Arduino.h>
#include "MotAxis.h"
#include "MFMotAxis.h"
#include "CommandMessenger.h"
#include "MFBoards.h"
#include "allocateMem.h"

/*
// these defines should come from the connector, but how to handle this amount of parameters and exceeding uint8_t??
#define MAXSTEPS            900     // number of steps for complete stroke
#define MOVINGTIME          4000    // moving time for complete stroke in ms
#define MAXSPEEDAXIS        600     // max. speed for the stepper, overrules MFBoard.h
#define MAXACCELAXIS        800     // max. Accel. for the stepper, overrules MFBoard.h
*/

namespace MotAxis
{

    uint8_t MotAxisRegistered = 0;
    MFMotAxis *motaxis[MAX_MOTAXIS];

    void Add(uint8_t analogPin, uint8_t syncButton, uint8_t stepper, uint8_t enablePin, uint8_t startPosition,
             uint16_t movingTime, uint16_t maxSteps, uint16_t maxSpeed, uint16_t maxAccel)
//    void Add(uint8_t analogPin, uint8_t syncButton, uint8_t stepper, uint8_t enablePin, uint8_t startPosition)
    {
        if (MotAxisRegistered == MAX_MOTAXIS)
            return;
        if (!FitInMemory(sizeof(MFMotAxis))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Button does not fit in Memory"));
            return;
        }
//        motaxis[MotAxisRegistered] = new (allocateMemory(sizeof(MFMotAxis))) MFMotAxis(analogPin, syncButton, stepper, startPosition, MOVINGTIME, MAXSTEPS, enablePin, MAXSPEEDAXIS, MAXACCELAXIS);
        motaxis[MotAxisRegistered] = new (allocateMemory(sizeof(MFMotAxis))) MFMotAxis(analogPin, syncButton, stepper, startPosition, movingTime, maxSteps, enablePin, maxSpeed, maxAccel);
        MotAxisRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added Stepper Setpoint"));
#endif
    }

    void startPosition()
    {
        for (int i = 0; i != MotAxisRegistered; i++)
        {
            motaxis[i]->startPosition();
        }
    }
    void Clear()
    {
        for (int i = 0; i != MotAxisRegistered; i++)
        {
            motaxis[i]->detach();
        }
        MotAxisRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Cleared Stepper Setpoint"));
#endif
    }

    void update()
    {
        for (int i = 0; i != MotAxisRegistered; i++)
        {
            motaxis[i]->update();
        }
    }

    void OnSetModule()
    {
        int axis = cmdMessenger.readInt16Arg();
        int temp = cmdMessenger.readInt16Arg();
        char *value = cmdMessenger.readStringArg();
        temp = (uint8_t)cmdMessenger.readInt16Arg();
        temp = (uint8_t)cmdMessenger.readInt16Arg();
        int16_t newValue = atoi(value);
        if (newValue < -1000)
            newValue = -1000;
        if (newValue > 1000)
            newValue = 1000;
        motaxis[axis]->setSetpoint(newValue / 2); // divide by 2 to get -500 ... +500 like the analog values
        setLastCommandMillis();
    }
} // end of namespace SetpointStepper

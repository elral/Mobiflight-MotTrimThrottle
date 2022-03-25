#include <Arduino.h>
#include "MotAxis.h"
#include "MFMotAxis.h"
#include "CommandMessenger.h"
#include "MFBoards.h"
#include "allocateMem.h"


namespace MotAxis
{
    uint8_t MotAxisRegistered = 0;
    MFMotAxis *motaxis[MAX_MOTAXIS];

    void handlerOnMotaxis(uint8_t eventId,/* uint8_t pin,*/ const char *name)
    {
        cmdMessenger.sendCmdStart(kButtonChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t analogPin, const char *syncName, uint8_t stepper, uint8_t enablePin, uint8_t startPosition,
             uint16_t movingTime, uint16_t maxSteps, uint16_t maxSpeed, uint16_t maxAccel)
    {
        if (MotAxisRegistered == MAX_MOTAXIS)
            return;
        if (!FitInMemory(sizeof(MFMotAxis))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Button does not fit in Memory"));
            return;
        }
        motaxis[MotAxisRegistered] = new (allocateMemory(sizeof(MFMotAxis))) MFMotAxis(analogPin, syncName, stepper, startPosition, movingTime, maxSteps, enablePin, maxSpeed, maxAccel);
        motaxis[MotAxisRegistered]->attachHandler(handlerOnMotaxis);
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

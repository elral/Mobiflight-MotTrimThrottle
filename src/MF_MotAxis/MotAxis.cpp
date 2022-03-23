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

    void Add(uint8_t analogPin, uint8_t syncButton, uint8_t stepper)
    {
        if (MotAxisRegistered == MAX_MOTAXIS)
            return;
        
        if (!FitInMemory(sizeof(MFMotAxis))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Button does not fit in Memory"));
            return;
        }
        motaxis[MotAxisRegistered] = new (allocateMemory(sizeof(MFMotAxis))) MFMotAxis(analogPin, syncButton, stepper);

        MotAxisRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added Stepper Setpoint"));
#endif
    }

    void Clear()
    {
        for (int i = 0; i != MotAxisRegistered; i++) {
            motaxis[i]->detach();
        }
        MotAxisRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Cleared Stepper Setpoint"));
#endif
    }

    void update()
    {
        for (int i = 0; i != MotAxisRegistered; i++) {
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

    int16_t GetSetpoint(uint8_t axis)
    {
        if (axis >= MotAxisRegistered)
            return 0;
        return motaxis[axis]->getSetpoint();
    }

} // end of namespace SetpointStepper

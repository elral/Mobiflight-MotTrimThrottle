#include <Arduino.h>
#include "Stepper.h"
#include "CommandMessenger.h"
#include "MFBoards.h"
#include "MFStepper.h"
#include "allocateMem.h"
#include "mobiflight.h"


namespace Stepper
{
    MFStepper *steppers[MAX_STEPPERS];
    uint8_t steppersRegistered = 0;

    void Add(int pin1, int pin2, int pin3, int pin4, int btnPin1)
    {
        if (steppersRegistered == MAX_STEPPERS)
            return;

        if (!FitInMemory(sizeof(MFStepper))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Stepper does not fit in Memory!"));
            return;
        }
        steppers[steppersRegistered] = new (allocateMemory(sizeof(MFStepper))) MFStepper;
        steppers[steppersRegistered]->attach(pin1, pin2, pin3, pin4, btnPin1);
        steppers[steppersRegistered]->setMaxSpeed(STEPPER_SPEED);
        steppers[steppersRegistered]->setAcceleration(STEPPER_ACCEL);
        // autoreset is not released yet
        if (btnPin1 > 0) {
            // this triggers the auto reset if we need to reset
            steppers[steppersRegistered]->reset();
        }

        // all set
        steppersRegistered++;

#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added stepper"));
#endif
    }

    void Clear()
    {
        for (int i = 0; i != steppersRegistered; i++) {
            steppers[i]->detach();
        }
        steppersRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Cleared steppers"));
#endif
    }

    void OnSet()
    {
        int stepper = cmdMessenger.readInt16Arg();
        long newPos = cmdMessenger.readInt32Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper]->moveTo(newPos);
        setLastCommandMillis();
    }

    void OnSetRelative()
    {
        int stepper = cmdMessenger.readInt16Arg();
        long newPos = cmdMessenger.readInt32Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper]->move(newPos);
        setLastCommandMillis();
    }

    void SetRelative(uint8_t _stepper, int16_t _pos)
    {
        if (_stepper >= steppersRegistered)
            return;
        steppers[_stepper]->move(_pos);
        setLastCommandMillis();
    }

    void OnReset()
    {
        int stepper = cmdMessenger.readInt16Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper]->reset();
        setLastCommandMillis();
    }

    void OnSetZero()
    {
        int stepper = cmdMessenger.readInt16Arg();

        if (stepper >= steppersRegistered)
            return;
        steppers[stepper]->setZero();
        setLastCommandMillis();
    }

    void update()
    {
        for (int i = 0; i != steppersRegistered; i++) {
            steppers[i]->update();
        }
    }
} // end of namespace Stepper

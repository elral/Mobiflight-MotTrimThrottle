#include <Arduino.h>
#include "Analog.h"
#include "CommandMessenger.h"
#include "MFAnalog.h"
#include "MFBoards.h"
#include "allocateMem.h"
#include "mobiflight.h"

namespace Analog
{
    MFAnalog *analog[MAX_ANALOG_INPUTS];
    uint8_t analogRegistered = 0;

    void handlerOnAnalogChange(int value, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kAnalogChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(value);
        cmdMessenger.sendCmdEnd();
    };

    uint8_t Add(uint8_t pin = 1, char const *name = "AnalogInput", uint8_t sensitivity = 3)
    {
        if (analogRegistered == MAX_ANALOG_INPUTS)
            return 0;

        if (!FitInMemory(sizeof(MFAnalog))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("AnalogIn does not fit in Memory"));
            return 0;
        }
        analog[analogRegistered] = new (allocateMemory(sizeof(MFAnalog))) MFAnalog(pin, name, sensitivity);
        analog[analogRegistered]->attachHandler(handlerOnAnalogChange);
        analogRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added analog device "));
#endif
        return (analogRegistered - 1);
    }

    void Clear()
    {
        analogRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Cleared analog devices"));
#endif
    }

    void read()
    {
        for (int i = 0; i != analogRegistered; i++) {
            analog[i]->update();
        }
    }

    int16_t getActualValue(uint8_t channel)
    {
        return analog[channel]->getActualValue(); // range is -512 ... +511
    }

    void readAverage()
    {
        for (int i = 0; i != analogRegistered; i++) {
            analog[i]->readBuffer();
        }
    }
} // end of namespace Analog

#include <Arduino.h>
#include "Output.h"
#include "CommandMessenger.h"
#include "MFBoards.h"
#include "MFOutput.h"
#include "allocateMem.h"
#include "mobiflight.h"


namespace Output
{
    MFOutput *outputs[MAX_OUTPUTS];
    uint8_t outputsRegistered = 0;

    void Add(uint8_t pin = 1)
    {
        if (outputsRegistered == MAX_OUTPUTS)
            return;

        if (!FitInMemory(sizeof(MFOutput))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Output does not fit in Memory"));
            return;
        }
        outputs[outputsRegistered] = new (allocateMemory(sizeof(MFOutput))) MFOutput(pin);
        outputsRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added output"));
#endif
    }

    void Clear()
    {
        outputsRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Cleared outputs"));
#endif
    }

    void OnSet()
    {
        // Read led state argument, interpret string as boolean
        int pin = cmdMessenger.readInt16Arg();
        int state = cmdMessenger.readInt16Arg();
        // Set led
        analogWrite(pin, 255 - state); // why does the UI sends the pin number and not the x.th output number like other devices? Negate the output
                                       //  output[pin]->set(state);      // once this is changed uncomment this
        setLastCommandMillis();
    }

    void PowerSave(bool state)
    {
        for (int i = 0; i != outputsRegistered; ++i) {
            outputs[i]->powerSavingMode(state);
        }
    }
} // end of namespace Output

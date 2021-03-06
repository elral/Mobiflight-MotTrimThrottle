#include <Arduino.h>
#include "button.h"
#include "CommandMessenger.h"
#include "MFBoards.h"
#include "MFButton.h"
#include "allocateMem.h"
#include "mobiflight.h"


namespace Button
{
    MFButton *buttons[MAX_BUTTONS];
    uint8_t buttonsRegistered = 0;

    void handlerOnButton(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kButtonChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin = 1, char const *name = "Button")
    {
        if (buttonsRegistered == MAX_BUTTONS)
            return;

        if (!FitInMemory(sizeof(MFButton))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("Button does not fit in Memory"));
            return;
        }
        buttons[buttonsRegistered] = new (allocateMemory(sizeof(MFButton))) MFButton(pin, name);
        buttons[buttonsRegistered]->attachHandler(handlerOnButton);
        buttonsRegistered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Added button ") /* + name */);
#endif
    }

    void Clear()
    {
        buttonsRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kStatus, F("Cleared buttons"));
#endif
    }

    void read()
    {
        for (int i = 0; i != buttonsRegistered; i++) {
            buttons[i]->update();
        }
    }

    void press(uint8_t button)
    {
        if (button < buttonsRegistered)
            buttons[button]->triggerOnPress();
    }

    void release(uint8_t button)
    {
        if (button < buttonsRegistered)
            buttons[button]->triggerOnRelease();
    }

    void OnTrigger()
    {
        // Trigger all button release events first...
        for (int i = 0; i != buttonsRegistered; i++) {
            buttons[i]->triggerOnRelease();
        }
        // ... then trigger all the press events
        for (int i = 0; i != buttonsRegistered; i++) {
            buttons[i]->triggerOnPress();
        }
    }
} // end of namespace button

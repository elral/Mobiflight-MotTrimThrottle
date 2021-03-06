#include <Arduino.h>
#include "CommandMessenger.h"
#include "Analog.h"
#include "Button.h"
#include "MFBoards.h"
#include "Output.h"
#include "MotAxis.h"
#include "Stepper.h"
#include "config.h"
#include "mobiflight.h"


CmdMessenger cmdMessenger = CmdMessenger(Serial);
unsigned long lastCommand;

void OnTrigger();
void OnUnknownCommand();

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
    // Attach callback methods
    cmdMessenger.attach(OnUnknownCommand);

    cmdMessenger.attach(kSetModule, MotAxis::OnSetModule);
    cmdMessenger.attach(kSetPin, Output::OnSet);
    cmdMessenger.attach(kSetStepper, Stepper::OnSetRelative);
    cmdMessenger.attach(kResetStepper, Stepper::OnReset);
    cmdMessenger.attach(kSetZeroStepper, Stepper::OnSetZero);

    cmdMessenger.attach(kGetInfo, OnGetInfo);
    cmdMessenger.attach(kGetConfig, OnGetConfig);
    cmdMessenger.attach(kSetConfig, OnSetConfig);
    cmdMessenger.attach(kResetConfig, OnResetConfig);
    cmdMessenger.attach(kSaveConfig, OnSaveConfig);
    cmdMessenger.attach(kActivateConfig, OnActivateConfig);
    cmdMessenger.attach(kSetName, OnSetName);
    cmdMessenger.attach(kGenNewSerial, OnGenNewSerial);
    cmdMessenger.attach(kTrigger, OnTrigger);
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
    lastCommand = millis();
    cmdMessenger.sendCmd(kStatus, F("n/a"));
}

uint32_t getLastCommandMillis()
{
    return lastCommand;
}

void setLastCommandMillis()
{
    lastCommand = millis();
}

void OnTrigger()
{
    Button::OnTrigger();
}

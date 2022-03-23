#include <Arduino.h>
#include "mobiflight.h"
#include "Analog.h"
#include "Button.h"
#include "CommandMessenger.h"
#include "MFBoards.h"
#include "Output.h"
#include "MotAxis.h"
#include "Stepper.h"
#include "config.h"


#define MF_BUTTON_DEBOUNCE_MS 10        // time between updating the buttons
#define MF_ANALOGAVERAGE_DELAY_MS 10    // time between updating the analog average calculation
#define MF_ANALOGREAD_DELAY_MS 50       // time between sending analog values
#define OUTOFSYNC_RANGE 6               // if delta is below, then it's synced
#define OUTOFSYNC_TIME 100              // min. time for out of sync detection, in ms

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds
uint32_t lastButtonUpdate = 0;
uint32_t lastAnalogAverage = 0;
uint32_t lastAnalogRead = 0;
int16_t setPoint = 0;
int16_t oldsetPoint = 0;
bool inMove = false;
uint32_t time2move = 0;
int16_t actualValue = 0;
int16_t deltaSteps = 0;

bool synchronizedTrim = true;
uint32_t lastSyncTrim = 0;
bool synchronizedThrottle = true;
uint32_t lastSyncThrottle = 0;

// ************************************************************
// Power saving
// ************************************************************
void SetPowerSavingMode(bool state)
{
    // disable the lights ;)
    powerSavingMode = state;
    Output::PowerSave(state);
// Maybe there is one for the stepper needed....
#ifdef DEBUG2CMDMESSENGER
    if (state)
        cmdMessenger.sendCmd(kStatus, F("On"));
    else
        cmdMessenger.sendCmd(kStatus, F("Off"));
#endif
}

void updatePowerSaving()
{
    if (!powerSavingMode && ((millis() - getLastCommandMillis()) > (POWER_SAVING_TIME * 1000))) {
        // enable power saving
        SetPowerSavingMode(true);
    } else if (powerSavingMode && ((millis() - getLastCommandMillis()) < (POWER_SAVING_TIME * 1000))) {
        // disable power saving
        SetPowerSavingMode(false);
    }
}

// ************************************************************
// Reset Board
// ************************************************************
void ResetBoard()
{
    generateSerial(false);
    setLastCommandMillis();
    loadConfig();
    _restoreName();
}

enum {
    TrimWheel,
    Throttle
};

// ************************************************************
// Setup
// ************************************************************
void setup()
{
    Serial.begin(115200);
    attachCommandCallbacks();
    cmdMessenger.printLfCr();
    ResetBoard();
    MotAxis::startPosition();
    // Time Gap between Inputs, do not read at the same loop
    lastButtonUpdate = millis() + 0;
    lastAnalogAverage = millis() + 4;
    lastAnalogRead = millis() + 4;
    lastSyncTrim = millis();
    lastSyncThrottle = millis();
}

// ************************************************************
// Loop function
// ************************************************************
void loop()
{
    // Process incoming serial data, and perform callbacks
    cmdMessenger.feedinSerialData();
    updatePowerSaving();

    if (getStatusConfig()) {
        if (millis() - lastButtonUpdate >= MF_BUTTON_DEBOUNCE_MS) {
            lastButtonUpdate = millis();
    //        Button::read();   Button 0 and 1 is simulated lost of sync, must not be read in as it gots wrong status
    // maybe better not to read in via loadconfig()??
        }
        if (millis() - lastAnalogRead >= MF_ANALOGREAD_DELAY_MS) {
            lastAnalogRead = millis();
            Analog::read();
        }
        if (millis() - lastAnalogAverage >= MF_ANALOGAVERAGE_DELAY_MS) {
            lastAnalogAverage = millis();
            Analog::readAverage();
        }
        MotAxis::update();                                  // must be called as often as poosible to update closed loop control
        Stepper::update();                                  // must be called as often as poosible to move the stepper
    }
}

// 1,0,0,12345678,255;  Modul 0 = setpoint TrimWheel -> LedSegment::GetSetpoint(0)
// 1,1,0,12345678,255;  Modul 1 = setpoint Throttle  -> LedSegment::GetSetpoint(1)
// 3,0,800;             Stepper 0 relative  Stepper::OnSetRelative() gets still valie from CMDMessenger!! additional function required to keep receiving from UI??
// 3,1,-800;            Stepper 1 relative
// 2,4,255;             Disable Stepper 0
// 2,4,0;               Enable Stepper 0

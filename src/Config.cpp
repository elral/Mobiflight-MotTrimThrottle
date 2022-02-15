#include <Arduino.h>
#include "MFBoards.h"
#include "config.h"
#include "MFEEPROM.h"
#include "CommandMessenger.h"
#include "Button.h"
#include "configuration.h"
#include "Output.h"
#include "Analog.h"
#include "SetpointStepper.h"
#include "Stepper.h"

// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION "2.0.1" //STR_VALUE(BUILD_VERSION)

MFEEPROM MFeeprom;

const uint8_t MEM_OFFSET_NAME = 0;
const uint8_t MEM_LEN_NAME = 48;
const uint8_t MEM_OFFSET_SERIAL = MEM_OFFSET_NAME + MEM_LEN_NAME;
const uint8_t MEM_LEN_SERIAL = 11;
const uint8_t MEM_OFFSET_CONFIG = 0;          // start reading with first character from Flash

const char    type[sizeof(MOBIFLIGHT_TYPE)] = MOBIFLIGHT_TYPE;
char          serial[MEM_LEN_SERIAL] = MOBIFLIGHT_SERIAL;
char          name[MEM_LEN_NAME] = MOBIFLIGHT_NAME;
const int     MEM_LEN_CONFIG = MEMLEN_CONFIG;
char          nameBuffer[MEM_LEN_CONFIG] = "";
boolean       configActivated = false;

void readConfig();

// ************************************************************
// configBuffer handling
// ************************************************************

void loadConfig()
{
#ifdef DEBUG2CMDMESSENGER
  cmdMessenger.sendCmd(kStatus, F("Load config"));
#endif
  readConfig();
  configActivated = true;
}

void OnSetConfig()
{
  cmdMessenger.sendCmd(kStatus, -1);
}

void OnResetConfig()
{
  cmdMessenger.sendCmd(kStatus, F("OK"));
}

void OnSaveConfig()
{
  cmdMessenger.sendCmd(kConfigSaved, F("OK"));
}

void OnActivateConfig()
{
  cmdMessenger.sendCmd(kConfigActivated, F("OK"));
}

// reads an ascii value which is '.' terminated from Flash and returns it's value
uint8_t readUintFromFlash (volatile uint16_t *addreeprom)
{
  char params[4] = {0};                                           // max 3 (255) digits NULL terminated
  uint8_t counter = 0;
  do
  {
    params[counter++] = pgm_read_byte_near(configuration + (*addreeprom)++);      // read character from eeprom and locate next buffer and eeprom location
    if (*addreeprom > sizeof(configuration))                      // abort if EEPROM size will be exceeded
      return 0;
  }
  while ( params[counter-1] != '.' && counter < sizeof(configuration));  // reads until limiter '.' and for safety reason not more then size of params[]
  params[counter-1] = 0x00;                                       // replace '.' by NULL to terminate the string
  return atoi(params);
}

// reads a string from Flash at given address which is ':' terminated and saves it in the nameBuffer
// once the nameBuffer is not needed anymore, just read until the ":" termination -> see function below
bool readNameFromFlash(uint16_t *addreeprom, char* buffer, uint16_t *addrbuffer)
{
  char temp = 0;
  do
  {
    temp = pgm_read_byte_near(configuration + (*addreeprom)++);   // read the first character
    if (*addreeprom > sizeof(configuration))                      // abort if EEPROM size will be exceeded
      return false;
    buffer[(*addrbuffer)++] = temp;                               // save character and locate next buffer position
    if (*addrbuffer >= MEMLEN_CONFIG_BUFFER) {                    // nameBuffer will be exceeded
      return false;                                               // abort copying from EEPROM to nameBuffer
    }
  } while (temp != ':');                                          // reads until limiter ':' and locates the next free buffer position
  buffer[(*addrbuffer)-1] = 0x00;                                 // replace ':' by NULL, terminates the string
  return true;
}

// reads the Flash until end of command which ':' terminated
bool readEndCommandFromFlash(uint16_t *addreeprom)
{
  char temp = 0;
  do
  {
    temp = pgm_read_byte_near(configuration + (*addreeprom)++);
    if (*addreeprom > sizeof(configuration))                      // abort if EEPROM size will be exceeded
      return false;
  }
  while (temp != ':');                                            // reads until limiter ':'
  return true;
}


void readConfig()
{
  uint16_t addreeprom = MEM_OFFSET_CONFIG;                        // define first memory location where config is saved in EEPROM
  uint16_t addrbuffer = 0;                                        // and start with first memory location from nameBuffer
  char params[6] = "";
  char command = readUintFromFlash(&addreeprom);                  // read the first value from EEPROM, it's a device definition
  bool copy_success = true;                                       // will be set to false if copying input names to nameBuffer exceeds array dimensions
                                                                  // not required anymore when pins instead of names are transferred to the UI

  if (command == 0)                                               // just to be sure
    return;

  do                                                              // go through the EEPROM until it is NULL terminated
  {
    switch (command)
    {
    case kTypeButton:
      params[0] = readUintFromFlash(&addreeprom);                 // get the Pin number
      Button::Add(params[0], &nameBuffer[addrbuffer]);            // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
      copy_success = readNameFromFlash(&addreeprom, nameBuffer, &addrbuffer); // copy the NULL terminated name to nameBuffer and set to next free memory location
      break;

    case kTypeOutput:
      params[0] = readUintFromFlash(&addreeprom);                 // get the Pin number
      Output::Add(params[0]);
      copy_success = readEndCommandFromFlash(&addreeprom);        // check EEPROM until end of name
      break;

    case kTypeServo:                                              // used for setting the setpoint
      SetpointStepper::Add();
      copy_success = readEndCommandFromFlash(&addreeprom);        // check EEPROM until end of name
      break;

    case kTypeStepper:
      params[0] = readUintFromFlash(&addreeprom);                 // get the Pin1 number
      params[1] = readUintFromFlash(&addreeprom);                 // get the Pin2 number
      params[2] = readUintFromFlash(&addreeprom);                 // get the Pin3 number
      params[3] = readUintFromFlash(&addreeprom);                 // get the Pin4 number
      params[4] = readUintFromFlash(&addreeprom);                 // get the Button number
      Stepper::Add(params[0], params[1], params[2], params[3], params[4]);
      copy_success = readEndCommandFromFlash(&addreeprom);        // check EEPROM until end of name
      break;

    case kTypeAnalogInput:
      params[0] = readUintFromFlash(&addreeprom);                 // get the pin number
      params[1] = readUintFromFlash(&addreeprom);                 // get the sensitivity
      Analog::Add(params[0], &nameBuffer[addrbuffer], params[1]); // MUST be before readNameFromEEPROM because readNameFromEEPROM returns the pointer for the NEXT Name
      copy_success = readNameFromFlash(&addreeprom, nameBuffer, &addrbuffer);  // copy the NULL terminated name to to nameBuffer and set to next free memory location
      break;

    default:
      copy_success = readEndCommandFromFlash(&addreeprom);       // check EEPROM until end of name
    }
    command = readUintFromFlash(&addreeprom);
  } while (command && copy_success);
  if (!copy_success) {                                            // too much/long names for input devices
    nameBuffer[MEMLEN_CONFIG_BUFFER-1] = 0x00;                    // terminate the last copied (part of) string with 0x00
    cmdMessenger.sendCmd(kStatus, F("Failure on reading config"));
  }
}

void OnGetConfig()
{
  setLastCommandMillis();
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg("");
  for (uint16_t i = 0; i < sizeof(configuration) - 1; i++)        // Do not consider the terminating NULL from string -> '-1'
  {
    cmdMessenger.sendArg((char)pgm_read_byte_near(configuration + i));
  }
  cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
  setLastCommandMillis();
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(type);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(serial);
  cmdMessenger.sendCmdArg(VERSION);
  cmdMessenger.sendCmdEnd();
}

bool getStatusConfig()
{
  return configActivated;
}

// ************************************************************
// serial number handling
// ************************************************************
void generateSerial(bool force)
{
  MFeeprom.read_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
  if (!force && serial[0] == 'S' && serial[1] == 'N')
    return;
  randomSeed(analogRead(RANDOM_SEED_INPUT));
  sprintf(serial, "SN-%03x-", (unsigned int)random(4095));
  sprintf(&serial[7], "%03x", (unsigned int)random(4095));
  MFeeprom.write_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
}

void OnGenNewSerial()
{
  generateSerial(true);
  cmdMessenger.sendCmd(kInfo,serial);
}

// ************************************************************
// Naming handling
// ************************************************************

void _restoreName()
{
  return;
}

void OnSetName()
{
  return;
}

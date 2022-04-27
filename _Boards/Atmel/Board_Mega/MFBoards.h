#ifndef MFBoardMega_h
#define MFBoardMega_h

#define MF_SEGMENT_SUPPORT 1
#define MF_STEPPER_SUPPORT 1
#define MF_ANALOG_SUPPORT 1

//#define MODULE_MAX_PINS 69
#define MAX_OUTPUTS 40
#define MAX_BUTTONS 68
#define MAX_STEPPER_SETPOINT 4
#define MAX_MOTAXIS 2
#define MAX_STEPPERS 10
#define MAX_ANALOG_INPUTS 16

//#define STEPS 64
#define STEPPER_SPEED 400 // 300 already worked, 467, too?
#define STEPPER_ACCEL 800

#define MOBIFLIGHT_TYPE         "MotTrimThrot Mega"
#define MOBIFLIGHT_SERIAL       "1234567890"
#define MOBIFLIGHT_NAME         "MobiFlight Mega"
#define EEPROM_SIZE             4096    // EEPROMSizeMega
#define MEMLEN_CONFIG           1496    // max. size for config which wil be stored in EEPROM
#define MEMLEN_CONFIG_BUFFER    1000    // max. size for configBuffer, contains only names from inputs
#define MF_MAX_DEVICEMEM        1500    // max. memory size for devices

#define RANDOM_SEED_INPUT A0

#endif

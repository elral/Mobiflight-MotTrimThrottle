#ifndef MFBoardUno_h
#define MFBoardUno_h

#define MF_SEGMENT_SUPPORT 1
#define MF_STEPPER_SUPPORT 1
#define MF_ANALOG_SUPPORT 1

//#define MODULE_MAX_PINS 13
#define MAX_OUTPUTS 8
#define MAX_BUTTONS 8
#define MAX_STEPPER_SETPOINT 2
#define MAX_MOTAXIS 2
#define MAX_STEPPERS 2
#define MAX_ANALOG_INPUTS 3

#define STEPS 64
#define STEPPER_SPEED 400 // 300 already worked, 467, too?
#define STEPPER_ACCEL 800

#define MOBIFLIGHT_TYPE         "MotAxis Uno"
#define MOBIFLIGHT_SERIAL       "0987654321"
#define MOBIFLIGHT_NAME         "MobiFlight Uno"
#define EEPROM_SIZE             1024    // EEPROMSizeUno
#define MEMLEN_CONFIG           286     // max. size for config which wil be stored in EEPROM
#define MEMLEN_CONFIG_BUFFER    220     // max. size for configBuffer, contains only names from inputs
#define MF_MAX_DEVICEMEM        300     // max. memory size for devices

#define RANDOM_SEED_INPUT A0

#endif

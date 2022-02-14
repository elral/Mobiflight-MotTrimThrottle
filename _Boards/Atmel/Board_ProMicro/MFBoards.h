#ifndef MFBoardMicro_h
#define MFBoardMicro_h

#define MF_SEGMENT_SUPPORT 1
#define MF_STEPPER_SUPPORT 1
#define MF_ANALOG_SUPPORT 1

//#define MODULE_MAX_PINS 21
#define MAX_OUTPUTS 18
#define MAX_BUTTONS 18
#define MAX_STEPPER_SETPOINT 1
#define MAX_STEPPERS 3
#define MAX_ANALOG_INPUTS 5

#define STEPS 64
#define STEPPER_SPEED 400 // 300 already worked, 467, too?
#define STEPPER_ACCEL 800

#define MOBIFLIGHT_TYPE         "MotTrimThrot Micro"
#define MOBIFLIGHT_SERIAL       "0987654321"
#define MOBIFLIGHT_NAME         "MobiFlight Micro"
#define EEPROM_SIZE             1024    // EEPROMSizeMicro
#define MEMLEN_CONFIG           440     // max. size for config which wil be stored in EEPROM
#define MEMLEN_CONFIG_BUFFER    350     // max. size for configBuffer, contains only names from inputs
#define MF_MAX_DEVICEMEM        400     // max. memory size for devices

#define RANDOM_SEED_INPUT A0

#endif
